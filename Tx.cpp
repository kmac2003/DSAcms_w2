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

//variables
char msgOut[BUFSIZE];
size_t len;
BOOL sending = TRUE;

//creates and sends new text messages
void newText(HANDLE* hComTx){
	system("cls");
	printf("\nType your messages below...\n");
	printf("Enter 'q' to stop sending\n");

	while (sending) { 
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
			sending = FALSE;
			printf("\nStopped sending\n");
			clearScreen();
			break;
		}
		//send message
		outputToPort(hComTx, msgOut, strlen(msgOut) + 1);
		//printf("Message sent: %s\n", msgOut);
	}
}

//creates and sends new audio message
void newAudio(HANDLE* hComTx){
	char name[MAX_FILENAME];
	printf("\nEnter a name for this recording: ");
	scanf_s("%63s", name, (unsigned)_countof(name));
	while (getchar() != '\n'); //flush input

	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();
	printf("\nRecording complete\n");
	
	//send a start tag
	const char* header = "[AUDIO_START]";
	outputToPort(hComTx, (LPCVOID)header, (DWORD)strlen(header));
	Sleep(100);

	//send actual audio bytes
	DWORD audioBytes = (DWORD)(lBigBufSize * sizeof(short));
	outputToPort(hComTx, (LPCVOID)iBigBuf, audioBytes);
	Sleep(100);

	//send an end tag
	const char* footer = "[AUDIO_END]";
	outputToPort(hComTx, (LPCVOID)footer, (DWORD)strlen(footer));

	printf("%s sent successfully! Total bytes: %ld\n", name, lBigBufSize);

	//reopen recording device for the next recording
	InitializeRecording();
}

//main transmitter branch loop
void transmitterLoop(HANDLE* hComTx){
	int Tx_choice;
	int running = TRUE;

	while (running) {
		transmittingMenu(); //Tx print menu

		printf("\nSelect an option: "); //collect user input
		scanf_s("%d", &Tx_choice);
		while (getchar() != '\n'); //flush extra input from 'enter'

		switch (Tx_choice) {
		case NEW_TEXT:
			newText(hComTx); //create and send new text messages
			break;

		case NEW_AUDIO:
			newAudio(hComTx);
			break;

		case Tx_GO_BACK:
			goBack();
			running = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}
