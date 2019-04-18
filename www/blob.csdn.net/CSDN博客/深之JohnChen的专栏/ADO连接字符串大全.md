# ADO连接字符串大全  - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:30:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1544


**ADO连接字符串大全 **

This page contains sample ADO connection strings for ODBC DSN / DSN-Less,
OLE DB Providers, Remote Data Services (RDS), MS Remote, MS DataShape. 

Also included are ADO.NET connection strings for each .NET Managed Provider 
(SQLClient, OLEDB, and ODBC).

These sample connection strings are compiled 
by Carl Prothman, a Microsoft Visual Basic MVP 
Enjoy!

Table of Contents
ODBC DSN Connections DSN 
File DSN

ODBC DSN-Less Connections　 ODBC Driver for AS/400 
ODBC Driver for Access 
ODBC Driver for dBASE 
ODBC Driver for Excel 
ODBC Driver for MySQL 
ODBC Driver for Oracle 
ODBC Driver for Paradox 
ODBC Driver for SQL Server 
ODBC Driver for Sybase 
ODBC Driver for Sybase SQL Anywhere 
ODBC Driver for Text 
ODBC Driver for Teradata 
ODBC Driver for Visual FoxPro

OLE DB Data Link Connections Data Link File (UDL)

OLE DB Data Provider Connections　 OLE DB Provider for AS/400 
OLE DB Provider for Active Directory Service 
OLE DB Provider for DB2 
OLD DB Provider for Internet Publishing 
OLE DB Provider for Index Server 
OLE DB Provider for Microsoft Jet 
OLE DB Provider for ODBC Databases 
OLE DB Provider for Oracle (From Microsoft) 
OLE DB Provider for Oracle (From Oracle) 
OLE DB Provider for Simple Provider 
OLE DB Provider for SQL Server 

Remote Data Service (RDS) Connections RDS Data Control - Connect Property 
RDS Data Control - URL Property

ADO URL Connections ADO Recordset

MS Remote Provider Connections MS Remote - Access (Jet) 
MS Remote - SQL Server

Data Shape Provider Connections　 MS DataShape - SQL Server

.NET Managed Provider Connections SQL Client .NET Managed Provider (System.Data.SqlClient) 
OLE DB .NET Managed Provider (System.Data.OleDb) 
ODBC .NET Managed Provider (System.Data.ODBC) 

ODBC DSN Connections
Using an ODBC DSN (Data Source Name) is a two step process. 

1) You must first create the DSN via the "ODBC Data Source Administrator" program 
found in your computer's Control Panel (or Administrative Tools menu in Windows 2000).
Make sure to create a SYSTEM DSN (not a USER DSN) when using ASP. 
Note: You can also create the DSN via VB code.

2) Then use the following connection string - with your own DSN name of course.　 ;-)

ODBC - DSN 

oConn.Open "DSN=AdvWorks;" & _ 
　　　　　　　　　 "Uid=Admin;" & _ 
　　　　　　　　　 "Pwd=;

You can also create and use a File DSN. Then use the following ADO Connection string:

ODBC - File DSN 

oConn.Open "FILEDSN=c:/somepath/mydb.dsn;" & _ 
　　　　　　　　　 "Uid=Admin;" & _
　　　　　　　　　 "Pwd=;"

For more information, see: About ODBC data sources and 
How to Use File DSNs and DSN-less Connections

Note: The problem with DSN is that Users can (and will) modify them (or delete by mistake), 
then your program won't work so well... So it's better to use a DSN-Less or OLE DB Provider 
connection string with a Trusted Connection if possible! 

ODBC DSN-Less Connections
ODBC Driver for AS/400 

oConn.Open "Driver={Client Access ODBC Driver (32-bit)};" & _
　　　　　　　　　 "System=myAS400;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For more information, see:　 A Fast Path to AS/400 Client/Server 

ODBC Driver for Access 

For Standard Security:

oConn.Open "Driver={Microsoft Access Driver (*.mdb)};" & _ 
　　　　　　　　　 "Dbq=c:/somepath/mydb.mdb;" & _
　　　　　　　　　 "Uid=Admin;" & _
　　　　　　　　　 "Pwd=;"

If you are using a Workgroup (System database):

