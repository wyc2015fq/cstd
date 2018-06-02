#include "RongyiSocket.h"
#include "MyLog.h"
#include "CommonFunction.h"

///////////////////////////////// server /////////////////////////////////////////
const unsigned long READ_SOCKET_DATA_SIZE	= 1024;
const unsigned long SOCKET_DATA_COMMON_SIZE	= 1024 * 2;

bool RongyiSocketServer::RongyiWSAStartup()
{
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData) == 0){
		return true;
	}else{
		LOGOUT_ERR("[%d]", WSAGetLastError());
		return false;
	}
}

void RongyiSocketServer::RongyiWSACleanup()
{
	WSACleanup();
}

RongyiSocketServer::RongyiSocketServer(unsigned long dwPort, unsigned long dwClientMaxCount, void* pWmd, RONGYISOCKET_CALLBACK pFuncCallBack)
{
	m_listenSocket = INVALID_SOCKET;
	m_dwServerPort = dwPort;
	m_pWmd = pWmd;
	m_pFuncCallBack = pFuncCallBack;

	//初始化WSA
	
	m_AcceptConnentThread = new RongyiAsyncThread();
	m_ReceiveDataThreadPool = new RongyiAsyncThreadPool(dwClientMaxCount, 1);

	if (m_AcceptConnentThread){
			m_AcceptConnentThread->PostRequest(NULL, this, &RongyiSocketServer::StaticAcceptConnentThreadRun, NULL, NULL);
	}
}

RongyiSocketServer::~RongyiSocketServer(void)
{
	if (m_listenSocket != INVALID_SOCKET){
		closesocket(m_listenSocket);
		m_listenSocket = INVALID_SOCKET;
	}

	if (m_AcceptConnentThread){
		delete m_AcceptConnentThread;
		m_AcceptConnentThread = NULL;
	}

	if (m_ReceiveDataThreadPool){
		delete m_ReceiveDataThreadPool;
		m_ReceiveDataThreadPool = NULL;
	}
}

void RongyiSocketServer::StaticAcceptConnentThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if (pWnd)
	{
		RongyiSocketServer* pThis = (RongyiSocketServer*)pWnd;
		pThis->AcceptConnentThreadRun(arg, pbCancelCurrentTask);
	}
}

void RongyiSocketServer::AcceptConnentThreadRun(void* arg, const bool* pbCancelCurrentTask)
{
	// arg is a char* which have no meaning, just used to start thread
	if (!m_ReceiveDataThreadPool){
		return;
	}

	// creat server
	m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(m_dwServerPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	int nRet = bind( m_listenSocket, (sockaddr*)&sin, (int)(sizeof(sin)));
	if ( nRet == SOCKET_ERROR ){
		unsigned long errCode = GetLastError();
		return;
	}

	listen( m_listenSocket, 5);
	sockaddr_in clientAddr;
	int nameLen = sizeof( clientAddr );
	while(false == *pbCancelCurrentTask && m_listenSocket != INVALID_SOCKET)
	{
		SOCKET clientSock = accept( m_listenSocket, (sockaddr*)&clientAddr, &nameLen );
		if (m_listenSocket == INVALID_SOCKET){
			break;
		}
		if ( clientSock == INVALID_SOCKET ){
			continue;
		}
		LOGOUT_INFO("[%d][%s]", clientSock, inet_ntoa(clientAddr.sin_addr));
		SocketClientTask* pClientTask = new SocketClientTask;
		if (pClientTask)
		{
			pClientTask->socketClient = clientSock;
			if (WSA_INVALID_EVENT != (pClientTask->evWaitEvent = WSACreateEvent()))
			{
				if (m_ReceiveDataThreadPool->PostRequest(pClientTask
					, this
					, &RongyiSocketServer::StaticReceiveDataThreadRun
					, &RongyiSocketServer::StaticStopCurrentTask
					, &RongyiSocketServer::StaticFreeTask)){
					continue;
				}
			}

			// Error Log
			delete pClientTask;
			pClientTask = NULL;
		}
	}
}

void RongyiSocketServer::StaticStopCurrentTask(void* pWnd, void*& pTask)
{
	if (pWnd)
	{
		RongyiSocketServer* pThis = (RongyiSocketServer*)pWnd;
		pThis->StopCurrentTask(pTask);
	}
}

void RongyiSocketServer::StopCurrentTask(void*& pTask)
{
	if (pTask){
		SocketClientTask* pClientTask = (SocketClientTask*)pTask;
		WSASetEvent(pClientTask->evWaitEvent);
	}
}

void RongyiSocketServer::StaticFreeTask(void* pWnd, void*& pTask)
{
	if (pWnd)
	{
		RongyiSocketServer* pThis = (RongyiSocketServer*)pWnd;
		pThis->FreeTask(pTask);
	}
}

void RongyiSocketServer::FreeTask(void*& pTask)
{
	if (pTask)
	{
		SocketClientTask* pClientTask = (SocketClientTask*)pTask;
		delete pClientTask;
		pTask = NULL;
	}
}


void RongyiSocketServer::StaticReceiveDataThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if (pWnd)
	{
		RongyiSocketServer* pThis = (RongyiSocketServer*)pWnd;
		pThis->ReceiveDataThreadRun(arg, pbCancelCurrentTask);
	}
}

