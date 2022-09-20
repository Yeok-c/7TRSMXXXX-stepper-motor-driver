//串口服务程序
#include "stdafx.h"
#include "SerialService.h"

static serial_t    serialport[MAXSERIALNUM];
CRITICAL_SECTION   criticalsync;
static DWORD totalindex = 0;
UINT ReceiveData_Thread(LPVOID pParam);
/*===============================================================================
函数名称:InitSerialService();
函数功能:初始化串口参数
入口参数:无
出口参数:无
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
函数名称:ClearRxBuf();
函数功能:清空接收缓冲区
入口参数:id -- 串口管理ID号(0~MAXSERIALNUM-1)
出口参数:无
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
函数名称:BeginSerialThread();
函数功能:开启串口接收线程
入口参数:id -- 串口管理ID号(0~MAXSERIALNUM-1)
出口参数:无
===============================================================================*/
static void BeginSerialThread(int id)
{
	serialport[id].pthread = AfxBeginThread(ReceiveData_Thread,(LPVOID)id);
}

/*===============================================================================
函数名称:EndSerialThread();
函数功能:关闭串口接收线程
入口参数:id -- 串口管理ID号(0~MAXSERIALNUM-1)
出口参数:无
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
	
	//如果关闭超时，则强行关闭线程
	if(clostcnt >= 50)
	{
		TerminateThread(serialport[id].pthread->m_hThread,0);
	}
}

/*===============================================================================
函数名称:OpenSerialPort();
函数功能:打开串口
入口参数:portnr         -- 串口号
		 baud           -- 波特率
		 parity         -- 校验位(NOPARITY,ODDPARITY,EVENPARITY)
		 databits       -- 数据位(5,6,7,8)
		 stopbits       -- 停止位(ONESTOPBIT,ONE5STOPBITS,TWOSTOPBITS)
		 receiveprocess -- 接收回调函数(如果为NULL，则不开启串口线程)
出口参数:-1             -- 端口打开失败
		 -2             -- 打开的串口数目多余管理数目
		 其他           -- 串口管理ID号(0~MAXSERIALNUM-1)
===============================================================================*/
int OpenSerialPort(UINT  portnr,		                          
				   UINT  baud,			                         
				   UINT  parity,		                         
				   UINT  databits,		                          
				   UINT  stopbits,                                
				   PFCALLBACK   receiveprocess,
				   void *puser)	                       // 接收回调处理函数	 
					
