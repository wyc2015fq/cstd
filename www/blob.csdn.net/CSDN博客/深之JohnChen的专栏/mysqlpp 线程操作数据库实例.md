# mysqlpp 线程操作数据库实例 - 深之JohnChen的专栏 - CSDN博客

2018年06月08日 11:20:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：549



//MySqlConnectionPool.h

```cpp
#pragma once

#include "mysqlpp/include/cpool.h"
#include "mysqlpp/include/connection.h"
#include "boost/thread/thread.hpp"    //boost库线程头文件
#include <string>
#include <vector>
#include "DatabaseDataDefine.h"

class MySqlConnection : public mysqlpp::Connection
{
public:
	MySqlConnection() : itime_(time(0)) { }
	time_t instantiation_time() const { return itime_; }

private:
	time_t itime_;
};

class MySqlConnectionPool : public mysqlpp::ConnectionPool
{
public:
	MySqlConnectionPool(const char* pDatabaseName, const char* pServer, const char* pUserName, const char* pPassword, unsigned int uiPort,const char *pCharsetNameOption="gbk",unsigned int nMaxConnectNum=5, unsigned int nMinRecordRows=15) {
		strcpy_s(m_szDatabaseName, 127, pDatabaseName);
		strcpy_s(m_szServer, 63, pServer);
		strcpy_s(m_szUserName, 63, pUserName);
		strcpy_s(m_szPassword, 63, pPassword);
		strcpy_s(m_szCharsetNameOption, 31, pCharsetNameOption);
		m_uiPort = uiPort;
		m_nMaxConnectNum = nMaxConnectNum;
		m_nMinRecordRows = nMinRecordRows;
		m_conns_in_use = 0;
		m_pThreadObject = NULL;
		m_bThreadExit = true;
		m_listBatchEventLogSql.clear();
		m_vecBatchEventLogSql.clear();
	};

	virtual ~MySqlConnectionPool() {};

	//测试数据库连接
	static bool	TestConnect(char *pDatabaseName, char *pServer, char *pUsernmae,char *pPassword,unsigned int uiPort);

	//start thread
	int		startThrad(void *pParameter);
	//stop thread
	int		stopThread();

	unsigned int max_idle_time() { return 10; };

	mysqlpp::Connection* grab()
	{
		while (m_conns_in_use > m_nMaxConnectNum) {
			Sleep(1000);
		}

		++m_conns_in_use;
		return mysqlpp::ConnectionPool::grab();
	}

	// Other half of in-use conn count limit
	void release(const mysqlpp::Connection* pc)
	{
		mysqlpp::ConnectionPool::release(pc);
		--m_conns_in_use;
	}

	//插入批量语句
	int InsertBatchSql(char *pSql);

	//插入事件日志
	void InsertBatchEventLog(DB_DeviceEventLogInfo *pEventLogInfo);

private:
	MySqlConnection* create() { 
		MySqlConnection *pMyConn = new MySqlConnection();
		if (pMyConn != NULL)
		{
			/*
			mysqlpp::MultiStatementsOption *optMultiStatementsOption = new mysqlpp::MultiStatementsOption(true);
			pMyConn->set_option(optMultiStatementsOption);
			*/

			mysqlpp::ConnectTimeoutOption * optConnectTimeoutOption = new  mysqlpp::ConnectTimeoutOption(10);
			pMyConn->set_option(optConnectTimeoutOption);

			mysqlpp::ReadTimeoutOption * optReadTimeoutOption = new  mysqlpp::ReadTimeoutOption(30);
			pMyConn->set_option(optReadTimeoutOption);

			std::string strCharsetNameOption = m_szCharsetNameOption;
			mysqlpp::SetCharsetNameOption * optCharsetNameOption = new  mysqlpp::SetCharsetNameOption(strCharsetNameOption);
			pMyConn->set_option(optCharsetNameOption);

			pMyConn->set_option(new mysqlpp::ReconnectOption(false));
			bool bSuccess = false;
			try {
				if (m_uiPort == 3306)
				{
					bSuccess = pMyConn->connect(m_szDatabaseName, m_szServer, m_szUserName, m_szPassword);
				}
				else
				{
					bSuccess = pMyConn->connect(m_szDatabaseName, m_szServer, m_szUserName, m_szPassword, m_uiPort);
				}
			}
			catch (mysqlpp::Exception & ex)
			{
				//连接数据库失败，释放连接
				release(pMyConn);
				//sleep 1000，再连接
				Sleep(1000);
			}
		}
		return pMyConn;
	}
	void destroy(mysqlpp::Connection* cp) { delete cp; }

	int		threadPorcess(void *pParameter);

	char m_szDatabaseName[128];
	char m_szServer[64];
	char m_szUserName[64];
	char m_szPassword[64];
	unsigned int m_uiPort;
	char m_szCharsetNameOption[32];
	unsigned int m_nMaxConnectNum;
	unsigned int m_conns_in_use;
	unsigned int m_nMinRecordRows;

	boost::thread * m_pThreadObject;
	bool m_bThreadExit;
	std::vector<std::string> m_listBatchEventLogSql;	
	boost::shared_mutex m_mutexBatchSql;
	std::vector<DB_DeviceEventLogInfo> m_vecBatchEventLogSql;
	boost::shared_mutex m_mutexBatchEventLogSql;
};
```


