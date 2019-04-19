# VC++/MFC学习笔记(五) - SHINING的博客 - CSDN博客
2008年07月21日 17:25:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：2703
 ADO技术介绍
ADO是Microsoft最新和最强大的数据访问接口OLE DB而设计的，是一个便于使用的应用程序层。OLE DB为任何数据源都提供了高性能的访问，这些数据源包括关系和非关系数据库、电子邮件、文件系统、文本和图形以及自定义业务对象等。ADO在关键的Internet方案中使用最少的网络流量，并且在前端和数据源之间使用最少的层数，所有这些都是为了提供轻量、高性能的接口。同时ADO使用与DAO相似的约定和特性，使得它更易于学习。这里先介绍一下ADO技术的历史和ADO访问数据源的特点，然后介绍访问方法。
1.ADO的历史回顾
ADO共发布了1.0、1.5和2.0三个版本。
第一个版本1.0是RDO的一个功能子集，它的目标是为了帮助开发人员在IIS（Internet Informatioin Server）上建立ASP（Active Server Pages）应用。
第二个版本1.5是随IIS4.0和Internet Explorer4.0一起发布的。它也被包含在MDAC（Microsoft Data Access Components）里。从这个版本开始，ADO成为了功能和运作效率上都高出RDO和DAO一筹的数据库界面。
最新的版本2.0加入了别的数据库客户技术从没有的新技术。ADO2.0实际上是基于MSADO15.DLL这个动态链接库的，这个库文件的名字虽然和ADO1.5的一样，但是它实现了更新的接口。ADO2.0的新技术有：
1）异步操作和事件模型。
2）数据集的持续性。
3）层次化的数据运输。
2.ADO特点概述
用ADO访问数据源的特点可概况如下：
第一，易于使用，可以说这是ADO最重要的特点之一。由于ADO是最高成数据库访问技术，所以相对于ODBC来说，具有面向对象的特点。同时，在ADO对象结构中，对象与对象之间的层次结构不是非常明显，这会给编写数据库程序带来很多便利，比如，在应用程序中如果要使用记录集对象，就不一定要先建立连接、会话对象，如果需要就可以直接构造记录集对象。总之，已经没有必要去关心对象的构造层次和构造顺序了。
第二，可以访问多种数据源。这一点和OLE DB一样的，使应用程序具有很好的通用性和灵活性。
第三，访问数据源效率高。这是由于ADO本身就是基于OLE DB的接口，自然具有OLD DB的特点。
第四，方便地Web应用。ADO可以以ActiveX控件的形式出现，这就大大方便了Web应用程序的编制。
第五，技术编程接口丰富。ADO支持Visual C++、Visual Basic、Visual J++以及VBScript和JavaScript等脚本语言。
3.ADO的对象
ADO包含了连接对象（Connection）、命令对象（Command）、记录集对象（RecordSet）、字段对象（Field）、参数对象（Parameter）、错误对象（Error）、属性对象（Property）和集合与事件等。
1）连接（Connection）
用于表示和数据源的连接，以及处理一些命令和事务。通过它可以从应用程序访问数据源，是交换数据必需的环境。
2）命令（Command）
用于执行某些命令来进行诸如查询、修改数据库结构的操作。
3）记录集（RecordSet）
用于和处理数据源的表格表，它是在表中修改、检索数据的最主要的方法。
4）字段（Field）
用于描述数据集中的列信息。
5）参数（Parameter）
用于对传递给数据源的命令赋参数值。
6）错误（Error）
用于承载说产生错误的详细信息。
7）属性（Property）
通过属性，每个ADO对象借此来让用户描述和控制自身的行为。
8）集合（Set）
集合是一种可方便地包含其他特殊类型对象的对象类型。ADO提供了4种类型的集合。
a、Connention对象具有Errors集合
b、Command对象具有Parameters集合
c、Recordset对象具有Fields集合
d、Connection、Command、Recordset、Field对象都具有Properties集合
9）事件（Event）
事件模型是异步操作的基础，这是ADO2.0引进的新特性。
4.在Visual C++中使用ADO
为在Visual C++中使用ADO，需要在头文件中加入下面几行代码：
#import "C:/Program Files/Common Files/system/ado/msado15.dll" no_namespace rename("EOF","adoEOF")
#include <adoid.h>
#include <adoint.h>
#include "icrsint.h"
第一行的#import语句告诉编译器把此指令中的动态连接库文件引入到程序中，并从库中抽取其中的对象的类的信息，并产生出两个头文件包含在工程中。其中的no_namespace用来对动态链接库的名称域进行隔离。最后的rename，把ADO中的EOF重新命名，避免和其他地方定义的EOF产生冲突。
第二、第三行引入的头文件定义了ADO2.0类和接口标识。
第四行引入了ADO2.0数据绑定扩展。
5.到数据源的连接
Connection对象用来实现和数据源的连接。创建一个Connection对象非常简单，只需声明一个_ConnectionPtr型的指针，调用它的CreateInstance方法即可（_ConnectionPtr是所谓的智能指针，包装了一个Connection接口指针，具体技术细节可以参考有关的COM书籍）。
_ConnectionPtr pConn=NULL;
try
{
    //创建Connection对象实例
    pConn.CreateInstance(__uuidof(Connection));
    _bstr_t conn="DSN=SomeDS;UID=sa;PWD=123";
    //打开连接
    pConn->Open(conn,"","",-1);
    //执行一条SQL命令
    _bstr_t sql="DELETE * FROM some_table WHERE id='111'";
    pConn->Close();
}
catch(_com_error &e)
{
    AfxMessageBox(e.ErrorMessage());
}
上边的代码演示了用Connection对象建立连接，执行一条SQL语句后关闭连接的全过程。
Open函数的原型如下：
HRESULT Open(_bstr_t ConnectionString,
    _bstr_t UID,
    _bstr_T PSWD,
    long option);
