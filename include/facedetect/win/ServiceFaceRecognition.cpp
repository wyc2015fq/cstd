#include "StdAfx.h"
#include "ServiceFaceRecognition.h"
#include "MaidianData.h"
#include "CommonFunction.h"
#include "MyLog.h"
#include "RongyiTimer.h"
#include "MyLock.h"
#include "RongyiCHttpClient.h"
#include "RongyiSyncEvent.h"
#include <sys/stat.h>
#include "ZipImplement.h"
#include "RongyiCHttpClient.h"
#include "md5.h"

const DWORD CHECK_DATACHANGE_TIMER_SEC		= 5;
const DWORD UPLOAD_IMAGE_TIMER_SEC			= 5 * 60;
const DWORD FR_SOCKET_SERVER_PORT			= 30101;

ServiceFaceRecognition::ServiceFaceRecognition(const string& strDataPath
	, string strStatusServiceAddr
	, string strUploadServiceAddr
	, void* pBackappServerWnd
	, PFUNC_INTERNAL_SERVICE_API pFuncInterService
	, DWORD dwUploadSpeedKB)
	: ServiceBase("ServiceFaceRecognition", strDataPath, pBackappServerWnd, pFuncInterService)
	, m_bStartFunction(false)
	, m_strStatusReportServiceAddr(strStatusServiceAddr)
	, m_strUploadServiceAddr(strUploadServiceAddr)
	, m_pSocketServer(NULL)
	, m_bDataChanged(true)
	, m_dwCheckDataChangeTimer(0)
	, m_dwUploadImageTimer(0)
	, m_bOpenDB(false)
	, m_dwUploadSpeedKB(dwUploadSpeedKB)
{
	InitializeCriticalSection(&m_csLastDataLock);
	InitializeCriticalSection(&m_csCreateSocket);
	InitializeCriticalSection(&m_csDBOperate);
	string strPathTemp = CommonFunction::GetExePath();
	string strBackappPath =  string(strPathTemp.c_str(), strPathTemp.rfind("\\"));
	m_strFRDir = strBackappPath + "\\rongyiapp-faceRecgnition";

	// kill exe since BackExe maybe restarted but the FR Exe don't detach
	string strCmd = "taskkill /im \"" + string(FR_TERGETEXE_NAME) + "\" /f";
	system(strCmd.c_str());

	//////////////////////////////////////////////////////////////////////////
	{
		HANDLE hMutexFileChange = ::CreateMutexA(0, false, RONGYI_CHANGE_FILE_MUTEX_NAME);
		if (hMutexFileChange){
			WaitForSingleObject(hMutexFileChange, DEFAULT_WAIT_CHANGE_FILES_TIME_SEC);
		}

		string strMachineNoFilePath = strBackappPath + "\\terminalsetting.json";
		string strTerminalSettingContent;
		CommonFunction::ReadContentFromFile(strMachineNoFilePath, strTerminalSettingContent);

		if (hMutexFileChange){
			ReleaseMutex(hMutexFileChange);
			hMutexFileChange = NULL;
			CloseHandle(hMutexFileChange);
		}

		JsonContentMap mapRet;
		MyJsonValue::ParserJsonObject(strTerminalSettingContent, mapRet);
		MyJsonValue::GetStringJsonParamFromMap(&mapRet, "machineno", m_strCurrentMachineNo);
		LOGOUT_INFO("[%s]", m_strCurrentMachineNo.c_str());
	}
	//////////////////////////////////////////////////////////////////////////
}

ServiceFaceRecognition::~ServiceFaceRecognition(void)
{
	DeleteCriticalSection(&m_csDBOperate);
	DeleteCriticalSection(&m_csCreateSocket);
	DeleteCriticalSection(&m_csLastDataLock);
}


/*virtual*/
string ServiceFaceRecognition::DoFunc(const string& strFunc, const string& strData)
{
	LOGOUT_INFO("[%s][%s]", strFunc.c_str(), strData.c_str());
	string strSubFunc;
	string strParam;
	GetFuntionParam(strData, strSubFunc, strParam);

	if ("start" == strSubFunc)
	{
		return StartFRFunction(strParam);
	}
	else if ("stop" == strSubFunc)
	{
		return StopFRFunction(strParam);
	}
	else
	{
		ERROR_RETURN(1, "ServiceFaceRecognition function type err");
	}
}

