/*
===================================================== SECTION HEADER =====================================================

Program Name:   EECE72405-25F
Author:			KIEN MACARTNEY
Date:           OCT 30 2025
Comments:		Projects III - Coded Messaging System

				Audio queue implementation file

==========================================================================================================================
*/
#include "RS232Comm.h"
#include "audioQueue.h"
#include "ui.h"

link front = NULL;
link rear = NULL;
int messageCount = 0;

//inits an empty queue
void initQueue(){
	front = rear = NULL;
}

//checks if the queue is empty
int isQueueEmpty(){
	return (front == NULL);
}

//removes the first message node from the queue
link deQueue() {
	if (isQueueEmpty()) {
		printf("Queue is empty — nothing to dequeue.\n");
		return NULL;
	}

	link temp = front;
	front = front->pNext;

	if (front == NULL) {
		rear = NULL;
	}

	temp->pNext = NULL;
	messageCount--;

	printf("Dequeued message. Remaining: %d\n", messageCount);
	return temp; // Caller now responsible for freeing message memory!
}

//frees all queued messages
void clearQueue(){
	while (!isQueueEmpty()) {
		link temp = deQueue();
		if (temp) {
			free(temp->Data.buffer);
			free(temp);
		}
	}
}

//returns a pointer to front node without removing
link peekQueue() {
	if (isQueueEmpty()) {
		printf("Queue is empty\n");
		return NULL;
	}
	return front;
}

//display queue contents
void displayQueue() {
	if (isQueueEmpty()) {
		printf("\nQueue is empty.\n");
		return;
	}

	printf("\n========== FULL QUEUE ==========\n");

	link current = front;
	int index = 1;

	while (current != NULL) {
		printf("\nMessage #%d\n", index++);
		if (current->Data.type == PAYLOAD_TEXT) {
			printf("Type       : Text\n");
			printf("Filename   : %s\n", current->Data.filename);
			if (current->Data.text)
				printf("Message    : %s\n", current->Data.text);
		}
		else if (current->Data.type == PAYLOAD_AUDIO) {
			printf("Type       : Audio\n");
			printf("Filename   : %s\n", current->Data.filename);
			printf("Samples    : %ld\n", current->Data.size);
		}
		else {
			printf("Type       : Unknown\n");
		}

		// Print header info if available
		printf("SID        : %d\n", current->Data.hdr.sid);
		printf("RID        : %d\n", current->Data.hdr.rid);
		printf("Priority   : %d\n", current->Data.hdr.priority);
		printf("PayloadSz  : %ld\n", current->Data.hdr.payloadSize);
		printf("MsgType    : %d\n", current->Data.hdr.payLoadType);
		printf("Encryption : %d\n", current->Data.hdr.encryption);
		printf("Compression: %d\n", current->Data.hdr.compression);

		current = current->pNext;
	}

	printf("\n===============================\n");
}

//queues audio + header 
void enqueueAudioAndHdr(short* buf, long size, const char* name, Header* hdr){
	if (!buf || !hdr) {
		return;
	}

	link newNode = (link)malloc(sizeof(Node));
	if (newNode == NULL) {
		printf("ERROR: malloc failed for queuing new node\n"); //check if malloc worked
		return;
	}

	//copy audio buffer
	newNode->Data.buffer = (short*)malloc(size * sizeof(short));
	if (newNode->Data.buffer == NULL) {
		printf("ERROR: malloc failed for message buffer\n");
		free(newNode);
		return;
	}

	memcpy(newNode->Data.buffer, buf, size * sizeof(short)); //make this queue node hold its own copy of the recorded sound
	newNode->Data.size = size;	

	//copy label
	strncpy_s(newNode->Data.filename, MAX_FILENAME, name, _TRUNCATE); 

	//copy header
	newNode->Data.hdr = *hdr;

	//no text for audio
	newNode->Data.text = NULL;
	newNode->Data.type = PAYLOAD_AUDIO;
	newNode->pNext = NULL;

	//insert into the queue based on priority
	if (!front || hdr->priority < front->Data.hdr.priority) {
		newNode->pNext = front;
		front = newNode;
		if (!rear) {
			rear = newNode;
		}		
	}
	else {
		link current = front;
		while (current->pNext && current->pNext->Data.hdr.priority <= hdr->priority) {
			current = current->pNext;
		}
		newNode->pNext = current->pNext;
		current->pNext = newNode;
		if (!newNode->pNext) {
			rear = newNode;
		}
	}
	messageCount++;
	printf("Audio queued with priority: %d. Queue size: %d\n", hdr->priority, messageCount);
}

//queues text + header 
void enqueueTextAndHdr(const char* msg, const char* label, Header *header) {
	if (!msg) return;

	link newNode = (link)malloc(sizeof(Node));
	if (!newNode) {
		printf("Error: Failed to allocate memory for queue node.\n");
		return;
	}

	//fill node data
	newNode->Data.type = PAYLOAD_TEXT;
	newNode->Data.buffer = NULL;
	newNode->Data.size = 0;
	newNode->Data.hdr = *header;

	// Copy text string
	size_t len = strlen(msg);
	newNode->Data.text = (char*)malloc(len + 1);
	if (!newNode->Data.text) {
		printf("Error: Failed to allocate memory for text.\n");
		free(newNode);
		return;
	}
	strcpy_s(newNode->Data.text, len + 1, msg);

	// Copy label
	strncpy_s(newNode->Data.filename, MAX_FILENAME, label, _TRUNCATE);

	// No audio for text, set buffer and size to 0
	newNode->Data.buffer = NULL;
	newNode->Data.size = 0;

	newNode->pNext = NULL;

	// priority sorting insert
	if (isQueueEmpty() || header->priority < front->Data.hdr.priority) {
		newNode->pNext = front;
		front = newNode;
		if (!rear) {
			rear = newNode;
		}
	}
	else {
		link current = front;
		while (current->pNext && current->pNext->Data.hdr.priority <= header->priority) {
			current = current->pNext;
		}
		newNode->pNext = current->pNext;
		current->pNext = newNode;
		if (current == rear) {
			rear = newNode;
		}
	}
	messageCount++;
	printf("Text queued with piority: %d. Queue size: %d\n", header->priority, messageCount);
}


