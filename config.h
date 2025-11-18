/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 6 2025
Comments:		Projects III - Coded Messaging System

				Config header file

==========================================================================================================================
*/
#pragma once

#define CONFIG_FILE "config.txt"

typedef struct Config {
	int COM_TX;
	int COM_RX;
	int HEADERS;
	int ENCRYPT;
	int COMPRESS;
	int SID;
	int RID;
	int PRIORITY;
	int MSGTYPE;
};

extern Config cfg;

int loadConfig(const char* filename, Config* cfg);
int saveConfig(const char* filename, const Config* cfg);
void setDefaultConfig(Config* cfg);
void displayFullConfiguration();


/*
void saveConfig(int tx, int rx, int headers, int encrypt, int compress, int sid);
void loadConfig(int* tx, int* rx, int* headers, int* encrypt, int* compress, int* sid);
*/