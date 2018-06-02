#pragma once
#include <winsock2.h>
#pragma comment(lib, "WS2_32")
#include "RongyiAsyncThread.h"
#include "list"
#include "string"
using namespace std;


typedef std::string (*RONGYISOCKET_CALLBACK)(void* pWnd, const SOCKET& sid, const std::string& strRevData);
const unsigned long MAX_NUM_SOCKET = RONGYIINTERRUPTTREAD_POOR_MAX_SIZE;

struct SocketClientTask
{
	SOCKET socketClient;
	WSAEVENT evWaitEvent;
	string strBackupData;

	SocketClientTask()
	{
		socketClient = INVALID_SOCKET;
		evWaitEvent = WSA_INVALID_EVENT;
		strBackupData = "";
	}
};

typedef list<SOCKET*> CLIENTLIST;

class RongyiSocketServer
{
public:
	static bool RongyiWSAStartup();
	static void RongyiWSACleanup();
	RongyiSocketServer(unsigned long dwPort, unsigned long dwClientMaxCount, void* pWmd, RONGYISOCKET_CALLBACK pFuncCallBack);
	virtual ~RongyiSocketServer(void);

	static void StaticAcceptConnentThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
	void AcceptConnentThreadRun(void* arg, const bool* pbCancelCurrentTask);
	static void StaticReceiveDataThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
	void ReceiveDataThreadRun(void* arg, const bool* pbCancelCurrentTask);
	static void StaticStopCurrentTask(void* pWnd, void*& pTask);
	void StopCurrentTask(void*& pTask);
	static void StaticFreeTask(void* pWnd, void*& pTask);
	void FreeTask(void*& pTask);
	bool SendDataToClient(const SOCKET& socketClient, const string& strData);

private:

	string ByteToString(const byte* pbyData, const DWORD& dwLen);

	void ReceiveClientData(SocketClientTask* &pTask);

	unsigned long m_dwServerPort;
	SOCKET m_listenSocket;

	void* m_pWmd;
	RONGYISOCKET_CALLBACK m_pFuncCallBack;
	RongyiAsyncThread* m_AcceptConnentThread;
	RongyiAsyncThreadPool* m_ReceiveDataThreadPool;
};

enum RongyiClientStatus
{
	RongyiClientStatus_init = 0,
	RongyiClientStatus_Connected,
	RongyiClientStatus_Disconnected,
	RongyiClientStatus_ConnectFailure,
};

class RongyiSocketClient
{
public:
	RongyiSocketClient(string strServerAddr, unsigned dwServerPort, void* pWmd, RONGYISOCKET_CALLBACK pFuncCallBack);
	virtual ~RongyiSocketClient();

	// thread function
	static void StaticOnReceiveThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
	void OnReceiveThread(void* arg, const bool* pbCancelCurrentTask);
	static void StaticOnProcessDataThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
	void OnProcessDataThread(void* arg, const bool* pbCancelCurrentTask);
	static void StaticFreeReceiveDataTask(void* pWnd, void*& pTask);
	void FreeReceiveDataTask(void*& pTask);

	bool SendData(const char* pData, unsigned dwLen);

private:
	int m_dwStatus;
	string m_strServerAddr;
	unsigned m_dwServerPort;
	SOCKET m_sclient;
	char* m_pData;
	char* m_pReadBuffer;
	void* m_pWmd;
	RONGYISOCKET_CALLBACK m_pFuncCallBack;


	RongyiAsyncThread m_RecieveThread;
	RongyiAsyncThread m_ProcessDataThread;

};

