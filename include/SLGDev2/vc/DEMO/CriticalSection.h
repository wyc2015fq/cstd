

#ifndef _CRITICAL_SECTION_
#define _CRITICAL_SECTION_

#include <windows.h>

class CriticalSection
{
	CRITICAL_SECTION m_sect;
#ifdef _DEBUG
	long m_Count;
#endif

public:
	CriticalSection()
	{
#ifdef _DEBUG
		m_Count = 0;
#endif
		::InitializeCriticalSection( &m_sect );
	}
	~CriticalSection()
	{
#ifdef _DEBUG
		if( m_Count != 0 )
		{
			::MessageBoxW( NULL , L"the counter is not zero" , L"warning!",MB_OK );
		}
#endif
		::DeleteCriticalSection( &m_sect );
	}
	
	
	void Lock( void )
	{
#ifdef _DEBUG
		InterlockedIncrement( &m_Count );
#endif
		::EnterCriticalSection( &m_sect );
	}
	
	void Unlock( void )
	{
#ifdef _DEBUG
		InterlockedDecrement( &m_Count );
#endif
		::LeaveCriticalSection( &m_sect );	

	}
};


class AoutLock
{
	CriticalSection* m_pCritalSection;
public:
	AoutLock( CriticalSection* pCritalSection )
	{
		if( pCritalSection )
		{
			pCritalSection->Lock();
			m_pCritalSection = pCritalSection;
		}	
	}
	~AoutLock()
	{
		if( m_pCritalSection )
			m_pCritalSection->Unlock();
	}
};

#endif