/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Transmitting implementation file

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
#include "phonebook.h"

char msgOut[BUFSIZE];
size_t len;
int receiverID;
int priority;
int msgType;
//menu vars
int instantText = TRUE;
int transmitting = TRUE;
int sendingNewText = TRUE;
int makingHeader = TRUE;
int audioMsg = TRUE;


//**********************************************************************************    SENDING TEXTS
//creates and sends new text messages
void instantTextMsg(HANDLE* hComTx){
	system("cls");
	printf("\nType your messages below...\n");
	printf("Enter 'q' to stop sending\n");

	instantText = TRUE;
	while (instantText) {
		//collect string from the user
		printf("\nEnter message: ");
		fgets(msgOut, sizeof(msgOut), stdin); 

		//trim newline if present
		len = strlen(msgOut);
		if (len > 0 && msgOut[len - 1] == '\n') { 
			msgOut[len - 1] = '\0';
			len--;
		}
		//check for quit command
		if (_stricmp(msgOut, "q") == 0) { //if user quits return to Tx menu
			instantText = FALSE;
			printf("\nStopped sending\n");
			clearScreen();
			break;
		}
		//send message
		outputToPort(hComTx, msgOut, strlen(msgOut) + 1);
		//printf("Message sent: %s\n", msgOut);
	}
}

//send text messages with a header
void sendTextWithHeader(HANDLE* hComTx) {

	printf("\nEnter message to send (q to quit):\n");
	fgets(msgOut, sizeof(msgOut), stdin);

	len = strlen(msgOut);
	if (len > 0 && msgOut[len - 1] == '\n') {
		msgOut[len - 1] = '\0';
		len--;
	}

	if (_stricmp(msgOut, "q") == 0)
		return;

	unsigned int inputSize = (unsigned int)(len + 1);
	unsigned char* inBytes = (unsigned char*)msgOut;
	unsigned char* payload = inBytes;
	unsigned int payloadSize = inputSize;

	// Check compression settings
	if (cfg.COMPRESS == RLE) {
		payload = new unsigned char[inputSize * 2 + 10]; // worst case buffer
		payloadSize = compressTextRLE(inBytes, inputSize, payload, inputSize * 2 + 10);
		printf("Text compression (RLE): %u -> %d bytes (%.1f%%)\n",
			inputSize, payloadSize, (100.0f * payloadSize) / inputSize);
	}
	else if (cfg.COMPRESS == HUFFMAN) {
		//add huffman text compression here
	}
	else {
		//compression is off, send as is
	}

	// Build header
	Header txHeader = buildHeader(payloadSize, TEXT);

	// Transmit header + payload
	transmit(&txHeader, payload, hComTx);
	printf("\nText message sent!\n");
	logHeaderToPhonebook(&txHeader);

	if (cfg.COMPRESS == RLE || cfg.COMPRESS == HUFFMAN) { //if compression is used, free the payload
		delete[] payload;
	}
	enterToContinue();
}

//**********************************************************************************    SENDING AUDIO
//record and send new audio message
void recordAndSendAudio(HANDLE* hComTx) {
	if (!InitializeRecording()) {
		printf("Error: Failed to initialize recording device.\n");
		return;
	}

	if (!RecordBuffer(iBigBuf, lBigBufSize)) {
		printf("Error: Recording failed.\n");
		CloseRecording();
		return;
	}

	printf("\nRecording complete. (%ld samples)\n", lBigBufSize);

	unsigned int inputSize = lBigBufSize * sizeof(short);
	unsigned char* inBytes = (unsigned char*)iBigBuf;
	unsigned char* payload = inBytes;
	unsigned int payloadSize = inputSize;

	// Check compression settings
	if (cfg.COMPRESS == RLE) {
		payload = new unsigned char[inputSize * 2 + 10]; // worst-case
		payloadSize = compressAudioRLE(inBytes, inputSize, payload, inputSize * 2 + 10);
		printf("Audio compression (RLE): %u -> %d bytes (%.1f%%)\n",
			inputSize, payloadSize, (100.0f * payloadSize) / inputSize);
	}
	else if (cfg.COMPRESS == HUFFMAN) {
		//add huffman audio compression here
	}
	else {
		//compression off, send as is
	}

	// Build header
	Header txHeader = buildHeader(payloadSize, AUDIO);

	printf("Sending audio clip...\n");
	transmit(&txHeader, payload, hComTx);
	logHeaderToPhonebook(&txHeader);

	if (cfg.COMPRESS == RLE || cfg.COMPRESS == HUFFMAN) {
		delete[] payload;
	}

	CloseRecording();
	printf("Audio transmission complete.\n");

	enterToContinue();
}

//**********************************************************************************    MAIN TRANSMITTER LOOPS
//create the outgoing header
void composeHeaderLoop() {
	system("cls");
	makingHeader = TRUE;

	while (makingHeader) {
		printf("Enter Receiver ID:\n> ");
		receiverID = getInput();
		cfg.RID = receiverID;
		printf("\nReceiver ID set to: %d\n\n", receiverID);

		printf("Enter Message Priority (1-5):\n> ");
		priority = getInput();

		if (priority < 1 || priority > 5) {
			printf("\nInvalid priority! Must be 1–5.\n");
			continue;
		}

		cfg.PRIORITY = priority;
		printf("\nPriority set to: %d\n\n", priority);

		printf("Message type:\n1: Audio\n2: Text\n> ");
		int choice = getInput();

		if (choice == TEXT) {
			cfg.MSGTYPE = TEXT;  // Text
			sendTextWithHeader(&hComTx);
		}
		else if (choice == AUDIO) {
			cfg.MSGTYPE = AUDIO;  // Audio
			recordAndSendAudio(&hComTx);
		}
		else {
			invalid();
			continue;
		}

		saveConfig(CONFIG_FILE, &cfg); // Save changes to disk
		printf("\nMessage configuration saved!\n");
		Sleep(700);

		break; // exit loop back to Tx menu
	}

	clearScreen();
}

//main transmitter branch loop
void transmitterLoop(HANDLE* hComTx){
	transmitting = TRUE;
	while (transmitting) {
		transmittingMenu(); //Tx print menu
		int Tx_choice = getInput();

		switch (Tx_choice) {
		case INSTANT_TEXT:
			instantTextMsg(hComTx);
			break;

		case COMPOSE_HEADER:
			composeHeaderLoop();
			break;

		case Tx_GO_BACK:
			goBack();
			transmitting = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}