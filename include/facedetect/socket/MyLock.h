#pragma once
#include "windows.h"
class MyCriticalLock
{
public:

	MyCriticalLock(CRITICAL_SECTION& _csLock)
		:csLock(_csLock)
	{
		::EnterCriticalSection(&csLock);
	}

	~MyCriticalLock(void)
	{
		::LeaveCriticalSection(&csLock);
	}

private:
	CRITICAL_SECTION& csLock;
};

