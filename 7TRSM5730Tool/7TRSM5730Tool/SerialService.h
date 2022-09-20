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
	unsigned int portnum;             //�˿ں�
	unsigned int baud;                //������
	unsigned int parity;              //У��λ
	unsigned int databits;            //����λ
	unsigned int stopbits;            //ֹͣλ
	
	HANDLE	     hcomm;               //���ھ��
	BOOL         status;              //���ڵ�ǰ״̬
	BOOL         usethread;           //�����Ƿ�ʹ���߳̽���
	BOOL         threadstatus;        //�����߳�״̬
	//BOOL         closethread;         //�رմ����߳�
	CWinThread*  pthread;             //���ڽ����߳�ָ��
	PFCALLBACK   ProcessRecData;      //���ڽ��ջص�����ָ��
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