//(unsigned long dwPort, unsigned long dwClientMaxCount, void* pWmd, RONGYISOCKET_CALLBACK pFuncCallBack);
string ServiceFaceRecognition::StartFRFunction(const string& strData)
{
	LOGOUT_INFO("");
	// start socket server
	// CAUTION! WSAStartup first!!! 
	if(!RongyiSocketServer::RongyiWSAStartup()){
		ERROR_RETURN(111, "socket start error");
	}

	{
		MyCriticalLock lock(m_csCreateSocket);
		if(!m_pSocketServer){
			m_pSocketServer = new RongyiSocketServer(FR_SOCKET_SERVER_PORT, 5/*Mac conecttion count*/, this, ServiceFaceRecognition::StaticRecieveClientData);
		}
	}
	

	if(!m_pSocketServer){
		ERROR_RETURN(112, "socket Mem error");
	}

	// start face recognition
	string strCmd = "taskkill /im \"" + string(FR_TERGETEXE_NAME) + "\" /f";
	system(strCmd.c_str());

	string strStartPRBatPath = m_strFRDir + "\\startFR.bat";
	if(!CommonFunction::IsPathExist(strStartPRBatPath)){
		///////////////////////////////// set bat /////////////////////////////////////////
		string strDisk = string(m_strFRDir.c_str(), m_strFRDir.find(":") + 1);
		strCmd = strDisk + "\n";
		string strCdPath = string(m_strFRDir.c_str() + m_strFRDir.find(":") + 1, m_strFRDir.length() - m_strFRDir.find(":") - 1);
		strCmd = strCmd + "cd " + strCdPath + "\n";
		strCmd = strCmd + "start \"\" \"" + m_strFRDir + "\\faceRecgnition.exe\"";
		CommonFunction::WriteContentToFile(strStartPRBatPath, strCmd, eWriteMode_OverWrite);
	}
	CommonFunction::RunBatCmdFile(strStartPRBatPath);

	// openDB
	OpenDB((m_strFRDir + "\\" + FR_SQLITEDB_NAME).c_str());

	// set status for the first time data checked(since the FR.EXE maybe get some error and start fialure)
	m_bDataChanged = true;

	// start check Timer
	if(m_dwCheckDataChangeTimer != 0){
		RongyiTimer::KillRongyiTimer(m_dwCheckDataChangeTimer);
		m_dwCheckDataChangeTimer = 0;
	}
	m_dwCheckDataChangeTimer = RongyiTimer::SetRongyiTimer(CHECK_DATACHANGE_TIMER_SEC * 1000, this, &ServiceFaceRecognition::StaticOnRongyiTimer);

	if(m_dwUploadImageTimer != 0){
		RongyiTimer::KillRongyiTimer(m_dwUploadImageTimer);
		m_dwUploadImageTimer = 0;
	}
	m_dwUploadImageTimer = RongyiTimer::SetRongyiTimer(UPLOAD_IMAGE_TIMER_SEC * 1000, this, &ServiceFaceRecognition::StaticOnRongyiTimer);
	void* pbyte = new (void*);
	m_cCheckFunctionThread.PostRequest(pbyte, this, &ServiceFaceRecognition::StaticOnCheckFunctionThread, NULL, NULL);

	ERROR_RETURN(0, "success");
}

string ServiceFaceRecognition::StopFRFunction(const string& strData)
{
	if(m_dwUploadImageTimer != 0){
		RongyiTimer::KillRongyiTimer(m_dwUploadImageTimer);
		m_dwUploadImageTimer = 0;
	}

	if(m_dwCheckDataChangeTimer != 0){
		RongyiTimer::KillRongyiTimer(m_dwCheckDataChangeTimer);
		m_dwCheckDataChangeTimer = 0;
	}

	m_bDataChanged = false;

	// stop face recognition 
	CloseDB();
	m_bOpenDB = false;

	string strCmd = "taskkill /im \"" + string(FR_TERGETEXE_NAME) + "\" /f";
	system(strCmd.c_str());

	// stop socket server
	{
		MyCriticalLock lock(m_csCreateSocket);
		if(m_pSocketServer){
			delete m_pSocketServer;
			m_pSocketServer = NULL;
		}
	}

	// CAUTION! Check if there is other socket user to decide if need WSACleanup
	// Case the WebSocket function used the socket, CANNOT stop socket
	// RongyiSocketServer::RongyiWSACleanup();

	ERROR_RETURN(0, "success");
}

