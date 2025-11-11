/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				XOR Encrpytion implementation file

==========================================================================================================================
*/
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "sound.h"
#include "audioQueue.h"
#include "config.h"
#include "compress.h"
#include "encrypt.h"

//**********************************************************************************    XOR ENCRYPT / DECRPYT
//xor encrpyt
void xorEncrypt(const char* input, char* output, char key) {
	int i = 0;
	while (input[i] != '\0') {
		output[i] = input[i] ^ key;
		i++;
	}
	output[i] = '\0';
}

//xor decrpyt
void xorDecrypt(const char* input, char* output, char key) {
	int i = 0;
	while (input[i] != '\0') {
		output[i] = input[i] ^ key; // same as encode
		i++;
	}
	output[i] = '\0';
}

//strip enter
static void strip_newline(char* s) {
	size_t n = strlen(s);
	if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

// test function
void testXorEncryption(HANDLE* hComTx) {
	char input[256];
	char encoded[256];
	char decoded[256];
	char choice[8];
	char key;

	printf("Enter a character key for XOR encryption: ");
	scanf_s(" %c", &key);
	getchar(); // consume leftover newline

	printf("\nEnter text to encrypt: ");
	if (!fgets(input, sizeof(input), stdin)) {
		printf("Input error.\n");
		return;
	}
	strip_newline(input);

	xorEncrypt(input, encoded, key);

	printf("\nEncrypted text (raw XOR bytes):\n");
	for (size_t i = 0; i < strlen(encoded); i++) {
		printf("%02X ", (unsigned char)encoded[i]);
	}
	printf("\n");

	// send to COM port
	outputToPort(hComTx, encoded, strlen(encoded));

	printf("\nWould you like to decrypt and display it? (y/n): ");
	if (!fgets(choice, sizeof(choice), stdin)) return;

	if (tolower(choice[0]) == 'y') {
		xorDecrypt(encoded, decoded, key);
		printf("\nDecrypted text:\n%s\n", decoded);
	}
	else {
		printf("Done. Encrypted data sent to COM port.\n");
	}
	Sleep(2000);
	system("cls");
}

//**********************************************************************************    RLE ENCRYPT / DECRPYT
/*************************************************************************
* New byte-oriented RLE functions with overflow protection
*************************************************************************/

int RLE_Encode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc) {
    if (inlen == 0) return 0;
    if (outlen == 0) return -1;

    unsigned int inpos = 0, outpos = 0;

    while (inpos < inlen) {
        unsigned char current = in[inpos];
        unsigned int count = 1;

        // Count consecutive identical bytes
        while (inpos + count < inlen && in[inpos + count] == current && count < 255) {
            count++;
        }

        // Check if we should use RLE encoding
        if (count > 3 || current == esc) {
            // Use RLE encoding - need 3 bytes in output
            if (outpos + 3 > outlen) {
                return -1; // Output buffer overflow
            }

            out[outpos++] = esc;
            out[outpos++] = (unsigned char)count;
            out[outpos++] = current;
            inpos += count;
        }
        else {
            // Copy literal bytes
            for (unsigned int i = 0; i < count; i++) {
                if (outpos >= outlen) {
                    return -1; // Output buffer overflow
                }
                out[outpos++] = current;
            }
            inpos += count;
        }
    }

    return (int)outpos;
}

int RLE_Decode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc) {
    if (inlen == 0) return 0;
    if (outlen == 0) return -1;

    unsigned int inpos = 0, outpos = 0;

    while (inpos < inlen && outpos < outlen) {
        unsigned char current = in[inpos++];

        if (current == esc && inpos < inlen) {
            // Potential RLE encoded sequence
            unsigned int count = in[inpos++];

            if (inpos < inlen) {
                unsigned char value = in[inpos++];

                // Check for output buffer overflow
                if (outpos + count > outlen) {
                    return -1; // Output buffer overflow
                }

                // Write the repeated bytes
                for (unsigned int i = 0; i < count; i++) {
                    out[outpos++] = value;
                }
            }
            else {
                // Malformed input - ESC at end without complete sequence
                if (outpos < outlen) {
                    out[outpos++] = current;
                }
                if (inpos - 1 < inlen && outpos < outlen) {
                    out[outpos++] = (unsigned char)count;
                }
            }
        }
        else {
            // Literal byte
            if (outpos < outlen) {
                out[outpos++] = current;
            }
            else {
                return -1; // Output buffer overflow
            }
        }
    }

    return (int)outpos;
}