// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "debug.h"

/////////////////////////////////////////////////////////////////////////////
// CMySocket

CMySocket::CMySocket()
{
}

CMySocket::~CMySocket()
{
	Close();
}

void CMySocket::Init(HWND hWnd,int msgId)
{
	DlgHwnd = hWnd;
	CWndMsg = msgId;
}

void CMySocket::PostMsg()
{
	::PostMessage(DlgHwnd,CWndMsg,(WPARAM)&msg,0);
}
void CMySocket::SendMsg()
{
	::SendMessage(DlgHwnd,CWndMsg,(WPARAM)&msg,0);
}



// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMySocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMySocket member functions

void CMySocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	DEBUGMSG(DEBUG_INFO,(TEXT("OnAccept()\r\n")));
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CMySocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	DEBUGMSG(DEBUG_INFO,(TEXT("OnClose()\r\n")));
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CMySocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	DEBUGMSG(DEBUG_INFO,(TEXT("OnConnect()\r\n")));
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySocket::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	DEBUGMSG(DEBUG_INFO,(TEXT("OnOutOfBandData()\r\n")));
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	msg.buffer.resize(256);

	int len = Receive(&msg.buffer[0],256);
	if(len == SOCKET_ERROR) {
		msg.buffer.resize(0);
	} else {
	    msg.buffer.resize(len);
		msg.cmd = TCP_RX_RECEIVED;
		SendMsg();
		//DEBUGMSG(DEBUG_INFO,(TEXT("OnReceive(%d):%s\r\n"),len,&msg.buffer[0]));
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void CMySocket::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	DEBUGMSG(DEBUG_INFO,(TEXT("OnSend()\r\n")));

	msg.cmd = TCP_CONNECT_SUCCESS;
	SendMsg();
	
	CAsyncSocket::OnSend(nErrorCode);
}
