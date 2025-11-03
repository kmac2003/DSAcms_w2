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

void playText(HANDLE* hComRx);
void playAudio(HANDLE* hComRx);
void receiverLoop(HANDLE* hComRx);
