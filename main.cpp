/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 2 ***

				

==========================================================================================================================
*/
#include "sound.h"
#include "audioQueue.h"
#include "RS232Comm.h"
#include "ui.h"
#include "Tx.h"
#include "Rx.h"
#include <stdio.h>
#include <windows.h>


int	main() {


	//Communication variables and parameters
	HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
	HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
	int nComRate = 9600;								// Baud (Bit) rate in bits/second 
	int nComBits = 8;									// Number of bits per frame
	COMMTIMEOUTS timeout = { 0 };						// A commtimeout struct variable

	int running = TRUE;
	int Rx_choice;
	int Tx_choice;
	

	initQueue();
	InitializePlayback();
	InitializeRecording();

	label_A: //checkpoint for users returning to the startup menu

	int mode = selectStation(); //users decide if this is transmitter or receiver

	//transmitter
	if (mode == TRANSMITTER) {
		wchar_t COMPORT_Tx[] = L"COM6";
		initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);

		while (running) {
			transmittingMenu();
			printf("\nSelect an option: "); //collect user input
			scanf_s("%d", &Tx_choice);
			while (getchar() != '\n'); //flush extra input from 'enter'


			switch (Tx_choice) {
			case NEW_TEXT:
				newText(&hComTx); //create and send new text messages
				break;

			case NEW_AUDIO:
				system("cls");
				printf("\nrecording audio\n");
				break;

			case Tx_TESTING:
				system("cls");
				printf("\ntesting\n");
				break;

			case Tx_PHONEBOOK:
				system("cls");
				printf("\nlooking at phonebook\n");
				break;

			case Tx_GO_BACK:
				Tx_goBack();
				goto label_A;
				break;
			}
		}
		
	}
	//receiver
	else if (mode == RECEIVER) {
		wchar_t COMPORT_Rx[] = L"COM5";
		initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);

		while (running) {
			receivingMenu();
			printf("\nSelect an option: "); //collect user input
			scanf_s("%d", &Rx_choice);
			while (getchar() != '\n'); //flush extra input from 'enter'

			switch (Rx_choice) {
			case PLAY_TEXT:
				playText(&hComRx);
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
				goto label_A;
				break;
			}
		}
	}
	//quit program
	else if (mode == QUIT) {
		printf("\nQuitting program...\n");
		Sleep(500);
		exit(0);
	}
	//invalid input
	else {
		printf("\nINVALID SELECTION\n");
	}

	//cleanup
	purgePort(&hComTx);
	purgePort(&hComRx);
	CloseHandle(hComTx);
	CloseHandle(hComRx);
	ClosePlayback();
	CloseRecording();
	clearQueue(); //free all queue memory

	return 0;
}