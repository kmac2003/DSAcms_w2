/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 11 2025
Comments:		Projects III - Coded Messaging System

                Settings implementation file

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
#include "settings.h"

#define OFF 0
#define ON 1
#define XOR 1
#define VIGENERE 2
#define HUFFMAN 1
#define RLE 2

//headers
int setHeader = 0;
int inToggleHeader = TRUE;
//encryption
int setEncrypt = 0;
int inEncryptType = TRUE;
//compression
int setCompress = 0;
int inCompressType = TRUE;

//**********************************************************************************    MODIFY SETTING FUNCTIONS
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

    Sleep(500);
    system("cls");
    Sleep(500);

    *txPortNum = newTx; //assign new com port numbers to original pointers
    *rxPortNum = newRx;

    saveComPorts(*txPortNum, *rxPortNum); //saves com port numbesr to config text file
    printf("\nCOM ports updated: Tx = COM%d, Rx = COM%d\n", *txPortNum, *rxPortNum);
}

//allow user to turn message headers on/off
void toggleHeader() {
    system("cls");
    inToggleHeader = TRUE;
    while (inToggleHeader) {
        printf("\n === Enable or disable headers === \n");
        printf("\n1 for ON\n0 for OFF\n\n");
        setHeader = getInput();
        if (setHeader == ON) { //valid header state
            printf("\nHeaders enabled!\n");
            clearScreen();
            break;
        }
        else if (setHeader == OFF) {
            printf("\nHeaders disabled!\n");
            clearScreen();
            break;
        }
        else {
            invalid();
        }
    }
}

//set encryption type (Vigenere or XOR)
void encryptType(){
    system("cls");
    inEncryptType = TRUE;
    while (inEncryptType) {
        printf("\n === Set encryption type === \n");
        printf("\n1 for XOR\n2 for VIGENERE\n0 to disable\n\n");
        setEncrypt = getInput();
        if (setEncrypt == XOR) { //XOR = 1
            printf("\nEncrypting with XOR\n");
            clearScreen();
            break;
        }
        else if (setEncrypt == VIGENERE) { //VIGENERE = 2
            printf("\nEncrypting with VIGENERE\n");
            clearScreen();
            break;
        }
        else if (setEncrypt == OFF) { //OFF = 0
            printf("\nEncryption OFF\n");
            clearScreen();
            break;
        }
        else {
            invalid();
        }
    }
}

//set encryption type (Huffman or RLE)
void compressType(){
    system("cls");
    inCompressType = TRUE;
    while (inCompressType) {
        printf("\n === Set compression type === \n");
        printf("\n1 for HUFFMAN\n2 for RLE\n0 to disable\n\n");
        setCompress = getInput();
        if (setCompress == HUFFMAN) { //HUFFMAN = 1
            printf("\nCompressing with HUFFMAN\n");
            clearScreen();
            break;
        }
        else if (setCompress == RLE) { //RLE = 2
            printf("\nCompressing with RLE\n");
            clearScreen();
            break;
        }
        else if (setCompress == OFF) { //OFF = 0
            printf("\nCompression OFF\n");
            clearScreen();
            break;
        }
        else {
            invalid();
        }
    }
}

//**********************************************************************************    STATE DISPLAY FUNCTIONS
//display whether headers are ON or OFF
void displayHeaderState() {
    if (setHeader == OFF) {
        printf("Headers:\tOFF\n");
    }
    else {
        printf("Headers:\tON\n");
    }
}

//display what kind of encryption is used
void displayEncryptionType(){
    if (setEncrypt == XOR) {
        printf("Encryption:\tXOR\n");
    }
    else if (setEncrypt == VIGENERE){
        printf("Encryption:\tVIGENERE\n");
    }
    else {
        printf("Encryption:\tOFF\n");
    }
}

//display what kind of compression is used
void displayCompressionType(){
    if (setCompress == HUFFMAN) {
        printf("Compression:\tHUFFMAN\n");
    }
    else if (setCompress == RLE){
        printf("Compression:\tRLE\n");
    }
    else {
        printf("Compression:\tOFF\n");
    }
}

//switch case for all setting configurations
void settingsLoop() {
    system("cls");
    int inSettings = TRUE;

    while (inSettings) {
        settingsMenu();
        int setOption = getInput(); //collect setting choice

        switch (setOption) {
        case CONFIG_COM:
            configureComPorts(&txPortNum, &rxPortNum);
            break;

        case TOGGLE_HEADERS:
            toggleHeader();
            break;

        case ENCRYPT_TYPE:
            encryptType();
            break;

        case COMPRESS_TYPE:
            compressType();
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