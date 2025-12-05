/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Receiver implementation file

==========================================================================================================================
*/
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

#define DECOMPRESS_MAX_SIZE 65536

//variables
char msgIn[BUFSIZE];
BOOL listening = TRUE;
DWORD bytesRead = 0;
DWORD totalRead = 0;
int choice = 0;

//receives the message and header, then plays the string or audio
void receiveMessage(HANDLE* hComRx) {
	Header rxHeader;
	void* rxPayload = NULL;
	DWORD bytesRead;

	system("cls");
	printf("\nReceiving...\n");

	bytesRead = receive(&rxHeader, &rxPayload, hComRx);
	if (bytesRead == 0 || rxPayload == NULL) {
		printf("\nError: No data received.\n");
		return;
	}

	// Display header info
	printf("\n===== RECEIVED MESSAGE HEADER =====\n");
	printf("SID\t\t%d\n", rxHeader.sid);
	printf("RID\t\t%d\n", rxHeader.rid);
	printf("Priority\t%d\n", rxHeader.priority);
	printf("Size\t\t%d\n", rxHeader.payloadSize);
	printf("PayloadType\t%s\n", (rxHeader.payLoadType == AUDIO) ? "AUDIO" : "TEXT");
	printf("Encryption\t%s\n",
		(rxHeader.encryption == XOR) ? "XOR" :
		(rxHeader.encryption == VIGENERE) ? "VIGENERE" : "OFF");
	printf("Compression\t%s\n",
		(rxHeader.compression == RLE) ? "RLE" :
		(rxHeader.compression == HUFFMAN) ? "HUFFMAN" : "OFF");
	printf("===================================\n");

	struct tm now = getTimeStruct();

	// Error detection if enabled
	if (cfg.ERR_DTCT == ON) {
		bool ok = validateReceivedPayload((char*)rxPayload, rxHeader.payloadSize);
		if (!ok) {
			printf("Payload rejected: uncorrectable error.\n");
			free(rxPayload);
			enterToContinue();
			return;
		}
	}

	// TEXT payload
	if (rxHeader.payLoadType == PAYLOAD_TEXT) {
		unsigned char* textBuffer = (unsigned char*)rxPayload;
		textBuffer[rxHeader.payloadSize - 1] = '\0';

		printf("\nReceived Text:\n(%02d:%02d:%02d) %s\n",
			now.tm_hour, now.tm_min, now.tm_sec, textBuffer);

		// Decompression prompt for RLE or Huffman text
		if (rxHeader.compression == RLE || rxHeader.compression == HUFFMAN) {
			char choice;
			printf("\nMessage is %s-compressed. Decompress message? (y/n): ",
				(rxHeader.compression == RLE) ? "RLE" : "Huffman");
			scanf_s(" %c", &choice);
			while (getchar() != '\n'); // flush input buffer

			if (choice == 'y' || choice == 'Y') {
				unsigned char* decompressed =
					(unsigned char*)malloc(DECOMPRESS_MAX_SIZE);

				if (!decompressed) {
					printf("Memory error: cannot decompress.\n");
				}
				else {
					if (rxHeader.compression == RLE) {
						RLE_Uncompress(textBuffer, decompressed, rxHeader.payloadSize);
					}
					else if (rxHeader.compression == HUFFMAN) {
						Huffman_Uncompress(textBuffer, decompressed,
							rxHeader.payloadSize,
							DECOMPRESS_MAX_SIZE);
					}

					printf("\nDecompressed Text:\n(%02d:%02d:%02d) %s\n",
						now.tm_hour, now.tm_min, now.tm_sec, decompressed);

					free(rxPayload);
					rxPayload = decompressed;
				}
			}
		}

		// Enqueue text for later processing
		char label[MAX_FILENAME];
		snprintf(label, MAX_FILENAME, "Text Msg %d", rxHeader.priority);
		enqueueTextAndHdr((char*)rxPayload, label, &rxHeader);
	}

	// AUDIO payload
	else if (rxHeader.payLoadType == PAYLOAD_AUDIO) {
		unsigned char* audioBufferBytes = (unsigned char*)rxPayload;
		long numSamples = rxHeader.payloadSize / sizeof(short);
		short* audioBuffer = (short*)audioBufferBytes;

		// Automatic decompression for RLE or Huffman audio
		if (rxHeader.compression == RLE || rxHeader.compression == HUFFMAN) {
			short* decompressed = (short*)malloc(lBigBufSize * sizeof(short)); // or estimated size
			if (!decompressed) {
				printf("Memory error: cannot decompress audio.\n");
			}
			else {
				if (rxHeader.compression == RLE) {
					decompressAudioRLE(audioBufferBytes, rxHeader.payloadSize,
						(unsigned char*)decompressed,
						lBigBufSize * sizeof(short));
				}
				else if (rxHeader.compression == HUFFMAN) {
					Huffman_Uncompress(audioBufferBytes, (unsigned char*)decompressed,
						rxHeader.payloadSize,
						lBigBufSize * sizeof(short));
				}

				audioBuffer = decompressed;
				numSamples = lBigBufSize; // update sample count
				free(rxPayload);
				rxPayload = decompressed;
			}
		}

		if (!InitializePlayback()) {
			printf("Playback initialization failed.\n");
			free(rxPayload);
			return;
		}

		printf("Playing received audio (%ld samples)...\n", numSamples);
		PlayBuffer(audioBuffer, numSamples);
		ClosePlayback();

		char label[MAX_FILENAME];
		snprintf(label, MAX_FILENAME, "Audio Msg %d", rxHeader.priority);
		enqueueAudioAndHdr(audioBuffer, numSamples, label, &rxHeader);
	}
	else {
		printf("Unknown payload type!\n");
	}

	free(rxPayload);
	enterToContinue();
}