各个参数的意义为：
a、ConnectionString：包含连接信息的字符串
b、UID：访问数据库的用户名。
c、PSWD：访问数据库的口令。
d、option：可选参数。
在用Open函数建立连接之前，还可以先设置一些Connection对象的属性。其中用的有ConnectionTimeOut属性和Mode属性。前者用来设置建立连接时的等待时间，后者用来设置数据库的打开模式。打开模式可以是下列值的组合。
a、adModeRead：读模式。
b、adModeWrite：写模式。
c、adModeReadWrite：读写模式。
d、adModeShareDenyNone：防止用户用任何权限打开连接。
e、adModeShareDenyRead：防止用户以读权限打开连接。
f、adModeShareDenyWrite：防止用户以写权限打开连接。
g、adModeShareExclusive：防止其他用户打开连接。
h、adModeUnknown：未知模式。
6.使用记录集
记录集对象是ADO中最常用的对象，以后下面将详细地介绍它的功能。
首先是Recordset对象的创建函数Open，它的原型如下：
HRESULT Open(const_variant_t &source,
    const_variant & connection,
    enum CursorTypeenum cursorType,
    enum LockTypeEnum lockType,
    long options);
对其参数解释如下：
source参数是一个变体类型，它可以是一个Command对象、一个SQL语句、一个表名或一个存储过程，甚至可以是一个URL、一个文件名、一个流对象。
connection也是一个变体类型，它可以是一个Connection对象，也可以是一个指明连接目标的字符串。
cursorType指明了数据集游标的类型。它可以是下列几个值：adOpenDynamic，adOpenForwardOnly，adOpenKeyset，adOpenStatic，adOpenUnspecified。
lockType参数可以是下列几个值之一：adLockBatchOptimistic，adLockOptimistic，adLockPessimistic，adLockReadOnly，adLockUnspecified。
options参数指明了第一个参数source的类型，其值可以是adCmdUnspecified，adCmdText，adCmdTable，adCmdStoredProc，adCmdUnkown，adCmdFile，adCmdTableDirect。
Recordset最常用的3个方法涉及到数据库的增加、更改和删除操作。下面的代码解释了他们的用法。
删除函数Delete的用法。
HRESULT hr;
_bstr_t bstrQuery("SELECT * FROM Products WHERE PartNumber='8TRACK-003'");
_variant_t vNull;
vNull.vt=VT_ERROR;
vNull.scode=DISP_E_PARAMNOTFOUND;
try
{
    hr=pRecordSet.CreateInstance(__uuidof(Recordset));
    if(SUCCEEDED(hr))
    {
        //这里假设Connection对象pConnection已经初始化。
        pRecordSet->PutRefActiveConnectin(pConnection);
        hr=pRecordSet->Open(_variant_t(bstrQuery),vNUll,
            adOpenForwardOnly,adLockOptimistic,adCmdText);
        if(!pRecordSet->GetadoEOF())
        {
            //删除当前游标所指的记录
            pRecordSet->Delete(adAffectCurrent);
            pRecordSet->Close();
        }
    }
}
catch(_com_error &e)
{
    //异常处理部分，省略
}
下面的代码演示了修改函数Update的用法。
_RecordsetPtr pRcordSet;
HRESULT hr;
_bstr_t bstrQuery("SELELCT * FROM Products WHERE PartNumber='8TRACK-003'");
_variant_t vNull;
vNull.vt=VT_ERROR;
vNull.scode=DISP_E_PARAMNOTFOUND;
try
{
    hr=pRecordSet.CreateInstance(__uuidof(Recordset));
    if(SUCCEEDED(hr))
    {
        //这里假设Connection对象pConnection已经初始化。
        pRecordSet->PutRefActiveConnectin(pConnection);
        hr=pRecordSet->Open(_variant_t(bstrQuery),vNUll,
            adOpenForwardOnly,adLockOptimistic,adCmdText);
        if(!pRecordSet->GetadoEOF())
        {
            pRecordSet->PutCollect(L"ProductName",L"Bell Botooms and Bass Guitar");
            //执行更新操作
            pRecordSet->Update(vNull,vNull);
            pRecordSet->Close();
        }
    }
}
catch(_com_error &e)
{
    //异常处理部分，省略
}
下面的代码演示了AddNew函数的用法。
HRESULT hr;
_bstr_t bstrQuery("SELELCT * FROM Products WHERE PartNumber= IS NULL");
_variant_t vNull;
vNull.vt=VT_ERROR;
vNull.scode=DISP_E_PARAMNOTFOUND;
try
{
    hr=pRecordSet.CreateInstance(__uuidof(Recordset));
    if(SUCCEEDED(hr))
    {
        //这里假设Connection对象pConnection已经初始化。
        pRecordSet->PutRefActiveConnectin(pDoc->pConnection);
        hr=pRecordSet->Open(_variant_t(bstrQuery),vNUll,
            adOpenForwardOnly,adLockOptimistic,adCmdText);
        if(SUCCEEDED(h))
        {
            //创建一个记录字段信息的数组
            COleSafeArray vaFieldlist;
            vaFieldlist.CreateOneDim(VT_VARIANT,3);
            long lArrayIndex[1];
            lArrayIndex[0]=0;
            vaFieldlist.PutElement(lArrayIndex,&(_variatn_t("PartNumber")));
            lArrayIndex[0]=1;
            vaFieldlist.PutElement(lArrayIndex,&(_variatn_t("ProductName")));
            lArrayIndex[0]=2;
            vaFieldlist.PutElement(lArrayIndex,&(_variatn_t("Price")));
            //创建一个保存字段值的数组
            COleSafeArry vaValuelist;
            vaValuelist.CreateOneDim(VT_VARIANT,3);
            lArrayIndex[0]=0;
            vaValuelist.PutElement(lArrayIndex,&(_variatn_t("8TRACK-003")));
            lArrayIndex[0]=1;
            vaValuelist.PutElement(lArrayIndex,&(_variatn_t("Bell Botoom Hits")));
            lArrayIndex[0]=2;
            vaValuelist.PutElement(lArrayIndex,&(_variatn_t((float)19.95)));
            //执行添加操作
            pRecordSet->AddNew(vaFieldlist,vaValuelist);
            pRecordSet->Close();
        }
    }
}
catch(_com_error &e)
{
    //异常处理部分，省略
}
从上面的示例代码看出，AddNew和Update方法的参数都是两个变体类型的数组。这种参数在编程的时候比较麻烦，需要编写从VARIANT类型到C++类型的转换代码。而且，使用VARIANT类型检索C/C++数据的过程也有损性能。ADO For VC++ Extensions引入了数据绑定技术，解决了上述的问题。它能把数据直接绑定到C++类型上；通过提供简化接口使用的宏，使它成为一个灵活、易用、高效的工具。
为了绑定一个数据库里的表格到一个C++对象，要求这个C++对象必须继承CADORecordBinding类。通过宏，使我们能很容易地构造出一个这样的对象。例子代码如下：
class CCPRs : public CADORecordBinding
{
    BEGIN_ADO_BINDING(CCPRs)
        ADO_VARIABLE_LENGTH_ENTRY2(1,adVarChar,m_sz_no,
            sizeof(m_sz_no),m_sts_no,TRUE)
        ADO_VARIABLE_LENGTH_ENTRY2(2,adVarChar,m_sz_name,
            sizeof(m_sz_name),m_sts_name,TRUE)
        ADO_VARIABLE_LENGTH_ENTRY2(3,adVarChar,m_sz_kind,
            sizeof(m_sz_kind),m_sts_kind,TRUE)
        ADO_VARIABLE_LENGTH_ENTRY2(4,adSingle,m_f_price,
            sizeof(m_f_price),m_sts_price,TRUE)
        ADO_VARIABLE_LENGTH_ENTRY2(5,adVarChar,m_sz_detail,
            sizeof(m_sz_detail),m_sts_detail,TRUE)
    END_ADO_BINGDING()
public:
    ULONG m_sts_no;
    ULONG m_sts_name;
    ULONG m_sts_kind;
    ULONG m_sts_price;
    ULONG m_sts_detail;
    CHAR m_sz_no[10];
    CHAR m_sz_name[20];
    CHAR m_sz_kind[202];
    float m_f_price;
    CHAR m_sz_detail[50];
};
由于宏的帮助，这种类的定义是相当简单而清晰的。数据绑定写在BEGIN_ADO_BINGDING和END_ADO_BINDING之间。
ADO_VARIABLE_LENGTH_ENTRY2宏的第一个参数指明了字段在表中的顺序，该顺序不能填错，否则运行时会出现错误。这个宏的第二个参数指明了说绑定的字段的类型。有趣的是，adVarChar可以应付大部分类型，哪怕是日期或整数。浮点数这样的类型。当然，为了提高效率和方便编程，还是设置成正确的类型为好。第四个参数用来刻画字段的状态--对每一个字段，还要定义与之对应的参整数变量来记录此字段的状态。最后一个参数指明了此字段是否可写，如果是FALSE，说明是只读的字段。
下面的例子演示了使用这种方法读取表格，并把其中的price字段加倍保存回数据库中。其中数据绑定的部分见上例。
_RecordsetPtr pRst=NULL;
IADORecordBinding *picRs=NULL;
CCPRs rs;
try
{
    _bstr_t strSQL="SELECT * FROM CP";
    TESTHR(pRst.CreateInstrance(__uuidof(Recordset)));
    pRst=m_DBCnt->Execute(strSQL,NULL,adCmdText);
    TESTHR(pRst->QueryInterface(__uuidof(IADORecordBinding),(LPVOID*)&picRs));
    TESTHR(picRs->BindToRecordset(&rs));
    while(!pRst->adoEOF)
    {
        //打印出读取的记录
        printf("%s/n",rs.m_sz_no);
        printf("%s/n",rs.m_sz_name);
        printf("%s/n",rs.m_sz_kind);
        printf("%f/n",rs.m_sz_price);
        printf("%s/n",rs.m_sz_detail);
        //价格翻倍后保存
        rs.m_f_price*=2;
        picRs->Update(&rs);
        //游标移动到下一个记录
        pRst->MoveNext();
    }
    picRs->Release();
    pRst->Close();
}
catch(_com_err &e)
{
    AfxMessageBox(e.ErrorMessage());
    return;
}
7.使用命令
在ADO中，可以用Connection对象的Execute()方法执行命令，也可以用Command对象。
第一条路径相对简单。Execute函数的原型如下：
_RecordsetPtr Execute(_bstr_t cmd,
    VARIANT* rcds,
    long optn);
