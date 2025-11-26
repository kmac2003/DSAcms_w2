/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

                User interface implementation file

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

int running = TRUE;

//get current time or date
struct tm getTimeStruct() {
    time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    return *current_time;
}

//w1
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

//**********************************************************************************    MENU QOL FUNCTIONS
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

//notify user, delay and clear screen
void goBack() {
    printf("\nReturning...\n");
    Sleep(500);
    system("cls");
}

//delay and clear screen
void clearScreen(){
    Sleep(1000);
    system("cls");
}

//collect choice from user
int getInput() {
    int value;
    printf("Select an option: ");
    scanf_s("%d", &value);
    while (getchar() != '\n'); // flush the extra newline from input buffer
    return value;
}

//force user to hit enter to continue
void enterToContinue() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n'); // wait for user
    clearScreen();
}

//w2
//**********************************************************************************    PRINTF MENUS
//select whether the program functions as a transmitter or receiver
int selectStation() {
    struct tm now = getTimeStruct();
    printf("================================\n");
    printf("    & CMS: Kien Matthew Troy\n");
    printf("    Date: (%02d:%02d:%02d)\n", now.tm_mday, (now.tm_mon + 1), (now.tm_year) + 1900);
    printf("================================\n");
    printf("1 - Transmit\n");
    printf("2 - Receive\n");
    printf("3 - Testing\n");
    printf("4 - Phonebook\n");
    printf("5 - Settings\n");
    printf("6 - Quit\n\n");
    int mode = getInput();
    return mode;
}

//**********************************************************************************    RECEIVER MENUS
//selects what kind of message user wishes to receive
void receivingMenu(){
    printf("\n============= RECEIVING STATION ================\n");
    printf("1. INSTANT messaging\n");
    printf("2. Listen for advanced messages\n");
    printf("3. Show queue\n");
    printf("4. Back");
    printf("\n===========================================================\n");
}

//**********************************************************************************    TRANSMITTER MENUS
//selects what kind of message user wishes to send
void transmittingMenu() {
    printf("\n============= TRANSMITTING STATION ================\n");
    printf("1. Instant text message\n");
    printf("2. Compose header\n");
    printf("3. Back");
    printf("\n===========================================================\n");
}

//**********************************************************************************    SETTINGS MENU
//diplays setting choices
void settingsMenu() {
    printf("\n============= SETTINGS ================\n");
    printf("1. Set COM port numbers\n");
    printf("2. Toggle headers\n");
    printf("3. Toggle error detection\n");
    printf("4. Set encryption type\n");
    printf("5. Set compression type\n");
    printf("6. Set SID\n");
    printf("7. Back");
    printf("\n===========================================\n\n");
}

//**********************************************************************************    TESTING MENU
//displays testing cases
void testingMenu() {
    printf("\n============= TESTING ================\n");
    printf("1. ** Loopback\n");
    printf("2. ** Construct header\n");
    printf("3. ** Sorting queue\n");
    printf("4. ** Error detection\n");
    printf("5. Encrypt / decrypt\n");
    printf("6. Compress / decompress\n");
    printf("7. Back");
    printf("\n===========================================================\n");
}

//**********************************************************************************    PHONEBOOK MENU
//open message history
void phonebookMenu() {
    printf("\n============= PHONEBOOK ================\n");
    printf("1. Total messages\n");
    printf("2. Search by SID\n");
    printf("3. Search by RID\n");
    printf("4. Print phonebook\n");
    printf("5. Back");
    printf("\n===========================================\n\n");
}

//**********************************************************************************    CORE LOOP
//runs the core Tx/Rx system
void runModeLoop(){
    
    wchar_t txPortName[10];

    loadConfig(CONFIG_FILE, &cfg);

    while (running) {

        //display program conditions
        displayFullConfiguration();
        
        swprintf(txPortName, 10, L"COM%d", (txPortNum = cfg.COM_TX)); //formats PortNum into L"COM#"

        int mode = selectStation(); //users decide if this is transmitter or receiver

        switch (mode) {
        case TRANSMITTER:
            hComTx = setupComPort(txPortName, nComRate, nComBits, timeout); //setup Tx port
            transmitterLoop(&hComTx); //run transmitter menu loop
            CloseHandle(hComTx);
            purgePort(&hComTx);
            break;

        case RECEIVER:
            receiverLoop(); //run receiver menu loop
            break;

        case TESTING:
            hComTx = setupComPort(txPortName, nComRate, nComBits, timeout); //setup Tx port
            testingLoop(&hComTx);
            CloseHandle(hComTx);
            purgePort(&hComTx);
            break;

        case PHONEBOOK:
            phonebookLoop();
            break;

        case SETTINGS:
            settingsLoop();
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

