

#ifndef __CMD_DATA_TYPE__
#define __CMD_DATA_TYPE__


#define IO_MAX   32

typedef    unsigned char  u8_t;
typedef    unsigned short u16_t;
typedef    unsigned int   u32_t;

#define   IO_MAX                 32
#define   IO_TIMING_SIZE_MAX     64

#define   CMD_ACK_OK                  1
#define   CMD_ACK_KO                  0


#define   CMD_GET_IO_OUT_VALUE           1
#define   CMD_GET_IO_IN_VALUE            2
#define   CMD_SET_IO_OUT_VALUE           3
#define   CMD_GET_IO_NAME                4
#define   CMD_SET_IO_NAME                5
#define   CMD_GET_TIMING_INFO            6
#define   CMD_SET_TIMING_INFO            7
#define   CMD_GET_RTC_VALUE              8
#define   CMD_SET_RTC_VALUE              9
#define   CMD_SET_TIMING_ON_MSK          10
#define   CMD_GET_TIMING_ON_MSK          11
#define   CMD_SET_INPUT_CTL_ON_MSK       12
#define   CMD_GET_INPUT_CTL_ON_MSK       13
#define   CMD_SET_HOST_ADDRESS           14
#define   CMD_GET_HOST_ADDRESS           15
#define   CMD_SET_SYSTEM_RESET           16
#define   CMD_REV_IO_SOME_BIT            36
#define   CMD_SET_IP_CONFIG              33
#define   CMD_GET_IP_CONFIG              34
#define   CMD_GET_INPUT_CTL_MODE_INDEX   40
#define   CMD_SET_INPUT_CTL_MODE_INDEX   41

#pragma pack(push)//保存对齐状态
#pragma pack(1)  //设置为1字节对齐

typedef struct _CmdHead
{
  u8_t  cmd;
  u16_t cmd_index;
  u8_t  cmd_option;
  u16_t cmd_len;
  u8_t  data_checksum;
} CmdHead;

void    SwapOrder_CmdHead(CmdHead * cmd);
#define GET_CMD_DATA(pCmd)     ((void *)(((char *)pCmd)+sizeof(CmdHead)))

//加密功能
typedef struct _CmdMcuId
{
  u8_t  IdLength;
  u8_t  IdValue[1];
} CmdMcuId;

typedef struct _CmdIoValue
{
  u8_t    io_count;
  u8_t    io_value[4];
} CmdIoValue;

typedef struct _CmdIoBit
{
  u8_t    io_index;
  u8_t    io_enable;
} CmdIoBit;

typedef struct _CmdIoIndex
{
  u8_t    io_msk[4];
} CmdIobitmap;

typedef struct _CmdMode
{
  u8_t    mode;
} CmdMode;

typedef struct _CmdInputMode
{
  u8_t    mode[8];
} CmdInputMode;

typedef struct _CmdInputModeIndex
{
	u8_t    index;
    u8_t    mode;
} CmdInputModeIndex;
#define INPUT_TRIGGER_FLIP_MODE          0x00  //触发反转模式
#define INPUT_SINGLE_TRIGGER_MODE        0x01  //单触发模式
#define INPUT_TRIGGER_TO_OPEN_MODE       0x02  //触发开通模式
#define INPUT_TRIGGER_TO_OFF_MODE        0x03  //触发关闭模式
#define INPUT_TRIGGER_OFF_MODE           0x07  //控制关闭


typedef struct _CmdRtcValue
{
  u8_t  second;
  u8_t  minute;
  u8_t  hour;
  u8_t  day;
  u8_t  month;
  u8_t  week;
  u8_t  year;
} CmdRtcValue;

#define REPEAT_ONCE        0
#define REPEAT_SECOND      1
#define REPEAT_MINUTE      2
#define REPEAT_HOUR        3
#define REPEAT_DAY         4
#define REPEAT_MONTH       5
#define REEAAT_YEAR        6
//
typedef struct _IoTimingType
{
  u8_t  index;
  u8_t  start_time[4];
  u8_t  repeat_type      : 3;

  u8_t  open_period_3    : 5;
  u8_t  open_period_2;
  u8_t  open_period_1    : 4;

  u8_t  repeat_times_2   : 4;
  u8_t  repeat_times_1   : 6;

  u8_t  repeat_delay_3   : 2;
  u8_t  repeat_delay_2;
  u8_t  repeat_delay_1   : 2;

  u8_t  io_count         : 6;
  u8_t  io_msk[IO_MAX/8];
  u8_t  check_sum;
} IoTimingType;

typedef struct _CmdIoTimingSize
{
  u8_t size;
} CmdIoTimingSize;

typedef struct _CmdIoTimingType
{
  u8_t  index;
  u32_t start_time;
  u8_t  repeat_type;
  u32_t open_period;
  u16_t repeat_times;
  u16_t repeat_delay;
  u8_t  io_count;
  u8_t  io_msk[IO_MAX/8];
  u8_t  check_sum;
} CmdIoTimingType;

void SwapOrder_CmdIoTimingType(CmdIoTimingType * citt);

typedef struct _CmdSetIoTiming
{
  IoTimingType  io_time;
} CmdSetIoTiming;
//---------------------配置区管理操作-----------------------------
typedef struct _CmdConfigDataInfo
{
  u16_t  start_addr;
  u16_t  config_length;
  u16_t  sector_size;
  u16_t  write_block_size;
  u16_t  read_block_size;
} CmdConfigDataInfo;

typedef struct _CmdConfigBlockData
{
  u16_t  block_addr;
  u16_t  block_size;
  u8_t   block_data[1];  //可能不止一个字节,只是为了操作方便
} CmdConfigBlockData;

typedef struct _CmdIpConfigData
{
  u8_t   ipaddr[4];
  u8_t   netmask[4];
  u8_t   gateway[4];
  u8_t   dns[4];
  u16_t  port;
  u16_t  webport;
} CmdIpConfigData;



typedef struct _host_address
{
	u8_t     type;  //type = 0:192.168.1.111....,type = 1 : www.hostaddr.com
	char     host_address[64];
	u16_t    port;
} host_address;

#define  HOST_ADDR_IS_IP_MODE       0
#define  HOST_ADDR_IS_DOMAIN_MODE   1
//
#define  HOST_IS_UDP                0
#define  HOST_IS_TCP_SERVER         1
#define  HOST_IS_TCP_CLIENT         2


#pragma pack(pop)//恢复对齐状态


#endif