void RongyiSocketServer::ReceiveDataThreadRun(void* arg, const bool* pbCancelCurrentTask)
{
	if (!arg){
		return;
	}

	SocketClientTask* pTask = (SocketClientTask*)arg;
	WSAEventSelect( pTask->socketClient, pTask->evWaitEvent, FD_READ | FD_CLOSE | FD_ALL_EVENTS);

	int iEvenIndex = 0;
	while(false == *pbCancelCurrentTask && m_listenSocket != INVALID_SOCKET)
	{
		iEvenIndex = WSAWaitForMultipleEvents( 1, &pTask->evWaitEvent, FALSE, WSA_INFINITE, FALSE );
		if (m_listenSocket == INVALID_SOCKET){
			closesocket(pTask->socketClient);
			break;
		}

		if ( iEvenIndex == WSA_WAIT_FAILED )
		{
			Sleep(1);
			continue;
		}
		else
		{ 
			WSAResetEvent( &pTask->evWaitEvent/*[ iEvenIndex - WSA_WAIT_EVENT_0]*/);
			WSANETWORKEVENTS wsaNetWorkEvent;
			int nRet = WSAEnumNetworkEvents( pTask->socketClient, pTask->evWaitEvent/*[iEvenIndex - WSA_WAIT_EVENT_0]*/, &wsaNetWorkEvent );
			if ( SOCKET_ERROR == nRet )
			{
				LONG lerr = GetLastError();
				Sleep(1);
				continue;
			}
			else if ( wsaNetWorkEvent.lNetworkEvents & FD_READ )
			{
				if ( wsaNetWorkEvent.iErrorCode[FD_READ_BIT] != 0 ){
					//occur error
					closesocket(pTask->socketClient);
					break;
				}else {
					ReceiveClientData(pTask);
				}
			}
			else if( wsaNetWorkEvent.lNetworkEvents & FD_CLOSE )
			{
				if ( wsaNetWorkEvent.iErrorCode[FD_CLOSE_BIT] != 0 ){
					//occur error
					closesocket(pTask->socketClient);
					break;
				}else{
					closesocket(pTask->socketClient);
					break;
				}  
			}

		} // iEvenIndex

	} // while

	WSACloseEvent(pTask->evWaitEvent);
}

