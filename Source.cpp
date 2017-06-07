#pragma once  
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers  

#include <stdio.h>  
#include <tchar.h>  
#include<iostream>
#include <vector>
#include "stdio.h"  
#include "Winsock2.h"  
#include "iostream"  
#include "string"  
#include<pthread.h>
#include"Server.h"
#pragma comment(lib,"ws2_32.lib")
//should add the under line if not add lib in the project.(appendicular dependent)  
//#pragma comment(lib, "ws2_32.lib")  
using namespace std;
#define MAX 1024
//define host IP and usable port.  
#define HOST_IP 127.0.0.1  
#define HOST_PORT 8080  
//#import "c:\Program Files\Common Files\System\ADO\msado15.dll"  no_namespace rename("EOF", "EndOfFile") 
//置入文件，连接数据库需要用到
int main()
{
	cout << "服务器成功启动" << endl;
	Room GameRoom;
	
		GameRoom.Rece_SendMe();
	
	
	cin.get();
	return 0;
}