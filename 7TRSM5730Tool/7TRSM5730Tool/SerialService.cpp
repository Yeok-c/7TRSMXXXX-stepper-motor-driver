//���ڷ������
#include "stdafx.h"
#include "SerialService.h"

static serial_t    serialport[MAXSERIALNUM];
CRITICAL_SECTION   criticalsync;
static DWORD totalindex = 0;
UINT ReceiveData_Thread(LPVOID pParam);
/*===============================================================================
��������:InitSerialService();
��������:��ʼ�����ڲ���
��ڲ���:��
���ڲ���:��
===============================================================================*/
void InitSerialService()
{
	for(int i = 0;i < MAXSERIALNUM;i++)
	{
		serialport[i].portnum        = 0;
		serialport[i].baud           = 9600;
		serialport[i].databits       = 8;
		serialport[i].stopbits       = ONESTOPBIT;
		serialport[i].parity         = NOPARITY;
		serialport[i].hcomm          = NULL;
		serialport[i].status         = FALSE;
		serialport[i].usethread      = FALSE;
		serialport[i].threadstatus   = FALSE;
		//serialport[i].closethread    = FALSE;
		serialport[i].ProcessRecData = NULL;
		serialport[i].closethreadflag.ResetEvent();
		 serialport[i].puserdata = NULL;
	}
}

/*===============================================================================
��������:ClearRxBuf();
��������:��ս��ջ�����
��ڲ���:id -- ���ڹ���ID��(0~MAXSERIALNUM-1)
���ڲ���:��
===============================================================================*/
void ClearRxBuf(int id)
{
	HANDLE hComm;
	hComm = serialport[id].hcomm;
	if(hComm != INVALID_HANDLE_VALUE)
	{
		PurgeComm(hComm, PURGE_RXABORT|PURGE_RXCLEAR);
	}
}

/*===============================================================================
��������:BeginSerialThread();
��������:�������ڽ����߳�
��ڲ���:id -- ���ڹ���ID��(0~MAXSERIALNUM-1)
���ڲ���:��
===============================================================================*/
static void BeginSerialThread(int id)
{
	serialport[id].pthread = AfxBeginThread(ReceiveData_Thread,(LPVOID)id);
}

/*===============================================================================
��������:EndSerialThread();
��������:�رմ��ڽ����߳�
��ڲ���:id -- ���ڹ���ID��(0~MAXSERIALNUM-1)
���ڲ���:��
===============================================================================*/
static void EndSerialThread(int id)
{
	int clostcnt = 0;
	
	serialport[id].closethreadflag.SetEvent();
	do 
	{
		clostcnt++;
		Sleep(100);
	} while((serialport[id].threadstatus) && (clostcnt < 50));
	
	//����رճ�ʱ����ǿ�йر��߳�
	if(clostcnt >= 50)
	{
		TerminateThread(serialport[id].pthread->m_hThread,0);
	}
}

/*===============================================================================
��������:OpenSerialPort();
��������:�򿪴���
��ڲ���:portnr         -- ���ں�
		 baud           -- ������
		 parity         -- У��λ(NOPARITY,ODDPARITY,EVENPARITY)
		 databits       -- ����λ(5,6,7,8)
		 stopbits       -- ֹͣλ(ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS)
		 receiveprocess -- ���ջص�����(���ΪNULL���򲻿��������߳�)
���ڲ���:-1             -- �˿ڴ�ʧ��
		 -2             -- �򿪵Ĵ�����Ŀ���������Ŀ
		 ����           -- ���ڹ���ID��(0~MAXSERIALNUM-1)
===============================================================================*/
int OpenSerialPort(UINT  portnr,		                          
				   UINT  baud,			                         
				   UINT  parity,		                         
				   UINT  databits,		                          
				   UINT  stopbits,                                
				   PFCALLBACK   receiveprocess,
				   void *puser)	                       // ���ջص�������	 
					
