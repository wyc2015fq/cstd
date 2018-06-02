// DebugAssistant.cpp: implementation of the DebugAssistant class.
// 
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <windows.h>
#include "DebugAssistant.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _OUTPUT_INFO_
DebugAssistant _DebugInfo( DEBUG_CONSOLE/*DEBUG_FILE ,  _T( "chsjcard.log" )*/ );
#endif

DebugAssistant::DebugAssistant( DEBUG_TYPE mode ,  LPTSTR fileName ):
m_DebugMode( mode ),
m_OutputHandle( NULL ),
m_pFile( NULL )
{
	if( DEBUG_CONSOLE == m_DebugMode )
	{
		AllocConsole();
		m_OutputHandle = ::GetStdHandle( STD_OUTPUT_HANDLE );
	}
	if( NULL != fileName )
		m_pFile = _tfopen( fileName , _T("wb") );
}

DebugAssistant::~DebugAssistant()
{
	if( DEBUG_CONSOLE == m_DebugMode )
	{
		if( m_OutputHandle )
		{
			::CloseHandle( m_OutputHandle );
			m_OutputHandle = NULL;
		}
		FreeConsole();
	}
	if( m_pFile )
		fclose( m_pFile );
}

void DebugAssistant::OutputInfo( LPTSTR format , ... )
{
    TCHAR msg[1024] = {0};
	va_list arg_ptr;
	va_start( arg_ptr, format );
	_vstprintf( msg , format, arg_ptr );
	_tcscat( msg , _T("\n") );
	va_end(arg_ptr);
	long length = _tcslen( msg );
	AoutLock lock( &m_SynLog );
	switch( m_DebugMode )
	{
	case DEBUG_CONSOLE:
		{
			ULONG beWritten = 0;
			::WriteConsole( m_OutputHandle , msg , length , &beWritten , NULL );
		}
		break;
	case DEBUG_STDOUT:
		::OutputDebugString( msg );
		break;
	case DEBUG_FILE:
		if( m_pFile )
		{
			::fwrite( msg , sizeof(TCHAR) , length , m_pFile );
			::fwrite( _T("\n") , sizeof(TCHAR) , 1 ,  m_pFile );
		}
		break;
	default:
		break;
	}
}
