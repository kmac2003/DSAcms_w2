/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Transmitting header file

==========================================================================================================================
*/
#pragma once
#include <Windows.h>

#define AUDIO 1
#define TEXT 2

extern int receiverID;
extern char msgOut[];

//text
void instantTextMsg(HANDLE* hComTx);
void sendTextWithHeader(HANDLE* hComTx);
//audio
void recordAndSendAudio(HANDLE* hComTx);
//main loops
//composing header
void composeHeaderLoop();
//main transmitter loops
void transmitterLoop(HANDLE* hComTx);
