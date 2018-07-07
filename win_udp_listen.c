//FILE: win_udp_listen.c
//NAME: Michael Edukonis
//DATE: July 7, 2018
//DESCRIPTION: UDP Listener that will report where data is received from and print the data receieved in tab delimited format
//This is to be used as a standard framework or starting point for any UDP receiving needs on windows platform.


#include <stdio.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <time.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library - this is a system library - can also do
//Settings->linker->Additional dependency : add your library name.

#pragma warning(disable:4996) //localtime function depracated - use this to kill the warning.
#define BUFLEN 512  //Max length of buffer
#define PORT 30000  //The port on which to listen for incoming data


int main()
{
	SOCKET s;
	struct sockaddr_in listener, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	char address[16];
	WSADATA wsa;
	slen = sizeof(si_other);

	//Initialise winsock
	//printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	//printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//Prepare the sockaddr_in structure
	listener.sin_family = AF_INET;
	listener.sin_addr.s_addr = INADDR_ANY;
	listener.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&listener, sizeof(listener)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//keep listening for data
	while (1)
	{
		
		//printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		//receive
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		struct tm *tm2 = localtime(&t);
		char date_text[64];
		char time_text[64];
		strftime(date_text, sizeof(date_text), "%F", tm);
		strftime(time_text, sizeof(time_text), "%R", tm2);

		printf(InetNtop(AF_INET, &si_other.sin_addr, address, sizeof(address)));
		printf("\t %s", date_text);
		printf("\t %s", time_text);
		printf("\t %s \n", buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}