void ServiceFaceRecognition::SetMallIdAndTerminalId(const string& strMallId,const string& strTernimalId)
{
	LOGOUT_INFO("in");
	m_strCurrentMallID = strMallId;
	m_strCurrentTerminalID = strTernimalId;
	m_cCheckFunctionThread.PostRequest(NULL, this, &ServiceFaceRecognition::StaticOnCheckFunctionThread, NULL, NULL);
}

void ServiceFaceRecognition::StaticOnCheckFunctionThread(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if(pWnd){
		ServiceFaceRecognition* pThis = (ServiceFaceRecognition*)pWnd;
		pThis->OnCheckFunctionThread(arg, pbCancelCurrentTask);
	}
}

void ServiceFaceRecognition::OnCheckFunctionThread(void* arg, const bool* pbCancelCurrentTask)
{
	if(arg){
		UploadImage();
		return;
	}

	LOGOUT_INFO("");
	if(m_strCurrentMallID != "")
	{
		RongyiCHttpClient cHttp;
		do 
		{
			string strHttpRet;
			int iRet = cHttp.Post(m_strStatusReportServiceAddr + "/api/v1/report/isreportface", "mall_id=" + m_strCurrentMallID, strHttpRet);
			LOGOUT_INFO("[%d][%s][%s]", iRet, (m_strStatusReportServiceAddr + "/api/v1/report/isreportface?" + "mall_id=" + m_strCurrentMallID).c_str(), strHttpRet.c_str());
			if(0 == iRet){
				JsonContentMap mapRet;
				string strResult, strEnable;
				MyJsonValue::ParserJsonObject(strHttpRet, mapRet);
				MyJsonValue::GetStringJsonParamFromMap(&mapRet, "result", strResult);
				MyJsonValue::ParserJsonObject(strResult, mapRet);
				MyJsonValue::GetStringJsonParamFromMap(&mapRet, "enable", strEnable);
				if("true" == strEnable || "TRUE" == strEnable){
					LOGOUT_INFO("start FR");
					StartFRFunction("");
					break;
				}else if("false" == strEnable || "FALSE" == strEnable){
					LOGOUT_INFO("stop FR");
					StopFRFunction("");
					break;
				}else{
					// retry
					Sleep(10 * 1000);
				}
			}
			else
			{
				// retry
				Sleep(10 * 1000);
			}
		} while(1);
	}
	else
	{
		LOGOUT_INFO("stop FR");
		StopFRFunction("");
	}
}

string ServiceFaceRecognition::StaticRecieveClientData(void* pWnd, const SOCKET& sid, const std::string& strRevData)
{
	if(pWnd){
		ServiceFaceRecognition* pThis = (ServiceFaceRecognition*)pWnd;
		return pThis->RecieveClientData(sid, strRevData);
	}else{
		return "Mem Err";
	}
}

string ServiceFaceRecognition::RecieveClientData(const SOCKET& sid, const std::string& strRevData)
{
	string strType;
	JsonContentMap mapRet;
	MyJsonValue::ParserJsonObject(strRevData, mapRet);
	MyJsonValue::GetStringJsonParamFromMap(&mapRet, "type", strType);
	if(SOCKET_MESSAGE_NEW == strType){
		// new face deteched
		MyCriticalLock lock(m_csLastDataLock);
		m_bDataChanged = true;

	}else if(SOCKET_MESSAGE_START == strType){
		// start
		MyCriticalLock lock(m_csLastDataLock);
		m_bDataChanged = true;
	}else{
		LOGOUT_ERR("[%s]", strType.c_str());
	}

	return "";

	/*
	sClientData* pData = new sClientData;
	if(pData){
		pData->pChData = new char[strRevData.length() + 1];
		if(pData->pChData){
			memcpy(pData->pChData, strRevData.c_str(), strRevData.length());
			pData->pChData[strRevData.length()] = '\0';
			pData->socket = sid;
			m_ClientDataProcessThread.PostRequest((void*)pData, this, &ServiceFaceRecognition::StaticOnClientDataProcessThread);
			return "success";
		}else{
			delete pData;
			return "Mem Err";
		}
	}else{
		return "Mem Err";
	}*/
}

