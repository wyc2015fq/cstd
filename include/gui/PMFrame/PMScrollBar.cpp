#include "StdAfx.h"
#include "PMScrollBar.h"

PMScrollBar::PMScrollBar():mSmallAchorLen(10)
{
	mType		= SCROLL_VERTICAL;	//滚动条的类型	
	mMax		= 100;				//最大的
	mActualMax  = 0;				//实际的最大值
	mStep		= 5;				//按钮的
	mLargeStep	= 20;				//鼠标的
	mPageValue  = 20;
	mValue		= 10;				//当前位置
	mState      = BUTTON_DEFAULT;	
	mVisible    = false;

	mLaferScroll				= NULL;		//存储绘制的滚动条数据	
	m_ScrollBackGround			= NULL;		//滚动条背景图片正常
	m_ScrollButtonHome			= NULL;		//滚动条上按钮图片正常
	m_ScrollButtonHomePress		= NULL;		//滚动条上按钮图片按下
	m_ScrollButtonHomeEnter		= NULL;		//滚动条上按钮图片鼠标进入
	m_ScrollButtonEnd			= NULL;		//滚动条下按钮图片正常
	m_ScrollButtonEndPress		= NULL;		//滚动条下按钮图片按下
	m_ScrollButtonEndEnter		= NULL;		//滚动条下按钮图片鼠标进入
	m_ScrollAnchor				= NULL;		//滚动条滚动子图片正常
	m_ScrollAnchorPress			= NULL;		//滚动条滚动子图片按下
	m_ScrollAnchorEnter			= NULL;		//滚动条滚动子图片鼠标进入
}

PMScrollBar::~PMScrollBar(void)
{
	Clear();
}	

PMVoid PMScrollBar::Initialize( IInput *father,HWND hWnd,IntRect &ContentRect,TJSON *LaferScroll )
{
	mFather		   = father;
	mLaferScroll   = LaferScroll;		//存储绘制的滚动条数据
	mBoundary	   = ContentRect;
	mTiTaRect.set(0,0,mBoundary.width(),mBoundary.height());
	ScreenGDI::Initialize(hWnd,mTiTaRect.ToRECT());
	mLaferRect     = mTiTaRect;
	mRectButtonHome= GetRect(*mLaferScroll,SCROLL_RECTBUTTONUP);
	mRectButtonEnd = GetRect(*mLaferScroll,SCROLL_RECTBUTTONDOWN);
	mType = mLaferScroll->GetName()==SCROLL_HSCROLLBAR?SCROLL_HORIZON:SCROLL_VERTICAL;
}

PMVoid PMScrollBar::ReCalcParameter(PMBool bSetMax)
{
	if(bSetMax)
	{
		TJSON &lafer = *mLaferScroll;
		IntSize size;
		CalcAnchorRect();
		if(m_ScrollAnchor)		delete m_ScrollAnchor;
		if(m_ScrollAnchorPress)	delete m_ScrollAnchorPress;
		if(m_ScrollAnchorEnter)	delete m_ScrollAnchorEnter;
		size.set(mRectAnchor.width(),mRectAnchor.height());
		m_ScrollAnchor				= CreateLaferPicture(size,lafer,SCROLL_ANCHOR);	
		m_ScrollAnchorPress			= CreateLaferPicture(size,lafer,SCROLL_ANCHORPRESS);	
		m_ScrollAnchorEnter			= CreateLaferPicture(size,lafer,SCROLL_ANCHORENTER);
	}else
	{
		PMInt scope = mEndScope - mHomeScope ;
		if(mType == SCROLL_HORIZON)
		{
			PMInt length = mRectAnchor.width();
			scope -= length;
			mRectAnchor.left   = mTiTaRect.left + mRectButtonHome.width() + (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue)) ;
			mRectAnchor.right  = mRectAnchor.left + length;
		}else
		{
			PMInt length = mRectAnchor.height();	
			scope -= length;
			mRectAnchor.top   = mTiTaRect.top + mRectButtonHome.height() + (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue)) ;
			mRectAnchor.bottom  = mRectAnchor.top + length;
		}
	}
}

