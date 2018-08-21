//********************************************************************************
//文件：PMScrollBar.h
//作者：feiren
//时间: 2011-8-20
//用途：封装表格的滚动条实现方法
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMSCROLLBAR_H__
#define __PMSCROLLBAR_H__

#include "PMNavigator.h"
#include "PMThread.h"

#pragma once
class PMScrollBar;
enum PMSBType//滚动条类型
{
	SCROLL_NONE     = 0x0000,
	SCROLL_VERTICAL = 0x0001,
	SCROLL_HORIZON  = 0x0010,
	SCROLL_BOTH		= 0x0011,
	SCROLL_AUTO		= 0x0100,
};

class PMScrollBar:public IInput,public ScreenGDI
{
public:
	PMScrollBar();
	virtual ~PMScrollBar(void);
	PMVoid		Initialize(IInput *father,HWND hWnd,IntRect &ContentRect,TJSON *LaferScroll);
	PMVoid		LoadPicture();
	PMVoid		SetPageValue(PMInt PageValue)	{mPageValue = PageValue;}
	PMInt		GetPageValue()					{return mPageValue;}
	PMVoid		SetMax		(PMInt max);
	PMInt		GetMax		()					{return mMax;}
	PMVoid		SetActualMax(PMInt ActualMax)	{mActualMax = ActualMax;}
	PMInt		GetActualMax()					{return mActualMax;}
	PMVoid		SetStep		(PMInt step)		{mStep = step;}
	PMInt		GetStep		()					{return mStep;}
	PMVoid		SetLargeStep(PMInt LargeStep)	{mLargeStep = LargeStep;}
	PMInt		GetLargeStep()					{return mLargeStep;}
	PMVoid		SetValue	(PMInt value);
	PMInt		GetValue	()					{return mValue;}
	PMInt		GetScope	();
	PMBool		IsVertical  ();
	PMBool		IsHorition  ();
	PMVoid		ReCalcParameter(PMBool bSetMax);
	PMVoid		Show();
	PMVoid		Hide();	
	PMVoid		Draw();		
private:
	PMBool		OnLButtonDBLCLK	(TInputData *EventData) ;	//鼠标左键双击
	PMBool		OnPaint			(TInputData *EventData) ;	//重新绘制
	PMBool		OnEnter			(TInputData *EventData) ;   //鼠标进入
	PMBool		OnLeave			(TInputData *EventData) ;	//鼠标出来
	PMBool		OnMouseMove		(TInputData *EventData) ;	//控件鼠标移动
	PMBool		OnMouseWheel	(TInputData *EventData) ;	//控件鼠标滚轮滚动
	PMBool		OnLButtonDown	(TInputData *EventData) ;	//鼠标左键按下	
	PMBool		OnLButtonUp		(TInputData *EventData) ;	//鼠标左键松开
	PMBool		OnSystemTimmer	(TInputData *EventData) ;   //定时器消息	
private:
	IntPoint	GetTiTaPosition	(TInputData *EventData);
	PMVoid		HandleVScroll	(PMInt iChange);
	PMVoid		HandleHScroll	(PMInt iChange);
	PMVoid      HandleScrollFinish();
	PMVoid		CalcAnchorRect();
	PMVoid		Clear();		
	PMVoid		ReCalcState		(PMInt x,PMInt y);
	PMVoid		ChangeState		(PMInt iState);
	PMVoid		MoveAnchor		(IntPoint &pt);	
	PMVoid		ChangeValue		(PMInt iChange,PMBool bRecalc=false);
	PMVoid		HandleChange	(PMInt iChange);
private:
	friend class PMScrollManager;
	enum TBTNState
	{
		BUTTON_DEFAULT,
		BUTTON_HOME_ENTER,
		BUTTON_HOME_PRESS,
		BUTTON_END_ENTER,
		BUTTON_END_PRESS,
		BUTTON_ANCHOR_ENTER,
		BUTTON_ANCHOR_PRESS,
		BUTTON_ANCHOR_HOME_SIDE_PRESS,
		BUTTON_ANCHOR_END_SIDE_PRESS,
	};
	PMSBType	mType;			//滚动条的类型
	PMInt		mMax;			//最大的
	PMInt		mActualMax;		//最大的
	PMInt		mStep;			//按钮的
	PMInt		mLargeStep;		//鼠标的
	PMInt		mPageValue;		//页大小
	PMInt		mValue;			//当前位置
	PMInt		mHomeScope;
	PMInt		mEndScope;	
	PMInt		mOldX;
	PMInt		mOldY;	
	PMLong		mTicc;
	TBTNState   mState;
	const PMInt mSmallAchorLen;
	PMTasks		mTasks;
	
	IntRect		mTiTaRect;					//滚动条GUI绘制范围
	TJSON  *	mLaferScroll;				//存储绘制的滚动条数据
	IntRect		mRectButtonHome;			//上按钮的位置范围
	IntRect		mRectButtonEnd;				//下按钮的位置范围
	IntRect		mRectAnchor;				//滚动子的位置范围	

	PMPicture*	m_ScrollBackGround;			//滚动条背景图片正常
	PMPicture*	m_ScrollButtonHome;			//滚动条上按钮图片正常
	PMPicture*	m_ScrollButtonHomePress;	//滚动条上按钮图片按下
	PMPicture*	m_ScrollButtonHomeEnter;	//滚动条上按钮图片鼠标进入

	PMPicture*	m_ScrollButtonEnd;			//滚动条下按钮图片正常
	PMPicture*	m_ScrollButtonEndPress;		//滚动条下按钮图片按下
	PMPicture*	m_ScrollButtonEndEnter;		//滚动条下按钮图片鼠标进入

	PMPicture*	m_ScrollAnchor;				//滚动条滚动子图片正常
	PMPicture*	m_ScrollAnchorPress;		//滚动条滚动子图片按下
	PMPicture*	m_ScrollAnchorEnter;		//滚动条滚动子图片鼠标进入
};

class PMScrollManager
{
public:	
	PMVoid		Initialize(IInput *father,HWND hWnd,IntRect &ContentRect,TJSON *jnScrollLafer,PMInt ScrollType);
	PMVoid		Show();
	PMScrollBar mHScrollBar;
	PMScrollBar mVScrollBar;
	PMNavigator mNavigator;
};
#endif