void ServiceFaceRecognition::FreeThreadData(void* &pData)
{
	if(!pData){
		return;
	}

	sClientData* pDes = (sClientData*)pData;
	delete pDes;
	pDes = NULL;
	pData = NULL;
}

// timer
void ServiceFaceRecognition::StaticOnRongyiTimer(void* pWnd, const unsigned long& dwTimerID)
{
	if (pWnd)
	{
		ServiceFaceRecognition* pThis = (ServiceFaceRecognition*)pWnd;
		pThis->OnRongyiTimer(dwTimerID);
	}
}

void ServiceFaceRecognition::OnRongyiTimer(const unsigned long& dwTimerID)
{
	if(m_dwCheckDataChangeTimer == dwTimerID)
	{
		CheckDataChange();
	}
	else if(m_dwUploadImageTimer == dwTimerID)
	{
		UploadImage();
	}
}

void ServiceFaceRecognition::CheckDataChange()
{
	{
		MyCriticalLock lock(m_csLastDataLock);
		if(!m_bDataChanged){
			return;
		}else{
			m_bDataChanged = false;
		}
	}

	// Read DataBase
	CheckDB();
}

void ServiceFaceRecognition::CheckDB()
{
	// 1. check
	string strSQLiteData = GetSatifyQueryConditionData("select rowid, * from faceinfo where send=0", FR_SEND_MAXCOUNT);
	//LOGOUT_ERR("the strSQLiteData is [%s]", strSQLiteData.c_str());
	if (strSQLiteData == "")
	{
		//LOGOUT_INFO("the strSQLiteData is empty");
		return;
	}
	//LOGOUT_INFO("[%s]", strSQLiteData.c_str());

	// 2. send
	JsonContentList listRet;
	MyJsonArray::ParserJsonArray(strSQLiteData, listRet);
	for (JsonContentList::iterator itr = listRet.begin(); itr != listRet.end(); ++itr)
	{
		string strJsonTemp = *itr;

		string strSendData, strSetSql;
		JsonContentMap mapRet;
		MyJsonValue::ParserJsonObject(strJsonTemp, mapRet);
		MyJsonValue::GetStringJsonParamFromMap(&mapRet, "data", strSendData);
		MyJsonValue::GetStringJsonParamFromMap(&mapRet, "sql", strSetSql);

		RongyiCHttpClient cHttp;
		string strSendResult;
		string strPostData = "mall_id=" + m_strCurrentMallID + "&terminal_id=" + m_strCurrentTerminalID + "&face_data=" + strSendData;
		int iSendRet = cHttp.Post(m_strStatusReportServiceAddr + "/api/v1/report/face", strPostData, strSendResult);
		if(0 == iSendRet){
			JsonContentMap mapRet;
			string strMeta, strErrNo;
			MyJsonValue::ParserJsonObject(strSendResult, mapRet);
			MyJsonValue::GetStringJsonParamFromMap(&mapRet, "meta", strMeta);
			MyJsonValue::ParserJsonObject(strMeta, mapRet);
			MyJsonValue::GetStringJsonParamFromMap(&mapRet, "errno", strErrNo);
			if("0" == strErrNo){
				// set
				UpataDB(strSetSql);
			}else{
				LOGOUT_ERR("[%s][%s]", m_strStatusReportServiceAddr.c_str(), strErrNo.c_str());
			}
		}else{
			LOGOUT_ERR("[%s][%d]", m_strStatusReportServiceAddr.c_str(), iSendRet);
		}
	}
}

////////////////////////////////// DB ////////////////////////////////////////
void ServiceFaceRecognition::OpenDB(const char* fileName)
{
	try
	{
		if (!m_bOpenDB	&& CommonFunction::IsPathExist(fileName))
		{
			m_faceDB.open(fileName);
			m_bOpenDB = true;

			CheckAndUploadImageColumn();
		}
	}
	catch (CppSQLite3Exception e)
	{
		LOGOUT_ERR("open face db error: [%s].", e.errorMessage());	
	}
}

