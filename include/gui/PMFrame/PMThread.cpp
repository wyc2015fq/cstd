#include "StdAfx.h"
#include "PMThread.h"

PMThread::PMThread(PMVoid):mThreadState(THREAD_STATE_STOPED)
{	
	Start();
}

PMThread::~PMThread(PMVoid)
{	
	Stop();
}

DWORD WINAPI PMThread::WorkerProc( LPVOID lParam )
{
	PMThread *pThread = (PMThread*) lParam;
	pThread->Work();
	return PMSuccess;
}

PMVoid PMThread::Start()
{
	InitializeCriticalSection(&mCsListAction);
	mWorkEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	HANDLE hThread = CreateThread(NULL,0,PMThread::WorkerProc,this,0,&mThreadId); 
	CloseHandle(hThread);	
	mThreadState = THREAD_STATE_RUN;
}

PMVoid PMThread::Stop()
{
	if(mThreadState!=THREAD_STATE_STOPED)
	{
		mStopEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
		Lock();
		Wakeup();
		mThreadState = THREAD_STATE_STOP;
		UnLock();
		WaitForSingleObject(mStopEvent,INFINITE);
		DeleteCriticalSection(&mCsListAction);
		CloseHandle(mWorkEvent);
		CloseHandle(mStopEvent);
	}	
}

PMVoid PMThread::Lock()
{
	EnterCriticalSection(&mCsListAction);
}

PMVoid PMThread::UnLock()
{
	LeaveCriticalSection(&mCsListAction);
}

PMVoid PMThread::Wait()
{
	WaitForSingleObject(mWorkEvent,INFINITE);
}

PMVoid PMThread::Wakeup()
{
	if(mThreadState == THREAD_STATE_WAIT)
	{
		SetEvent(mWorkEvent);
		mThreadState = THREAD_STATE_RUN;
	}	
}

PMVoid PMThread::Work()
{
	while (mThreadState > THREAD_STATE_STOP)
	{
		Lock();
		if(!mTasks.empty())
		{
			UnLock();
			CallBack();
		}else
		{
			mThreadState = THREAD_STATE_WAIT;
			UnLock();
			Wait();			
		}
	}
	mThreadState = THREAD_STATE_STOPED;
	SetEvent(mStopEvent);
}

PMUnlong PMThread::GetThreadID()
{
	return mThreadId;
}

PMVoid PMThread::AddTask( PMVoid *task )
{
	Lock();
	mTasks.push_back(task);
	Wakeup();	
	UnLock();
}
