// VC2005_Dll_relay.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVC2005_Dll_relayApp:
// �йش����ʵ�֣������ VC2005_Dll_relay.cpp
//

class CVC2005_Dll_relayApp : public CWinApp
{
public:
	CVC2005_Dll_relayApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVC2005_Dll_relayApp theApp;