第一个参数cmd用一个字符串作为参数，第三个参数说明了cmd的类型：
adCmdText：cmd是一个SQL命令。
adCmdTable：cmd指明了一个表名。
adCmdStoredProc：cmd指明了一个存储过程。
Execute返回的结果是一个Recordset指针。下面是一段Execute的简单例子：
_bstr_t sql="SELECT id FROM tb1 WHERE id='Tom' AND pswd='111'";
_RecordsetPtr pRst;
pRst=pDBCnt->Execute(sql,NULL,adCmdText);
if(!pRst->adoEOF)
        Message("登录成功! ");
第二条路径使用的Command对象相对复杂，需要设置很多属性和参数，因而功能也更为强大。这里通过一段示例来解释它的用法。
HRESULT hr=S_OK;
//定义字符串常量
_bstr_t strSQLChange("UPDATE Titles SET Type= "
    "'self_help' WHERE Type='psychology'");
_bstr_t strSQLChange("UPDATE Titles SET Type= "
    "'psychology' WHERE Type='self_help'");
_bstr_t strCnn("Provider=sqloledb;Data Source=MyServer; "
    "Initial Catalog=pubs;User Id=sa;Password=;");
//定义ADO对象
_ConnectioinPtr pConnection=NULL;
_CommandPtr pCmdChange=NULL;
_RecordsetPtr pRstTitle=NULL;
try
{
    //建立连接
    TESTHR(pConnection.CreateInstance(__uuidof(Connection)));
    pConnection->Open(strCnn,"","",adConnectionUnspecified);
    //创建命令对象
    TESTHR(pCmdChange.CreateInstance(__uuidof(Command)));
    pCmdChange->ActiveConnection=pConnection;
    pCmdChange->CommandText=strSQLChagne;
    //打开titles表
    TESTHR(pRstTitles.CreateInstance(__uuidof(Recordset)));
    pRstTitles->Open("Title",_variant_t((IDispatch*)pConnection,
        true),adOpenStatic,adLockOptimistic,adCmdTable);
    //打印出原有的数据
    printf("/n/nData in Titles table before executing the query:/n");
    //这里省略这个函数的实现
    PrintOutput(pRstTitles);
    //清除原有错误记录
    pConnection->Errors->Clear();
    //执行命令
    pCmdChange->Execute(NULL,NULL,adCmdText);
    pRstTitles->Requery(adCmdUnknown);
    //输出新数据
    printf("/n/n/tData in Titles table after executing the query:/n");
    PrintOutput(pRstTitles);
    //恢复记录数据
    pConnection->Execute(strSQLRestore,NULL,adExecuteNoRecords);
    pRstTitles->Requery(adCmdUnknown);
    printf("/n/n/tData after exec. query to restore original info:/n");
    PrintOutput(pRstTitles);
    //结束程序，释放对象
    pRstTitles->Close();
    pConnection->Close();
}
catch(_com_error &e)
{
    //异常处理部分，省略
}
8.使用事务
在数据库中，事务的概念可以把多个操作作为单一的最基本的活动来进行。例如，在一个银行数据库中，需要从一个账户取钱汇入另一个账户中。这里涉及了至少两个操作：减少A的账户，增加B的账户。如果第一个操作顺利完成而第二个操作不幸失败的化，数据库数据的完整性将受到破坏--操作前后银行里的钱的总数会变少，说这里就要用到ADO中事务处理的概念。在所有操作开始之前调用Connection对象的Begintrans方法来开始一个事务：
pDBCnt->BeginTrans();
在所有操作成功之后，调用Connection对象的CommitTrans方法提交此事务，这时数据库的内容菜作实质新的改变：
pDBCnt->CommitTrans();
如果操作中途出现异常，则在异常处理中使用RollBackTrans取消这次事务，数据库将回到BeginTrans之前的状态。
catch(_com_error &e)
{
    AfxMessageBox(e.ErrorMessage());
    pDBCnt->RollBackTrans();
}
9.使用ADO事件
ADO从2.0版本开始加入了对事件的支持。ADO事件是由某些操作在开始之前或结束之后发出的通知；所谓通知，实质上是对预定义的时间处理回调函数的调用。
ADO事件分两类：ConnectionEvent和RecordsetEvent。前者出现在连接（Connection）打开、切断，事务（Transaction）开始、提交或命令（Command）被执行等与Connection对象有关的操作处；后者则出现在与记录集（Recordset）对象有关的操作处，如在记录集中定位，修改记录、删除记录集行等。
若按时间性质来分，ADO事件又可以分为Will事件、Complete事件和其他事件3类。顾名思义，Will事件发生在某个操作之前，Complete 事件发生在某个操作完成之后。
表1和表2是一些典型的ADO事件
表1：                                    ConnectionEvent
---------------------------------------------------------------------------------------------
        连接事件（ConnectionEvent）                |                说明
