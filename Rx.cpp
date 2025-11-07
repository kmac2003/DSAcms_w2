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

//variables
char msgIn[BUFSIZE];
BOOL listening = TRUE;
BOOL receivingAudio = FALSE;
DWORD bytesRead = 0;
DWORD totalBytes = 0;
const DWORD maxAudioBytes = (DWORD)(lBigBufSize * sizeof(short)); //safe byte limit

//continuously receives and displays new text messages until user decides to stop
void playText(HANDLE* hComRx){
	system("cls");
	printf("\nListening for incoming text messages...\n");
	printf("Press 'q' to stop listening\n\n");

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
		//attempt to read from com port
		bytesRead = inputFromPort(hComRx, msgIn, BUFSIZE - 1);
		if (bytesRead > 0) {
			msgIn[bytesRead] = '\0';
			printf("Message received: %s\n", msgIn);
		}
		Sleep(100);
	}
}

//CURRENTLY BROKEN
//receive and play audio message
void playAudio(HANDLE* hComRx) {
	system("cls");
	printf("\nListening for incoming audio messages...\n");

	while (listening) {
		bytesRead = inputFromPort(hComRx, msgIn, sizeof(msgIn) - 1);
		if (bytesRead == 0) {
			Sleep(50);
			continue; //no data yet
		}
		msgIn[bytesRead] = '\0';

		//detect start of audio message
		if (!receivingAudio && strstr(msgIn, "[AUDIO_START]")) {
			printf("\nAudio start detected!\n");
			receivingAudio = TRUE;
			totalBytes = 0;
			continue;
		}
		//detect end of audio message
		if (receivingAudio && strstr(msgIn, "[AUDIO_END]")) {
			printf("\nAudio end detected! Total bytes: %ld\n", (unsigned long)totalBytes);
			PlayBuffer(iBigBuf, totalBytes / sizeof(short));
			printf("\nPlayback complete\n");
			break;
		}

		if (receivingAudio) {
			// Be careful not to overflow buffer
			if (totalBytes + bytesRead <= maxAudioBytes) {
				memcpy((char*)iBigBuf + totalBytes, msgIn, bytesRead);
				totalBytes += bytesRead;
			}
			else {
				printf("Warning: audio buffer full! Stopping.\n");
				receivingAudio = FALSE;
				break;
			}
		}
	}
}

//main transmitter branch loop
void receiverLoop(HANDLE* hComRx){
	int running = TRUE;

	while (running) {
		receivingMenu();
		int Rx_choice = getInput();

		switch (Rx_choice) {
		case PLAY_TEXT:
			playText(hComRx); //receive and play text messages
			break;

		case PLAY_AUDIO:
			//playAudio(hComRx);
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
