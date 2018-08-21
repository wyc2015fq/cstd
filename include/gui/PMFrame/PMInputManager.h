//********************************************************************************
//文件：PMInputManager.h
//作者：feiren
//时间：2011-8-20
//用途：所有窗口的鼠标键盘消息
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMINPUTMANAGER_H__
#define __PMINPUTMANAGER_H__
#include "PMSpaceAlgorithm.h"
#include "PMCliboardManager.h"
#pragma once

#define  WM_SCROLL_MOVE WM_USER + 1

class PMInputManager;
class IInput;
enum PMMouseButton
{
	Mouse_Left = 0,
	Mouse_Right,
	Mouse_Middle	
};


class TInputData
{
public:
	enum SystemKey
	{
		VLK_BACK	= 0x08,
		VLK_TAB		= 0x09,
		VLK_CLEAR	= 0x0C,	//\r
		VLK_RETURN	= 0x0D,	//\n
		VLK_SHIFT	= 0x10,
		VLK_CONTROL = 0x11,
		VLK_ALT		= 0x12,
		VLK_PAUSE	= 0x13,
		VLK_ESCAPE	= 0x1B,
		VLK_SPACE	= 0x20,
		VLK_PRIOR	= 0x21,
		VLK_NEXT	= 0x22,
		VLK_END		= 0x23,
		VLK_HOME	= 0x24,
		VLK_LEFT	= 0x25,
		VLK_UP		= 0x26,
		VLK_RIGHT	= 0x27,
		VLK_DOWN	= 0x28,
		VLK_PRINT	= 0x2A,
		VLK_INSERT	= 0x2D,
		VLK_DELETE	= 0x2E,
		VLK_LWIN	= 0x5B,
		VLK_RWIN	= 0x5C,
		VLK_SLEEP	= 0x5F,
		VLK_F1		= 0x70,
		VLK_F2		= 0x71,
		VLK_F3		= 0x72,
		VLK_F4		= 0x73,
		VLK_F5		= 0x74,
		VLK_F6		= 0x75,
		VLK_F7		= 0x76,
		VLK_F8		= 0x77,
		VLK_F9		= 0x78,
		VLK_F10		= 0x79,
		VLK_F11		= 0x7A,
		VLK_F12		= 0x7B,
		VLK_NUMLOCK = 0x90,
		VLK_SCROLL	= 0x91
	};
	TInputData();
	~TInputData();
	HWND						mHwnd;	
	PMUnint						mMessage;
	PMInt						mMouseX;
	PMInt						mMouseY;
	PMInt						mMouseZ;
	PMInt						mOldMouseX;
	PMInt						mOldMouseY;
	PMInt						mSizeWidth;
	PMInt						mSizeHeight;
	PMLong						mTimmerTick;
	PMChar						mKey[4];
	TJSON*						mUserInfo;
	IntRect						mPainRect;
	VectorString				mDropFiles;
	set<PMChar>					mKeyState;			//按下而没有松开的按键
	set<PMMouseButton>			mButtonState;		//按下而没有松开的鼠标键
	PMBool	IsLButtonPress	();
	PMBool	IsControlPress	();
	PMBool	IsAltPress		();
	PMBool	IsShiftPress	();	
	PMVoid  Clear			();
};

class IInput
{
public:
	IInput(IInput *father=NULL);
	virtual ~IInput();
	virtual PMBool OnSetFocus		(TInputData *EventData) {return false;}	//控件得到焦点
	virtual PMBool OnLostFocus		(TInputData *EventData) {return false;}	//控件失去焦点
	virtual PMBool OnMouseDrag		(TInputData *EventData) {return false;}	//控件开始拖拽
	virtual PMBool OnDragOver		(TInputData *EventData) {return false;}	//控件拖拽经过
	virtual PMBool OnMouseDrop		(TInputData *EventData) {return false;}	//控件拖拽放下
	virtual PMBool OnMouseMove		(TInputData *EventData) {return false;}	//控件鼠标移动
	virtual PMBool OnMouseWheel		(TInputData *EventData) {return false;}	//控件鼠标滚轮滚动
	virtual PMBool OnLButtonDown	(TInputData *EventData) {return false;}	//鼠标左键按下
	virtual PMBool OnLButtonDBLCLK	(TInputData *EventData) {return false;}	//鼠标左键双击
	virtual PMBool OnRButtonDown	(TInputData *EventData) {return false;}	//鼠标右键按下
	virtual PMBool OnRButtonDBLCLK	(TInputData *EventData) {return false;}	//鼠标右键双击
	virtual PMBool OnMButtonDown	(TInputData *EventData) {return false;}	//鼠标中间键按下
	virtual PMBool OnLButtonUp		(TInputData *EventData) {return false;}	//鼠标左键松开
	virtual PMBool OnRButtonUp		(TInputData *EventData) {return false;}	//鼠标右键松开
	virtual PMBool OnMButtonUp		(TInputData *EventData) {return false;}	//鼠标中间键松开
	virtual PMBool OnKeyDown		(TInputData *EventData) {return false;}	//键盘非系统键按下
	virtual PMBool OnKeyUp			(TInputData *EventData) {return false;}	//键盘非系统键松开
	virtual PMBool OnChar			(TInputData *EventData) {return false;}	//键盘普通输入
	virtual PMBool OnIMEChar		(TInputData *EventData) {return false;}	//键盘输入法输入
	virtual PMBool OnSize			(TInputData *EventData) {return false;}	//大小改变
	virtual PMBool OnPaint			(TInputData *EventData) {return false;}	//重新绘制
	virtual PMBool OnDropFiles		(TInputData *EventData) {return false;}	//从explorer拖拽文件到控件
	virtual PMBool OnSystemTimmer	(TInputData *EventData) {return false;} //定时器消息
	virtual PMBool OnCaretTimmer	(TInputData *EventData) {return false;} //插入符号定时器消息
	virtual PMBool OnGifTimmer		(TInputData *EventData) {return false;} //GIF变化定时器
	virtual PMBool OnEnter			(TInputData *EventData) {return false;} //鼠标进入
	virtual PMBool OnLeave			(TInputData *EventData) {return false;} //鼠标出来
	virtual PMBool OnUserMessage	(TInputData *EventData) {return false;}

