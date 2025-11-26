/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 25 2025
Comments:		Projects III - Coded Messaging System

				Phonebook implementation file

==========================================================================================================================
*/
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Tx.h"
#include "Rx.h"
#include "ui.h"
#include "RS232Comm.h"
#include "audioQueue.h"
#include "sound.h"
#include "config.h"
#include "compress.h"
#include "encrypt.h"
#include "settings.h"
#include "testing.h"
#include "phonebook.h"

//log messages into the phonebook
void logHeaderToPhonebook(Header* h) {
    FILE* fp = fopen("phonebook.txt", "a");
    if (!fp) {
        printf("Error: Could not open phonebook.txt for logging!\n");
        return;
    }

    // Timestamp
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    // Start of message indicator
    fprintf(fp, "%% [%s]\n", timestamp);

    // Match EXACT format of buildHeader() printout
    fprintf(fp, "SID\t\t%d\n", h->sid);
    fprintf(fp, "RID\t\t%d\n", h->rid);
    fprintf(fp, "Priority\t%d\n", h->priority);
    fprintf(fp, "Size\t\t%ld bytes\n", h->payloadSize);

    if (h->payLoadType == AUDIO)
        fprintf(fp, "PayloadType\tAUDIO\n");
    else
        fprintf(fp, "PayloadType\tTEXT\n");

    if (h->encryption == XOR)
        fprintf(fp, "Encryption\tXOR\n");
    else if (h->encryption == VIGENERE)
        fprintf(fp, "Encryption\tVIGENERE\n");
    else
        fprintf(fp, "Encryption\tOFF\n");

    if (h->compression == RLE)
        fprintf(fp, "Compression\tRLE\n");
    else if (h->compression == HUFFMAN)
        fprintf(fp, "Compression\tHUFFMAN\n");
    else
        fprintf(fp, "Compression\tOFF\n");

    fprintf(fp, "-----------------------------------\n\n");

    fclose(fp);
}