PMVoid PMScrollBar::LoadPicture()
{
	TJSON &lafer = *mLaferScroll;
	IntSize size;
	CalcAnchorRect();
	size.set(mTiTaRect.width(),mTiTaRect.height());
	m_ScrollBackGround			= CreateLaferPicture(size,lafer,SCROLL_BACKGROUND);	

	size.set(mRectButtonHome.width(),mRectButtonHome.height());
	m_ScrollButtonHome			= CreateLaferPicture(size,lafer,SCROLL_BUTTONHOME);	
	m_ScrollButtonHomePress		= CreateLaferPicture(size,lafer,SCROLL_BUTTONHOMEPRESS);	
	m_ScrollButtonHomeEnter		= CreateLaferPicture(size,lafer,SCROLL_BUTTONHOMEENTER);	

	size.set(mRectButtonEnd.width(),mRectButtonEnd.height());
	m_ScrollButtonEnd			= CreateLaferPicture(size,lafer,SCROLL_BUTTONEND);
	m_ScrollButtonEndPress		= CreateLaferPicture(size,lafer,SCROLL_BUTTONENDPRESS);		
	m_ScrollButtonEndEnter		= CreateLaferPicture(size,lafer,SCROLL_BUTTONENDENTER);	

	size.set(mRectAnchor.width(),mRectAnchor.height());
	m_ScrollAnchor				= CreateLaferPicture(size,lafer,SCROLL_ANCHOR);	
	m_ScrollAnchorPress			= CreateLaferPicture(size,lafer,SCROLL_ANCHORPRESS);	
	m_ScrollAnchorEnter			= CreateLaferPicture(size,lafer,SCROLL_ANCHORENTER);			
}

PMVoid PMScrollBar::Draw()
{
	//1、绘制背景
	DrawPicture(m_ScrollBackGround,mTiTaRect);
	//2、绘制开始按钮
	if(mState == BUTTON_HOME_ENTER)			DrawPicture(m_ScrollButtonHomeEnter,mRectButtonHome);
	else if(mState == BUTTON_HOME_PRESS)	DrawPicture(m_ScrollButtonHomePress,mRectButtonHome);
	else									DrawPicture(m_ScrollButtonHome,mRectButtonHome);
	//3、绘制结束按钮
	if(mState == BUTTON_END_ENTER)			DrawPicture(m_ScrollButtonEndEnter,mRectButtonEnd);
	else if(mState == BUTTON_END_PRESS)		DrawPicture(m_ScrollButtonEndPress,mRectButtonEnd);
	else									DrawPicture(m_ScrollButtonEnd,mRectButtonEnd);
	//4、绘制滚动子
	if(mState == BUTTON_ANCHOR_ENTER)		DrawPicture(m_ScrollAnchorEnter,mRectAnchor);
	else if(mState == BUTTON_ANCHOR_PRESS)	DrawPicture(m_ScrollAnchorPress,mRectAnchor);
	else									DrawPicture(m_ScrollAnchor,mRectAnchor);
}

PMBool PMScrollBar::OnPaint( TInputData *EventData )
{
	//Show();
	return true;
}

PMBool PMScrollBar::OnLButtonDBLCLK( TInputData *EventData )
{
	return OnLButtonDown(EventData);
}

PMVoid PMScrollBar::Clear()
{
	if(m_ScrollBackGround)
	{
		delete m_ScrollBackGround;
		m_ScrollBackGround = NULL;
	}
	if(m_ScrollButtonHome)
	{
		delete m_ScrollButtonHome;
		m_ScrollButtonHome = NULL;
	}
	if(m_ScrollButtonHomePress)
	{
		delete m_ScrollButtonHomePress;
		m_ScrollButtonHomePress = NULL;
	}
	if(m_ScrollButtonHomeEnter)
	{
		delete m_ScrollButtonHomeEnter;
		m_ScrollButtonHomeEnter = NULL;
	}
	if(m_ScrollButtonEnd)
	{
		delete m_ScrollButtonEnd;
		m_ScrollButtonEnd = NULL;
	}
	if(m_ScrollButtonEndPress)
	{
		delete m_ScrollButtonEndPress;
		m_ScrollButtonEndPress = NULL;
	}
	if(m_ScrollButtonEndEnter)
	{
		delete m_ScrollButtonEndEnter;
		m_ScrollButtonEndEnter = NULL;
	}
	if(m_ScrollAnchor)
	{
		delete m_ScrollAnchor;
		m_ScrollAnchor = NULL;
	}
	if(m_ScrollAnchorPress)
	{
		delete m_ScrollAnchorPress;
		m_ScrollAnchorPress = NULL;
	}
	if(m_ScrollAnchorEnter)
	{
		delete m_ScrollAnchorEnter;
		m_ScrollAnchorEnter = NULL;
	}
}

PMInt PMScrollBar::GetScope()
{
	if(mType == SCROLL_HORIZON)return mTiTaRect.height();
	else return mTiTaRect.width();
}

