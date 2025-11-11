/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				RS232 cable header file

==========================================================================================================================
*/
#pragma once

#define EX_FATAL 1
#define BUFSIZE 140

//w5
typedef struct header Header;

//w5
struct header {
	short int sid;
	short int rid;
	char priority;
	short int seqNum;
	long int payloadSize;		// Number of bytes in payload after this header
	char payLoadType;			// 0: Text, 1: Audio, 2: Image etc.
	char encryption;			// 0: None, 1: XOR,	  2: Vigenere	3: Both
	char compression;			// 0: None, 1: RLE,	  2: Huffman,	3: Both
};

extern int nComRate;				// Baud (Bit) rate in bits/second 
extern int nComBits;				// Number of bits per frame
extern COMMTIMEOUTS timeout;		// A commtimeout struct variable

//w5 additions
void transmit(Header* txHeader, void* txPayload, HANDLE* hCom);
DWORD receive(Header* rxHeader, void** rxPayload, HANDLE* hCom);

// Prototype the functions to be used
void initPort(HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
void purgePort(HANDLE* hCom);
void outputToPort(HANDLE* hCom, LPCVOID buf, DWORD szBuf);
DWORD inputFromPort(HANDLE* hCom, LPVOID buf, DWORD szBuf);

// Sub functions
void createPortFile(HANDLE* hCom, wchar_t* COMPORT);
static int SetComParms(HANDLE* hCom, int nComRate, int nComBits, COMMTIMEOUTS timeout);

//w2
HANDLE setupComPort(const wchar_t* portName, int nComRate, int nComBits, COMMTIMEOUTS timeout);
