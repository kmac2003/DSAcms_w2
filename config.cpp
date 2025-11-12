/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 6 2025
Comments:		Projects III - Coded Messaging System

				Config implementation file

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
#include "sound.h"
#include "audioQueue.h"
#include "config.h"
#include "compress.h"
#include "encrypt.h"

#define CONFIG_FILE "config.txt"

//saves com ports to config.txt
void saveComPorts(int TxPort, int RxPort){
	FILE* file = fopen(CONFIG_FILE, "w");
	if (file) {
		fprintf(file, "%d %d\n", TxPort, RxPort);
		fclose(file);
		printf("\nCOM ports saved to %s\n", CONFIG_FILE);
	}
	else {
		printf("ERROR: Could not open %s for writing\n", CONFIG_FILE);
	}
}

//loads com ports from config.txt, defaults to 5 and 6
void loadComPorts(int* TxPort, int* RxPort){
	FILE* file = fopen(CONFIG_FILE, "r");
	if (file) {
		fscanf_s(file, "%d %d", TxPort, RxPort);
		fclose(file);
		printf("COM ports:\nTx:\t\tCOM_%d\nRx:\t\tCOM_%d\n", *TxPort, *RxPort);
	}
	else {
		//default values for Kien's computer
		*TxPort = 6;
		*RxPort = 5;
		printf("No config file found, using defaults (Tx = COM6, Rx = COM5)\n");
		saveComPorts(*TxPort, *RxPort);
	}
}
