/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Receiver implementation file

				*** Week 2 ***



==========================================================================================================================
*/
#include "Rx.h"
#include "RS232Comm.h"
#include <stdio.h>

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

void Rx_goBack(){
	printf("\nReturning to previous menu...\n");
	Sleep(1000);
	system("cls");
}
