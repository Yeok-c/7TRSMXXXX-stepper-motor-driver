
// 7TRSM5730ToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "7TRSM5730Tool.h"
#include "7TRSM5730ToolDlg.h"
#include "afxdialogex.h"
#include "SerialService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy7TRSM5730ToolDlg �Ի���




CMy7TRSM5730ToolDlg::CMy7TRSM5730ToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy7TRSM5730ToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mEdit1Str       =_T("1");
	mSubdivide      = 0;
}

void CMy7TRSM5730ToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mEdit1Str);
	DDX_Text(pDX, IDC_EDIT2, mEdit2Str);
	DDX_Text(pDX, IDC_EDIT3, mEdit3Str);
	DDX_Text(pDX, IDC_EDIT4, mEdit4Str);
	DDX_Text(pDX, IDC_EDIT5, mEdit5Str);
	DDX_Text(pDX, IDC_EDIT6, mEdit6Str);
	DDX_Text(pDX, IDC_EDIT7, mEdit7Str);
	DDX_Text(pDX, IDC_EDIT8, mEdit8Str);
	DDX_Text(pDX, IDC_EDIT9, mEdit9Str);
	DDX_Text(pDX, IDC_EDIT10, mEdit10Str);
	DDX_Text(pDX, IDC_EDIT11, mEdit11Str);
	DDX_Text(pDX, IDC_EDIT12, mEdit12Str);
	DDX_Text(pDX, IDC_EDIT13, mEdit13Str);
	DDX_Text(pDX, IDC_EDIT14, mEdit14Str);
	DDX_Text(pDX, IDC_EDIT15, mEdit15Str);
}

BEGIN_MESSAGE_MAP(CMy7TRSM5730ToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()             //��ʱ����Ӧ
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTip)  
	ON_BN_CLICKED(IDC_OPEN_SERIAL_BTN, &CMy7TRSM5730ToolDlg::OnBnClickedOpenSerialBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &CMy7TRSM5730ToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &CMy7TRSM5730ToolDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMy7TRSM5730ToolDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON6, &CMy7TRSM5730ToolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &CMy7TRSM5730ToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMy7TRSM5730ToolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CMy7TRSM5730ToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON9, &CMy7TRSM5730ToolDlg::OnBnClickedButton9)
	ON_EN_SETFOCUS(IDC_EDIT6, &CMy7TRSM5730ToolDlg::OnEnSetfocusEdit6)
	ON_EN_SETFOCUS(IDC_EDIT7, &CMy7TRSM5730ToolDlg::OnEnSetfocusEdit7)
END_MESSAGE_MAP()


// CMy7TRSM5730ToolDlg ��Ϣ�������

BOOL CMy7TRSM5730ToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mReadMotorState = FALSE;
	
	//��ʼ�����ں�ѡ���б�
	for(int i = 0;i < 30;i++)
	{
		CString temp;
		temp.Format(_T("COM%d"),i+1);

		((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_PORT))->InsertString(i,temp);
	}
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_PORT))->SetCurSel(0);

	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(0,_T("2400"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(1,_T("4800"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(2,_T("9600"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(3,_T("19200"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(4,_T("38400"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->InsertString(5,_T("115200"));
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(2);

	//��ʼ������
	InitSerialService();
	ReadPortConfig();
	EnableToolTips(TRUE);   
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMy7TRSM5730ToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMy7TRSM5730ToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ȡ����������Ϣ
void CMy7TRSM5730ToolDlg::ReadPortConfig()
{
	CString str;
	CString temp;

	WCHAR buf[100];

	GetPrivateProfileString(_T("PortConfig"),_T("port"),NULL,buf,100,_T(".\\config.ini"));
	str = buf;
	//���û���ҵ������ļ�������һ��Ĭ�������ļ�
	if(str.GetLength() == 0)
	{
		WritePrivateProfileString(_T("PortConfig"),_T("port"),_T("1"),_T(".\\config.ini"));
		return;
	}
	int portsel = _ttoi(buf);
	if(portsel <= 0) return;
	((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_PORT))->SetCurSel(portsel-1);

//================================================================================
	GetPrivateProfileString(_T("BaudRateConfig"),_T("BaudRate"),NULL,buf,100,_T(".\\config.ini"));
	str = buf;
	//���û���ҵ������ļ�������һ��Ĭ�������ļ�
	if(str.GetLength() == 0)
	{
		WritePrivateProfileString(_T("BaudRateConfig"),_T("BaudRate"),_T("9600"),_T(".\\config.ini"));
		return;
	}
	portsel = _ttoi(buf);
	if( portsel == 2400 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(0);
	if( portsel == 4800 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(1);
	if( portsel == 9600 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(2);
	if( portsel == 19200 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(3);
	if( portsel == 38400 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(4);
	if( portsel == 15200 ) ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->SetCurSel(5);

}
//д�봮��������Ϣ
void CMy7TRSM5730ToolDlg::WritePortConfig()
{
	int portsel = ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_PORT))->GetCurSel();
	CString temp;
	temp.Format(_T("%d"),portsel+1);
	WritePrivateProfileString(_T("PortConfig"),_T("port"),temp,_T(".\\config.ini"));

	portsel = ((CComboBox *)GetDlgItem(IDC_COMBOX_SERIAL_RATE))->GetCurSel();
	if( portsel == 0 )  temp.Format(_T("2400"));
	if( portsel == 1 )  temp.Format(_T("4800"));
	if( portsel == 2 )  temp.Format(_T("9600"));
	if( portsel == 3 )  temp.Format(_T("19200"));
	if( portsel == 4 )  temp.Format(_T("38400"));
	if( portsel == 5 )  temp.Format(_T("15200"));
	WritePrivateProfileString(_T("BaudRateConfig"),_T("BaudRate"),temp,_T(".\\config.ini"));


}


void WINAPI CMy7TRSM5730ToolDlg::ReceiveFromModbusRtuDevice(unsigned char data,int id,void* puser)
{
	
}
/*
�򿪴���
*/
void CMy7TRSM5730ToolDlg::OnBnClickedOpenSerialBtn()
{
    CString temp;
	CString str_com,StrBandRate;
	UINT portnr,mBandRate;

	GetDlgItem(IDC_COMBOX_SERIAL_PORT)->GetWindowText(str_com);
	
	portnr  = UINT(_ttoi(str_com.Right(str_com.GetLength()-3)));

	GetDlgItem(IDC_OPEN_SERIAL_BTN)->GetWindowText(temp);

	GetDlgItem(IDC_COMBOX_SERIAL_RATE)->GetWindowText(StrBandRate);
	mBandRate = UINT(_ttoi(StrBandRate));

	if(!temp.Compare(_T("��")))
	{
		//mSerialId = OpenSerialPort(portnr,9600,NOPARITY,8,ONESTOPBIT,ReceiveFromModbusRtuDevice,this);
		mSerialId = OpenSerialPort(portnr,mBandRate,NOPARITY,8,ONESTOPBIT,NULL,this);
		if(mSerialId == -1)
		{
			AfxMessageBox(_T("Port open failed!"));
			return;
		}
		else
		{
			GetDlgItem(IDC_OPEN_SERIAL_BTN)->SetWindowText(_T("�ر�"));
			GetDlgItem(IDC_COMBOX_SERIAL_PORT)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBOX_SERIAL_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
			mPortState = TRUE;
		}

		WritePortConfig();

		SetTimer(MODBUS_SCAN_TIME,500,NULL);//
	}
	else
	{
		//�رմ����߳�
		if(mSerialId != -1)
		{
			GetDlgItem(IDC_OPEN_SERIAL_BTN)->SetWindowText(_T("��"));
			GetDlgItem(IDC_COMBOX_SERIAL_PORT)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBOX_SERIAL_RATE)->EnableWindow(TRUE);
			CloseSerialPort(mSerialId);
			mPortState = FALSE;
		}
		KillTimer(MODBUS_SCAN_TIME);
	}
}

unsigned char CMy7TRSM5730ToolDlg::SendFunction03(
	unsigned char   mDeviceADdr,
	unsigned short  mRegStartAddr,
    unsigned short  mRegNumber,
	CModBusCmd03Res *pModBusCmd03Res)
{
	CModBusCmd03         mModBusCmd03;
	unsigned short       mCrc;
	unsigned char *pCharData;
	CString mStr;
//............................................
	mModBusCmd03.mAddr              = mDeviceADdr;
	mModBusCmd03.mFunctionCode      = 0x03;
	mModBusCmd03.mRegStartAddrHight = (unsigned char)(mRegStartAddr>>8);
	mModBusCmd03.mRegStartAddrLow   = (unsigned char)(mRegStartAddr&0xff);
	mModBusCmd03.mRegNumberHight    = (unsigned char)(mRegNumber>>8);
	mModBusCmd03.mRegNumberLow      = (unsigned char)(mRegNumber&0xff);
	mCrc = ModRtuCrc( (unsigned char*)(&mModBusCmd03),6);	
	mModBusCmd03.CrcLow             = (unsigned char)(mCrc&0xff);
	mModBusCmd03.CrcHight           = (unsigned char)(mCrc>>8);
//..addr[1] +  Function[1] + mReturnByteNumber[1] + Vaule[mReturnByteNumber] CRC[2]
	pModBusCmd03Res->mRxLength       = 5 + mRegNumber *2;
//..Send Data
	pCharData = (unsigned char*)(&mModBusCmd03);
	mStr.Format(_T("Send[Function Code03]:"));
    for(unsigned char i=0;i< sizeof(CModBusCmd03);i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR2)->SetWindowText(mStr);

	TransmitDataToPort(mSerialId,LPBYTE(&mModBusCmd03),sizeof(CModBusCmd03),500);
//..Recive Data
	long mRxLength = 
	ReceiveDataFromPort(mSerialId,(LPBYTE)pModBusCmd03Res,pModBusCmd03Res->mRxLength,800);
	if( mRxLength < 3) return 1;
	pModBusCmd03Res->mCrcLow    = *(((unsigned char*)pModBusCmd03Res)+ mRxLength-2);
	pModBusCmd03Res->mCrcHight  = *(((unsigned char*)pModBusCmd03Res)+ mRxLength-1);
//..
	mCrc = ModRtuCrc((unsigned char*)pModBusCmd03Res,pModBusCmd03Res->mRxLength-2);
	if( mCrc != ( (pModBusCmd03Res->mCrcHight<<8) + pModBusCmd03Res->mCrcLow) ) return 2;
	if( mRxLength != pModBusCmd03Res->mRxLength ) return 3;

//..data display  1[����С��3]  2[У��] 3[���յ����ݳ��Ȳ���ȷ]
	

	pCharData = (unsigned char*)pModBusCmd03Res;
	mStr.Format(_T("Function code03[%d]:"),pModBusCmd03Res->mRxLength);
    for(unsigned char i=0;i<pModBusCmd03Res->mRxLength;i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
    return 0;
}
/*
��ȡ����
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton2()
{
	//���Ͳ��� 03��
	CModBusCmd03Res      mModBusCmd03Res;
	unsigned char  mRes;	
	unsigned short mRegData;	

	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);

	if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ==1 )
	{
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(0);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
		AfxMessageBox(_T("Pause Scan"));
		return;
	}
	ClearRxBuf(mSerialId);
//..��ȡ   ģ���ַ �������ϸ�� ��λ��ʽ 
	mRes = SendFunction03(mDeviceAddr,3000,4,&mModBusCmd03Res);
	if( mRes == 0 )
	{//���յ����ݳ��� mModBusCmd03Res.mRxLength		
		// ((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		mRegData = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//��ַ
		mEdit2Str.Format(_T("%d"),mRegData&0xff);
		//ϸ��
		mRegData   = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//ϸ��
		if(mRegData > 256 ) 
		   mRegData   = mRegData &0xff; 
		mSubdivide = mRegData;
		mEdit3Str.Format(_T("%d"),mRegData);

		{mRegData = (mModBusCmd03Res.mRegData[4]<<8) + mModBusCmd03Res.mRegData[5];//��λ��ʽ
		unsigned char mWorkMode = mRegData&0xff;
		//����ʹ�ܵ��				  
			if( ( mWorkMode & 0x10) == 0 )
				        ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_EN))->SetCheck(1);
			else  ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_EN))->SetCheck(0);

			if( (mWorkMode & 0x20) == 0x20 ) 
			{//Բ���˶�
				 ((CButton*)GetDlgItem(IDC_RADIO_MODE_LINE))->SetCheck(0);
				 ((CButton*)GetDlgItem(IDC_RADIO_CIRCLE))->SetCheck(1);
			}
			else
			{//ֱ���˶�
				 ((CButton*)GetDlgItem(IDC_RADIO_MODE_LINE))->SetCheck(1);
				 ((CButton*)GetDlgItem(IDC_RADIO_CIRCLE))->SetCheck(0);
			}

			if( (mWorkMode & 0x40) == 0x40 ) 
			{//�͵�ƽ����
				 ((CButton*)GetDlgItem(IDC_RADIO_HOME_H))->SetCheck(0);
				 ((CButton*)GetDlgItem(IDC_RADIO_HOME_L))->SetCheck(1);
			}
			else
			{//�ߵ�ƽ����
				 ((CButton*)GetDlgItem(IDC_RADIO_HOME_H))->SetCheck(1);
				 ((CButton*)GetDlgItem(IDC_RADIO_HOME_L))->SetCheck(0);
				 
			}

			if( ( mWorkMode & 0x80) == 0x80 ) //������з�����
				        ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_DIR))->SetCheck(1);
			else  ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_DIR))->SetCheck(0);
		}
		mRegData = (mModBusCmd03Res.mRegData[6]<<8) + mModBusCmd03Res.mRegData[7];//������
		mEdit12Str.Format(_T("%d"),mRegData);UpdateData(FALSE);
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length<3]  2[LRC] 3[length erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
	Sleep(100);
//..��ȡ ���ٲ���   //:����ϵ��
	mRes = SendFunction03(mDeviceAddr,3004,4,&mModBusCmd03Res);
	if( mRes == 0 )
	{
		unsigned int   mRegData32;
		mRegData = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//��ַ
		mRegData32 = mRegData32 + mRegData;
		mEdit4Str.Format(_T("%d"),mRegData);

		float   mRegDataFloat;
		
		mRegData = (mModBusCmd03Res.mRegData[4]<<8) + mModBusCmd03Res.mRegData[5];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[6]<<8) + mModBusCmd03Res.mRegData[7];//��ַ
		mRegData32 = mRegData32 + mRegData;

		*((unsigned int*)&mRegDataFloat) = mRegData32;
		mEdit5Str.Format(_T("%.2f"),mRegDataFloat);
		

		UpdateData(FALSE);
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length<3]  2[LRC] 3[length erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
	Sleep(200);
//..��ȡ //:����ߵ�ƽʱ��:�������ٶ� //:����ߵ�ƽʱ��:�㶨�ٶ� //:�����
	mRes = SendFunction03(mDeviceAddr,3008,4,&mModBusCmd03Res);
	if( mRes == 0 )
	{
		//mSubdivide
		//�����ٶ�
		mRegData = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//
		//if( (mRegData * mSubdivide )!=0 )
		//mEdit7Str.Format(_T("%.1f"),60000000.0/(mRegData * mSubdivide *200));// 1000000/(mRegData * mSubdivide *200)*60
		mEdit7Str.Format(_T("%d"),mRegData);// 1000000/(mRegData * mSubdivide *200)*60
		//����ٶ�
		mRegData = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//
		//if( (mRegData * mSubdivide )!=0 )
		//mEdit6Str.Format(_T("%.1f"),60000000.0/(mRegData * mSubdivide *200));
		mEdit6Str.Format(_T("%d"),mRegData);
		
		unsigned int   mRegData32;
		mRegData = (mModBusCmd03Res.mRegData[4]<<8) + mModBusCmd03Res.mRegData[5];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[6]<<8) + mModBusCmd03Res.mRegData[7];//��ַ
		mRegData32 = mRegData32 + mRegData;		
		mEdit8Str.Format(_T("%d"),mRegData32);
		

		UpdateData(FALSE);
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length<3]  2[LRC] 3[length erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
//:��е0��Ĳο�λ��  �������벽��
	Sleep(200);
	mRes = SendFunction03(mDeviceAddr,3012,4,&mModBusCmd03Res);
	if( mRes == 0 )
	{
		
		unsigned int   mRegData32;
		mRegData = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//��ַ
		mRegData32 = mRegData32 + mRegData;
		mEdit15Str.Format(_T("%d"),mRegData);

		mRegData = (mModBusCmd03Res.mRegData[4]<<8) + mModBusCmd03Res.mRegData[5];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[6]<<8) + mModBusCmd03Res.mRegData[7];//��ַ
		mRegData32 = mRegData32 + mRegData;		
		mEdit9Str.Format(_T("%d"),mRegData32);		
		UpdateData(FALSE);
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length<3]  2[LRC] 3[length erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
//:����ֵ
	Sleep(200);
	mRes = SendFunction03(mDeviceAddr,3016,2,&mModBusCmd03Res);
	if( mRes == 0 )
	{
		
		unsigned int   mRegData32;
		float   mRegDataFloat;

		mRegData = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//��ַ
		mRegData32 = mRegData<<16;
		mRegData = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//��ַ
		mRegData32 = mRegData32 + mRegData;

		*((unsigned int*)&mRegDataFloat) = mRegData32;
		mEdit14Str.Format(_T("%.2f"),mRegDataFloat);
		//mEdit14Str.Format(_T("%.1f"),  *((float*)(&mRegData)));
		UpdateData(FALSE);
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length<3]  2[LRC] 3[length erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	SetTimer(MODBUS_SCAN_TIME,500,NULL);//
}
unsigned char CMy7TRSM5730ToolDlg::SendFunction16(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegNumber,CModBusCmd16 *pModBusCmd16,unsigned int mResTimeOut )
{
	CModBusCmd16Res      mModBusCmd16Res;
	unsigned short       mCrc;
	unsigned char mDataLable;

	unsigned char *pCharData;
	CString mStr;
//............................................
	pModBusCmd16->mAddr              = mDeviceAddr;
	pModBusCmd16->mFunctionCode      = 16;
	pModBusCmd16->mRegStartAddrHight = (unsigned char)(mRegStartAddr>>8);
	pModBusCmd16->mRegStartAddrLow   = (unsigned char)(mRegStartAddr&0xff);
	pModBusCmd16->mRegNumberHight    = (unsigned char)(mRegNumber>>8);
	pModBusCmd16->mRegNumberLow      = (unsigned char)(mRegNumber&0xff);
	pModBusCmd16->mByteNumber        = mRegNumber<<1;//7�ֽ�
	
	
	mCrc = ModRtuCrc( (unsigned char*)pModBusCmd16,7 + pModBusCmd16->mByteNumber );		
	pModBusCmd16->CrcLow             = (unsigned char)(mCrc&0xff);
	pModBusCmd16->CrcHight           = (unsigned char)(mCrc>>8);

	mDataLable = pModBusCmd16->mByteNumber;
	pModBusCmd16->mRegData[mDataLable++] = pModBusCmd16->CrcLow;
	pModBusCmd16->mRegData[mDataLable++] = pModBusCmd16->CrcHight;
//..�������ݳ���	
	pModBusCmd16->mTxLength          = 9 + pModBusCmd16->mByteNumber;
//..Send Data
	pCharData = (unsigned char*)(pModBusCmd16);
	mStr.Format(_T("Send[Function Code16]:"));
    for(unsigned char i=0;i< pModBusCmd16->mTxLength;i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR2)->SetWindowText(mStr);

	TransmitDataToPort(mSerialId,(LPBYTE)pModBusCmd16,pModBusCmd16->mTxLength,500);
//..Recive Data
	long mRxLength = 
	ReceiveDataFromPort(mSerialId,(LPBYTE)(&mModBusCmd16Res),8,mResTimeOut);
	if( mRxLength < 3) return 1;
//..
	mCrc = ModRtuCrc((unsigned char*)(&mModBusCmd16Res),6);
	if( mRxLength != 8 ) return 3;
	if( mCrc != (( mModBusCmd16Res.CrcHight <<8) + mModBusCmd16Res.CrcLow) ) return 2;
	

//..data display
	

	pCharData = (unsigned char*)(&mModBusCmd16Res);
	mStr.Format(_T("Function Code16[%d]:"),mRxLength);
    for(unsigned char i=0;i< mRxLength;i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
    return 0;
}
/*
�������
Cmd16:2004
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton7()
{
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	unsigned int   mRegData;
	unsigned char  mRes;
//...	
	UpdateData(TRUE);

	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRegData = _wtoi(mEdit11Str);	
	mRegStart = 2004;
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData >> 0);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);
	Sleep(200);
	if( mRes == 0 )
	{
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	

}

/*
�������
Cmd16:2006
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton8()
{
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	unsigned int   mRegData;
	unsigned char  mRes;
//...	
	UpdateData(TRUE);

	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRegData = _wtoi(mEdit11Str);
	mRegStart = 2006;
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData >> 0);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);
	Sleep(200);
	if( mRes == 0 )
	{
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
}

/*
�����λ  cmd16:2002
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton6()
{
	CModBusCmd16 mCModBusCmd16;
	unsigned short mRegStart;
	int   mRegData;
	unsigned char  mRes;
//...	
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRegData = _wtoi(mEdit10Str);
	mRegStart = 2002;
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData >> 0);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);
	Sleep(200);
	if( mRes == 0 )
	{
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
}
void CMy7TRSM5730ToolDlg::ReadMotorState()
{
//���Ͳ��� 03��
	CModBusCmd03Res      mModBusCmd03Res;
	unsigned char  mRes;	
	unsigned short mRegData;
	int            mRegData32;		
	CString mStr;
//..��ȡ   ģ���ַ �������ϸ�� ��λ��ʽ 
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRes = SendFunction03(mDeviceAddr,1000,5,&mModBusCmd03Res);
	if( mRes == 0 )
	{//���յ����ݳ��� mModBusCmd03Res.mRxLength		
		mRegData   = (mModBusCmd03Res.mRegData[0]<<8) + mModBusCmd03Res.mRegData[1];//
		mRegData32 = mRegData<<16;
		mRegData   = (mModBusCmd03Res.mRegData[2]<<8) + mModBusCmd03Res.mRegData[3];//
		mRegData32 = mRegData32 + mRegData;
		mStr.Format(_T("Current Pos:%d"),mRegData32);
		GetDlgItem(IDC_STATIC_POSITION)->SetWindowText(mStr);


		mRegData   = (mModBusCmd03Res.mRegData[4]<<8) + mModBusCmd03Res.mRegData[5];//
		mRegData32 = mRegData<<16;
		mRegData   = (mModBusCmd03Res.mRegData[6]<<8) + mModBusCmd03Res.mRegData[7];//
		mRegData32 = mRegData32 + mRegData;
		mStr.Format(_T("Object Pos:%d"),mRegData32);
		GetDlgItem(IDC_STATIC_POSITION_OBJECT)->SetWindowText(mStr);	

		mRegData   = (mModBusCmd03Res.mRegData[8]<<8) + mModBusCmd03Res.mRegData[9];//		

		if( (mRegData &0xff) == 0xff)                mStr.Format(_T("State:undefine[%03x]"),mRegData);			
	    else if( ((mRegData &0xff) & 0xf0 )== 0x10 ) mStr.Format(_T("State:Not found Zero Switch[%03x]"),mRegData);
		else if( ((mRegData &0xff) & 0xf0 )== 0x60 ) mStr.Format(_T("Top Switch Collision[%03x]"),mRegData);
		else if( ((mRegData &0xff) & 0xf0 )== 0x70 ) mStr.Format(_T("bottom Switch Collision[%03x]"),mRegData);
	    else if( ((mRegData &0xff) & 0xf0 ) != 0 )   mStr.Format(_T("State:erro[%03x]"),mRegData);			
//..��������״̬
	    else if( ((mRegData &0xff) & 0x08 ) == 0x08 )mStr.Format(_T("State:Reset[%03x]..."),mRegData);			
	    else if( ((mRegData &0xff) & 0x07 ) == 0x0 ) mStr.Format(_T("State:Idle[%03x]"),mRegData);			
        else if( ((mRegData &0xff) & 0x07 ) == 0x01 )mStr.Format(_T("State:ACC[%03x]..."),mRegData);			
	    else if( ((mRegData &0xff) & 0x07 ) == 0x02 )mStr.Format(_T("State:ConstantSpeed[%03x]..."),mRegData);			
	    else if( ((mRegData &0xff) & 0x07 )== 0x03 ) mStr.Format(_T("State:DEC[%03x]..."),mRegData);			


	
		GetDlgItem(IDC_STATIC_STATE)->SetWindowText(mStr);
		
	}//if( mRes == 0 )
	else
	{		
		mStr.Format(_T("%d"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
}
/***************************************************************************
**              ��ʱ������
************************
** Write Data:2008.02.15
** Edit  Data:2008.02.15
** Auther    :WQS
** port:COM 
***************************************************************************/
void CMy7TRSM5730ToolDlg::OnTimer(UINT nIDEvent) 
{
	//SetTimer  KillTimer
//..ͨѶ����	
	if( nIDEvent == MODBUS_SCAN_TIME)
	{//
		if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ==1 )
		{
			mReadMotorState = TRUE;
		    ReadMotorState();
		    mReadMotorState = FALSE;
		}		
	}//if( nIDEvent == MODBUS_SCAN_TIME)
}
unsigned char CMy7TRSM5730ToolDlg::SendFunction06(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegData,unsigned int mResTimeOut )
{
	CModBusCmd06      mModBusCmd06;;
	CModBusCmd06Res   mModBusCmd06Res;
	unsigned short       mCrc;

	unsigned char *pCharData;
	CString mStr; 

//............................................
	mModBusCmd06.mAddr              = mDeviceAddr;
	mModBusCmd06.mFunctionCode      = 6;
	mModBusCmd06.mRegAddrHight      = (unsigned char)(mRegStartAddr>>8);
	mModBusCmd06.mRegAddrLow        = (unsigned char)(mRegStartAddr&0xff);
	mModBusCmd06.mRegValueHight     = (unsigned char)(mRegData>>8);
	mModBusCmd06.mRegValueLow       = (unsigned char)(mRegData&0xff);
	
	mCrc = ModRtuCrc( (unsigned char*)(&mModBusCmd06),6);		
	mModBusCmd06.CrcLow             = (unsigned char)(mCrc&0xff);
	mModBusCmd06.CrcHight           = (unsigned char)(mCrc>>8);
//..Send Data
	pCharData = (unsigned char*)(&mModBusCmd06);
	mStr.Format(_T("Send[Function Code:%02x]:"),mModBusCmd06.mFunctionCode);
    for(unsigned char i=0;i< 8;i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR2)->SetWindowText(mStr);

	TransmitDataToPort(mSerialId,(LPBYTE)(&mModBusCmd06),8,500);
	 
//..Recive Data
	long mRxLength = 
	ReceiveDataFromPort(mSerialId,(LPBYTE)(&mModBusCmd06Res),8,mResTimeOut);
	if( mRxLength < 3) return 1;
//..
	mCrc = ModRtuCrc((unsigned char*)(&mModBusCmd06Res),6);
	if( mRxLength != 8 ) return 3;
	if( mCrc != (( mModBusCmd06Res.CrcHight <<8) + mModBusCmd06Res.CrcLow) ) return 2;
	

//..data display
	
	
    


	pCharData = (unsigned char*)(&mModBusCmd06Res);
	mStr.Format(_T("Function Code06[%d]:"),mRxLength);
    for(unsigned char i=0;i< 8;i++)
	{
			CString mStrHex;
			mStrHex.Format(_T(" %02x"),*pCharData++);
			mStr = mStr + mStrHex;			
	}//for(unsigned char i=0;i<mModBusCmd03Res.mRxLength;i++)		
	GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
    return 0;
}
/*
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char  mRes;		
	CString mStr;
//..2000:�����λ
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRes = SendFunction06(mDeviceAddr,2000,1,1000);
	Sleep(200);
	if( mRes == 0 )
	{
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length < 3]  2[LRC] 3[length  erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	

}


void CMy7TRSM5730ToolDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char  mRes;		
	CString mStr;
//..2001:���ֹͣ
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
	mRes = SendFunction06(mDeviceAddr,2001,1,1000);
	Sleep(200);
	if( mRes == 0 )
	{
	}
	else
	{
		CString mStr;
		mStr.Format(_T("%d  1[length < 3]  2[LRC] 3[length  erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
	}	
}

void CMy7TRSM5730ToolDlg::DisResErro(unsigned char mRes)
{
	CString mStr;
		mStr.Format(_T("%d  1[length < 3]  2[LRC] 3[length  erro]"),mRes);
		GetDlgItem(IDC_STATIC_INFOR)->SetWindowText(mStr);
}
/*
д����
*/
void CMy7TRSM5730ToolDlg::OnBnClickedButton3()
{
    unsigned char  mRes,i;		
	unsigned short mRegAddr,mRegData;
	unsigned short  mRegStart;
	int            mRegData32;
	float          mRegDataFloat;
	CModBusCmd16 mCModBusCmd16;	
	
//.......................	
	CModBusCmd16   mModBusCmd16Res; 
	ReceiveDataFromPort(mSerialId,(LPBYTE)(&mModBusCmd16Res),100,1000);
	

	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
//..ģ���ַ:3000
	mRegAddr = 3000;
	mRegData = ( (unsigned char)(_ttoi(mEdit2Str)) &0xff);	
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);
		if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set Device Addr")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);
//:�������ϸ��:3001
	mRegAddr = 3001;
	mRegData = _ttoi(mEdit3Str); 
	if(  (mRegData == 1) || (mRegData == 2) || (mRegData == 4)|| (mRegData == 8)|| (mRegData == 16)|| (mRegData == 32)|| (mRegData == 64)|| (mRegData == 128)|| (mRegData == 256) ) {}
	else { AfxMessageBox(_T("Indexer Must:1 2 4 8 16 32 64 128 256"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}

	mSubdivide =mRegData;
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set  Indexer")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);
	
//:ResetMode:3002
	mRegAddr = 3002;
	mRegData = 0; 
	
	if( ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_EN))->GetCheck() ==0 )  mRegData = mRegData | 0x10;//���в�ʹ��
	if( ((CButton*)GetDlgItem(IDC_RADIO_CIRCLE))->GetCheck() )        mRegData = mRegData | 0x20;//Բ���˶�
	if( ((CButton*)GetDlgItem(IDC_RADIO_HOME_L))->GetCheck() )        mRegData = mRegData | 0x40;//�͵�ƽ����
	if( ((CButton*)GetDlgItem(IDC_CHECK_MOTOR_DIR))->GetCheck() ==1 ) mRegData = mRegData | 0x80;//������

	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);;	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}	
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set  Work Mode")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);
//://:������:3003
	mRegAddr = 3003;
	mRegData = _ttoi(mEdit12Str); 
	if(  (mRegData == 2400) || (mRegData == 4800) || (mRegData == 9600)|| (mRegData == 19200)|| (mRegData == 38400)|| (mRegData == 115200)) {}
	else { AfxMessageBox(_T("baud rate Must:2400 4800 9600 19200 38400 115200"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,1000);
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}		
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set baud rate")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);



//:�����  3010	
	mRegStart = 3010;
	mRegData32 = _ttoi(mEdit8Str); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData32 >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData32 >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData32 >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData32 >> 0);
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(200);}
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set Maximum distance Setps")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);
//:�������벽��  3014	
	mRegStart = 3014;
	mRegData32 = _ttoi(mEdit9Str); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData32 >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData32 >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData32 >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData32 >> 0);
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(200);}
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to Set Switch off Setps"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}
	Sleep(100);
//:����ϵ��  3006	
	mRegStart = 3006;
	mRegDataFloat = (float)(_ttof(mEdit5Str)); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 0);
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,3000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(1000);}		
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to acc coefficient"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}
	Sleep(100);
//:����  3016	
	mRegStart = 3016;
	mRegDataFloat = (float)(_ttof(mEdit14Str)); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 0);
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,1000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}		
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to set current"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}

	Sleep(100);
//:����ߵ�ƽʱ��:�������ٶ�3008
	mRegAddr = 3008;
	
	//float mRpm = (float)(_ttof(mEdit7Str));	
	//if( mRpm*mSubdivide ==0) return;
	//mRegData = (unsigned short)(300000/(mRpm*mSubdivide));
	mRegData = _ttoi(mEdit7Str); 	
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,3000);
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(3000);}		
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to set Start Speed")); GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);return;}
	Sleep(100);
//:���ٲ���  3004	
	mRegStart = 3004;
	mRegData32 = _ttoi(mEdit4Str); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)(mRegData32 >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)(mRegData32 >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)(mRegData32 >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)(mRegData32 >> 0);
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,3000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}	
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to set Acc Steps"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}
	Sleep(100);
