/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				Testing implementation file

==========================================================================================================================
*/
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "sound.h"
#include "audioQueue.h"
#include "config.h"
#include "compress.h"
#include "encrypt.h"
#include "settings.h"
#include "testing.h"
#include "fileIO.h"

#define NORMAL 1
#define CORRUPT 2

char currQuote[MAX_QUOTE_LENGTH]; // Buffer to write the message to
int newchoice = 0;

//demonstrates huffman compression / decompression
void huffmanDemo() {
    unsigned char input[1024];
    unsigned char compressed[2048];  // must be larger than input
    unsigned char decompressed[1024];
    int compressed_size;
    char choice;

    //get user input
    printf("Enter a string to compress:\n> ");
    fgets((char*)input, sizeof(input), stdin);

    //remove newline character if present
    size_t len = strlen((char*)input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("Empty input — nothing to compress.\n");
        return;
    }

    //compress using Huffman_Compress
    compressed_size = Huffman_Compress(input, compressed, len);
    printf("\nCompressed %d bytes -> %d bytes.\n", (int)len, compressed_size);

    //display compressed data in hex
    printf("Compressed data (hex):\n");
    for (int i = 0; i < compressed_size; ++i)
        printf("%02X ", compressed[i]);
    printf("\n");

    //ask user if they want to decompress
    printf("\nDecompress the data? (y/n): ");
    choice = getchar();
    if (choice != 'y' && choice != 'Y') {
        printf("Exiting without decompression.\n");
        return;
    }

    //decompress back to text
    Huffman_Uncompress(compressed, decompressed, compressed_size, len);
    decompressed[len] = '\0';  // null-terminate restored string

    //display result
    printf("\nDecompressed string:\n%s\n", decompressed);
}

//strip enter
void stripNewline(char* s) {
	size_t n = strlen(s);
	if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

// test function
void xorDemo() {
	char input[256];
	char encoded[256];
	char decoded[256];
	char choice[8];
	char key;

	printf("Enter a character key for XOR encryption: ");
	scanf(" %c", &key);
	getchar(); // consume leftover newline

	printf("\nEnter text to encrypt: ");
	if (!fgets(input, sizeof(input), stdin)) {
		printf("Input error.\n");
		return;
	}
	stripNewline(input);

	xorEncrypt(input, encoded, key);

	printf("\nEncrypted text (raw XOR bytes):\n");
	for (size_t i = 0; i < strlen(encoded); i++) {
		printf("%02X ", (unsigned char)encoded[i]);
	}
	printf("\n");

	printf("\nDecrpyt? (y/n): ");
    if (!fgets(choice, sizeof(choice), stdin)) {
        return;
    }

	if (tolower(choice[0]) == 'y') {
		xorDecrypt(encoded, decoded, key);
		printf("\nDecrypted text:\n%s\n", decoded);
	}
    Sleep(10000);
	clearScreen();
}

//error detection wrapper function
void testErrDetection(HANDLE* hComTx) {
    system("cls");
    printf("Send normal quote (1)\nSend corrupted quote (2)\n");
    int errorChoice = getInput();

    if (errorChoice == NORMAL) {
        sendNormalQuote(hComTx);
    }
    else if (errorChoice == CORRUPT) {
        sendCorruptedQuote(hComTx);
    }
    else {
        printf("Invalid choice.\n");
    }

    Sleep(3000);
    clearScreen();
}

//error testing
unsigned char computeChecksum(const char* data, int length) {
    unsigned int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += (unsigned char)data[i];
    }
    return (unsigned char)(sum & 0xFF); // modulo 256
}

//introduce intentional errors
void introduceBitErrors(char* data, int length, float errorRate) {
    // Only introduce an error based on probability
    if ((float)rand() / RAND_MAX < errorRate) {

        // Pick one random byte in the payload
        int byteIndex = rand() % length;

        // Pick one random bit to flip
        unsigned char bit = 1 << (rand() % 8);

        // Flip it
        data[byteIndex] ^= bit;

        printf("Injected 1-bit error at byte %d (bit mask 0x%02X)\n", byteIndex, bit);
    }
}

/*
void introduceBitErrors(char* data, int length, float errorRate) {
    for (int i = 0; i < length; i++) {
        if ((float)rand() / RAND_MAX < errorRate) {
            unsigned char bit = 1 << (rand() % 8);
            data[i] ^= bit;
        }
    }
}
*/

//send a corrupted quote with the checksum
void sendCorruptedQuote(HANDLE* hComTx) {
    // STEP 1: Get a random quote from fileIO
    getRandQuote(currQuote);
    char* quote = currQuote;
    if (!quote) {
        printf("Error: Could not load quote.\n");
        return;
    }

    int payloadSize = strlen(quote);
    int txSize = payloadSize + 1;    // +1 for checksum byte

    // STEP 2: Compute checksum BEFORE corruption
    unsigned char checksum = computeChecksum(quote, payloadSize);

    // Append checksum to end
    char* messageWithChecksum = (char*)malloc(payloadSize + 2);
    memcpy(messageWithChecksum, quote, payloadSize);
    messageWithChecksum[payloadSize] = checksum;
    messageWithChecksum[payloadSize + 1] = '\0';

    // STEP 3: Introduce bit errors INTO payload ONLY
    introduceBitErrors(messageWithChecksum, payloadSize);

    printf("\n>>> Sending QUOTE with Forced Errors\n");
    printf("Original Checksum: %u\n", checksum);

    // STEP 4: Transmit using your serial function
    Header txHeader = buildHeader(txSize, TEXT);
    transmit(&txHeader, (unsigned char*)messageWithChecksum, hComTx);

    free(messageWithChecksum);

    enterToContinue();
}

//send a normal quote with the checksum
void sendNormalQuote(HANDLE* hComTx) {
    // STEP 1: Load quote
    getRandQuote(currQuote);
    char* quote = currQuote;
    if (!quote) {
        printf("Error: Could not load quote.\n");
        return;
    }

    int payloadSize = strlen(quote);
    int txSize = payloadSize + 1;    // +1 for checksum byte

    // STEP 2: Compute checksum
    unsigned char checksum = computeChecksum(quote, payloadSize);

    // Append checksum
    char* messageWithChecksum = (char*)malloc(payloadSize + 2);
    memcpy(messageWithChecksum, quote, payloadSize);
    messageWithChecksum[payloadSize] = checksum;
    messageWithChecksum[payloadSize + 1] = '\0';

    printf("\n>>> Sending NORMAL QUOTE\n");
    printf("Checksum: %u\n", checksum);

    // STEP 3: Build header + transmit
    Header txHeader = buildHeader(txSize, TEXT);
    transmit(&txHeader, (unsigned char*)messageWithChecksum, hComTx);

    free(messageWithChecksum);

    enterToContinue();
}

//switch case for all testing functions
void testingLoop(HANDLE* hComTx) {
    system("cls");
    int inTesting = TRUE;
    while (inTesting) {

        testingMenu();
        int testOption = getInput();

        switch (testOption) {
        case LOOPBACK:
            getRandQuote(currQuote);
            break;

        case CONSTRUCT_HEADER:
            break;

        case SORTING_QUEUE:
            break;

        case ERROR_DETECT:
            testErrDetection(hComTx);
            break;

        case ENCRYPT_DECRPYT:
            xorDemo();
            break;

        case COMPRESS_DECOMPRESS:
            huffmanDemo();
            break;

        case BACK_TESTING:
            goBack();
            inTesting = FALSE;
            break;

        default:
            invalid();
            break;
        }
    }
}
