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
#include "config.h"

int running = TRUE;
int txPortNum;
int rxPortNum;

//w1
//record new audio and save it in the front of the queue
void recordNew() {
    system("cls");
    char name[MAX_FILENAME];
    printf("\nEnter a name for this recording: ");
    scanf_s("%63s", name, (unsigned)_countof(name));
    while (getchar() != '\n'); //flush input

    RecordBuffer(iBigBuf, lBigBufSize);
    CloseRecording();

    printf("\nRecording complete!\n");
    printf("Adding message '%s' to queue...\n", name);
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
    Sleep(1500);
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

//w3
//**********************************************************************************    SETTINGS FUNCTIONS
//allow user to select desired com port numbers
void configureComPorts(int* txPortNum, int* rxPortNum) {
    system("cls");
    printf("=== COM Port Configuration ===\n");
    printf("Current COM ports: Tx = COM%d, Rx = COM%d\n", *txPortNum, *rxPortNum);

    printf("\nEnter new Transmitter COM number: ");
    int newTx;
    scanf_s("%d", &newTx);

    printf("Enter new Receiver COM number: ");
    int newRx;
    scanf_s("%d", &newRx);

    *txPortNum = newTx; //assign new com port numbers to original pointers
    *rxPortNum = newRx;

    saveComPorts(*txPortNum, *rxPortNum); //saves com port numbesr to config text file
    printf("\nCOM ports updated: Tx = COM%d, Rx = COM%d\n", *txPortNum, *rxPortNum);
}

//switch case for all setting configurations
void settingsLoop() {
    system("cls");
    int inSettings = TRUE;

    while (inSettings) {
        settingsMenu();
        int option = getInput(); //collect setting choice

        switch (option) {
        case CONFIG_COM:
            configureComPorts(&txPortNum, &rxPortNum);
            break;

        case Set_GO_BACK:
            goBack();
            inSettings = FALSE;
            //return to main menu
            break;

        default:
            invalid();
            break;
        }
    }
}

//w2
//**********************************************************************************    PRINTF MENUS
//select whether the program functions as a transmitter or receiver
int selectStation() {
    printf("====================================================\n");
    printf("    CMS PROJECT - KIEN MATTHEW TROY\n");
    printf("====================================================\n");
    printf("1 - Transmit\n");
    printf("2 - Receive\n");
    printf("3 - Enter testing\n");
    printf("4 - Open phonebook\n");
    printf("5 - Settings\n");
    printf("6 - Quit\n\n");
    int mode = getInput();
    return mode;
}

//**********************************************************************************    RECEIVER MENUS
//selects what kind of message user wishes to receive
void receivingMenu(){
    printf("\n============= RECEIVING STATION ================\n");
    printf("1. Play recent text message\n");
    printf("2. Play recent audio message\n");
    printf("3. Back");
    printf("\n===========================================================\n");
}

//**********************************************************************************    TRANSMITTER MENUS
//selects what kind of message user wishes to send
void transmittingMenu() {
    printf("\n============= TRANSMITTING STATION ================\n");
    printf("1. Write a new text message\n");
    printf("2. Record new audio message\n");
    printf("3. Back");
    printf("\n===========================================================\n");
}

//displays options once audio message is recorded
void newAudioSubMenu() {
    printf("\n============= AUDIO MESSAGE OPTIONS ================\n");
    printf("1. Compress audio\n");
    printf("2. Encrypt audio\n");
    printf("3. Add audio message info\n");
    printf("4. Delete\n");
    printf("5. Send");
    printf("\n===========================================================\n");
}

//user can select instant messaging, or messages that can be encrypted or compressed
void newTextTypeMenu() {
    printf("\n============= SELECT TEXT MESSAGE TYPE ================\n");
    printf("1. Instant\n");
    printf("2. Advanced\n");
    printf("3. Back");
    printf("\n===========================================================\n");
}

//displays options once text message is recorded
void newTextAdvancedMenu() {
    printf("\n============= ADVANCED TEXT MESSAGE ================\n");
    printf("1. Compress text\n");
    printf("2. Encrypt text\n");
    printf("3. Add text message info\n");
    printf("4. Delete\n");
    printf("5. Send\n");
    printf("6. Back");
    printf("\n===========================================================\n");
}

//**********************************************************************************    SETTINGS MENUS
//diplays setting choices
void settingsMenu() {
    printf("\n============= SETTINGS ================\n");
    printf("1. Set com port numbers\n");
    printf("2. Back");
    printf("\n===========================================\n\n");
}

//**********************************************************************************    CORE LOOP
//runs the core Tx/Rx system
void runModeLoop(){
    HANDLE hComRx;			//pointer to receiver com port
    HANDLE hComTx;			//pointer to transmitter com port
    wchar_t txPortName[10];
    wchar_t rxPortName[10];

    //init com ports
    loadComPorts(&txPortNum, &rxPortNum);

    while (running) {
        swprintf(txPortName, 10, L"COM%d", txPortNum); //formats PortNum into L"COM#"
        swprintf(rxPortName, 10, L"COM%d", rxPortNum);

        int mode = selectStation(); //users decide if this is transmitter or receiver

        switch (mode) {
        case TRANSMITTER:
            hComTx = setupComPort(txPortName, nComRate, nComBits, timeout); //setup Tx port
            transmitterLoop(&hComTx); //run transmitter menu loop
            CloseHandle(hComTx);
            purgePort(&hComTx);
            break;

        case RECEIVER:
            hComRx = setupComPort(rxPortName, nComRate, nComBits, timeout); //setup Rx port
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

