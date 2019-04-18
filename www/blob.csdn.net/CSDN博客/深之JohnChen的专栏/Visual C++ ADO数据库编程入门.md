# Visual C++ ADO数据库编程入门 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:27:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2091


**Visual C++ ADO数据库编程入门**

ADO 是目前在Windows环境中比较流行的客户端数据库编程技术。ADO是建立在OLE DB底层技术之上的高级编程接口，因而它兼具有强大的数据处理功能（处理各种不同类型的数据源、分布式的数据处理等等）和极其简单、易用的编程接口，因而得到了广泛的应用。而且按微软公司的意图，OLE DB和ADO将逐步取代 ODBC和DAO。现在介绍ADO各种应用的文章和书籍有很多，本文着重站在初学者的角度，简要探讨一下在VC++中使用ADO编程时的一些问题。我们希望阅读本文之前，您对ADO技术的基本原理有一些了解。

　　一、在VC++中使用ADO编程

　　ADO实际上就是由一组Automation对象构成的组件，因此可以象使用其它任何Automation对象一样使用ADO。ADO中最重要的对象有三个：Connection、Command和Recordset，它们分别表示连接对象、命令对象和记录集对象。如果您熟悉使用MFC中的ODBC类（CDatabase、CRecordset)编程，那么学习ADO编程就十分容易了。

　　使用ADO编程时可以采用以下三种方法之一：

　　1、使用预处理指令#import

#import "C:/Program Files/Common Files/System/ADO/msado15.dll" /
no_namespace rename("EOF", "EndOfFile") 

　　但要注意不能放在stdAfx.h文件的开头，而应该放在所有include指令的后面。否则在编译时会出错。
程序在编译过程中，VC++会读出msado15.dll中的类型库信息，自动产生两个该类型库的头文件和实现文件msado15.tlh和msado15.tli（在您的Debug或Release目录下）。在这两个文件里定义了ADO的所有对象和方法，以及一些枚举型的常量等。我们的程序只要直接调用这些方法就行了，与使用MFC中的COleDispatchDriver类调用Automation对象十分类似。

　　2、使用MFC中的CIDispatchDriver

　　就是通过读取msado15.dll中的类型库信息，建立一个COleDispatchDriver类的派生类，然后通过它调用ADO对象。

　　3、直接用COM提供的API

　　如使用如下代码：

CLSID clsid;
HRESULT hr = ::CLSIDFromProgID(L"ADODB.Connection", &clsid);
if(FAILED(hr))
{...}
::CoCreateInstance(clsid, NULL, CLSCTX_SERVER, IID_IDispatch, (void **)
&pDispatch);
if(FAILED(hr))
{...} 

　　以上三种方法，第一和第二种类似，可能第一种好用一些，第三种编程可能最麻烦。但可能第三种方法也是效率最高的，程序的尺寸也最小，并且对ADO的控制能力也最强。

　　据微软资料介绍，第一种方法不支持方法调用中的默认参数，当然第二种方法也是这样，但第三种就不是这样了。采用第三种方法的水平也最高。当你需要绕过ADO而直接调用OLE DB底层的方法时，就一定要使用第三种方法了。

　　ADO编程的关键，就是熟练地运用ADO提供的各种对象(object)、方法(method)、属性(property)和容器（collection）。另外，如果是在MS SQL或Oracle等大型数据库上编程，还要能熟练使用SQL语言。
二、使用#import方法的编程步骤

　　这里建议您使用#import的方法，因为它易学、易用，代码也比较简洁。

　　1、 添加#import指令

　　打开stdafx.h文件，将下列内容添加到所有的include指令之后：

#include <icrsint.h> //Include support for VC++ Extensions
#import "C:/Program Files/Common Files/System/ADO/msado15.dll" /
no_namespace rename("EOF", "adoEOF") 

　　其中icrsint.h文件包含了VC++扩展的一些预处理指令、宏等的定义，用于COM编程时使用。

　　2、定义_ConnectionPtr型变量，并建立数据库连接

　　建立了与数据库服务器的连接后，才能进行其他有关数据库的访问和操作。ADO使用Connection对象来建立与数据库服务器的连接，所以它相当于MFC中的CDatabase类。和CDatabase类一样，调用Connection对象的Open方法即可建立与服务器的连接。

　　数据类型 _ConnectionPtr实际上就是由类模板_com_ptr_t而得到的一个具体的实例类，其定义可以到msado15.tlh、comdef.h 和comip.h这三个文件中找到。在msado15.tlh中有：

_COM_SMARTPTR_TYPEDEF(_Collection, __uuidof(_Collection)); 

　　经宏扩展后就得到了_ConnectionPtr类。_ConnectionPtr类封装了Connection对象的Idispatch接口指针，及一些必要的操作。我们就是通过这个指针来操纵Connection对象。类似地，后面用到的_CommandPtr和_RecordsetPtr类型也是这样得到的，它们分别表示命令对象指针和记录集对象的指针。

　　（1）、连接到MS SQL Server

　　注意连接字符串的格式，提供正确的连接字符串是成功连接到数据库服务器的第一步，有关连接字符串的详细信息参见微软MSDN Library光盘。

　　本例连接字符串中的server_name，database_name，user_name和password在编程时都应该替换成实际的内容。

_ConnectionPtr pMyConnect=NULL;
HRESULT hr=pMyConnect.CreateInstance(__uuidof(Connection)));
if(FAILED(hr))return;

