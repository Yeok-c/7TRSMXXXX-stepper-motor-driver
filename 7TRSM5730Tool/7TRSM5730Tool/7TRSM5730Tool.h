
// 7TRSM5730Tool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy7TRSM5730ToolApp:
// �йش����ʵ�֣������ 7TRSM5730Tool.cpp
//

class CMy7TRSM5730ToolApp : public CWinApp
{
public:
	CMy7TRSM5730ToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy7TRSM5730ToolApp theApp;