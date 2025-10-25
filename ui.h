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

enum Choices {
	RECORD_NEW = 1,
	SAVE_FRONT = 2,
	PLAY_FRONT = 3,
	DELETE_FRONT = 4,
	QUIT = 5
};

extern short iBigBuf[]; //global recording buffer
extern long lBigBufSize; //total number of samples

void displayMenu();
void recordNew();
void saveFront();
void playFront();
void deleteFront();