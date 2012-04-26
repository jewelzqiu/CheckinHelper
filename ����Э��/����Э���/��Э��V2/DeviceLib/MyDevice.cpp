#include "StdAfx.h"
#include "MyDevice.h"
#include "MyDeviceDef.h"
#include "debug.h"

#define THISINFO       1

unsigned int m_send_cmd_index;

typedef struct _MyIoDevice
{
	//lock
	CRITICAL_SECTION  cs;
	//
	CString ipaddr;
	CString port;
	//
	unsigned char io_out;
	unsigned char io_in;
	//tx
	TxDataCall * txcall;
	void * txdata;
	//msg
	HWND   DlgHwnd;
	int    CWndMsg;
	//
} IoDevice;

void LockDev(IoDevice * dev)
{
	EnterCriticalSection(&dev->cs);
}

void UnlockDev(IoDevice * dev)
{
	LeaveCriticalSection(&dev->cs);
}

class ClassWaperIoDevice : public ClassWaper
{
public:
	ClassWaperIoDevice() : handle(new IoDevice)
	{
	    TRCEIN();
		IoDevice * pdev = (IoDevice *)handle;
		InitializeCriticalSection(&pdev->cs);
	    TRCEOUT();
	}
	virtual ~ClassWaperIoDevice()
	{
	    TRCEIN();
		IoDevice * pdev = (IoDevice *)handle;
		DeleteCriticalSection(&pdev->cs);
		delete pdev;
		handle = NULL;
	    TRCEOUT();
	}
	IoDevice * GetPtr(void) { return (IoDevice *)handle; }
private:
	void * handle;
};

#define MyDeviceToIoDevice(dev)   (((ClassWaperIoDevice *)((dev).get()))->GetPtr())

MyIoDevice CreateDevice(void)
{
	TRCEIN();
	ClassWaper * pdevice = new ClassWaperIoDevice;
	boost::shared_ptr<ClassWaper> ret(pdevice);
	TRCEOUT();
	return ret;
}

void IoDevInitDevice(MyIoDevice dev,TxDataCall * txcall,void *data,HWND hWnd,int MsgId)
{
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	LockDev(pdev);
	pdev->txcall  = txcall;
	pdev->txdata  = data;
	pdev->DlgHwnd = hWnd;
	pdev->CWndMsg = MsgId;
	UnlockDev(pdev);
}


//
//包装消息
//
void IoDevPostWndMsg(MyIoDevice dev,msgpag & msg)
{
	IoDevice * pdev = MyDeviceToIoDevice(dev);
    LockDev(pdev);
	::PostMessage(pdev->DlgHwnd,pdev->CWndMsg,(WPARAM)&msg,0);
	UnlockDev(pdev);
}

static msgpag          gmsg;
static msg_ipconfig    mipconfig;
static msg_input_mode  ginputmode;
static msg_io_value    gioallmsk;
//
void IoDevChangeStatus(MyIoDevice dev,unsigned int mode,CString str)
{
	gmsg.msg = mode;
	gmsg.str = str;
	IoDevPostWndMsg(dev,gmsg);
}

BOOL IoDevTxData(MyIoDevice dev,std::vector<unsigned char> & buffer)
{
	BOOL ret;
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	LockDev(pdev);
	if(!pdev->txcall) {
		UnlockDev(pdev);
		return FALSE;
	}
	ret = (*pdev->txcall)(buffer,pdev->txdata); 
	UnlockDev(pdev);
	return ret;
}

