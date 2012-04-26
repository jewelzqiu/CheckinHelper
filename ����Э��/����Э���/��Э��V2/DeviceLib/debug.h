
#ifndef __DEBUG_H__
#define __DEBUG_H__

//---------------------------
//可以修改这些定义
#define DEBUG_ALL      1
//
#define DEBUG_INFO     1
#define DEBUG_FUNC     0
#define DEBUG_ERROR    1
#define DEBUG_WARN     1
//---------------------------

#if DEBUG_ALL

#define DEBUGMSG(on,str)	\
	if(on)do{   \
		CString _str;   \
        _str.Format##str##;   \
		OutputDebugString(CString(_str));   \
	}while(0)

#define DEBUGINFO   DEBUGMSG   

#define TRCEIN()    DEBUGMSG(DEBUG_INFO,(TEXT("---> { %s\r\n"),_T(__FUNCTION__)))
#define TRCEOUT()   DEBUGMSG(DEBUG_INFO,(TEXT("<--- } %s\r\n"),_T(__FUNCTION__)))

#else
#define DEBUGMSG(on,str) do{}while(0)
	
#define TRCEIN()   do{}while(0)
#define TRCEOUT()  do{}while(0)

#endif



//----------------------------
#define  ASSERTMSG_ON        1
#define  ASSERTMSG_LINE_ON   1
#define  ASSERTMSG_FILE_ON   1
//----------------------------

#if ASSERTMSG_ON && ASSERTMSG_LINE_ON && ASSERTMSG_FILE_ON
#define  ASSERTMSG(valid,str) \
	if(!valid)do{   \
	    CStringA _strpre,_str;   \
		_strpre.Format("ASSERT failed in file:%s,line:%d,",__FILE__,__LINE__);  \
        _str.Format##str##;   \
		OutputDebugString(CStringW(_strpre+_str));   \
		ASSERT(0);  \
	}while(0)
#elif ASSERTMSG_ON && ASSERTMSG_FILE_ON
	if(!valid)do{   \
	    CStringA _strpre,_str;   \
		_strpre.Format("ASSERT failed in file:%s,",__FILE__);  \
        _str.Format##str##;   \
		OutputDebugString(CStringW(_strpre+_str));   \
		ASSERT(0);  \
	}while(0)
#elif ASSERTMSG_ON && ASSERTMSG_LINE_ON
	if(!valid)do{   \
	    CStringA _strpre,_str;   \
		_strpre.Format("ASSERT failed in line:%d,",__LINE__);  \
        _str.Format##str##;   \
		OutputDebugString(CStringW(_strpre+_str));   \
		ASSERT(0);  \
	}while(0)
#elif ASSERTMSG_ON
	if(!valid)do{   \
		CStringA _str;   \
        _str.Format##str##;   \
		_str = "ASSERT failed," + _str;  \
		OutputDebugString(CStringW(_str));   \
		ASSERT(0);  \
	}while(0)
#else
#define ASSERTMSG(on,str) \
	do{}while(0)
#endif



#endif
