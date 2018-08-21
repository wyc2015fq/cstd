//********************************************************************************
//文件：PMNavigator.h
//作者：feiren
//时间：2011-8-20
//用途：封装转跳器,点击既可以转跳相应的地方
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMNAVIGATOR_H__
#define __PMNAVIGATOR_H__

#include "PMScreenGDI.h"
#include "PMInputManager.h"
#include "PMScrollConst.h"
#include "PMPictureManager.h"
#pragma once

class PMNavigator:public IInput,public ScreenGDI
{
public:
	PMNavigator();
	virtual ~PMNavigator();
	PMVoid		Initialize		(IInput *father,HWND hWnd,IntRect &ClientRect,TJSON *LaferNavigator);
	PMVoid		SetScope		(IntSize &Scope);			//设置范围
	PMVoid		SetScopeWidth	(PMInt ScopeWidth);			//设置范围
	PMVoid		SetScopeHeiht	(PMInt ScopeHeight);		//设置范围
	PMVoid		SetNavigator	(IntRect &Navigator);		//设置显示区域
	PMVoid		SetNavigatorX	(IntSize &NavigatorX);		//设置显示区域
	PMVoid		SetNavigatorY	(IntSize &NavigatorY);		//设置显示区域
	PMVoid		Draw();	
	PMVoid		Show();
private:
	PMBool		OnLButtonDown	(TInputData *EventData);	//鼠标左键按下		
	PMBool		OnMouseMove		(TInputData *EventData);	//鼠标移动
	IntPoint	GetTiTaPosition	(TInputData *EventData);
private:
	IntRect		GetDrawRect();
	PMVoid		CalcNavigator(PMInt x,PMInt y);
	PMVoid		Clear();
	IntRect		mTitaRect;
	IntSize		mScope;
	IntRect		mNavigator;
	PMPicture*	m_NavigatorBackGround;			//转跳器背景
	PMInt		mLineColor;
	TJSON	*	mLaferNavigator;
};

#endif
