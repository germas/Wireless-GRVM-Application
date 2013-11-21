
/*-----------------------------------------------------------------------------
--  SOURCE FILE:    Packet.cpp
--
--  PROGRAM:        Wireless Protocol (GRVM)
--
--  FUNCTIONS:      
--                              
--  
--
--  DATE:           
--
--  REVISIONS:      ...
--
--  DESIGNER:       
--
--  PROGRAMMER:     
--
--  NOTES:
--
-----------------------------------------------------------------------------*/

#include "Packet.h"

CHAR* packetize(FILE* bufferWithFile, int sentPacketCounter)
{
	char data[PACKET_BYTES_DATA];
	char packet[PACKET_BYTES_TOTAL];
	size_t ndx;

	//1020 x sentPacketCounter = startingLocation
	int StartLoc = PACKET_BYTES_DATA * sentPacketCounter;

	// Go to the begining of the line
	if(fseek(bufferWithFile, StartLoc, SEEK_SET) != 0)
	{
		fprintf(stderr, "%s", "Unable to seek to the next line..");
		//return
	}

	// Read 1020 chars from the file buffer, starting at startingLocation into packet string
	if(fgets(data, PACKET_BYTES_DATA, bufferWithFile) == NULL)
	{
		fprintf(stderr, "%s", "Unable to read the next line..");
		//return 
	}

	// If we encounter eof
	for(ndx = 0; ndx < PACKET_BYTES_DATA; ndx++)
	{
		if(data[ndx] == EOF)
			break;
	}

	// Pad remains Bytes with null
	while(ndx < PACKET_BYTES_DATA)
	{
		data[ndx] = '\0';
	}

	// Add control bytes to the packet
	packet[1] = (sentPacketCounter % 2 == 0) ? DC1 : DC2;

	// Add data bytes to the packet
	packet[2] = data[PACKET_BYTES_DATA];
	
	// Add the trailer bytes to the packet (CRC)
	//packet[PACKET_BYTES_DATA] = ;

	return packet;
}


BOOL PacketCheck(HWND hwnd, char packet[1024], int *waitForType)
{
    //switch (char[1])
	switch(packet[1])
	{
	case ENQ:
		//send(ACK);

		//Set "what we're waiting for" flag to PACKET_DC1
		*waitForType = DC1;
	return TRUE;
	case DC1:
		//if we're waiting for a DC2 packet
		if(*waitForType == DC2)
		{
			//send (NAK);
			break;
		}

		if (!ErrorCheck(packet[1022], packet[1023]))
		{
			//send(NAK);
			break;
		}
	
		send (ACK);
		Display(hwnd);//read the remaining 1020 characters 
	return TRUE;
	
			
	case DC2:
		// if we're waiting for a DC1 packet
		if (*waitForType == DC1)
		{
			send (NAK);
			break;
		}
		if (!ErrorCheck(char[1022], char[1023]))
		{
			sendControlPacket (NAK);
			break;
		}
		send (ACK);
		Display();//read the remaining 1020 characters 
	return TRUE;
		
	case NAK:

		//Set "What we're waiting for" flag to ACK
		*waitForType = ACK;
		send (previous packet); //need a way to keep that
	break;
		
	case EOT:
		// GO back to IDLE state
		*waitForType = ENQ;
	break;
    }
}