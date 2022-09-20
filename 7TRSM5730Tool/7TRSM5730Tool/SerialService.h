#pragma once
#include   <afxmt.h> 

#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2

#define MAXRXBUF            1024
#define MAXTXBUF            1024
#define MAXSERIALNUM        10

typedef void (WINAPI *PFCALLBACK)(unsigned char data,int id,void* puser);

typedef struct
{
	unsigned int portnum;             //端口号
	unsigned int baud;                //波特率
	unsigned int parity;              //校验位
	unsigned int databits;            //数据位
	unsigned int stopbits;            //停止位
	
	HANDLE	     hcomm;               //串口句柄
	BOOL         status;              //串口当前状态
	BOOL         usethread;           //串口是否使用线程接收
	BOOL         threadstatus;        //串口线程状态
	//BOOL         closethread;         //关闭串口线程
	CWinThread*  pthread;             //串口接收线程指针
	PFCALLBACK   ProcessRecData;      //串口接收回调函数指针
	CEvent       closethreadflag;

	void*       puserdata;

	HANDLE      hwritemutex;
}serial_t;

void InitSerialService();
int  OpenSerialPort(UINT  portnr,UINT  baud,UINT parity,UINT databits,UINT stopbits,PFCALLBACK receiveprocess,void *puser);
int  CloseSerialPort(int id);
long TransmitDataToPort(int id,LPBYTE pbuf,DWORD writelen,DWORD dwWait);
long ReceiveDataFromPort(int id,LPBYTE pbuf,DWORD readlen,DWORD dwWait);
void ClearRxBuf(int id);
BOOL GetSerialState(int id);