//MySqlConnectionPool.cpp

```cpp
#include "MySqlConnectionPool.h"
#include "mysqlpp/include/mysql++.h"

int MySqlConnectionPool::startThrad(void *pParameter)
{
	try {
		mysqlpp::ScopedConnection cp(*this, true);
		if (!cp->thread_aware()) {
			//cerr << "MySQL++ wasn't built with thread awareness!  " << argv[0] << " can't run without it." << endl;
			return 1;
		}
	}
	catch (mysqlpp::Exception& e) {
		//cerr << "Failed to set up initial pooled connection: " << e.what() << endl;
		return 1;
	}

	m_bThreadExit = false;
	m_pThreadObject = new boost::thread(boost::bind(&MySqlConnectionPool::threadPorcess, this, this));

	return 0;
}

int	MySqlConnectionPool::stopThread()
{
	m_bThreadExit = true;
	if (m_pThreadObject != NULL)
	{
		m_pThreadObject->join();
		delete m_pThreadObject;
		m_pThreadObject = NULL;
	}

	return 0;
}
int	MySqlConnectionPool::threadPorcess(void *pParameter)
{
	unsigned int uiSleepTime = 3000, nn = 0;
	while (true)
	{
		if (m_bThreadExit)
		{
			break;
		}

		mysqlpp::Connection::thread_start();
		mysqlpp::ScopedConnection cp(*this, true);
		if (!cp || !cp->connected()) {
			//cerr << "Failed to get a connection from the pool!" << endl;
			mysqlpp::Connection::thread_end();
			Sleep(1000);
			continue;
		}

		//business proccess
		std::string strSql = "";
		{
			boost::unique_lock<boost::shared_mutex> locker(m_mutexBatchSql);
			if (m_listBatchEventLogSql.size() > 0)
			{
				int nCount = 1;
				for (int i = 0; i < nCount; i++)
				{
					strSql = m_listBatchEventLogSql[0];
					try {
						mysqlpp::Transaction trans(*cp);
						bool bSuccess = false;
						mysqlpp::Query query = cp->query();
						bool bExecute = query.exec(strSql.c_str());
						//mysqlpp::SimpleResult srSimpleResult = query.execute(strSql.c_str(), strSql.size());
						trans.commit();
						/*
						mysqlpp::Query query(cp->query("select * from t_deviceinfo"));
						mysqlpp::StoreQueryResult res = query.store();
						for (size_t j = 0; j < res.num_rows(); ++j)
						{
						}
						*/
					}
					catch (mysqlpp::Exception & ex) {
						//char szLog[1024] = { 0 };
						//sprintf(szLog, "General error:%s\n", ex.what());
						//OutputDebugString(szLog);
						//cerr << "General error: " << ex.what() << endl;
						break;
					}
					m_listBatchEventLogSql.erase(m_listBatchEventLogSql.begin());
				}
			}
		}
		strSql = "";
		{
			/*
			MySQL大批量数据插入
			MySQL官方网站查了查资料，发现MySQL支持在一条INSERT语句中插入多条记录，格式如下：
			INSERT table_name (column1, column2, ..., columnN)
			VALUES (rec1_val1, rec1_val2, ..., rec1_valN),
			(rec2_val1, rec2_val2, ..., rec2_valN),
			... ...
			(recM_val1, recM_val2, ..., recM_valN);
			*/
			boost::unique_lock<boost::shared_mutex> locker(m_mutexBatchEventLogSql);
			if (m_vecBatchEventLogSql.size() > 0)
			{
				int nCount = std::min(m_vecBatchEventLogSql.size(), m_nMinRecordRows);
				for (int i = 0; i < nCount; i++)
				{
					char szSql[1024] = { 0 };
					char szCurrentTime[64] = { 0 };
					sprintf_s(szCurrentTime, 63, "str_to_date('%s','%s')", m_vecBatchEventLogSql[i].szCurrentTime, "%Y-%m-%d %H:%i:%s");
					if (i == 0)
					{
						sprintf_s(szSql, 1023, "insert into t_eventlog(DeviceGuidID,EventType,SubEventType,Description,GpsEffect,Lat,Lng,CurrentTime) values ('%s',%d,%d,'%s',%d,%.7f,%.7f,%s)",
							m_vecBatchEventLogSql[i].szGuidId, m_vecBatchEventLogSql[i].uiEventType, m_vecBatchEventLogSql[i].uiSubEventType, m_vecBatchEventLogSql[i].szDescription,
							m_vecBatchEventLogSql[i].bGpsEffect, m_vecBatchEventLogSql[i].fLat, m_vecBatchEventLogSql[i].fLng, szCurrentTime);
						strSql = szSql;
					}
					else
					{
						sprintf_s(szSql, 1023, ",('%s',%d,%d,'%s',%d,%.7f,%.7f,%s)",
							m_vecBatchEventLogSql[i].szGuidId, m_vecBatchEventLogSql[i].uiEventType, m_vecBatchEventLogSql[i].uiSubEventType, m_vecBatchEventLogSql[i].szDescription,
							m_vecBatchEventLogSql[i].bGpsEffect, m_vecBatchEventLogSql[i].fLat, m_vecBatchEventLogSql[i].fLng, szCurrentTime);
						strSql += szSql;
					}
					if (i == nCount - 1)
					{
						strSql += ";";
					}
				}
				for (int i = 0; i < nCount; i++)
				{
					m_vecBatchEventLogSql.erase(m_vecBatchEventLogSql.begin());
				}
				try {
					mysqlpp::Transaction trans(*cp);
					bool bSuccess = false;
					mysqlpp::Query query = cp->query();
					bool bExecute = query.exec(strSql.c_str());
					trans.commit();
				}
				catch (mysqlpp::Exception & ex) {
					break;
				}
			}
		}
		/////////////////////////////////////////////////////////////////////////
		mysqlpp::Connection::thread_end();

		for (nn = 0; nn <= uiSleepTime; nn += 200)
		{
			if (m_bThreadExit)
			{
				break;
			}
			Sleep(200);
		}
	}

	return 0;
}

//插入批量语句
int MySqlConnectionPool::InsertBatchSql(char *pSql)
{
	boost::unique_lock<boost::shared_mutex> locker(m_mutexBatchSql);
	m_listBatchEventLogSql.push_back(pSql);

	return 0;
}

//插入事件日志
void MySqlConnectionPool::InsertBatchEventLog(DB_DeviceEventLogInfo *pEventLogInfo)
{
	boost::unique_lock<boost::shared_mutex> locker(m_mutexBatchEventLogSql);
	m_vecBatchEventLogSql.push_back(*pEventLogInfo);
}

//测试数据库连接
bool	MySqlConnectionPool::TestConnect(char *pDatabaseName, char *pServer, char *pUsernmae, char *pPassword, unsigned int uiPort)
{
	mysqlpp::Connection conn(false);
	mysqlpp::ConnectTimeoutOption * optConnectTimeoutOption = new  mysqlpp::ConnectTimeoutOption(10);
	conn.set_option(optConnectTimeoutOption);
	mysqlpp::ReadTimeoutOption * optReadTimeoutOption = new  mysqlpp::ReadTimeoutOption(10);
	conn.set_option(optReadTimeoutOption);
	bool bSuccess = false;
	try {
		if (uiPort == 3306)
		{
			bSuccess = conn.connect(pDatabaseName, pServer, pUsernmae, pPassword);
		}
		else
		{
			bSuccess = conn.connect(pDatabaseName, pServer, pUsernmae, pPassword, uiPort);
		}
		conn.disconnect();
	}
	catch (...)
	{
		bSuccess = false;
	}

	return bSuccess;
}
```

