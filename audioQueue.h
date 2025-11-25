/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Audio queue implementation file

==========================================================================================================================
*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGES 10 //max number of queued messages, 10 for now
#define MAX_FILENAME 64 //maximum file name size

typedef enum {
    PAYLOAD_NONE = 0,
    PAYLOAD_AUDIO = 1,
    PAYLOAD_TEXT = 2
} PayloadType;

typedef struct item Item;
typedef struct node Node;
typedef Node* link;

struct item {
    PayloadType type;              // audio or text
    short* buffer;                 // pointer to audio samples
    long size;                     // number of samples
    char* text;                    // pointer to text string
    char filename[MAX_FILENAME];   // message label / filename
};

struct node {
    Item Data;                     // holds message info
    link pNext;                    // pointer to next node
};

//global vars
extern link front;
extern link rear;
extern int messageCount;

//function definitions
void initQueue();
int isQueueEmpty();
void enqueueAudio(short* buf, long size, const char* name);
link deQueue();
void clearQueue();
link peekQueue();

void enqueueText(const char* msg, const char* label);