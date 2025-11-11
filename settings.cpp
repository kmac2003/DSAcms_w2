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