{
	HANDLE hComm;
	WCHAR szPort[50];
	COMMTIMEOUTS	  commTimeouts;                                    //超时时间
	DCB				  dcb;                                             //串口参数配置
	
	//sprintf_s(szPort,"COM%d", portnr);                                 //格式化参数
	wsprintfW(szPort,_T("\\\\.\\COM%d"), portnr);
	int id;                                                            //查找可用串口管理ID号
	for(id = 0;id < MAXSERIALNUM;id++)
	{
		if(serialport[id].status == FALSE) break;
	}
	if(id == MAXSERIALNUM) return -2;
	
	//为创建的串口获取一个句柄
	hComm =CreateFile(szPort,                                     //COM口
					  GENERIC_READ|GENERIC_WRITE,                      //允许读和写
					  0,                                               //独占方式
					  NULL,                                            //安全性属性,默认NULL
					  OPEN_EXISTING,                                   //打开而不是创建,该参数必须置为OPEN_EXISTING
					  FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,      //重叠方式
					  NULL);
	if(hComm == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	
	InitializeCriticalSection(&criticalsync);                          //初始化临界区
	
	commTimeouts.ReadIntervalTimeout = MAXWORD;                           //设置超时参数
	commTimeouts.ReadTotalTimeoutMultiplier = 10;
	commTimeouts.ReadTotalTimeoutConstant = 100;
	commTimeouts.WriteTotalTimeoutMultiplier = 100;
	commTimeouts.WriteTotalTimeoutConstant = 100;
	SetCommTimeouts(hComm,&commTimeouts);

	SetCommMask(hComm,EV_RXCHAR);                                       //设置串口接收消息
	
	GetCommState(hComm,&dcb);                                           //设置串口参数

	dcb.BaudRate = baud;                                                //波特率为9600
	dcb.ByteSize = databits;                                            //每个字节有8位
	dcb.Parity   = parity;                                              //无奇偶校验位
	dcb.StopBits = stopbits;                                            //停止位
	SetCommState(hComm,&dcb);

	SetupComm(hComm,MAXRXBUF,MAXTXBUF);                                 //设置输入输出缓冲区的大小
	
	//清空缓冲区
	PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	
	serialport[id].hcomm          = hComm;
	serialport[id].portnum        = portnr;
	serialport[id].baud           = baud;
	serialport[id].databits       = databits;
	serialport[id].stopbits       = stopbits;
	serialport[id].parity         = parity;
	serialport[id].status         = TRUE;
	serialport[id].puserdata      = puser;

	//是否使用线程方式接收数据
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
函数名称:CloseSerialPort();
函数功能:关闭串口
入口参数:id -- 串口管理ID号(0~MAXSERIALNUM-1)
出口参数:-1 -- 关闭失败
		 1  -- 关闭成功
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
函数名称:TransmitDataToPort();
函数功能:串口发送函数
入口参数:id       -- 串口管理ID号(0~MAXSERIALNUM-1)
		 pbuf     -- 待发送数据地址
		 writelen -- 待发送数据长度
		 dwWait   -- 最大等待延时(ms)
出口参数:真实发送数据长度
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
	m_osWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                 //创建读事件
	
	WaitForSingleObject(serialport[id].hwritemutex, INFINITE);
	ClearCommError(hComm,&dwErrorFlags,&ComStat);
	bWriteStat = WriteFile(hComm,pbuf,writelen,&totalwritenum,&m_osWrite);

	if(!bWriteStat)
	{
		DWORD err = GetLastError();

		if(err == ERROR_IO_PENDING)                 
		{
			GetOverlappedResult(hComm,&m_osWrite,&totalwritenum,TRUE);    //当串口写操作进行完毕后，m_osRead的hEvent事件会变为有信号
			
			ReleaseMutex(serialport[id].hwritemutex);
			CloseHandle(m_osWrite.hEvent);
			return totalwritenum;
			
		}
		if(err = ERROR_INVALID_HANDLE)
		{
			AfxMessageBox(_T("无效句柄，请查看串口是否已经打开!"));
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
函数名称:ReceiveDataFromPort();
函数功能:串口接收函数
入口参数:id       -- 串口管理ID号(0~MAXSERIALNUM-1)
		 pbuf     -- 接收数据地址
		 writelen -- 待接收数据长度
		 dwWait   -- 最大等待延时(ms)
出口参数:真实接收数据长度
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

	m_osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                   //创建读事件

	ClearCommError(hComm,&dwErrorFlags,&ComStat);                          //获得通信错误并报告串口的当前状态并清除串口的错误标志

	BOOL bReadStatus = ReadFile(hComm,pbuf,readlen,&totalreadnum,&m_osRead);

	if(!bReadStatus)                                                       //如果ReadFile函数返回FALSE
	{
		DWORD err = GetLastError();
		if(err == ERROR_IO_PENDING)                                        //GetLastError()函数返回ERROR_IO_PENDING,表明串口正在进行读操作 	
		{
			WaitForSingleObject(m_osRead.hEvent,dwWait);
			GetOverlappedResult(hComm,&m_osRead,&totalreadnum,TRUE);      //使用WaitForSingleObject函数等待，直到读操作完成或延时已达到等待时间
			//当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号
			if(totalreadnum == 0) TRACE("没有接收到字符\r\n");

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
函数名称:ReceiveData_Thread();
函数功能:串口接收线程
入口参数:pParam -- 串口管理ID号(0~MAXSERIALNUM-1)
出口参数:无
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
 
	m_osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);                    //创建读事件
 
	ClearCommError(hComm,&dwErrorFlags,&ComStat);                           //获得通信错误并报告串口的当前状态并清除串口的错误标志
	PurgeComm(hComm, PURGE_RXABORT|PURGE_RXCLEAR);                          //清除接收缓冲区
	
	serialport[id].threadstatus = TRUE;

	handarray[0] = m_osRead.hEvent;
	handarray[1] = serialport[id].closethreadflag.m_hObject;
	while(1)
	{
		bResult = WaitCommEvent(hComm,&Event,&m_osRead);                    //如果发生串口事件
		//if (WaitForSingleObject(m_osRead.hEvent,1000)==WAIT_TIMEOUT) continue;
		eventindex = WaitForMultipleObjects(2,handarray,FALSE,1);
		if(eventindex == WAIT_TIMEOUT) continue;
		else if(eventindex == 1) break;

		if((Event&EV_RXCHAR) == EV_RXCHAR)
		{
			ClearCommError(hComm,&dwErrorFlags,&ComStat);                   //读取串口缓冲区中的字节数ComStat.cbInQue
			Event = 0;                                                      //事件标志清零，否则会不停的进入
	  
			if(ComStat.cbInQue == 0) continue;
					
			BOOL bReadStatus = ReadFile(hComm,rxbuf,ComStat.cbInQue,        //读取缓冲区中的数据，数据长度由ComStat.cbInQue指定
										&totalreadnum,&m_osRead);
			rxbuf[totalreadnum] = 0;

			EnterCriticalSection(&criticalsync);                            //进入临界区,防止多线程同时调用相同的数据处理函数
																			//因为数据处理函数很可能是非可重入函数

			for(DWORD i = 0;i < totalreadnum;i++)                             
			{
				serialport[id].ProcessRecData(rxbuf[i],id,serialport[id].puserdata);                    //处理接收到的数据信息
				totalindex++;
			}	   
			//TRACE("线程ID:%d收到%s\r\n",id,rxbuf);
			//TRACE("共收到%d\r\n",totalindex);
			LeaveCriticalSection(&criticalsync);                            //离开临界区,必须与EnterCriticalSection一块使用
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
函数名称:BOOL GetSerialState(void);
函数功能:获取串口当前状态
入口参数:无
出口参数:FALSE -- 未打开，TRUE -- 串口已打开
===============================================================================*/
BOOL GetSerialState(int id)
{
	return serialport[id].status;
}

