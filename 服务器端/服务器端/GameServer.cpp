#include"Server.h"
#include <stdio.h>  
#include <tchar.h>  
#include<iostream>
#include <vector>
#include "stdio.h"  
#include "Winsock2.h"  
#include "iostream"  
#include "string"  
#pragma comment(lib,"ws2_32.lib")

#define MAX 1024
//define host IP and usable port.  
#define HOST_IP 127.0.0.1  
#define HOST_PORT 8080  
#define OK_STR "Conn_Ok"
#define THANKS "你已经成功连接服务器"
int LENGTH;    //窗口长度
int HIGTH;   //窗口宽度
Room::Room()
{
	int version_a = 1;//low bit  
	int version_b = 1;//high bit  
	WORD versionRequest = MAKEWORD(version_a, version_b);
	WSAData wsaData;
	int err;
	//wsa startup  
	err = WSAStartup(versionRequest, &wsaData);

	if (err != 0) {
		printf("ERROR!");
		
	}
	//check whether the version is 1.1, if not print the error and cleanup wsa?  
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		printf("WRONG WINSOCK VERSION!");
		WSACleanup();
		
	}
	 client_num = 0;
	 socServer = socket(AF_INET, SOCK_DGRAM, 0);
	//infomation of address, always NOT being operated directly.  
	
	//Struct sin_addr is  used to defind IP address,  it's a property of addr_in.  
	//It's nest three structs as S_un_b, S_un_w and S-un.(union)  
	addr_Srv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//set protocol family  
	addr_Srv.sin_family = AF_INET;
	//set host port  
	addr_Srv.sin_port = htons(HOST_PORT);

	//bind socket to  the host  
	bind(socServer, (SOCKADDR*)&addr_Srv, sizeof(SOCKADDR));
	printf("test\n");
}


int Room::Rece_SendMe()
{
	SOCKADDR_IN in_addr;        //接入地址
	int fromlen = sizeof(SOCKADDR);
	while (client_num < 2)
	{
		recvfrom(socServer, recvBuf, 100, 0, (SOCKADDR*)&in_addr, &fromlen);        //读取消息到缓冲区
		//if (strcmp(recvBuf, OK_STR) == 0)                 //收到链接成功的消息,写入玩家地址
		printf("buf: %s\n", recvBuf);
		//	this->addr_Clt[client_num] = in_addr;
		if (strcmp(recvBuf, OK_STR) == 0)
		{
			addr_Clt[client_num] = in_addr;
			client_num++;
		}
		sendto(socServer, THANKS, strlen(THANKS) + 1, 0, (SOCKADDR*)&in_addr, sizeof(SOCKADDR));
		
	}
	while (true)
	{
		recvfrom(socServer, recvBuf, 100, 0, (SOCKADDR*)&in_addr, &fromlen);
		for (int i = 0; i < player; i++)
		{
			sendto(socServer, recvBuf, strlen(recvBuf) + 1, 0, (SOCKADDR*)&addr_Clt[i], sizeof(SOCKADDR));//对client转发信息
		}
	}
	
	return 0;
}