PMVoid PMScrollBar::CalcAnchorRect()
{
	PMInt width  = mTiTaRect.width();
	PMInt height = mTiTaRect.height();
	PMInt length;
	PMInt scope;
	PMInt position;
	if(mType == SCROLL_HORIZON)
	{
		mHomeScope = mTiTaRect.left + mRectButtonHome.width();
		mEndScope  = mTiTaRect.right - mRectButtonEnd.width();
		width-=mRectButtonHome.width();
		width-=mRectButtonEnd.width();
		if(mPageValue>=mMax)
		{
			length = width;			
		}else
		{
			length = (PMInt)((PMDouble)mPageValue/(mMax)*width);
			if (length<mSmallAchorLen)length = mSmallAchorLen;
		}
		scope    = width - length;
		position = (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue));
		mRectAnchor.left   = mTiTaRect.left + mRectButtonHome.width() + position ;
		mRectAnchor.top	   = mTiTaRect.top;
		mRectAnchor.right  = mRectAnchor.left + length;
		mRectAnchor.bottom = mTiTaRect.bottom;		
	}
	else 
	{
		mHomeScope = mTiTaRect.top + mRectButtonHome.height();
		mEndScope  = mTiTaRect.bottom - mRectButtonEnd.height();
		height-=mRectButtonHome.height();
		height-=mRectButtonEnd.height();
		if(mPageValue>=mMax)
		{
			length = height;			
		}else
		{
			length = (PMInt)((PMDouble)mPageValue/(mMax)*height);
			if (length<mSmallAchorLen)length = mSmallAchorLen;
		}
		scope    = height - length;
		position = (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue));
		mRectAnchor.left   = mTiTaRect.left ;
		mRectAnchor.top	   = mTiTaRect.top + mRectButtonHome.height() + position;
		mRectAnchor.right  = mTiTaRect.right ;
		mRectAnchor.bottom = mRectAnchor.top + length;		
	}
}

PMVoid PMScrollBar::ChangeState( PMInt iState )
{
	if(mState!=iState)
	{
		mState = (TBTNState)iState ;
		Show();
	}
}

PMVoid PMScrollBar::MoveAnchor( IntPoint &pt )
{
	if(mType == SCROLL_HORIZON)
	{
		PMInt length   = mRectAnchor.width();
		PMInt scope    = mEndScope - mHomeScope - length;
		PMInt iChange  = (PMInt)((PMDouble)(pt.x - mOldX)/scope*(mMax-mPageValue));
		ChangeValue(iChange);		
	}else
	{
		PMInt length   = mRectAnchor.height();
		PMInt scope    = mEndScope - mHomeScope - length;
		PMInt iChange = (PMInt)((PMDouble)(pt.y - mOldY)/scope*(mMax-mPageValue));
		ChangeValue(iChange);
	}
}

PMBool PMScrollBar::OnEnter( TInputData *EventData )
{
	if(!EventData->IsLButtonPress())
	{
		IntPoint pt = GetTiTaPosition(EventData);
		if(mRectButtonHome.inside(pt))	   ChangeState(BUTTON_HOME_ENTER);
		else if (mRectButtonEnd.inside(pt))ChangeState(BUTTON_END_ENTER);
		else if (mRectAnchor.inside(pt))   ChangeState(BUTTON_ANCHOR_ENTER);
	}
	return true;
}

PMBool PMScrollBar::OnLeave( TInputData *EventData )
{
	if(!EventData->IsLButtonPress())
	{
		ChangeState(BUTTON_DEFAULT);
	}
	return true;
}

PMBool PMScrollBar::OnMouseMove( TInputData *EventData )
{
	IntPoint pt = GetTiTaPosition(EventData);
	if(EventData->IsLButtonPress())
	{
		if(mState==BUTTON_ANCHOR_PRESS)
		{			
			MoveAnchor(pt);
			mOldX = pt.x;
			mOldY = pt.y;		
		}else 
		{
			ReCalcState(pt.x,pt.y);
		}		
	}else
	{
		if(mRectButtonHome.inside(pt))	   ChangeState(BUTTON_HOME_ENTER);
		else if (mRectButtonEnd.inside(pt))ChangeState(BUTTON_END_ENTER);
		else if (mRectAnchor.inside(pt))   ChangeState(BUTTON_ANCHOR_ENTER);
		else ChangeState(BUTTON_DEFAULT);
	}
	return true;
}

PMBool PMScrollBar::OnMouseWheel( TInputData *EventData )
{
	if(mType == SCROLL_VERTICAL)
	{
		mFather->OnMouseWheel(EventData);
	}else
	{
		ChangeValue((-EventData->mMouseZ)*mLargeStep);	
	}
	return true;
}

