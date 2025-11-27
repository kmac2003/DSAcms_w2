//everything receiver header
/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Receiver header file

==========================================================================================================================
*/
#pragma once

#include <Windows.h>
#include "RS232Comm.h"


void receiveMessage(HANDLE* hComRx);
//text
void rxInstantText(HANDLE* hComRx);
bool validateReceivedPayload(char* data, int length);
bool correctPayload(char* data, int length);
void queueLoop();
//void receiveTextMessage(HANDLE* hComRx);
//audio
//void receiveAudioAndPlay(HANDLE* hComRx);
//main receiver loop
void receiverLoop();
