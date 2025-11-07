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

enum TRANSMITTER_CHOICES {
	NEW_TEXT = 1,
	NEW_AUDIO = 2,
	Tx_GO_BACK = 3
};

enum RECEIVER_CHOICES {
	PLAY_TEXT = 1,
	PLAY_AUDIO = 2,
	Rx_GO_BACK = 3
};

//w3
enum Tx_TEXT_SUBMENU {
	COMPRESS_TEXT = 1,
	ENCRYPT_TEXT = 2,
	ADD_INFO_TEXT = 3,
	Tx_DELETE_TEXT = 4,
	SEND_TEXT = 5
};

enum Tx_AUDIO_SUBMENU {
	COMPRESS_AUDIO = 1,
	ENCRYPT_AUDIO = 2,
	ADD_INFO_AUDIO = 3,
	Tx_DELETE_AUDIO = 4,
	SEND_AUDIO = 5
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
void goBack();
void clearScreen();
int getInput();

//w3

//w2
int selectStation();
void receivingMenu();
void transmittingMenu();
void newAudioSubMenu();
void newTextSubMenu();
void runModeLoop();