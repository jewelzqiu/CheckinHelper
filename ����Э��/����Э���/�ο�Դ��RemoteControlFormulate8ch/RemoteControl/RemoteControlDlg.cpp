// RemoteControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteControl.h"
#include "RemoteControlDlg.h"
#include "MyDeviceDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "debug.h"

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRemoteControlDlg 对话框




CRemoteControlDlg::CRemoteControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRemoteControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MY_IO_DEV_MESSAGE,OnMyDeviceMessage)
	ON_MESSAGE(WM_MY_SOCKET_MESSAGE,OnMySocketMessage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CRemoteControlDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRemoteControlDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CRemoteControlDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CH1, &CRemoteControlDlg::OnBnClickedButtonCh1)
	ON_BN_CLICKED(IDC_BUTTON_CH2, &CRemoteControlDlg::OnBnClickedButtonCh2)
	ON_BN_CLICKED(IDC_BUTTON_CH3, &CRemoteControlDlg::OnBnClickedButtonCh3)
	ON_BN_CLICKED(IDC_BUTTON_CH4, &CRemoteControlDlg::OnBnClickedButtonCh4)
	ON_BN_CLICKED(IDC_BUTTON_CH5, &CRemoteControlDlg::OnBnClickedButtonCh5)
	ON_BN_CLICKED(IDC_BUTTON_CH6, &CRemoteControlDlg::OnBnClickedButtonCh6)
	ON_BN_CLICKED(IDC_BUTTON_CH7, &CRemoteControlDlg::OnBnClickedButtonCh7)
	ON_BN_CLICKED(IDC_BUTTON_CH8, &CRemoteControlDlg::OnBnClickedButtonCh8)
	ON_BN_CLICKED(IDC_BUTTON_FLASH_OUT, &CRemoteControlDlg::OnBnClickedButtonFlashOut)
	ON_BN_CLICKED(IDC_BUTTON_FLASH_IN, &CRemoteControlDlg::OnBnClickedButtonFlashIn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TRI_ALL, &CRemoteControlDlg::OnBnClickedButtonTriAll)
	ON_BN_CLICKED(IDC_BUTTON_SEL_ON, &CRemoteControlDlg::OnBnClickedButtonSelOn)
	ON_BN_CLICKED(IDC_BUTTON_SEL_OFF, &CRemoteControlDlg::OnBnClickedButtonSelOff)
END_MESSAGE_MAP()


LRESULT CRemoteControlDlg::OnMyDeviceMessage(WPARAM wParam, LPARAM lParam)
{
	msgpag * msg = (msgpag *)wParam;
	DEBUGMSG(DEBUG_INFO,(TEXT("My IoDevice AckMsg(%d)"),msg->msg));
	switch(msg->msg) 
	{
	case COMMAND_SENDING:
		DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_SENDING Ack OK")));
		SetSysMode(SYS_WAIT_CMD_ACK,msg->str);
		break;
	case COMMAND_ACK_OK:
		DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_OK Ack OK")));
		SetSysMode(SYS_CMD_SUCCESS,msg->str);
		break;
	case COMMAND_ACK_KO:
		DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_KO Ack OK")));
		SetSysMode(SYS_CMD_SUCCESS,msg->str);
		break;
	case COMMAND_ACK_GET_IP_OK:
		{
			DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_GET_IP_OK Ack OK")));
		    SetSysMode(SYS_CMD_SUCCESS,msg->str);
		}
		break;
	case CMD_ACK_GET_INPUT_MODE_OK:
		{
		    SetSysMode(SYS_CMD_SUCCESS,msg->str);
		}
	case COMMAND_ACK_GET_IO_OUT_OK:
		{
			DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_GET_IO_OUT_OK Ack OK")));
			msg_io_value  * ioch = (msg_io_value *)msg->handle;
			out_msk = ioch->on_msk;
		    SetSysMode(SYS_CMD_SUCCESS,msg->str);
			m_dlgparam.check_box_on = out_msk;
			UpdataDialogParam();
			RewDrawOutView();
		}
		break;
	case COMMAND_ACK_SET_IO_OUT_OK:
		{
			DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_SET_IO_OUT_OK Ack OK")));
			msg_io_value  * ioch = (msg_io_value *)msg->handle;
			out_msk = ioch->on_msk;
		    SetSysMode(SYS_CMD_SUCCESS,msg->str);
			m_dlgparam.check_box_on = out_msk;
			UpdataDialogParam();
			RewDrawOutView();
		}
		break;
	case COMMAND_ACK_GET_IO_IN_OK:
		{
			DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_GET_IO_IN_OK Ack OK")));
			msg_io_value  * ioch = (msg_io_value *)msg->handle;
			in_msk = ioch->on_msk;
		    SetSysMode(SYS_CMD_SUCCESS,msg->str);
			RewDrawInView();
		}
		break;
	case COMMAND_ACK_REV_SOME_BIT:
		{
			DEBUGMSG(DEBUG_INFO,(TEXT("COMMAND_ACK_SET_IO_IN_OK Ack OK")));
			//绘制窗口
			msg_io_value  * ioch = (msg_io_value *)msg->handle;
			out_msk ^=  ioch->on_msk;
			SetSysMode(SYS_CMD_SUCCESS,msg->str); //这一行规更新了下面的变量，必须先执行
			m_dlgparam.check_box_on = out_msk; 
			UpdataDialogParam();
			RewDrawOutView();
		}
		break;
	default:
		DEBUGMSG(DEBUG_INFO,(TEXT("OnMyDeviceMessage Unknow Msg(%d)"),msg->msg));
		SetSysMode(SYS_CMD_SUCCESS,_T("返回未能识别的应答!"));
		break;
	}
	return 0;
}
LRESULT CRemoteControlDlg::OnMySocketMessage(WPARAM wParam, LPARAM lParam)
{
	MyMSG_TYPE * msg = (MyMSG_TYPE *)wParam;
	switch(msg->cmd)
	{
	case TCP_CONNECT_SUCCESS:
		SocketConnected();
		break;
	case TCP_RX_RECEIVED:
		DEBUGMSG(DEBUG_INFO,(TEXT("RxReceive(%d bytes)"),msg->buffer.size()));
		SocketReceive(msg->buffer);
		break;
	default:
		break;
	}
	return 0;
}

BOOL TxCallBackFunction(std::vector<unsigned char> & tx,void * data)
{
	CRemoteControlDlg * pthis = (CRemoteControlDlg *)data;
	return pthis->m_socket.Send(&tx[0],(int)tx.size());
}

void CRemoteControlDlg::SocketConnected(void)
{
	SetSysMode(SYS_CONNECT_OK,_T("连接成功."));
	m_device = CreateDevice();
	IoDevInitDevice(m_device,TxCallBackFunction,this,this->GetSafeHwnd(),WM_MY_IO_DEV_MESSAGE);
}

void CRemoteControlDlg::SocketReceive(std::vector<unsigned char> & buffer)
{
	IoDevRxCallBack(m_device,buffer);
}



// CRemoteControlDlg 消息处理程序

BOOL CRemoteControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_socket.Init(this->GetSafeHwnd(),WM_MY_SOCKET_MESSAGE);

	SetSysMode(SYS_NO_CONNECT,_T("未连接!"));

	m_socket_is_opened = false;

	m_dlgparam.ipaddr = _T("192.168.1.250");
	m_dlgparam.port   = _T("2000");
	UpdataDialogParam();
	out_msk = 0;
	in_msk = 0;
	m_dlgparam.check_box_on = 0;

	this->SetTimer(NULL,1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemoteControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CRemoteControlDlg::RewDrawOutView(void)
{
	DrawOutValueViewChannelX(0,(out_msk&(1<<0))?true:false);
	DrawOutValueViewChannelX(1,(out_msk&(1<<1))?true:false);
	DrawOutValueViewChannelX(2,(out_msk&(1<<2))?true:false);
	DrawOutValueViewChannelX(3,(out_msk&(1<<3))?true:false);
	DrawOutValueViewChannelX(4,(out_msk&(1<<4))?true:false);
	DrawOutValueViewChannelX(5,(out_msk&(1<<5))?true:false);
	DrawOutValueViewChannelX(6,(out_msk&(1<<6))?true:false);
	DrawOutValueViewChannelX(7,(out_msk&(1<<7))?true:false);
}
void CRemoteControlDlg::RewDrawInView(void)
{
	DrawInValueViewChannelX(0,(in_msk&(1<<0))?true:false);
	DrawInValueViewChannelX(1,(in_msk&(1<<1))?true:false);
	DrawInValueViewChannelX(2,(in_msk&(1<<2))?true:false);
	DrawInValueViewChannelX(3,(in_msk&(1<<3))?true:false);
}


void CRemoteControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	RewDrawOutView();
	RewDrawInView();
	//
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CRemoteControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRemoteControlDlg::OnBnClickedOk()
{
}

void CRemoteControlDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CRemoteControlDlg::SetSysMode(unsigned int mode,CString status)
{
	sys_state = mode;

	m_idle_time = 0;

	ReadDialogParam();

	m_dlgparam.status = _T("状态:") + status;

    switch(sys_state)
    {
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
		m_dlgparam.connectbuttun = _T("连接");
		break;
	case SYS_CONNECT_OK:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_CMD_SUCCESS:
	case SYS_LINK_PINGING:
		m_dlgparam.connectbuttun = _T("断开");
    default:
        break;
    }

	UpdataDialogParam();

    switch(sys_state)
    {
	case SYS_NO_CONNECT:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_NO_CONNECT----")));
		break;
	case SYS_CONNECT_FAILED:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_CONNECT_FAILED----")));
		break;
	case SYS_CONNECT_OK:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_CONNECT_PINGING----")));
		break;
	case SYS_CONNECTING:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_CONNECTING----")));
		break;
	case SYS_WAIT_CMD_ACK:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_WAIT_CMD_ACK----")));
		break;
	case SYS_CMD_SUCCESS:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_CMD_SUCCESS----")));
		break;
	case SYS_LINK_PINGING:
		DEBUGMSG(DEBUG_INFO,(TEXT("----------SYS_LINK_PINGING----")));
		break;
    default:
        break;
    }
}


void CRemoteControlDlg::ReadDialogParam(void)
{
	GetDlgItem(IDC_EDIT_IPADDR)->GetWindowText(m_dlgparam.ipaddr);
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(m_dlgparam.port);
	m_dlgparam.check_box_on = 0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH1))->GetCheck()==1)?(1<<0):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH2))->GetCheck()==1)?(1<<1):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH3))->GetCheck()==1)?(1<<2):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH4))->GetCheck()==1)?(1<<3):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH5))->GetCheck()==1)?(1<<4):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH6))->GetCheck()==1)?(1<<5):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH7))->GetCheck()==1)?(1<<6):0;
	m_dlgparam.check_box_on |= (((CButton*)GetDlgItem(IDC_CHECK_CH8))->GetCheck()==1)?(1<<7):0;
}
void CRemoteControlDlg::UpdataDialogParam(void)
{
	GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText(m_dlgparam.ipaddr);
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(m_dlgparam.port);
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(m_dlgparam.status);
	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(m_dlgparam.connectbuttun);
	unsigned int msk = m_dlgparam.check_box_on;
#if 0
	DEBUGMSG(DEBUG_INFO,(TEXT("UpdataDialogParam checkbox(0x%x)"),msk));
	((CButton*)GetDlgItem(IDC_CHECK_CH1))->SetCheck((msk&(1<<0))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH2))->SetCheck((msk&(1<<1))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH3))->SetCheck((msk&(1<<2))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH4))->SetCheck((msk&(1<<3))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH5))->SetCheck((msk&(1<<4))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH6))->SetCheck((msk&(1<<5))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH7))->SetCheck((msk&(1<<6))?1:0);
	((CButton*)GetDlgItem(IDC_CHECK_CH8))->SetCheck((msk&(1<<7))?1:0);
#endif
}

void CRemoteControlDlg::DrawOutValueViewChannelX(unsigned char channel,bool on)
{
	static const int idcmap[8] = 
	{
		IDC_STATIC_OUT_CH1,
		IDC_STATIC_OUT_CH2,
		IDC_STATIC_OUT_CH3,
		IDC_STATIC_OUT_CH4,
		IDC_STATIC_OUT_CH5,
		IDC_STATIC_OUT_CH6,
		IDC_STATIC_OUT_CH7,
		IDC_STATIC_OUT_CH8,
	};
	if(channel >= 8) {
		return ;
	}
	int id = idcmap[channel];
	CDC * pDC = GetDlgItem(id)->GetDC();
	///CDC   dcOn,dcOff;
	//dcOn.CreateCompatibleDC(pDC);
	//dcOff.CreateCompatibleDC(pDC);
	RECT rect;
    GetDlgItem(id)->GetClientRect(&rect);
    

	//CBitmap * pOldOnBitmap = dcOn.SelectObject(&m_yellolightBitmap);
	//CBitmap * pOldOffBitmap = dcOff.SelectObject(&m_greyBitmap);
	//
	if(on) {
	    //pDC->BitBlt(0,0,m_rectShowPic.Width(),m_rectShowPic.Height(),&dcOn,0,0,SRCCOPY);
		pDC->FillSolidRect(&rect, RGB(255, 0, 0));
	} else {
		//pDC->BitBlt(0,0,m_rectShowPic.Width(),m_rectShowPic.Height(),&dcOff,0,0,SRCCOPY);
		pDC->FillSolidRect(&rect, RGB(200, 200, 200));
	}
	//
	//dcOn.SelectObject(pOldOnBitmap);
	//dcOff.SelectObject(pOldOffBitmap);
	this->ReleaseDC(pDC);
}
void CRemoteControlDlg::DrawInValueViewChannelX(unsigned char channel,bool on)
{
#if 0
	static const int idcmap[5] = 
	{
		IDC_STATIC_IN1,
		IDC_STATIC_IN2,
		IDC_STATIC_IN3,
		IDC_STATIC_IN4,
	};
	if(channel >= 4) {
		return ;
	}
	int id = idcmap[channel];
	CDC * pDC = GetDlgItem(id)->GetDC();
	RECT rect;
    GetDlgItem(id)->GetClientRect(&rect);
	if(on) {
		pDC->FillSolidRect(&rect, RGB(255, 0, 0));
	} else {
		pDC->FillSolidRect(&rect, RGB(200, 200, 200));
	}
	this->ReleaseDC(pDC);
#endif
}


void CRemoteControlDlg::OnBnClickedButtonConnect()
{
	ReadDialogParam();
	if(m_socket_is_opened) {
		SetSysMode(SYS_NO_CONNECT,_T("未连接!"));
		DEBUGMSG(DEBUG_INFO,(TEXT("关闭连接")));
		m_socket.Close();
		m_socket_is_opened = false;
	} else {
	  CStringA str8;
	  str8 = m_dlgparam.port;
	  UINT port = atoi(str8.GetBuffer());
	  DEBUGMSG(DEBUG_INFO,(TEXT("CreateTCP(%s,%d)\r\n"),m_dlgparam.ipaddr,port));
	  SetSysMode(SYS_CONNECTING,_T("正在连接中!"));
	  m_socket_is_opened = true;
	  m_socket.Create();
	  m_socket.Connect(m_dlgparam.ipaddr,port);
	}
}


void CRemoteControlDlg::OnBnClickedButtonChN(unsigned int ch)
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }

	SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));

	IoDevSendRevIoOutValueChannelX(m_device,ch);
}

void CRemoteControlDlg::OnBnClickedButtonCh1()
{
	OnBnClickedButtonChN(1<<0);
}

void CRemoteControlDlg::OnBnClickedButtonCh2()
{
	OnBnClickedButtonChN(1<<1);
}

void CRemoteControlDlg::OnBnClickedButtonCh3()
{
	OnBnClickedButtonChN(1<<2);
}

void CRemoteControlDlg::OnBnClickedButtonCh4()
{
	OnBnClickedButtonChN(1<<3);
}

void CRemoteControlDlg::OnBnClickedButtonCh5()
{
	OnBnClickedButtonChN(1<<4);
}
void CRemoteControlDlg::OnBnClickedButtonCh6()
{
	OnBnClickedButtonChN(1<<5);
}
void CRemoteControlDlg::OnBnClickedButtonCh7()
{
	OnBnClickedButtonChN(1<<6);
}
void CRemoteControlDlg::OnBnClickedButtonCh8()
{
	OnBnClickedButtonChN(1<<7);
}

void CRemoteControlDlg::OnBnClickedButtonTriAll()
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }

	SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));

	IoDevSendRevIoOutValueChannelX(m_device,0xFF);
}


void CRemoteControlDlg::OnBnClickedButtonFlashOut()
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }

	SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));

	IoDevSendQueryIoOutValue(m_device);
}

void CRemoteControlDlg::OnBnClickedButtonSelOn()
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }
	ReadDialogParam();
	SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));
	DEBUGMSG(DEBUG_INFO,(TEXT("Open(0x%x)"),m_dlgparam.check_box_on));
	IoDevSendSetIoOutValue(m_device,m_dlgparam.check_box_on);
}

void CRemoteControlDlg::OnBnClickedButtonSelOff()
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }

	//SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));

	//IoDevSendSetIoOutValue(m_device,0x00);

}



void CRemoteControlDlg::OnBnClickedButtonFlashIn()
{
    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		break;
	case SYS_NO_CONNECT:
	case SYS_CONNECT_FAILED:
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
	case SYS_LINK_PINGING:
    default:
        return ;
    }

	SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));

	IoDevSendQueryIoInValue(m_device);
}

void CRemoteControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool query_in = false;

	if(m_idle_time < 60*5) {
		++m_idle_time;
	}

    switch(sys_state)
    {
	case SYS_CONNECT_OK:
	case SYS_CMD_SUCCESS:
		{
			//DEBUGMSG(DEBUG_INFO,(TEXT("SUCC:QueryIN")));
			if(m_idle_time >= 5) {
			    SetSysMode(SYS_WAIT_CMD_ACK,_T("等待应答....."));
				if(0) { //query_in) {
					IoDevSendQueryIoInValue(m_device);
				} else {
					IoDevSendQueryIoOutValue(m_device);
				}
				query_in = !query_in;
			}
		}
		break;
	case SYS_CONNECTING:
	case SYS_WAIT_CMD_ACK:
		{
			if(m_idle_time >= 10) {
			  OnBnClickedButtonConnect();
			}
		}
		break;
	case SYS_CONNECT_FAILED:
		{
			if(m_idle_time >= 30) {
			  OnBnClickedButtonConnect();
			}
		}
		break;
	case SYS_NO_CONNECT:
		{
			if(m_idle_time >= 15) {
			  OnBnClickedButtonConnect();
			}
		}
		break;
	case SYS_LINK_PINGING:
    default:
        break;
    }
	CDialog::OnTimer(nIDEvent);
}


