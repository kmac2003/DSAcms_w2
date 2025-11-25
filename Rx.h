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

/*
int readBytes(HANDLE* hCom, void* buffer, DWORD bytesToRead);
int readHeader(HANDLE* hCom, Header* hdr);
int readPayload(HANDLE* hCom, void* buffer, long payLoadSize);
*/

//text
void rxInstantText(HANDLE* hComRx);
void receiveTextMessage(HANDLE* hComRx);
//audio
void receiveAudioAndPlay(HANDLE* hComRx);
//main receiver loop
void receiverLoop();
