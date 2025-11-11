/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

				XOR Encrpytion implementation file

==========================================================================================================================
*/
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "sound.h"
#include "audioQueue.h"
#include "config.h"
#include "compress.h"
#include "encrypt.h"

//**********************************************************************************    XOR ENCRYPT / DECRPYT
//xor encrpyt
void xorEncrypt(const char* input, char* output, char key) {
	int i = 0;
	while (input[i] != '\0') {
		output[i] = input[i] ^ key;
		i++;
	}
	output[i] = '\0';
}

//xor decrpyt
void xorDecrypt(const char* input, char* output, char key) {
	int i = 0;
	while (input[i] != '\0') {
		output[i] = input[i] ^ key; // same as encode
		i++;
	}
	output[i] = '\0';
}




//**********************************************************************************    RLE ENCRYPT / DECRPYT
/*************************************************************************
* New byte-oriented RLE functions with overflow protection
*************************************************************************/

int RLE_Encode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc) {
    if (inlen == 0) return 0;
    if (outlen == 0) return -1;

    unsigned int inpos = 0, outpos = 0;

    while (inpos < inlen) {
        unsigned char current = in[inpos];
        unsigned int count = 1;

        // Count consecutive identical bytes
        while (inpos + count < inlen && in[inpos + count] == current && count < 255) {
            count++;
        }

        // Check if we should use RLE encoding
        if (count > 3 || current == esc) {
            // Use RLE encoding - need 3 bytes in output
            if (outpos + 3 > outlen) {
                return -1; // Output buffer overflow
            }

            out[outpos++] = esc;
            out[outpos++] = (unsigned char)count;
            out[outpos++] = current;
            inpos += count;
        }
        else {
            // Copy literal bytes
            for (unsigned int i = 0; i < count; i++) {
                if (outpos >= outlen) {
                    return -1; // Output buffer overflow
                }
                out[outpos++] = current;
            }
            inpos += count;
        }
    }

    return (int)outpos;
}

int RLE_Decode(unsigned char* in, unsigned int inlen, unsigned char* out, unsigned int outlen, unsigned char esc) {
    if (inlen == 0) return 0;
    if (outlen == 0) return -1;

    unsigned int inpos = 0, outpos = 0;

    while (inpos < inlen && outpos < outlen) {
        unsigned char current = in[inpos++];

        if (current == esc && inpos < inlen) {
            // Potential RLE encoded sequence
            unsigned int count = in[inpos++];

            if (inpos < inlen) {
                unsigned char value = in[inpos++];

                // Check for output buffer overflow
                if (outpos + count > outlen) {
                    return -1; // Output buffer overflow
                }

                // Write the repeated bytes
                for (unsigned int i = 0; i < count; i++) {
                    out[outpos++] = value;
                }
            }
            else {
                // Malformed input - ESC at end without complete sequence
                if (outpos < outlen) {
                    out[outpos++] = current;
                }
                if (inpos - 1 < inlen && outpos < outlen) {
                    out[outpos++] = (unsigned char)count;
                }
            }
        }
        else {
            // Literal byte
            if (outpos < outlen) {
                out[outpos++] = current;
            }
            else {
                return -1; // Output buffer overflow
            }
        }
    }

    return (int)outpos;
}