// ��ʾ��ǰʱ��.cpp : ����Ӧ�ó������ڵ㡣
//
#define HAVE_STRUCT_TIMESPEC
#include "stdafx.h"
#include<pthread.h>
#include "tank.h"
#include <windows.h> 
#include <math.h> 
#include <process.h>  
#include <WS2tcpip.h>
#include "Winsock2.h"  

#include<stdio.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib, "pthreadVC2.lib")
#pragma comment(lib,"Msimg32.lib")
#define OK_STR "Conn_Ok"
#define MAX_LOADSTRING 100
#define HOST_IP "127.0.0.1"  
#define HOST_PORT 8080  
#define TIMER1 1
#define Map_Size 20
#define TEXT_SIZE 20  //����������ռ�
#define PLAYER_NUM 4
SOCKET socClient ;
SOCKADDR_IN addrSrv;

int LENGTH;
int HIGTH;  //��ȡ���ڵĳ���
int py = 0;
//enum state{space,hardWall,frahWall,halfWall,glassWall,tankA,tankB,tankC,tankD};  //�����ͼö�����Ͷ�����Щש��,�Լ�̹������
BITMAP			bitmap, bitmap2,BGP;
HDC   hdcMem;
HDC hdcMem2;
HDC hdcBGP;
TCHAR  buff[256] = { NULL };
char test[100];
int cxBlock;
int cyBlock;
Player OldBro[PLAYER_NUM];
Place Map[Map_Size][Map_Size];
static	HBITMAP	hBitmap, hBitmap2, hBitmapBGP;			//λͼ���  ��ʾλͼ

int InitMap(Place Map[Map_Size][Map_Size]);          //��ʼ����ͼ
int InitPlayer(int len,int hei);                    //��ʼ�����λ��
int Analy();                       //�����յ���ָ����������ж�
void *Listen(void  *X);
int Draw(HDC hdc);                 //��ͼ����

// ȫ�ֱ���: 
	
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;
	pthread_t ListenThread;
	pthread_create(&ListenThread, NULL, Listen, 0);
	
    // ����Ϣѭ��: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	

    return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   InitMap( Map);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hWnd, TIMER1, 100, NULL);
	HINSTANCE		hInstance;
	LONG now_time;
	//TCHAR  buff[16] = { NULL };
	//LPCTSTR str="hello";
	
	
	
    switch (message)
    {
	case WM_CREATE:
	{
		hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
		//��ȡ���ڵ�ʵ�����

		hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));	//��λͼ���ص��ڴ���
		hBitmap2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmapBGP = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);               //1��wall
		GetObject(hBitmap2, sizeof(BITMAP), &bitmap2);        //2  frog
		GetObject(hBitmapBGP, sizeof(BITMAP), &BGP);        //����
		return 0;
	}
	case WM_SIZE:
		//COORD info = GetLargestConsoleWindowSize(GetStdHandle(STD_OUTPUT_HANDLE));
		LENGTH = LOWORD(lParam);         //��ȡ���ڴ�С
		HIGTH = HIWORD(lParam);
		cxBlock = LENGTH / Map_Size;
		cyBlock =( HIGTH-TEXT_SIZE) / Map_Size;
		//cxBlock = info.X / (2 * DIVISIONS);
		//cyBlock = info.Y / DIVISIONS;
		InitPlayer(LENGTH, HIGTH);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER1:
		{

			//py++;
			InvalidateRect(hWnd, NULL, TRUE);//�ػ���Ϸ//ÿ��һ�εĴ���
									 //TextOut(hdc,500,50,(LPCTSTR)now_time,sizeof((LPCTSTR)now_time));
											 //SendMessage(hWnd, WM_CALC_DONE, 0, lTime);
		}
		break;
		}
	case WM_KEYDOWN:
		switch (wParam) //���ݰ�����������
		{
		case VK_UP:
			if (OldBro[0].Player_Y > 0 && OldBro[0].Player_Y < HIGTH)
			{
				OldBro[0].Player_Y -= 50;
				sendto(socClient, "1", strlen("1") + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
			}         //������Ϣ�����������ĳ�����������������ˣ���������ת����
			break;
		case VK_DOWN:
			if (OldBro[0].Player_Y > 0 && OldBro[0].Player_Y < HIGTH)
			{
				OldBro[0].Player_Y += 50;
				sendto(socClient, "2", strlen("2") + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
			}
			break;
		case VK_LEFT:
			if (OldBro[0].Player_X >= 0 && OldBro[0].Player_X < LENGTH)
			{
				OldBro[0].Player_X -= 50;
				sendto(socClient, "3", strlen("3") + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
			}
			break;
		case VK_RIGHT:
			if (OldBro[0].Player_X >= 0 && OldBro[0].Player_X < LENGTH)
			{
				OldBro[0].Player_X += 50;
				sendto(socClient, "4", strlen("4") + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
			}
			break;
		//case VK_SPACE:
			//Y = Y - 70;
			break;
		}
		return 0;
	case WM_LBUTTONDOWN:
	{
		
			//_beginthread(thread1,0,NULL);
		
	}
	break;

    case WM_PAINT:
        {    now_time = GetCurrentTime();
            PAINTSTRUCT ps;
			
			wsprintf(buff, TEXT("%d"), py);
            HDC hdc = BeginPaint(hWnd, &ps);
			Draw(hdc);
			TextOut(hdc, 10, 0, buff, 10);
			// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    }
    return (INT_PTR)FALSE;
}
int Draw(HDC hdc)
{       //��ʾ�����ͳ���ͷ��
	hdcMem = CreateCompatibleDC(hdc);//����һ��������hdc�豸�����������hdcMem  ��Ҫ���������ڴ��н�ͼ
	hdcMem2 = CreateCompatibleDC(hdc);
	hdcBGP = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);   //��λͼѡ��hdcMem��
	SelectObject(hdcMem2, hBitmap2);
	SelectObject(hdcBGP, hBitmapBGP);
	BitBlt(hdc, 0, 0, LENGTH, HIGTH, hdcBGP, 0, 0, SRCCOPY);//����bmpλͼ
	for (int i = 0; i < Map_Size;i++)
	{
		for (int j = 0; j < Map_Size; j++)
		{   if(Map[i][j].thing==frahWall)
			BitBlt(hdc, i*cxBlock, j*cyBlock+TEXT_SIZE, cxBlock, cyBlock, hdcMem, 0, 0, SRCCOPY);//����bmpλͼ
		}
	}
	BitBlt(hdc, OldBro[0].Player_X, OldBro[0].Player_Y, 50, 50, hdcMem2, 0, 0, SRCCOPY);//����bmpλͼ
	//BitBlt(hdc, 0, 0, 1000, 1000, hdcBGP, 0, 0, SRCCOPY);//����bmpλͼ
	//BitBlt(hdc, 500, 300, 50, 50, hdcMem, 0, 0, SRCCOPY);//����bmpλͼ
	
	
	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);
	return 0;
}

int InitMap(Place Map[Map_Size][Map_Size])
{
	for (int i=0; i < Map_Size; i++)
	{
		for (int j=0; j < Map_Size; j++)
		{
			if ((i %3==0)&&(j%2==0))
				Map[i][j].thing = frahWall;
			else Map[i][j].thing = space;

		}
	}
	
	return 0;
}

void *Listen(void  *X)                 //��������
{
	int version_a = 1;//low bit  
	int version_b = 1;//high bit  
					  //makeword  
	WORD versionRequest = MAKEWORD(version_a, version_b);
	WSAData wsaData;
	int error;
	error = WSAStartup(versionRequest, &wsaData);

	if (error != 0) {
		printf("ERROR!");

	}
	//check whether the version is 1.1, if not print the error and cleanup wsa?  
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		printf("WRONG WINSOCK VERSION!");
		WSACleanup();

	}
	char requestStr[100];

	//build a sockeet   
	 socClient = socket(AF_INET, SOCK_DGRAM, 0);
	 addrSrv;        // a instance of SOCKADDR_IN, which is used in format of SOCKADDR.  
	inet_pton(AF_INET, "127.0.0.1", (void *)&addrSrv.sin_addr.S_un.S_addr);        //set the host IP  
	addrSrv.sin_family = AF_INET;     //set the protocol family  
	addrSrv.sin_port = htons(HOST_PORT);      //set the port number  

											  // array to store the data that server feedback.  
	char cRecvBuf[4];
	//  int len=sizeof(SOCKADDR);  
	int fromlen = sizeof(SOCKADDR);
	sendto(socClient, OK_STR, strlen(OK_STR) + 1, 0, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	recv(socClient, cRecvBuf, strlen(cRecvBuf) + 1, 0);
	strcpy_s(cRecvBuf, "\0");
	char newBuff[16];
	while (true)
	{

		if (recv(socClient, newBuff, strlen(newBuff) + 1, 0))
		{   /*
			ע�⣬����ʹ��atoi��������ת��������int�����ڵڶ���ת����ʱ�򣬻Ῠ������
			*/


			py = atoi(newBuff);
			if (py == 1)
			{
				OldBro[0].Player_Y -= 50;
			}
			if (py == 2)
			{
				OldBro[0].Player_Y += 50;
			}
			if (py == 3)
			{
				OldBro[0].Player_X -= 50;
			}
			if (py == 4)
			{
				OldBro[0].Player_X += 50;
			}

		}
		//output the cRecvBuf to cmd  
		//printf(" cRecvBuf: %s\n", cRecvBuf) ;
		//	strcpy_s(test, cRecvBuf);

	}
}

int InitPlayer(int len,int hei)
{
	int i = rand() % 4;
	OldBro[0].ID = i;
	if (OldBro[0].ID == 0)
	{
		OldBro[0].Player_X =0;
		OldBro[0].Player_Y = hei;
	}
	if (OldBro[0].ID == 1)
	{
		OldBro[0].Player_X = len;
		OldBro[0].Player_Y = hei;
	}
	if (OldBro[0].ID == 2)
	{
		OldBro[0].Player_X = len;
		OldBro[0].Player_Y = TEXT_SIZE;
	}
	if (OldBro[0].ID == 3)
	{
		OldBro[0].Player_X = 0;
		OldBro[0].Player_Y = TEXT_SIZE;
	}
	return 0;
}