PMBool PMScrollBar::OnLButtonDown( TInputData *EventData )
{		
	IntPoint pt = GetTiTaPosition(EventData);
	if(mRectButtonHome.inside(pt))
	{
		mState = BUTTON_HOME_PRESS;
		ChangeValue(-mStep,true);
	}
	else if (mRectButtonEnd.inside(pt))
	{
		mState = BUTTON_END_PRESS;
		ChangeValue(mStep,true);
	}
	else if (mRectAnchor.inside(pt))
	{
		mOldX = pt.x;
		mOldY = pt.y;
		ChangeState(BUTTON_ANCHOR_PRESS);
	}
	else 
	{		
		if((mType == SCROLL_HORIZON&&pt.x<mRectAnchor.left)||(mType == SCROLL_VERTICAL&&pt.y<mRectAnchor.top))
		{
			mState = BUTTON_ANCHOR_HOME_SIDE_PRESS;
			ChangeValue(-mPageValue,true);
		}else
		{
			mState = BUTTON_ANCHOR_END_SIDE_PRESS;
			ChangeValue(mPageValue,true);
		}		
	}
	mTicc =GetTickCount();
	return true;
}

PMBool PMScrollBar::OnLButtonUp( TInputData *EventData )
{
	IntPoint pt = GetTiTaPosition(EventData);
	if(mRectButtonHome.inside(pt))		ChangeState(BUTTON_HOME_ENTER);
	else if (mRectButtonEnd.inside(pt))	ChangeState(BUTTON_END_ENTER);
	else if (mRectAnchor.inside(pt))	ChangeState(BUTTON_ANCHOR_ENTER);
	else ChangeState(BUTTON_DEFAULT);
	HandleScrollFinish();
	return true;
}

PMBool PMScrollBar::OnSystemTimmer( TInputData *EventData )
{
	if(EventData->IsLButtonPress()&&GetTickCount()-mTicc>500)//只需要每秒五次的频率
	{
		switch(mState)
		{
		case BUTTON_HOME_PRESS:
			ChangeValue(-mStep,true);	
			break;
		case BUTTON_END_PRESS:
			ChangeValue(mStep,true);	
			break;
		case BUTTON_ANCHOR_HOME_SIDE_PRESS:
			ChangeValue(-mPageValue,true);
			break;
		case BUTTON_ANCHOR_END_SIDE_PRESS:
			ChangeValue(mPageValue,true);
			break;
		default:
			return true;
		}
		IntPoint pt = GetTiTaPosition(EventData);
		ReCalcState(pt.x,pt.y);
	}
	return true;
}

PMVoid PMScrollBar::ReCalcState( PMInt x,PMInt y )
{
	if(mTiTaRect.inside(x,y))
	{
		if(mRectButtonHome.inside(x,y))		
		{
			if(mState!=BUTTON_HOME_PRESS)mState = BUTTON_HOME_ENTER;
		}
		else if (mRectButtonEnd.inside(x,y))
		{
			if(mState!=BUTTON_END_PRESS)mState = BUTTON_END_ENTER;
		}
		else if (mRectAnchor.inside(x,y))	ChangeState(BUTTON_ANCHOR_ENTER);
		else 
		{
			if((mType == SCROLL_HORIZON&&x<mRectAnchor.left)||(mType == SCROLL_VERTICAL&&y<mRectAnchor.top))
			{
				mState = BUTTON_ANCHOR_HOME_SIDE_PRESS;
			}else
			{
				mState = BUTTON_ANCHOR_END_SIDE_PRESS;
			}
		}
	}else
	{
		mState = BUTTON_DEFAULT;
	}
}

PMVoid PMScrollBar::SetMax( PMInt max )
{
	mMax = max;
}

PMVoid PMScrollBar::SetValue( PMInt value )
{
	mValue = value;
}

PMVoid PMScrollBar::Show()
{	
	if(mVisible==false)return;
	Draw();
	DrawFrame(&mBoundary);
}

PMVoid PMScrollBar::Hide()
{

}

PMVoid PMScrollBar::HandleScrollFinish()
{
	if(mType == SCROLL_VERTICAL)mFather->OnVScrollFinish();
	else mFather->OnHScrollFinish();
}

PMBool PMScrollBar::IsVertical()
{
	return mType == SCROLL_VERTICAL;
}

PMBool PMScrollBar::IsHorition()
{
	return mType == SCROLL_HORIZON;
}

