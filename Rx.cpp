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

#define OLD 0
#define NEW 1

//variables
char msgIn[BUFSIZE];
BOOL listening = TRUE;
DWORD bytesRead = 0;
DWORD totalRead = 0;
int choice = 0;

//read exact number of bytes from the com port
int readBytes(HANDLE* hCom, void* buffer, DWORD bytesToRead){
	while (totalRead < bytesToRead) {
		if (!ReadFile(*hCom, (char*)buffer + totalRead, bytesToRead - totalRead, &bytesRead, NULL)){
			return 0; //failure
		}
		if (bytesRead == 0) {
			Sleep(1);
			continue;
		}
		totalRead += bytesRead;
	}
	return 1;
}

//read an incoming header
int readHeader(HANDLE* hCom, Header* hdr){
	DWORD headerSize = sizeof(Header);

	if (!readBytes(hCom, hdr, headerSize)) {
		return 0; //failed to read complete header
	}
	return 1; //success
}

//read payload into the buffer
int readPayload(HANDLE* hCom, void* buffer, long payloadSize){
	if (payloadSize <= 0) {
		printf("Invalid payload size\n");
		return 0;
	}
	return readBytes(hCom, buffer, (DWORD)payloadSize);
}

//**********************************************************************************    RECEIVING TEXTS
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

//**********************************************************************************    RECEIVING AUDIO
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

//main transmitter branch loop
void receiverLoop(HANDLE* hComRx){
	int running = TRUE;
	while (running) {
		receivingMenu();
		int Rx_choice = getInput();

		switch (Rx_choice) {
		case PLAY_TEXT:
			rxInstantText(hComRx); //receive and play text messages
			break;

		case PLAY_AUDIO:
			receiveAudioAndPlay(hComRx);
			break;

		case SHOW_QUEUE:
			choice = 0;

			while (1) {

				printf("\n1 - Receive and Queue 3 Random Quotes\n");
				printf("2 - View Current Queue Entries\n");
				printf("0 - Exit\n");
				printf("Enter option: ");
				scanf_s("%d", &choice);

				if (choice == 0) break;

				switch (choice) {

				case 1: // Receive quotes and queue
				{
					for (int i = 0; i < 3; i++) {

						char rxMsg[256] = { 0 };

						BOOL ok = inputFromPort(hComRx, rxMsg, sizeof(rxMsg) - 1);

						if (!ok) {
							printf("\nRX Error reading quote %d!\n", i + 1);
							break;
						}

						printf("\nRX <- Quote %d: %s\n", i + 1, rxMsg);

						char label[32];
						snprintf(label, sizeof(label), "Quote_%d", i + 1);

						enqueueText(rxMsg, label);
					}
					break;
				}

				case 2: // Print queue contents
				{
					link node = peekQueue();
					if (!node) {
						printf("Queue is empty.\n");
						break;
					}

					printf("\n--- QUEUE CONTENTS ---\n");
					int index = 1;
					while (node) {
						// Print the actual string stored in the queue
						printf("\n%d: \"%s\" (label: %s, size: %ld)\n",
							index++,
							node->Data.text,      // <-- assuming 'text' holds the queued string
							node->Data.filename,  // optional label
							node->Data.size);     // optional size
						node = node->pNext;
					}
					break;
				}

				default:
					printf("Invalid selection.\n");
					break;
				}
			}
			break;

		case Rx_GO_BACK:
			goBack();
			running = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}

void receiverLoopNew(HANDLE* hComRx) {
	printf("\nReceiver started. Listening for incoming messages...\n");

	struct tm now = getTimeStruct(); //get the current time

	while (listening) {
		//hComRx = setupComPort(rxPortName, nComRate, nComBits, timeout); //setup Rx port

		// Allocate space for a fresh header each cycle
		Header* rxHeader = (Header*)malloc(sizeof(Header));
		if (!rxHeader) {
			printf("ERROR: Failed to allocate header.\n");
			Sleep(1000);
			continue;
		}

		//attempt to read the header
		if (!readHeader(hComRx, rxHeader)) {
			free(rxHeader);
			Sleep(10); // Prevent CPU overuse
			continue;
		}

		//prevent printing garbage
		if (rxHeader->payloadSize <= 0 ||
			rxHeader->payloadSize > 4096 ||
			(rxHeader->payLoadType != 1 && rxHeader->payLoadType != 2))
		{
			// Invalid header, skip
			free(rxHeader);
			continue;
		}

		printf("\n===== INCOMING MESSAGE HEADER =====\n");
		printf("Sender ID:     %d\n", rxHeader->sid);
		printf("Receiver ID:   %d\n", rxHeader->rid);
		printf("Priority:      %d\n", rxHeader->priority);
		printf("Payload Size:  %ld bytes\n", rxHeader->payloadSize);
		printf("Payload Type:  %d\n", rxHeader->payLoadType);
		printf("Encryption:    %d\n", rxHeader->encryption);
		printf("Compression:   %d\n", rxHeader->compression);
		printf("===================================\n");

		// Allocate buffer for payload
		char* payload = (char*)malloc(rxHeader->payloadSize + 1);
		if (!payload) {
			printf("ERROR: Memory allocation failed.\n");
			Sleep(5000);
			break;
		}

		bytesRead = inputFromPort(hComRx, payload, rxHeader->payloadSize);
		payload[bytesRead] = '\0';

		// Handle text or audio based on type
		if (rxHeader->payLoadType == 1) {
			printf("(%02d:%02d:%02d) %s\n", now.tm_hour, now.tm_min, now.tm_sec, payload);

			char choice;

			do {
				printf("\nDone reading? (y/n): ");
				choice = getchar();
				while (getchar() != '\n'); // clear input buffer
			} while (choice != 'y' && choice != 'Y');


			free(payload);
			free(rxHeader);
			clearScreen();


			CloseHandle(hComRx);
			purgePort(hComRx);


		}
		/*
		else if (rxHeader->payLoadType == 2) {
			printf("\nReceived Audio (%ld bytes) — Playing audio...\n", rxHeader->payloadSize);
			//playAudio(payload, rxHeader.payloadSize); // Customize based on your audio code
		}
		else {
			printf("Unknown payload type.\n");
		}
		*/

		printf("Listening for next message...\n");
	}
}