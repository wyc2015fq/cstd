#include "StdAfx.h"
#include "PMInputManager.h"
#include "PMScreenGDI.h"

#define GET_HIWORD(param) ((short)HIWORD(param))
#define GET_LOWORD(param) ((short)LOWORD(param))


IInput::IInput(IInput *father):mFather(father),mZorder(0),mVisible(true),mbFocus(false)
{
	mFocusWidget=this;
	mCurrentWidget=this;
	RegisterWidget();	
}

IInput::~IInput()
{
	UnRegisterWidget();	
}

PMVoid IInput::SetFather( IInput* _Father )
{
	UnRegisterWidget();	
	mFather = _Father;
	RegisterWidget();
}

PMVoid IInput::SetSize( const IntRect &_Boundary )
{
	mBoundary = _Boundary;
}

PMVoid IInput::SetMargin( const IntRect &_Margin )
{
	mMargin = _Margin;
}

PMVoid IInput::SetZorder( PMInt _Zorder )
{
	mZorder = _Zorder;
}

IntRect & IInput::GetMargin()
{
	return mMargin;
}

IntRect & IInput::GetSize()
{
	return mBoundary;
}

IInput* IInput::GetFather()
{
	return mFather;
}

PMInt IInput::GetZorder()
{
	return mZorder;
}

PMVoid IInput::SetVisible( PMBool _Visible )
{
	mVisible = _Visible;
}

PMBool IInput::GetVisible()
{
	return mVisible;
}

PMBool IInput::GetFocus()
{
	return mbFocus;
}

IInput * IInput::GetFocusInput( TInputData *EventData )
{
	if(mChildrenWidgets.empty())
	{		
		mFocusWidget = this;
		return mFocusWidget;
	}else
	{
		//1、如果是自己的话需要重新计算
		if(mFocusWidget == this||!mFocusWidget->mBoundary.inside(EventData->mMouseX,EventData->mMouseY))
		{
			PMBool bFocus = true;
			for (vector<IInput*>::iterator it = mChildrenWidgets.begin(); it!=mChildrenWidgets.end();it++)
			{
				if((*it)->mVisible&&(*it)->mBoundary.inside(EventData->mMouseX,EventData->mMouseY))
				{
					mFocusWidget = *it;				
					bFocus = false;
					break;
				}
			}
			if(bFocus)mFocusWidget = this;
		}
		return mFocusWidget;
	}
}

IInput * IInput::GetCurrentInput( TInputData *EventData )
{
	if(mChildrenWidgets.empty())
	{
		mCurrentWidget = this;
		return mCurrentWidget;
	}else
	{
		//1、不变
		if(mCurrentWidget!=this&&mCurrentWidget->mBoundary.inside(EventData->mMouseX,EventData->mMouseY))
		{
			return mCurrentWidget;
		}
		//2、有改变
		for (vector<IInput*>::iterator it = mChildrenWidgets.begin(); it!=mChildrenWidgets.end();it++)
		{
			if((*it)->mVisible&&(*it)->mBoundary.inside(EventData->mMouseX,EventData->mMouseY))
			{
				mCurrentWidget = *it;
				return mCurrentWidget;
			}
		}
		//3、变回father自己
		mCurrentWidget = this;
		return mCurrentWidget;
	}
}

PMVoid IInput::UnRegisterWidget()
{
	if(mFather)
	{
		remove(mFather->mChildrenWidgets.begin(),mFather->mChildrenWidgets.end(),this);
		if(mFather->mFocusWidget == this)
		{
			mFather->mFocusWidget = mFather;
		}
	}else
	{
		PMInputManager::GetSingleton()->UnRegisterWidget(this);
	}
}

PMVoid IInput::RegisterWidget()
{
	if(mFather)
	{
		mFather->mChildrenWidgets.push_back(this);
	}else
	{
		PMInputManager::GetSingleton()->RegisterWidget(this);
	}
}