PMVoid PMScrollBar::ChangeValue( PMInt iChange,PMBool bRecalc )
{
	PMInt OldValue = mValue;
	mValue += iChange;
	if(mValue<0)mValue = 0;
	else if (mValue>mMax-mPageValue)mValue = mMax-mPageValue>0?mMax-mPageValue:0;
	iChange = mValue-OldValue;
	if(iChange!=0)
	{
		PMInt maxLen = mValue+mPageValue;
		if(bRecalc&&mActualMax<mMax)
		{
			SetMax(max(mActualMax,maxLen));
			ReCalcParameter(true);
		}else if (mType == SCROLL_VERTICAL)
		{
			PMInt length   = mRectAnchor.height();
			PMInt scope    = mEndScope - mHomeScope - length;
			PMInt position = (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue));
			mRectAnchor.left   = mTiTaRect.left ;
			mRectAnchor.top	   = mHomeScope + position;
			mRectAnchor.right  = mTiTaRect.right ;
			mRectAnchor.bottom = mRectAnchor.top + length;
		}else
		{
			PMInt length   = mRectAnchor.width();
			PMInt scope    = mEndScope - mHomeScope - length;
			PMInt position = (PMInt)((PMDouble)scope*mValue/(mMax-mPageValue));

			mRectAnchor.left   = mHomeScope + position ;
			mRectAnchor.top	   = mTiTaRect.top;
			mRectAnchor.right  = mRectAnchor.left + length;
			mRectAnchor.bottom = mTiTaRect.bottom;
		}
		Draw();
		if(mType == SCROLL_VERTICAL)mFather->OnVScroll(iChange);
		else mFather->OnHScroll(iChange);
	}
}

PMVoid PMScrollBar::HandleChange( PMInt iChange )
{
	if (mType == SCROLL_VERTICAL)
	{
		mFather->OnVScroll(iChange);
	}else
	{
		mFather->OnHScroll(iChange);
	}
}

IntPoint PMScrollBar::GetTiTaPosition( TInputData *EventData )
{
	IntPoint pt;
	pt.x = EventData->mMouseX - mBoundary.left;
	pt.y = EventData->mMouseY - mBoundary.top;
	return pt;
}

PMVoid PMScrollManager::Initialize( IInput *father,HWND hWnd,IntRect &ContentRect,TJSON *jnScrollLafer,PMInt ScrollType )
{
	TJSON  *jnVScrollBar = jnScrollLafer->Find(SCROLL_VSCROLLBAR);
	TJSON  *jnHScrollBar = jnScrollLafer->Find(SCROLL_HSCROLLBAR);
	TJSON  *jnNavigator  = jnScrollLafer->Find(NAVIGATOR_NAME);
	PMInt   Width		 = jnVScrollBar->Find(WIDTH)->ToInt();
	PMInt   Height		 = jnHScrollBar->Find(WIDTH)->ToInt();
	IntRect nRect;
	mHScrollBar.SetVisible(false);
	mVScrollBar.SetVisible(false);
	mNavigator.SetVisible(false);
	if(ScrollType&SCROLL_HORIZON)
	{
		nRect.top    = ContentRect.bottom - Height;
		nRect.left   = ContentRect.left;
		nRect.right  = ContentRect.right - (ScrollType&SCROLL_VERTICAL?Width:0);
		nRect.bottom = ContentRect.bottom;
		mHScrollBar.Initialize(father,hWnd,nRect,jnHScrollBar);
		mHScrollBar.SetVisible(true);
	}
	if(ScrollType&SCROLL_VERTICAL)
	{
		nRect.top    = ContentRect.top;
		nRect.left   = ContentRect.right - Width;
		nRect.right  = ContentRect.right;
		nRect.bottom = ContentRect.bottom - (ScrollType&SCROLL_HORIZON?Height:0);
		mVScrollBar.Initialize(father,hWnd,nRect,jnVScrollBar);
		mVScrollBar.SetVisible(true);
	}
	if(ScrollType==SCROLL_BOTH)
	{
		nRect.top    = ContentRect.bottom - Height;
		nRect.left   = ContentRect.right  - Width;
		nRect.right  = ContentRect.right;
		nRect.bottom = ContentRect.bottom;
		mNavigator.Initialize(father,hWnd,nRect,jnNavigator);
		mNavigator.SetVisible(true);
	}
	if(ScrollType&SCROLL_HORIZON)ContentRect.bottom  -= mHScrollBar.GetScope();
	if(ScrollType&SCROLL_VERTICAL)ContentRect.right  -= mVScrollBar.GetScope();
}

PMVoid PMScrollManager::Show()
{
	mVScrollBar.Show();
	mHScrollBar.Show();
	mNavigator.Show();	
}