//DatabaseDataDefine.h

```cpp
//设备事件日志信息
struct DB_DeviceEventLogInfo {
	unsigned int uiID;
	char szGuidId[40];			//guid
	char szDeviceId[20];		//device id	
	unsigned int uiEventType;
	unsigned int uiSubEventType;
	char szDescription[256];	//描述
	unsigned int bGpsEffect;
	double fLat;
	double fLng;
	char szCurrentTime[27];
};
```

//MySqlOperateWrapper.h

```cpp
#pragma once

#include "mysql++.h"
#include "MySqlConnectionPool.h"
#include <string>
#include <list>

#ifdef _DEBUG
#pragma comment(lib,"Debug/mysqlpp_d.lib")
#else
#pragma comment(lib,"Release/mysqlpp.lib")
#endif

//MySqlOperateParameter
struct MySqlOperateParameter {
	char szDatabaseName[128];
	char szServer[64];
	char szUserName[64];
	char szPassword[64];
	unsigned int uiPort;
	char szCharsetNameOption[32];
	unsigned char bIsUseConnectionPool;
	unsigned char bBatchProcessByThread;
	unsigned char ucBatchInsertRowNumEveryOneTime;
	unsigned char nMaxConnectNum;
};

class MySqlOperateWrapper
{
public:
	MySqlOperateWrapper();
	virtual ~MySqlOperateWrapper();

	//初始化
	int Init(void *pParameter,int nParameterSize);

	//反初始化
	int UnInit();

	//从连接池中获取与释放连接
	mysqlpp::Connection* GetConnection();
	void ReleaseConnection(const mysqlpp::Connection* pc);

	//查询sql语句
	int ExecuteSQlQuery(mysqlpp::Connection * pConn, char *pSql, mysqlpp::UseQueryResult & useQueryResult);

	//执行sql语句
	int ExecuteSQl(mysqlpp::Connection * pConn, char *pSql, mysqlpp::SimpleResult & srSimpleResult);

	//插入批量语句
	int InsertBatchSql(char *pSql);

	//插入事件日志
	void InsertBatchEventLog(DB_DeviceEventLogInfo *pEventLogInfo);

protected:
	MySqlOperateParameter m_spParameter;
	bool m_bMysqlConnect;

	MySqlConnectionPool * m_pMySqlConnectionPool;
};
```