//://:����ߵ�ƽʱ��:�㶨�ٶ�:�������ٶ�3009
	mRegAddr = 3009;
	//mRpm = (float)(_ttof(mEdit6Str));	
	//if( mRpm*mSubdivide ==0) return;
	//mRegData = (unsigned short)(300000/(mRpm*mSubdivide));
	mRegData = _ttoi(mEdit6Str); 
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,6000);
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}		
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to set constant Speed"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}

	Sleep(100);
//..����д��flash 4002
    mRegAddr = 4002;
	mRegData = 0;	
	for(i=0;i<3;i++)
	{
	    mRes = SendFunction06(mDeviceAddr,mRegAddr,mRegData,3000);	
	    if( mRes == 0 )	break; else { DisResErro(mRes); Sleep(500);}	
	}//for(unsigned char i=0,i<3;i++)
	if(i >=3 ) { AfxMessageBox(_T("Failed to set successful parameter to write to flash"));GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); return;}
//...
     AfxMessageBox(_T("The parameter is written successfully. If you change the address or baud rate, restart the device"));
	 GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	 GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	 Sleep(100);
}


void CMy7TRSM5730ToolDlg::OnBnClickedButton9()
{
    unsigned short mRegStart,mRegData;
	unsigned char mRes;
	float mRegDataFloat;
	CModBusCmd16 mCModBusCmd16;	
//..............
	UpdateData(TRUE);
	unsigned char mDeviceAddr = (unsigned char)(_ttoi(mEdit1Str));
//..............
	mRegStart = 2010;
	mRegDataFloat = (float)(_ttof(mEdit13Str)); 	
	mCModBusCmd16.mRegData[0] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 24);
	mCModBusCmd16.mRegData[1] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 16);
	mCModBusCmd16.mRegData[2] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 8);
	mCModBusCmd16.mRegData[3] = (unsigned char)((*(unsigned int*)(&mRegDataFloat)) >> 0);
	mRes = SendFunction16(mDeviceAddr,mRegStart,2,&mCModBusCmd16,3000);	
	if( mRes == 0 )	{}
	else { DisResErro(mRes);}	

}


