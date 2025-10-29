/*

===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 15 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 1 ***

				The goal of this week is to create an implementation of a queuing package for messages. The program must
				record n seconds of audio from the device's microphone and play it back to the user.

				Characteristics of the program include:
				- 8 bits per sample
				- 8kHz sample rate
				- Programmability is a bonus

				What is the purpose of the queue?
				Right now, the code can record, playback, save and load only ONE audio message at a time. A queue package
				would allow the storage of multiple messages which could be played back or deleted later.

				What was chanegd to the code for this week?
				- added audioQueue.cpp and audioQueue.h to implement the queue structure
				- created a menu system that calls the functions in those files
				- modularized some sections in the main to clean it up
				- removed "iBigBufNew" as it is no longer needed

==========================================================================================================================

*/
#include "sound.h"
#include "audioQueue.h"
#include "ui.h"
#include "RS232Comm.h"
#include <stdio.h>
#include <windows.h>


int	main() {


	int running = TRUE;
	int Rx_choice;
	int Tx_choice;


	initQueue();
	InitializePlayback();
	InitializeRecording();

	label_A: //checkpoint for users returning to the startup menu

	int mode = selectStation(); //users decide if this is transmitter or receiver

	if (mode == TRANSMITTER) {
		while (running) {
			transmittingMenu();
			printf("\nSelect an option: "); //collect user input
			scanf_s("%d", &Tx_choice);
			while (getchar() != '\n'); //flush extra input from 'enter'

			switch (Tx_choice) {
			case NEW_TEXT:
				system("cls");
				printf("\nrecording text\n");
				break;

			case NEW_AUDIO:
				system("cls");
				printf("\nrecording audio\n");
				//recordNew();
				break;

			case Tx_TESTING:
				system("cls");
				printf("\ntesting\n");
				//playFront();
				break;

			case Tx_PHONEBOOK:
				system("cls");
				printf("\nlooking at phonebook\n");
				//deleteFront();
				break;

			case Tx_GO_BACK:
				printf("\nReturning to previous menu...\n");
				Sleep(1000);
				system("cls");
				goto label_A;
				break;
			}
		}
	}

	else if (mode == RECEIVER) {
		while (running) {
			receivingMenu();
			printf("\nSelect an option: "); //collect user input
			scanf_s("%d", &Rx_choice);
			while (getchar() != '\n'); //flush extra input from 'enter'

			switch (Rx_choice) {
			case PLAY_TEXT:
				system("cls");
				printf("\nplaying text\n");
				break;
			case PLAY_AUDIO:
				system("cls");
				printf("\nplaying audio\n");
				break;
			case Rx_TESTING:
				system("cls");
				printf("\ntesting\n");
				break;
			case Rx_PHONEBOOK:
				system("cls");
				printf("\nlooking at phonebook\n");
				break;
			case Rx_GO_BACK:
				printf("\nReturning to previous menu...\n");
				Sleep(1000);
				system("cls");
				goto label_A;
				break;
			}
		}
	}
	else if (mode == QUIT) {
		printf("\nQuitting program...\n");
		Sleep(500);
		exit(0);
	}
	else {
		printf("\nINVALID SELECTION\n");
	}

	

	ClosePlayback();
	CloseRecording();
	clearQueue(); //free all queue memory

	return 0;
}