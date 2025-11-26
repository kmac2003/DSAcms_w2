/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				Testing header file

==========================================================================================================================
*/
#pragma once

void huffmanDemo();
void stripNewline(char* s);
void xorDemo();
void testingLoop(HANDLE* hComTx);
//error testing
void testErrDetection(HANDLE* hComTx);
unsigned char computeChecksum(const char* data, int length);
void introduceBitErrors(char* data, int length, float errorRate = 0.05f);
void sendNormalQuote(HANDLE* hComTx);
void sendCorruptedQuote(HANDLE* hComTx);
