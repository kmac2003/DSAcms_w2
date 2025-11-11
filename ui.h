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

//main menu
enum SELECT_MODE {
	TRANSMITTER = 1,
	RECEIVER = 2,
	TESTING = 3,
	PHONEBOOK = 4,
	SETTINGS = 5,
	QUIT = 6
};

//transmitter
enum TRANSMITTER_CHOICES {
	NEW_TEXT = 1,
	NEW_AUDIO = 2,
	Tx_GO_BACK = 3
};

enum Tx_AUDIO_SUBMENU {
	COMPRESS_AUDIO = 1,
	ENCRYPT_AUDIO = 2,
	ADD_INFO_AUDIO = 3,
	Tx_DELETE_AUDIO = 4,
	SEND_AUDIO = 5
};

enum TEXT_TYPE {
	INSTANT = 1,
	ADVANCED = 2,
	Tx_SUB_GO_BACK = 3
};

enum Tx_TEXT_ADVANCED_MENU {
	COMPRESS_TEXT = 1,
	ENCRYPT_TEXT = 2,
	ADD_INFO_TEXT = 3,
	DELETE_TEXT = 4,
	SEND_TEXT = 5,
	GO_BACK_TEXT = 6
};

//receiver
enum RECEIVER_CHOICES {
	PLAY_TEXT = 1,
	PLAY_AUDIO = 2,
	Rx_GO_BACK = 3
};

//settings
enum SETTINGS {
	CONFIG_COM = 1,
	Set_GO_BACK = 2
};

//testing
enum TESTING {
	VALIDATE_HARDWARE = 1,
	LOOPBACK = 2,
	CONSTRUCT_HEADER = 3,
	SORTING_QUEUE = 4,
	ERROR_DETECT = 5,
	ENCRYPT_DECRPYT = 6,
	COMPRESS_DECOMPRESS = 7,
	BACK_TESTING = 8
};

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
//main menu
int selectStation();
//receiver
void receivingMenu();
//transmitter
void transmittingMenu();
void newAudioSubMenu();
void newTextTypeMenu();
void newTextAdvancedMenu();
//settings
void settingsMenu();
//testing
void testingMenu();
//main loop
void runModeLoop();