void RongyiSocketServer::ReceiveClientData(SocketClientTask* &pTask)
{
	if(!pTask || !m_pFuncCallBack){
		return;
	}

	char* pDataTemp = NULL;
	char* pBufferResult = NULL;
	do 
	{
		pDataTemp = new char[READ_SOCKET_DATA_SIZE + 1];
		if (!pDataTemp){
			break;
		}

		memset(pDataTemp, 0, sizeof(char)* (READ_SOCKET_DATA_SIZE + 1));

		unsigned long dwCurrentLen = SOCKET_DATA_COMMON_SIZE + 1;
		pBufferResult = new char[dwCurrentLen];
		if (!pBufferResult){
			break;
		}
		memset(pBufferResult, 0, sizeof(char)* dwCurrentLen);
		unsigned long dwRevLen = 0;
		while(1)
		{
			int iReSize = recv(pTask->socketClient, pDataTemp, READ_SOCKET_DATA_SIZE, 0);
			if (iReSize == SOCKET_ERROR){
				closesocket( pTask->socketClient );
				break;
			}

			if (dwRevLen + iReSize > dwCurrentLen){
				if(!CommonFunction::ReAllocMem(pBufferResult, dwCurrentLen, dwCurrentLen + SOCKET_DATA_COMMON_SIZE)){
					break;
				}
				dwCurrentLen += SOCKET_DATA_COMMON_SIZE;
			}

			memcpy(pBufferResult + dwRevLen, pDataTemp, iReSize);
			dwRevLen += iReSize;

			if (iReSize < READ_SOCKET_DATA_SIZE){
				break;
			}
		}

		if (0 != dwRevLen)
		{
			string strFixedData;
			if(pTask->strBackupData.length() != 0){
				strFixedData = pTask->strBackupData + pBufferResult;
			}else{
				strFixedData = pBufferResult;
			}

			do 
			{
				int iStartFlagIndex = -1;
				int iEndFlagIndex = -1;
				const char* pCheck = strFixedData.c_str();
				DWORD dwCheckLen = strFixedData.length();
				for (int i = 0; i < dwCheckLen; ++i)
				{
					if(pCheck[i] == '#') {
						if(0 == i || (i > 0 &&pCheck[i -1] != '\\')) {
							iStartFlagIndex = i;
						}
					}else if(pCheck[i] == '$'){
						if(0 == i || (i > 0 &&pCheck[i -1] != '\\')) {
							iEndFlagIndex = i;
						}
					}

					if(iStartFlagIndex >= 0 && iEndFlagIndex >= 0){
						break;
					}
				}

				if(iEndFlagIndex >= 0) // get end
				{
					if(iStartFlagIndex >= 0 && iEndFlagIndex > iStartFlagIndex){ // get start
						string strRealData = string(pCheck + iStartFlagIndex + 1, iEndFlagIndex - iStartFlagIndex - 1);
						CommonFunction::StringReplace(strRealData, "\\#", "#");
						CommonFunction::StringReplace(strRealData, "\\$", "$");
						string strRet = (*m_pFuncCallBack)(m_pWmd, pTask->socketClient, strRealData);
						//send(socket, strRet.c_str(), strRet.length(), 0);
					}

					if(iEndFlagIndex == (dwCheckLen - 1)){
						pTask->strBackupData = "";
						break;
					}else{
						strFixedData = string(pCheck + iEndFlagIndex + 1, dwCheckLen - (iEndFlagIndex + 1));
						pTask->strBackupData = strFixedData;
					}
				}
				else
				{
					pTask->strBackupData = strFixedData;
					break;
				}
			} while(1);

			
		}

	} while(0);

	if (pDataTemp){
		delete []pDataTemp;
	}

	if (pBufferResult){
		delete []pBufferResult;
	}
}

bool RongyiSocketServer::SendDataToClient(const SOCKET& socketClient, const string& strData)
{
	if(SOCKET_ERROR == send(socketClient, strData.c_str(), strData.length(), 0)){
		LOGOUT_ERR("[%d]", WSAGetLastError());
		return false;
	}else{
		return true;
	}
}

string RongyiSocketServer::ByteToString(const byte* pbyData, const DWORD& dwLen)
{
	string strRet = "";
	for (int i = 0; i < dwLen; ++i)
	{
		// process tens
		byte byData = pbyData[i] / 16;
		if(byData >= 10 && byData <= 15) {
			byData = 'A' + byData - 10;
		}else {
			byData = '0' + byData;
		}
		strRet = strRet + string((char*)(&byData), 1);

		// process ones
		byData = pbyData[i] % 16;
		if(byData >= 10 && byData <= 15) {
			byData = 'A' + byData - 10;
		}else {
			byData = '0' + byData;
		}
		strRet = strRet + string((char*)(&byData), 1);
	}

	return strRet;
}

const unsigned SOCKET_CLIENT_DATA_SIZE = 10 * 1024;
const unsigned SOCKET_CLIENT_READ_BUFFER_SIZE = 1 * 1024;

////////////////////////////////// client ////////////////////////////////////////
RongyiSocketClient::RongyiSocketClient(string strServerAddr, unsigned dwServerPort, void* pWmd, RONGYISOCKET_CALLBACK pFuncCallBack)
	:m_dwStatus(RongyiClientStatus_init)
	,m_pWmd(pWmd)
	,m_pFuncCallBack(pFuncCallBack)
	,m_dwServerPort(dwServerPort)
	,m_strServerAddr(strServerAddr)
	,m_sclient(INVALID_SOCKET)
{
	m_pData = new char[SOCKET_CLIENT_DATA_SIZE];
	if (m_pData){
		memset(m_pData, 0x00, SOCKET_CLIENT_DATA_SIZE);
	}
	m_pReadBuffer = new char[SOCKET_CLIENT_READ_BUFFER_SIZE];
	if (m_pReadBuffer){
		memset(m_pReadBuffer, 0x00, SOCKET_CLIENT_READ_BUFFER_SIZE);
	}

	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data; 
	if(0 == WSAStartup(sockVersion, &data)){
		m_RecieveThread.PostRequest(NULL, this, &RongyiSocketClient::StaticOnReceiveThread, NULL, NULL);
	}
}

