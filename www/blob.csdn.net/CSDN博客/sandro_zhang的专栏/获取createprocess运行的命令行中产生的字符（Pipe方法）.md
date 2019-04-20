# 获取createprocess运行的命令行中产生的字符（Pipe方法） - sandro_zhang的专栏 - CSDN博客
2011年12月01日 22:40:18[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：2736
感谢[http://www.vckbase.com/english/code/misc/redirect.shtml.htm](http://www.vckbase.com/english/code/misc/redirect.shtml.htm)所提供的源码，或从以下链接下载
[http://download.csdn.net/detail/sandro_zhang/3869045](http://download.csdn.net/detail/sandro_zhang/3869045)
主要函数整理如下。
```cpp
void Redirect()
{
    wchar_t command_line[] = L"output.exe";
	HANDLE					PipeReadHandle;
	HANDLE					PipeWriteHandle;
	PROCESS_INFORMATION		ProcessInfo;
	SECURITY_ATTRIBUTES		SecurityAttributes;
	STARTUPINFO				StartupInfo;
	BOOL					Success;
	//--------------------------------------------------------------------------
	//	Zero the structures.
	//--------------------------------------------------------------------------
	ZeroMemory( &StartupInfo,			sizeof( StartupInfo ));
	ZeroMemory( &ProcessInfo,			sizeof( ProcessInfo ));
	ZeroMemory( &SecurityAttributes,	sizeof( SecurityAttributes ));
	//--------------------------------------------------------------------------
	//	Create a pipe for the child's STDOUT.
	//--------------------------------------------------------------------------
	SecurityAttributes.nLength              = sizeof(SECURITY_ATTRIBUTES);
	SecurityAttributes.bInheritHandle       = TRUE;
	SecurityAttributes.lpSecurityDescriptor = NULL;
	Success = CreatePipe
	(
		&PipeReadHandle,		// address of variable for read handle
		&PipeWriteHandle,		// address of variable for write handle
		&SecurityAttributes,	// pointer to security attributes
		0						// number of bytes reserved for pipe (use default size)
	);
	if ( !Success )
	{
		//ShowLastError(_T("Error creating pipe"));
		return;
	}	
	//--------------------------------------------------------------------------
	//	Set up members of STARTUPINFO structure.
	//--------------------------------------------------------------------------
	StartupInfo.cb           = sizeof(STARTUPINFO);
	StartupInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	StartupInfo.wShowWindow  = SW_HIDE;
	StartupInfo.hStdOutput   = PipeWriteHandle;
	StartupInfo.hStdError    = PipeWriteHandle;
	//----------------------------------------------------------------------------
	//	Create the child process.
	//----------------------------------------------------------------------------
	Success = CreateProcess
	( 
		NULL,					// pointer to name of executable module
		command_line,	// command line 
		NULL,					// pointer to process security attributes 
		NULL,					// pointer to thread security attributes (use primary thread security attributes)
		TRUE,					// inherit handles
		0,						// creation flags
		NULL,					// pointer to new environment block (use parent's)
		NULL,	// pointer to current directory name
		&StartupInfo,			// pointer to STARTUPINFO
		&ProcessInfo			// pointer to PROCESS_INFORMATION
	);                 
	if ( !Success )
	{
		//ShowLastError(_T("Error creating process"));
		return;
	}
	DWORD	BytesLeftThisMessage = 0;
	DWORD	NumBytesRead;
    TCHAR	PipeData[BUF_SIZE] = {0}; 
	DWORD	TotalBytesAvailable = 0;
	for ( ; ; )
	{ 
		NumBytesRead = 0;
		Success = PeekNamedPipe
		( 
			PipeReadHandle,				// handle to pipe to copy from 
			PipeData,					// pointer to data buffer 
			1,							// size, in bytes, of data buffer 
			&NumBytesRead,				// pointer to number of bytes read 
			&TotalBytesAvailable,		// pointer to total number of bytes available
			&BytesLeftThisMessage		// pointer to unread bytes in this message 
		);
		if ( !Success )
		{
			break;
		}
		if ( NumBytesRead )
		{
			Success = ReadFile
			(
				PipeReadHandle,		// handle to pipe to copy from 
				PipeData,			// address of buffer that receives data
				BUF_SIZE - 1,		// number of bytes to read
				&NumBytesRead,		// address of number of bytes read
				NULL				// address of structure for data for overlapped I/O
			);
			if ( !Success )
			{
				break;
			}
			//------------------------------------------------------------------
			//	Zero-terminate the data.
			//------------------------------------------------------------------
			PipeData[NumBytesRead] = '\0';
			//------------------------------------------------------------------
			//	Replace backspaces with spaces.
			//------------------------------------------------------------------
			for ( DWORD ii = 0; ii < NumBytesRead; ii++ )
			{
				if ( PipeData[ii] == _T('\b') )
				{
					PipeData[ii] = ' ';
				}
			}
			
			//------------------------------------------------------------------
			//	If we're running a batch file that contains a pause command, 
			//	assume it is the last output from the batch file and remove it.
			//------------------------------------------------------------------
			TCHAR  *ptr = _tcsstr(PipeData, _T("Press any key to continue . . ."));
			if ( ptr )
			{
				*ptr = '\0';
			}
            /*
                Take the PipeData,if current process's unicode different with the child process,
                there should be handled appropriately.
                wstring data = string2wstring((LPCSTR)PipeData, BUF_SIZE);
            */
            cout << wstring2string( data );
		}
		else
		{
			//------------------------------------------------------------------
			//	If the child process has completed, break out.
			//------------------------------------------------------------------
			if ( WaitForSingleObject(ProcessInfo.hProcess, 0) == WAIT_OBJECT_0 )	//lint !e1924 (warning about C-style cast)
			{
				break;
			}
		}
		
	}
	//--------------------------------------------------------------------------
	//	Close handles.
	//--------------------------------------------------------------------------
	Success = CloseHandle(ProcessInfo.hThread);
	if ( !Success )
	{
		break;
	}
	Success = CloseHandle(ProcessInfo.hProcess);
	if ( !Success )
	{
		break;
	}
	Success = CloseHandle(PipeReadHandle);
	if ( !Success )
	{
		break;
	}
	Success = CloseHandle(PipeWriteHandle);
	if ( !Success )
	{
		break;
	}
}
```