void ServiceFaceRecognition::CloseDB()
{
	if (m_bOpenDB)
	{
		m_faceDB.close();
	}
}

void ServiceFaceRecognition::CheckAndUploadImageColumn()
{
	if(!m_bOpenDB){
		return;
	}

	string querySQL = "select sql from sqlite_master where type = 'table' and name = 'faceinfo'";

	try
	{
		MyCriticalLock lock(m_csDBOperate);

		CppSQLite3Query queryReult; 
		try
		{
			queryReult = m_faceDB.execQuery(querySQL.c_str());
		}
		catch (CppSQLite3Exception e)
		{
			LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
		}

		if (!queryReult.eof()) {
			string strSql = queryReult.getStringField("sql");
			int iMask = strSql.find("send");
			if(iMask < 0) {
				querySQL = "alter table faceinfo add column send INT default 0";
				try
				{
					m_faceDB.execDML(querySQL.c_str());
				}
				catch (CppSQLite3Exception e)
				{
					LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
				}	
			}

			iMask = strSql.find("imgupload");
			if(iMask < 0) {
				querySQL = "alter table faceinfo add column imgupload INT default 0";
				try
				{
					m_faceDB.execDML(querySQL.c_str());
				}
				catch (CppSQLite3Exception e)
				{
					LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
				}	
			}

			iMask = strSql.find("sideface");
			if(iMask < 0) {
				querySQL = "alter table faceinfo add column sideface INT default 0";
				try
				{
					m_faceDB.execDML(querySQL.c_str());
				}
				catch (CppSQLite3Exception e)
				{
					LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
				}	
			}

		}
	}
	catch (CppSQLite3Exception e)
	{
		LOGOUT_ERR("[%s][%s]", querySQL.c_str(), e.errorMessage());
	}
}

bool ServiceFaceRecognition::UpataDB(const string& strSql)
{
	if (m_bOpenDB)
	{
		try
		{
			MyCriticalLock lock(m_csDBOperate);
			m_faceDB.execDML(strSql.c_str());
			
			return true;
		}
		catch (CppSQLite3Exception e)
		{
			LOGOUT_ERR("[%s][%s]", strSql.c_str(), e.errorMessage());
			return false;
		}
	}

	return false;
}

string ServiceFaceRecognition::GetFaceRecognitionDBName()
{
	string strDest = m_strFRDir + "\\" + FR_SQLITEDB_NAME;
	return strDest;
}

