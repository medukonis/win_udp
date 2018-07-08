//FILE: win_udp_server.c
//NAME: Michael Edukonis
//DATE: July 8, 2018
//DESCRIPTION: UDP server for windows platform that will send a bit of data by way of udp broadcast. 


#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library - this is a system library - can also do
//Settings->linker->Additional dependency : add your library name.

#pragma warning(disable:4996) //localtime function depracated - use this to kill the warning.
#define PORT 30000  //The port on which to send data
#define ADDRESS "10.0.0.255"


int main()
{
	SOCKET sendSocket;
	struct sockaddr_in sender;
	char address[16];
	WSADATA wsa;

	//Initialise winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//Create a socket
	if ((sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Prepare the sender structure
	sender.sin_family = AF_INET;
	sender.sin_addr.s_addr = inet_addr(ADDRESS);
	sender.sin_port = htons(PORT);

	//send data by udp
	sendto(sendSocket, "ello", 4, 0, (SOCKADDR *) &sender, sizeof(sender));
	
	closesocket(sendSocket);
	WSACleanup();

	return 0;
}