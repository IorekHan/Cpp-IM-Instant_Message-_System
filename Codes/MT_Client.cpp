#include <stdio.h>
#include <winsock.h>
#include <graphics.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

SOCKET serverSocket;

void communication();

int main() {
	initgraph(300, 500, 1);

	//Protocol version info
	

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	// Confirm that the WinSock DLL supports 2.2.
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("Could not find a usable version of Winsock.dll: %d\n", GetLastError());
		WSACleanup();
		return -1;
	}
	else {
		printf("The Winsock 2.2 dll was found okay\n");
	}


	//Set up socket
	//TCP
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket) {
		WSACleanup();
		printf("Socket set up error: %d\n", GetLastError());
		return -1;
	}
	else {
		printf("Socket set up okay\n");
	}


	//Determine server protocol address cluster

	SOCKADDR_IN sAddr = { 0 };
	sAddr.sin_family = AF_INET; // 1st para
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.20.15.239"); // IP address, check with ipconfig in cmd
	sAddr.sin_port = htons(9999); // socket number


	// Connect to server
	int r = connect(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (-1 == r) {
		closesocket(serverSocket);
		WSACleanup();
		printf("Connection to server error: %d\n", GetLastError());
		return -1;
	}
	else {
		printf("Connection to server okay\n");
	}

	// Concurrent for input
	CreateThread(NULL, NULL,
		(LPTHREAD_START_ROUTINE)communication,
		NULL, NULL, NULL);
	
	// Receive boardcast
	char recvBuff[256];
	int n = 0;
	while (1) {
		r = recv(serverSocket, recvBuff, 255, NULL);
		if (r > 0) {
			recvBuff[r] = 0;
			settextcolor(WHITE);
			outtextxy(3, n * 20, recvBuff);
			n++;
		}
	}


	//Shut down socket
	closesocket(serverSocket);


	//Clear protocol version info
	WSACleanup();

	return 0;

}

void communication() {
	// Communication
	// Read user input and send to the server
	char buff[256];
	while (1) {
		printf("Please input your msg\n");
		cin >> buff;

		send(serverSocket, buff, strlen(buff), NULL);
		
	}
}