#include "RongyiAsyncThread.h"
#include "MyLog.h"

//////////////////////////////// RongyiAsyncThread //////////////////////////////////////////
RongyiAsyncThread::RongyiAsyncThread()
	:m_bQuit(false)
	,m_bCancelCurrentTask(false)
	,m_pCurrentTask(NULL)
{
	InitializeCriticalSection(&m_csRequest);
	InitializeCriticalSection(&m_csCurrentTask);

	// init thread
	m_workHandle = (HANDLE)_beginthreadex(
		NULL, // Security descriptor for new thread; must be NULL for Window
		0, // stack size
		(unsigned (__stdcall*)(void*))(&RongyiAsyncThread::StaticRun), // run function
		this, // arg list
		CREATE_SUSPENDED, // Initial state of new thread (0 for running or CREATE_SUSPENDED for waiting resume)
		&m_dwThreadID // Points to a 32-bit variable that receives the thread identifier
		);

	m_taskList.clear();
}

RongyiAsyncThread::~RongyiAsyncThread(void)
{
	StopThreadAsync();
	StopThreadSync();
	ClearRequest();

	DeleteCriticalSection(&m_csCurrentTask);
	DeleteCriticalSection(&m_csRequest);
}

unsigned RongyiAsyncThread::StaticRun(void* arg)
{
	if (!arg){
		return 1;
	}

	RongyiAsyncThread* pThread = (RongyiAsyncThread*) arg;
	return pThread->Run();
}

unsigned RongyiAsyncThread::Run()
{
	while(1)
	{
		if (m_bQuit){
			break;
		}

		ThreadRequest* pTask = NULL;
		while(!m_bQuit && NULL != (pTask = GetRequest()))
		{
			// do function
			if (pTask->pFuncRun){
				(*pTask->pFuncRun)(pTask->pWnd, pTask->pData, &m_bCancelCurrentTask);
			}
			
			// lock to free current task
			{
				MyCriticalLock cLock(m_csCurrentTask);
				delete pTask;
				pTask = NULL;

				m_pCurrentTask = NULL;
			}
		}

		if (m_bQuit){
			break;
		}

		SuspendThread(m_workHandle);
	}

	return 0;
}

void RongyiAsyncThread::StartThread()
{
	ResumeThread(m_workHandle);
}

void RongyiAsyncThread::Terminate()
{
	TerminateThread(m_workHandle, 100);
}

void RongyiAsyncThread::StopThreadAsync()
{
	m_bQuit = true;

	if (!m_bCancelCurrentTask)
	{
		MyCriticalLock cLock(m_csCurrentTask);
		m_bCancelCurrentTask = true;
		// stop current task
		if (m_pCurrentTask && m_pCurrentTask->pFuncStopCurrentTask){
			(*m_pCurrentTask->pFuncStopCurrentTask)(m_pCurrentTask->pWnd, m_pCurrentTask->pData);
		}
	}
}

// sys function, block!!!
void RongyiAsyncThread::StopThreadSync()
{
	//FunctionInOutTrace cLogTest(L"RongyiInterruptThread::StopThread");
	unsigned long dwExitCode = 0;
	GetExitCodeThread(m_workHandle, &dwExitCode);
	if (STILL_ACTIVE == dwExitCode)
	{
		m_bQuit = true;

		if (!m_bCancelCurrentTask)
		{
			MyCriticalLock cLock2(m_csCurrentTask);
			m_bCancelCurrentTask = true;
		}
		
		ResumeThread(m_workHandle);
		WaitForSingleObject(m_workHandle, INFINITE);
	}
}

