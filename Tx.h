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

extern int receiverID;
//text
void instantTextMsg(HANDLE* hComTx);
void sendTextWithHeader(HANDLE* hComTx);
void composeHeaderLoop();

//audio
void recordAndSendAudio(HANDLE* hComTx);
void listenToMsg();

//main transmitter loop
void transmitterLoop(HANDLE* hComTx);
