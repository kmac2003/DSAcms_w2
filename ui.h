/*

================================== SECTION HEADER =====================================

Program Name:   EECE72405-25F
Author:         KIEN MACARTNEY
Date:           OCT 15 2025
Comments:		Projects III - Coded Messaging System

				User interface header file

=======================================================================================

*/

#pragma once

enum SELECT_MODE {
	TRANSMITTER = 1,
	RECEIVER = 2,
	QUIT = 3
};

//w2
enum TRANSMITTER_CHOICES {
	NEW_TEXT = 1,
	NEW_AUDIO = 2,
	Tx_TESTING = 3,
	Tx_PHONEBOOK = 4,
	Tx_GO_BACK = 5
};

//w2
enum RECEIVER_CHOICES {
	PLAY_TEXT = 1,
	PLAY_AUDIO = 2,
	Rx_TESTING = 3,
	Rx_PHONEBOOK = 4,
	Rx_GO_BACK = 5
};

//w1
extern short iBigBuf[]; //global recording buffer
extern long lBigBufSize; //total number of samples

//w1
void transmittingMenu();
void recordNew();
void saveFront();
void playFront();
void deleteFront();

//w1
int selectStation();
void receivingMenu();