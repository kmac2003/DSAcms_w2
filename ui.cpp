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
void displayMenu(){
    printf("\n============= CMS MENU - KIEN TROY MATTHEW ================\n");
    printf("1. Record a new message\n");
    printf("2. Save current message\n");
    printf("3. Play front message\n");
    printf("4. Delete front message\n");
    printf("5. Exit");
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
