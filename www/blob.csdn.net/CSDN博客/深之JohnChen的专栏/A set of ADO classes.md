# A set of ADO classes - 深之JohnChen的专栏 - CSDN博客

2008年06月19日 00:19:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3716标签：[function																[parameters																[constraints																[string																[reference																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=constraints&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)


A set of ADO classes - version 2.20
源出
[http://www.codeproject.com/KB/database/caaadoclass1.aspx](http://www.codeproject.com/KB/database/caaadoclass1.aspx)
- [Download source version 2.09 - 10.6 Kb](http://www.codeproject.com/KB/database/CAAAdoClass1/AdoClass209_src.zip)
- [Download source version 2.20 - 14.4 Kb](http://www.codeproject.com/KB/database/CAAAdoClass1/AdoClass220_src.zip)
- [Download MS Data Control Bind sample - 36.2 Kb](http://www.codeproject.com/KB/database/CAAAdoClass1/DataBindingSample.zip)

## [Overview](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Overview)
- **[The CADODatabase Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheAdoClasses1)**- [CADODatabase](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADODatabase)
- [Open](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OpenDatabase)
- [Execute](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Execute)
- [GetRecordsAffected](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetDatabaseRecordsAffected)
- [GetActiveConnection](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetActiveConnection)
- [GetRecordCount](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetRecordCount)
- [BeginTransaction](http://www.codeproject.com/KB/database/caaadoclass1.aspx#BeginTrans)
- [CommitTransaction](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CommitTrans)
- [RollbackTransaction](http://www.codeproject.com/KB/database/caaadoclass1.aspx#RollbackTrans)
- [IsOpen](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsOpenDatabase)
- [Close](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CloseDatabase)
- [SetConnectionString](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetConnStr)
- [GetConnectionString](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetConnStr)
- [SetConnectionMode](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetConnMode)
- [GetLastError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetLastErrorDB)
- [GetLastErrorString](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetLastErrorStringDB)

- **[The CADORecordset Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheAdoClasses2)**- [CADORecordset](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADORecordset)
- [Open](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OpenRecordset)
- [Execute](http://www.codeproject.com/KB/database/caaadoclass1.aspx#ExecuteRecordset)
- [GetQuery](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetQuery)
- [SetQuery](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetQuery)
- [RecordBinding](http://www.codeproject.com/KB/database/caaadoclass1.aspx#RecordBinding)
- [GetRecordCount](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetRecordCount)
- [IsOpen](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsOpenRecordset)
- [Close](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CloseRecordset)
- [GetFieldCount](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetFieldCount)
- [GetFieldValue](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetFieldValue)
- [IsFieldNull](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsFieldNull)
- [IsFieldEmpty](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsFieldEmpty)
- [IsEof](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsEof)
- [IsBof](http://www.codeproject.com/KB/database/caaadoclass1.aspx#IsBof)
- [Move](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Move)
- [AbsolutePage](http://www.codeproject.com/KB/database/caaadoclass1.aspx#AbsolutePage)
- [GetPageCount](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetPageCount)
- [PageSize](http://www.codeproject.com/KB/database/caaadoclass1.aspx#PageSize)
- [AbsolutePosition](http://www.codeproject.com/KB/database/caaadoclass1.aspx#AbsolutePosition)
- [GetFieldInfo](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetFieldInfo)
- [GetChunk](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetChunk)
- [AppendChunk](http://www.codeproject.com/KB/database/caaadoclass1.aspx#AppendChunk)
- [GetString](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetString)
- [GetLastError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetLastErrorRS)
- [GetLastErrorString](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetLastErrorStringRS)
- [AddNew](http://www.codeproject.com/KB/database/caaadoclass1.aspx#AddNew)
- [Edit](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Edit)
- [Delete](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Delete)
- [Update](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Update)
- [CancelUpdate](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CancelUpdate)
- [SetFieldValue](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetFieldValue)
- [Find](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Find)
- [FindFirst](http://www.codeproject.com/KB/database/caaadoclass1.aspx#FindFirst)
- [FindNext](http://www.codeproject.com/KB/database/caaadoclass1.aspx#FindNext)
- [GetBookmark](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetBookmark)
- [SetBookmark](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetBookmark)
- [SetFilter](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetFilter)
- [SetSort](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetSort)
- [GetRecordset](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetRecordset)
- [GetActiveConnection](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetRecordsetActiveConnection)
- [Clone](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Clone)
- [SaveAsXML](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SaveAsXML)
- [OpenXML](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OpenXML)
- [OpenSchema](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OpenSchema)
- [Requery](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Requery)

- **[The CADOParameter Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheAdoClasses3)**- [CADOParameter](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADOParameter)
- [SetPrecision](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetPrecision)
- [SetScale](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetScale)
- [SetValue](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetParameterValue)
- [GetValue](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetParameterValue)
- [SetName](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetParameterName)
- [GetName](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetParameterName)
- [GetType](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetParameterType)
- [GetParameter](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetParameter)

- **[The CADOCommand Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheAdoClasses4)**- [CADOCommand](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADOCommand)
- [AddParameter](http://www.codeproject.com/KB/database/caaadoclass1.aspx#AddParameter)
- [SetText](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetCommandText)
- [GetText](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetCommandText)
- [SetType](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetCommandType)
- [GetType](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetCommandType)
- [GetCommand](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetCommand)
- [Execute](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CommandExecute)
- [GetRecordsAffected](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetCommandRecordsAffected)

- **[The CJectEngine Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheJetEngineClass)**- [CompactDatabase](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CompactDatabase)

- **[The CADOException Class](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheAdoClasses5)**- [CADOException](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADOException)
- [GetErrorMessage](http://www.codeproject.com/KB/database/caaadoclass1.aspx#ExceptionGetErrorMessage)
- [GetError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#ExceptionGetError)
- [SetError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#ExceptionSetError)

- **[COLEDBDataLink](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OLEDBDataLinkClass)**
- **[COLEDBDataLink](http://www.codeproject.com/KB/database/caaadoclass1.aspx#COLEDBDataLink)**
**[New](http://www.codeproject.com/KB/database/caaadoclass1.aspx#DataLinkNew)[Edit](http://www.codeproject.com/KB/database/caaadoclass1.aspx#DataLinkEdit)****[COLEDBException](http://www.codeproject.com/KB/database/caaadoclass1.aspx#COLEDBException)****[GetErrorMessage](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OleDbExceptionGetErrorMessage)****[GetError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OleDbExceptionGetError)[SetError](http://www.codeproject.com/KB/database/caaadoclass1.aspx#OleDbExceptionSetError)****[Common Questions](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CommonQuestions)**
## **Overview**

I created these classes to make it easy to work with ADO. For this I created the `CADODatabse` class and the `CADORecordset` class.

### **The CADODatabase Class**

The `CADODatabase` class has a set of functions that corresponds to the `_ConnectionPtr`.

**CADODatabase::CADODatabase**

Creates a `CADODatabase` object.
CADODatabase();
**CADODatabase::Open**

The `Open` function opens a connection to a database.
BOOL Open(LPCTSTR lpstrConnection = _T(""));
throw(CADOException);
**Parameters**

`LPCTSTR lpstrConnection` - The connection string.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**Note:** the class has the function `SetConnectionString`. You can insert the connection string through this function. In this case you can do the following:

![](http://www.codeproject.com/images/minus.gif) Collapse
//Sample with Connection string for SQL Server

CADODatabase* pAdoDb = new CADODatabase();
CString strConnection = _T("");

strConnection = _T("Provider=MSDASQL;PersistSecurityInfo=False;""Trusted_Connection=Yes;""Data Source=Access Sql Server;catalog=sampledb");
pAdoDb->SetConnectionString(strConnection);

if(pAdoDb->Open())
  DoSomething();
.
.
.

//Sample with Connection String for Access database

CADODatabase* pAdoDb = new CADODatabase();
CString strConnection = _T("");

strConnection = _T("Provider=Microsoft.Jet.OLEDB.4.0;""Data Source=C://VCProjects//ADO//Test//dbTest.mdb");
pAdoDb->SetConnectionString(strConnection);

if(pAdoDb->Open())
{
  DoSomething();
  .
  .
  .
  pAdoDb->Close();
}

delete pAdoDb;
**CADODatabase::Execute**

The `Execute` function executes a SQL statement in the open database.
BOOL Execute(LPCTSTR lpstrExec);
throw(CADOException);
**Parameters**

`LPCTSTR lpstrExec` - A string pointer containing the SQL statement to execute.
CADODatabase* pAdoDb = new CADODatabase();
CString strConnection = _T("");

strConnection = _T("Provider=MSDASQL;""PersistSecurityInfo=False;""Trusted_Connection=Yes""Data Source=Access Sql Server;catalog=sampledb");

if(pAdoDb->Open(strConnection))
 pAdoDb->Execute("Delete From tblClients Where Cheker = 3");
**Return Value**

The function returns `TRUE` if it was executed successfully.

**CADODatabase::GetRecordsAffected**

The `GetRecordsAffcted` function returns the number of records affected by the last SQL statement executed.
int GetRecordsAffected();
**CADODatabase::GetActiveConnection**

The `GetActiveConnection` returns the active connection.
_ConnectionPtr GetActiveConnection();
**CADODatabase::GetRecordCount**

`GetRecordCount` returns the number of records affected in a query.
DWORD GetRecordCount(_RecordsetPtr m_pRs);
**Parameters**

`_RecorsetPtr m_Prs` - The recordset.

**CADODatabase::BeginTransaction**

Call this function to initiate a transaction. After you call `BeginTransaction`, updates you make to your data take effect when you commit the transaction.
long BeginTransaction();
**CADODatabase::CommitTransaction**

Call `CommitTransaction` function to commit a transaction, for example, save a group of edits and updates to one or more databases.
long CommitTransaction();
**CADODatabase::RollbackTransaction**

Call `RollbackTransaction` function to end the current transaction and restore all databases to their condition before the transaction was begun.
long RollbackTransaction();
**CADODatabase::IsOpen**

The `IsOpen` function returns the status of the connection with the database.
BOOLIsOpen();
**Return Value**

The function returns `TRUE` if the connection to the database is open.

**CADODatabase::Close**

The `Close` function closes the connection to the database.
void Close();
**CADODatabase::SetConnectionString**

With the `SetConnectionString`, you can insert the connection string.
void SetConnectionString(LPCTSTR lpstrConnection);
**Parameters**

`LPCTSTR lpstrConnection` - A connection string used for opening the database.

See [CADODatabase::Open();](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Open1)

**CADODatabase::GetConnectionString**

The `GetConnectionString` function returns the connection string used for making a connection with a database.
CString GetConnectionString();
**CADODatabase::SetConnectionMode**

The `SetConnectionMode` function sets the mode of connection.
void SetConnectionMode(cadoConnectModeEnum nMode);
**Parameters**

`cadoConnectModeEnum nMode` - a variable that defines the connection mode to be used. It can be one of the following:
- `CADODatabase::connectModeUnknown`
- `CADODatabase::connectModeRead`
- `CADODatabase::connectModeWrite`
- `CADODatabase::connectModeReadWrite`
- `CADODatabase::connectModeShareDenyRead`
- `CADODatabase::connectModeShareDenyWrite`
- `CADODatabase::connectModeShareExclusive`
- `CADODatabase::connectModeShareDenyNone`

**CADODatabase::GetLastError**

The `GetLastError` function returns the last error code.
DWORD GetLastError();
**CADODatabase::GetLastErrorString**

The `GetLastErrorString` function returns the last error string.
CString GetLastErrorString();
### **The CADORecordset Class**

The `CADORecordset` class has a set of functions that corresponds to the `_RecordsetPtr`.

**CADORecordset::CADORecordset**

Creates a `CADORecordset` object.
CADODatabase();
CADORecordset(CADODatabase* pAdoDatabase);
**Parameters**

`CADODatabase* pAdoDatabase` - A `CADODatabase` object pointer.

**CADORecordset::Open**

The `Open` function opens a recordset.
BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), 
                     int nOption = CADORecordset::openUnknown);
BOOL Open(LPCTSTR lpstrExec = _T(""), 
                     int nOption = CADORecordset::openUnknown);
**Parameters**
- `_ConnectionPtr mpdb` - A connection pointer. 
- `LPCTSTR lpstrExec` - A string pointer containing an **SQL**`SELECT` statement. 
- `int nOption` - An integer that defines the access mode. The values are as follows:
    - `CADORecordset::openUnknown`
- `CADORecordset::openQuery`
- `CADORecordset::openTable`
- `CADORecordset::openStoredProc`

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::Execute**

The `Execute` function opens a recordset.
BOOL Execute(CADOCommand* pCommand);
throw(CADOException);
**Parameters**

`CADOCommand* pCommand` - A `CADOCommand` pointer.

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADORecordset::GetQuery;**

`GetQuery` returns the string containing the **SQL**`SELECT` statement.
CString GetQuery();
**CADORecordset::SetQuery**
void SetQuery(LPCSTR strQuery);
**Parameters**

`LPCTSTR strQuery` - A string pointer containing an **SQL**`SELECT` statement.

**CADORecordset::RecordBinding**
BOOL RecordBinding(CADORecordBinding pAdoRecordBinding);
**Parameters**

`CADORecordBinding pAdoRecordBinding`

**CADORecordset::GetRecordCount**

`GetRecordCount` returns the number of records accessed in the recordset.
DWORD GetRecordCount();
[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::IsOpen**

`IsOpen` determines if the recordset is open.
BOOL IsOpen();
**Return Value**

This member function returns `TRUE` if the recordset has not been closed.

**CADORecordset::Close**

The `Close` function closes the recordset.
void Close();
[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::GetFieldCount**

The `GetFieldCount` function returns the number of fields in the recordset.
long GetFieldCount();
**CADORecordset::GetFieldValue**

The `GetFieldValue` function returns a value that contains the value of a field.
BOOL GetFieldValue(LPCTSTR lpFieldName, int nValue);
BOOL GetFieldValue(int nIndex, int nValue);
BOOL GetFieldValue(LPCTSTR lpFieldName, long lValue);
BOOL GetFieldValue(int nIndex, long lValue);
BOOL GetFieldValue(LPCTSTR lpFieldName, unsignedlong ulValue);
BOOL GetFieldValue(int nIndex, unsignedlong ulValue);
BOOL GetFieldValue(LPCTSTR lpFieldName, double dbValue);
BOOL GetFieldValue(int nIndex, double dbValue);
BOOL GetFieldValue(LPCTSTR lpFieldName, CString strValue, 
                         CString strDateFormat = _T(""));
BOOL GetFieldValue(int nIndex, CString strValue, 
                         CString strDateFormat = _T(""));
BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime time);
BOOL GetFieldValue(int nIndex, COleDateTime time);
BOOL GetFieldValue(LPCTSTR lpFieldName, bool bValue);
BOOL GetFieldValue(int nIndex, bool bValue);
BOOL GetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue);
BOOL GetFieldValue(int nIndex, COleCurrency cyValue);
throw(CADOException);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset’s `Fields` collection, for lookup by index. 
- `double dbValue` - A reference to an object that will store the value of a field. 
- `long lValue` - A reference to an object that will store the value of a field. 
- `unsigned long ulValue` - A reference to an object that will store the value of a field. 
- `int nValue` - A reference to an object that will store the value of a field. 
- `CString strValue` - A reference to an object that will store the value of a field. 
- `CString strDateFormat` - A formatting time string similar to the `strftime` formatting string. The more common are:
    - **%a** - Abbreviated weekday name 
- **%A** - Full weekday name 
- **%b** - Abbreviated month name 
- **%B** - Full month name 
- **%c** - Date and time representation appropriate for locale 
- **%d** - Day of month as decimal number (01 - 31) 
- **%H** - Hour in 24-hour format (00 - 23) 
- **%I** - Hour in 12-hour format (01 - 12) 
- **%j** - Day of year as decimal number (001 - 366) 
- **%m** - Month as decimal number (01 - 12) 
- **%M** - Minute as decimal number (00 - 59) 
- **%p** - Current locale’s A.M./P.M. indicator for 12-hour clock 
- **%S** - Second as decimal number (00 - 59) 
- **%U** - Week of year as decimal number, with Sunday as first day of week (00 - 53) 
- **%w** - Weekday as decimal number (0 - 6; Sunday is 0) 
- **%W** - Week of year as decimal number, with Monday as first day of week (00 - 53) 
- **%x** - Date representation for current locale 
- **%X** - Time representation for current locale 
- **%y** - Year without century, as decimal number (00 - 99) 
- **%Y** - Year with century, as decimal number 

- `COleDateTime time` - A reference to an object that will store the value of a field. 
- `bool bValue` - A reference to an object that will store the value of a field. 
- `COleCurrency cyValue` - A reference to an object that will store the value of a field. 

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::IsFieldNull**

The `IsFieldNull` function determines if the field data is null.
BOOL IsFieldNull(LPCTSTR lpFieldName);
BOOL IsFieldNull(int nIndex);
throw(CADOException);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset’s `Fields` collection, for lookup by index. 

**Return Value**

This function returns `TRUE` if the field data is null.

**CADORecordset::IsFieldEmpty**

The `IsFieldEmpty` function determines if the field data is empty.
BOOL IsFieldEmpty(LPCTSTR lpFieldName);
BOOL IsFieldEmpty(int nIndex);
throw(CADOException);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset’s `Fields` collection, for lookup by index. 

**Return Value**

This function returns `TRUE` if the field data is empty.

**CADORecordset::IsEof**
BOOL IsEof();
**Return Value**

This function returns `TRUE` if the current position contains no records.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::IsBof**
BOOL IsBof();
**Return Value**

This function returns `TRUE` if the current position is the bottom of the recordset.

**CADORecordset::MoveFirstCADORecordset::MoveNextCADORecordset::MovePreviousCADORecordset::MoveLast**

These functions make the first/next/previous/or last record of the recordset as the current record.
void MoveFirst();
void MoveNext();
void MovePrevious();
void MoveLast();
[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample01)

**CADORecordset::GetAbsolutePageCADORecordset::SetAbsolutePage**

Indicates on which page the current record resides.
long GetAbsolutePage();
void SetAbsolutePage(int nPage);
**Parameters**

`int nPage` - The number of the page starting from 1.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample05)

**CADORecordset::GetPageCount**

`GetPageCount` returns the number of pages in the recordset.
long GetPageCount();
[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample05)

**CADORecordset::GetPageSizeCADORecordset::SetPageSize**

Indicates the number of records per page.
long GetPageSize();
void SetPageSize(int nSize);
**Parameters**

`int nSize` - set the number of records per page.

**For example**
CADORecordset* pRs = new CADORecordset(pDb);

if(pRs->Open("MyBigTable", CADORecordset::openTable))
{
    pRs->SetPageSize(5);
    for(registerint nPageIndex = 1; nPageIndex <= pRs->GetPageCount(); 
        nPageIndex++)
    {
        pRs->SetAbsolutePage(nPageIndex);
        for(registerint nRecNumber = 0; nRecNumber < pRs->GetPageSize(); 
            nRecNumber++)
        {
            long lVal;
            pRs->GetFieldValue("ID", lVal);
            pRs->MoveNext();

            if(pRs->IsEof())
                break;
        }
    }
    pRs->Close();
}

delete pRs;
**CADORecordset::GetAbsolutePositionCADORecordset::SetAbsolutePosition**

Indicates the position of the record in the recordset.
long GetAbsolutePosition();
void SetAbsolutePosition(int nPosition);
**Parameters**

`int nPosition` - Move to the position in the recordset.

`GetAbsolutePosition()` can return the position of the record or one of the following values:
- `CADORecordset::positionUnknown`
- `CADORecordset::positionBOF`
- `CADORecordset::positionEOF`

**CADORecordset::GetFieldInfo**

`GetFieldInfo` returns the attributes of a field.
BOOL GetFieldInfo(LPCTSTR lpFieldName, CAdoFieldInfo* fldInfo);
BOOL GetFieldInfo(int nIndex, CAdoFieldInfo* fldInfo);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset's `Fields` collection, for lookup by index. 
- `CAdoFieldInfo* fldInfo` - A struct that returns the field attributes. 
struct CADOFieldInfo
{
    char m_strName[30];
    short m_nType;
    long m_lSize;
    long m_lDefinedSize;
    long m_lAttributes;
    short m_nOrdinalPosition;
    BOOL m_bRequired;
    BOOL m_bAllowZeroLength;
    long m_lCollatingOrder;
};
The element `m_nType` of the class `CADOFieldInfo` can be one of the following values:
- `CADORecordset::typeEmpty`
- `CADORecordset::typeTinyInt`
- `CADORecordset::typeSmallInt`
- `CADORecordset::typeInteger`
- `CADORecordset::typeBigInt`
- `CADORecordset::typeUnsignedTinyInt`
- `CADORecordset::typeUnsignedSmallInt`
- `CADORecordset::typeUnsignedInt`
- `CADORecordset::typeUnsignedBigInt`
- `CADORecordset::typeSingle`
- `CADORecordset::typeDouble`
- `CADORecordset::typeCurrency`
- `CADORecordset::typeDecimal`
- `CADORecordset::typeNumeric`
- `CADORecordset::typeBoolean`
- `CADORecordset::typeError`
- `CADORecordset::typeUserDefined`
- `CADORecordset::typeVariant`
- `CADORecordset::typeIDispatch`
- `CADORecordset::typeIUnknown`
- `CADORecordset::typeGUID`
- `CADORecordset::typeDate`
- `CADORecordset::typeDBDate`
- `CADORecordset::typeDBTime`
- `CADORecordset::typeDBTimeStamp`
- `CADORecordset::typeBSTR`
- `CADORecordset::typeChar`
- `CADORecordset::typeVarChar`
- `CADORecordset::typeLongVarChar`
- `CADORecordset::typeWChar`
- `CADORecordset::typeVarWChar`
- `CADORecordset::typeLongVarWChar`
- `CADORecordset::typeBinary`
- `CADORecordset::typeVarBinary`
- `CADORecordset::typeLongVarBinary`
- `CADORecordset::typeChapter`
- `CADORecordset::typeFileTime`
- `CADORecordset::typePropVariant`
- `CADORecordset::typeVarNumeric`
- `CADORecordset::typeArray`

**For example**
if(prs->Open("Clients", CADORecordset::openTable))
{
    CADOFieldInfo pInfo;

    prs->GetFieldInfo("Description", &pInfo);

    if(pInfo.m_nType == CADORecordset::typeVarChar)
        AfxMessageBox("The type Description Field Is VarChar");
}

if(prs->Open("TestTable", CADORecordset::openTable))
{
    CADOFieldInfo* fInfo = new CADOFieldInfo;

    prs.GetFieldInfo(0, fInfo);
    CString strFieldName = fInfo->m_strName;
    prs->Close();
}
**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::GetChunk**

This function returns all, or a portion, of the contents of a large text or binary data `Field` object.
BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
BOOL GetChunk(int nIndex, CString& strValue);
BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData);
BOOL GetChunk(int nIndex, LPVOID pData);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset's `Fields` collection, for lookup by index. 
- `CString& strValue` - A string pointer that contains the data that returns from the object. 
- `LPVOID pData` - A pointer that contains the data that returns from the object. 

**Return Value**

Returns `TRUE` if it was executd successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample04)

**CADORecordset::AppendChunk**

This function appends data to a large text or binary data field.
BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes);
BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes);
throw(CADOException);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset's `Fields` collection, for lookup by index. 
- `LPVOID lpData` - A pointer that contains the data to append to the object. 
- `UINT nBytes` - A `UINT` that indicates the size of the data to be inserted. 

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
**//Sample of AppendChunckprs.AddNew();prs.SetFieldValue("ID", 5);prs.SetFieldValue("Description", "Client 05");prs.SetFieldValue("Checker", 1);prs.AppendChunk("Document",     "This Document is the story of Bob and his Friends...", 37);prs.Update();**//Sample of GetChunckchar data[1024];prs.GetChunk("Document", (LPVOID)&data);****
**CADORecordset::GetString**

This function returns a recordset as a string.
CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, 
                  long numRows = 0);
**Parameters**
- `LPCTSTR lpCols` - A columns delimiter. 
- `LPCTSTR lpRows` - A rows delimiter. 
- `LPCTSTR lpNull` - An expression that represents a null value. 
- `long numRows` - The number of rows affected. 

**CADORecordset::GetLastError**

The `GetLastError` function returns the last error code.
DWORD GetLastError();
**CADORecordset::GetLastErrorString**

The `GetLastErrorString` function returns the last error string.
CString GetLastErrorString();
**CADORecordset::AddNew**

The `AddNew` function adds a record in the open recordset.
BOOL AddNew();
throw(CADOException);
**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample01)

**CADORecordset::Edit**

The `Edit` function allows changes to the current record in the open recordset.
void Edit();
**CADORecordset::Delete**

The `Delete` function deletes the current record in the open recordset.
BOOL Delete();
**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::Update**

The `Update` function updates the pending updates in the current record.
BOOL Update();
throw(CADOException);
**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample01)

**CADORecordset::CancelUpdate**

The `CancelUpdate` function cancels any pending update in the open recordset.
void CancelUpdate();
**CADORecordset::SetFieldValue**

The `SetFieldValue` function sets the value of a field.
BOOL SetFieldValue(int nIndex, int nValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
BOOL SetFieldValue(int nIndex, long lValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
BOOL SetFieldValue(int nIndex, unsignedlong ulValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, unsignedlong ulValue);
BOOL SetFieldValue(int nIndex, double dblValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
BOOL SetFieldValue(int nIndex, CString strValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
BOOL SetFieldValue(int nIndex, COleDateTime time);
BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);
BOOL SetFieldValue(int nIndex, bool bValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, bool bValue);
BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
BOOL SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue);
**Parameters**
- `LPCTSTR lpFieldName` - A pointer to a string that contains the name of a field. 
- `int nIndex` - A zero-based index of the field in the recordset’s `Fields` collection, for lookup by index. 
- `int nValue` - A pointer to an object containing the value of the field. 
- `long lValue` - A pointer to an object containing the value of the field. 
- `unsigned long lValue` - A pointer to an object containing the value of the field. 
- `double dbValue` - A pointer to an object containing the value of the field. 
- `CString strValue` - A pointer to an object containing the value of the field. 
- `COleDateTime time` - A pointer to an object containing the value of the field. 
- `bool bValue` - A pointer to an object containing the value of the field. 
- `COleCurrency cyValue` - A pointer to an object containing the value of the field. 

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample01)

**For example**
CADORecordset* prs = new CADORecordset(m_pAdoDb);
prs->Open("Test", openTable);

prs->AddNew();
prs->SetFieldValue(0, "dataU");
prs->SetFieldValue(1, "data01");
prs->SetFieldValue(2, (long)51000);
COleDateTime time = COleDateTime(2001,6,15, 10, 8, 30);
prs->SetFieldValue(3, time);
prs->Update();
prs->Close();

delete prs;
**CADORecordset::Find**

The `Find` function locates a string from the current position in the open recordset using an operator of comparison.
BOOL Find(LPCTSTR lpFind, 
          int nSearchDirection = CADORecordset::searchForward);
**Parameters**
- `LPCTSTR lpFind` - A string expression used to locate the record. 
- `int nSearchDirection` - A value that indicates the type of operation. The possible values are:
    - `CADORecordset::searchForward` - Find the next location. 
- `CADORecordset::searchBackward` - Find the previous location. 

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
if(prs.Find("Field0 LIKE 'dataU%'"))
{
  prs.Delete();
  while(prs.FindNext())
    prs.Delete();
}
**CADORecordset::FindFirst**

The `FindFirst` function locates a string from the beginning in the open recordset using an operator of comparison.
BOOL FindFirst(LPCTSTR lpFind);
**Parameters**

`LPCTSTR lpFind` - A string expression used to locate the record.

**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::FindNext**

The `FindNext` function locates a string from the last position in the open recordset using the operator of comparison used in `FindFirst` or **`Find`** functions.
BOOL FindNext();
**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample02)

**CADORecordset::GetBookmark**

The `GetBookmark` function saves the position of the current record.
BOOL GetBookmark();
**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::SetBookmark**

The `SetBookmark` function returns to the position saved at any time.
void SetBookmark();
**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::SetFilter**

The `SetFilter` indicates a filter for data in an open `Recordset`.
BOOL SetFilter(LPCTSTR strFilter);
throw(CADOException);
**Parameters**

`LPCTSTR strFilter` - a string composed by one or more individual clauses concatenated with **AND** or **OR** operators.

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
CADORecordset* pRs = new CADORecordset(pDb);

if(pRs->Open("tblTest", CADORecordset::openTable))
{
    CString strFilter = _T("LastName = 'Smith' And Age > 30");
    pRs->SetFilter(strFilter);
    .
    .
    pRs->Close();
}

delete pRs
**CADORecordset::SetSort**

The `SetSort` function sets the sort order for records in a `CADORecordset` object.
BOOL SetSort(LPCTSTR lpstrCriteria);
throw(CADOException);
**Parameters**

`LPCTSTR lpstrCriteria` - A string that contains the `ORDER BY` clause of an SQL statement.

**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::GetRecordset**

The `GetRecordset` function returns a pointer to an open recordset.
_RecordsetPtr GetRecordset();
**CADORecordset::GetActiveConnection**

The `GetActiveConnection` returns the active connection.
_ConnectionPtr GetActiveConnection();
**CADORecordset::Clone**

The `Clone` function creates a duplicate `CADORecordset` object from an existing `CADORecordset` object.
BOOL Clone(CADORecordset pAdoRecordset);
throw(CADOException);
**Parameters**

`CADORecordset pAdoRecordset` is an existing `CADORecordset` object.

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
CADORecordset m_pRs; //Original Recordset
CADORecordset RS;   //Duplicate Recordset
CADORecordset* pRs = CADORecordset(m_pDb);

if(pRs->Open("tblTest", CADORecordset::openTable)) //Open the Original 
Recordset
{
    pRs->Clone(RS); //Create the clone of the original Recordset
long lClonRecs = RS.GetRecordCount();
    long lOrigRecs = m_pRs->GetRecordCount();
    .
    .
    RS.Close();
    pRs->Close();
}

delete pRs;
**CADORecordset::SaveAsXML**

The `SaveAsXML` function saves the open recordset in a file with XML format.
BOOL SaveAsXML(LPCTSTR lpstrXMLFile);
**Parameters**

`LPCTSTR strXMLFile` - a string that indicates the complete path name of the file where the recordset is to be saved.

**Return Value**

Returns `TRUE` if it was executed successfully.

**CADORecordset::OpenXML**

The `OpenXML` function opens a XML file format in a recordset.
BOOL OpenXML(LPCTSTR lpstrXMLFile);
**Parameters**

`LPCTSTR strXMLFile` - a string that indicates the complete path name of the XML file to be opened.

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
CADORecordset* pRs = CADORecordset(pDB);

if(prs->OpenXML("C://My.XML"))
{
    CString strClient = _T("");
    double dblVal = 0;
    long lrecs = prs->GetRecordCount();

    if(!prs->IsEof())
        prs->MoveFirst();

    prs->GetFieldValue("Budget", dblVal);
    prs->GetFieldValue("ClientName", strClient);

    prs->Close();
}
**CADORecordset::OpenSchema**

The `OpenSchema` function obtains database schema information from the provider.
BOOL OpenSchema(int nSchema, LPCTSTR SchemaID = _T(""));
throw(CADOException);
**Parameters**
- `int nSchema` - A value that represents the type of schema query. 
- `LPCTSTR SchemaID = _T("")` - The GUID for a provider-schema query not defined by the OLE DB specification. 

|Constant|Value|Description|Constraint Columns|
|----|----|----|----|
|**`schemaAsserts`**|0|Returns the assertions defined in the catalog that are owned by a given user.            ([ASSERTIONS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbassertions_rowset.asp))|`CONSTRAINT_ CATALOG`*`CONSTRAINT_ SCHEMA`*`CONSTRAINT_NAME`|
|`**schemaCatalogs**`|1|Returns the physical attributes associated with catalogs accessible from the DBMS.            ([CATALOGS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcatalogs_rowset.asp))|`CATALOG_NAME`|
|`**schemaCharacterSets**`|2|Returns the character sets defined in the catalog that are accessible to a given user.            ([CHARACTER_SETS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcharacter_sets_rowset.asp))|`CHARACTER_ SET_CATALOG`*`CHARACTER_ SET_SCHEMA`*`CHARACTER_SET _NAME`*|
|`**schemaCheckConstraints**`|5|Returns the check constraints defined in the catalog that are owned by a given user.            ([CHECK_CONSTRAINTS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcheck_constraints_rowset.asp))|`CONSTRAINT_ CATALOG`*`CONSTRAINT_ SCHEMA`*`CONSTRAINT_NAME`|
|`**schemaCollations**`|3|Returns the character collations defined in the catalog that are accessible to a given user.([COLLATIONS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcollations_rowset.asp))|`COLLATION_ CATALOG`*`COLLATION_SCHEMA``COLLATION_NAME`|
|`**schemaColumnPrivileges**`|13|Returns the privileges on columns of tables defined in the catalog that are available to, or granted by, a given user.([COLUMN_PRIVILEGES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcolumn_privileges_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``COLUMN_NAME``GRANTOR``GRANTEE`|
|`**schemaColumns**`|4|Returns the columns of tables (including views) defined in the catalog that are accessible to a given user.([COLUMNS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcolumns_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``COLUMN_NAME`|
|**`schemaColumnsDomainUsage`**|11|Returns the columns defined in the catalog that are dependent on a domain defined in the catalog and owned by a given user.            ([COLUMN_DOMAIN_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbcolumn_domain_usage_rowset.asp))|`DOMAIN_CATALOG``DOMAIN_SCHEMA``DOMAIN_NAME``COLUMN_NAME`|
|**`schemaConstraintColumnUsage`**|6|Returns the columns used by referential constraints, unique constraints, check constraints, and assertions, defined in the catalog and owned by a given user.            ([CONSTRAINT_COLUMN_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbconstraint_column_usage_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``COLUMN_NAME`|
|**`schemaConstraintTableUsage`**|7|Returns the tables that are used by referential constraints, unique constraints, check constraints, and assertions defined in the catalog and owned by a given user.            ([CONSTRAINT_TABLE_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbconstraint_table_usage_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME`|
|**`schemaCubes`**|32|Returns information about the available cubes in a schema (or the catalog, if the provider does not support schemas).            ([CUBES Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olapcubes_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME`|
|**`schemaDBInfoKeywords`**|30|Returns a list of provider-specific keywords.            ([IDBInfo::GetKeywords*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbidbinfo__getkeywords.asp))|<None>|
|**`schemaDBInfoLiterals`**|31|Returns a list of provider-specific literals used in text commands.            ([IDBInfo::GetLiteralInfo*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbidbinfo__getliteralinfo.asp))|<None>|
|**`schemaDimensions`**|33|Returns information about the dimensions in a given cube. It has one row for each dimension.            ([DIMENSIONS Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olapdimensions_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``DIMENSION_NAME``DIMENSION_ UNIQUE_NAME`*|
|**`schemaForeignKeys`**|27|Returns the foreign key columns defined in the catalog by a given user.            [(FOREIGN_KEYS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbforeign_keys_rowset.asp))|`PK_TABLE_CATALOG``PK_TABLE_SCHEMA``PK_TABLE_NAME``FK_TABLE_CATALOG``FK_TABLE_SCHEMA``FK_TABLE_NAME`|
|**`schemaHierarchies`**|34|Returns information about the hierarchies available in a dimension.            ([HIERARCHIES Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olaphierarchies_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``DIMENSION_ UNIQUE_NAME`*`HIERARCHY_NAME``HIERARCHY_ UNIQUE_NAME`*|
|**`schemaIndexes`**|12|Returns the indexes defined in the catalog that are owned by a given user.            ([INDEXES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbindexes_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``INDEX_NAME``TYPE``TABLE_NAME`|
|**`schemaKeyColumnUsage`**|8|Returns the columns defined in the catalog that are constrained as keys by a given user.            ([KEY_COLUMN_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbkey_column_usage_rowset.asp))|`CONSTRAINT_ CATALOG`*`CONSTRAINT_ SCHEMA`*`CONSTRAINT_NAME``TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``COLUMN_NAME`|
|**`schemaLevels`**|35|Returns information about the levels available in a dimension.            ([LEVELS Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olaplevels_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``DIMENSION_ UNIQUE_NAME`*`HIERARCHY_ UNIQUE_NAME`*`LEVEL_NAME``LEVEL_ UNIQUE_NAME`*|
|**`schemaMeasures`**|36|Returns information about the available measures.            ([MEASURES Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olapmeasures_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``MEASURE_NAME``MEASURE_ UNIQUE_NAME`*|
|**`schemaMembers`**|38|Returns information about the available members.            ([MEMBERS Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olapmembers_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``DIMENSION_ UNIQUE_NAME`*`HIERARCHY_ UNIQUE_NAME`*`LEVEL_ UNIQUE_NAME`*`LEVEL_NUMBER``MEMBER_NAME``MEMBER_UNIQUE _NAME`*`MEMBER_CAPTION``MEMBER_TYPE`Tree operator (For more information, see the [OLE DB for OLAP](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbpart3_ole_db_for_olap.asp) documentation.)|
|**`schemaPrimaryKeys`**|28|Returns the primary key columns defined in the catalog by a given user.            ([PRIMARY_KEYS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbprimary_keys_rowset.asp))|`PK_TABLE_CATALOG``PK_TABLE_SCHEMA``PK_TABLE_NAME`|
|**`schemaProcedureColumns`**|29|Returns information about the columns of rowsets returned by procedures.            ([PROCEDURE_COLUMNS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbprocedure_columns_rowset.asp))|`PROCEDURE_ CATALOG`*`PROCEDURE_SCHEMA``PROCEDURE_NAME``COLUMN_NAME`|
|**`schemaProcedureParameters`**|26|Returns information about the parameters and return codes of procedures.            ([PROCEDURE_PARAMETERS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbprocedure_parameters_rowset.asp))|`PROCEDURE_ CATALOG`*`PROCEDURE_SCHEMA``PROCEDURE_NAME``PARAMETER_NAME`|
|**`schemaProcedures`**|16|Returns the procedures defined in the catalog that are owned by a given user.            ([PROCEDURES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbprocedures_rowset.asp))|`PROCEDURE_ CATALOG`*`PROCEDURE_SCHEMA``PROCEDURE_NAME``PROCEDURE_TYPE`|
|**`schemaProperties`**|37|Returns information about the available properties for each level of the dimension.            ([PROPERTIES Rowset*](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/olapproperties_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``CUBE_NAME``DIMENSION_ UNIQUE_NAME`*`HIERARCHY_ UNIQUE_NAME`*`LEVEL_ UNIQUE_NAME`*`MEMBER_ UNIQUE_NAME`*`PROPERTY_TYPE``PROPERTY_NAME`|
|**`schemaProviderSpecific`**|-1|Used if the provider defines its own nonstandard schema queries.|<Provider specific>|
|**`schemaProviderTypes`**|22|Returns the (base) data types supported by the data provider.            ([PROVIDER_TYPES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbprovider_types_rowset.asp))|`DATA_TYPE``BEST_MATCH`|
|**`schemaReferentialConstraints`**|9|Returns the referential constraints defined in the catalog that are owned by a given user.            ([REFERENTIAL_CONSTRAINTS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbreferential_constraints_rowset.asp))|`CONSTRAINT_ CATALOG`*`CONSTRAINT_ SCHEMA`*`CONSTRAINT_NAME`|
|**`schemaSchemata`**|17|Returns the schemas (database objects) that are owned by a given user.            ([SCHEMATA Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbschemata_rowset.asp))|`CATALOG_NAME``SCHEMA_NAME``SCHEMA_OWNER`|
|**`schemaSQLLanguages`**|18|Returns the conformance levels, options, and dialects supported by the SQL-implementation processing data defined in the catalog.            ([SQL_LANGUAGES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbsql_languages_rowset.asp))|<None>|
|**`schemaStatistics`**|19|Returns the statistics defined in the catalog that are owned by a given user.            ([STATISTICS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbstatisticsrowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME`|
|**`schemaTableConstraints`**|10|Returns the table constraints defined in the catalog that are owned by a given user.            ([TABLE_CONSTRAINTS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbtable_constraints_rowset.asp))|`CONSTRAINT_ CATALOG`*`CONSTRAINT_ SCHEMA`*`CONSTRAINT_NAME``TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``CONSTRAINT_TYPE`|
|**`schemaTablePrivileges`**|14|Returns the privileges on tables defined in the catalog that are available to, or granted by, a given user.            ([TABLE_PRIVILEGES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbtable_privileges_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``GRANTOR``GRANTEE`|
|**`schemaTables`**|20|Returns the tables (including views) defined in the catalog that are accessible to a given user.            ([TABLES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbtables_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME``TABLE_TYPE`|
|**`schemaTranslations`**|21|Returns the character translations defined in the catalog that are accessible to a given user.            ([TRANSLATIONS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbtranslations_rowset.asp))|`TRANSLATION_ CATALOG`*`TRANSLATION_ SCHEMA`*`TRANSLATION _NAME`*|
|**`schemaTrustees`**|39|Reserved for future use.||
|**`schemaUsagePrivileges`**|15|Returns the USAGE privileges on objects defined in the catalog that are available to, or granted by, a given user.            ([USAGE_PRIVILEGES Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbusage_privileges_rowset.asp))|`OBJECT_CATALOG``OBJECT_SCHEMA``OBJECT_NAME``OBJECT_TYPE``GRANTOR``GRANTEE`|
|**`schemaViewColumnUsage`**|24|Returns the columns on which viewed tables, defined in the catalog and owned by a given user, are dependent.            ([VIEW_COLUMN_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbview_column_usage_rowset.asp))|`VIEW_CATALOG``VIEW_SCHEMA``VIEW_NAME`|
|**`schemaViews`**|23|Returns the views defined in the catalog that are accessible to a given user.            ([VIEWS Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbviews_rowset.asp))|`TABLE_CATALOG``TABLE_SCHEMA``TABLE_NAME`|
|**`schemaViewTableUsage`**|25|Returns the tables on which viewed tables, defined in the catalog and owned by a given user, are dependent.            ([VIEW_TABLE_USAGE Rowset](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/oledb/htm/oledbview_table_usage_rowset.asp))|`VIEW_CATALOG``VIEW_SCHEMA``VIEW_NAME`|

*****Ignore the spaces in these values, as they have been added to avoid page scrolling.

**Return Value**

Returns `TRUE` if it was executed successfully.

**For Example**

![](http://www.codeproject.com/images/minus.gif) Collapse
try
{
    CAD if(pdb.Open("Data Source=sysdevpdc;Provider=""MSOLAP;Initial Catalog=Foodmart 2000"))
    {
        CADORecordset prs(&pdb);
        if(prs.OpenSchema(CADORecordset::schemaMeasures))
        {
            while(!prs.IsEof())
            {dset::schemaMeasures))
        {
            while(!prs.IsEof())
            {
                CString strValue;
                CString strUniqueName;
                CString strSqlColumnName;
                int nDataType = 0;
                int nValue = 0;

                prs.GetFieldValue("CUBE_NAME", strValue);
                prs.GetFieldValue("MEASURE_NAME", strValue);
                prs.GetFieldValue("MEASURE_AGGREGATOR", nValue);
                prs.GetFieldValue("MEASURE_UNIQUE_NAME", 
                                                  strUniqueName);
                prs.GetFieldValue("MEASURE_NAME_SQL_COLUMN_NAME", 
                                               strSqlColumnName);
                prs.GetFieldValue("DATA_TYPE", nDataType);
                prs.MoveNext();
            }
        }
        prs.Close();
    }
}
catch(CADOException &e)
{
...
}
**CADORecordset::Requery**

The `Requery` function refreshes an open recordset.
BOOL Requery();
throw(CADOException);
**Return Value**

Returns `TRUE` if it was executed successfully.

## Sample 01:

![](http://www.codeproject.com/images/minus.gif) Collapse
CADODatabase* pAdoDb = new CADODatabase();
CString strConnection = "";

strConnection = _T("Provider=MSDASQL;""PersistSecurityInfo=False;Trusted_Connection=Yes
Data Source=Access Sql Server;catalog=sampledb");

if(pAdoDb->Open((LPCTSTR)strConnection))
{
    CString strQry = _T("");
    int numRecords;

    strQry.Format(_T("sp_StoreClientFields_ps '%s', %d"), 
                  (LPCTSTR)strParam1, nParam2);

    CADORecordset* pRs = new CADORecordset(pAdoDb);

    if(!pRs->Open((LPCTSTR)strQry))
    {
    delete pRs;
    delete pAdoDb;
    return FALSE
    }

    numRecords = pRs->GetRecordCount();
    while(!pRs->IsEof())
    {
      CString strVal = _T("");
      int nVal = 0;
      //Get Numeric Field Value
      pRs->GetFieldValue("NumField1", nVal)

      //Get String Field Data
      pRs->GetFieldValue("StrField..", strVal)
        DoSomething(nVal, strVal);

        pRs->MoveNext();
    }
    pRs->Close();
}
elsereturn FALSE;
### **The CADOParameter Class**

The `CADOParameter` class has a set of functions that corresponds to the `_ParameterPtr`.

**CADOParameter::CADOParameter**

Creates a `CADOParameter` object.
CADOParameter(int nType, long lSize = 0, int nDirection = paramInput, 
              CString strName = _T(""));
**Parameters**
- `int nType` - An `int` value that specifies the data type of the `CADOParameter` object. Can be one of the values specified in [CADORecordset::GetFieldInfo](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetFieldInfoType). If you are using `CADORecordset::typeNumeric` or `CADORecordset::typeDecimal`, you must define the **precision** and **scale** values. 
- `long lSize = 0` - An optional `long` value that specifies the maximum length for the parameter value in bytes or characters. 
- `int nDirection = paramInput` - An optional `int` value that specifies the direction of the `CADOParameter` object. Can be one of the following values:
    - `CADOParameter::paramUnknown` - Indicates that the parameter direction is unknown. 
- `CADOParameter::paramInput` - Default. Indicates that the parameter represents an input parameter. 
- `CADOParameter::paramOutput` - Indicates that the parameter represents an output parameter. 
- `CADOParameter::paramInputOutput` - Indicates that the parameter represents both an input and output parameter. 
- `CADOParameter::paramReturnValue` - Indicates that the parameter represents a return value. 
- `CString strName = _T("")` - An optional string that specifies the name of the `CADOParameter` object. 

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADOParameter::SetValue**

The `SetValue` function sets the value for the `CADOParameter` object.
BOOL SetValue(int nValue);
BOOL SetValue(long lValue);
BOOL SetValue(double dbValue);
BOOL SetValue(CString strValue);
BOOL SetValue(COleDateTime time);
BOOL SetValue(_variant_t vtValue);
**Parameters**
- `int nValue` - An `int` value containing the parameter value. 
- `long lValue` - A `long` value containing the parameter value. 
- `double dbValue` - A `double` value containing the parameter value. 
- `CString strValue` - A string value containing the parameter value. 
- `COleDateTime time` - A time value containing the parameter value. 
- `_variant_t vtValue` - A variant value containing the parameter value. 

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADOParameter::SetPrecision**

The `SetPrecison` function sets the precision for the `CADOParameter` object.
void SetPrecision(int nPrecision);
**CADOParameter::SetScale**

The `SetScale` function sets the scale for the `CADOParameter` object.
void SetScale(int nScale);
**CADOParameter::GetValue**

The `GetValue` function returns the value of the `CADOParameter` object.
BOOL GetValue(int& nValue);
BOOL GetValue(long& lValue);
BOOL GetValue(double& dbValue);
BOOL GetValue(CString& strValue, CString strDateFormat = _T(""));
BOOL GetValue(COleDateTime& time);
BOOL GetValue(_variant_t& vtValue);
**Parameters**
- `int& nValue` - A reference to an `int` that will store the value of the parameter. 
- `long& lValue` - A reference to a `long` that will store the value of the parameter. 
- `double& dbValue` - A reference to a `double` that will store the value of the parameter. 
- `CString& strValue` - A reference to a string that will store the value of the parameter. 
- `CString strDateFormat = _T("")` - A formatting time string similar to the `strftime` formatting string. 
- `COleDateTime& time` - A reference to a time object that will store the value of the parameter. 
- `_variant_t& vtValue` - A reference to a variant object that will store the value of the parameter. 

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADOParameter::SetName**

The `SetName` function sets the name of the `CADOParameter` object. 
CString SetName(CString strName);
**Parameters**

`CString strName` - A string specifying the parameter name.

**CADOParameter::GetName**

The `GetName` function returns the `CADOParameter` object.
CString GetName();
**CADOParameter::GetType**

The `GetType` function returns the type of the `CADOParameter` object.
int GetType();
**CADOParameter::GetParameter**

The `GetParameter` function returns a pointer to a `_Parameter` object.
_ParameterPtr GetParameter();
### **The CADOCommand Class**

The `CADOCommand` class has a set of functions that corresponds to the `_CommandPtr`.

**CADOCommand::CADOCommand**

Creates a `CADOCommand` object.
CADOCommand(CADODatabase* pAdoDatabase, CString strCommandText = _T(""),
                                  int nCommandType = typeCmdStoredProc);
**Parameters**
- `CADODatabase* pAdoDatabase` - A `CADODatabase` object pointer. 
- `CString strCommandText = _T("")` - An optional string that indicates the text of the `CADOCcommand` object. 
- `int nCommandType = typeCmdStoredProc` - An optional `int` value that indicates the type of the `CADOCommand` object. Can be one of the following values:
    - `CADOCommand::typeCmdText` - Evaluates `CommandText` as a textual definition of a command or stored procedure call. 
- `CADOCommand::typeCmdTable` - Evaluates `CommandText` as a table name whose columns are all returned by an internally generated SQL query. 
- `CADOCommand::typeCmdTableDirect` - Evaluates `CommandText` as a table name whose columns are all returned. 
- `CADOCommand::typeCmdStoredProc` - Default. Evaluates `CommandText` as a stored procedure name. 
- `CADOCommand::typeCmdUnknown` - Indicates that the type of the command in the `CommandText` property is not known. 
- `CADOCommand::typeCmdFile` - Evaluates `CommandText` as the file name of a persistently stored recordset. Used with `Recordset.Open` or `Requery` only. 

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADOCommand::AddParameter**

The `AddParameter` function.
BOOL AddParameter(CADOParameter* pAdoParameter);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, int nValue);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, long lValue);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, double dblValue, int nPrecision = 0,
                          int nScale = 0);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, CString strValue);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, COleDateTime time);
BOOL AddParameter(CString strName, int nType, int nDirection,
                          long lSize, _variant_t vtValue, int nPrecision = 0,
                          int nScale = 0);
**Parameters**
- `CADOParameter* pAdoParameter` - A pointer to a `CADOParameter` object. 
- `CString strName` - A string that specifies the name of the parameter. 
- `int nType` - An `int` value that specifies the data type of the `CADOParameter` object. Can be one of the values specified in [`CADORecordset::GetFieldInfo`](http://www.codeproject.com/KB/database/caaadoclass1.aspx#GetFieldInfoType). If you are using `CADORecordset::typeNumeric` or `CADORecordset::typeDecimal`, you must define the **precision** and **scale** values. 
- `int nDirection` - An `int` value that specifies the direction of the `CADOParameter` object. Can be one of the values specified in [`CADOParameter::CADOParameter`](http://www.codeproject.com/KB/database/caaadoclass1.aspx#CADOParameter). 
- `long lSize` - A `long` value that specifies the maximum length for the parameter value in bytes or characters. 
- `int nValue` - An `int` value containing the parameter value. 
- `long lValue` - A `long` value containing the parameter value. 
- `double dblValue` - A `double` value containing the parameter value. 
- `int nPrecision` - An `int` value containing the precision of the parameter value. 
- `int nScale` - An `int` value containing the scale of the parameter value. 
- `CString strValue` - A string value containing the parameter value. 
- `COleDateTime time` - A time value containing the parameter value. 
- `_variant_t vtValue` - A variant value containing the parameter value. 

**Return Value**

Returns `TRUE` if it was executed successfully.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

**CADOCommand::SetText**

The `SetText` function sets the command text of the `CADOCommand` object.
void SetText(CString strCommandText);
**Parameters**

`CString strCommandText` - A string that indicates the command text.

**CADOCommand::GetText**

The `GetText` function returns the command text of the `CADOCommand` object.
CString GetText();
**CADOCommand::SetType**

The `SetType` function sets the type of the `CADOCommand` object.
void SetType(int nCommandType);
**Parameters**

`int nCommandType` - A `int` value that indicates the type of the command.

**CADOCommand::GetType**

The `GetType` function returns the type of the `CADOCommand` object.
int GetType();
**CADOCommand::GetCommand**

The `GetCommand` function returns a `Command` pointer.
_CommandPtr GetCommand();
**CADOCommand::Execute**

The `Execute` function executes a command text.
BOOLExecute();
**Return Value**

Returns `TRUE` if it was executed successfully.

**CADOCommand::GetRecordsAffected**

The `GetRecordsAffected` function returns the number of records affected by the last command executed.
int GetRecordsAffected();
## Sample 02:

![](http://www.codeproject.com/images/minus.gif) Collapse

```
**//SQL SCRIPT...**
Create Procedure sp_OutputTest
@IN1 int,
@OutInt int Output,
@OutChar varchar(20) Output
As   
    SELECT    
        @OutChar = 'Hello World'
    SELECT    
        @OutInt = 10 * @IN1
    return (90)
GO

**//Visual C++ Code...**

CADOParameter pParamRetVal(CADORecordset::typeInteger, sizeof(int),
                           CADOParameter::paramReturnValue);
CADOParameter pParamIn(CADORecordset::typeInteger, sizeof(int));
CADOParameter pParamOutInt(CADORecordset::typeInteger, sizeof(int),
                           CADOParameter::paramOutput);
CADOParameter pParamOutChar(CADORecordset::typeChar, sizeof(char) * 20,
                            CADOParameter::paramOutput);

pParamIn.SetValue(2);

CADOCommand pCmd(&pDb, "sp_OutputTest");

pCmd.AddParameter(&pParamRetVal);
pCmd.AddParameter(&pParamIn);
pCmd.AddParameter(&pParamOutInt);
pCmd.AddParameter(&pParamOutChar);

CADORecordset* prs = new CADORecordset(pDb);

if(pRs->Execute(&pCmd))
{
    int nVal = 0;
    int nRetVal = 0;
    CString str = _T("");

    pParamRetVal.GetValue(nRetVal);
    pParamOutInt.GetValue(nVal);
    pParamOutChar.GetValue(str);
}

delete pRS;
```

### **The CJectEngine Class**

The `CJetEngine` class has a set of functions that corresponds to the `_RecordsetPtr`.

**CJetEngine::CompactDatabase**

Compacts an Access database.
BOOLCompactDatabase(CString strDatabaseSource, CString strDatabaseDestination)
**Parameters**
- `CString strDatabaseSource` - A connection string to the source database. 
- `CString strDatabaseDestination` - A connection string to the destination database. 

**Remarks:**

The connection strings must be like this: `"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:/the Database Path"`.

**Return Value**

Returns `TRUE` if it was executed successfully.

**For example**
CString strCompactedDBConnection;
CString strConnection;

CString strCompactedDBPath = _T("C://My Projects//Database//SampleDB.tmp");
CString strDatabasePath = _T("C://My Projects//Database//SampleDB.mdb");

strCompactedDBConnection.Format("Provider=Microsoft.Jet.OLEDB.""4.0;Data Source=%s", strCompactedDBPath.GetBuffer(0));
strConnection.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", 
                                         strDatabasePath.GetBuffer(0));
if(CJetEngine::CCompactDatabase(strConnection, strCompactedDBConnection))
{
    ::CopyFile(strCompactedDBPath.GetBuffer(0), 
          strDatabasePath.GetBuffer(0), FALSE);
}
### **The CADOException Class**

The `CADOException` class represents an exception condition. The class includes public data members you can use to determine the cause of the exception.

**CADOException::CADOException**

Creates a `CADOException` object.
CADOExcpetion();
To retrieve the ADO error information:
- Construct a `CADOException` object. 
- Call any exception object’s `GetErrorMesaage` or `GetError` member function, to retrieve information about the error, via the exception object. 

**For Example**
try
{
    if(pDb.Open(_T("Provider=Microsoft.JET.OLEDB.4.0;") 
                _T("Data source = c://vc projects//ado//dbTest.mdb;")))
    {
        CADORecordset pRS(&pDb);

        if(pRS.Open("tbltest", CADORecordset::openTable))
        {
            pRS.SetSort("Descriptions");
            .
            .
            .
        }
        pRS.Close();
    }
}
catch(CADOException &e)
{
    AfxMessageBox(e.GetErrorMessage());
}
**CADOException::GetErrorMessage**
CString GetErrorMessage() const;
**Return Value**

Returns error description.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample07)

**CADOException::GetError**
long GetError();
**Return Value**

Returns the error code.

**CADOException::SetError**
void SetError(long lErrorCode = 0);
void SetErrorMessage(LPCSTR lpstrError = _T(""));
**Parameters**
- `long lErrorCode = 0` - A `long` value containing the error code. 
- `LPCSTR lpstrError = _T("")` - A string value containing the error description. 

### **The COLEDBDataLink Class**

The `COLEDBDataLink` class is an utility class to create and edit datalinks.

**COLEDBDataLink::COLEDBDataLink**

Creates a `COLEDBDataLink` object.
COLEDBDataLink();
**COLEDBDataLink::New**
CString New(HWND hWnd = NULL);
throw(COLEDBException);
**Parameters**

`HWND hWnd = NULL` - The parent window handle.

**Return Value**

Returns a string informing the connection string corresponding to the created connection.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample08)

**COLEDBDataLink::Edit**
void Edit(LPCSTR lpstrConnectionString, HWND hWnd);
throw(COLEDBException);
**Parameters**
- `LPCSTR lpstrConnectionString`
- `HWND hWnd` - The parent window handle. 

**For Example**
COLEDBDataLink dtlnk;

try
{
    dtlnk.Edit("Provider=Microsoft.JET.OLEDB.4.0;Data source"" = c://vc projects//ado//dbTest.mdb;", this->m_hWnd);
}
catch(COLEDBException &e)
{
    AfxMessageBox(e.GetErrorMessage());
}
### **The COLEDBException Class**

The `COLEDBException` class represents an exception condition. The class includes public data members you can use to determine the cause of the exception.

**COLEDBException::COLEDBException**

Creates a `COLEDBException` object.
COLEDBException();
To retrieve the OLEDB error information:
- Construct a `COLEDBException` object. 
- Call any exception object’s `GetErrorMessage` or `GetError` member function to retrieve information about the error, via the exception object. 

**For Example**
CADODatabase pDb;
COLEDBDataLink dtlnk;

try
{
    pDb.Open(dtlnk.New(this->m_hWnd));
}
catch(COLEDBException &e)
{
    AfxMessageBox(e.GetErrorMessage());
}
**COLEDBException::GetErrorMessage**
CString GetErrorMessage() const;
**Return Value**

Returns error description.

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#Sample08)

**COLEDBException::GetError**
long GetError();
**Return Value**

Returns the error code.

**COLEDBException::SetError**
void SetError(long lErrorCode = 0);
void SetErrorMessage(LPCSTR lpstrError = _T(""));
**Parameters**
- `long lErrorCode = 0` - A `long` value containing the error code. 
- `LPCSTR lpstrError = _T("")` - A string value containing the error description. 

## Common Questions:

### How Bind the MS DataGrid Control to CADORecordset:
- **First:** You need to add the MS DataGrid Control to your application. 
- **Second:** Apply the `CADORecordset::GetRecordset` function to the `SetRefDataSource` function of the data control class. 
if(m_pRs.Open("tblTest", CADORecordset::openTable))
{
    **//m_datagridctrl is the CDataGrid Class...    m_datagridctrl.SetCaption("Clients");    m_datagridctrl.SetRefDataSource(NULL);    m_datagridctrl.SetRefDataSource((LPUNKNOWN)m_pRs.GetRecordset());    m_datagridctrl.Refresh();}**
For reference on how to add a data control to your project, please see [this article](http://www.codeproject.com/database/msdatagrid.asp) by **Rashid Thadha**.

### How do I re-sort a Recordset?

The answer is easy. You have several functions and properties that are part of `_RecordsetPtr`. Using `CADORecordset::GetRecordset()`, you get a pointer to `_RecordsetPtr`. You can use it for setting the `Sort` property.

**Note:** You don't need to use `_RecordsetPtr::Refresh()`.
if(m_pRs.Open("tblTest", CADORecordset::openTable))
{
    m_pRs.GetRecordset()->PutSort("Field01 DESC");
}
Now: you have the function [`CADORecordset::SetSort();`](http://www.codeproject.com/KB/database/caaadoclass1.aspx#SetSort)

### fatal error C1010: unexpected end of file while looking for precompiled header directive

For solving this common problem, you need to use **automatic use of precompiled headers**. Select Project/Settings (or ALT+F7) then, in the **Project Settings** dialog, select the **C/C++** tab, then select the **Precompiled Headers** item in the combo box, and select the **Automatic use of precompiled headers** option.

### How do you get the return codes from your stored procedures in SQL Server?

You must use the `CADOParameter` class. `CADOParameter::paramReturnValue` gets the value that is returned by the stored procedure. It always must be an integer and must be in the first place in the list of parameters. If you are not using parameters for input or output, you only need to use this parameter....

[See Sample](http://www.codeproject.com/KB/database/caaadoclass1.aspx#TheSample02)

### I've been trying to work with the CADOCommand class and be able to pass in a UUID field. I've not been very successful so far. Do you by chance have an example that shows this?

You are talking about `uniqueidentifier` type field? Okay, the problem is the following:

First you can update the table using the SQL syntax `NEWID()`. That command will do for you a unique identifier string like: **"{B6B83A8C-F92C-4FA7-9641-631143E6056C}"**... If you want to insert data into that type of field you need to use a string like that... In conclusion we can say: you need to use a **string parameter type**, with the GUID format, and it must be unique... Aren't there a lot of conditions!!!...

But I wrote a sample: In the following sample, you can see that I used the UUID parameter as string. You can see in the stored procedure that the parameter is of **`uniqueidentifier`** type.
**//Visual C++ SampleCADOParameter pParamRet(CADORecordset::typeInteger, sizeof(int),                        CADOParameter::paramReturnValue);CADOParameter pParamInChar(CADORecordset::typeChar, sizeof(char) * 40,                           CADOParameter::paramInput);CADOParameter pParamID(CADORecordset::typeInteger, sizeof(int));CString strUUID = CString("{B6B83A8C-F92C-4FA7-9641-631143E6056C}");pParamInChar.SetValue(strUUID);pParamID.SetValue(1);CADOCommand pCmd(&m_pDB, "sp_UUIDTest");pCmd.AddParameter(&pParamRet);pCmd.AddParameter(&pParamInChar);pCmd.AddParameter(&pParamID);if(pRs.Execute(&pCmd)){    int nVal = 0;    pParamRet.GetValue(nVal);}**
```
**//SQL SCRIPT**
Create Procedure sp_UUIDTest
@UUID uniqueidentifier = NULL,
@ID integer
AS
    UPDATE
        systable01
    SET
        UUID = @UUID
    WHERE
        sysfield00 = @ID
    
    if @@Error = 0
        return 1
    else
        return 99
GO
```

### ADO and ADOX together

I receive several emails per day asking me how I can use the **ADO** and **ADOX** classes together. The answer is, right now, from version 2.10 of ADO, you can use the two types of classes together. For this, you must include the headers and CPP files of each class and then must include the headers in the project.

**Remarks**

It's very important to verify that the includes must be in the following order. First the **ADOX** classes header and then the **ADO** classes header.
#include<adox.h>#include<ado2.h>
**Carlos A. Antollini.**

## Updates
- 25 Apr 2001 - Version 1.0 released. 
- 20 May 2001 - Version 1.1
    
Added the following functions:
- `AbsolutePage`, `GetPageCount`, `PageSize`, `AbsolutePosition`, `GetFieldInfo`, `GetChunk`, `GetString`.

- 29-Jun-2001 - Version 1.2
    
Added the following functions:
- `AddNew`, `Edit`, `Delete`, `Update`, `CancelUpdate`, `SetFieldValue`, `Find`, `FindFirst`, `FindNext`, `SetBookmark`, `GetBookmark`.

- 29-Sept-2001 - Version 1.25
    - Added support for SQL Server 2000. 
- Corrected bug reported by Robert Pittenger for empty fields. 

- 10-Oct-2001 - Version 1.30
    - Corrected bug reported by Giles Forster and Rainer Mangold in `SetFieldValue` for empty fields. 
- Added the data type enum. 

- 05-Nov-2001 - Version 1.37
    - Update for adding capacity to bind the DataGrid to `CADORecodset`. 
- Added function `GetLastErrorString` and changed the data type returned by `GetLastError`. 
- Added functions `Clone` and `SetFilter` (petitioned by Marcel Stoica). 

- 15-Jan-2002 - Version 1.40
    - Added the new `GetChunk` and `AppendChunk` functions written by Giles Forster. 

- 15-Feb-2002 - Version 1.45
    - Corrected bug reported by J. C. Martinez Galán in decimal fields. 
- Added **XML** support. 

- 04-Apr-2002 - Version 2.00
    - Added `CADOParameter` and `CADOCommand` classes. 

- 14-May-2002 - Version 2.01
    - Added `COleCurrency` and `bool` in `CADORecordset::GetFieldValue` and `CADORecordset::SetFieldValue`. 

- 25-July-2002 - Version 2.04
    - Added `CADODatabase::GetRecordsAffcetd`, `CADOCommand::GetRecordsAffected`, `CADOCommand::Execute`, `CADOParameter::SetPrecision`, `CADOParamater::SetScale` and bug corrected in `CADOCommand::AddParameter`. 

- 2-July-2003 - Version 2.10
    - Added compatibility with the [ADOX](http://www.codeproject.com/KB/database/AdoXClass.html) classes to be used together. (Petitioned by hundreds of users.) 
- Added `CADODatabase::SetConnectionMode`, `CADORecordset::OpenSchema`. 
- Added `CJetEngine::CCompactDatabase`. 

- 2-October-2003 - Version 2.11
    - Memory leak in `CADOCommand` fixed by Jan Stocker. 

- 08-August-2004 - `COLEDBDataLink` was created under Version 1.01.
    - `COLEDBDataLink` class was created, and added to ADO classes. 

- 10-Februry-2005 - Version 2.15
    - `CADOException` class was added. 

## Special thanks

These classes received many suggestions from the users. Thanks to all for your collaboration.
- Rainer Mangold 
- Christian Graus 
- Mike Osbahr 
- Francis BONTHOUX 
- Spiros Prantalos 
- Robert Pittenger 
- Giles Forster 
- Rob Sandhu 
- Marcel Stoica 
- Daniel Berman 
- Jan Stocker 
- And tons of people requesting for new versions. 