_bstr_t strConnect="Provider=SQLOLEDB; Server=server_name;"
"Database=database_name; uid=user_name; pwd=password;"; 
//connecting to the database server now:
try{pMyConnect->Open(strConnect,"","",NULL);}
catch (_com_error &e)
{
::MessageBox(NULL,e.Description(),"警告",MB_OK │ MB_ICONWARNING);
} 

　　注意Connection对象的Open方法中的连接字符串参数必须是BSTR或_bstr_t类型。另外，本例是直接通过OLE DB Provider建立连接，所以无需建立数据源。

　　（2）、通过ODBC Driver连接到Database Server连接字符串格式与直接用ODBC编程时的差不多：

_bstr_t strConnect="DSN=datasource_name; Database=database_name; uid=user_name; pwd=password;"; 

　　此时与ODBC编程一样，必须先建立数据源。

　　3、定义_RecordsetPtr型变量，并打开数据集

　　定义_RecordsetPtr型变量，然后通过它调用Recordset对象的Open方法，即可打开一个数据集。所以Recordset对象与MFC中的CRecordset类类似，它也有当前记录、当前记录指针的概念。如：

_RecordsetPtr m_pRecordset;
if(!FAILED(m_pRecordset.CreateInstance( __uuidof( Recordset )))
{
m_pDoc->m_initialized=FALSE;
return;
}

try{
m_pRecordset->Open(_variant_t("mytable"),
_variant_t((IDispatch *)pMyConnect,true), adOpenKeyset,
adLockOptimistic, adCmdTable);
}
catch (_com_error &e)
{
::MessageBox(NULL,"无法打开mytable表。","提示",
MB_OK │ MB_ICONWARNING);
} 

　　Recordset对象的Open方法非常重要，它的第一个参数可以是一个SQL语句、一个表的名字或一个命令对象等等；第二个参数就是前面建立的连接对象的指针。此外，用Connection和Command对象的Execute方法也能得到记录集，但是只读的。
4、读取当前记录的数据

　　我认为读取数据的最方便的方法如下：

try{
m_pRecordset->MoveFirst(); 
while(m_pRecordset->adoEOF==VARIANT_FALSE) 
{
//Retrieve column's value: 
CString sName=(char*)(_bstr_t)(m_pRecordset->Fields->GetItem
(_variant_t("name"))->Value);
short cAge=(short)(m_pRecordset->Fields->GetItem
(_variant_t("age"))->Value);
//Do something what you want to do:
......
m_pRecordset->MoveNext(); 
}
}//try
catch (_com_error &e)
{
CString str=(char*)e.Description();
::MessageBox(NULL,str+"/n又出毛病了。","提示",
MB_OK │ MB_ICONWARNING);
} 

　　本例中的name和age都是字段名，读取的字段值分别保存在sName和cAge变量内。例中的Fields是Recordset对象的容器，GetItem方法返回的是Field对象，而Value则是Field对象的一个属性（即该字段的值）。通过此例，应掌握操纵对象属性的方法。例如，要获得Field 对象的Value属性的值可以直接用属性名Value来引用它（如上例），但也可以调用Get方法，例如：

CString sName=(char*)(_bstr_t)(m_pRecordset->Fields->GetItem
(_variant_t("name"))->GetValue()); 

　　从此例还可以看到，判断是否到达记录集的末尾，使用记录集的adoEOF属性，其值若为真即到了结尾，反之则未到。判断是否到达记录集开头，则可用BOF属性。

　　另外，读取数据还有一个方法，就是定义一个绑定的类，然后通过绑定的变量得到字段值（详见后面的介绍）。

　　5、修改数据

　　方法一：

try{
m_pRecordset->MoveFirst(); 
while(m_pRecordset->adoEOF==VARIANT_FALSE) 
{ 
m_pRecordset->Fields->GetItem
(_variant_t("姓名"))->Value=_bstr_t("赵薇");
......
m_pRecordset->Update();

m_pRecordset->MoveNext(); 
}
}//try 

　　改变了Value属性的值，即改变了字段的值。

　　方法二：

m_pRecordset->Fields->GetItem
(_variant_t("姓名"))->PutValue(_bstr_t("赵薇")); 

　　方法三：就是用定义绑定类的方法（详见后面的介绍）。

　　6、添加记录

　　新记录添加成功后，即自动成为当前记录。AddNew方法有两种形式，一个含有参数，而另一个则不带参数。

　　方法一（不带参数）：

// Add new record into this table:
try{
if(!m_pRecordset->Supports(adAddNew)) return;

m_pRecordset->AddNew(); 
m_pRecordset->Fields->GetItem
(_variant_t("姓名"))->Value=_bstr_t("赵薇");
m_pRecordset->Fields->GetItem
(_variant_t("性别"))->Value=_bstr_t("女");
m_pRecordset->Fields->GetItem
(_variant_t("age"))->Value=_variant_t((short)20);
m_pRecordset->Fields->GetItem
(_variant_t("marry"))->Value=_bstr_t("未婚");
m_pRecordset->Update(); 
}//try
catch (_com_error &e)
{
::MessageBox(NULL, "又出毛病了。","提示",MB_OK │ MB_ICONWARNING);
} 

　　这种方法弄完了还要调用Update()。

　　方法二（带参数）：

_variant_t varName[4],narValue[4];
varName[0] = L"姓名";
varName[1] = L"性别";
varName[2] = L"age";
varName[3] = L"marry";
narValue[0]=_bstr_t("赵薇");
narValue[1]=_bstr_t("女");
narValue[2]=_variant_t((short)20);
narValue[3]=_bstr_t("未婚");

const int nCrit = sizeof varName / sizeof varName[0];
// Create SafeArray Bounds and initialize the array
SAFEARRAYBOUND rgsaName[1],rgsaValue[1];
rgsaName[0].lLbound = 0; 
rgsaName[0].cElements = nCrit;
SAFEARRAY *psaName = SafeArrayCreate( VT_VARIANT, 1, rgsaName );
rgsaValue[0].lLbound = 0;
rgsaValue[0].cElements = nCrit;
SAFEARRAY *psaValue = SafeArrayCreate( VT_VARIANT, 1, rgsaValue );
// Set the values for each element of the array
HRESULT hr1=S_OK.hr2=S_OK;
for( long i = 0 ; i < nCrit && SUCCEEDED( hr1 ) && SUCCEEDED( hr2 );i++) 
{ 
hr1=SafeArrayPutElement(psaName, &i,&varName[i]);
hr2=SafeArrayPutElement(psaValue, &i,&narValue[i]); }

// Initialize and fill the SafeArray
VARIANT vsaName,vsaValue; 
vsaName.vt = VT_VARIANT │ VT_ARRAY;
vsaValue.vt = VT_VARIANT │ VT_ARRAY;
V_ARRAY(&vsaName) = psaName;//&vsaName->parray=psaName;
//see definition in oleauto.h file.
V_ARRAY(&vsaValue) = psaValue;

// Add a new record:
m_pRecordset->AddNew(vsaName,vsaValue); 

　　这种方法不需要调用Update，因为添加后，ADO会自动调用它。此方法主要是使用SafeArray挺麻烦。

　　方法三：就是用定义绑定类的方法（详见后面的介绍）。7、删除记录

　　调用Recordset的Delete方法就行了，删除的是当前记录。要了解Delete的其它用法请查阅参考文献。

try{
m_pRecordset->MoveFirst(); 
while(m_pRecordset->adoEOF==VARIANT_FALSE) 
{
CString sName=(char*)(_bstr_t)(m_pRecordset->Fields->GetItem
(_variant_t("姓名"))->Value);
if(::MessageBox(NULL,"姓名="+sName+"/n删除她吗？",
"提示",MB_YESNO │ MB_ICONWARNING)==IDYES)
{
m_pRecordset->Delete(adAffectCurrent); 
m_pRecordset->Update();
}
m_pRecordset->MoveNext(); 
}
}//try
catch (_com_error &e)
{
::MessageBox(NULL,"又出毛病了。","提示",MB_OK │ MB_ICONWARNING);
} 

　　8、使用带参数的命令

　　Command对象所代表的就是一个Provider能够理解的命令，如SQL语句等。使用Command对象的关键就是把表示命令的语句设置到CommandText属性中，然后调用Command对象的Execute方法就行了。一般情况下在命令中无需使用参数，但有时使用参数，可以增加其灵活性和效率。

　　(1). 建立连接、命令对象和记录集对象

　　本例中表示命令的语句就是一个SQL语句（SELECT语句）。SELECT语句中的问号?就代表参数，如果要多个参数，就多放几个问号，每个问号代表一个参数。

_ConnectionPtr Conn1;
_CommandPtr Cmd1;
ParametersPtr *Params1 = NULL; // Not an instance of a smart pointer.
_ParameterPtr Param1;
_RecordsetPtr Rs1;

try
{
// Create Connection Object (1.5 Version)
Conn1.CreateInstance( __uuidof( Connection ) );
Conn1->ConnectionString = bstrConnect;
Conn1->Open( bstrEmpty, bstrEmpty, bstrEmpty, -1 );
// Create Command Object
Cmd1.CreateInstance( __uuidof( Command ) );
Cmd1->ActiveConnection = Conn1;
Cmd1->CommandText = _bstr_t("SELECT * FROM mytable WHERE age< ?");
}//try 

　　要注意命令对象必须与连接对象关联起来才能起作用，本例中将命令对象的ActiveConnection属性设置为连接对象的指针，即为此目的：

Cmd1->ActiveConnection = Conn1; 

　　(2). 创建参数对象，并给参数赋值

// Create Parameter Object
Param1 = Cmd1->CreateParameter( _bstr_t(bstrEmpty),
adInteger,
adParamInput,
-1,
_variant_t( (long) 5) );
Param1->Value = _variant_t( (long) 5 );
Cmd1->Parameters->Append( Param1 ); 

　　用命令对象的方法来创建一个参数对象，其中的长度参数（第三个）如果是固定长度的类型，就填-1，如果是字符串等可变长度的就填其实际长度。Parameters是命令对象的一个容器，它的Append方法就是把创建的参数对象追加到该容器里。Append进去的参数按先后顺序与SQL语句中的问号从左至右一一对应。

　　(3). 执行命令打开记录集

// Open Recordset Object
Rs1 = Cmd1->Execute( &vtEmpty, &vtEmpty2, adCmdText ); 

　　但要注意，用Command和Connection对象的Execute方法得到的Recordset是只读的。因为在打开Recordset之前，我们无法设置它的LockType属性（其默认值为只读）。而在打开之后设置LockType不起作用。

　　我发现用上述方法得到记录集Rs1后，不但Rs1中的记录无法修改，即使直接用SQL语句修改同一表中任何记录都不行。

　　要想能修改数据，还是要用Recordset自己的Open方法才行，如：

try{
m_pRecordset->Open((IDispatch *) Cmd1, vtMissing,
adOpenStatic, adLockOptimistic, adCmdUnspecified);
}
catch (_com_error &e)
{
::MessageBox(NULL,"mytable表不存在。","提示",MB_OK │ MB_ICONWARNING);
} 

　　Recordset对象的Open方法真是太好了，其第一个参数可以是SQL语句、表名字、命令对象指针等等。

　　9、响应ADO的通知事件

　　通知事件就是当某个特定事件发生时，由Provider通知客户程序，换句话说，就是由Provider调用客户程序中的一个特定的方法（即事件的处理函数）。所以为了响应一个事件，最关键的就是要实现事件的处理函数。

　　(1). 从ConnectionEventsVt接口派生出一个类

　　为了响应_Connection的通知事件，应该从ConnectionEventsVt接口派生出一个类：

class CConnEvent : public ConnectionEventsVt
{
private:
ULONG m_cRef;
public:
CConnEvent() { m_cRef = 0; };
~CConnEvent() {};

STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
STDMETHODIMP_(ULONG) AddRef(void);
STDMETHODIMP_(ULONG) Release(void);
STDMETHODIMP raw_InfoMessage( 
struct Error *pError,
EventStatusEnum *adStatus,
struct _Connection *pConnection);
STDMETHODIMP raw_BeginTransComplete( 
LONG TransactionLevel,
struct Error *pError,
EventStatusEnum *adStatus,
struct _Connection *pConnection);
......
};  

　　　(2). 实现每一个事件的处理函数(凡是带raw_前缀的方法都把它实现了)：

STDMETHODIMP CConnEvent::raw_InfoMessage( 
struct Error *pError,
EventStatusEnum *adStatus,
struct _Connection *pConnection)
{
*adStatus = adStatusUnwantedEvent;
return S_OK;
}; 

　　有些方法虽然你并不需要，但也必须实现它，只需简单地返回一个S_OK即可。但如果要避免经常被调用，还应在其中将adStatus参数设置为adStatusUnwantedEvent，则在本次调用后，以后就不会被调用了。
另外还必须实现QueryInterface, AddRef, 和Release三个方法: 

STDMETHODIMP CConnEvent::QueryInterface(REFIID riid, void ** ppv) 
{
*ppv = NULL;
if (riid == __uuidof(IUnknown) ││ 
riid == __uuidof(ConnectionEventsVt)) *ppv = this;
if (*ppv == NULL)
return ResultFromScode(E_NOINTERFACE);
AddRef();
return NOERROR;
}
STDMETHODIMP_(ULONG) CConnEvent::AddRef() { return ++m_cRef; };
STDMETHODIMP_(ULONG) CConnEvent::Release()
{ 
if (0 != --m_cRef) return m_cRef;
delete this;
return 0;
} 

　　(3). 开始响应通知事件

// Start using the Connection events
IConnectionPointContainer *pCPC = NULL;
IConnectionPoint *pCP = NULL;

hr = pConn.CreateInstance(__uuidof(Connection));
if (FAILED(hr)) return;

hr = pConn->QueryInterface(__uuidof(IConnectionPointContainer), 
(void **)&pCPC);
if (FAILED(hr)) return;
hr = pCPC->FindConnectionPoint(__uuidof(ConnectionEvents), &pCP);
pCPC->Release();
if (FAILED(hr)) return;

pConnEvent = new CConnEvent();
hr = pConnEvent->QueryInterface(__uuidof(IUnknown), (void **) &pUnk);
if (FAILED(hr)) return rc; 
hr = pCP->Advise(pUnk, &dwConnEvt);
pCP->Release();
if (FAILED(hr)) return;

pConn->Open("dsn=Pubs;", "sa", "", adConnectUnspecified);  

　　也就是说在连接(Open)之前就做这些事。

　　(4). 停止响应通知事件

pConn->Close();
// Stop using the Connection events
hr = pConn->QueryInterface(__uuidof(IConnectionPointContainer), 
(void **) &pCPC);
if (FAILED(hr)) return;
hr = pCPC->FindConnectionPoint(__uuidof(ConnectionEvents), &pCP);
pCPC->Release();
if (FAILED(hr)) return rc;
hr = pCP->Unadvise( dwConnEvt );
pCP->Release();
if (FAILED(hr)) return; 

　　在连接关闭之后做这件事10、邦定数据

　　定义一个绑定类，将其成员变量绑定到一个指定的记录集，以方便于访问记录集的字段值。

　　(1). 从CADORecordBinding派生出一个类：

class CCustomRs : public CADORecordBinding
{
BEGIN_ADO_BINDING(CCustomRs)
ADO_VARIABLE_LENGTH_ENTRY2(3, adVarChar, m_szau_fname, 
sizeof(m_szau_fname), lau_fnameStatus, false)
ADO_VARIABLE_LENGTH_ENTRY2(2, adVarChar, m_szau_lname, 
sizeof(m_szau_lname), lau_lnameStatus, false)
ADO_VARIABLE_LENGTH_ENTRY2(4, adVarChar, m_szphone, 
sizeof(m_szphone), lphoneStatus, true)
END_ADO_BINDING()

public:
CHAR m_szau_fname[22];
ULONG lau_fnameStatus;
CHAR m_szau_lname[42];
ULONG lau_lnameStatus;
CHAR m_szphone[14];
ULONG lphoneStatus;
}; 

　　其中将要绑定的字段与变量名用BEGIN_ADO_BINDING宏关联起来。每个字段对应于两个变量，一个存放字段的值，另一个存放字段的状态。字段用从1开始的序号表示，如1，2，3等等。

　　特别要注意的是：如果要绑定的字段是字符串类型，则对应的字符数组的元素个数一定要比字段长度大2（比如m_szau_fname[22]，其绑定的字段au_fname的长度实际是20），不这样绑定就会失败。我分析多出的2可能是为了存放字符串结尾的空字符null和BSTR字符串开头的一个字（表示BSTR的长度）。这个问题对于初学者来说可能是一个意想不到的问题。

　　CADORecordBinding类的定义在icrsint.h文件里，内容是：

class CADORecordBinding
{
public:
STDMETHOD_(const ADO_BINDING_ENTRY*, GetADOBindingEntries) (VOID) PURE;
};

BEGIN_ADO_BINDING宏的定义也在icrsint.h文件里，内容是：
#define BEGIN_ADO_BINDING(cls) public: /
typedef cls ADORowClass; /
const ADO_BINDING_ENTRY* STDMETHODCALLTYPE GetADOBindingEntries() { /
static const ADO_BINDING_ENTRY rgADOBindingEntries[] = { 

ADO_VARIABLE_LENGTH_ENTRY2宏的定义也在icrsint.h文件里：
#define ADO_VARIABLE_LENGTH_ENTRY2(Ordinal, DataType, Buffer, Size, Status, Modify)/
{Ordinal, /
DataType, /
0, /
0, /
Size, /
offsetof(ADORowClass, Buffer), /
offsetof(ADORowClass, Status), /
0, /
classoffset(CADORecordBinding, ADORowClass), /
Modify},

#define END_ADO_BINDING宏的定义也在icrsint.h文件里：
#define END_ADO_BINDING() {0, adEmpty, 0, 0, 0, 0, 0, 0, 0, FALSE}};/
return rgADOBindingEntries;} 

　　　(2). 绑定

_RecordsetPtr Rs1;
IADORecordBinding *picRs=NULL;
CCustomRs rs;
......
Rs1->QueryInterface(__uuidof(IADORecordBinding), 
(LPVOID*)&picRs));
picRs->BindToRecordset(&rs); 

　　派生出的类必须通过IADORecordBinding接口才能绑定，调用它的BindToRecordset方法就行了。

　　(3). rs中的变量即是当前记录字段的值

//Set sort and filter condition:
// Step 4: Manipulate the data
Rs1->Fields->GetItem("au_lname")->Properties->GetItem("Optimize")->Value = true; 
Rs1->Sort = "au_lname ASC";
Rs1->Filter = "phone LIKE '415 5*'";

Rs1->MoveFirst();
while (VARIANT_FALSE == Rs1->EndOfFile)
{
printf("Name: %s/t %s/tPhone: %s/n", 
(rs.lau_fnameStatus == adFldOK ? rs.m_szau_fname : ""), 
(rs.lau_lnameStatus == adFldOK ? rs.m_szau_lname : ""),
(rs.lphoneStatus == adFldOK ? rs.m_szphone : ""));
if (rs.lphoneStatus == adFldOK)
strcpy(rs.m_szphone, "777");
TESTHR(picRs->Update(&rs)); // Add change to the batch
Rs1->MoveNext();
}
Rs1->Filter = (long) adFilterNone;
......
if (picRs) picRs->Release();
Rs1->Close();
pConn->Close(); 

　　只要字段的状态是adFldOK，就可以访问。如果修改了字段，不要忘了先调用picRs的Update（注意不是Recordset的Update），然后才关闭，也不要忘了释放picRs（即picRs->Release();）。

　　(4). 此时还可以用IADORecordBinding接口添加新纪录

if(FAILED(picRs->AddNew(&rs)))
...... 

　　11. 访问长数据

　　在Microsoft SQL中的长数据包括text、image等这样长类型的数据，作为二进制字节来对待。

　　可以用Field对象的GetChunk和AppendChunk方法来访问。每次可以读出或写入全部数据的一部分，它会记住上次访问的位置。但是如果中间访问了别的字段后，就又得从头来了。

　　请看下面的例子：

//写入一张照片到数据库：
VARIANT varChunk;
SAFEARRAY *psa;
SAFEARRAYBOUND rgsabound[1];

//VT_ARRAY │ VT_UI1
CFile f("h://aaa.jpg",CFile::modeRead);
BYTE bVal[ChunkSize+1];
UINT uIsRead=0;
//Create a safe array to store the array of BYTES 
while(1)
{
uIsRead=f.Read(bVal,ChunkSize);
if(uIsRead==0)break;
rgsabound[0].cElements =uIsRead;
rgsabound[0].lLbound = 0;
psa = SafeArrayCreate(VT_UI1,1,rgsabound);
for(long index=0;index<uIsRead;index++) 
{
if(FAILED(SafeArrayPutElement(psa,&index,&bVal[index])))
::MessageBox(NULL,"啊，又出毛病了。","提示",MB_OK │ MB_ICONWARNING);
}
varChunk.vt = VT_ARRAY│VT_UI1;
varChunk.parray = psa;
try{
m_pRecordset->Fields->GetItem("photo")->AppendChunk(varChunk); 
}
catch (_com_error &e)
{
CString str=(char*)e.Description();
::MessageBox(NULL,str+"/n又出毛病了。","提示",MB_OK │ MB_ICONWARNING);
}
::VariantClear(&varChunk);
::SafeArrayDestroyData( psa);
if(uIsRead<ChunkSize)break;
}//while(1) 
f.Close();

//从数据库读一张照片：
CFile f;
f.Open("h://bbb.jpg",CFile::modeWrite│CFile::modeCreate);
long lPhotoSize = m_pRecordset->Fields->Item["photo"]->ActualSize; 
long lIsRead=0;

_variant_t varChunk;
BYTE buf[ChunkSize];
while(lPhotoSize>0)
{
lIsRead=lPhotoSize>=ChunkSize? ChunkSize:lPhotoSize;
varChunk = m_pRecordset->Fields->
Item["photo"]->GetChunk(lIsRead);
for(long index=0;index<lIsRead;index++) 
{ 
::SafeArrayGetElement(varChunk.parray,&index,buf+index); 
}
f.Write(buf,lIsRead);
lPhotoSize-=lIsRead;
}//while()
f.Close(); 

12. 使用SafeArray问题

　　学会使用SafeArray也是很重要的，因为在ADO编程中经常要用。它的主要目的是用于automation中的数组型参数的传递。因为在网络环境中，数组是不能直接传递的，而必须将其包装成SafeArray。实质上SafeArray就是将通常的数组增加一个描述符，说明其维数、长度、边界、元素类型等信息。SafeArray也并不单独使用，而是将其再包装到VARIANT类型的变量中，然后才作为参数传送出去。在VARIANT的vt成员的值如果包含VT_ARRAY│...,那么它所封装的就是一个SafeArray，它的parray成员即是指向SafeArray的指针。SafeArray中元素的类型可以是VARIANT能封装的任何类型，包括VARIANT类型本身。 

　　使用SafeArray的具体步骤：

　　方法一：

　　包装一个SafeArray：

　　(1). 定义变量，如：

VARIANT varChunk;
SAFEARRAY *psa;
SAFEARRAYBOUND rgsabound[1]; 

　　(2). 创建SafeArray描述符：

uIsRead=f.Read(bVal,ChunkSize);//read array from a file.
if(uIsRead==0)break;
rgsabound[0].cElements =uIsRead;
rgsabound[0].lLbound = 0;
psa = SafeArrayCreate(VT_UI1,1,rgsabound); 

　　(3). 放置数据元素到SafeArray：

for(long index=0;index<uIsRead;index++) 
{
if(FAILED(SafeArrayPutElement(psa,&index,&bVal[index])))
::MessageBox(NULL,"出毛病了。","提示",MB_OK │ MB_ICONWARNING);
} 

　　一个一个地放，挺麻烦的。

　　(4). 封装到VARIANT内：

varChunk.vt = VT_ARRAY│VT_UI1;
varChunk.parray = psa; 

　　这样就可以将varChunk作为参数传送出去了。

　　读取SafeArray中的数据的步骤：

　　(1). 用SafeArrayGetElement一个一个地读

BYTE buf[lIsRead];
for(long index=0;index<lIsRead;index++) 
{ 
::SafeArrayGetElement(varChunk.parray,&index,buf+index); 
} 

　　就读到缓冲区buf里了。

　　方法二：

　　使用SafeArrayAccessData直接读写SafeArray的缓冲区：

　　(1). 读缓冲区：

BYTE *buf;
SafeArrayAccessData(varChunk.parray, (void **)&buf);
f.Write(buf,lIsRead);
SafeArrayUnaccessData(varChunk.parray); 

　　(2). 写缓冲区：

BYTE *buf;
::SafeArrayAccessData(psa, (void **)&buf);
for(long index=0;index<uIsRead;index++) 
{
buf[index]=bVal[index]; 
}
::SafeArrayUnaccessData(psa);

varChunk.vt = VT_ARRAY│VT_UI1;
varChunk.parray = psa; 

　　这种方法读写SafeArray都可以，它直接操纵SafeArray的数据缓冲区，比用SafeArrayGetElement和SafeArrayPutElement速度快。特别适合于读取数据。但用完之后不要忘了调用::SafeArrayUnaccessData(psa)，否则会出错的。

　　13. 使用书签( bookmark )

　　书签可以唯一标识记录集中的一个记录，用于快速地将当前记录移回到已访问过的记录，以及进行过滤等等。Provider会自动为记录集中的每一条记录产生一个书签，我们只需要使用它就行了。我们不能试图显示、修改或比较书签。ADO用记录集的Bookmark属性表示当前记录的书签。

　　用法步骤：

　　(1). 建立一个VARIANT类型的变量

_variant_t VarBookmark;

　　(2). 将当前记录的书签值存入该变量

　　也就是记录集的Bookmark属性的当前值。

VarBookmark = rst->Bookmark;

　　(3). 返回到先前的记录

　　将保存的书签值设置到记录集的书签属性中：

// Check for whether bookmark set for a record
if (VarBookmark.vt == VT_EMPTY)
printf("No Bookmark set!/n");
else 
rst->Bookmark = VarBookmark; 

　　设置完后，当前记录即会移动到该书签指向的记录。14、设置过滤条件

　　Recordset对象的Filter属性表示了当前的过滤条件。它的值可以是以AND或OR连接起来的条件表达式（不含WHERE关键字）、由书签组成的数组或ADO提供的FilterGroupEnum枚举值。为Filter属性设置新值后Recordset的当前记录指针会自动移动到满足过滤条件的第一个记录。例如：

rst->Filter = _bstr_t ("姓名='赵薇' AND 性别=’女’"); 

　　在使用条件表达式时应注意下列问题：

　　（1）、可以用圆括号组成复杂的表达式

　　例如：

rst->Filter = _bstr_t ("(姓名='赵薇' AND 性别=’女’) OR AGE<25"); 

　　但是微软不允许在括号内用OR，然后在括号外用AND，例如：

rst->Filter = _bstr_t ("(姓名='赵薇' OR 性别=’女’) AND AGE<25"); 

　　必须修改为：

rst->Filter = _bstr_t ("(姓名='赵薇' AND AGE<25) OR (性别=’女’ AND AGE<25)"); 

　　（2）、表达式中的比较运算符可以是LIKE

　　LIKE后被比较的是一个含有通配符*的字符串，星号表示若干个任意的字符。

　　字符串的首部和尾部可以同时带星号*

rst->Filter = _bstr_t ("姓名 LIKE '*赵*' "); 

　　也可以只是尾部带星号：

rst->Filter = _bstr_t ("姓名 LIKE '赵*' "); 

　　Filter属性值的类型是Variant，如果过滤条件是由书签组成的数组，则需将该数组转换为SafeArray，然后再封装到一个VARIANT或_variant_t型的变量中，再赋给Filter属性。

　　15、索引与排序

　　（1）、建立索引

　　当以某个字段为关键字用Find方法查找时，为了加快速度可以以该字段为关键字在记录集内部临时建立索引。只要将该字段的Optimize属性设置为true即可，例如：

pRst->Fields->GetItem("姓名")->Properties->
GetItem("Optimize")->PutValue("True");
pRst->Find("姓名 = '赵薇'",1,adSearchForward);
......
pRst->Fields->GetItem("姓名")->Properties->
GetItem("Optimize")->PutValue("False");
pRst->Close(); 

　　说明：Optimize属性是由Provider提供的属性（在ADO中称为动态属性），ADO本身没有此属性。

　　（2）、排序

　　要排序也很简单，只要把要排序的关键字列表设置到Recordset对象的Sort属性里即可，例如：

pRstAuthors->CursorLocation = adUseClient;
pRstAuthors->Open("SELECT * FROM mytable",
_variant_t((IDispatch *) pConnection),
adOpenStatic, adLockReadOnly, adCmdText);
......
pRst->Sort = "姓名 DESC, 年龄 ASC"; 

　　关键字（即字段名）之间用逗号隔开，如果要以某关键字降序排序，则应在该关键字后加一空格，再加DESC（如上例）。升序时ASC加不加无所谓。本操作是利用索引进行的，并未进行物理排序，所以效率较高。
但要注意，在打开记录集之前必须将记录集的CursorLocation属性设置为adUseClient，如上例所示。Sort属性值在需要时随时可以修改。

　　16、事务处理

　　ADO中的事务处理也很简单，只需分别在适当的位置调用Connection对象的三个方法即可，这三个方法是：

　　（1）、在事务开始时调用

pCnn->BeginTrans(); 

　　（2）、在事务结束并成功时调用

pCnn->CommitTrans (); 

　　（3）、在事务结束并失败时调用

pCnn->RollbackTrans (); 

　　在使用事务处理时，应尽量减小事务的范围，即减小从事务开始到结束（提交或回滚）之间的时间间隔，以便提高系统效率。需要时也可在调用BeginTrans()方法之前，先设置Connection对象的IsolationLevel属性值，详细内容参见MSDN中有关ADO的技术资料
三、使用ADO编程常见问题解答

　　以下均是针对MS SQL 7.0编程时所遇问题进行讨论。

　　1、连接失败可能原因

　　Enterprise Managemer内，打开将服务器的属性对话框，在Security选项卡中，有一个选项Authentication。

　　如果该选项是Windows NT only，则你的程序所用的连接字符串就一定要包含Trusted_Connection参数，并且其值必须为yes，如：

"Provider=SQLOLEDB;Server=888;Trusted_Connection=yes"
";Database=master;uid=lad;"; 

　　如果不按上述操作，程序运行时连接必然失败。

　　如果Authentication选项是SQL Server and Windows NT，则你的程序所用的连接字符串可以不包含Trusted_Connection参数，如：

"Provider=SQLOLEDB;Server=888;Database=master;uid=lad;pwd=111;"; 

　　因为ADO给该参数取的默认值就是no，所以可以省略。我认为还是取默认值比较安全一些。

　　2、改变当前数据库的方法

　　使用Tansct-SQL中的USE语句即可。

　　3、如何判断一个数据库是否存在

　　(1)、可打开master数据库中一个叫做SCHEMATA的视图，其内容列出了该服务器上所有的数据库名称。

　　(2) 、更简便的方法是使用USE语句，成功了就存在；不成功，就不存在。例如：

try{
m_pConnect->Execute ( _bstr_t("USE INSURANCE_2002"),NULL,
adCmdText│adExecuteNoRecords );
}
catch (_com_error &e)
{
blSuccess=FALSE;
CString str="数据库INSURANCE_2002不存在！/n";
str+=e.Description();
::MessageBox(NULL,str,"警告",MB_OK │ MB_ICONWARNING); 
} 

　　4、判断一个表是否存在

　　（1）、同样判断一个表是否存在，也可以用是否成功地打开它来判断，十分方便，例如：

try{
m_pRecordset->Open(_variant_t("mytable"),
_variant_t((IDispatch *)m_pConnection,true), adOpenKeyset,
adLockOptimistic, adCmdTable);
}
catch (_com_error &e)
{
::MessageBox(NULL,"该表不存在。","提示",MB_OK │ MB_ICONWARNING);
} 

　　(2)、要不然可以采用麻烦一点的办法，就是在MS-SQL服务器上的每个数据库中都有一个名为sysobjects的表，查看此表的内容即知指定的表是否在该数据库中。

　　(3)、同样，每个数据库中都有一个名为TABLES的视图(View)，查看此视图的内容即知指定的表是否在该数据库中。

　　5、类型转换问题

　　（1）、类型VARIANT_BOOL

　　类型VARIANT_BOOL等价于short类型。The VARIANT_BOOL is equivalent to short. see it's definition below: 
typdef short VARIANT_BOOL

　　（2）、_com_ptr_t类的类型转换

　　_ConnectionPtr可以自动转换成IDspatch*类型，这是因为_ConnectionPtr实际上是_com_ptr_t类的一个实例，而这个类有此类型转换函数。

　　同理，_RecordsetPtr和_CommandPtr也都可以这样转换。

　　（3）、_bstr_t和_variant_t类

　　在ADO编程时，_bstr_t和_variant_t这两个类很有用，省去了许多BSTR和VARIANT类型转换的麻烦。

　　6、打开记录集时的问题

　　在打开记录集时，在调用Recordset的Open方法时，其最后一个参数里一定不能包含adAsyncExecute，否则将因为是异步操作，在读取数据时无法读到数据。

　　7、异常处理问题

　　对所有调用ADO的语句一定要用try和catch语句捕捉异常，否则在发生异常时，程序会异常退出。

　　8、使用SafeArray问题

　　在初学使用中，我曾遇到一个伤脑筋的问题，一定要注意：

　　在定义了SAFEARRAY的指针后，如果打算重复使用多次，则在中间可以调用::SafeArrayDestroyData释放数据，但决不能调用::SafeArrayDestroyDescriptor，否则必然出错，即使调用SafeArrayCreate也不行。例如：

SAFEARRAY *psa;
......
//When the data are no longer to be used:
::SafeArrayDestroyData( psa); 

　　我分析在定义psa指针时，一个SAFEARRAY的实例（也就是SAFEARRAY描述符）也同时被自动建立了。但是只要一调用::SafeArrayDestroyDescriptor，描述符就被销毁了。

　　所以我认为::SafeArrayDestroyDescriptor可以根本就不调用，即使调用也必须在最后调用。

　　9、重复使用命令对象问题

　　一个命令对象如果要重复使用多次（尤其是带参数的命令），则在第一次执行之前，应将它的Prepared属性设置为TRUE。这样会使第一次执行减慢，但却可以使以后的执行全部加快。

　　10、绑定字符串型字段问题

　　如果要绑定的字段是字符串类型，则对应的字符数组的元素个数一定要比字段长度大2（比如m_szau_fname[22]，其绑定的字段au_fname的长度实际是20），不这样绑定就会失败。

　　11、使用AppendChunk的问题

　　当用AddNew方法刚刚向记录集内添加一个新记录之后，不能首先向一个长数据字段（image类型）写入数据，必须先向其他字段写入过数据之后，才能调用AppendChunk写该字段，否则出错。也就是说，AppendChunk不能紧接在AddNew之后。另外，写入其他字段后还必须紧接着调用AppendChunk，而不能调用记录集的Update方法后，才调用AppendChunk，否则调用AppendChunk时也会出错。换句话说，就是必须AppendChunk在前，Update在后。因而这个时候就不能使用带参数的AddNew了，因为带参数的AddNew会自动调用记录集的Update，所以AppendChunk就跑到Update的后面了，就只有出错了！因此，这时应该用不带参数的AddNew。

　　我推测这可能是MS SQL 7.0的问题，在MS SQL 2000中则不存在这些问题，但是AppendChunk仍然不能在Update之后。

　　四、小结

　　一般情况下，Connection和Command的Execute用于执行不产生记录集的命令，而Recordset的Open用于产生一个记录集，当然也不是绝对的。特别Command主要是用于执行参数化的命令，可以直接由Command对象执行，也可以将Command对象传递给Recordset的Open。

　　本文中的代码片断均在VC++ 6.0、Windows NT 4.0 SP6和MS SQL 7.0中调试通过。相信您读过之后，编写简单的数据库程序应该没有问题了。当然要编写比较实用的、复杂一点的程序，还需要对OLE DB、ADO以及数据库平台再多了解一点，希望您继续努力，一定会很快成功的！详细参考资料请参见微软MSDN July 2000光盘或MS SQL 7.0在线文档资料（Books online）。文中难免有错误和不妥之处，敬请各位批评指正！

