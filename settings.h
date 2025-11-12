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

extern int setHeader;
extern int setEncrypt;
extern int setCompress;

void configureComPorts(int* txPortNum, int* rxPortNum);
void toggleHeader();
void encryptType();
void compressType();
//display data functions
void displayHeaderState();
void displayEncryptionType();
void displayCompressionType();
//main settings loop
void settingsLoop();