void CMy7TRSM5730ToolDlg::OnEnSetfocusEdit6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	float mRpm = (float)(_ttof(mEdit6Str));	
	int   mSubdivide = _ttoi(mEdit3Str);
	
	if(  (mSubdivide == 1) || (mSubdivide == 2) || (mSubdivide == 4)|| (mSubdivide == 8)|| (mSubdivide == 16)|| (mSubdivide == 32)|| (mSubdivide == 64)|| (mSubdivide == 128)|| (mSubdivide == 256) ) {}
	else return;
	CString mStr;//    1SȦ��: mRpm/60   1S����:mRpm*mSubdivide*200/60
	// 1����������(s): 60/(mRpm*mSubdivide*200)
	// 300000/(mRpm*mSubdivide)
	if( mRpm*mSubdivide ==0) return;
	//mStr.Format(_T("��������us:%d"),(unsigned short)(300000/(mRpm*mSubdivide)));
    	

	//GetDlgItem(IDC_STATIC_INFOR2)->SetWindowText(mStr);
}


void CMy7TRSM5730ToolDlg::OnEnSetfocusEdit7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	float mRpm = (float)(_ttof(mEdit7Str));	
	int   mSubdivide = _ttoi(mEdit3Str);
	
	if(  (mSubdivide == 1) || (mSubdivide == 2) || (mSubdivide == 4)|| (mSubdivide == 8)|| (mSubdivide == 16)|| (mSubdivide == 32)|| (mSubdivide == 64)|| (mSubdivide == 128)|| (mSubdivide == 256) ) {}
	else return;
	CString mStr;//    1SȦ��: mRpm/60   1S����:mRpm*mSubdivide*200/60
	// 1����������(s): 60/(mRpm*mSubdivide*200)
	// 300000/(mRpm*mSubdivide)
	if( mRpm*mSubdivide ==0) return;
	//mStr.Format(_T("��������us:%d"),(unsigned short)(300000/(mRpm*mSubdivide)));    	
	GetDlgItem(IDC_STATIC_INFOR2)->SetWindowText(mStr);
}
BOOL   CMy7TRSM5730ToolDlg::OnToolTip(UINT id,NMHDR *pNMHDR,LRESULT *pResult)  
{ 
	    CString mStr;
        //LPWSTR  lpstr = (LPWSTR)(LPCWSTR)str;

        TOOLTIPTEXT   *pTTT=(TOOLTIPTEXT*)pNMHDR;   
        UINT  uID=pNMHDR->idFrom;   
        if(pTTT->uFlags  &  TTF_IDISHWND)    uID=::GetDlgCtrlID((HWND)uID);   
        if(uID==NULL)   return   FALSE;   

        switch(uID)
        {   //lpszText:LPWSTR
		   case   IDC_RADIO_MODE_LINE:pTTT->lpszText= _T("ֱ���˶�:���λ�ú��յ㲻�غ�"); break;
           case   IDC_RADIO_CIRCLE:pTTT->lpszText= _T("Բ���˶�:�������յ�λ���ظ�"); break;
		   case   IDC_CHECK_MOTOR_DIR:pTTT->lpszText= _T("������:�õ����ǰ�����з����෴,Ŀ���Ǳ������е����һ�½ӷ�,��������������"); break;
		   case   IDC_CHECK_MOTOR_EN:pTTT->lpszText= _T("����ʹ��:������˶���ʱ���Ƿ�����"); break;
		   case   IDC_RADIO_HOME_H:pTTT->lpszText= _T("�ߵ�ƽ����:PNP����źŸߵ�ƽ����"); break;
		   case   IDC_RADIO_HOME_L:pTTT->lpszText= _T("�͵�ƽ����:PNP����źŵ͵�ƽ����"); break;
		   case   IDC_STATIC_MAX_SPEED:pTTT->lpszText= _T("����ٶȣ���е����֧�ֵ�����ٶ�"); break;
		   case   IDC_STATIC_START_SPPED:pTTT->lpszText= _T("�����ٶ�:������ٶ�ͬʱҲ�������ٶ�"); break;
           case   IDC_STATIC_ACC_STEP:pTTT->lpszText= _T("���ٲ���:����������ٶȿ�ʼ���ٵ�����ٶȣ�ʹ�õĲ���"); break;
	       case   IDC_STATIC_COF:pTTT->lpszText= _T("����ϵ��:��ֵԽС�����ٵĻ�е����Խ��"); break;
		   case   IDC_STATIC_OFF_ZERO:pTTT->lpszText= _T("���䲽��:����λ��ʱ�����⿪���ź���Ч,���������ߵĲ���,������꣬�����ֹ�翪���ź���Ч���򱨴�"); break;
		   case   IDC_BUTTON9:pTTT->lpszText= _T("����rpm:��ǰ�������ʱ���ı��µ��ٶ�����;����������1.8������"); break;
		   //case   IDC_RADIO_PPS:pTTT->lpszText= _T("PPS:һ���Ӳ�������ߵĲ����������"); break; 
		   //case   IDC_RADIO_US:pTTT->lpszText= _T("����us:��������Ĳ���������[��λ:us]"); break; 		   		   			   
		   case   IDC_EDIT6://mEdit7Str    _wtoi(mEdit7Str)   
			   UpdateData (TRUE);
			   if( mEdit6Str.GetLength() !=0 )
			   {  
				   mStr.Format(_T("����rpm:%.1f   @ϸ��%d@�����1.8��"), 300000.0f/(_wtoi(mEdit6Str)*_wtoi(mEdit3Str) ) ,_wtoi(mEdit3Str));					
				   pTTT->lpszText=mStr.AllocSysString();
					
			   }
			   else  pTTT->lpszText= _T("??");  
			   break;
           case   IDC_EDIT7: //mEdit7Str 
			   UpdateData (TRUE);
			   if( mEdit7Str.GetLength() !=0 )
			   {				    
					mStr.Format(_T("����rpm:%.1f   @ϸ��%d@�����1.8��"), 300000.0f/(_wtoi(mEdit7Str)*_wtoi(mEdit3Str) ) ,_wtoi(mEdit3Str));					
					pTTT->lpszText=mStr.AllocSysString();
			   }
			   else  pTTT->lpszText= _T("??");  
			   break;
        }
        return   TRUE;
  }