# C++ IM(Instant Message) System

## Intro
This projects build a IM(Instant Message) system, which is a simple online chatting system.<br>
* Coded with C++.
* Based on TCP protocol.
* Win 2.2 dll.
* Libs:
    EasyX (<graphics.h> included)
* If you encountered a problem with outtextyx when compiling, please use multibyte character set.

A screen shot of 3 clients with the server.<br>
Each client has a window to show chatting history.
![ScreenShot](https://github.com/IorekHan/Cpp-IM-Instant_Message-_System/blob/main/111.png)

## Detailed Introduction
The MT_Client and MT_Server codes allows the server to listen to multiple clients, receiving and boardcasting messages to all clients.

### 1. Set max client number

Please change this line of code in MT_Server.cpp to set the max volume of clients. Default is 128.
```C++
constexpr auto MUL = 128;
```

### 2. Set server IP address

Please change lines in both MT_Client.cpp and MT_Server.cpp to set the server address. This should be where you run the server.exe. If you are running of your PC, you can use following codes in cmd to see your IP.
```cmd
ipconfig
```
Change IP in codes:
```C++
sAddr.sin_addr.S_un.S_addr = inet_addr("SERVER IP ADDRESS");
```

### 3. Set up connection socket number
Please change lines in both MT_Client.cpp and MT_Server.cpp to set the socket number, a number between 8000 - 10000 is recommended.
```C++
sAddr.sin_port = htons(9999);
```

### 4. Set up message max length
Please change lines in both MT_Client.cpp and MT_Server.cpp to set the message max length, default is 256 bytes.
```C++
char buff[256];
char tmp[256];
```

### 5. Build the program
I build .exe with Visual Studio 2022. And you can run the server and multiple clients to test the system.
