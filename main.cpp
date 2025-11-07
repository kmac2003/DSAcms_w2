/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           NOV 6 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 3 ***
				
				What changes were made for this week?

				- newText() and playText() now run continuously
						allows for infinite text messaging if user chooses. This option must be selected on the
						transmitter side (Transmitter > New Text > Instant)
				- Audio recording/playback on home station implemented
						currently struggling to get this to send accross the RS232 cable so further testing is required
				- COM port number customization
						COM port numbers are displayed on top level menu, numbers are stored in a config file, users can
						change these numbers in Settings
				- More menus and sub-menus were written

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