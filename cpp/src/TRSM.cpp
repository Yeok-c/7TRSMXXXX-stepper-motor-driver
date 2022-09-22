#include "TRSM.h"

//=========================================================================
unsigned char TRSM::n_steps(int mDeviceAddr, long steps){
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	unsigned char  mRes;
	long mRegData;

	if(steps > 0){ //forwards
		mRegStart = 2004;
		mRegData = steps;
	}
	else if (steps<0){ // backwards
		mRegStart = 2006;
		mRegData = -steps;
	}
	else {
		return 0;
	}
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData >> 0);
	printf("\nn_steps: %d steps", steps);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);
	return mRes;
}

unsigned char TRSM::change_addr(int mDeviceAddr, unsigned char mRegData){
    unsigned char  mRes;	
	unsigned short mRegAddr;
	unsigned short  mRegStart;
	mRegAddr = 3000;
	printf("\nchange_addr: old=%u, new=%d", mDeviceAddr, mRegData);
	mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);
	return mRes;
}

unsigned char TRSM::change_accel_param(int mDeviceAddr, float mRegDataFloat){
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	unsigned char  mRes;

	mRegStart = 3006;
	mCModBusCmd16.mRegData[0] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 0);
	
	printf("\nchange_accel_param: %05f", mRegDataFloat);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,3000);
	
	return mRes;
}

unsigned char TRSM::change_accel_steps(int mDeviceAddr, int mRegData32){
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	unsigned char  mRes;

	mRegStart = 3004;
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData32 >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData32 >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData32 >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData32 >> 0);
	printf("\nchange_accel_steps: %u", mRegData32);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,3000);	
	return mRes;
}

unsigned char TRSM::change_microstep(int mDeviceAddr, unsigned short mRegData){
    unsigned char  mRes;	
	unsigned short mRegAddr;
	unsigned short  mRegStart;
	mRegAddr = 3001;
	printf("\nchange_microstep: %u", mRegData);
	if ((mRegData == 1) || (mRegData == 2) || (mRegData == 4)|| (mRegData == 8)|| (mRegData == 16)|| (mRegData == 32)){
		mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);
	}
	else {
		mRes=0;
	}
	return mRes;
}

// Not working
// unsigned char change_switch_offset(int mDeviceAddr, int mRegData32){
// 	CModBusCmd16 mCModBusCmd16;
//     unsigned char  mRes;	
// 	unsigned short mRegAddr,mRegData;
// 	unsigned short  mRegStart;
// 	mRegAddr = 3014;
// 	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData32 >> 24);
// 	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData32 >> 16);
// 	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData32 >> 8);
// 	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData32 >> 0);
// 	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);	

// 	return mRes;
// }

unsigned char TRSM::change_period_initial(int mDeviceAddr, unsigned short mRegData){
    unsigned char  mRes;	
	unsigned short mRegAddr;
	unsigned short  mRegStart;
	mRegAddr = 3008;
	printf("\nchange_period_initial: %u", mRegData);
	mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,3000);
	return mRes;
}

unsigned char TRSM::change_period_constant(int mDeviceAddr, unsigned short mRegData){
    unsigned char  mRes;	
	unsigned short mRegAddr;
	unsigned short  mRegStart;
	mRegAddr = 3009;
	printf("\nchange_period_constant: %u", mRegData);
	mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,6000);
	return mRes;
}

unsigned char TRSM::flash_parameters(int mDeviceAddr){
	CModBusCmd16 mCModBusCmd16;
    unsigned char  mRes;	
	unsigned short mRegAddr,mRegData;
	unsigned short  mRegStart;
	mRegAddr = 4002;
	mRegData = 0;
	printf("\nflashing parameters");
	mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,3000);
	return mRes;
}

void TRSM::append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
}

unsigned char TRSM::SendFunction16(
	unsigned char mDeviceAddr,
	unsigned short mRegStartAddr,
	unsigned short mRegNumber,
	CModBusCmd16 *pModBusCmd16,
	unsigned int mResTimeOut 
	){

	CModBusCmd16Res      mModBusCmd16Res;
	unsigned short       mCrc;
	unsigned char mDataLable;
	unsigned char *pCharData;
	char mStr[80]="";

//--------------------------------------------

	pModBusCmd16->mAddr              = mDeviceAddr;
	pModBusCmd16->mFunctionCode      = 16;
	pModBusCmd16->mRegStartAddrHight = (unsigned char)(mRegStartAddr>>8);
	pModBusCmd16->mRegStartAddrLow   = (unsigned char)(mRegStartAddr&0xff);
	pModBusCmd16->mRegNumberHight    = (unsigned char)(mRegNumber>>8);
	pModBusCmd16->mRegNumberLow      = (unsigned char)(mRegNumber&0xff);
	pModBusCmd16->mByteNumber        = mRegNumber<<1;//7×Ö½Ú
	
	
	mCrc = ModRtuCrc( 
		(unsigned char*)pModBusCmd16,
		7 + pModBusCmd16->mByteNumber 
		);		
	pModBusCmd16->CrcLow             = (unsigned char)(mCrc&0xff);
	pModBusCmd16->CrcHight           = (unsigned char)(mCrc>>8);

	mDataLable = pModBusCmd16->mByteNumber;
	pModBusCmd16->mRegData[mDataLable++] = pModBusCmd16->CrcLow;
	pModBusCmd16->mRegData[mDataLable++] = pModBusCmd16->CrcHight;
	pModBusCmd16->mTxLength          = 9 + pModBusCmd16->mByteNumber;
	pCharData = (unsigned char*)(pModBusCmd16);
	
	printf("\nSendFunction16:");
    for(unsigned char i=0;i< pModBusCmd16->mTxLength;i++){
		char mStrHex[3];
		printf("%02x",*pCharData++); //reformat as hex
		// sprintf(mStrHex, " %02x",*pCharData++); //reformat as hex
		// strcat(mStr, mStrHex);
	}
	return 1;
}

unsigned char TRSM::SendFunction06(
	unsigned char mDeviceAddr,
	unsigned short mRegStartAddr,
	unsigned short mRegData,
	unsigned int mResTimeOut ){

	CModBusCmd06      mModBusCmd06;;
	CModBusCmd06Res   mModBusCmd06Res;
	unsigned short       mCrc;
	unsigned char *pCharData;
	char mStr[80]="";

//............................................
	mModBusCmd06.mAddr              = mDeviceAddr;
	mModBusCmd06.mFunctionCode      = 6;
	mModBusCmd06.mRegAddrHight      = (unsigned char)(mRegStartAddr>>8);
	mModBusCmd06.mRegAddrLow        = (unsigned char)(mRegStartAddr&0xff); //!
	mModBusCmd06.mRegValueHight     = (unsigned char)(mRegData>>8);//!!
	mModBusCmd06.mRegValueLow       = (unsigned char)(mRegData&0xff);
	
	mCrc = ModRtuCrc( (unsigned char*)(&mModBusCmd06),6);		
	mModBusCmd06.CrcLow             = (unsigned char)(mCrc&0xff);
	mModBusCmd06.CrcHight           = (unsigned char)(mCrc>>8);

	pCharData = (unsigned char*)(&mModBusCmd06);

	printf("\nSendFunction06:");
    for(unsigned char i=0;i< 8;i++)
	{
		char mStrHex[3];
		printf("%02x",*pCharData++); //reformat as hex
		// sprintf(mStrHex, " %02x",*pCharData++); //reformat as hex
		// strcat(mStr, mStrHex);	
	}
	return 1;
}

// 01 06 0b c0 03 20 8a fa
// 01 06 0b b9 00 20 5b d3