//MySqlOperateWrapper.cpp

```cpp
#include "stdafx.h"
#include "MySqlOperateWrapper.h"

MySqlOperateWrapper::MySqlOperateWrapper()
{
	memset(&m_spParameter, 0, sizeof(m_spParameter));
	m_bMysqlConnect = false;
	m_pMySqlConnectionPool = NULL;
}

MySqlOperateWrapper::~MySqlOperateWrapper()
{
}

//初始化
int MySqlOperateWrapper::Init(void *pParameter, int nParameterSize)
{
	memcpy(&m_spParameter, pParameter, nParameterSize);
	bool bSuccess = MySqlConnectionPool::TestConnect(m_spParameter.szDatabaseName, m_spParameter.szServer,
		m_spParameter.szUserName, m_spParameter.szPassword, m_spParameter.uiPort);
	if (!bSuccess)
	{
		//连接数据库失败
		return 1;
	}

	int nRet = 0;
	m_pMySqlConnectionPool = new MySqlConnectionPool(m_spParameter.szDatabaseName, m_spParameter.szServer,
		m_spParameter.szUserName, m_spParameter.szPassword, m_spParameter.uiPort, m_spParameter.szCharsetNameOption,
		m_spParameter.nMaxConnectNum,m_spParameter.ucBatchInsertRowNumEveryOneTime);
	if (m_spParameter.bBatchProcessByThread)
	{
		m_pMySqlConnectionPool->startThrad(this);
	}
	return nRet;
}

//反初始化
int MySqlOperateWrapper::UnInit()
{
	int nRet = 0;
	if (m_spParameter.bBatchProcessByThread)
	{
		if (m_pMySqlConnectionPool != NULL)
		{
			m_pMySqlConnectionPool->stopThread();
			m_pMySqlConnectionPool->shrink();
			delete m_pMySqlConnectionPool;
			m_pMySqlConnectionPool = NULL;
		}
	}

	return nRet;
}

//从连接池中获取与释放连接
mysqlpp::Connection* MySqlOperateWrapper::GetConnection()
{
	if (m_pMySqlConnectionPool == NULL)
		return NULL;

	return m_pMySqlConnectionPool->grab();
}
void MySqlOperateWrapper::ReleaseConnection(const mysqlpp::Connection* pc)
{
	if (m_pMySqlConnectionPool != NULL)
	{
		m_pMySqlConnectionPool->release(pc);
	}
}

//执行语句
int MySqlOperateWrapper::ExecuteSQlQuery(mysqlpp::Connection * pConn, char *pSql, mysqlpp::UseQueryResult & useQueryResult)
{
	if (!pConn)
		return -1;

	try {
		mysqlpp::Query query = pConn->query(pSql);
		mysqlpp::UseQueryResult res = query.use();
		useQueryResult = res;
		return 0;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		TRACE("Query error: %s\n", er.what());
		return -2;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		//cerr << "Conversion error: " << er.what() << endl <<"\tretrieved data size: " << er.retrieved <<", actual size: " << er.actual_size << endl;
		return -3;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		TRACE("Error: %s\n", er.what());
		return -4;
	}

	return -1;
}

//执行sql语句
int MySqlOperateWrapper::ExecuteSQl(mysqlpp::Connection * pConn, char *pSql, mysqlpp::SimpleResult & srSimpleResult)
{
	if (!pConn)
		return -1;

	try {
		mysqlpp::Query query = pConn->query();
		srSimpleResult = query.execute(pSql, strlen(pSql));
		return 0;
	}
	catch (const mysqlpp::BadQuery& er) {
		// Handle any query errors
		TRACE("Query error: %s\n", er.what());
		return -2;
	}
	catch (const mysqlpp::BadConversion& er) {
		// Handle bad conversions
		return -3;
	}
	catch (const mysqlpp::Exception& er) {
		// Catch-all for any other MySQL++ exceptions
		TRACE("Error: %s\n", er.what());
		return -4;
	}

	return -1;
}

//插入批量语句
int MySqlOperateWrapper::InsertBatchSql(char *pSql)
{
	if (m_spParameter.bBatchProcessByThread && m_pMySqlConnectionPool != NULL)
	{
		m_pMySqlConnectionPool->InsertBatchSql(pSql);
	}

	return 0;
}

//插入事件日志
void MySqlOperateWrapper::InsertBatchEventLog(DB_DeviceEventLogInfo *pEventLogInfo)
{
	if (m_spParameter.bBatchProcessByThread && m_pMySqlConnectionPool != NULL)
	{
		m_pMySqlConnectionPool->InsertBatchEventLog(pEventLogInfo);
	}
}
```