//continuously receives and displays new text messages until user decides to stop
void rxInstantText(HANDLE* hComRx){
	system("cls");
	printf("\nListening for incoming text messages...\n");
	printf("Press 'q' to stop listening\n\n");

	listening = TRUE;
	while (listening) { //check if user wants to stop listening
		if (_kbhit()) { //scan for the letter q, if pressed flag user and return to Rx menu
			char ch = _getch();
			if (ch == 'q' || ch == 'Q') {
				listening = FALSE;
				printf("\nStopped listening\n");
				clearScreen();
				break;
			}
		}
		struct tm now = getTimeStruct(); //get the current time
		//attempt to read from com port
		bytesRead = inputFromPort(hComRx, msgIn, BUFSIZE - 1);
		if (bytesRead > 0) {
			msgIn[bytesRead] = '\0';
			printf("(%02d:%02d:%02d) %s\n", now.tm_hour, now.tm_min, now.tm_sec, msgIn);
		}
		Sleep(100);
	}
}

//validate the checksum
bool validateReceivedPayload(char* data, int length) {
	unsigned char receivedChecksum = (unsigned char)data[length - 1];
	unsigned char computed = computeChecksum(data, length - 1);

	if (computed != receivedChecksum) {
		printf("!!! ERROR: Checksum mismatch !!!\n");
		printf("Expected: %u | Got: %u\n", receivedChecksum, computed);
		return false;
	}
	printf("Checksum OK! No transmission errors.\n");
	return true;
}

//attempt to correct the payload
bool correctPayload(char* data, int length) {
	unsigned char receivedChecksum = (unsigned char)data[length - 1];
	unsigned char computed = computeChecksum(data, length - 1);

	if (computed == receivedChecksum) {
		printf("Checksum OK! No transmission errors.\n");
		return true;
	}

	printf("Checksum mismatch! Attempting correction...\n");

	// SINGLE BIT flip correction
	for (int i = 0; i < length - 1; i++) {
		for (int bit = 0; bit < 8; bit++) {

			data[i] ^= (1 << bit);  // flip bit

			unsigned char recompute = computeChecksum(data, length - 1);

			if (recompute == receivedChecksum) {
				printf("Single-bit error corrected at byte %d, bit %d\n", i, bit);
				return true;
			}

			data[i] ^= (1 << bit);  // restore original if not correct
		}
	}

	printf("❌ Could not correct payload\n");
	return false;
}

