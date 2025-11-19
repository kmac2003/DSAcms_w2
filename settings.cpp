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

//headers
int inToggleHeader = TRUE;
//encryption
int inEncryptType = TRUE;
//compression
int inCompressType = TRUE;
//SID
int inSID = TRUE;

//**********************************************************************************    MODIFY SETTING FUNCTIONS
//allow user to select desired com port numbers
void configureComPorts() {
    system("cls");
    printf("=== COM Port Configuration ===\n");
    printf("Current COM ports: Tx = COM%d, Rx = COM%d\n", cfg.COM_TX, cfg.COM_RX);

    printf("\nEnter new Transmitter COM number: ");
    cfg.COM_TX = getInput();

    printf("Enter new Receiver COM number: ");
    cfg.COM_RX = getInput();

    //update port nums
    txPortNum = cfg.COM_TX;
    rxPortNum = cfg.COM_RX;

    saveConfig(CONFIG_FILE, &cfg);
    printf("\nCOM ports updated!\n");
}

//allow user to turn message headers on/off
void toggleHeader() {
    system("cls");
    printf("\n=== Enable or disable headers ===\n");
    printf("1 = ON\n0 = OFF\n");

    int choice = getInput();
    cfg.HEADERS = (choice == 1) ? 1 : 0;

    saveConfig(CONFIG_FILE, &cfg);
    printf("\nHeaders %s!\n", cfg.HEADERS ? "ON" : "OFF");
}

//set encryption type (Vigenere or XOR)
void encryptType() {
    system("cls");
    printf("\n=== Set encryption type ===\n");
    printf("1 = XOR\n2 = VIGENERE\n0 = OFF\n");

    int choice = getInput();
    if (choice >= 0 && choice <= 2) {
        cfg.ENCRYPT = choice;
        saveConfig(CONFIG_FILE, &cfg);
        printf("\nEncryption mode updated.\n");
    }
    else {
        invalid();
    }
}

//set encryption type (Huffman or RLE)
void compressType() {
    system("cls");
    printf("\n=== Set compression type ===\n");
    printf("1 = Huffman\n2 = RLE\n0 = OFF\n");

    int choice = getInput();
    if (choice >= 0 && choice <= 2) {
        cfg.COMPRESS = choice;
        saveConfig(CONFIG_FILE, &cfg);
        printf("\nCompression mode updated.\n");
    }
    else {
        invalid();
    }
}

//write sender ID number
void configSID() {
    system("cls");
    printf("\n=== Configure Sender ID ===\n");
    printf("Enter integer 0–255: ");

    int number = getInput();
    if (number >= 0 && number <= 255) {
        cfg.SID = number;
        saveConfig(CONFIG_FILE, &cfg);
        printf("\nSender ID updated.\n");
    }
    else {
        printf("\nInvalid SID!\n");
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
            configureComPorts();
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

        case SENDERID:
            configSID();
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