调用实例

```cpp
MySqlOperateWrapper mowMySqlOperateWrapper;
mowMySqlOperateWrapper.init();//初始化

//执行简单的插入/更新/删除sql语句
char szSql[1024] = {0};//sql语句
...//sql语句赋值		
mysqlpp::Connection * pConn = mowMySqlOperateWrapper.GetConnection();
if (!pConn)
		return false;
	mysqlpp::SimpleResult srSimpleResult;
	int nRetExecute = mowMySqlOperateWrapper.ExecuteSQl(pConn,szSql, srSimpleResult);
	if (nRetExecute == 0)
	{
		if (srSimpleResult)
		{
			dbDeviceEventLogInfo.uiID = srSimpleResult.insert_id();
		}

		bRet = true;
	}
	mowMySqlOperateWrapper.ReleaseConnection(pConn);

//执行一个查询sql语句
mysqlpp::Connection * pConn = mowMySqlOperateWrapper.GetConnection();
if (!pConn)
		return false;
		
char szSql[1024] = {0};//sql语句
...//sql语句赋值		
mysqlpp::UseQueryResult useQueryResult;
	int nRetExecute = mowMySqlOperateWrapper.ExecuteSQlQuery(pConn,szSql, useQueryResult);
	if (nRetExecute == 0)
	{
		bRet = true;
	}
	if (bRet)
	{
		DB_DeviceEventLogInfo deviceEventLogInfo = { 0 };
		if (useQueryResult)
		{
			while (mysqlpp::Row row = useQueryResult.fetch_row())
			{
				//获取数据
				deviceEventLogInfo.uiID = row["ID"];
				strcpy_s(deviceEventLogInfo.szGuidId, 40, row["DeviceGuidID"]);
				deviceEventLogInfo.uiEventType = row["EventType"];
				deviceEventLogInfo.uiSubEventType = row["SubEventType"];
				strcpy_s(deviceEventLogInfo.szDescription, row["Description"]);
				deviceEventLogInfo.bGpsEffect = row["GpsEffect"];
				deviceEventLogInfo.fLat = row["Lat"];
				deviceEventLogInfo.fLng = row["Lng"];
				strcpy_s(deviceEventLogInfo.szCurrentTime, 26, row["CurrentTime"]);
				strcpy_s(deviceEventLogInfo.szDeviceId, 19, row["DeviceId"]);
			}
		}
	}
	mowMySqlOperateWrapper.ReleaseConnection(pConn);		
	
	//插入批量事件日志记录
	DB_DeviceEventLogInfo deviceEventLogInfo = { 0 };
	...//赋值	
	mowMySqlOperateWrapper.InsertBatchEventLog(&deviceEventLogInfo);
	
	
	mowMySqlOperateWrapper.UnInit();//反初始化
```