//-------------------------------------------------------------------------
//接口,查询继电器状态
BOOL IoDevSendQueryIoOutValue(MyIoDevice dev)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead));
	CmdHead * pcmd = (CmdHead *)&buffer[0];
	pcmd->cmd = CMD_GET_IO_OUT_VALUE;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = 0;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送查询命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallQueryIoOutValue(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdIoValue)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			CmdIoValue * pio = (CmdIoValue *)GET_CMD_DATA(pcmd);
			gioallmsk.on_msk = pio->io_value[0];
			gmsg.str.Format(_T("查询输出口成功."));
			gmsg.msg = COMMAND_ACK_GET_IO_OUT_OK;
			gmsg.handle = &gioallmsk;
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//设置输出值
//
BOOL IoDevSendSetIoOutValue(MyIoDevice dev,unsigned char io_out)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(CmdIoValue));
	CmdHead    * pcmd = (CmdHead *)&buffer[0];
	CmdIoValue * pio  = (CmdIoValue *)GET_CMD_DATA(pcmd);
	pio->io_count = 8;
	pio->io_value[0]  = io_out;
	pio->io_value[1]  = 0;
	pio->io_value[2]  = 0;
	pio->io_value[3]  = 0;
	pcmd->cmd = CMD_SET_IO_OUT_VALUE;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(CmdIoValue);
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送设置命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallSetIoOutValue(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdIoValue)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			CmdIoValue * pio  = (CmdIoValue *)GET_CMD_DATA(pcmd);
			gmsg.msg = COMMAND_ACK_SET_IO_OUT_OK;
			gmsg.str = _T("输出更新成功.");
			gmsg.handle = &gioallmsk;
			gioallmsk.on_msk = pio->io_value[0];
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//
//翻转输出值
//
BOOL IoDevSendRevIoOutValueChannelX(MyIoDevice dev,unsigned char channel_msk)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(CmdIobitmap));
	CmdHead    * pcmd = (CmdHead *)&buffer[0];
	CmdIobitmap * pio = (CmdIobitmap *)GET_CMD_DATA(pcmd);
	pio->io_msk[0]  = channel_msk;
	pio->io_msk[1]  = 0;
	pio->io_msk[2]  = 0;
	pio->io_msk[3]  = 0;
	pcmd->cmd = CMD_REV_IO_SOME_BIT;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(CmdIobitmap);
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送设置命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallRevIoOutValueChannelX(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdIoBit)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			CmdIobitmap * pio = (CmdIobitmap *)GET_CMD_DATA(pcmd);
			CString bitmap;
			unsigned int i;
			for(i=0;i<8;i++) {
				if(pio->io_msk[0]&(1<<i)) {
					bitmap = bitmap + _T("1");
				} else {
					bitmap = bitmap + _T("0");
				}
			}
			gmsg.str.Format(_T("触发位号:%s"),bitmap);
			gmsg.msg = COMMAND_ACK_REV_SOME_BIT;
			gmsg.handle = &gioallmsk;
			gioallmsk.on_msk = pio->io_msk[0];
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//查询输入
//
BOOL IoDevSendQueryIoInValue(MyIoDevice dev)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead));
	CmdHead * pcmd = (CmdHead *)&buffer[0];
	pcmd->cmd = CMD_GET_IO_IN_VALUE;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = 0;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送查询命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallQueryIoInValue(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdIoValue)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			CmdIoValue * pio = (CmdIoValue *)GET_CMD_DATA(pcmd);
			gioallmsk.on_msk = pio->io_value[0];
			gmsg.str.Format(_T("查询输入口成功."));
			gmsg.msg = COMMAND_ACK_GET_IO_IN_OK;
			gmsg.handle = &gioallmsk;
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//设置工作模式
//
BOOL IoDevSendSetInputCtrlModeIndex(MyIoDevice dev,unsigned char index,unsigned char input_mode)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(CmdInputModeIndex));
	CmdHead          * pcmd = (CmdHead *)&buffer[0];
    CmdInputModeIndex * pio = (CmdInputModeIndex *)GET_CMD_DATA(pcmd);
	pcmd->cmd = CMD_SET_INPUT_CTL_MODE_INDEX;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(CmdInputModeIndex);
	pio->mode  = input_mode;
	pio->index = index;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送设置命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallSetInputCtrlMode(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("设置InputControlMode成功."));
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//查询工作模式
//
BOOL IoDevSendQueryInputCtrlMode(MyIoDevice dev,unsigned char index)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(CmdInputModeIndex));
	CmdHead          * pcmd = (CmdHead *)&buffer[0];
    CmdInputModeIndex * pio = (CmdInputModeIndex *)GET_CMD_DATA(pcmd);
	pcmd->cmd = CMD_GET_INPUT_CTL_MODE_INDEX;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(CmdInputModeIndex);
	pio->index = index;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送查询命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckCallQueryInputCtrlMode(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdInputModeIndex)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			CmdInputModeIndex * pio = (CmdInputModeIndex *)GET_CMD_DATA(pcmd);
			//DEBUGINFO(THISINFO,(TEXT("读取IndexControl成功:%d"),COMMAND_ACK_GET_INPUT_CONTROL_MODE_OK));
			gmsg.msg = CMD_ACK_GET_INPUT_MODE_OK;
			gmsg.handle = &ginputmode;
			gmsg.str = _T("读取输入控制模式成功.");
			ginputmode.index = pio->index;
			ginputmode.mode = pio->mode;
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//设置IP地址
//
BOOL IoDevSendSetIpConfig(MyIoDevice dev,DWORD ipaddr,DWORD netmask,DWORD gateway,DWORD dns,unsigned int port,unsigned int webport)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(CmdIpConfigData));
	CmdHead        * pcmd = (CmdHead *)&buffer[0];
	CmdIpConfigData * pio = (CmdIpConfigData *)GET_CMD_DATA(pcmd);
	pcmd->cmd = CMD_SET_IP_CONFIG;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(CmdIpConfigData);
	//
	pio->ipaddr[0] = (unsigned char)(ipaddr>>0)&0xFF;
	pio->ipaddr[1] = (unsigned char)(ipaddr>>8)&0xFF;
	pio->ipaddr[2] = (unsigned char)(ipaddr>>16)&0xFF;
	pio->ipaddr[3] = (unsigned char)(ipaddr>>24)&0xFF;
	//
	pio->netmask[0] = (unsigned char)(netmask>>0)&0xFF;
	pio->netmask[1] = (unsigned char)(netmask>>8)&0xFF;
	pio->netmask[2] = (unsigned char)(netmask>>16)&0xFF;
	pio->netmask[3] = (unsigned char)(netmask>>24)&0xFF;
	//
	pio->gateway[0] = (unsigned char)(gateway>>0)&0xFF;
	pio->gateway[1] = (unsigned char)(gateway>>8)&0xFF;
	pio->gateway[2] = (unsigned char)(gateway>>16)&0xFF;
	pio->gateway[3] = (unsigned char)(gateway>>24)&0xFF;
	//
	pio->dns[0] = (unsigned char)(dns>>0)&0xFF;
	pio->dns[1] = (unsigned char)(dns>>8)&0xFF;
	pio->dns[2] = (unsigned char)(dns>>16)&0xFF;
	pio->dns[3] = (unsigned char)(dns>>24)&0xFF;
	//
	pio->port = port;
	pio->webport = webport;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送设置命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}

