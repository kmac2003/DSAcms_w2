/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 2 ***
				
				What changes were made for this week?


				

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