{
	HANDLE hComm;
	WCHAR szPort[50];
	COMMTIMEOUTS	  commTimeouts;                                    //��ʱʱ��
	DCB				  dcb;                                             //���ڲ�������
	
	//sprintf_s(szPort,"COM%d", portnr);                                 //��ʽ������
	wsprintfW(szPort,_T("\\\\.\\COM%d"), portnr);
	int id;                                                            //���ҿ��ô��ڹ���ID��
	for(id = 0;id < MAXSERIALNUM;id++)
	{
		if(serialport[id].status == FALSE) break;
	}
	if(id == MAXSERIALNUM) return -2;
	
	//Ϊ�����Ĵ��ڻ�ȡһ�����
	hComm =CreateFile(szPort,                                     //COM��
					  GENERIC_READ|GENERIC_WRITE,                      //�������д
					  0,                                               //��ռ��ʽ
					  NULL,                                            //��ȫ������,Ĭ��NULL
					  OPEN_EXISTING,                                   //�򿪶����Ǵ���,�ò���������ΪOPEN_EXISTING
					  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,      //�ص���ʽ
					  NULL);
	if(hComm == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	
	InitializeCriticalSection(&criticalsync);                          //��ʼ���ٽ���
	
	commTimeouts.ReadIntervalTimeout = MAXWORD;                           //���ó�ʱ����
	commTimeouts.ReadTotalTimeoutMultiplier = 10;
	commTimeouts.ReadTotalTimeoutConstant = 100;
	commTimeouts.WriteTotalTimeoutMultiplier = 100;
	commTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(hComm,&commTimeouts);

	SetCommMask(hComm,EV_RXCHAR);                                       //���ô��ڽ�����Ϣ
	
	GetCommState(hComm,&dcb);                                           //���ô��ڲ���

	dcb.BaudRate = baud;                                                //������Ϊ9600
	dcb.ByteSize = databits;                                            //ÿ���ֽ���8λ
	dcb.Parity   = parity;                                              //����żУ��λ
	dcb.StopBits = stopbits;                                            //ֹͣλ
	SetCommState(hComm,&dcb);

	SetupComm(hComm,MAXRXBUF,MAXTXBUF);                                 //������������������Ĵ�С
	
	//��ջ�����
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	serialport[id].hcomm          = hComm;
	serialport[id].portnum        = portnr;
	serialport[id].baud           = baud;
	serialport[id].databits       = databits;
	serialport[id].stopbits       = stopbits;
	serialport[id].parity         = parity;
	serialport[id].status         = TRUE;
	serialport[id].puserdata      = puser;

	//�Ƿ�ʹ���̷߳�ʽ��������
	if(receiveprocess != NULL)
	{
		serialport[id].usethread  = TRUE;
		serialport[id].ProcessRecData = receiveprocess;
		BeginSerialThread(id);
	}
	serialport[id].hwritemutex = CreateMutex(NULL, FALSE, NULL);
	
	return id;
}

/*===============================================================================
��������:CloseSerialPort();
��������:�رմ���
��ڲ���:id -- ���ڹ���ID��(0~MAXSERIALNUM-1)
���ڲ���:-1 -- �ر�ʧ��
		 1  -- �رճɹ�
===============================================================================*/
int CloseSerialPort(int id)
{
	if(id < 0 || id >= MAXSERIALNUM) return FALSE;
	if(serialport[id].hcomm != INVALID_HANDLE_VALUE)
	{
		if(serialport[id].usethread == TRUE)
		{
			EndSerialThread(id);
		}
		
		if(serialport[id].status == TRUE)
		{
			BOOL state = CloseHandle(serialport[id].hcomm);

			if(state == TRUE)
			{
				serialport[id].hcomm = NULL;
				serialport[id].status = FALSE;
				return 1;
			}
		}

		CloseHandle(serialport[id].hwritemutex);
	}  
	return -1;
}

/*===============================================================================
��������:TransmitDataToPort();
��������:���ڷ��ͺ���
��ڲ���:id       -- ���ڹ���ID��(0~MAXSERIALNUM-1)
		 pbuf     -- ���������ݵ�ַ
		 writelen -- ���������ݳ���
		 dwWait   -- ���ȴ���ʱ(ms)
���ڲ���:��ʵ�������ݳ���
===============================================================================*/
long TransmitDataToPort(int id,LPBYTE pbuf,DWORD writelen,DWORD dwWait)
{
	HANDLE hComm;
	DWORD totalwritenum = 0;
	DWORD dwErrorFlags;
	COMSTAT ComStat;
	OVERLAPPED m_osWrite;
	BOOL bWriteStat;
	
	if(id < 0 || id >= MAXSERIALNUM) return FALSE;
	hComm = serialport[id].hcomm;
	if(hComm == NULL) return 0;

	memset(&m_osWrite,0,sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                 //�������¼�
	
	WaitForSingleObject(serialport[id].hwritemutex, INFINITE);
	ClearCommError(hComm,&dwErrorFlags,&ComStat);
	bWriteStat = WriteFile(hComm,pbuf,writelen,&totalwritenum,&m_osWrite);

	if(!bWriteStat)
	{
		DWORD err = GetLastError();

		if(err == ERROR_IO_PENDING)                 
		{
			GetOverlappedResult(hComm,&m_osWrite,&totalwritenum,TRUE);    //������д����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
			
			ReleaseMutex(serialport[id].hwritemutex);
			CloseHandle(m_osWrite.hEvent);
			return totalwritenum;
			
		}
		if(err = ERROR_INVALID_HANDLE)
		{
			AfxMessageBox(_T("��Ч�������鿴�����Ƿ��Ѿ���!"));
		}

		ReleaseMutex(serialport[id].hwritemutex);
		CloseHandle(m_osWrite.hEvent);
		return 0;
	}

	ReleaseMutex(serialport[id].hwritemutex);
	CloseHandle(m_osWrite.hEvent);

	return totalwritenum;
}

/*===============================================================================
��������:ReceiveDataFromPort();
��������:���ڽ��պ���
��ڲ���:id       -- ���ڹ���ID��(0~MAXSERIALNUM-1)
		 pbuf     -- �������ݵ�ַ
		 writelen -- ���������ݳ���
		 dwWait   -- ���ȴ���ʱ(ms)
���ڲ���:��ʵ�������ݳ���
===============================================================================*/
long ReceiveDataFromPort(int id,LPBYTE pbuf,DWORD readlen,DWORD dwWait)
{
	HANDLE hComm;
	DWORD totalreadnum = 0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	OVERLAPPED m_osRead;
	COMMTIMEOUTS timeout;

	if(id < 0 || id >= MAXSERIALNUM) return FALSE;
	hComm = serialport[id].hcomm;	
	if(hComm == NULL) return 0;

	if(readlen <=0) return 0;

	memset(&timeout,0,sizeof(timeout));
	memset(&m_osRead,0,sizeof(OVERLAPPED));

	timeout.ReadIntervalTimeout = 10;
	timeout.ReadTotalTimeoutMultiplier = 1;
	timeout.ReadTotalTimeoutConstant = dwWait;

	SetCommTimeouts(hComm,&timeout);

	m_osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                   //�������¼�

	ClearCommError(hComm,&dwErrorFlags,&ComStat);                          //���ͨ�Ŵ��󲢱��洮�ڵĵ�ǰ״̬��������ڵĴ����־

	BOOL bReadStatus = ReadFile(hComm,pbuf,readlen,&totalreadnum,&m_osRead);

	if(!bReadStatus)                                                       //���ReadFile��������FALSE
	{
		DWORD err = GetLastError();
		if(err == ERROR_IO_PENDING)                                        //GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж����� 	
		{
			WaitForSingleObject(m_osRead.hEvent,dwWait);
			GetOverlappedResult(hComm,&m_osRead,&totalreadnum,TRUE);      //ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ�ȴ�ʱ��
			//�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
			if(totalreadnum == 0) TRACE("û�н��յ��ַ�\r\n");

			//CloseHandle(m_osRead.hEvent);
			//return totalreadnum;
		}

		//CloseHandle(m_osRead.hEvent);
		//return totalreadnum;  
	}

	CloseHandle(m_osRead.hEvent);
	return totalreadnum;
}

/*===============================================================================
��������:ReceiveData_Thread();
��������:���ڽ����߳�
��ڲ���:pParam -- ���ڹ���ID��(0~MAXSERIALNUM-1)
���ڲ���:��
===============================================================================*/
UINT ReceiveData_Thread(LPVOID pParam)
{
	int id;
	HANDLE hComm;
	DWORD totalreadnum = 0;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	OVERLAPPED m_osRead;
	BOOL  bResult = TRUE;
	DWORD Event = 0;
	DWORD dwError = 0;
	DWORD eventindex;
	unsigned char rxbuf[80000];
	HANDLE handarray[2];
	
	id = (int)pParam;
	hComm = serialport[id].hcomm;
   
	memset(&m_osRead,0,sizeof(OVERLAPPED));
 
	m_osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                    //�������¼�
 
	ClearCommError(hComm,&dwErrorFlags,&ComStat);                           //���ͨ�Ŵ��󲢱��洮�ڵĵ�ǰ״̬��������ڵĴ����־
	PurgeComm(hComm, PURGE_RXABORT|PURGE_RXCLEAR);                          //������ջ�����
	
	serialport[id].threadstatus = TRUE;

	handarray[0] = m_osRead.hEvent;
	handarray[1] = serialport[id].closethreadflag.m_hObject;
	while(1)
	{
		bResult = WaitCommEvent(hComm,&Event,&m_osRead);                    //������������¼�
		//if (WaitForSingleObject(m_osRead.hEvent,1000)==WAIT_TIMEOUT) continue;
		eventindex = WaitForMultipleObjects(2,handarray,FALSE,1);
		if(eventindex == WAIT_TIMEOUT) continue;
		else if(eventindex == 1) break;

		if((Event&EV_RXCHAR) == EV_RXCHAR)
		{
			ClearCommError(hComm,&dwErrorFlags,&ComStat);                   //��ȡ���ڻ������е��ֽ���ComStat.cbInQue
			Event = 0;                                                      //�¼���־���㣬����᲻ͣ�Ľ���
	  
			if(ComStat.cbInQue == 0) continue;
					
			BOOL bReadStatus = ReadFile(hComm,rxbuf,ComStat.cbInQue,        //��ȡ�������е����ݣ����ݳ�����ComStat.cbInQueָ��
										&totalreadnum,&m_osRead);
			rxbuf[totalreadnum] = 0;

			EnterCriticalSection(&criticalsync);                            //�����ٽ���,��ֹ���߳�ͬʱ������ͬ�����ݴ�����
																			//��Ϊ���ݴ������ܿ����Ƿǿ����뺯��

			for(DWORD i = 0;i < totalreadnum;i++)                             
			{
				serialport[id].ProcessRecData(rxbuf[i],id,serialport[id].puserdata);                    //������յ���������Ϣ
				totalindex++;
			}	   
			//TRACE("�߳�ID:%d�յ�%s\r\n",id,rxbuf);
			//TRACE("���յ�%d\r\n",totalindex);
			LeaveCriticalSection(&criticalsync);                            //�뿪�ٽ���,������EnterCriticalSectionһ��ʹ��
		}
	}

	ClearCommError(hComm,&dwErrorFlags,&ComStat);
	PurgeComm(hComm, PURGE_RXABORT|PURGE_RXCLEAR);
	serialport[id].threadstatus = FALSE;
	CloseHandle(m_osRead.hEvent);

	TRACE("thread is close\r\n");
	return 0;
}

/*===============================================================================
��������:BOOL GetSerialState(void);
��������:��ȡ���ڵ�ǰ״̬
��ڲ���:��
���ڲ���:FALSE -- δ�򿪣�TRUE -- �����Ѵ�
===============================================================================*/
BOOL GetSerialState(int id)
{
	return serialport[id].status;
}

