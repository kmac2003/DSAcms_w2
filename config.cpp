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
#include "settings.h"
#include "testing.h"

Config cfg;

//load config into memory
int loadConfig(const char* filename, Config* cfg) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Config not found. Applying defaults...\n");
        setDefaultConfig(cfg);
        saveConfig(filename, cfg); // Write a fresh file
        return 0;
    }

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '%' || line[0] == '\n')
            continue;

        char key[32];
        int value;
        if (sscanf(line, "%31[^=]=%d", key, &value) == 2) {

            if (strcmp(key, "COM_TX") == 0)     cfg->COM_TX = value;
            else if (strcmp(key, "COM_RX") == 0)     cfg->COM_RX = value;
            else if (strcmp(key, "HEADERS") == 0)    cfg->HEADERS = value;
            else if (strcmp(key, "ERR_DTCT") == 0)     cfg->ERR_DTCT = value;
            else if (strcmp(key, "ENCRYPT") == 0)    cfg->ENCRYPT = value;
            else if (strcmp(key, "COMPRESS") == 0)   cfg->COMPRESS = value;
            else if (strcmp(key, "SID") == 0)        cfg->SID = value;
            else if (strcmp(key, "RID") == 0)        cfg->RID = value;
            else if (strcmp(key, "PRIORITY") == 0)   cfg->PRIORITY = value;
            else if (strcmp(key, "MSGTYPE") == 0)    cfg->MSGTYPE = value;
        }
    }

    fclose(fp);
    return 1;
}

//save config from memory
int saveConfig(const char* filename, const Config* cfg) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not save config!\n");
        return 0;
    }

    fprintf(fp, "%%--SETTINGS--%%\n");
    fprintf(fp, "COM_TX=%d\n", cfg->COM_TX);
    fprintf(fp, "COM_RX=%d\n", cfg->COM_RX);
    fprintf(fp, "HEADERS=%d\n", cfg->HEADERS);
    fprintf(fp, "ERR_DTCT=%d\n", cfg->ERR_DTCT);
    fprintf(fp, "ENCRYPT=%d\n", cfg->ENCRYPT);
    fprintf(fp, "COMPRESS=%d\n", cfg->COMPRESS);
    fprintf(fp, "SID=%d\n", cfg->SID);
    fprintf(fp, "%%--TX-PARAMS--%%\n");
    fprintf(fp, "RID=%d\n", cfg->RID);
    fprintf(fp, "PRIORITY=%d\n", cfg->PRIORITY);
    fprintf(fp, "MSGTYPE=%d\n", cfg->MSGTYPE);

    fclose(fp);
    return 1;
}

//set or update a parameter
void setDefaultConfig(Config* cfg) {
	cfg->COM_TX = 4;
	cfg->COM_RX = 6;
	cfg->HEADERS = 0;
	cfg->ENCRYPT = 0;
    cfg->ERR_DTCT = 0;
	cfg->COMPRESS = 0;
	cfg->SID = 1;

	cfg->RID = 0;
	cfg->PRIORITY = 0;
	cfg->MSGTYPE = 0;
}

void displayFullConfiguration() {
    printf("=== SYSTEM SETTINGS ===\n");
    printf("Tx\t\tCOM%d\n", cfg.COM_TX);
    printf("Rx\t\tCOM%d\n", cfg.COM_RX);
    if (cfg.HEADERS == ON) {
        printf("HEADERS\t\tON\n");
    }
    else {
        printf("HEADERS\t\tOFF\n");
    }

    if (cfg.ERR_DTCT == ON) {
        printf("ERR DETECT\tON\n");
    }
    else {
        printf("ERR DETECT\tOFF\n");
    }

    if (cfg.ENCRYPT == XOR) { 
        printf("ENCRYPT\t\tXOR\n");
    }
    else if (cfg.ENCRYPT == VIGENERE) {
        printf("ENCRYPT\t\tVIGENERE\n");
    }
    else {
        printf("ENCRYPT\t\tOFF\n");
    }

    if (cfg.COMPRESS == HUFFMAN) {
        printf("COMPRESS\tHUFFMAN\n");
    }
    else if (cfg.COMPRESS == RLE) {
        printf("COMPRESS\tRLE\n");
    }
    else {
        printf("COMPRESS\tOFF\n");
    }
    printf("SID\t\t%d\n", cfg.SID);
    printf("\n=== Tx HEADER SETTINGS ===\n");
    printf("RID\t\t%d\n", cfg.RID);
    printf("PRIORITY\t%d\n", cfg.PRIORITY);

    if (cfg.MSGTYPE == TEXT) {
        printf("TYPE\t\tTEXT\n");
    }
    else if (cfg.MSGTYPE == AUDIO) {
        printf("TYPE\t\tAUDIO\n");
    }
}
