#if !defined(AFX_MYSOCKET_H__5DABD08D_94A9_4AB5_9243_FE6A22AC2EE9__INCLUDED_)
#define AFX_MYSOCKET_H__5DABD08D_94A9_4AB5_9243_FE6A22AC2EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySocket.h : header file
//

#include <vector>

#define TCP_CONNECT_SUCCESS    1
#define TCP_CONNECT_FAILED     2
#define TCP_RX_RECEIVED        3

typedef struct _MyMSG_TYPE
{
	unsigned int cmd;
	std::vector<unsigned char> buffer;
} MyMSG_TYPE;

/////////////////////////////////////////////////////////////////////////////
// CMySocket command target

class CMySocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CMySocket();
	CMySocket(CWnd * dlg);
	virtual ~CMySocket();
	//
	void Init(HWND hWnd,int msgId);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMySocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	void PostMsg();
	void SendMsg();
	//
	HWND   DlgHwnd;
	int    CWndMsg;
	MyMSG_TYPE msg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__5DABD08D_94A9_4AB5_9243_FE6A22AC2EE9__INCLUDED_)
