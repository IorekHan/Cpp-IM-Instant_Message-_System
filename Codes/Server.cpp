#include <stdio.h>
#include <winsock.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

int main() {

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
	sAddr.sin_addr.S_un.S_addr = inet_addr("10.20.15.250"); // IP address, check with ipconfig in cmd
	sAddr.sin_port = htons(9876); // socket number
	

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
	

	//Receive connection

	SOCKET clientSocket = accept(serverSocket, NULL, NULL);
	if (SOCKET_ERROR == clientSocket) {
		closesocket(serverSocket);
		WSACleanup();
		printf("Connection crushed: %d\n", GetLastError());
		return -1;
	}
	else {
		printf("Connection okay\n");
	}

	//Communication
	//Receive data from client and display
	char buff[256];
	while (1) {
		r = recv(clientSocket, buff, 255, NULL);
		if (r > 0) {
			buff[r] = 0;
			printf(">>:%s\n", buff);
		}
	}

	//Shut down socket
	closesocket(serverSocket);


	//Clear protocol version info
	WSACleanup();

	return 0;

}