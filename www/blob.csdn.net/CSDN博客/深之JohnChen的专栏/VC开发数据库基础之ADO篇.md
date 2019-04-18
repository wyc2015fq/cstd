# VC开发数据库基础之ADO篇  - 深之JohnChen的专栏 - CSDN博客

2005年09月06日 11:28:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1698


**VC开发数据库基础之ADO篇  (1)**
一、ADO简介
ADO(ActiveX Data Object)是Microsoft数据库应用程序开发的新接口，是建立在OLE DB之上的高层数据库访问技术，请不必为此担心，即使你对OLE DB，COM不了解也能轻松对付ADO,因为它非常简单易用，甚至比你以往所接触的ODBC API、DAO、RDO都要容易使用，并不失灵活性。本文将详细地介绍在VC下如何使用ADO来进行数据库应用程序开发，并给出示例代码。
本文示例代码 

二、基本流程
万事开头难，任何一种新技术对于初学者来说最重要的还是“入门”，掌握其要点。让我们来看看ADO数据库开发的基本流程吧！
(1)初始化COM库，引入ADO库定义文件
(2)用Connection对象连接数据库
(3)利用建立好的连接，通过Connection、Command对象执行SQL命令，或利用Recordset对象取得结果记录集进行查询、处理。
(4)使用完毕后关闭连接释放对象。

准备工作:
为了大家都能测试本文提供的例子，我们采用Access数据库，您也可以直接在我们提供的示例代码中找到这个test.mdb。
下面我们将详细介绍上述步骤并给出相关代码。
【1】COM库的初始化
我们可以使用AfxOleInit()来初始化COM库，这项工作通常在CWinApp::InitInstance()的重载函数中完成，请看如下代码:

BOOL CADOTest1App::InitInstance()
{
AfxOleInit();
...... 

【2】用#import指令引入ADO类型库
我们在stdafx.h中加入如下语句：(stdafx.h这个文件哪里可以找到?你可以在FileView中的Header Files里找到)
#import "c:/program files/common files/system/ado/msado15.dll" no_namespace rename("EOF","adoEOF")
这一语句有何作用呢？其最终作用同我们熟悉的#include类似,编译的时候系统会为我们生成msado15.tlh,ado15.tli两个C++头文件来定义ADO库。

几点说明:
(1) 您的环境中msado15.dll不一定在这个目录下，请按实际情况修改
(2) 在编译的时候肯能会出现如下警告，对此微软在MSDN中作了说明，并建议我们不要理会这个警告。
msado15.tlh(405) : warning C4146: unary minus operator applied to unsigned type, result still unsigned 

【3】创建Connection对象并连接数据库
首先我们需要添加一个指向Connection对象的指针:
_ConnectionPtr m_pConnection;
下面的代码演示了如何创建Connection对象实例及如何连接数据库并进行异常捕捉。

BOOL CADOTest1Dlg::OnInitDialog()
{
CDialog::OnInitDialog();
HRESULT hr;
try
{
hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
if(SUCCEEDED(hr))
{
hr = m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=test.mdb","","",adModeUnknown);///连接数据库
///上面一句中连接字串中的Provider是针对ACCESS2000环境的，对于ACCESS97,需要改为:Provider=Microsoft.Jet.OLEDB.3.51; }
}
catch(_com_error e)///捕捉异常
{
CString errormessage;
errormessage.Format("连接数据库失败!/r/n错误信息:%s",e.ErrorMessage());
AfxMessageBox(errormessage);///显示错误信息
} 

在这段代码中我们是通过Connection对象的Open方法来进行连接数据库的，下面是该方法的原型
HRESULT Connection15::Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
ConnectionString为连接字串,UserID是用户名, Password是登陆密码,Options是连接选项,用于指定Connection对象对数据的更新许可权,
Options可以是如下几个常量:
adModeUnknown:缺省。当前的许可权未设置
adModeRead:只读
adModeWrite:只写
adModeReadWrite:可以读写
adModeShareDenyRead:阻止其它Connection对象以读权限打开连接
adModeShareDenyWrite:阻止其它Connection对象以写权限打开连接
adModeShareExclusive:阻止其它Connection对象打开连接
adModeShareDenyNone:允许其它程序或对象以任何权限建立连接

我们给出一些常用的连接方式供大家参考:
(1)通过JET数据库引擎对ACCESS2000数据库的连接

m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C://test.mdb","","",adModeUnknown);

(2)通过DSN数据源对任何支持ODBC的数据库进行连接:
m_pConnection->Open("Data Source=adotest;UID=sa;PWD=;","","",adModeUnknown);

(3)不通过DSN对SQL SERVER数据库进行连接： m_pConnection->Open("driver={SQL Server};Server=127.0.0.1;DATABASE=vckbase;UID=sa;PWD=139","","",adModeUnknown);

其中Server是SQL服务器的名称，DATABASE是库的名称

Connection对象除Open方法外还有许多方法，我们先介绍Connection对象中两个有用的属性ConnectionTimeOut与State
ConnectionTimeOut用来设置连接的超时时间，需要在Open之前调用，例如: m_pConnection->ConnectionTimeout = 5;///设置超时时间为5秒
m_pConnection->Open("Data Source=adotest;","","",adModeUnknown);

State属性指明当前Connection对象的状态，0表示关闭，1表示已经打开，我们可以通过读取这个属性来作相应的处理，例如:
if(m_pConnection->State)
m_pConnection->Close(); ///如果已经打开了连接则关闭它

【4】执行SQL命令并取得结果记录集
为了取得结果记录集，我们定义一个指向Recordset对象的指针:_RecordsetPtr m_pRecordset;
并为其创建Recordset对象的实例: m_pRecordset.CreateInstance("ADODB.Recordset");
SQL命令的执行可以采用多种形式，下面我们一进行阐述。

(1)利用Connection对象的Execute方法执行SQL命令
Execute方法的原型如下所示:
_RecordsetPtr Connection15::Execute ( _bstr_t CommandText, VARIANT * RecordsAffected, long Options ) 其中CommandText是命令字串，通常是SQL命令。参数RecordsAffected是操作完成后所影响的行数, 参数Options表示CommandText中内容的类型，Options可以取如下值之一：
adCmdText:表明CommandText是文本命令
adCmdTable:表明CommandText是一个表名
adCmdProc:表明CommandText是一个存储过程
adCmdUnknown:未知

Execute执行完后返回一个指向记录集的指针，下面我们给出具体代码并作说明。 _variant_t RecordsAffected;
///执行SQL命令：CREATE TABLE创建表格users,users包含四个字段:整形ID,字符串username,整形old,日期型birthday
m_pConnection->Execute("CREATE TABLE users(ID INTEGER,username TEXT,old INTEGER,birthday DATETIME)",&RecordsAffected,adCmdText);
///往表格里面添加记录
m_pConnection->Execute("INSERT INTO users(ID,username,old,birthday) valueS (1, ''''''''Washington'''''''',25,''''''''1970/1/1'''''''')",&RecordsAffected,adCmdText);
///将所有记录old字段的值加一
m_pConnection->Execute("UPDATE users SET old = old+1",&RecordsAffected,adCmdText);
///执行SQL统计命令得到包含记录条数的记录集
m_pRecordset = m_pConnection->Execute("SELECT COUNT(*) FROM users",&RecordsAffected,adCmdText);
_variant_t vIndex = (long)0;
_variant_t vCount = m_pRecordset->GetCollect(vIndex);///取得第一个字段的值放入vCount变量
m_pRecordset->Close();///关闭记录集
CString message;
message.Format("共有%d条记录",vCount.lVal);
AfxMessageBox(message);///显示当前记录条数

(2)利用Command对象来执行SQL命令
_CommandPtr m_pCommand;
m_pCommand.CreateInstance("ADODB.Command");
_variant_t vNULL;
vNULL.vt = VT_ERROR;
vNULL.scode = DISP_E_PARAMNOTFOUND;///定义为无参数
m_pCommand->ActiveConnection = m_pConnection;///非常关键的一句，将建立的连接赋值给它
m_pCommand->CommandText = "SELECT * FROM users";///命令字串
m_pRecordset = m_pCommand->Execute(&vNULL,&vNULL,adCmdText);///执行命令，取得记录集

在这段代码中我们只是用Command对象来执行了SELECT查询语句，Command对象在进行存储过程的调用中能真正体现它的作用。下次我们将详细介绍。 

(3)直接用Recordset对象进行查询取得记录集 
例如 

m_pRecordset->Open("SELECT * FROM users",_variant_t((IDispatch *)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);

Open方法的原型是这样的:
HRESULT Recordset15::Open ( const _variant_t & Source, const _variant_t & ActiveConnection, enum CursorTypeEnum CursorType, enum LockTypeEnum LockType, long Options ) 
其中：
①Source是数据查询字符串
②ActiveConnection是已经建立好的连接（我们需要用Connection对象指针来构造一个_variant_t对象) 
③CursorType光标类型，它可以是以下值之一,请看这个枚举结构:
enum CursorTypeEnum
{
adOpenUnspecified = -1,///不作特别指定
adOpenForwardOnly = 0,///前滚静态光标。这种光标只能向前浏览记录集，比如用MoveNext向前滚动,这种方式可以提高浏览速度。但诸如BookMark,RecordCount,AbsolutePosition,AbsolutePage都不能使用
adOpenKeyset = 1,///采用这种光标的记录集看不到其它用户的新增、删除操作，但对于更新原有记录的操作对你是可见的。
adOpenDynamic = 2,///动态光标。所有数据库的操作都会立即在各用户记录集上反应出来。
adOpenStatic = 3///静态光标。它为你的记录集产生一个静态备份，但其它用户的新增、删除、更新操作对你的记录集来说是不可见的。
};
④LockType锁定类型，它可以是以下值之一，请看如下枚举结构：
enum LockTypeEnum
{
adLockUnspecified = -1,///未指定
adLockReadOnly = 1,///只读记录集
adLockPessimistic = 2,悲观锁定方式。数据在更新时锁定其它所有动作，这是最安全的锁定机制
adLockOptimistic = 3,乐观锁定方式。只有在你调用Update方法时才锁定记录。在此之前仍然可以做数据的更新、插入、删除等动作
adLockBatchOptimistic = 4，乐观分批更新。编辑时记录不会锁定，更改、插入及删除是在批处理模式下完成。
}; 
⑤Options请参考本文中对Connection对象的Execute方法的介绍

【5】记录集的遍历、更新
根据我们刚才通过执行SQL命令建立好的users表，它包含四个字段:ID,username,old,birthday
以下的代码实现：打开记录集，遍历所有记录，删除第一条记录，添加三条记录，移动光标到第二条记录，更改其年龄，保存到数据库。

_variant_t vUsername,vBirthday,vID,vOld;
_RecordsetPtr m_pRecordset;
m_pRecordset.CreateInstance("ADODB.Recordset");
m_pRecordset->Open("SELECT * FROM users",_variant_t((IDispatch*)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
while(!m_pRecordset->adoEOF)///这里为什么是adoEOF而不是EOF呢?还记得rename("EOF","adoEOF")这一句吗?
{
vID = m_pRecordset->GetCollect(_variant_t((long)0));///取得第1列的值,从0开始计数，你也可以直接给出列的名称，如下一行
vUsername = m_pRecordset->GetCollect("username");///取得username字段的值
vOld = m_pRecordset->GetCollect("old");
vBirthday = m_pRecordset->GetCollect("birthday");
///在DEBUG方式下的OUTPUT窗口输出记录集中的记录
if(vID.vt != VT_NULL && vUsername.vt != VT_NULL && vOld.vt != VT_NULL && vBirthday.vt != VT_NULL)
TRACE("id:%d,姓名:%s,年龄:%d,生日:%s/r/n",vID.lVal,(LPCTSTR)(_bstr_t)vUsername,vOld.lVal,(LPCTSTR)(_bstr_t)vBirthday);
m_pRecordset->MoveNext();///移到下一条记录
}
m_pRecordset->MoveFirst();///移到首条记录
m_pRecordset->Delete(adAffectCurrent);///删除当前记录
///添加三条新记录并赋值
for(int i=0;i<3;i++)
{
m_pRecordset->AddNew();///添加新记录
m_pRecordset->PutCollect("ID",_variant_t((long)(i+10)));
m_pRecordset->PutCollect("username",_variant_t("叶利钦"));
m_pRecordset->PutCollect("old",_variant_t((long)71));
m_pRecordset->PutCollect("birthday",_variant_t("1930-3-15"));
}
m_pRecordset->Move(1,_variant_t((long)adBookmarkFirst));///从第一条记录往下移动一条记录,即移动到第二条记录处
m_pRecordset->PutCollect(_variant_t("old"),_variant_t((long)45));///修改其年龄
m_pRecordset->Update();///保存到库中

【6】关闭记录集与连接 
记录集或连接都可以用Close方法来关闭
m_pRecordset->Close();///关闭记录集
m_pConnection->Close();///关闭连接

至此，我想您已经熟悉了ADO操作数据库的大致流程，也许您已经胸有成竹，也许您还有点胡涂，不要紧！建议你尝试写几个例子，这样会更好地熟悉ADO,最后我给大家写了一个小例子，例子中读出所有记录放到列表控件中、并可以添加、删除、修改记录。
点这里下载示例代码

后记：限于篇幅ADO中的许多内容还没有介绍，下次我们将详细介绍Recordset对象的属性、方法并解决几个关键的技术：绑定方式处理记录集数据、存储过程的调用、事务处理、图象在数据库中的保存与读取、与表格控件的配合使用等 

