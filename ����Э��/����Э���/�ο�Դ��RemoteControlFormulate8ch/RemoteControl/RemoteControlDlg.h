// RemoteControlDlg.h : 头文件
//

#pragma once


#include "MySocket.h"
#include "MyDevice.h"

#define SYS_NO_CONNECT         0x00
#define SYS_CONNECT_OK         0x01
#define SYS_CONNECTING         0x02
#define SYS_WAIT_CMD_ACK       0x03
#define SYS_CMD_SUCCESS        0x04
#define SYS_LINK_PINGING       0x05
#define SYS_CONNECT_FAILED     0x06



struct MainDlalogPramList
{
	CString  ipaddr;
	CString  port;
	CString  connectbuttun;
	//
	CString  status;
	//
	unsigned int check_box_on;
};

// CRemoteControlDlg 对话框
class CRemoteControlDlg : public CDialog
{
	friend BOOL TxCallBackFunction(std::vector<unsigned char> & tx,void * data);
// 构造
public:
	CRemoteControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTECONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
	void SetSysMode(unsigned int mode,CString status);
	void ReadDialogParam(void);
	void UpdataDialogParam(void);
	void SocketConnected(void);
	void SocketReceive(std::vector<unsigned char> & buffer);
	void OnBnClickedButtonChN(unsigned int ch);
	void DrawOutValueViewChannelX(unsigned char channel,bool on);
	void DrawInValueViewChannelX(unsigned char channel,bool on);
	void RewDrawOutView(void);
	void RewDrawInView(void);
private:
	struct MainDlalogPramList  m_dlgparam;
	CMySocket  m_socket;
	BOOL       m_socket_is_opened;
	MyIoDevice m_device;
	unsigned int m_idle_time;
	int          sys_state;
	unsigned char out_msk;
	unsigned char in_msk;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMyDeviceMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMySocketMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedButtonConnect();
public:
	afx_msg void OnBnClickedButtonCh1();
public:
	afx_msg void OnBnClickedButtonCh2();
public:
	afx_msg void OnBnClickedButtonCh3();
public:
	afx_msg void OnBnClickedButtonCh4();
public:
	afx_msg void OnBnClickedButtonCh5();
	afx_msg void OnBnClickedButtonCh6();
	afx_msg void OnBnClickedButtonCh7();
	afx_msg void OnBnClickedButtonCh8();
public:
	afx_msg void OnBnClickedButtonFlashOut();
public:
	afx_msg void OnBnClickedButtonFlashIn();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnBnClickedButtonTriAll();
public:
	afx_msg void OnBnClickedButtonSelOn();
public:
	afx_msg void OnBnClickedButtonSelOff();
};