void AckCallSetIpConfig(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
		    IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("设置IpConfig成功."));
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//查询IP地址
//
BOOL IoDevSendQueryIpConfig(MyIoDevice dev)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead));
	CmdHead * pcmd = (CmdHead *)&buffer[0];
	pcmd->cmd = CMD_GET_IP_CONFIG;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = 0;
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送查询内部IP命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}

void AckCallGetIpConfig(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)+sizeof(CmdIpConfigData)) {
		CmdIpConfigData * pio = (CmdIpConfigData *)GET_CMD_DATA(pcmd);
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			DEBUGINFO(THISINFO,(TEXT("GetIpConfig:ip:%d.%d.%d.%d,netmask:%d.%d.%d.%d"),
				pio->ipaddr[0],pio->ipaddr[1],pio->ipaddr[2],pio->ipaddr[3],
				pio->netmask[0],pio->netmask[1],pio->netmask[2],pio->netmask[3]));
			mipconfig.ipaddr.Format(_T("%d.%d.%d.%d"),pio->ipaddr[0],pio->ipaddr[1],pio->ipaddr[2],pio->ipaddr[3]);
			mipconfig.netmask.Format(_T("%d.%d.%d.%d"),pio->netmask[0],pio->netmask[1],pio->netmask[2],pio->netmask[3]);
			mipconfig.gateway.Format(_T("%d.%d.%d.%d"),pio->gateway[0],pio->gateway[1],pio->gateway[2],pio->gateway[3]);
			mipconfig.dns.Format(_T("%d.%d.%d.%d"),pio->dns[0],pio->dns[1],pio->dns[2],pio->dns[3]);
			mipconfig.port.Format(_T("%d"),pio->port);
			mipconfig.webport.Format(_T("%d"),pio->webport);
			gmsg.msg = COMMAND_ACK_GET_IP_OK;
			gmsg.handle = &mipconfig;
			gmsg.str = _T("读取IpConfig成功.");
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//设置主机的地址
//
BOOL IoDevSendSetHostAddress(MyIoDevice dev,unsigned char hostaddr[64],unsigned int port,unsigned char type)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead)+sizeof(host_address));
	CmdHead        * pcmd = (CmdHead *)&buffer[0];
	host_address    * pio = (host_address *)GET_CMD_DATA(pcmd);
	pcmd->cmd = CMD_SET_HOST_ADDRESS;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = sizeof(host_address);
	//
	memcpy(pio->host_address,hostaddr,64);
	pio->port = port;
	pio->type = type;

	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送设置命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}

void AckSetHostAddress(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
		    IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("设置主机地址成功."));
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//读取主机的地址
//
BOOL IoDevSendGetHostAddress(MyIoDevice dev)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead));
	CmdHead        * pcmd = (CmdHead *)&buffer[0];
	pcmd->cmd = CMD_GET_HOST_ADDRESS;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = 0;
	//
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送读取命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}

