/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Main file

				*** Week 2 ***
				
				What changes were made for this week?

				- RS232 starter file provided in eConestoga was studied and dissected into a home and receiving station
				- Provided text-messaging system was functionalized and implemented
				- roughly 70% of the menu system framework was coded, providing a skeleton of a more advanced program
				  that currently lacks the functionality, but will be written later
				- main was largely modularized and cleaned up
				- new dedicated Rx and Tx files specific to the transmitting and receiving functionalities were created,
				  these will be added to as more functionality is introduced to the program

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