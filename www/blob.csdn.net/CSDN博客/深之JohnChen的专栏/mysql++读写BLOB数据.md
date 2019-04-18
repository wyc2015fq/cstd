# mysql++读写BLOB数据 - 深之JohnChen的专栏 - CSDN博客

2018年08月04日 18:12:14[byxdaz](https://me.csdn.net/byxdaz)阅读数：603


       通过SSQLS( Specialized SQL Structure)可以将一张表完全对应到一个C++结构体中，然后只要对于这个结构体进行操作，同时再加入到对应的mysqlpp:: Query对应的方法中，我们就可以执行对应的SQL语句。通过读写BLOB数据。

1、使用sql_create_n宏函数，将数据库表字段映射到一个对象数据结构中。

sql_create_2，sql_create_3，sql_create_4，sql_create_5，sql_create_6等等。

   #define sql_create_2(NAME, CMP, CONTR, T1, I1, T2, I2) \

        sql_create_complete_2(NAME, CMP, CONTR, T1, I1, #I1, T2, I2, #I2) \

         //NAME：表名；CMP：比较字段个数，从1开始此参数指定参与比较的字段个数；CONTR：构造初始化参数个数，需要初始化的参数个数，用0表示无需参数初始化；T1:字段类型；I1:字段名；T2:字段类型；I2:字段名

2、写数据

  1）获取Query

  2）inset 插入上面定义对象得实例

  3）execute执行

3、读数据

  1）获取Query

  2）执行sql，query.store()

  3）获取数据，将数据赋值到对象实例中。

        mysqlpp::Query query = pConn->query();

        query << szSql;

        mysqlpp::StoreQueryResult res = query.store();

        t_electronicWaybill waybill = res[0];        //查询一条记录

完整代码

```
#include <mysql++.h>
#include <ssqls.h>

sql_create_5(t_electronicWaybill,
	1,2,
	mysqlpp::sql_int, id,
	mysqlpp::sql_int_unsigned, device_id,
	mysqlpp::sql_blob_null, waybill_data,
	mysqlpp::sql_int, waybill_datasize,
	mysqlpp::sql_datetime, currenttime)
	
//插入运单
bool InsertElectronicWaybill(unsigned int nDeviceId, char *pData, int nDataSize, char *pCurrentTime)
{
	bool bSuccess = false;
	mysqlpp::Connection* pConn = GetConnection();		//从连接池中获取一个连接
	if (pConn == NULL)
	{
		return false;
	}
	t_electronicWaybill waybill;
	waybill.waybill_data.data.assign(pData,nDataSize);
	waybill.waybill_datasize = nDataSize;
	waybill.device_id = nDeviceId;
	waybill.id = 0;
	int nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMinute = 0, nSecond = 0;
	try {
		sscanf(pCurrentTime, "%d-%d-%d %d:%d:%d",&nYear,&nMonth,&nDay,&nHour,&nMinute,&nSecond);
	}
	catch(...){
	}
	mysqlpp::DateTime *pDateTimeTemp = new mysqlpp::DateTime(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	waybill.currenttime = *pDateTimeTemp;
	mysqlpp::Query query = pConn->query();
	query.insert(waybill);
	mysqlpp::SimpleResult res;
	try {
		res = query.execute();
		bSuccess = true;
	}
	catch (mysqlpp::Exception & ex)
	{
		OutputDebugString(ex.what());
	}
	if (pDateTimeTemp != NULL)
	{
		delete pDateTimeTemp;
	}
	ReleaseConnection(pConn);						//释放连接
	return bSuccess;
}

//查询运单
bool QueryElectronicWaybill(unsigned int uiRecordId, unsigned int & nDeviceId, char *pData, int & nDataSize, char *pCurrentTime)
{
	bool bSuccess = false;
	mysqlpp::Connection* pConn = GetConnection();		//从连接池中获取一个连接
	if (pConn == NULL)
	{
		return false;
	}
	char szSql[256] = { 0 };
	sprintf_s(szSql, "select * from t_electronicWaybill where id = %u", uiRecordId);
	try {
		mysqlpp::Query query = pConn->query();
		query << szSql;
		mysqlpp::StoreQueryResult res = query.store();
        //读取数据方式1：vector方式，res[] 
		t_electronicWaybill waybill = res[0];
        //读取数据方式2：使用迭代器方式
/*
mysqlpp::Query query = con.query(szSql);
vector<t_electronicWaybill> res;
query.storein(res);
vector<t_electronicWaybill>::iterator it = res.begin();
*/
		memcpy(pData, waybill.waybill_data.data, waybill.waybill_data.data.length());
		nDataSize = waybill.waybill_datasize;
		nDeviceId = waybill.device_id;
		if (pCurrentTime != NULL)
		{
			sprintf_s(pCurrentTime, 23, "%04d-%02d-%02d %02d:%02d:%02d", waybill.currenttime.year(), waybill.currenttime.month(),
				waybill.currenttime.day(), waybill.currenttime.hour(), waybill.currenttime.minute(), waybill.currenttime.second());
		}
	}
	catch (mysqlpp::Exception & ex)
	{
		OutputDebugString(ex.what());
	}
	ReleaseConnection(pConn);		//释放连接
	return bSuccess;
}
```

Navicat for MySQL 查看BLOB字段内容

https://blog.csdn.net/lwei_998/article/details/41871329

