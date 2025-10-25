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
#include <stdio.h>
#include <windows.h>

int	main(){
	
	int running = TRUE;
	int choice;

	initQueue();
	InitializePlayback();
	InitializeRecording();

	while (running) {
		displayMenu();
		printf("\nSelect an option: ");
		scanf_s("%d", &choice);
		while (getchar() != '\n'); //flush extra input from 'enter'

		switch (choice) {
		case RECORD_NEW:
			system("cls");
			recordNew();
			break;

		case SAVE_FRONT:
			system("cls");
			saveFront();
			break;

		case PLAY_FRONT:
			system("cls");
			playFront();
			break;

		case DELETE_FRONT:
			system("cls");
			deleteFront();
			break;

		case QUIT:
			printf("\Quitting program...\n");
			running = FALSE;
			break;
		}
	}
	ClosePlayback();
	CloseRecording();
	clearQueue(); //free all queue memory
	system("pause");
	return 0;
}