#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

constexpr auto MUL = 128;

// Create multi sockets
SOCKET clientSocket[MUL];

//Record client nums for boardcast
int clientCount;


void communication(int idx);

int main() {

	clientCount = 0;

	//Protocol version info
	//TCP

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
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.20.15.239"); // IP address, check with ipconfig in cmd
	sAddr.sin_port = htons(9999); // socket number


	//Binding

	int r = bind(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (-1 == r) {
		WSACleanup();
		printf("Binding error: %d\n", GetLastError());
		return -1;
	}
	else {
		printf("Binding okay\n");
	}


	//Listen to socket

	r = listen(serverSocket, 10);
	if (-1 == r) {
		WSACleanup();
		printf("Listening error: %d\n", GetLastError());
	}
	else {
		printf("Listening okay\n");
	}



	//Receive multiple connection
	for (int i = 0; i < MUL; i++) {
		clientSocket[i] = accept(serverSocket, NULL, NULL);
		if (SOCKET_ERROR == clientSocket[i]) {
			closesocket(serverSocket);
			WSACleanup();
			printf("Connection crushed: %d\n", GetLastError());
			return -1;
		}
		
		clientCount++;
		printf("Client %d connected\n", clientCount);

		//Concurrent for multiple clients and boardcasting
		CreateThread(NULL, NULL,
			(LPTHREAD_START_ROUTINE)communication,
			(LPVOID)i,
			NULL, NULL);

		
	}

	//Shut down socket
	closesocket(serverSocket);


	//Clear protocol version info
	WSACleanup();

	return 0;

}
	

	


void communication(int idx) {
	//Communication
	//Receive data from other clients and display
	char buff[256];
	int r;
	char tmp[256];
	while (1) {
		
		r = recv(clientSocket[idx], buff, 255, NULL);
		if (r > 0) {

			buff[r] = 0;
			printf(">>:%s\n", buff);

			memset(tmp, 0, 256);
			sprintf_s(tmp, "%d:%s", idx, buff);


			// Boardcast
			for (int i = 0; i < clientCount; i++) {
				send(clientSocket[i], tmp, strlen(tmp), NULL);


			}
		}
	}
}