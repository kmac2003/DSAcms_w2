/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

                User interface implementation file

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

//w1
//record a new message and enqueue it
void recordNew(){
    char name[MAX_FILENAME];
    printf("\nEnter a name for this recording: ");
    scanf_s("%63s", name, (unsigned)_countof(name));
    while (getchar() != '\n'); //flush input

    RecordBuffer(iBigBuf, lBigBufSize);
    CloseRecording();

    printf("\nRecording complete. Adding message '%s' to queue...\n", name);
    enqueue(iBigBuf, lBigBufSize, name);

    //reopen recording device for the next recording
    InitializeRecording();
}

//save the front message in the queue to file
void saveFront(){
    if (isQueueEmpty()) {
        printf("\nQueue is empty. Nothing to save\n");
        return;
    }
    link frontNode = peekQueue();
    saveAudio(frontNode->Data.buffer, frontNode->Data.size, frontNode->Data.filename);
}

//play the front message in the queue
void playFront(){
    if (isQueueEmpty()) {
        printf("\nQueue is empty. No messages to play\n");
        return;
    }
    link frontNode = peekQueue();
    printf("\nPlaying message: %s\n", frontNode->Data.filename);
    PlayBuffer(frontNode->Data.buffer, frontNode->Data.size);
    ClosePlayback();
    InitializePlayback();
}

//delete the front message in the queue
void deleteFront(){
    if (isQueueEmpty()) {
        printf("\nQueue is empty. Nothing to delete\n");
        return;
    }
    link deleted = deQueue();
    printf("Deleted message: %s\n", deleted->Data.filename);

    //free audio buffer
    free(deleted->Data.buffer);
    free(deleted);
}

//program quit
void quit() {
    printf("\nQuitting program...\n");
    Sleep(500);
    exit(0);
}

//notify user of invalid input
void invalid() {
    printf("\nINVALID SELECTION\n");
    Sleep(1000);
    system("cls");
}

//w2
//select whether the program functions as a transmitter or receiver
int selectStation() {
    system("cls");
    int mode;
    printf("====================================================\n");
    printf("    CMS PROJECT - KIEN MATTHEW TROY\n");
    printf("====================================================\n");
    printf("1 - Transmit\n");
    printf("2 - Receive\n");
    printf("3 - Enter testing\n");
    printf("4 - Open phonebook\n");
    printf("5 - Settings\n");
    printf("6 - Quit\n");
    printf("\nEnter choice: ");
    scanf_s("%d", &mode);
    while (getchar() != '\n'); // flush newline
    return mode;
}

//print receiver menu
void receivingMenu(){
    system("cls");
    printf("\n============= RECEIVING STATION ================\n");
    printf("1. Play recent text message\n");
    printf("2. Play recent audio message\n");
    printf("3. Back to menu");
    printf("\n===========================================================\n");
}

//print transmitter menu
void transmittingMenu() {
    system("cls");
    printf("\n============= TRANSMITTING STATION ================\n");
    printf("1. Write a new text message\n");
    printf("2. Record new audio message\n");
    printf("3. Back to menu");
    printf("\n===========================================================\n");
}

//runs the core Tx/Rx system
void runModeLoop(){
    HANDLE hComRx;			//pointer to receiver com port
    HANDLE hComTx;			//pointer to transmitter com port
    int running = TRUE;

    while (running) {
        int mode = selectStation(); //users decide if this is transmitter or receiver

        switch (mode) {
        case TRANSMITTER:
            hComTx = setupComPort(L"COM6", nComRate, nComBits, timeout); //setup Tx port
            transmitterLoop(&hComTx); //run transmitter menu loop
            CloseHandle(hComTx);
            purgePort(&hComTx);
            break;

        case RECEIVER:
            hComRx = setupComPort(L"COM5", nComRate, nComBits, timeout); //setup Rx port
            receiverLoop(&hComRx); //run receiver menu loop
            CloseHandle(hComRx);
            purgePort(&hComRx);
            break;

        case TESTING:
            system("cls");
            printf("testing\n");
            break;

        case PHONEBOOK:
            system("cls");
            printf("phonebook\n");
            break;

        case SETTINGS:
            system("cls");
            printf("settings\n");
            break;

        case QUIT:
            quit();
            break;

        default: //invalid
            invalid();
            continue;
        }
    }
}