//queued messages loop
void queueLoop() {
	system("cls");

	int inQueue = TRUE;
	while (inQueue) {
		queueMenu();
		int choice = getInput();

		switch (choice) {
		case SHOW_FULL:
			displayQueue();
			enterToContinue();
			break;

		case SHOW_FIRST:
			viewMessages();
			break;

		case DISCARD_FIRST:
			deleteFrontMessage();
			break;

		case Q_BACK:
			goBack();
			inQueue = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}

/*
//receive header and text message
void receiveTextMessage(HANDLE* hComRx) {
	Header rxHeader;
	void* rxPayload = NULL;
	DWORD bytesRead;

	// Receive header + payload using RS232Comm.cpp function
	bytesRead = receive(&rxHeader, &rxPayload, hComRx);

	if (bytesRead == 0 || rxPayload == NULL) {
		printf("\nError: No data received.\n");
		return;
	}

	// Ensure payload is null-terminated for safe printing
	char* textMessage = (char*)rxPayload;
	textMessage[rxHeader.payloadSize - 1] = '\0';

	// Print header info
	printf("\n===== RECEIVED MESSAGE HEADER =====\n");
	printf("SID\t\t%d\n", rxHeader.sid);
	printf("RID\t\t%d\n", rxHeader.rid);
	printf("Priority\t%d\n", rxHeader.priority);
	printf("Size\t\t%d\n", rxHeader.payloadSize);
	printf("Msg Type\t%d\n", rxHeader.payLoadType);
	printf("Encryption\t%d\n", rxHeader.encryption);
	printf("Compression\t%d\n", rxHeader.compression);
	printf("===================================\n");

	// Print message content
	struct tm now = getTimeStruct();
	printf("(%02d:%02d:%02d) %s\n", now.tm_hour, now.tm_min, now.tm_sec, textMessage);

	//enqueue the text message and header
	char label[MAX_FILENAME];
	snprintf(label, MAX_FILENAME, "Text Msg %d", rxHeader.priority);
	enqueueTextAndHdr(textMessage, label, &rxHeader);

	// Free the allocated payload buffer
	free(rxPayload);

	// Prompt user to continue
	printf("\nPress Enter to continue...");
	while (getchar() != '\n'); // wait for Enter
	clearScreen();
}
*/

/*
void receiveAudioAndPlay(HANDLE* hComRx) {
	Header rxHeader;
	void* rxPayload = NULL;   // this will be allocated inside receive()
	DWORD bytesRead;

	printf("Waiting for audio transmission...\n");

	// Receive header + payload
	bytesRead = receive(&rxHeader, &rxPayload, hComRx);
	if (bytesRead == 0 || rxPayload == NULL) {
		printf("Error: no data received.\n");
		return;
	}

	printf("Received payload: %ld bytes (type '%c')\n", rxHeader.payloadSize, rxHeader.payLoadType);

	// --- CAST BUFFER BACK TO AUDIO FORMAT ---
	short* audioBuffer = (short*)rxPayload;  // convert from void* to short*
	long numSamples = rxHeader.payloadSize / sizeof(short);

	// Play the received audio
	if (!InitializePlayback()) {
		printf("Playback initialization failed.\n");
		free(rxPayload);
		return;
	}

	printf("Playing received audio (%ld samples)...\n", numSamples);
	PlayBuffer(audioBuffer, numSamples);
	ClosePlayback();

	// Free the malloc'd buffer
	free(rxPayload);

	printf("Audio playback complete.\n");
}
*/

//main transmitter branch loop
void receiverLoop() {
	int receiving = TRUE;

	// Setup COM port each time the loop starts
	wchar_t rxPortName[10];
	swprintf(rxPortName, 10, L"COM%d", cfg.COM_RX);
	HANDLE hComRx = setupComPort(rxPortName, nComRate, nComBits, timeout);

	// Receiver menu loop
	while (receiving) {
		receivingMenu();
		int choice = getInput();

		switch (choice) {
		case Rx_INSTANT:
			rxInstantText(&hComRx);
			break;

		case LISTENING:
			receiveMessage(&hComRx);
			break;

		case MANAGE_QUEUE:
			queueLoop();
			break;

		case Rx_GO_BACK:
			goBack();
			receiving = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
	// Teardown COM port after user exits menu
	purgePort(&hComRx);
	CloseHandle(hComRx);
	hComRx = INVALID_HANDLE_VALUE;
}