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
#include <time.h>

//main menu
enum SELECT_MODE {
	TRANSMITTER = 1,
	RECEIVER,
	TESTING,
	PHONEBOOK,
	SETTINGS,
	QUIT
};

//transmitter
enum TRANSMITTER_CHOICES {
	INSTANT_TEXT = 1,
	COMPOSE_HEADER,
	Tx_GO_BACK
};

//receiver
enum RECEIVER_CHOICES {
	Rx_INSTANT = 1,
	LISTENING,
	MANAGE_QUEUE,
	Rx_GO_BACK
};

enum QUEUE_CHOICES {
	SHOW_FULL = 1,
	SHOW_FIRST,
	DISCARD_FIRST,
	Q_BACK
};

//settings
enum SETTINGS {
	CONFIG_COM = 1,
	TOGGLE_HEADERS,
	TOGGLE_ERR_DTCT,
	ENCRYPT_TYPE,
	COMPRESS_TYPE,
	SENDERID,
	Set_GO_BACK
};

//testing
enum TESTING {
	LOOPBACK = 1,
	CONSTRUCT_HEADER,
	SORTING_QUEUE,
	ERROR_DETECT,
	ENCRYPT_DECRPYT,
	COMPRESS_DECOMPRESS,
	BACK_TESTING
};

//phonebook
enum PHONEBOOK {
	VIEW_ALL = 1,
	SEARCH_SID,
	SEARCH_RID,
	PRINT_ALL,
	Pb_BACK
};

//get current time
struct tm getTimeStruct();

//w1

void quit();
void invalid();
void goBack();
void clearScreen();
int getInput();
void enterToContinue();

//w2
//main menu
int selectStation();
//receiver
void receivingMenu();
void queueMenu();
//transmitter
void transmittingMenu();
//settings
void settingsMenu();
//testing
void testingMenu();
//phonebook
void phonebookMenu();
//main loop
void runModeLoop();