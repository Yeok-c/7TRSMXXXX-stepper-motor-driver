
// 7TRSM5730ToolDlg.h : 头文件
//

#pragma once
#include "Modbus.h"

#define WM_REC_MODBUS_RTU_DATA_MSG  WM_USER+2016
#define MODBUS_SCAN_TIME            WM_USER+2017


// CMy7TRSM5730ToolDlg 对话框
class CMy7TRSM5730ToolDlg : public CDialogEx
{
// 构造
public:
	CMy7TRSM5730ToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY7TRSM5730TOOL_DIALOG };
	int  mSerialId;
	BOOL mPortState;
	BOOL mReadMotorState;
	UINT mSubdivide;

	CString mEdit1Str;
	CString mEdit2Str;
	CString mEdit3Str;
	CString mEdit4Str;
	CString mEdit5Str;
	CString mEdit6Str;
	CString mEdit7Str;
	CString mEdit8Str;
	CString mEdit9Str;
	CString mEdit10Str;
	CString mEdit11Str;
	CString mEdit12Str;
	CString mEdit13Str;
	CString mEdit14Str;
	CString mEdit15Str;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	void ReadPortConfig();
	void WritePortConfig();
	static void WINAPI ReceiveFromModbusRtuDevice(unsigned char data,int id,void* puser);

	unsigned char SendFunction03(unsigned char mDeviceADdr,unsigned short mRegStartAddr,unsigned short mRegNumber,CModBusCmd03Res *pModBusCmd03Res);
	unsigned char SendFunction16(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegNumber,CModBusCmd16 *pModBusCmd16, unsigned int mResTimeOut );
	unsigned char SendFunction06(unsigned char mDeviceAddr,unsigned short mRegStartAddr,unsigned short mRegData, unsigned int mResTimeOut );
	void ReadMotorState();
	void DisResErro(unsigned char mRes);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTip(UINT id,NMHDR *pNMHDR,LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedOpenSerialBtn();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnEnSetfocusEdit6();
	afx_msg void OnEnSetfocusEdit7();
};
