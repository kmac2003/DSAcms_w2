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
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Config file not found! Creating default...\n");
        setDefaultConfig(cfg);
        saveConfig(filename, cfg);
        return -1;
    }

    char line[128];
    int section = 0; // 0 = none, 1 = SETTINGS, 2 = TX-PARAMS

    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters
        line[strcspn(line, "\r\n")] = 0;

        // Skip empty lines or comments
        if (line[0] == '\0' || line[0] == '#')
            continue;

        // Check sections
        if (strcmp(line, "%%--SETTINGS--%%") == 0) {
            section = 1;
            continue;
        }
        if (strcmp(line, "%%--TX-PARAMS--%%") == 0) {
            section = 2;
            continue;
        }

        // Parse lines based on section
		if (section == 1) { // SETTINGS
			int value;
			if (sscanf(line, "COM_TX = %d", &value) == 1) cfg->COM_TX = value;
			else if (sscanf(line, "COM_RX = %d", &value) == 1) cfg->COM_RX = value;
			else if (sscanf(line, "HEADERS = %d", &value) == 1) cfg->HEADERS = value;
			else if (sscanf(line, "ENCRYPT = %d", &value) == 1) cfg->ENCRYPT = value;
			else if (sscanf(line, "COMPRESS = %d", &value) == 1) cfg->COMPRESS = value;
			else if (sscanf(line, "SID = %d", &value) == 1) cfg->SID = value;
		}
		else if (section == 2) { // TX-PARAMS
			int value;
			if (sscanf(line, "RID = %d", &value) == 1) cfg->RID = value;
			else if (sscanf(line, "PRIORITY = %d", &value) == 1) cfg->PRIORITY = value;
			else if (sscanf(line, "MSGTYPE = %d", &value) == 1) cfg->MSGTYPE = value;
		}
    }

    fclose(file);

    printf("\n--- Loaded Config ---\n");
    printf("COM_TX: %d\n", cfg->COM_TX);
    printf("COM_RX: %d\n", cfg->COM_RX);
    printf("HEADERS: %d\n", cfg->HEADERS);
    printf("ENCRYPT: %d\n", cfg->ENCRYPT);
    printf("COMPRESS: %d\n", cfg->COMPRESS);
    printf("SID: %d\n", cfg->SID);
    printf("RID: %d\n", cfg->RID);
    printf("PRIORITY: %d\n", cfg->PRIORITY);
    printf("MSGTYPE: %d\n", cfg->MSGTYPE);

    return 0;
}

//save config from memory
int saveConfig(const char* filename, const Config* cfg) {
	FILE* file = fopen(filename, "w");
	if (!file) {
		printf("Error: Could not save config.\n");
		return -1;
	}

	fprintf(file, "%%--SETTINGS--%%\n");
	fprintf(file, "COM_TX=%d\n", cfg->COM_TX);
	fprintf(file, "COM_RX=%d\n", cfg->COM_RX);
	fprintf(file, "HEADERS=%d\n", cfg->HEADERS);
	fprintf(file, "ENCRYPT=%d\n", cfg->ENCRYPT);
	fprintf(file, "COMPRESS=%d\n", cfg->COMPRESS);
	fprintf(file, "SID=%d\n", cfg->SID);
	fprintf(file, "%%--TX-PARAMS--%%\n");
	fprintf(file, "RID=%d\n", cfg->RID);
	fprintf(file, "PRIORITY=%d\n", cfg->PRIORITY);
	fprintf(file, "MSGTYPE=%d\n", cfg->MSGTYPE);

	fclose(file);
	printf("Config saved successfully.\n");
	return 0;
}

//set or update a parameter
void setDefaultConfig(Config* cfg) {
	cfg->COM_TX = 5;
	cfg->COM_RX = 6;
	cfg->HEADERS = 0;
	cfg->ENCRYPT = 0;
	cfg->COMPRESS = 0;
	cfg->SID = 1;

	cfg->RID = 0;
	cfg->PRIORITY = 0;
	cfg->MSGTYPE = 0;
}

void displayFullConfiguration() {
	system("cls");

	printf("=====================================\n");
	printf("       CURRENT CONFIGURATION\n");
	printf("=====================================\n\n");

	printf("---- SETTINGS ----\n");
	printf("COM_TX:\t\tCOM%d\n", cfg.COM_TX);
	printf("COM_RX:\t\tCOM%d\n", cfg.COM_RX);
	printf("HEADERS:\t%s\n", cfg.HEADERS ? "ON" : "OFF");

	switch (cfg.ENCRYPT) {
	case 1: printf("ENCRYPTION:\tXOR\n"); break;
	case 2: printf("ENCRYPTION:\tVIGENERE\n"); break;
	default: printf("ENCRYPTION:\tOFF\n"); break;
	}

	switch (cfg.COMPRESS) {
	case 1: printf("COMPRESSION:\tHUFFMAN\n"); break;
	case 2: printf("COMPRESSION:\tRLE\n"); break;
	default: printf("COMPRESSION:\tOFF\n"); break;
	}

	printf("SID:\t\t%d\n", cfg.SID);

	printf("\n---- TX PARAMETERS ----\n");
	printf("RID:\t\t%d\n", cfg.RID);
	printf("PRIORITY:\t%d\n", cfg.PRIORITY);

	switch (cfg.MSGTYPE) {
	case 1: printf("MSGTYPE:\tTEXT\n"); break;
	case 2: printf("MSGTYPE:\tAUDIO\n"); break;
	default: printf("MSGTYPE:\tNONE\n"); break;
	}

	printf("\n=====================================\n\n");
}
