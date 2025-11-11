/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				XOR Encrpytion header file

==========================================================================================================================
*/
#pragma once

//XOR encrypt and decrypt
void testXorEncryption(HANDLE* hComTx);
void xorEncrypt(const char* input, char* output, char key);
void xorDecrypt(const char* input, char* output, char key);

//RLE encrpyt and decrypt
int RLE_Encode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc);
int RLE_Decode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc);