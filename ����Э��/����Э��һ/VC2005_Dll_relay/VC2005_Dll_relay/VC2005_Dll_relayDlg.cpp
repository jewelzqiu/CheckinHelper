// VC2005_Dll_relayDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VC2005_Dll_relay.h"
#include "VC2005_Dll_relayDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CVC2005_Dll_relayDlg �Ի���




CVC2005_Dll_relayDlg::CVC2005_Dll_relayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC2005_Dll_relayDlg::IDD, pParent)
	, command_str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC2005_Dll_relayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVC2005_Dll_relayDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CVC2005_Dll_relayDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CVC2005_Dll_relayDlg ��Ϣ�������

BOOL CVC2005_Dll_relayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CString in = _T("set_relay_val ETH:TCP:192.168.1.250:2000  R1:ON  R2:ON");
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(in);

	this->UpdateData(false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CVC2005_Dll_relayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVC2005_Dll_relayDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CVC2005_Dll_relayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}








///������DLLʹ��˵��
//DLL����ֻ��һ���������ַ��������Ҳ���ַ���������������������ַ����������ĳ���
int DeviceQuaryCommand(LPSTR quary,LPSTR out,int out_len);
#pragma   comment(lib, "RelayControlCtl.lib")
//----


void CVC2005_Dll_relayDlg::OnBnClickedButtonSend()
{
	CString   in_box;
	this->GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(in_box);
	CStringA  astr;
	astr = in_box;
	char * pbuffer = astr.GetBuffer(1024);
	DeviceQuaryCommand(pbuffer,pbuffer,1024);
	CString out_box;
	out_box = astr;
	GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowText(out_box);
}
