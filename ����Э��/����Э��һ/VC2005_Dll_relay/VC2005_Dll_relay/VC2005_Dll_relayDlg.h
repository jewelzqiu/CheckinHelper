// VC2005_Dll_relayDlg.h : ͷ�ļ�
//

#pragma once


// CVC2005_Dll_relayDlg �Ի���
class CVC2005_Dll_relayDlg : public CDialog
{
// ����
public:
	CVC2005_Dll_relayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_VC2005_DLL_RELAY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSend();
public:
	CString command_str;
public:
	CString ack_string;
};
