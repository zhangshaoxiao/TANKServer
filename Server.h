#include "Winsock2.h"  
#ifndef Server_H
#define Server_H
#define player 4
class Room        //��Ϸ����
{
public:
	SOCKADDR_IN addr_Srv;
	SOCKET socServer;
	char recvBuf[100];  //�յ���Ϣ�Ļ�����
	int client_num;         //��Ϸ����
	SOCKADDR_IN addr_Clt[player];  //ÿ����ҵĵ�ַ
	Room();                  //���캯��
	//int CheckRe(SOCKADDR_IN addr);           //�ж��ظ�IP
	//int SendMe();            //������Ϣ
	int Rece_SendMe();             //������Ϣ��ת��
};

#endif