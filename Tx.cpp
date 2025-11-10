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


//variables
char msgOut[BUFSIZE];
size_t len;
int transmitting = TRUE;
int sendingNewText = TRUE;
int advancedMenu = TRUE;
int audioMsg = TRUE;

//**********************************************************************************    SENDING TEXTS
//creates and sends new text messages
void newText(HANDLE* hComTx){
	system("cls");
	printf("\nType your messages below...\n");
	printf("Enter 'q' to stop sending\n");

	transmitting = TRUE;
	while (transmitting) {
		//collect string from the user
		printf("\nEnter message:\n");
		fgets(msgOut, sizeof(msgOut), stdin); 

		//trim newline if present
		len = strlen(msgOut);
		if (len > 0 && msgOut[len - 1] == '\n') { 
			msgOut[len - 1] = '\0';
			len--;
		}
		//check for quit command
		if (_stricmp(msgOut, "q") == 0) { //if user quits return to Tx menu
			transmitting = FALSE;
			printf("\nStopped sending\n");
			clearScreen();
			break;
		}
		//send message
		outputToPort(hComTx, msgOut, strlen(msgOut) + 1);
		//printf("Message sent: %s\n", msgOut);
	}
}

//ask if user wishes to send instant or advanced text message
void selectTextType(HANDLE* hComTx) {
	system("cls");
	sendingNewText = TRUE;

	while (sendingNewText) {
		newTextTypeMenu();
		int textType = getInput();

		switch (textType) {
		case INSTANT:
			newText(hComTx); //create and send new text messages
			break;

		case ADVANCED:
			newTextAdvanced();
			break;

		case Tx_SUB_GO_BACK:
			goBack();
			sendingNewText = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}

//TEXT: if user wishes to compress, encrypt or send message
void newTextAdvanced(){
	system("cls");

	advancedMenu = TRUE;
	while (advancedMenu) {
		newTextAdvancedMenu();
		int textAdvanced = getInput();

		switch (textAdvanced) {
		case COMPRESS_TEXT:
			printf("compressing...\n");
			break;

		case ENCRYPT_TEXT:
			printf("encrypting...\n");
			break;

		case ADD_INFO_TEXT:
			printf("adding info...\n");
			break;

		case DELETE_TEXT:
			printf("deleting...\n");
			break;

		case SEND_TEXT:
			printf("sending...\n");
			break;

		case GO_BACK_TEXT:
			goBack();
			advancedMenu = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}

//**********************************************************************************    SENDING AUDIO
//record and send new audio message
void recordAndSendAudio(HANDLE* hComTx) {
	Header txHeader;

	//initialize recording
	if (!InitializeRecording()) {
		printf("Error: Failed to initialize recording device.\n");
		return;
	}
	//record into buffer
	if (!RecordBuffer(iBigBuf, lBigBufSize)) {
		printf("Error: Recording failed.\n");
		CloseRecording();
		return;
	}
	printf("\n\nRecording complete. (%ld samples)\n", lBigBufSize);

	//prepare header
	txHeader.payLoadType = 'A';  // 'A' for audio
	txHeader.payloadSize = lBigBufSize * sizeof(short);

	//send header and payload
	printf("Sending audio clip (%ld bytes)...\n", txHeader.payloadSize);
	transmit(&txHeader, (void*)iBigBuf, hComTx);

	//clean up
	CloseRecording();
	printf("Audio transmission complete.\n");
}

//AUDIO: if user wishes to compress, encrypt, delete or send message
void newAudioOptions() {
	audioMsg = TRUE;
	while (audioMsg) {
		newAudioSubMenu();
		int msgOption = getInput();

		switch (msgOption) {
		case COMPRESS_AUDIO:
			printf("compressing...\n");
			break;
		case ENCRYPT_AUDIO:
			printf("encrpyting...\n");
			break;
		case ADD_INFO_AUDIO:
			printf("adding information regarding audio message...\n");
			break;
		case Tx_DELETE_AUDIO:
			printf("deleting audio message...\n");
			break;
		case SEND_AUDIO:
			printf("sending message...\n");
			break;
		default:
			invalid();
			break;
		}
	}
}

//asks the user if they'd like to listen to the msg they just recorded
void listenToMsg(){
	char ch;
	transmitting = TRUE;
	printf("\nWould you like to listen to your recording? (y / n)\n");
	while (transmitting) {
		if (_kbhit()) {          // Check if a key has been pressed
			ch = _getch();       // Get the character without waiting for Enter
			if (ch == 'y' || ch == 'Y') {
				playFront(); //play message and then go to audio sub menu
				break;
			}
			else if (ch == 'n' || ch == 'N') {
				//user hit no, skip to audio sub menu
				break;
			}
			else {
				printf("\nInvalid key '%c'. Press 'y' or 'n'.\n", ch);
			}
		}
	}
	system("cls");
}

//**********************************************************************************    ENCRYPT / DECRPYT
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

//**********************************************************************************    MAIN TRANSMITTER LOOP
//main transmitter branch loop
void transmitterLoop(HANDLE* hComTx){
	transmitting = TRUE;
	while (transmitting) {
		transmittingMenu(); //Tx print menu
		int Tx_choice = getInput();

		switch (Tx_choice) {
		case NEW_TEXT:
			selectTextType(hComTx);
			break;

		case NEW_AUDIO:
			recordAndSendAudio(hComTx);
			//recordNew(); //function from w1
			//listenToMsg();
			//newAudioOptions();
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