	virtual PMVoid OnHScroll		(PMInt iRange)			{ }	//水平滚动条滚动
	virtual PMVoid OnVScroll		(PMInt iRange)			{ }	//竖直滚动条滚动
	virtual PMVoid OnHScrollFinish	()						{ } //滚动条滚动完成
	virtual PMVoid OnVScrollFinish	()						{ } //滚动条滚动完成
	virtual PMVoid OnNavigate		(IntRect &Navigator)	{ }	//改变浏览窗口
public:
	PMVoid	SetFather				(IInput* _Father);
	PMVoid	SetSize					(const IntRect &_Boundary);
	PMVoid	SetMargin				(const IntRect &_Margin);
	PMVoid	SetZorder				(PMInt _Zorder);
	PMVoid	SetVisible				(PMBool _Visible);	
	IntRect &GetMargin				();
	IntRect &GetSize				();
	IInput*	GetFather				(); 
	PMInt	GetZorder				(); 
	PMBool	GetVisible				(); 
	PMBool  GetFocus				();		
protected:
	PMVoid  HandleSystemTimmer		(TInputData *EventData);	
	PMVoid  RegisterWidget			();
	PMVoid  UnRegisterWidget		();
	IInput *GetFocusInput			(TInputData *EventData);
	IInput *GetCurrentInput			(TInputData *EventData);
	friend class PMInputManager;
	IntRect mMargin;
	IntRect mBoundary;
	IInput* mFather;
	PMInt   mZorder;
	PMBool  mVisible;
	PMBool  mbFocus;
	IInput *		mCurrentWidget;	
	IInput *		mFocusWidget;			
	vector<IInput*> mChildrenWidgets;		
};


class PMInputManager
{
public:	
	PMInputManager();
	~PMInputManager();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static PMInputManager *GetSingleton();
	static PMVoid  Initialize(HINSTANCE hInstance,HWND hWnd);
	PMVoid RegisterWidget		(IInput *widget);
	PMVoid UnRegisterWidget		(IInput *widget);
	PMVoid CreateSystemTimmer	(HWND hWnd);
	PMVoid ResetCaretTimmer		(HWND hWnd);
protected:
	PMVoid HandleTimmer			( WPARAM wParam, LPARAM lParam );	
	PMVoid HandleMouseMove		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleMouseWheel		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleLButtonDown	( WPARAM wParam, LPARAM lParam );
	PMVoid HandleLButtonDBLCLK	( WPARAM wParam, LPARAM lParam );
	PMVoid HandleRButtonDown	( WPARAM wParam, LPARAM lParam );
	PMVoid HandleRButtonDBLCLK	( WPARAM wParam, LPARAM lParam );
	PMVoid HandleMButtonDown	( WPARAM wParam, LPARAM lParam );
	PMVoid HandleLButtonUp		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleRButtonUp		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleMButtonUp		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleKeyDown		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleKeyUp			( WPARAM wParam, LPARAM lParam );
	PMVoid HandleChar			( WPARAM wParam, LPARAM lParam );
	PMVoid HandleIMEChar		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleSize			( WPARAM wParam, LPARAM lParam );
	PMVoid HandlePaint			( WPARAM wParam, LPARAM lParam );
	PMVoid HandleDropFiles		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleSetFocus		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleKillFocus		( WPARAM wParam, LPARAM lParam );
	PMVoid HandleUserMessage	( WPARAM wParam, LPARAM lParam );
private:
	PMVoid  CalcCurrentInput	();
	PMVoid  CalcFocusInput		();
	PMBool			mbFocus;				//当前的窗体是否具有焦点
	IInput *		mCurrentWidget;			//当前鼠标所在的控件
	IInput *		mFocusWidget;			//当前具有焦点的控件
	IntRect			mBoundary;				//当前窗体客户区的大小
	TInputData		mInputData;				//消息发送的数据存储结构
	vector<IInput*> mInputWidgets;			//只存储father的控件
	PMUnint			CommonTimmerID ;		//普通定时器的ID
	PMUnint			CaretTimmerID  ;		//插入光标定时器的ID
	PMUnint			GifTimmerID  ;		//插入光标定时器的ID
};


#endif