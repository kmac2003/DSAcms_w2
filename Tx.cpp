/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Transmitting implementation file

				*** Week 2 ***



==========================================================================================================================
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tx.h"
#include "RS232Comm.h"


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
