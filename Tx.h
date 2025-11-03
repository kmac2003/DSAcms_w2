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

void newText(HANDLE* hComTx);
void newAudio(HANDLE* hComTx);
void transmitterLoop(HANDLE* hComTx);