oConn.Open "Driver={Microsoft Access Driver (*.mdb)};" & _ 
　　　　　　　　　 "Dbq=c:/somepath/mydb.mdb;" & _
　　　　　　　　　 "SystemDB=c:/somepath/mydb.mdw;", _
　　　　　　　　　 "admin", ""

If MDB is located on a network share:

oConn.Open "Driver={Microsoft Access Driver (*.mdb)};" & _
　　　　　　　　　 "Dbq=//myServer/myShare/myPath/myDb.mdb;"

For more information, see: Microsoft Access Driver Programming Considerations

ODBC Driver for dBASE 

oConn.Open "Driver={Microsoft dBASE Driver (*.dbf)};" & _
　　　　　　　　 "DriverID=277;" & _
　　　　　　　　 "Dbq=c:/somepath;"

Note: Specify the filename in the SQL statement. For example:
　　　　 oRs.Open "Select * From user.dbf", oConn, , ,adCmdText

Note: MDAC 2.1 (or greater) requires the Borland Database Engine (BDE) to update dBase DBF files. (Q238431).

For more information, see: dBASE Driver Programming Considerations

ODBC Driver for Excel 

oConn.Open "Driver={Microsoft Excel Driver (*.xls)};" & _
　　　　　　　　　 "DriverId=790;" & _
　　　　　　　　　 "Dbq=c:/somepath/mySpreadsheet.xls;" & _
　　　　　　　　　 "DefaultDir=c:/somepath;" 

For more information, see: Microsoft Excel Driver Programming Considerations

ODBC Driver for MySQL (via MyODBC) 

To connect to a local database

oConn.Open "Driver={mySQL};" & _ 
　　　　　　　　　 "Server=MyServerName;" & _
　　　　　　　　　 "Option=16834;" & _
　　　　　　　　　 "Database=mydb;"

To connect to a remote database

oConn.Open "Driver={mySQL};" & _ 
　　　　　　　　　 "Server=db1.database.com;" & _
　　　　　　　　　 "Port=3306;" & _
　　　　　　　　　 "Option=131072;" & _
　　　　　　　　　 "Stmt=;" & _
　　　　　　　　　 "Database=mydb;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For more information, see: Programs Known to Work with MyODBC

ODBC Driver for Oracle 

For the current Oracle ODBC Driver from Microsoft:

oConn.Open "Driver={Microsoft ODBC for Oracle};" & _
　　　　　　　　　 "Server=OracleServer.world;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For the older Oracle ODBC Driver from Microsoft:

oConn.Open "Driver={Microsoft ODBC Driver for Oracle};" & _
　　　　　　　　　 "ConnectString=OracleServer.world;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For more information, see: Connection String Format and Attributes

ODBC Driver for Paradox 

oConn.Open "Driver={Microsoft Paradox Driver (*.db)};" & _
　　　　　　　　　 "DriverID=538;" & _
　　　　　　　　　 "Fil=Paradox 5.X;" & _
　　　　　　　　　 "DefaultDir=c:/dbpath/;" & _
　　　　　　　　　 "Dbq=c:/dbpath/;" & _
　　　　　　　　　 "CollatingSequence=ASCII;" 

Note: MDAC 2.1 (or greater) requires the Borland Database Engine (BDE) to update Paradox ISAM fDBF files. (Q230126).

For more information, see: Paradox Driver Programming Considerations

ODBC Driver for SQL Server 

For Standard Security:

oConn.Open "Driver={SQL Server};" & _ 
　　　 　　　　　 "Server=MyServerName;" & _
　　　　　　　　 "Database=myDatabaseName;" & _
　　　　　　　　 "Uid=myUsername;" & _
　　 　　　　　　 "Pwd=myPassword;"

For Trusted Connection security:

oConn.Open "Driver={SQL Server};" & _ 
　　　 　　　　　 "Server=MyServerName;" & _
　　　　　　　　　 "Database=myDatabaseName;" & _
　　　　　　　　　 "Uid=;" & _
　　 　　　　　　 "Pwd=;"

' or 

oConn.Open "Driver={SQL Server};" & _ 
　　　 　　　　　 "Server=MyServerName;" & _
　　　　　　　　　 "Database=myDatabaseName;" & _
　　　　　　　　　 "Trusted_Connection=yes;"

