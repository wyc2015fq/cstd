// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHSYSTEMREGISTER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHSYSTEMREGISTER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef _CH_SYSTEM_REGISTER_H
#define _CH_SYSTEM_REGISTER_H

#ifdef CHSYSTEMREGISTER_EXPORTS
#define CPMREG_API __declspec(dllexport)
#else
#define CPMREG_API __declspec(dllimport)
#endif

#define IN 
#define OUT 
#define INOUT 

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef STDCALL
#define STDCALL __stdcall
#endif

#ifndef WCHAR
typedef wchar_t WCHAR;
#endif

#ifdef __cplusplus
extern "C"{
#endif

	#define ARRAY_LEN 64
	#define CODE_LEN  35

	typedef struct  
	{
		char m_keyName[ARRAY_LEN+1];
		char m_keyValue[ARRAY_LEN+1];
	}REG_KEY;

	typedef struct  
	{
		int		  m_nCount; //结构体大小
		REG_KEY*  m_pRegKey;
	}REG_CONTENT;

	/************************************************************************
	* 函数名称: CPM_GetLocalMachineCode
	* 功能描述: 获取本地机器码
	* 参数列表: mCode 机器码存放缓存
	* 返回值: 获取状态
	* 说  明:获取成功将返回true，失败则返回false
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_GetLocalMachineCode( OUT char (&mCode)[CODE_LEN] );

	/************************************************************************
	* 函数名称: CPM_InitReg
	* 功能描述: 初始化注册
	* 参数列表: cFile 注册文件
	* 返回值: 初始化状态
	* 说   明:此接口既可用于获取注册状态又可用于生成注册文件
			 当用于获取注册状态时，外部可调用CPM_GetRegStatus和CPM_GetRegContent获取
			 具体状态信息
			 当用于生成注册文件时，外部可调用CPM_BeginReg、CPM_SetRegContent、CPM_DelRegContent
			 和CPM_EndReg完成注册，注册信息将写入到 cFile 中
			 当不再使用本模块时调用CPM_UnInitReg进行反初始化
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_InitReg( IN const WCHAR (&cFile)[MAX_PATH] );

	/************************************************************************
	* 函数名称: CPM_GetRegStatus
	* 功能描述: 获取注册状态
	* 参数列表: void
	* 返回值: 返回是否注册
	* 说  明: 此接口用于获取整个注册状态
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_GetRegStatus();

	/************************************************************************
	* 函数名称: CPM_GetRegStatus
	* 功能描述: 获取注册内容
	* 参数列表: regID 注册标识			  	           
	* 返回值:  注册内容
	* 说  明: 
	*************************************************************************/
	CPMREG_API REG_CONTENT* STDCALL CPM_GetRegContent( IN int regID );

	/************************************************************************
	* 函数名称: CPM_BeginReg
	* 功能描述: 启动注册
	* 参数列表: mCode 机器码			  	           
	* 返回值:  
	* 说  明:  调用此接口会将mcode内容写入注册文件中
	          此后可调用CPM_SetRegContent和CPM_DelRegContent对注册内容进行增加和删除
			  调用CPM_EndReg将完成注册
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_BeginReg( IN const char (&mCode)[CODE_LEN] );

	/************************************************************************
	* 函数名称: CPM_SetRegContent
	* 功能描述: 设置注册内容
	* 参数列表: regID      注册标识
			  regContent 注册内容
	* 返回值:  
	* 说  明:  
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_SetRegContent( IN int regID, IN const REG_CONTENT& regContent );

	/************************************************************************
	* 函数名称: CPM_DelRegContent
	* 功能描述: 删除注册内容
	* 参数列表: regID      注册标识
	* 返回值:  
	* 说  明:  
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_DelRegContent( IN int regID );

	/************************************************************************
	* 函数名称: CPM_EndReg
	* 功能描述: 完成注册
	* 参数列表: 
	* 返回值:  
	* 说  明:  调用此接口将会把注册内容全部写入文件
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_EndReg( void );
	
	/************************************************************************
	* 函数名称: CPM_UnInitReg
	* 功能描述: 反初始化注册模块
	* 参数列表: 
	* 返回值:  
	* 说  明:  上层模块如调用了CPM_InitReg接口则调用此接口进行一个反初始化
	*************************************************************************/
	CPMREG_API BOOL STDCALL CPM_UnInitReg( void );

#ifdef __cplusplus
}
#endif


#endif
