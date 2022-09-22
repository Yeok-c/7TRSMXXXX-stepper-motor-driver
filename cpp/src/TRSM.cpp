#include "TRSM.h"

TRSM::TRSM(    
		int ADDR    
	){
	addr = ADDR;
	accel_param = 0.01;
	accel_steps = 2000;
	microstep = 32;
	switch_offset = 1000;
	period_initial = 250;
	period_constant = 800;
	full_revolution_steps = 171072;
	opening_angle = 133;
	// Serial object
	char errorOpening = serial.openDevice(
		SERIAL_PORT, 
		9600,
		SERIAL_DATABITS_8,
		SERIAL_PARITY_NONE,
		SERIAL_STOPBITS_1
		);
	// If connection fails, return the error code otherwise, display a success message
	if (errorOpening!=1){
		printf ("Connection failed to %s\n",SERIAL_PORT);
	}
	else{
		printf ("Successful connection to %s\n",SERIAL_PORT);
	}

	// Set default config
	// change_addr(addr,1);
	change_accel_param(addr, accel_param);
	change_accel_steps(addr, accel_steps);
	change_microstep(addr,microstep);
	// change_switch_offset(addr, switch_offset);
	change_period_initial(addr, period_initial);
	change_period_constant(addr, period_constant);
	config_lock_when_stop(addr, true);
	flash_parameters(addr);
}

TRSM::~TRSM(){
	// Close the serial device
	serial.closeDevice();
}

//=========================================================================
unsigned char TRSM::open(int mDeviceAddr){
	move_angle(mDeviceAddr, -opening_angle);
}

unsigned char TRSM::close(int mDeviceAddr){
	move_angle(mDeviceAddr, opening_angle);
}

unsigned char TRSM::move_angle(int mDeviceAddr, int angle){
	int steps_to_take = angle * (full_revolution_steps/360);
	// printf("Steps to take: %u for full revolution: %u", steps_to_take, full_revolution_steps);
	n_steps(mDeviceAddr, steps_to_take);
}

unsigned char TRSM::n_steps(int mDeviceAddr, int steps){
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

	//!! Ensure lock!!
	// config_lock_when_stop(mDeviceAddr, true);
	// flash_parameters(mDeviceAddr);
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

unsigned char TRSM::config_lock_when_stop(int mDeviceAddr, bool state){
    // unsigned char  mRes;	
	// unsigned short mRegAddr,mRegData;
	// unsigned short  mRegStart;
	// mRegAddr = 3002;
	// mRegData = 0;
	// mRegData = mRegData | 0x10;
	// printf("\nConfig lock when motor is stopped");
	// mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);
	// return mRes;
	// Default: lock
	if(state==true){ //If lock
		unsigned char hex []= {
			0x01, 0x06, 0x0b, 0xba, 0x00, 0x40, 0xab, 0xfb
		};
		printf("\nSending locking config");
		for(unsigned char i=0;i<8;i++){
			serial.writeChar(hex[i]);
			printf(" %02x",hex[i]); //reformat as hex
		}
		printf("\n");
	}
	else{ //If do not lock
		unsigned char hex []= {
			0x01, 0x06, 0x0b, 0xba, 0x00, 0x50, 0xaa, 0x37
		};
		printf("\nSending UNlocking config");
		for(unsigned char i=0;i<8;i++){
			serial.writeChar(hex[i]);
			printf(" %02x",hex[i]); //reformat as hex
		}
		printf("\n");
	}
	Sleep(100);

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

unsigned char TRSM::flash_general_config(int mDeviceAddr){
	unsigned char hex [12][8]= {
		{0x01, 0x06, 0x0b, 0xb8, 0x00, 0x01, 0xca, 0x0b}, //3000 - Address
		{0x01, 0x06, 0x0b, 0xb9, 0x00, 0x20, 0x5b, 0xd3}, //3001 - microstep
		{0x01, 0x06, 0x0b, 0xba, 0x00, 0x40, 0xab, 0xfb}, //3002 - reset mode
		{0x01, 0x06, 0x0b, 0xbb, 0x25, 0x80, 0xe0, 0xfb}, //3003 - baud rate
		{0x01, 0x10, 0x0b, 0xc2, 0x00, 0x02, 0xe2, 0x10}, //3010 - !Max range
		{0x01, 0x10, 0x0b, 0xc6, 0x00, 0x02, 0xa3, 0xd1}, //3014 - !Switch offset
		{0x01, 0x10, 0x0b, 0xbe, 0x00, 0x02, 0x23, 0xc8}, //3006 - !Accel param
		{0x01, 0x10, 0x0b, 0xc8, 0x00, 0x02, 0xc2, 0x12}, //3016 - Voltage
		{0x01, 0x06, 0x0b, 0xc0, 0x00, 0xfa, 0x0b, 0x91}, //3008 - Init speed
		{0x01, 0x10, 0x0b, 0xbc, 0x00, 0x02, 0x82, 0x08}, //3004 - !Accel steps
		{0x01, 0x06, 0x0b, 0xc1, 0x03, 0x20, 0xdb, 0x3a}, //3009 - Constant speed
		{0x01, 0x06, 0x0f, 0xa2, 0x00, 0x00, 0x2b, 0x3c}, //4002 - Flash
	};
	
	for(unsigned char j=0; j<12; j++){
		printf("\n row: %d - ", j);
		for(unsigned char i=0;i<8;i++){
			serial.writeChar(hex[j][i]);
			printf(" %02x",hex[j][i]); //reformat as hex
		}
	}
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
		serial.writeChar(*pCharData);
		printf(" %02x",*pCharData++); //reformat as hex
		// sprintf(mStrHex, " %02x",*pCharData++); //reformat as hex
		// strcat(mStr, mStrHex);
	}
	printf("\n");

	Sleep(200);
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
		serial.writeChar(*pCharData);
		printf(" %02x",*pCharData++); //reformat as hex
	}
	printf("\n");
	Sleep(200);
	return 1;
}

    

// unsigned char output[] = {
// 	0x01, 0x10, 0x07, 0xd6, 0x00, 0x02, 0x04, 0x00, 0x00, 0x03, 0xe8, 0x58, 0x57
// 	};

// 01 06 0b c0 03 20 8a fa
// 01 06 0b b9 00 20 5b d3

