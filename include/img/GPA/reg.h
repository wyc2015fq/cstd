
#pragma once

#ifdef __cplusplus
extern "C"{
#endif
	
	typedef enum
	{
		eNORMALREG   = 0, //正常注册使用中
		eOUTDATAREG  = 1, //注册过期
		eNOREG       = 2  //未注册
	}REGSTATUS;

#ifndef BOOL 
#define BOOL int
#endif

	/************************************************************************
	* 函数名称: CPM_GetRegisterStatus
	* 功能描述: 获取软件注册状态
	* 参数列表: file 密钥保存文件
	* 返回值: 返回注册状态(true 注册成功 false 注册失败)
	* 说  明: 
	*************************************************************************/
	__declspec(dllexport) REGSTATUS  CPM_GetRegStatus();

	/************************************************************************
	* 函数名称: CPM_RegisterSys
	* 功能描述: 软件注册
	* 参数列表: file 密钥保存文件
	* 返回值: 返回注册状态(true 注册成功 false 注册失败)
	* 说  明: 
	*************************************************************************/
	__declspec(dllexport) BOOL  CPM_Register();

#ifdef __cplusplus
}
#endif


