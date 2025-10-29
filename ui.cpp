/*

================================== SECTION HEADER =====================================

Program Name:   EECE72405-25F
Author:         KIEN MACARTNEY
Date:           OCT 15 2025
Comments:		Projects III - Coded Messaging System

				User interface implementation file

=======================================================================================

*/
#include "ui.h"
#include "audioQueue.h"
#include "sound.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>



//print the user menu
void transmittingMenu(){
    printf("\n============= TRANSMITTING STATION ================\n");
    printf("1. Write a new text message\n");
    printf("2. Record new audio message\n");
    printf("3. Enter testing\n");
    printf("4. Open phonebook\n");
    printf("5. Back to menu");
    printf("\n===========================================================\n");
}

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

//select whether the program functions as a transmitter or receiver
int selectStation() {
    int mode;
    printf("====================================================\n");
    printf("    CMS PROJECT - KIEN MATTHEW TROY\n");
    printf("====================================================\n");
    printf("Which station do you want this device to be?\n");
    printf("1 - Transmitter\n");
    printf("2 - Receiver\n");
    printf("3 - Quit\n");
    printf("\nEnter choice: ");
    scanf_s("%d", &mode);
    while (getchar() != '\n'); // flush newline
    return mode;
}

void receivingMenu(){
    printf("\n============= RECEIVING STATION ================\n");
    printf("1. Write a new text message\n");
    printf("2. Record new audio message\n");
    printf("3. Enter testing\n");
    printf("4. Open phonebook\n");
    printf("5. Back to menu");
    printf("\n===========================================================\n");
}

