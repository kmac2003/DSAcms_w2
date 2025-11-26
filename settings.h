/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				Settings header file

==========================================================================================================================
*/
#pragma once

#define OFF 0
#define ON 1
#define XOR 1
#define VIGENERE 2
#define HUFFMAN 1
#define RLE 2


void configureComPorts();
void toggleHeader();
void toggleErrorDtct();
void encryptType();
void compressType();
void configSID();
//main settings loop
void settingsLoop();
