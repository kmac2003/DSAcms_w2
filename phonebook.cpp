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

//returns number of logs
int fnumLogs() {
    FILE* fp;
    errno_t err;
    char c;
    int counter = 0;

    if ((err = fopen_s(&fp, "phonebook.txt", "r")) == 0) {

        //read the file one character at a time
        //look for two percent signs, if we find one then look for another. if we find another, increment the counter

        while ((c = fgetc(fp)) != EOF) {
            if (c == '%') {
                counter++;
            }
        }
        printf("\nThere are %d messages in this phonebook\n", counter);
        fclose(fp);
        return(counter);
    }
    else { //protects against any issues opening the file
        printf("\nERROR: Failed to open file\n");
        fclose(fp);
        return(-1);
    }
}

//search by SID
void searchBySID() {
    FILE* fp;
    errno_t err;
    char buffer[256];
    int targetSID;
    int currentSID;
    int count = 0;

    printf("\nEnter SID to search for: ");
    scanf("%d", &targetSID);
    while (getchar() != '\n'); //clear the \n

    if ((err = fopen_s(&fp, "phonebook.txt", "r")) != 0) {
        printf("\nPhonebook not found or empty.\n");
        return;
    }

    printf("\nSearching for messages with SID = %d...\n", targetSID);
    while (fgets(buffer, sizeof(buffer), fp)) {
        // look for lines that start with SID and parse the number
        if (sscanf(buffer, "SID%*[\t ]%d", &currentSID) == 1) {
            if (currentSID == targetSID) {
                count++;
            }
        }
    }
    fclose(fp);
    printf("\nFound %d message(s) with SID = %d\n", count, targetSID);
    enterToContinue();
}

//search by RID
void searchByRID() {
    FILE* fp;
    errno_t err;
    char buffer[256];
    int targetRID;
    int currentRID;
    int count = 0;

    printf("\nEnter RID to search for: ");
    scanf("%d", &targetRID);
    while (getchar() != '\n'); //clear the \n

    if ((err = fopen_s(&fp, "phonebook.txt", "r")) != 0) {
        printf("\nPhonebook not found or empty.\n");
        return;
    }
    printf("\nSearching for messages with RID = %d...\n", targetRID);
    while (fgets(buffer, sizeof(buffer), fp)) {
        // look for lines that start with RID and parse the number
        if (sscanf(buffer, "RID%*[\t ]%d", &currentRID) == 1) {
            if (currentRID == targetRID) {
                count++;
            }
        }
    }
    fclose(fp);
    printf("\nFound %d message(s) with RID = %d\n", count, targetRID);
    enterToContinue();
}

//print entire contents of the phonebook
void printAllLogs() {
    FILE* fp;
    errno_t err;

    system("cls");
    if ((err = fopen_s(&fp, "phonebook.txt", "r")) != 0) {
        printf("\nPhonebook is empty or cannot be opened.\n");
        return;
    }
    printf("============= PHONEBOOK CONTENTS =============\n\n");
    int ch;
    int lineCount = 0;
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch); // Print exactly as stored in file
        lineCount++;
    }
    fclose(fp);
    if (lineCount == 0) {
        printf("\nPhonebook is empty\n");
    }
    printf("\n==============================================\n");

    enterToContinue();
}

//main phonebook loop
void phonebookLoop() {
    system("cls");
    int inPhonebook = TRUE;
    while (inPhonebook) {

        phonebookMenu();
        int phonebookOption = getInput();

        switch (phonebookOption) {
        case VIEW_ALL:
            fnumLogs();
            break;

        case SEARCH_SID:
            searchBySID();
            break;

        case SEARCH_RID:
            searchByRID();
            break;

        case PRINT_ALL:
            printAllLogs();
            break;

        case Pb_BACK:
            goBack();
            inPhonebook = FALSE;
            break;

        default:
            invalid();
            break;
        }
    }
}