// #pragma once
#include "Modbus.h"

// #define WM_REC_MODBUS_RTU_DATA_MSG  WM_USER+2016
// #define MODBUS_SCAN_TIME            WM_USER+2017


// // CMy7TRSM5730ToolDlg 0…90ˆ80†3¡ã0†7¨°
// class CMy7TRSM5730Tool : 
// {
// // 0†10†10ˆ8¨¬
// public:
// 	CMy7TRSM5730Tool();

// // 0…90ˆ80†3¡ã0†7¨°0‡80‹50†60‰6
// 	// enum { IDD = IDD_MY7TRSM5730TOOL_DIALOG };
// 	int  mSerialId;
// 	BOOL mPortState;
// 	BOOL mReadMotorState;
// 	UINT mSubdivide;

// 	CString mEdit1Str;
// 	CString mEdit2Str;
// 	CString mEdit3Str;
// 	CString mEdit4Str;
// 	CString mEdit5Str;
// 	CString mEdit6Str;
// 	CString mEdit7Str;
// 	CString mEdit8Str;
// 	CString mEdit9Str;
// 	CString mEdit10Str;
// 	CString mEdit11Str;
// 	CString mEdit12Str;
// 	CString mEdit13Str;
// 	CString mEdit14Str;
// 	CString mEdit15Str;
// 	protected:
// 	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 0‰0¡ì0…60‰0
// public:
// 	void ReadPortConfig();
// 	void WritePortConfig();
// 	static void WINAPI ReceiveFromModbusRtuDevice(unsigned char data,int id,void* puser);

// 	unsigned char SendFunction03(unsigned char mDeviceADdr,unsigned short mRegStartAddr,unsigned short mRegNumber,CModBusCmd03Res *pModBusCmd03Res);
// 	unsigned char SendFunction16(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegNumber,CModBusCmd16 *pModBusCmd16, unsigned int mResTimeOut );
// 	unsigned char SendFunction06(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegData, unsigned int mResTimeOut );
// 	void ReadMotorState();
// 	void DisResErro(unsigned char mRes);

// 	void forward();
// };
