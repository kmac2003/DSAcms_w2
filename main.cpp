/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 6 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 5 ***
				
				What changes were made for this week?

				Message parameters saved to CONFIG
				- message information used to construct the header is now saved to a config file
				- includes Tx number, Rx numbers, headers on/off, encryption type, compression type, and SID

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

int	main() {

	//initalization
	initQueue();
	InitializePlayback();
	InitializeRecording();

	//run main loop
	runModeLoop();

	//cleanup
	ClosePlayback();
	CloseRecording();
	clearQueue(); //free all queue memory

	return 0;
}