//////////////////////////////////////////////////////////////////
//																//
//		用途 : 时间/计时相关									//
//		创建 : [Foolish] / 2002-11-30							//
//		更新 :													//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef		__FOO_TIME_H__
#define		__FOO_TIME_H__
#pragma once
#include <windows.h>

//===================================================================
//	Declaration
//===================================================================
class FCTimeCount
{
public :
	FCTimeCount () ;

	void	Start () ;
	DWORD	GetInterval () ;
	void	MessageBoxInterval (bool bDot = true, HWND hParent = NULL) ;

protected :
	DWORD		m_dwStart ;
} ;

//===================================================================
//	Implement
//===================================================================
inline FCTimeCount::FCTimeCount () {
	this->Start () ;
}
inline void FCTimeCount::Start () {
	m_dwStart = ::GetTickCount () ;
}
inline DWORD FCTimeCount::GetInterval () {
	return ::GetTickCount () - m_dwStart ;
}
inline void FCTimeCount::MessageBoxInterval (bool bDot, HWND hParent) {
	TCHAR	szBuf[64] ;
	if (bDot)
	{
		TCHAR		num[64] ;
		::wsprintf (num, TEXT("%lu"), this->GetInterval()) ;
		NUMBERFMT	nf ;
		nf.NumDigits = 0 ;
		nf.LeadingZero = FALSE ;
		nf.Grouping = 3 ;
		nf.lpDecimalSep = TEXT(".") ;
		nf.lpThousandSep = TEXT(",") ;
		nf.NegativeOrder = 0 ;
		::GetNumberFormat (LOCALE_USER_DEFAULT, 0, num, &nf, szBuf, 64) ;
	}
	else
		::wsprintf (szBuf, TEXT("%lu"), this->GetInterval()) ;
	::lstrcat (szBuf, TEXT(" milliseconds")) ;
	::MessageBox (hParent, szBuf, TEXT("Interval"), MB_OK) ;
}

#endif