To Prompt user for username and password

oConn.Properties("Prompt") = adPromptAlways
oConn.Open "Driver={SQL Server};" & _ 
　　　 　　　　　 "Server=MyServerName;" & _ 
　　　 　　　　　 "DataBase=myDatabaseName;"

For more information, see: SQLDriverConnect (ODBC)

ODBC Driver for Sybase 

If using the Sybase System 11 ODBC Driver:

oConn.Open "Driver={SYBASE SYSTEM 11};" & _
　　　　　　　　　 "Srvr=myServerName;" & _
　　　　　　　　 "Uid=myUsername;" & _
　　 　　　　　　 "Pwd=myPassword;"

If using the Intersolv 3.10 Sybase ODBC Driver:

oConn.Open "Driver={INTERSOLV 3.10 32-BIT Sybase};" & _
　　　　　　　　　 "Srvr=myServerName;" & _
　　　　　　　　 "Uid=myUsername;" & _
　　 　　　　　　 "Pwd=myPassword;"

For more information, see: Sybase System 10 ODBC Driver Reference Guide

ODBC Driver for Sybase SQL Anywhere 

oConn.Open "ODBC; Driver=Sybase SQL Anywhere 5.0;" & _
　　　　　　　　　 "DefaultDir=c:/dbpath/;" & _
　　　　　　　　　 "Dbf=c:/sqlany50/mydb.db;" & _
　　　　　　　　 "Uid=myUsername;" & _
　　 　　　　　　 "Pwd=myPassword;"
　　　　　　　　　 "Dsn="""";"

Note: Including the DSN tag with a null string is absolutely critical or else you get the dreaded -7778 error.

For more information, see: Sybase SQL Anywhere User Guide

ODBC Driver for Teradata 

oConn.Open "Provider=Teradata;" & _
　　　　　　　　　 "DBCName=MyDbcName;" & _ 
　　　　　　　　　 "Database=MyDatabaseName;" & _ 
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For more information, see Teradata ODBC Driver

ODBC Driver for Text 

oConn.Open "Driver={Microsoft Text Driver (*.txt; *.csv)};" & _
　　　　　　　　　 "Dbq=c:/somepath/;" & _
　　　　　　　　　 "Extensions=asc,csv,tab,txt;" & _
　　　　　　　　　 "Persist Security Info=False"

Note: Specify the filename in the SQL statement. For example:

oRs.Open "Select * From customer.csv", _
　　　　　　　 oConn, adOpenStatic, adLockReadOnly, adCmdText

For more information, see: Text File Driver Programming Considerations

ODBC Driver for Visual FoxPro 

With a database container:

oConn.Open "Driver={Microsoft Visual FoxPro Driver};" & _
　　　　　　　　　 "SourceType=DBC;" & _
　　　　　　　　　 "SourceDB=c:/somepath/mySourceDb.dbc;" & _
　　　　　　　　　 "Exclusive=No;" 

Without a database container (Free Table Directory):

oConn.Open "Driver={Microsoft Visual FoxPro Driver};" & _
　　　　　　　　　 "SourceType=DBF;" & _
　　　　　　　　　 "SourceDB=c:/somepath/mySourceDbFolder;" & _
　　　　　　　　　 "Exclusive=No;" 

For more information, see: Visual FoxPro ODBC Driver and Q165492

OLE DB Data Link Connections
Data Link File (UDL) 

For Absolute Path:

oConn.Open "File Name=c:/somepath/myDatabaseName.udl;" 

For Relative Path:

oConn.Open "File Name=myDatabaseName.udl;"

For more information, see: HOWTO: Use Data Link Files with ADO

Note: Windows 2000 no longer contains the "New | Microsoft Data Link" menu　
anymore. You can add the Data Link menu back in the menu list by running the 
"C:/Program Files/Common Files/System/Ole DB/newudl.reg" reg file, 
then right-click on the desktop and select "New | Microsoft Data Link" menu.　 
Or you can also create a Data Link file by creating a text file and change it's 
file extension to ".udl", then double-click the file.

OLE DB Provider Connections
OLE DB Provider for AS/400 

oConn.Open "Provider=IBMDA400;" & _
　　　　　　　　　 "Data source=myAS400;"
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

For more information, see:　 A Fast Path to AS/400 Client/Server

OLE DB Provider for Active Directory Service 

oConn.Open "Provider=ADSDSOObject;" & _
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

For more information, see: Microsoft OLE DB Provider for Microsoft Active Directory Service

OLE DB Provider for DB2 

oConn.Open = "Provider=DB2OLEDB;" & 
　　　　　　　　　　 "Network Transport Library=TCPIP;" & 
　　　　　　　　　　 "Network Address=MyServer;" & _
　　　　　　　　　　 "Package Collection=MyPackage;" & 
　　　　　　　　　　 "Host CCSID=1142"
　　　　　　　　　　 "Initial Catalog=MyDB;" & 
　　　　　　　　　　 "User ID=MyUsername;" & _
　　　　　　　　　　 "Password=MyPassword;"

For more information, see: OLE DB Provider for DB2 
and INF: Configuring Data Sources for the Microsoft OLE DB Provider for DB2 

OLE DB Provider for Index Server 

oConn.Open "Provider=msidxs;" & _
　　　　　　　　　 "Data source=MyCatalog;"

For more information, see: Microsoft OLE DB Provider for Microsoft Indexing Service

OLE DB Provider for Internet Publishing 

oConn.Open "Provider=MSDAIPP.DSO;" & _
　　　　　 "Data Source=http://mywebsite/myDir;" & _ 
　　　　　 "User Id=myUsername;" & _
　　　　　 "Password=myPassword;" 

For more information, see: Microsoft OLE DB Provider for Internet Publishing and Q245359

OLE DB Provider for Microsoft Jet 

For standard security:

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/myDb.mdb;" & _ 
　　　　　　　　　 "User Id=admin;" & _
　　　　　　　　　 "Password=;"

If using a Workgroup (System Database):

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/mydb.mdb;" & _ 
　　　　　　　　　 "Jet OLEDB:System Database=MySystem.mdw;", _
　　　　　　　　　 "admin", "" 

Note, remember to convert both the MDB and the MDW to the 4.0 database format when using the 4.0 OLE DB Provider.

If MDB has a database password:

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/mydb.mdb;" & _ 
　　　　　　　　　 "Jet OLEDB:Database Password=MyDbPassword;", _
　　　　　　　　　 "admin", ""

If MDB is located on a network share:

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=//myServer/myShare/myPath/myDb.mdb;

If want to open up the Access database exclusively:

oConn.Mode = adModeShareExclusive
oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/myDb.mdb;" & _
　　　　　　　　　 "User Id=admin;" & _
　　　　　　　　　 "Password=;"

For more information, see: OLE DB Provider for Microsoft Jet, Q191754, Q225048, Q239114, and Q271908

You can also open an Excel Spreadsheet using the "OLE DB Provider for Microsoft Jet" 

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/myExcelSpreadsheet.xls;" & _
　　　　　　　　　 "Extended Properties=""Excel 8.0;HDR=Yes;"";" 

Where "HDR=Yes" means that there is a header row in the cell range 
(or named range), so the provider will not include the first row of the
selection into the recordset. If "HDR=No", then the provider will include
the first row of the cell range (or named ranged) into the recordset. 

For more information, see: Q278973

You can also open a Text File using the "OLE DB Provider for Microsoft Jet" 

oConn.Open "Provider=Microsoft.Jet.OLEDB.4.0;" & _ 
　　　　　　　　　 "Data Source=c:/somepath/;" & _ 
　　　　　　　　　 "Extended Properties=""text;HDR=Yes;FMT=Delimited;"";" 

' Then open a recordset based on a select on the actual file
oRs.Open "Select * From MyTextFile.txt", oConn, adOpenStatic, adLockReadOnly, adCmdText 

For more information, see: Q262537 

OLE DB Provider for ODBC Databases 

For Access (Jet):

oConn.Open "Provider=MSDASQL;" & _ 
　　　　　　　　　 "Driver={Microsoft Access Driver (*.mdb)};" & _
　　　　　　　　　 "Dbq=c:/somepath/mydb.mdb;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For SQL Server:

oConn.Open "Provider=MSDASQL;" & _ 
　　　　　　　　　 "Driver={SQL Server};" & _
　　　　　　　　　 "Server=myServerName;" & _
　　　　　　　　　 "Database=myDatabaseName;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

For more information, see: Microsoft OLE DB Provider for ODBC

OLE DB Provider for Oracle (from Microsoft) 

oConn.Open "Provider=msdaora;" & _
　　　　　　　　　 "Data Source=MyOracleDB;" & _ 
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

For more information, see: Microsoft OLE DB Provider for Oracle

OLE DB Provider for Oracle (from Oracle) 

For Standard Security:

oConn.Open "Provider=OraOLEDB.Oracle;" & _
　　　　　　　　　 "Data Source=MyOracleDB;" & _ 
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

For a Trusted Connection:

oConn.Open "Provider=OraOLEDB.Oracle;" & _
　　　　　　　　　 "Data Source=MyOracleDB;" & _ 
　　　　　　　　　 "User Id=/;" & _
　　　　　　　　　 "Password=;"
' Or

oConn.Open "Provider=OraOLEDB.Oracle;" & _
　　　　　　　　　 "Data Source=MyOracleDB;" & _ 
　　　　　　　　　 "OSAuthent=1;"

Note: "Data Source=" must be set to the appropriate Net8 name which is known to the naming method in use. For example, for Local Naming, it is the alias in the tnsnames.ora file; for Oracle Names, it is the Net8 Service Name. 

For more information, see: Connecting to an Oracle Database
(Note, if you get a Logon dialog, then click Cancel, then perform a one-time free signup with Oracle's TechNet system)

OLE DB Provider for Simple Provider 

The Microsoft OLE DB Simple Provider (OSP) allows ADO to access any data for which a provider has
been written using the OLE DB Simple Provider Toolkit. Simple providers are intended to access data
sources that require only fundamental OLE DB support, such as in-memory arrays or XML documents.

OSP in MDAC 2.6 has been enhanced to support opening hierarchical ADO Recordsets over arbitrary
XML files. These XML files may contain the ADO XML persistence schema, but it is not required. This
has been implemented by connecting the OSP to the MSXML2.DLL, therefore MSXML2.DLL or newer is
required.

oConn.Open "Provider=MSDAOSP;" & _
　　　　　　　　　 "Data Source=MSXML2.DSOControl.2.6;"

oRS.Open "http://WebServer/VirtualRoot/MyXMLFile.xml", oConn

For more information, see: Microsoft OLE DB Simple Provider

OLE DB Provider for SQL Server 

For Standard Security:

oConn.Open "Provider=sqloledb;" & _ 
　　　　　　　　　 "Data Source=myServerName;" & _
　　　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

For a Trusted Connection:

oConn.Open "Provider=sqloledb;" & _
　　　　　　　　　 "Data Source=myServerName;" & _
　　　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　　 "Integrated Security=SSPI;"

To connect to a "Named Instance" (SQL Server 2000)

oConn.Open "Provider=sqloledb;" & _
　　　　　　　　　 "Data Source=myServerName/Inst2;" & _
　　　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

To Prompt user for username and password:

oConn.Provider = "sqloledb"
oConn.Properties("Prompt") = adPromptAlways
oConn.Open "Data Source=myServerName;" & _
　　　　　　　　　 "Initial Catalog=myDatabaseName;" 

To connect via an IP address:

oConn.Open "Provider=sqloledb;" & _
　　　　　　　　　 "Data Source=xxx.xxx.xxx.xxx,1433;" & _
　　　　　　　　　 "Network Library=DBMSSOCN;" & _
　　　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　　 "User ID=myUsername;" & _
　　　　　　　　　 "Password=myPassword;" 
Note: 
- xxx.xxx.xxx.xxx is an IP address
- "Network Library=DBMSSOCN" tells OLE DB to use TCP/IP rather than Named Pipes (Q238949)
- 1433 is the default port number for SQL Server
- You can also add "Encrypt=yes" for encryption 

For more information, see: Microsoft OLE DB Provider for SQL Server

Remote Data Service (RDS) Connections
The following examples show how to connect to a remote database using the RDS Data Control.
When using the RDS DataControl's Server/SQL/Connect properties, the RDS DataControl uses the 
RDS DataFactory on the remote server. If you use the RDS DataControl's URL property, 
then the RDS DataFactory is not used at all.

WARNING: The RDS DataFactory can be a major security hole if not setup and configured correctly!
For more information, see RDS FAQ #24 

RDS DataControl - Connect Property 

With the RDS default handler disabled (not recommend due to security risks):

With oRdc
　 .Server = "http://carl2/"
　 .Sql = "Select * From Authors Where State = 'CA'"
　 .Connect = "Provider=sqloledb;" & _
　　　　　　　　　　 "Data Source=(local);" & _
　　　　　　　　　　 "Initial Catalog=pubs;" & _
　　　　　　　　　　 "User Id=sa;" & _
　　　　　　　　　　 "Password=;"
　 .Refresh
End With

With the RDS default handler enabled (recommend):

With oRdc
　 .Server = "http://carl2/"
　 .Handler = "MSDFMAP.Handler"
　 .Connect = "Data Source=MyConnectTag;"
　 .Sql = "MySQLTag(""CA"")"
　 .Refresh
End With 

The corresponding CONNECT and SQL sections in the default handler /WINNT/MSDFMAP.INI file would be:

[connect MyConnectTag]
Access = ReadWrite
Connect = "Provider=sqloledb;Data Source=(local);Initial Catalog=pubs;User Id=sa;Password=;" 

[sql MySQLTag]
Sql = "Select * From Authors Where State = '?'"

For more information about the RDS Default Handler, see:
Q243245, Q230680, and RDS Customization Handler Microsoft articles

RDS DataControl - URL Property 

To get records from a remote database:

With oRdc
　 .URL = "http://carlp0/Authors_GetByState.asp?state=CA"
　 .Refresh
End With

To save, set the URL property to an ASP web page:

With oRdc
　 .URL = "http://carlp0/rdsdatacontrol/Authors_Save.asp"
　 .SubmitChanges
End With

For more information, see: RDS URL Property

ADO URL Connections
ADO 2.5+ allows you to open up a Recordset based on XML returned from an ASP file over HTTP.　
This feature doesn't use RDS at all.

ADO Recordset 

To get records from a remote database:

oRs.Open "http://carlp0/Authors_GetByState.asp?state=CA", , _
　　　　　　　　　　　　　　　　　　　　　 adOpenStatic, adLockBatchOptimistic 
To save changes, you must use the MSXML's XMLHTTP object to POST back the updated XML.　
The Recordset's Update and UpdateBatch methods will not work in this case.

' Save Recordset into Stream
Set oStm = New ADODB.Stream
oRs.Save oStm, adPersistXML

' Use MSXML's XMLHTTP object to open ASP and post a XML stream
Set oXMLHTTP = New MSXML2.XMLHTTP30
oXMLHTTP.Open "POST", "http://carlp0/Authors_Save.asp", False
oXMLHTTP.Send oStm.ReadText

' If an error occurred
If oXMLHTTP.Status = 500 Then
　 Debug.Print oXMLHTTP.statusText
End If

For more information, see: ADO Recordset's Open Method

MS Remote Provider Connections
The following connections strings use Microsoft's remote provider (MS Remote). The MS Remote
provider tells ADO to communicate with the remote server (via the RDS DataFactory) and to use 
the remote provider that is installed on the remote server.

WARNING: The RDS DataFactory can be a major security hole if not setup and configured correctly!　
For more information, see RDS FAQ #24 

MS Remote - Access (Jet) 

If you want to use an ODBC DSN on the remote machine:

oConn.Open "Provider=MS Remote;" & _
　　　　　　　　　 "Remote Server=http://myServerName;" & _ 
　　　　　　　　　 "Remote Provider=MSDASQL;" & _
　　　　　　　　　 "DSN=AdvWorks;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;" 

If you want to use an OLE DB Provider on the remote machine:

oConn.Open "Provider=MS Remote;" & _ 
　　　　　　　　　 "Remote Server=http://myServerName;" & _
　　　　　　　　　 "Remote Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　 "Data Source=c:/somepath/mydb.mdb;", _
　　　　　　　　　 "admin", ""

If you want to use an OLE DB Provider on the remote machine (via RDS DataFactory Default Handler):

oConn.Open "Provider=MS Remote;" & _ 
　　　　　　　　　 "Remote Server=http://myServerName;" & _
　　　　　　　　　 "Handler=MSDFMAP.Handler;" & _
　　　　　　　　　 "Data Source=MyAdvworksConn;"

The corresponding entry in the /winnt/Msdfmap.ini file would be:

[connect MyAdvworksConn]
Access = ReadWrite
Connect = "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　 "Data Source=mydb.mdb;" & _
　　　　　　　 "User Id=admin;" & _
　　　　　　　 "Password=;"

MS Remote - SQL Server 

If you want to use an ODBC DSN on the remote machine:

oConn.Open "Provider=MS Remote;" & _
　　　　　　　　　 "Remote Server=http://myServerName;" & _ 
　　　　　　　　　 "Remote Provider=MSDASQL;" & _
　　　　　　　　　 "DSN=myDatabaseName;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;" 

If you want to use an OLE DB Provider on the remote machine:

oConn.Open "Provider=MS Remote;" & _ 
　　　　　　　　　 "Remote Server=http://myServerName;" & _
　　　　　　　　　 "Remote Provider=SQLOLEDB;" & _
　　　　　　　　 "Data Source=myServerName;" & _
　　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　 "User ID=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

If you want to use an OLE DB Provider on the remote machine (via RDS DataFactory Default Handler):

oConn.Open "Provider=MS Remote;" & _ 
　　　　　　　　 　"Remote Server=http://myServerName;" & _ 
　　　　　　　　 　"Handler=MSDFMAP.Handler;" & _
　　　　　　　　 　"Data Source=MyPubsConn;" 

The corresponding entry in the /winnt/Msdfmap.ini file would be:

[connect MyPubsConn]
Access = ReadWrite
Connect = "Provider=SQLOLEDB;" & _
　　　　　　　 "Data Source=myServerName;" & _
　　　　　　　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　 "User ID=myUsername;" & _
　　　　　　　 "Password=myPassword;"

For more information, see: Microsoft OLE DB Remoting Provider　 and Q240838

Data Shape Provider Connections
MS DataShape - SQL Server 

oConn.Open "Provider=MSDataShape;" & _
　　　　　　　　　 "Data Provider=SQLOLEDB;" & _
　　　　　　　　　 "Data Source=mySQLServerName;" & _
　　　　　　　　　 "Initial Catalog=myDatabase;" & _
　　　　　　　　　 "User ID=myUsername;" & _
　　　　　　　　　 "Password=myPassword;"

Then use a Shape command with SQL strings:
sSQL = "SHAPE {select * from authors} " & _
　　　　　 "APPEND ({select * from titleauthor} AS chapter " & _
　　　　　 "RELATE au_id TO au_id)"

Or use a Shape command that calls Stored Procedures:
sSQL = "SHAPE {exec spAuthors_LoadAll} " & _
　　　　　 "APPEND ({exec spTitleAuthor_LoadAll} AS chapter " & _
　　　　　 "RELATE au_id TO au_id)"

For more information, see: Microsoft Data Shaping Service for OLE DB and Q288409 

.NET Managed Provider Connections
SQL Client .NET Managed Provider (System.Data.SqlClient) 

The SQL Client .NET Managed Provide allows you to connect to a Microsoft SQL Server 7.0 
or 2000 database. For Microsoft SQL Server 6.0 or earlier, use the OLE DB .NET Data Provider 
with the "SQL Server OLE DB Provider" (SQLOLEDB).

Dim oSQLConnection As SqlClient.SqlConnection
Dim sConnString As String

sConnString = "Data Source=(local);" & _
　　　　　　　　　　 "Initial Catalog=NorthWind;" & _
　　　　　　　　　　 "Integrated Security=SSPI;" & _
　　　　　　　　　　 "Pooling=True;" & _
　　　　　　　　　　 "Min Pool Size=10;" & _
　　　　　　　　　　 "Max Pool Size=50;" & _
　　　　　　　　　　 "Connection Lifetime=30;" & _
　　　　　　　　　　 "Connection Reset=True;" & _
　　　　　　　　　　 "Enlist=True;"
oSQLConnection = New SqlClient.SqlConnection(sConnString)
oSQLConnection.Open()

For more information, see: System.Data.SQL Namespace and .NET Data Providers
Note: 'SQL' namespace got renamed to 'SQLClient'

OLE DB .NET Managed Provider (System.Data.OleDb) 

The OLE DB .NET Data Provider uses native OLE DB through COM interop to enable data access.　
To use the OLE DB .NET Data Provider, you must also use an OLE DB provider (e.g. SQLOLEDB,
MSDAORA, or Microsoft.JET.OLEDB.4.0).

For SQL Server OLE DB Provider (for SQL Server 6.0 or earlier)

Dim oOleDbConnection As OleDb.OleDbConnection
Dim sConnString As String

sConnString = "Provider=sqloledb;" & _ 
　　　　　　　　　　 "Data Source=myServerName;" & _
　　　　　　　　 　 "Initial Catalog=myDatabaseName;" & _
　　　　　　　　 　 "User Id=myUsername;" & _
　　　　　　　　　 "Password=myPassword;" 
oOleDbConnection = New OleDb.OleDbConnection(sConnString)
oOleDbConnection.Open()

For JET OLE DB Provider:

Dim oOleDbConnection As OleDb.OleDbConnection
Dim sConnString As String

sConnString = "Provider=Microsoft.Jet.OLEDB.4.0;" & _
　　　　　　　　　　 "Data Source=C:/Work/nwind.mdb;" & _
　　　　　　　　　　 "User ID=Admin;" & _
　　　　　　　　　　 "Password="";" 
oOleDbConnection = New OleDb.OleDbConnection(sConnString)
oOleDbConnection.Open()

For more information, see: System.Data.OleDb Namespace and .NET Data Providers
Note: 'ADO' namespace got renamed to 'OleDb'

ODBC .NET Managed Provider (System.Data.ODBC) 

The ODBC .NET Data Provider is an add-on component to the .NET Framework SDK Beta 2. 
It provides access to native ODBC drivers the same way the OLE DB .NET Data Provider 
provides access to native OLE DB providers.

For SQL Server ODBC Driver:

Dim oODBCConnection As Odbc.OdbcConnection
Dim sConnString As String

' Create and open a new ODBC Connection
sConnString = "Driver={SQL Server};" & _
　　　　　　　　　　 "Server=MySQLServerName;" & _
　　　　　　　　　　 "Database=MyDatabaseName;" & _
　　　　　　　　　　 "Uid=MyUsername;" & _
　　　　　　　　　　 "Pwd=MyPassword;"

oODBCConnection = New Odbc.OdbcConnection(sConnString)
oODBCConnection.Open()

For Oracle ODBC Driver:

Dim oODBCConnection As Odbc.OdbcConnection
Dim sConnString As String

' Create and open a new ODBC Connection
sConnString = "Driver={Microsoft ODBC for Oracle};" & _
　　　　　　　　　 "Server=OracleServer.world;" & _
　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　 "Pwd=myPassword;"

oODBCConnection = New Odbc.OdbcConnection(sConnString)
oODBCConnection.Open()

For Access (JET) ODBC Driver:

Dim oODBCConnection As Odbc.OdbcConnection
Dim sConnString As String

' Create and open a new ODBC Connection
sConnString = "Driver={Microsoft Access Driver (*.mdb)};" & _ 
　　　　　　　　　 "Dbq=c:/somepath/mydb.mdb;" & _
　　　　　　　　　 "Uid=Admin;" & _
　　　　　　　　　 "Pwd=;"

oODBCConnection = New Odbc.OdbcConnection(sConnString)
oODBCConnection.Open()

For all other ODBC Drivers:

Dim oODBCConnection As Odbc.OdbcConnection
Dim sConnString As String

' Create and open a new ODBC Connection
sConnString = "Dsn=myDsn;" & _
　　　　　　　　　　 "Uid=myUsername;" & _
　　　　　　　　　　 "Pwd=myPassword;"

oODBCConnection = New Odbc.OdbcConnection(sConnString)
oODBCConnection.Open()

