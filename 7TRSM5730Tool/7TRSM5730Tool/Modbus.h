/*
Modbus.h
*/

#pragma once
#include "StdAfx.h"

typedef struct
{//ReadHoldingRegisters 读保持寄存器
	unsigned char mAddr;
//..功能码	
    unsigned char mFunctionCode;
//..寄存器起始地址
	unsigned char mRegStartAddrHight;
	unsigned char mRegStartAddrLow;
//..读取寄存器的个数(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
//..Send	
}CModBusCmd03;

typedef struct
{//ReadHoldingRegisters 读保持寄存器
	  unsigned char mAddr;
//..响应码
	  unsigned char mFunctionCode;
	  unsigned char mReturnByteNumber;// = 2*mRegNumber
	  unsigned char mRegData[100];     // = 

	  //..
	 unsigned char mCrcHight;
	 unsigned char mCrcLow;
//..接收数据的长度
	  unsigned char mRxLength; 
}CModBusCmd03Res;




typedef struct
{//ReadHoldingRegisters 读保持寄存器
	  unsigned char mAddr;
//..功能码	
    unsigned char  mFunctionCode;
//..寄存器起始地址
	  unsigned char mRegStartAddrHight;
	  unsigned char mRegStartAddrLow;
//..写寄存器的个数(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;
//..数据字节数
    unsigned char mByteNumber;// = 2*mRegNumber	
//..发送数据
	unsigned char mRegData[100];     // = 
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
//..发送的字节长度
	 unsigned char mTxLength;
}CModBusCmd16;
typedef struct
{//ReadHoldingRegisters 读保持寄存器
	unsigned char mAddr;
//..功能码	
    unsigned char mFunctionCode;
//..寄存器起始地址
	unsigned char mRegStartAddrHight;
	unsigned char mRegStartAddrLow;
//..读取寄存器的个数(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd16Res;



typedef struct
{//Preset Single Register  
	unsigned char mAddr;
//..功能码	
    unsigned char  mFunctionCode;
//..寄存器地址
	unsigned char mRegAddrHight;
	unsigned char mRegAddrLow;
//..写寄存器值
    unsigned char mRegValueHight;	
	unsigned char mRegValueLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd06;

typedef struct
{//Preset Single Register  
	unsigned char mAddr;
//..功能码	
    unsigned char  mFunctionCode;
//..寄存器地址
	unsigned char mRegAddrHight;
	unsigned char mRegAddrLow;
//..写寄存器值
    unsigned char mRegValueHight;	
	unsigned char mRegValueLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd06Res;

unsigned short ModRtuCrc(unsigned char buf[], int len);






//$