void RongyiAsyncThread::PostRequest(void* pData 
	, void* pWnd 
	, RONGYIASYNCTREAD_FUNC_RUN pFuncRun 
	, RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask 
	, RONGYIASYNCTREAD_FUNC_FREE_TASK pFuncFreeTask 
	, const bool& bStartNow /* = true */
	, const bool& bAutoDeleteData /* = true */)
{
	ThreadRequest* pRequest = new ThreadRequest;
	if (pRequest)
	{
		pRequest->pData = pData;
		pRequest->pWnd = pWnd;
		pRequest->pFuncRun = pFuncRun;
		pRequest->pFuncStopCurrentTask = pFuncStopCurrentTask;
		pRequest->pFuncFreeTask = pFuncFreeTask;
		pRequest->bAutoDeleteData = bAutoDeleteData;
		
		MyCriticalLock cLock(m_csRequest);
		m_taskList.push_back(pRequest);
	}
	
	if (pRequest && bStartNow){
		ResumeThread(m_workHandle);
	}
}

ThreadRequest* RongyiAsyncThread::GetRequest()
{
	MyCriticalLock cLock1(m_csRequest);
	MyCriticalLock cLock2(m_csCurrentTask);

	ThreadRequest* pRet = NULL;
	THREAD_TASK_LIST::iterator itr = m_taskList.begin();
	if (itr != m_taskList.end()){
		pRet = *itr;
		m_taskList.erase(itr);

		// lock to set current task
		m_pCurrentTask = pRet;
		m_bCancelCurrentTask = false;
	}

	return pRet;
}

void RongyiAsyncThread::ClearRequest()
{
	MyCriticalLock cLock(m_csRequest);

	THREAD_TASK_LIST::iterator itr = m_taskList.begin();
	while (itr != m_taskList.end()){
		ThreadRequest* p = *itr;	
		if(p){
			delete p;
			p = NULL;
		}

		itr = m_taskList.erase(itr);
	}
}

unsigned long RongyiAsyncThread::GetRequestCount()
{
	MyCriticalLock cLock(m_csRequest);
	return m_taskList.size();
}

bool RongyiAsyncThread::IsTaskRunning()
{
	MyCriticalLock cLock(m_csCurrentTask);
	if (m_pCurrentTask){
		return true;
	}

	return false;
}

bool RongyiAsyncThread::IsTaskExist(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_EXIST_TASK pFuncTaskExit)
{

	if (!pFuncTaskExit){
		return false;
	}

	MyCriticalLock cLock1(m_csRequest);
	MyCriticalLock cLock2(m_csCurrentTask);
	if (m_pCurrentTask && (*pFuncTaskExit)(m_pCurrentTask->pWnd, m_pCurrentTask->pData, pDataChecked)){
		return true;
	}

	for (THREAD_TASK_LIST::iterator itr = m_taskList.begin(); itr != m_taskList.end(); ++itr)
	{
		ThreadRequest* pTask = *itr;
		if (pTask && (*pFuncTaskExit)(pTask->pWnd, pTask->pData, pDataChecked)){
			return true;
		}
	}

	return false;
}

// ffffffffff
void RongyiAsyncThread::StopIllegalTask(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_ILLEGAL_TASK pFuncIsIllegelTask)
{
	if (!pFuncIsIllegelTask){
		return;
	}

	{
		MyCriticalLock cLock(m_csRequest);

		THREAD_TASK_LIST::iterator itr = m_taskList.begin();
		while (itr != m_taskList.end())
		{
			ThreadRequest* pTask = *itr;
			if (pTask && (*pFuncIsIllegelTask)(pTask->pWnd, pTask->pData, pDataChecked)){				
				delete pTask;
				pTask = NULL;
				
				itr = m_taskList.erase(itr);
			}else{
				++itr;
			}
		}
	}

	// stop current task(set cancel flag to stop thread)
	{
		MyCriticalLock cLock(m_csCurrentTask);

		if (m_pCurrentTask && (*pFuncIsIllegelTask)(m_pCurrentTask->pWnd, m_pCurrentTask->pData, pDataChecked)){
			m_bCancelCurrentTask = true;
		}
	}
}


