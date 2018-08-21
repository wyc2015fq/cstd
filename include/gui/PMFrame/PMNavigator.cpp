#include "StdAfx.h"
#include "PMNavigator.h"

PMNavigator::PMNavigator():m_NavigatorBackGround(NULL),mLaferNavigator(NULL)
{
	mVisible = false;
}

PMNavigator::~PMNavigator()
{
	Clear();
}

PMVoid PMNavigator::SetScope( IntSize &Scope )
{
	mScope = Scope;
}

PMVoid PMNavigator::SetNavigator( IntRect &Navigator )
{
	mNavigator = Navigator;
}

PMVoid PMNavigator::Draw()
{
	DrawPicture (m_NavigatorBackGround,mTitaRect);
	SetPen	  (mLineColor);
	DrawRetangle(GetDrawRect());	
}

IntRect PMNavigator::GetDrawRect()
{
	IntRect size;
	size.left     = mTitaRect.left + (PMInt)((PMDouble)mNavigator.left   / mScope.width  * mTitaRect.width());
	size.top      = mTitaRect.top  + (PMInt)((PMDouble)mNavigator.top    / mScope.height * mTitaRect.height());

	size.right	  = size.left +  (PMInt)((PMDouble)mNavigator.width()/mScope.width * mTitaRect.width());
	size.bottom   = size.top  +  (PMInt)((PMDouble)mNavigator.height()/mScope.height * mTitaRect.height());
	if(size.left>=mTitaRect.right)size.left      = mTitaRect.right-1;
	if(size.top>=mTitaRect.bottom)size.top       = mTitaRect.bottom-1;
	if(size.right>=mTitaRect.right)size.right    = mTitaRect.right-1;
	if(size.bottom>=mTitaRect.bottom)size.bottom = mTitaRect.bottom-1;
	return size;
}

PMBool PMNavigator::OnLButtonDown( TInputData *EventData )
{
	IntPoint  pt     = GetTiTaPosition(EventData);
	CalcNavigator(pt.x,pt.y);
	mFather->OnNavigate(mNavigator);
	return true;
}

PMBool PMNavigator::OnMouseMove( TInputData *EventData )
{
	if(EventData->IsLButtonPress())
	{
		IntPoint  pt     = GetTiTaPosition(EventData);
		CalcNavigator(pt.x,pt.y);
		mFather->OnNavigate(mNavigator);
	}
	return true;
}

PMVoid PMNavigator::Initialize( IInput *father,HWND hWnd,IntRect &ClientRect,TJSON *LaferNavigator )
{
	Clear();
	mFather   = father;
	mBoundary = ClientRect;
	mTitaRect.set(0,0,mBoundary.width(),mBoundary.height());
	ScreenGDI::Initialize(hWnd,mTitaRect.ToRECT());
	mLaferNavigator = LaferNavigator;
	IntSize size(mTitaRect.width(),mTitaRect.height());
	m_NavigatorBackGround = CreateLaferPicture(size,*LaferNavigator,NAVIGATOR_NAME);	
	mLineColor = LaferNavigator->At(NAVIGATOR_LINECOLOR).ToInt();
}

PMVoid PMNavigator::CalcNavigator( PMInt x,PMInt y )
{
	PMInt Width   = mNavigator.width();
	PMInt Height  = mNavigator.height();
	PMInt CenterX = (PMInt)(((PMDouble)x-mTitaRect.left) / mTitaRect.width()  * mScope.width);
	PMInt CenterY = (PMInt)(((PMDouble)y-mTitaRect.top)  / mTitaRect.height() * mScope.height);
	mNavigator.left = CenterX - Width/2;
	if(mNavigator.left<0)
	{
		mNavigator.left = 0;
	}
	mNavigator.right = mNavigator.left + Width;

	mNavigator.top = CenterY - Height/2;
	if(mNavigator.top<0)
	{
		mNavigator.top = 0;
	}
	mNavigator.bottom = mNavigator.top + Height;
}

PMVoid PMNavigator::Clear()
{
	if(m_NavigatorBackGround)
	{
		delete m_NavigatorBackGround;
		m_NavigatorBackGround = NULL;
	}
}

PMVoid PMNavigator::SetScopeWidth( PMInt ScopeWidth )
{
	mScope.width = ScopeWidth;
}

PMVoid PMNavigator::SetScopeHeiht( PMInt ScopeHeight )
{
	mScope.height = ScopeHeight;
}

PMVoid PMNavigator::SetNavigatorX( IntSize &NavigatorX )
{
	mNavigator.left  = NavigatorX.width;
	mNavigator.right = NavigatorX.height;
}

PMVoid PMNavigator::SetNavigatorY( IntSize &NavigatorY )
{
	mNavigator.top    = NavigatorY.width;
	mNavigator.bottom = NavigatorY.height;
}

PMVoid PMNavigator::Show()
{
	if(mVisible==false)return;
	Draw();
	DrawFrame(&mBoundary);
}

IntPoint PMNavigator::GetTiTaPosition( TInputData *EventData )
{
	IntPoint pt;
	pt.x = EventData->mMouseX - mBoundary.left;
	pt.y = EventData->mMouseY - mBoundary.top;
	return pt;
}

