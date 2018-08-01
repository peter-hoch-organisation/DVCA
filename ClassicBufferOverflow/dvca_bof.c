#define _WIN32_WINNT 0x501

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define VERSION "1.00"
#define TYPE "Classic Buffer Overflow"
#define DEFAULT_PORT "31331"
#define DEFAULT_BUFFLEN 5000
#define AUTHOR "fokt"
#define GITHUB "github.com/fachrioktavian/DVCA"

DWORD WINAPI ConnHandler(LPVOID CSocket);
void ShowHeader();
void ShowWarning();


int main(int argc, char *argv[]) {
	ShowHeader();
	ShowWarning();
	LoadLib();
	
	WSADATA WsaD;
	SOCKET LSocket = INVALID_SOCKET;
	SOCKET CSocket = INVALID_SOCKET;
	struct addrinfo *res = NULL, hints;
	int Res;
	struct sockaddr_in CAddr;
	int CAddrL = sizeof(CAddr);
	
	Res = WSAStartup(MAKEWORD(2,2), &WsaD);
	if (Res != 0) {
		printf("[DVCA] WSAStartup error %d\n", Res);
		return 1;
	}
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	
	Res = getaddrinfo(NULL, DEFAULT_PORT, &hints, &res);
	if (Res != 0) {
		printf("[DVCA] Getaddrinfo error %d\n", Res);
		WSACleanup();
		return 1;
	}
	
	LSocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (LSocket == INVALID_SOCKET) {
		printf("Socket error %ld\n", WSAGetLastError());
		freeaddrinfo(res);
		WSACleanup();
		return 1;
	}
	
	Res = bind(LSocket, res->ai_addr, (int)res->ai_addrlen);
	if (Res == SOCKET_ERROR) {
		printf("Bind error %d\n", WSAGetLastError());
		closesocket(LSocket);
		WSACleanup();
		return 1;
	}
	
	freeaddrinfo(res);
	
	Res = listen(LSocket, SOMAXCONN);
	if (Res == SOCKET_ERROR) {
		printf("Listen error %d\n", WSAGetLastError());
		closesocket(LSocket);
		WSACleanup();
		return 1;
	}
	printf("[DVCA] Listen on port %s\n", DEFAULT_PORT);
	
	while (LSocket) {	
		CSocket = accept(LSocket, (SOCKADDR*)&CAddr, &CAddrL);
		if (CSocket == INVALID_SOCKET) {
			printf("Accept error %d\n", WSAGetLastError());
			closesocket(LSocket);
			WSACleanup();
			return 1;
		}
		
		printf("[DVCA] Client %s:%u is connected\n", inet_ntoa(CAddr.sin_addr), htons(CAddr.sin_port));
		CreateThread(0, 0, ConnHandler, (LPVOID)CSocket, 0, 0);
		
	}
	
	closesocket(LSocket);
	WSACleanup();
	
	return 0;
}

void ShowWarning() {
	printf("WARNING!\n");
	printf("This software is vulnerable, Be careful while using it on live system\n\n");
}

void ShowHeader() {
	printf("//////////////////////////////////////////\n");
	printf("/ Damn Vulnerability CWin32 Apps         /\n");
	printf("/ Version: %s                          /\n", VERSION);
	printf("/ Type: %s          /\n", TYPE);
	printf("/ Author: %s                           /\n", AUTHOR);
	printf("/ Github: %s /\n", GITHUB);
	printf("//////////////////////////////////////////\n\n");
}

DWORD WINAPI ConnHandler(LPVOID CSocket) {
	int Res, SendRes, i, k;
	int RecvBuffLen = DEFAULT_BUFFLEN;
	char *RecvBuff = malloc(DEFAULT_BUFFLEN);
	memset(RecvBuff, 0, DEFAULT_BUFFLEN);
	char Empty[1000];
	memset(Empty, 0, 1000);
	
	SOCKET C = (SOCKET)CSocket;
	const char *Wc = "DVCA v1.00 || Classic Buffer Overflow\nSend me anything :)\n\n";
	SendRes = send(C, Wc, strlen(Wc), 0);
	
	if (SendRes == SOCKET_ERROR) {
		printf("[DVCA] Error %d\n", SOCKET_ERROR);
		closesocket(C);
		return 1;
	}
	
	while(C) {
		Res = recv(C, RecvBuff, RecvBuffLen, 0);
		if(Res > 0) {
			const char *Txt = "[DVCA] Buffers are received\n";
			SendRes = send(C, Txt, strlen(Txt), 0);
			char *TmpBuff = malloc(3000);
			memset(TmpBuff, 0, 3000);
			strncpy(TmpBuff, RecvBuff, 3000);
			Process(TmpBuff);
			memset(TmpBuff, 0, 3000);
		} else if (Res ==0) {
			printf("[DVCA] Connection is close\n");
			closesocket(C);
			return 0;
		} else {
			printf("[DVCA] Receive failed, error %d\n", WSAGetLastError());
			closesocket(C);
			return 1;
		}
	}
}