//////////////////////////////// RongyiAsyncThreadPoor //////////////////////////////////////////
RongyiAsyncThreadPool::RongyiAsyncThreadPool(unsigned long dwPoorSize, unsigned long dwThreadMultiTaskCount, bool bWaitWhenBusy /* = true */)
	:m_dwPoorSize(dwPoorSize)
	,m_dwThreadMultiTaskCount(dwThreadMultiTaskCount)
	,m_bWaitWhenBusy(bWaitWhenBusy)
{
	for (unsigned long dwIndex = 0; dwIndex < RONGYIINTERRUPTTREAD_POOR_MAX_SIZE; ++dwIndex)
	{
		if (dwIndex < m_dwPoorSize)
		{
			m_threadPool[dwIndex] = new RongyiAsyncThread();
		}
		else
		{
			m_threadPool[dwIndex] = NULL;
		}
	}

	InitializeCriticalSection(&m_csRequest);
}

RongyiAsyncThreadPool::~RongyiAsyncThreadPool()
{
	for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
	{
		if (m_threadPool[dwIndex]){
			m_threadPool[dwIndex]->StopThreadAsync();
		}
	}

	for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
	{
		if (m_threadPool[dwIndex]){
			m_threadPool[dwIndex]->StopThreadSync();
			delete m_threadPool[dwIndex];
			m_threadPool[dwIndex] = NULL;
		}
	}

	DeleteCriticalSection(&m_csRequest);
}

bool RongyiAsyncThreadPool::PostRequest(void* pData
	, void* pWnd 
	, RONGYIASYNCTREAD_FUNC_RUN pFuncRun
	, RONGYIASYNCTREAD_FUNC_STOP_CURRENT_TASK pFuncStopCurrentTask
	, RONGYIASYNCTREAD_FUNC_FREE_TASK pFuncFreeTask 
	, const bool& bStartNow /* = true */)
{
	MyCriticalLock cLock(m_csRequest);
	
	do 
	{
		for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
		{
			if (m_threadPool[dwIndex])
			{
				unsigned long dwTaskCount = m_threadPool[dwIndex]->GetRequestCount();
				if ((dwTaskCount + 1 < m_dwThreadMultiTaskCount) // list can add task
					|| ( (dwTaskCount + 1 == m_dwThreadMultiTaskCount) && (!m_threadPool[dwIndex]->IsTaskRunning())) // check if thread now running
					)
				{
					m_threadPool[dwIndex]->PostRequest(pData, pWnd, pFuncRun, pFuncStopCurrentTask, pFuncFreeTask, bStartNow);
					return true;
				}
			}
		}

		if (m_bWaitWhenBusy){
			Sleep(1);
		}

	} while(m_bWaitWhenBusy);

	return false;
}

void RongyiAsyncThreadPool::StopIllegalTask(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_ILLEGAL_TASK pFuncIsIllegelTask)
{
	MyCriticalLock cLock(m_csRequest);

	for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
	{
		if (m_threadPool[dwIndex]){
			m_threadPool[dwIndex]->StopIllegalTask(pDataChecked, pFuncIsIllegelTask);
		}
	}
}

bool RongyiAsyncThreadPool::IsTaskExist(void* pDataChecked, RONGYIASYNCTREAD_FUNC_IS_EXIST_TASK pFuncTaskExit)
{
	MyCriticalLock cLock(m_csRequest);

	for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
	{
		if (m_threadPool[dwIndex] && m_threadPool[dwIndex]->IsTaskExist(pDataChecked, pFuncTaskExit)){
			return true;
		}
	}

	return false;
}

void RongyiAsyncThreadPool::StartThreads()
{
	MyCriticalLock cLock(m_csRequest);

	for (unsigned long dwIndex = 0; dwIndex < m_dwPoorSize; ++dwIndex)
	{
		if (m_threadPool[dwIndex]){
			m_threadPool[dwIndex]->StartThread();
		}
	}
}