#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {

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
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.20.15.250"); // IP address, check with ipconfig in cmd
	sAddr.sin_port = htons(9876); // socket number


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
	

	// Communication
	// Read user input and send to the server
	char buff[256];
	while (1) {
		printf("Please input your msg\n");
		cin >> buff;

		send(serverSocket, buff, strlen(buff), NULL);

	}


	//Shut down socket
	closesocket(serverSocket);


	//Clear protocol version info
	WSACleanup();

	return 0;

}