/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 3 ***
				
				What changes were made for this week?

				- reworked newText() and playText() to run continuously
				- implemented audio messaging system

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