string ServiceFaceRecognition::GetSatifyQueryConditionData(const string& querySQL, int num)
{
	if (!CommonFunction::IsPathExist(GetFaceRecognitionDBName()))
	{
		LOGOUT_ERR("the [%s] doesn't exist.", GetFaceRecognitionDBName().c_str());

		return "";
	}
	
	OpenDB(GetFaceRecognitionDBName().c_str());

	CppSQLite3Query queryReult;
	{
		MyCriticalLock lock(m_csDBOperate);
		try
		{
			queryReult = m_faceDB.execQuery(querySQL.c_str());
		}
		catch (CppSQLite3Exception e)
		{
			LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
		}
	}
	
	cJSON *pJsRoot = cJSON_CreateArray();
	cJSON *pJsRootItem = cJSON_CreateObject();
	cJSON *pJsRootItemDataValue = cJSON_CreateArray();

	int iIndex = 0;

	std::ostringstream ss;
	string rowIdFields;

	int faceId, enterTime, leftTime, rowId, sideface, age, gender, glass, smile;
	while (!queryReult.eof()) {

		rowId = queryReult.getIntField("rowid");
		faceId = queryReult.getIntField("faceid");
		enterTime = queryReult.getIntField("ftime");
		leftTime = queryReult.getIntField("stime");
		sideface = queryReult.getIntField("sideface");
		age = queryReult.getIntField("age");
		gender = queryReult.getIntField("gender");
		glass = queryReult.getIntField("glass");
		smile = queryReult.getIntField("smile");

		ss << rowId;
		rowIdFields = rowIdFields + ss.str() + ",";
		ss.str("");

		cJSON *pJsRootItemDataValueItem = cJSON_CreateArray();
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(rowId));		
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(enterTime));		
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(leftTime));		
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(faceId));		
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(sideface));
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(age));
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(gender));
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(glass));
		cJSON_AddItemToArray(pJsRootItemDataValueItem, cJSON_CreateNumber(smile));

		cJSON_AddItemToArray(pJsRootItemDataValue, pJsRootItemDataValueItem);

		++iIndex;
		if (iIndex == num)
		{
			// rest the iIndex for satisfy self-define num
			iIndex = 0;

			string updateSignCmd = "update faceinfo set send=1 where rowid in (" 
				+ string(rowIdFields, 0, rowIdFields.length()-1) + ")";
			rowIdFields.clear();

			cJSON_AddItemToObject(pJsRootItem, "data", cJSON_Duplicate(pJsRootItemDataValue, 1));
			cJSON_AddItemToObject(pJsRootItem, "sql", cJSON_CreateString(updateSignCmd.c_str()));
			cJSON_AddItemToArray(pJsRoot, cJSON_Duplicate(pJsRootItem, 1));
			cJSON_Delete(pJsRootItemDataValue);
			cJSON_Delete(pJsRootItem);

			pJsRootItem = cJSON_CreateObject();
			pJsRootItemDataValue = cJSON_CreateArray();
		}

		queryReult.nextRow();

		if (queryReult.eof()
			&& (iIndex > 0))
		{
			string updateSignCmd = "update faceinfo set send=1 where rowid in (" 
				+ string(rowIdFields, 0, rowIdFields.length()-1) + ")";
			rowIdFields.clear();
			cJSON_AddItemToObject(pJsRootItem, "data", cJSON_Duplicate(pJsRootItemDataValue, 1));
			cJSON_AddItemToObject(pJsRootItem, "sql", cJSON_CreateString(updateSignCmd.c_str()));
			cJSON_AddItemToArray(pJsRoot, cJSON_Duplicate(pJsRootItem, 1));
			cJSON_Delete(pJsRootItemDataValue);
			cJSON_Delete(pJsRootItem);
		}
	}

	char *out = NULL;
	out = cJSON_PrintUnformatted(pJsRoot);
	string strRet(out);
	cJSON_Delete(pJsRoot);
	free(out);

	return strRet;
}

