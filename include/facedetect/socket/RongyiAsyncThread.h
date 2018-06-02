#pragma once
#include "process.h"
#include "MyLock.h"
#include "list"
using namespace std;
const unsigned long RONGYIINTERRUPTTREAD_POOR_MAX_SIZE = 1000;

typedef void (*RONGYIASYNCTREAD_FUNC_RUN)(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
typedef void (*RONGYIASYNCTREAD_FUNC_FREE_TASK)(void* pWnd, void*& pTask);
typedef void (*RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK)(void* pWnd, void*& pData);
typedef bool (*RONGYIASYNCTREAD_FUNC_IS_ILLEGAL_TASK)(void* pWnd, void* pTask, void* arg);
typedef bool (*RONGYIASYNCTREAD_FUNC_IS_EXIST_TASK)(void* pWnd, void* pDataInList, void* pDataChecked);

struct ThreadRequest
{
	void* pData;
	void* pWnd;
	RONGYIASYNCTREAD_FUNC_RUN pFuncRun;
	RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask;
	RONGYIASYNCTREAD_FUNC_FREE_TASK pFuncFreeTask;
	bool bAutoDeleteData;
	ThreadRequest()
	{
		pData = NULL;
		pWnd = NULL;
		pFuncRun = NULL;
		pFuncStopCurrentTask = NULL;
		pFuncFreeTask = NULL;
		bAutoDeleteData = true;
	}

	~ThreadRequest()
	{
		if (bAutoDeleteData && pFuncFreeTask){
			(*pFuncFreeTask)(pWnd, pData);
		}

		if (bAutoDeleteData && pData){
			delete pData;
			pData = NULL;
		}
	}
};

typedef list<ThreadRequest*> THREAD_TASK_LIST;


class RongyiAsyncThread
{
public:
	RongyiAsyncThread(void);
	virtual ~RongyiAsyncThread(void);

	HANDLE& GetHandle(){
		return m_workHandle;
	}

	static unsigned StaticRun(void* arg);
	unsigned Run();
	void StartThread();

	void Terminate();

	// CAUTION!!!!!
	// Since this is a Sync function, used function StopThreadAsync to stop, and then used sync function to wait thread stop
	void StopCurrentTask(RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask);
	void StopThreadAsync();
	void StopThreadSync();

	void PostRequest(void* pData
		, void* pWnd
		, RONGYIASYNCTREAD_FUNC_RUN pFuncRun
		, RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask
		, RONGYIASYNCTREAD_FUNC_FREE_TASK pFuncFreeTask
		, const bool& bStartNow = true
		, const bool& bAutoDeleteData = true);
	unsigned long GetRequestCount();
	bool IsTaskRunning();

	bool IsTaskExist(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_EXIST_TASK pFuncTaskExit);
	void StopIllegalTask(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_ILLEGAL_TASK pFuncIsIllegelTask);

	unsigned int GetThreadID()
	{
		return m_dwThreadID;
	}

private:
	ThreadRequest* GetRequest();
	void ClearRequest();

	
	HANDLE m_workHandle;
	unsigned int m_dwThreadID;
	bool m_bQuit;
	bool m_bCancelCurrentTask;
	ThreadRequest* m_pCurrentTask;

	THREAD_TASK_LIST m_taskList;
	CRITICAL_SECTION m_csRequest;
	CRITICAL_SECTION m_csCurrentTask;
};

class RongyiAsyncThreadPool
{
public:
	RongyiAsyncThreadPool(unsigned long dwPoorSize, unsigned long dwThreadMultiTaskCount, bool bWaitWhenBusy = true);
	virtual ~RongyiAsyncThreadPool();

	bool PostRequest(void* pData
		, void* pWnd
		, RONGYIASYNCTREAD_FUNC_RUN pFuncRun
		, RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask
		, RONGYIASYNCTREAD_FUNC_FREE_TASK pFuncFreeTask
		, const bool& bStartNow = true);

	void StopIllegalTask(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_ILLEGAL_TASK pFuncIsIllegelTask);
	bool IsTaskExist(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_EXIST_TASK pFuncTaskExit);

	void StartThreads();

private:
	unsigned long m_dwPoorSize;
	unsigned long m_dwThreadMultiTaskCount;
	bool m_bWaitWhenBusy;
	RongyiAsyncThread* m_threadPool[RONGYIINTERRUPTTREAD_POOR_MAX_SIZE];

	CRITICAL_SECTION m_csRequest;
};



