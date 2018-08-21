//********************************************************************************
//文件：PMBaseAction.h
//作者：feiren
//时间：2011-8-20
//用途：封装控件的标准操作
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMBASEACTION_H__
#define __PMBASEACTION_H__

#include "PMSpaceAlgorithm.h"
#pragma once


class PMBaseAction
{
public:
	PMBaseAction(void);
	virtual ~PMBaseAction(void);
	PMVoid  Destroy							();					//清除数据
	PMVoid  UnDo							();					//撤销命令
	PMVoid  ReDo							();					//重做命令
	TJSON * CreateCommand					(PMInt type);		//创建命令
	PMBool  ExcuteForward					(IntRect &OutObj);	//执行位置往前
	PMBool  ExcuteBackward					(IntRect &OutObj);	//执行位置往后	
	virtual PMVoid    ClearCommandList		(TJSONS &cmdList);  //删除命令的内存对象		
	virtual PMVoid    AddExcute				(TJSON *pAction);	//添加命令	
	virtual PMVoid    UnExcute				(TJSON &pAction)=0;	//反向执行命令
	virtual PMVoid    Excute				(TJSON &pAction)=0;	//执行命令
	virtual PMVoid    AddPosition			(IntRect &InObj);//添加位置信息
protected:	
	TJSONS					mUndoList;							//撤销列表
	TJSONS					mRedoList;							//重做列表
	TJSONS					mPositionList;						//往前的位置列表
	PMInt					mIndex;								//往后的位置列表
};

#endif