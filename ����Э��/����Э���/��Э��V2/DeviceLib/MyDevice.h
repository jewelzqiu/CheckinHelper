#pragma once

#include <boost/shared_ptr.hpp>
#include <vector>

class ClassWaper
{
public:
	ClassWaper() {}
	virtual ~ClassWaper() {}
};

typedef struct _MSGPAG
{
	unsigned char              msg;
	CString                    str;
	void                    *  handle;
} msgpag;

#define COMMAND_SENDING             0x01
#define COMMAND_ACK_OK              0x02
#define COMMAND_ACK_KO              0x03
#define COMMAND_ACK_GET_IP_OK       0x04
#define CMD_ACK_GET_INPUT_MODE_OK   0x05
#define CMD_ACK_SET_INPUT_MODE_OK   0x06
#define COMMAND_ACK_GET_IO_OUT_OK   0x07
#define COMMAND_ACK_SET_IO_OUT_OK   0x08
#define COMMAND_ACK_GET_IO_IN_OK    0x09
#define COMMAND_ACK_REV_SOME_BIT    0x0A
#define COMMAND_ACK_GET_HOSTADDR    0x0B
#define COMMAND_ACK_RESET_SYSTEM    0x0C

typedef struct _msg_ipconfig
{
	CString  ipaddr;
	CString  netmask;
	CString  gateway;
	CString  dns;
	CString  port;
	CString  webport;
} msg_ipconfig;

typedef struct _msg_io_value
{
	unsigned int on_msk;
} msg_io_value;

typedef struct _msg_input_mode
{
	int index;
	int mode;
} msg_input_mode;

typedef struct _msg_io_bitmap
{
	int index;
	int on;
} msg_io_bitmap;

typedef boost::shared_ptr<ClassWaper> MyIoDevice;
typedef BOOL TxDataCall(std::vector<unsigned char> & tx,void * data); 
//创建设备
MyIoDevice CreateDevice(void);
void IoDevInitDevice(MyIoDevice dev,TxDataCall * txcall,void *data,HWND hWnd,int MsgId);
//Rx回调函数,必须被接收函数回调,否则设备不产生应答消息
BOOL IoDevRxCallBack(MyIoDevice dev,std::vector<unsigned char> & rx);
//设备接口
BOOL IoDevSendRevIoOutValueChannelX(MyIoDevice dev,unsigned char channel_msk);
BOOL IoDevSendQueryIoOutValue(MyIoDevice dev);
BOOL IoDevSendSetIoOutValue(MyIoDevice dev,unsigned char io_out);
BOOL IoDevSendQueryIoInValue(MyIoDevice dev);
BOOL IoDevSendSetInputCtrlModeIndex(MyIoDevice dev,unsigned char index,unsigned char input_mode);
BOOL IoDevSendSetInputCtrlModeIndex(MyIoDevice dev,unsigned char index,unsigned char input_mode);
BOOL IoDevSendQueryInputCtrlMode(MyIoDevice dev,unsigned char index);
BOOL IoDevSendSetIpConfig(MyIoDevice dev,DWORD ipaddr,DWORD netmask,DWORD gateway,DWORD dns,unsigned int port,unsigned int webport);
BOOL IoDevSendQueryIpConfig(MyIoDevice dev);
BOOL IoDevSendSetHostAddress(MyIoDevice dev,unsigned char hostaddr[64],unsigned int port,unsigned char type);
BOOL IoDevSendGetHostAddress(MyIoDevice dev);
BOOL IoDevSendResetDeviceSystem(MyIoDevice dev);

