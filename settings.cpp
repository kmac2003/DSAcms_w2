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

//**********************************************************************************    MODIFY SETTING FUNCTIONS
//allow user to select desired com port numbers
void configureComPorts() {
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n========== COM Port Config =========\n");
    printf("\nCurrent COM ports: Tx = COM%d, Rx = COM%d\n", cfg.COM_TX, cfg.COM_RX);

    printf("\nEnter new Transmitter COM number\n");
    cfg.COM_TX = getInput();

    printf("\nEnter new Receiver COM number\n");
    cfg.COM_RX = getInput();

    //update port nums
    txPortNum = cfg.COM_TX;
    rxPortNum = cfg.COM_RX;

    saveConfig(CONFIG_FILE, &cfg);
    printf("\nCOM ports updated!\n");

    enterToContinue();
}

//allow user to turn message headers on/off
void toggleHeader() {
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n========== Toggle headers ==========\n");
    printf("\n1 = ON\n0 = OFF\n");

    int choice = getInput();
    cfg.HEADERS = (choice == 1) ? 1 : 0;

    saveConfig(CONFIG_FILE, &cfg);
    printf("\nHeaders %s!\n", cfg.HEADERS ? "ON" : "OFF");

    enterToContinue();
}

//allow user to toggle the detection of errors in messages
void toggleErrorDtct(){
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n====== Toggle error detection ======\n");
    printf("\n1 = ON\n0 = OFF\n");

    int choice = getInput();
    cfg.ERR_DTCT = (choice == 1) ? 1 : 0;

    saveConfig(CONFIG_FILE, &cfg);
    printf("\nError detection %s!\n", cfg.ERR_DTCT ? "ON" : "OFF");

    enterToContinue();
}

//set encryption type (Vigenere or XOR)
void encryptType() {
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n======= Set encryption type ========\n");
    printf("\n1 = XOR\n2 = VIGENERE\n0 = OFF\n");

    int choice = getInput();
    if (choice >= 0 && choice <= 2) {
        cfg.ENCRYPT = choice;
        saveConfig(CONFIG_FILE, &cfg);
        if (cfg.ENCRYPT == XOR) {
            printf("\nEncryption set to XOR\n");
        }
        else if (cfg.ENCRYPT == VIGENERE) {
            printf("\nEncryption set to VIGENERE\n");
        }
        else {
            printf("\nEncryption OFF\n");
        }
    }
    else {
        invalid();
    }
    enterToContinue();
}

//set encryption type (Huffman or RLE)
void compressType() {
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n======= Set compression type =======\n");
    printf("\n1 = Huffman\n2 = RLE\n0 = OFF\n");

    int choice = getInput();
    if (choice >= 0 && choice <= 2) {
        cfg.COMPRESS = choice;
        saveConfig(CONFIG_FILE, &cfg);
        if (cfg.COMPRESS == RLE) {
            printf("\nCompression set to RLE\n");
        }
        else if (cfg.COMPRESS == HUFFMAN) {
            printf("\nCompression set to HUFFMAN\n");
        }
        else {
            printf("\nCompression OFF\n");
        }
    }
    else {
        invalid();
    }
    enterToContinue();
}

//write sender ID number
void configSID() {
    system("cls");
    printf("\n============= SETTINGS =============");
    printf("\n=========== Configure SID ==========\n");
    printf("\nEnter integer (0 to 255)\n");

    int number = getInput();
    if (number >= 0 && number <= 255) {
        cfg.SID = number;
        saveConfig(CONFIG_FILE, &cfg);
        printf("\nSender ID updated.\n");
    }
    else {
        printf("\nInvalid SID!\n");
    }
    enterToContinue();
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

        case TOGGLE_ERR_DTCT:
            toggleErrorDtct();
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