void AckGetHostAddress(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	static host_address ack_host_addr;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			if(len >= (sizeof(CmdHead)+sizeof(host_address)) && pcmd->cmd_len >= sizeof(host_address)) {
				host_address    * pio = (host_address *)GET_CMD_DATA(pcmd);
				memcpy(&ack_host_addr,pio,sizeof(host_address));
		        IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("读取主机地址成功."));
			    gmsg.msg = COMMAND_ACK_GET_HOSTADDR;
			    gmsg.handle = &ack_host_addr;
			    gmsg.str = _T("读取主机地址成功.");
			    IoDevPostWndMsg(dev,gmsg);
			} else {
				IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("ERROR:读取主机地址返回数据无效!"));
			}
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//
//重启设备操作系统
//
BOOL IoDevSendResetDeviceSystem(MyIoDevice dev)
{
	BOOL ret;
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	std::vector<unsigned char> buffer;
	buffer.resize(sizeof(CmdHead));
	CmdHead        * pcmd = (CmdHead *)&buffer[0];
	pcmd->cmd = CMD_SET_SYSTEM_RESET;
	pcmd->cmd_index = ++m_send_cmd_index;
	pcmd->cmd_len = 0;
	//
	IoDevChangeStatus(dev,COMMAND_SENDING,_T("正在发送重启命令..."));
	ret = IoDevTxData(dev,buffer);
	TRCEOUT();
	return ret;
}
void AckResetDeviceSystem(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	TRCEIN();
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	CmdHead * pcmd = (CmdHead *)&rx[0];
	if(len >= sizeof(CmdHead)) {
		if(pcmd->cmd_option == CMD_ACK_OK && pcmd->cmd_index == (u16_t)m_send_cmd_index) {
			IoDevChangeStatus(dev,COMMAND_ACK_OK,_T("重启系统成功,请稍后重新连接设备."));
			gmsg.msg = COMMAND_ACK_RESET_SYSTEM;
			gmsg.handle = NULL;
			gmsg.str = _T("重启系统成功,请稍后重新连接设备.");
			IoDevPostWndMsg(dev,gmsg);
		} else {
			IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令执行失败，请再次尝试执行此命令！"));
		}
	} else {
		IoDevChangeStatus(dev,COMMAND_ACK_KO,_T("命令返回数据异常，请再次尝试执行此命令！"));
	}
	TRCEOUT();
}
//---------------------------------------------------------------------------
//专门处理接收
//然后处理应答包，分别调用应答处理函数
//
typedef void AckCall(MyIoDevice,std::vector<unsigned char> & rx);
struct IoDeviceAckCall
{
	int       cmd;
	AckCall * ack;
};

struct IoDeviceAckCall  IoDevAcks[] =
{
	{CMD_REV_IO_SOME_BIT,AckCallRevIoOutValueChannelX},
	{CMD_SET_IO_OUT_VALUE,AckCallSetIoOutValue},
	{CMD_GET_IO_IN_VALUE,AckCallQueryIoInValue},
	{CMD_GET_IO_OUT_VALUE,AckCallQueryIoOutValue},
	//配置命令
	{CMD_GET_INPUT_CTL_MODE_INDEX,AckCallQueryInputCtrlMode},
	{CMD_SET_INPUT_CTL_MODE_INDEX,AckCallSetInputCtrlMode},
	{CMD_SET_IP_CONFIG,AckCallSetIpConfig},
	{CMD_GET_IP_CONFIG,AckCallGetIpConfig},
	{CMD_SET_HOST_ADDRESS,AckSetHostAddress},
	{CMD_GET_HOST_ADDRESS,AckGetHostAddress},
	{CMD_SET_SYSTEM_RESET,AckResetDeviceSystem},
	{0,NULL}
};

void DumpCmdHead(CmdHead * pcmd)
{
	CString cmd;
	cmd = _T("CmdHead:");
	unsigned int i;
	for(i=0;i<sizeof(CmdHead);i++) {
		CString str;
		if(i) cmd += _T(",");
		str.Format(_T("0x%x"),((unsigned char *)pcmd)[i]);
		cmd += str;
	}
	DEBUGINFO(THISINFO,(TEXT("%s"),cmd));
}

BOOL IoDevRxCallBack(MyIoDevice dev,std::vector<unsigned char> & rx)
{
	//发送消息
	IoDevice * pdev = MyDeviceToIoDevice(dev);
	int len = (int)rx.size();
	unsigned char * buffer = &rx[0];
	if(len >= sizeof(CmdHead)) {
		CmdHead * pcmd = (CmdHead *)buffer;
		DumpCmdHead(pcmd);
		unsigned int i = 0;
		while(1) {
			if(pcmd->cmd == IoDevAcks[i].cmd) {
			   if(IoDevAcks[i].ack) {
				  (*IoDevAcks[i].ack)(dev,rx);
				  break;
			   }
			}
			if(IoDevAcks[i].cmd == 0 && IoDevAcks[i].ack == NULL) {
				break;
			}
			i++;
		}
	} else {
		DEBUGMSG(DEBUG_ERROR,(TEXT("len < sizeof(CmdHead) ERROR!\r\n")));
	}
	return TRUE;
}
