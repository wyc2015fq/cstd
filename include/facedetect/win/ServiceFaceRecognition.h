#pragma once
#include "servicebase.h"
#include "RongyiAsyncThread.h"
#include "CommonFunction.h"
#include "RongyiSocket.h"
#include "CppSQLite3.h"


struct sClientData
{
	SOCKET socket;
	char*	pChData;
	sClientData()
	{
		socket = SOCKET_ERROR;
		pChData = NULL;
	}
	~sClientData()
	{
		if(pChData){
			delete []pChData;
			pChData = NULL;
		}
		socket = SOCKET_ERROR;
	}
};


// define message
/*
{
	"type":"new", 
	"data":{}
}
*/

const char SOCKET_MESSAGE_NEW[]			= "new";
const char SOCKET_MESSAGE_START[]		= "start";
const char FR_SQLITEDB_NAME[]			= "facedb\\face.db";
const char FR_TERGETEXE_NAME[]			= "faceRecgnition.exe";
const DWORD FR_SEND_MAXCOUNT			= 20;

struct sImageItem
{
	string strImagePath;
	string strImageNewRelativeName;
	sImageItem(){
		strImagePath = "";
		strImageNewRelativeName = "";
	}
};

typedef list<sImageItem> IMAGEITEM_LIST;

struct sImageInfo
{
	string strUpdateSql;
	IMAGEITEM_LIST strImageItemList;
	sImageInfo()
	{
		Clear();
	}

	void Clear(){
		strUpdateSql = "";
		strImageItemList.clear();
	}
};

typedef list<sImageInfo> IMAGESET_LIST;

class ServiceFaceRecognition : public ServiceBase
{
public:
	ServiceFaceRecognition(const string& strDataPath,
		string strStatusServiceAddr,
		string strUploadServiceAddr,
		void* pBackappServerWnd,
		PFUNC_INTERNAL_SERVICE_API pFuncInterService,
		DWORD dwUploadSpeedKB);
	~ServiceFaceRecognition(void);

	
public:
	virtual string DoFunc(const string& strFunc, const string& strData);

	string StartFRFunction(const string& strData);
	string StopFRFunction(const string& strData);

	void SetMallIdAndTerminalId(const string& strMallId,const string& strTernimalId);


private:
	///////////////////////////////// DB /////////////////////////////////////////
	void OpenDB(const char* fileName);
	void CloseDB();
	bool UpataDB(const string& strSql);
	string GetFaceRecognitionDBName();
	string GetSatifyQueryConditionData(const string& querySQL, int num);
	void CheckAndUploadImageColumn();

	CppSQLite3DB m_faceDB;
	bool m_bOpenDB;


private:
	bool	m_bStartFunction;
	CRITICAL_SECTION m_csCreateSocket;
	RongyiSocketServer* m_pSocketServer;
	void FreeThreadData(void* &pData);
	CRITICAL_SECTION m_csLastDataLock;
	bool m_bDataChanged;
	DWORD m_dwCheckDataChangeTimer ;
	string m_strFRDir;
	string m_strCurrentMallID;
	string m_strCurrentTerminalID;
	string m_strCurrentMachineNo;
	string m_strStatusReportServiceAddr;
	string m_strUploadServiceAddr;
	DWORD m_dwUploadSpeedKB;

	CRITICAL_SECTION m_csDBOperate;
	DWORD m_dwUploadImageTimer ;
	void UploadImage();
	IMAGESET_LIST __GetUploadImageListFromDB(const DWORD& dwSetCount = 200);

	// socket
	static string StaticRecieveClientData(void* pWnd, const SOCKET& sid, const std::string& strRevData);
	string RecieveClientData(const SOCKET& sid, const std::string& strRevData);
		

	// timer
	static void StaticOnRongyiTimer(void* pWnd, const unsigned long& dwTimerID);
	void OnRongyiTimer(const unsigned long& dwTimerID);
	void CheckDataChange();

	void CheckDB();

	RongyiAsyncThread m_cCheckFunctionThread;
	static void StaticOnCheckFunctionThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask);
	void OnCheckFunctionThread(void* arg, const bool* pbCancelCurrentTask);
};

