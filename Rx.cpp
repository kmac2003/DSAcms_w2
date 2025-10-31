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
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "sound.h"
#include "audioQueue.h"

//variables
char msgIn[BUFSIZE];
DWORD bytesRead;

//receives and displays new text messages
void playText(HANDLE* hComRx){
	system("cls");
	printf("\nplaying text\n");

	//receive message
	bytesRead = inputFromPort(hComRx, msgIn, BUFSIZE);
	msgIn[bytesRead] = '\0';
	printf("Message received: %s\n", msgIn);
}

//notify user, delay and clear screen
void Rx_goBack(){
	printf("\nReturning to previous menu...\n");
	Sleep(1000);
	system("cls");
}

//main transmitter branch loop
void receiverLoop(HANDLE* hComRx){
	int Rx_choice;
	int running = TRUE;

	while (running) {
		receivingMenu();

		printf("\nSelect an option: "); //collect user input
		scanf_s("%d", &Rx_choice);
		while (getchar() != '\n'); //flush extra input from 'enter'

		switch (Rx_choice) {
		case PLAY_TEXT:
			playText(hComRx); //receive and play text messages
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
			Rx_goBack();
			running = FALSE;
			break;
		}
	}

}