PMVoid IInput::HandleSystemTimmer( TInputData *EventData )
{
	OnSystemTimmer(EventData);
	for (vector<IInput*>::iterator it = mChildrenWidgets.begin(); it!=mChildrenWidgets.end();it++)
	{
		(*it)->OnSystemTimmer(EventData);
	}
}

PMInputManager * PMInputManager::GetSingleton()
{
	static PMInputManager InputManager;
	return &InputManager;
}

PMVoid PMInputManager::RegisterWidget( IInput *widget )
{
	mInputWidgets.push_back(widget);
	mFocusWidget = widget;
}

PMVoid PMInputManager::UnRegisterWidget( IInput *widget )
{
	remove(mInputWidgets.begin(),mInputWidgets.end(),widget);
	if(mFocusWidget == widget)
	{
		if(mInputWidgets.size()>0)
		{
			mFocusWidget = *mInputWidgets.begin();
		}else
		{
			mFocusWidget = NULL;
		}
	}
}

LRESULT CALLBACK PMInputManager::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PMInputManager *InputManager	  = PMInputManager::GetSingleton();	
	InputManager->mInputData.mHwnd	  = hWnd;
	InputManager->mInputData.mMessage = message;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_MOUSEMOVE:
		{				
			InputManager->HandleMouseMove(wParam,lParam);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			InputManager->HandleMouseWheel(wParam,lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			InputManager->HandleLButtonDown(wParam,lParam);
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			InputManager->HandleLButtonDBLCLK(wParam,lParam);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			SetCapture(hWnd);
			InputManager->HandleRButtonDown(wParam,lParam);
		}
		break;
	case WM_RBUTTONDBLCLK:
		{
			InputManager->HandleRButtonDBLCLK(wParam,lParam);
		}
		break;
	case WM_MBUTTONDOWN:
		{
			SetCapture(hWnd);
			InputManager->HandleMButtonDown(wParam,lParam);
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			InputManager->HandleLButtonUp(wParam,lParam);
		}
		break;
	case WM_RBUTTONUP:
		{
			ReleaseCapture();
			InputManager->HandleRButtonUp(wParam,lParam);
		}
		break;
	case WM_MBUTTONUP:
		{
			ReleaseCapture();
			InputManager->HandleMButtonUp(wParam,lParam);
		}
		break;
	case WM_TIMER:
		{
			InputManager->HandleTimmer(wParam,lParam);		
		}
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		{
			InputManager->HandleKeyDown(wParam,lParam);
		}
		break;
	case WM_CHAR:
		{
			InputManager->HandleChar(wParam,lParam);
		}
		break;
	case WM_IME_CHAR:
		{
			InputManager->HandleIMEChar(wParam,lParam);
		}
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		{
			InputManager->HandleKeyUp(wParam,lParam);
		}
		break;
	case WM_PAINT:		
		{
			hdc = BeginPaint(hWnd, &ps); 
			Assign(InputManager->mInputData.mPainRect,ps.rcPaint);
			InputManager->HandlePaint(wParam,lParam);
			EndPaint(hWnd, &ps); 
		}
		break;
	case WM_DROPFILES:
		{
			InputManager->HandleDropFiles(wParam,lParam);
		}
		break;
	case WM_KILLFOCUS:
		{
			InputManager->HandleKillFocus(wParam,lParam);
		}
		break;
	case WM_SETFOCUS:
		{
			InputManager->HandleSetFocus(wParam,lParam);
		}
		break;
	case WM_SIZE:
		{
			InputManager->HandleSize(wParam,lParam);
		}
		break;	
	case WM_SCROLL_MOVE:
		{
			InputManager->HandleUserMessage(wParam,lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

PMVoid PMInputManager::HandleMouseMove( WPARAM wParam, LPARAM lParam )
{

	mInputData.mMouseX = GET_LOWORD(lParam);
	mInputData.mMouseY = GET_HIWORD(lParam);
	if (mFocusWidget&&mInputData.IsLButtonPress())//如果左键按下则是mFocusWidget处理事件
	{
		mFocusWidget->OnMouseMove(&mInputData);
	}else
	{
		CalcCurrentInput();
		if(mCurrentWidget)
		{			
			mCurrentWidget->OnMouseMove(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleMouseWheel( WPARAM wParam, LPARAM lParam )
{
	mInputData.mMouseZ = GET_HIWORD(wParam)/WHEEL_DELTA;
	if(mFocusWidget)
	{
		if(!mFocusWidget->OnMouseWheel(&mInputData)&&mFocusWidget->GetFather())
		{
			mFocusWidget->GetFather()->OnMouseWheel(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleLButtonDown( WPARAM wParam, LPARAM lParam )
{
	mInputData.mOldMouseX = mInputData.mMouseX;
	mInputData.mOldMouseY = mInputData.mMouseY;
	mInputData.mButtonState.insert(Mouse_Left);
	CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnLButtonDown(&mInputData);
	}
}

PMVoid PMInputManager::HandleLButtonDBLCLK( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.erase(Mouse_Left);
	CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnLButtonDBLCLK(&mInputData);
	}
}

PMVoid PMInputManager::HandleRButtonDown( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.insert(Mouse_Right);
	CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnRButtonDown(&mInputData);
	}
}

PMVoid PMInputManager::HandleRButtonDBLCLK( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.erase(Mouse_Right);
	CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnRButtonDBLCLK(&mInputData);
	}
}

PMVoid PMInputManager::HandleMButtonDown( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.insert(Mouse_Middle);
	CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnMButtonDown(&mInputData);
	}
}

PMVoid PMInputManager::HandleLButtonUp( WPARAM wParam, LPARAM lParam )
{
	mInputData.mOldMouseX = 0;
	mInputData.mOldMouseY = 0;
	mInputData.mButtonState.erase(Mouse_Left);
	//CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnLButtonUp(&mInputData);
	}
}

PMVoid PMInputManager::HandleRButtonUp( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.erase(Mouse_Right);
	//CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnRButtonUp(&mInputData);
	}
}

PMVoid PMInputManager::HandleMButtonUp( WPARAM wParam, LPARAM lParam )
{
	mInputData.mButtonState.erase(Mouse_Middle);
	//CalcFocusInput();
	if(mFocusWidget)
	{
		mFocusWidget->OnMButtonUp(&mInputData);
	}
}

PMVoid PMInputManager::HandleKeyDown( WPARAM wParam, LPARAM lParam )
{
	mInputData.mKey[0] = wParam;	
	mInputData.mKey[1] = 0;
	mInputData.mKeyState.insert(wParam);	
	if(mFocusWidget)
	{
		if(!mFocusWidget->OnKeyDown(&mInputData)&&mFocusWidget->GetFather())
		{
			mFocusWidget->GetFather()->OnKeyDown(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleKeyUp( WPARAM wParam, LPARAM lParam )
{
	mInputData.mKeyState.erase(wParam);	
	if(mFocusWidget)
	{
		if(!mFocusWidget->OnKeyUp(&mInputData)&&mFocusWidget->GetFather())
		{
			mFocusWidget->GetFather()->OnKeyUp(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleChar( WPARAM wParam, LPARAM lParam )
{
	mInputData.mKey[0] = wParam;	
	mInputData.mKey[1] = 0;
	if(mFocusWidget)
	{
		if(!mFocusWidget->OnChar(&mInputData)&&mFocusWidget->GetFather())
		{
			mFocusWidget->GetFather()->OnChar(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleIMEChar( WPARAM wParam, LPARAM lParam )
{
	BYTE hiByte = wParam >> 8;
	BYTE loByte = wParam & 0x000000FF;
	if (hiByte == 0)
	{
		mInputData.mKey[0] = loByte;
		mInputData.mKey[1] = '\0';
	}
	else
	{
		mInputData.mKey[0] = hiByte;
		mInputData.mKey[1] = loByte;
		mInputData.mKey[2] = '\0';
	}
	if(mFocusWidget)
	{
		if(!mFocusWidget->OnChar(&mInputData)&&mFocusWidget->GetFather())
		{
			mFocusWidget->GetFather()->OnChar(&mInputData);
		}
	}
}

PMVoid PMInputManager::HandleSize( WPARAM wParam, LPARAM lParam )
{
	mInputData.mSizeHeight = GET_HIWORD(lParam);
	mInputData.mSizeWidth  = GET_LOWORD(lParam);
	for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
	{
		(*it)->OnSize(&mInputData);
	}
}

PMVoid PMInputManager::HandlePaint( WPARAM wParam, LPARAM lParam )
{
	for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
	{
		if((*it)->GetSize().intersect(mInputData.mPainRect))
		{
			(*it)->OnPaint(&mInputData);
		}
	}
}
PMVoid PMInputManager::HandleTimmer( WPARAM wParam, LPARAM lParam )
{
	if(GifTimmerID == wParam)
	{
		mInputData.mTimmerTick++;
		for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
		{
			(*it)->OnGifTimmer(&mInputData);
		}
		if (mFocusWidget)
		{
			if(!mFocusWidget->OnGifTimmer(&mInputData)&&mFocusWidget->GetFather())
			{
				mFocusWidget->GetFather()->OnGifTimmer(&mInputData);
			}
		}
	}else if(CommonTimmerID == wParam)
	{
		mInputData.mTimmerTick++;
		for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
		{
			(*it)->HandleSystemTimmer(&mInputData);
		}
	}else if(CaretTimmerID == wParam)//只有当前焦点的widget有消息
	{
		if (mFocusWidget)
		{
			if(!mFocusWidget->OnCaretTimmer(&mInputData)&&mFocusWidget->GetFather())
			{
				mFocusWidget->GetFather()->OnCaretTimmer(&mInputData);
			}
		}
	}
}

PMVoid PMInputManager::HandleDropFiles( WPARAM wParam, LPARAM lParam )
{
	CalcCurrentInput();
	if(mCurrentWidget)
	{
		mInputData.mDropFiles.clear();
		HDROP hDrop = (HDROP)wParam;
		char buffer[MAX_PATH] = { 0 };
		UINT fcount = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);
		for (UINT index = 0; index < fcount; ++index)
		{
			DragQueryFileA(hDrop, index, buffer, MAX_PATH);
			mInputData.mDropFiles.push_back(buffer);
		}		
		DragFinish(hDrop);
		mCurrentWidget->OnDropFiles(&mInputData);
	}
}

PMVoid PMInputManager::HandleSetFocus( WPARAM wParam, LPARAM lParam )
{
	mbFocus = true;
}

PMVoid PMInputManager::HandleKillFocus( WPARAM wParam, LPARAM lParam )
{
	mbFocus = false;
	mInputData.Clear();
}

PMVoid PMInputManager::CalcCurrentInput()
{
	IInput *OldCurrentWidget = mCurrentWidget;

	if(!(mCurrentWidget&&mCurrentWidget->GetSize().inside(mInputData.mMouseX,mInputData.mMouseY)))
	{
		PMBool bCurrent = false;
		for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
		{
			if((*it)->GetSize().inside(mInputData.mMouseX,mInputData.mMouseY))
			{
				mCurrentWidget = *it;
				bCurrent = true;
				break;
			}
		}
		if(!bCurrent)mCurrentWidget = NULL;
	}
	//获取孩子的当前控件
	if(mCurrentWidget&&mCurrentWidget->GetFather()==NULL)
	{
		mCurrentWidget = mCurrentWidget->GetCurrentInput(&mInputData);
	}
	if(OldCurrentWidget!=mCurrentWidget)
	{
		//1、当前控件离开
		if(OldCurrentWidget)
		{			
			OldCurrentWidget->OnLeave(&mInputData);
		}
		//2、当前控件进入
		if(mCurrentWidget)
		{			
			mCurrentWidget->OnSetFocus(&mInputData);
		}
	}
}

PMVoid PMInputManager::CalcFocusInput()
{
	IInput *OldFocusWidget = mFocusWidget;
	if(!(mFocusWidget&&mFocusWidget->GetSize().inside(mInputData.mMouseX,mInputData.mMouseY)))
	{
		PMBool bFocus = false;
		for (vector<IInput*>::iterator it = mInputWidgets.begin(); it!=mInputWidgets.end();it++)
		{
			if((*it)->GetSize().inside(mInputData.mMouseX,mInputData.mMouseY))
			{
				mFocusWidget = *it;
				bFocus = true;
				break;
			}
		}
		if(!bFocus)mFocusWidget = NULL;
	}
	//获取孩子的焦点控件
	if(mFocusWidget&&mFocusWidget->GetFather()==NULL)
	{
		mFocusWidget = mFocusWidget->GetFocusInput(&mInputData);
	}
	if(OldFocusWidget!=mFocusWidget)
	{
		//1、当前控件失去焦点
		if(OldFocusWidget)
		{			
			OldFocusWidget->OnLostFocus(&mInputData);
		}
		//2、当前控件得到焦点
		if(mFocusWidget)
		{			
			mFocusWidget->OnSetFocus(&mInputData);
		}
	}
}

PMVoid PMInputManager::CreateSystemTimmer( HWND hWnd )
{
	SetTimer(hWnd,CommonTimmerID,SYSTEM_TIMMER_TICC,NULL);
	SetTimer(hWnd,CaretTimmerID,CARET_TIMMER_TICC,NULL);
	SetTimer(hWnd,GifTimmerID,GIF_TIMMER_TICC,NULL);
	PMCliboardManager::GetSingleton()->Initialize(hWnd);
}

PMVoid PMInputManager::ResetCaretTimmer( HWND hWnd )
{
	KillTimer(hWnd,CaretTimmerID);
	SetTimer(hWnd,CaretTimmerID,CARET_TIMMER_TICC,NULL);
}

PMInputManager::PMInputManager():mCurrentWidget(NULL),mFocusWidget(NULL)
{
	CommonTimmerID = 9987;
	CaretTimmerID  = 9988;
	GifTimmerID	   = 9989;
}

PMInputManager::~PMInputManager()
{

}

PMVoid PMInputManager::HandleUserMessage( WPARAM wParam, LPARAM lParam )
{
	IInput *input = (IInput *)wParam;
	input->OnUserMessage(&mInputData);
}

PMVoid PMInputManager::Initialize( HINSTANCE hInstance,HWND hWnd )
{
	PMInputManager *imanager = GetSingleton();
	imanager->CreateSystemTimmer(hWnd);
	ScreenGDI::SetInstance(hInstance);
}


TInputData::TInputData()
{
	mHwnd		= NULL;	
	mMessage	= 0;
	mMouseX		= 0;
	mMouseY		= 0;
	mMouseZ		= 0;
	mSizeWidth	= 0;
	mSizeHeight	= 0;
	mTimmerTick = 0;
	memset(mKey,0,4);
	mUserInfo	= NULL;
}

TInputData::~TInputData()
{
	
}

PMBool TInputData::IsLButtonPress()
{
	return mButtonState.find(Mouse_Left)!=mButtonState.end();
}

PMBool TInputData::IsControlPress()
{
	return mKeyState.find(VLK_CONTROL)!=mKeyState.end();
}

PMBool TInputData::IsAltPress()
{
	return mKeyState.find(VLK_ALT)!=mKeyState.end();
}

PMBool TInputData::IsShiftPress()
{
	return mKeyState.find(VLK_SHIFT)!=mKeyState.end();
}

PMVoid TInputData::Clear()
{
	mKeyState.clear();
	mButtonState.clear();
}
