# 用Visual C++开发数据库应用程序(五) - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2154


用Visual C++开发数据库应用程序(五)

3．27 执行SQL语句

虽然通过CRecordSet类，我们可以完成 大多数的查询操作，而且在CRecordSet::Open()函数中也可以 提供SQL语句，但是有的时候我们还想进行一些其他操 作，例如建立新表，删除表，建立新的字段等等，这 时就需要使用到CDatabase类的直接执行SQL语句的机制。通 过调用CDatabase::ExecuteSQL()函数来完成SQL语句的直接执行：

如下代码所示

BOOL CDB::ExecuteSQLAndReportFailure(const CString& strSQL)

{

TRY

{

m_pdb->ExecuteSQL(strSQL); //直接执行SQL语句

}

CATCH (CDBException,e)

{

CString strMsg;

strMsg.LoadString(IDS_EXECUTE_SQL_FAILED);

strMsg+=strSQL;

return FALSE;

}

END_CATCH 

return TRUE;

}

应当指出的是，由于不同DBMS提 供的数据操作语句不尽相同，直接执行SQL语句可能会破坏软件的DBMS无关性，因此在应用中应当慎用此类操作。

3．28 注意

从CRecordSet导出的类中如果包含DataTime类型的数据，在VC中是用CTime类型来替代的，这时，构造函数没有赋于缺省值。这时，我们应当手工赋值。如下所示:

CTime m_time;

m_time=NULL;

3．3 总结

VisualC++中的ODBC类库可以帮助程序员完成绝大多数的数据库操作。利用ODBC技术使得程序员从具体的DBMS中解脱出来，从而极大的减少了软件开发的工作量，缩短开发周期，提高了效率和软件的可靠性。

4、使用DAO 

4．1 概述

Visual C++提供了对DAO的封装，MFC DAO类封装了DAO（数据库访问对象）的大部分功能，从面Visual C++程序就可以使用Visual C++提供的MFC DAO类方便的访问Microsoft Jet 数据库，编制简洁、有Visaul C++特色的数据库应用程序。

数据库访问对象（DAO）提供了一种通过程序代码创建和操纵数据库的机制。多个DAO对象构成一个体系结构，在这个结构里，各个DAO对象协同工作。DAO支持以下四个数据库选项：

打开访问数据库（MDB文件）——MDB文件是一个自包含的数据库，它包括查询定义、安全信息、索引、关系，当然还有实际的数据表。用户只须指定MDB文件的路径名。 

直接打开ODBC数据源——这里有一个很重要的限制。不能找开以Jet引擎作为驱动程序的ODBC数据源；只可以使用具有自己的ODBC驱动程序DLL的数据源。 

用Jet引擎找开ISAM型（索引顺序访问方法）数据源（包括dBase,FoxPro,Paradox,Btrieve,Excel或文本文件）——即使已经设置了ODBC数据源，要用Jet引擎来访问这些文件类型中的一种，也必须以ISAM型数据源的方式来找开文件，而不是以ODBC数据源的方式。 

给ACCESS数据库附加外部表——这实际上是用DAO访问ODBC数据源的首选方法。首先使用ACCESS把ODBC表添加到一个MDB文件上，然后依照第一选项中介绍的方法用DAO找开这个MDB文件就可以了。用户也可以用ACCESS把IASM文件附加到一个MDB文件上。 

4．2 应用DAO编程

4．21 打开数据库

CDaoWorkspace对象代表一个DAO Workspace对象，在MFC DAO体系结构中处于最高处，定义了一个用户的同数据库的会话，并包含打开的数据库，负责完成数据库的事务处理。我们可以使用隐含的workspace对象。

CDaoDatabase对象代表了一个到数据库的连接，在MFC中，是通过CDaoDatabase封装的。

在构造CDaoDatabase对象时，有如下两种方法:

创建一个CDaoDatabase对象，并向其传递一个指向一个已经找开的CdaoWorkspace对象的指针。 

创建一个CDaoDatabase对象，而不明确地指定使用的workspace,此时，MFC将创建一个新的临时的CDaoWorkspace对象。 

如下代码所示：

CDaoDatabase db;

db.Open(“test.mdb”,FALSE,FALSE,_T(“”);

其中参数一包括要打开的文件的全路径名。

4．22 查询记录

一个DAO recordset对象，代表一个数据记录的集合，该集合是一个库表或者是一个查询的运行结果中的全部记录。CDaoRecorset对象有三种类型：表、动态集、快照。

通常情况下，我们在应用程序中可以使用CDaoRecordset的导出类，这一般是通过ClassWizard或AppWizard来生成的。但我们也可以直接使用CDaoRecordset类生成的对象。此时，我们可以动态地绑定recordset对象的数据成员。

如下代码所示：

COleVariant var;

long id;

CString str;

CDaoRecordset m_Set(&db);

m_Set.Open(“查询的SQL语句”);

while(!m_Set.IsEOF())

{

/*

处理

m_Set.GetFieldValue(“ID”,var);

id=V_I4(var);

m_Set.GetFieldValue(“Name”,var);

str=var.pbVal;

*/

m_Set.MoveNext();

}

m_Set.Close();

4．23 添加记录

添加记录用AddNew函数，此时用SetFieldValue来进行赋值。

如下代码所示:

m_pDaoRecordset->AddNew ();

sprintf(strValue,"%s",>m_UserName );

m_pDaoRecordset->SetFieldValue ("UserName",strValue);

sprintf(strValue,"%d",m_PointId );

m_pDaoRecordset->SetFieldValue ("PointId",strValue);

dataSrc.SetDateTime (m_UpdateTime .GetYear ),m_UpdateTime .GetMonth ),m_UpdateTime .GetDay (),

m_UpdateTime .GetHour (),m_UpdateTime .GetMinute (),m_UpdateTime .GetSecond ());

valValue=dataSrc;

m_pDaoRecordset->SetFieldValue ("UpdateTime",valValue);

sprintf(strValue,"%f",m_pRecordset->m_OldValue );

m_pDaoRecordset->SetFieldValue ("OldValue",strValue);

sprintf(strValue,"%f",m_pRecordset->m_NewValue );

m_pDaoRecordset->SetFieldValue ("NewValue",strValue);

m_pDaoRecordset->Update ();

此时，要注意，日期时间型数据要用SetDataTime函数来赋值,这里面要用到COleVariant类型数据，具体用法可以参考有关帮助。

4．24 修改记录

修改记录用Edit()函数，把记录定位到要修改的位置，调用Edit函数，修改完成后，调用Update函数。

如下代码所示：

m_Set.Edit();

m_Set.SetFieldValue(“列名”,”字符串”);

m_Set.Update();

4．25 删除记录

删除记录用Delete()函数，使用后不需调用Update()函数。

4．26 统计记录

可以使用如下代码来统计记录数：

COleVariant varValue;

CDaoRecordset m_Set(&db);

m_Set.Open(dbOpenDynaset,”SQL语句”);

varValue=m_Set.GetFieldValue(0);

m_lMaxCount=V_I4(&varValue);

m_Set.Close();

如果是统计一张表中总记录，可以使用CDaoTableDef对象，如下代码所示：

CDaoTableDef m_Set(&gUseDB);

Count=m_Set.GetRecordCount();

m_Set.Close();

不能用CDaoRecordset对象的GetRecordCount()来取得记录数。

4．3 总结

使用DAO技术可以便我们方便的访问Microsoft Jet引擎数据库，由于Microsoft Jet不支持多线程，因此，必须限制调用到应用程序主线程的所有DAO。