void ServiceFaceRecognition::UploadImage()
{
	// 1. get image list wait to upload from DB
	IMAGESET_LIST ImangeSet = __GetUploadImageListFromDB();
	if (ImangeSet.empty())
	{
		LOGOUT_INFO("the imageSet is empty");
		return;
	}
	
	CZipImplement cZip;
	RongyiCHttpClient cHttp;
	string strUrl = m_strUploadServiceAddr + "/api/v1/report/faceres";
	string strUploadTempFolder = m_strFRDir + "\\uploadTemp";
	CommonFunction::DeleteDirectory(strUploadTempFolder);

	string strProcessPath = strUploadTempFolder + "\\image";

	// 2. zip
	for (IMAGESET_LIST::iterator itr = ImangeSet.begin(); itr != ImangeSet.end(); ++itr)
	{
		sImageInfo sInfo = *itr;

		// 2.1 create and copy image
		if(!CommonFunction::DeleteDirectory(strProcessPath)){
			continue;
		}

		CommonFunction::CreatePath(strProcessPath);
		for (IMAGEITEM_LIST::iterator secItr = sInfo.strImageItemList.begin(); secItr != sInfo.strImageItemList.end(); ++secItr)
		{
			string strItemPath = strProcessPath + "\\" + (*secItr).strImageNewRelativeName;
			string strSrcImage = (*secItr).strImagePath;
			CommonFunction::StringReplace(strSrcImage, "/", "\\");
			CopyFileA(strSrcImage.c_str(), strItemPath.c_str(), false);
		}

		string strZipFile = m_strFRDir + "\\uploadTemp\\" + m_strCurrentMallID  + "_" + m_strCurrentTerminalID + "_" + CommonFunction::GetCurrentTimeSec() + ".zip";
		bool bZipRet = cZip.Zip_PackFiles(CommonFunction::StringToWstring(strProcessPath, CP_UTF8), CommonFunction::StringToWstring(strZipFile, CP_UTF8));
		if (!bZipRet){
			continue;
		}

		// 3. upload
		MD5 md5;
		md5.Update(ifstream(strZipFile, ios::binary));
		string strCurMD5 = string(md5.ToString().c_str());

		string strResponse;
		HttpPostFileParam httpParam;
		httpParam.cFileParam[0].strName = "file";
		httpParam.cFileParam[0].strValue = strZipFile;

		httpParam.cValueParam[0].strName = "mall_id";
		httpParam.cValueParam[0].strValue = m_strCurrentMallID;
		httpParam.cValueParam[1].strName = "terminal_id";
		httpParam.cValueParam[1].strValue = m_strCurrentTerminalID;
		httpParam.cValueParam[2].strName = "md5sum";
		httpParam.cValueParam[2].strValue = strCurMD5;
		httpParam.cValueParam[3].strName = "machine_no";
		httpParam.cValueParam[3].strValue = m_strCurrentMachineNo;
		int ret = 0;
		{
			LOGOUT_TRACE("SendFile");
			ret = cHttp.http_post_file(strUrl.c_str(), httpParam, strResponse, m_dwUploadSpeedKB);
		}
		LOGOUT_INFO("upload[%d][%s][%s][%s]", ret, strUrl.c_str(), strZipFile.c_str(), strResponse.c_str());

		// 4. update to DB
		if(ret == 0){
			JsonContentMap mapRet;
			string strErrNO, strMeta;
			MyJsonValue::ParserJsonObject(strResponse, mapRet);
			MyJsonValue::GetStringJsonParamFromMap(&mapRet, "meta", strMeta);
			MyJsonValue::ParserJsonObject(strMeta, mapRet);
			MyJsonValue::GetStringJsonParamFromMap(&mapRet, "errno", strErrNO);
			if("0" == strErrNO){
				UpataDB(sInfo.strUpdateSql);
			}
		}
	}
}

IMAGESET_LIST ServiceFaceRecognition::__GetUploadImageListFromDB(const DWORD& dwSetCount /* = 200 */)
{
	IMAGESET_LIST SetRet;
	SetRet.clear();

	if (!CommonFunction::IsPathExist(GetFaceRecognitionDBName()))
    {
        return SetRet;
    }

	OpenDB(GetFaceRecognitionDBName().c_str());

	string strSql = "select faceinfo.rowid,faceimg.img from faceinfo,faceimg where faceinfo.send=1 and faceinfo.imgupload=0 and faceinfo.imgid=faceimg.rowid";

	CppSQLite3Query queryReult;
	{
		MyCriticalLock lock(m_csDBOperate);
		try
		{
			queryReult = m_faceDB.execQuery(strSql.c_str());
		}
		catch (CppSQLite3Exception e)
		{
			LOGOUT_ERR("open face db error: [%s].", e.errorMessage());
		}
	}

	int iSetIndex = 0;
	sImageInfo sImgList;
	while (!queryReult.eof())
	{
		// get Item
		sImageItem sItem;
		int iRowId = queryReult.getIntField("rowid");
		char szRowID[12] = {0};
		string strRowId = itoa(iRowId, szRowID, 10);
		sItem.strImageNewRelativeName = strRowId + ".jpg";
		sItem.strImagePath = queryReult.getStringField("img");
	
		// add item to list
		sImgList.strImageItemList.push_back(sItem);
		sImgList.strUpdateSql = sImgList.strUpdateSql + strRowId + ",";

		queryReult.nextRow();

		// check set
		++iSetIndex;
		if(iSetIndex == dwSetCount
			|| (queryReult.eof() && (iSetIndex > 0)))
		{
			sImgList.strUpdateSql = "update faceinfo set imgupload=1 where rowid in (" 
				+ string(sImgList.strUpdateSql.c_str(), sImgList.strUpdateSql.length() - 1) 
				+ ")";
			SetRet.push_back(sImgList);
			sImgList.Clear();
			iSetIndex = 0;
		}
	}

	return SetRet;
}