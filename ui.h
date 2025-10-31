/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				User interface header file

==========================================================================================================================
*/
#pragma once

#include <Windows.h>

//w2
enum SELECT_MODE {
	TRANSMITTER = 1,
	RECEIVER = 2,
	TESTING = 3,
	PHONEBOOK = 4,
	SETTINGS = 5,
	QUIT = 6
};

//w2
enum TRANSMITTER_CHOICES {
	NEW_TEXT = 1,
	NEW_AUDIO = 2,
	Tx_GO_BACK = 3
};

//w2
enum RECEIVER_CHOICES {
	PLAY_TEXT = 1,
	PLAY_AUDIO = 2,
	Rx_GO_BACK = 3
};

//w1
extern short iBigBuf[]; //global recording buffer
extern long lBigBufSize; //total number of samples

//w1
void recordNew();
void saveFront();
void playFront();
void deleteFront();
void quit();
void invalid();

//w2
int selectStation();
void receivingMenu();
void transmittingMenu();
void runModeLoop();