---------------------------------------------------------------------------------------------
BeginTransComplete                                |事务管理有关的事件
CommitTransComplete                                |
RollbackTransComplete                            |
---------------------------------------------------------------------------------------------
WillConnect                                        |连接相关的事件
ConnectionComplete                                |
Disconnect                                        |
---------------------------------------------------------------------------------------------
WillExecute                                        |命令相关的事件
ExecuteComplete                                    |
---------------------------------------------------------------------------------------------
InfoMessage                                        |关于当前操作有附加信息的通知
---------------------------------------------------------------------------------------------
表2：                                    RecordsetEvent
---------------------------------------------------------------------------------------------
        记录集事件（RecordsetEvent）            |                说明
---------------------------------------------------------------------------------------------
FetchProgress                                    |数据检索相关的事件
FetchComplete                                    |
---------------------------------------------------------------------------------------------
WillChangeField                                    |字段更改相关的事件
FieldChangeComplete                                |
---------------------------------------------------------------------------------------------
WillMove                                        |定位操作相关的事件
MoveComplete                                    |
EndOfRecordset                                    |
---------------------------------------------------------------------------------------------
WillChangeRecord                                |行更改相关的事件
RecordChangeComplete                            |
---------------------------------------------------------------------------------------------
WillChangeRecordset                                |数据集更改相关的事件
RecordsetChangeComplete                            |
---------------------------------------------------------------------------------------------
下面结合一个历程的片段来说明如何在应用程序中使用ADO事件。
//从ConnectionEventsVt派生出一个连接事件类的对象
class CConnEvent:public ConnectionEventVt
{
private:
    //接口引用计数
    ULONG m_cRef;
public:
    CConnEvent()    {m_cRef=0;};
    ~CConnEvent() {};
    //实现IUnknown的三个标准方法
    STDMETHODIMP QueryInterface(REFIID riid,void**ppv);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);
    //处理InfoMessage事件的回调函数
    STDMETHODIMP raw_InfoMessage(
        struct Error*pError;
        EventStatusEnum * adStatus,
        struct_Connection* pConnection);
    //处理BeginTransComplete事件的回调函数
    STDMETHODIMP raw_BeginTransComplete(
        LONG TransactionLevel,
        struct Error*pError;
        EventStatusEnum*adStatus,
        struct_Connection*pConnection);
    //处理CommitTransComplete事件的回调函数
    STDMETHODIMP raw_CommitTransComplete(
        struct Error*pError,
        EventStatusEnum *adStatus,
        struct_Connection *pConnection);
    //以下还有若干事件函数，在此省略
    //......
};
上面定义的这个类继承了ConnectionEventVt接口，实现了Connection事件类所有事件的回调函数。读者可以仿照它定义自己的RecordsetEvent事件类。这两个类是所谓的COM组件类，所以它们必须实现IUnknown的三个标准方法：QueryInterface、AddRef和Release。读者可以使用合适的工具（如ATL）简化这部分工作，这里只给出和这一节内容有关系最大的几个回调函数的代码。
//CConnEvent类的实现
STDMETHODIMP CConnEvent::raw_InfoMessage(
    struct Error*pError,
    EventStatusEnum*adStatus,
    struct_Connection *pConnection)
{
    *adStatus=adStatuUnwantedEvent;
    return S_OK;
};
STDMETHODIMP raw_BeginTransComplete(
    LONG TransactionLevel,
    struct Error*pError;
    EventStatusEnum*adStatus,
    struct_Connection*pConnection)
{
    *adStatus=adStatuUnwantedEvent;
    return S_OK;
};
STDMETHODIMP raw_CommitTransComplete(
    struct Error*pError,
    EventStatusEnum *adStatus,
    struct_Connection *pConnection)
{
    *adStatus=adStatuUnwantedEvent;
    return S_OK;
};
由于是演示，这几个函数没有做任何有意义的事情。需要注意每个函数的第一句，在这里把adStatus的值设为adStatusUnwantedEnvet,这样这个函数在第一次被调用之后将不会被再次调用。如果用户需要这个事件处理函数的工作，就必须去掉第一句。
下面的代码演示了怎样在程序中使用上面定义的ADO事件类。
int main(int argc,char*argv[])
{
    HRESULT hr;
    DWORD dwConnEvt;
    DWORD dwRstEvt;
    IConnectionPointContainer *pCRC =NULL;
    IConnectionPoint*pCP=NULL;
    lUnkown*pUnk=NULL;
    CRstEvent*pRstEvent=NULL;
    CConnEvent*pConnEvent=NULL;
    int rc=0;
    _RecordsetPtr pRst;
    _ConnectionPtr pConn;
    ::CoInitialize(NULL);
    hr=pConn.CreateInstance(__uuidof(Connection));
    if(FAILED)(hr)) return rc;
    hr=pRst.CreateInstance(__uuidof(Recordset));
    if(FAILED(hr)) return rc;
    //开始使用Connection事件
    hr=pConn->QueryInterface(__uuidof(IConnectionPointContainer),
        (void**)&pCPC);
    if (FAILED)(hr)) return rc;
    hr=pCRC->FindConnectionPoint(__uuidof(ConnectionEvents),&pCP);
    pCPC->Release();
    if (FAILED(hr)) return rc;
    //开始使用Recordset事件
    hr=pRst->QueryInterface(__uuidof(IConnectionPointContainter),
        (void**)&pCPC);
    if(FAILED(hr)) return rc;
    hr=pCRC->FindConnetionPoint(__uuidof(RecordsetEvents),&pCP);
    pCPC->Release();
    if (FAILED(hr)) return rc;
    pRstEvent=new CRstEvent();
    hr=pRstEvent->QueryInterface(__uuidof(IUnknown),(void**)&pUnk);
    if (FAILED(hr)) return rc;
    hr=pCP->Advise(pUnk,&dwRstEvt);
    pCP->Release();
    if (FAILED(hr)) return rc;
    //进行一些数据库操作
    pConn->Open("DSN=Pubs;","sa","",adConnectionUnspecified);
    pRst->Open("SELECT * FROM authors",(IDispatch*)pConn,
        adOpenStatic,adLockReadOnly,adCmdText);
    pRst->MoveFirst();
    while(pRst->EndOfFile==FALSE)
    {
        wprintf(L"Name='%s'/n",(wchar_t*)
            ((_bstr_t)pRst->Fields->GetItem("au_lname")->Value));
        pRst->MoveNext();
    }
    pRst->Close();
    pConn->Close();
    //停止使用Connection事件类
    hr=pConn->QueryInterface(__uuidof(IConnectionPointContainter),
        (void**)&pCPC);
    if(FAILED(hr)) return rc;
    hr=pCPC->FindConnectionPoint(__uuidof(ConnectionEvents),&pCP);
    pCPC->Release();
    if (FAILED(hr)) return rc;
    //停止使用Recordset事件类
    hr=pConn->QueryInterface(__uuidof(IConnectionPointContainter),
        (void**)&pCPC);
    if(FAILED(hr)) return rc;
    hr=pCPC->FindConnectionPoint(__uuidof(RecordsetEvents),&pCP);
    pCPC->Release();
    if (FAILED(hr)) return rc;
    hr=pCP->Unabvise(dwRstEvt);
    pCP->Release();
    if(FAILED(hr)) return rc;
    CoUninitialize();
    return 1;
} 
注: 本文内容基本来自网络,自己只做少量修改.
