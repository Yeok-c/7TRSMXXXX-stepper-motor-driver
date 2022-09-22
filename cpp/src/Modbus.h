/*
Modbus.h
*/

#pragma once

typedef struct
{//ReadHoldingRegisters �����ּĴ���
	unsigned char mAddr;
//..������	
    unsigned char mFunctionCode;
//..�Ĵ�����ʼ��ַ
	unsigned char mRegStartAddrHight;
	unsigned char mRegStartAddrLow;
//..��ȡ�Ĵ����ĸ���(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
//..Send	
}CModBusCmd03;

typedef struct
{//ReadHoldingRegisters �����ּĴ���
	  unsigned char mAddr;
//..��Ӧ��
	  unsigned char mFunctionCode;
	  unsigned char mReturnByteNumber;// = 2*mRegNumber
	  unsigned char mRegData[100];     // = 

	  //..
	 unsigned char mCrcHight;
	 unsigned char mCrcLow;
//..�������ݵĳ���
	  unsigned char mRxLength; 
}CModBusCmd03Res;




typedef struct
{//ReadHoldingRegisters �����ּĴ���
	  unsigned char mAddr;
//..������	
    unsigned char  mFunctionCode;
//..�Ĵ�����ʼ��ַ
	  unsigned char mRegStartAddrHight;
	  unsigned char mRegStartAddrLow;
//..д�Ĵ����ĸ���(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;
//..�����ֽ���
    unsigned char mByteNumber;// = 2*mRegNumber	
//..��������
	unsigned char mRegData[100];     // = 
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
//..���͵��ֽڳ���
	 unsigned char mTxLength;
}CModBusCmd16;
typedef struct
{//ReadHoldingRegisters �����ּĴ���
	unsigned char mAddr;
//..������	
    unsigned char mFunctionCode;
//..�Ĵ�����ʼ��ַ
	unsigned char mRegStartAddrHight;
	unsigned char mRegStartAddrLow;
//..��ȡ�Ĵ����ĸ���(16BIT)
    unsigned char mRegNumberHight;	
	unsigned char mRegNumberLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd16Res;



typedef struct
{//Preset Single Register  
	unsigned char mAddr;
//..������	
    unsigned char  mFunctionCode;
//..�Ĵ�����ַ
	unsigned char mRegAddrHight;
	unsigned char mRegAddrLow;
//..д�Ĵ���ֵ
    unsigned char mRegValueHight;	
	unsigned char mRegValueLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd06;

typedef struct
{//Preset Single Register  
	unsigned char mAddr;
//..������	
    unsigned char  mFunctionCode;
//..�Ĵ�����ַ
	unsigned char mRegAddrHight;
	unsigned char mRegAddrLow;
//..д�Ĵ���ֵ
    unsigned char mRegValueHight;	
	unsigned char mRegValueLow;	
//..
	unsigned char CrcLow;
	unsigned char CrcHight;
}CModBusCmd06Res;

extern "C" unsigned short ModRtuCrc(unsigned char buf[], int len);






//$
