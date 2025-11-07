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
void selectTextType(HANDLE* hComTx);
void newTextAdvanced();
void newAudioOptions();
void listenToMsg();
//encrypt and decrypt
void testXorEncryption(HANDLE* hComTx);
void xorEncrypt(const char* input, char* output, char key);
void xorDecrypt(const char* input, char* output, char key);
//main transmitter loop
void transmitterLoop(HANDLE* hComTx);
