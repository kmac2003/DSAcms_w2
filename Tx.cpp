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
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "sound.h"
#include "audioQueue.h"

//variables
size_t len;
char msgOut[BUFSIZE];

//creates and sends new text messages
void newText(HANDLE* hComTx){
	system("cls");
	printf("\nrecording text\n");

	//send message
	printf("\nEnter message:\n");
	fgets(msgOut, sizeof(msgOut), stdin); //collect string from the user
	//this removes any unwanted newlines at the end of the string
	len = strlen(msgOut);
	if (len > 0 && msgOut[len - 1] == '\n') {
		msgOut[len - 1] = '\0';
	}
	outputToPort(hComTx, msgOut, strlen(msgOut) + 1);
	printf("Message sent: %s\n", msgOut);
}

//notify user, delay and clear screen
void Tx_goBack(){
	printf("\nReturning to previous menu...\n");
	Sleep(1000);
	system("cls");
}

//main transmitter branch loop
void transmitterLoop(HANDLE* hComTx){
	int Tx_choice;
	int running = TRUE;

	while (running) {
		transmittingMenu();
		printf("\nSelect an option: "); //collect user input
		scanf_s("%d", &Tx_choice);
		while (getchar() != '\n'); //flush extra input from 'enter'

		switch (Tx_choice) {
		case NEW_TEXT:
			newText(hComTx); //create and send new text messages
			break;

		case NEW_AUDIO:
			printf("\nrecording audio\n");
			Sleep(2000);
			break;

		case Tx_GO_BACK:
			Tx_goBack();
			running = FALSE;
			break;

		default:
			invalid();
			break;
		}
	}
}