RongyiSocketClient::~RongyiSocketClient()
{
	if (m_sclient != INVALID_SOCKET){
		closesocket(m_sclient);
	}
	
	WSACleanup();
}

// thread function
void RongyiSocketClient::StaticOnReceiveThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if (pWnd)
	{
		RongyiSocketClient* pThis = (RongyiSocketClient*)pWnd;
		pThis->OnReceiveThread(arg, pbCancelCurrentTask);
	}
}


void RongyiSocketClient::OnReceiveThread(void* arg, const bool* pbCancelCurrentTask)
{
	if (m_dwStatus == RongyiClientStatus_Connected){
		return;
	}

	// init socket and connect server
	m_sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_sclient == INVALID_SOCKET){
		return ;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(m_dwServerPort);
	serAddr.sin_addr.S_un.S_addr = inet_addr(m_strServerAddr.c_str()); 
	if (connect(m_sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR){
		m_dwStatus = RongyiClientStatus_ConnectFailure;
		closesocket(m_sclient);
		return ;
	}

	m_dwStatus = RongyiClientStatus_Connected;

	/*char * sendData = "你好，TCP服务端，我是客户端!\n";
	send(sclient, sendData, strlen(sendData), 0);*/

	int dwReceiveSize = 0;
	unsigned dwReceiveSizeTotal = 0;
	while(1)
	{
		dwReceiveSize = recv(m_sclient, m_pReadBuffer, SOCKET_CLIENT_READ_BUFFER_SIZE, 0);
		if(dwReceiveSize > 0)
		{
			memcpy(m_pData + dwReceiveSizeTotal, m_pReadBuffer, dwReceiveSize);
			dwReceiveSizeTotal += dwReceiveSize;
			if (dwReceiveSize < SOCKET_CLIENT_READ_BUFFER_SIZE)
			{
				char* pPost = new char[dwReceiveSizeTotal + 1];
				memcpy(pPost, m_pData, dwReceiveSizeTotal);
				pPost[dwReceiveSizeTotal] = 0;
				m_ProcessDataThread.PostRequest(pPost, this, &RongyiSocketClient::StaticOnProcessDataThread, NULL, &RongyiSocketClient::StaticFreeReceiveDataTask);

				// send 
				dwReceiveSizeTotal = 0;
			}
		}
		else
		{
			m_dwStatus = RongyiClientStatus_Disconnected;
			break;
		}
	}
}

bool RongyiSocketClient::SendData(const char* pData, unsigned dwLen)
{
	if (m_dwStatus != RongyiClientStatus_Connected
		|| m_sclient == INVALID_SOCKET)
	{
		return false;
	}

	send(m_sclient, pData, dwLen, 0);

	return true;
}

void RongyiSocketClient::StaticFreeReceiveDataTask(void* pWnd, void*& pTask)
{
	if (pWnd)
	{
		RongyiSocketClient* pThis = (RongyiSocketClient*)pWnd;
		pThis->FreeReceiveDataTask(pTask);
	}
}

void RongyiSocketClient::FreeReceiveDataTask(void*& pTask)
{
	if (pTask)
	{
		char* pTemp = (char*) pTask;
		delete []pTemp;
		pTask = NULL;
	}
}

void RongyiSocketClient::StaticOnProcessDataThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if (pWnd)
	{
		RongyiSocketClient* pThis = (RongyiSocketClient*)pWnd;
		pThis->OnProcessDataThread(arg, pbCancelCurrentTask);
	}
}

void RongyiSocketClient::OnProcessDataThread(void* arg, const bool* pbCancelCurrentTask)
{
	if (!arg){
		return;
	}

	char* pTemp = (char*)arg;

	// do function
	if (m_pFuncCallBack)
	{
		(*m_pFuncCallBack)(m_pWmd, m_sclient, pTemp);
	}
}