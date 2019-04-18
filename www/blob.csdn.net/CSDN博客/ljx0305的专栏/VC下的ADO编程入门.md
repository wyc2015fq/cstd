# VC下的ADO编程入门 - ljx0305的专栏 - CSDN博客
2008年05月05日 19:38:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1350
VC下的ADO编程入门
n新建一基于对话框的应用程序zjy，在对话框上添加一个列表框和一个按钮，用ClassWizard给列表框添加一个变量m_list
n在StdAfx.h头文件的所有include语句下面添加如下一条语句
#import "c:/program files/common files/system/ado/msado15.dll" no_namespace rename ("EOF","adoEOF")
n在zjyDlg.h头文件处添加两个变量
         _ConnectionPtr m_pConnection;
         _RecordsetPtr m_pRecordset;
n在zjyDlg.cpp的BOOL CZjyDlg::OnInitDialog()的return TRUE;语句前添加如下语句
         if(!AfxOleInit())
         {
                   AfxMessageBox("COM Error!");
         }
n添加按钮的消息响应函数并写入下面代码
void CZjyDlg::OnButton1() 
{
         m_list.ResetContent();
         int i=0;
         m_pConnection.CreateInstance(__uuidof(Connection));
         m_pRecordset.CreateInstance(__uuidof(Recordset));
         try{
                   m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=E://ActiveX 数据对象//VC_ACCESS//zjy//MyDb1.mdb;","","",0); 
                   m_pRecordset->Open("SELECT * FROM MyTable1",(IDispatch *)m_pConnection,adOpenDynamic,adLockOptimistic,adCmdText);
                   m_pRecordset->MoveFirst();
                   while(!m_pRecordset->adoEOF)
                   {
                            i=i+1;
                            _variant_t theValue = m_pRecordset->GetCollect("n1");
                            if(theValue.vt != VT_NULL)
                                     m_list.AddString((char *)_bstr_t(theValue));
                            m_pRecordset->MoveNext();
                   }
                   m_pRecordset->Close();
                   m_pConnection->Close();
         }
         catch(_com_error e) 
         {
                   AfxMessageBox(e.ErrorMessage());
         }
         char str[32];
         sprintf(str,"table have %d record",i);
         AfxMessageBox(str);
         m_pRecordset = NULL;
         m_pConnection = NULL; 
}
完成的功能就是把Access数据库中的某表的某一字段添加到列表框中。下一篇介绍把SQL SERVER数据库中的某表添加到列表视图中。
在正式进入ADO编程之旅前，我们先来熟悉一下Connection Command Recordset三个重要对象的一些重要方法，这样在今后写ADO代码时，给方法传递正确的参数，Let's go
Connection方法
HRESULT Open ( _bstr_t ConnectionString, _bstr_t UserID, _bstr_t Password, long Options )
参数
1.ConnectionString   可选，包含连接信息
2.UserID   可选，包含建立连接时所使用的用户名称
3.Password   可选，包含建立连接时所用密码
4.OpenOptions   可选，ConnectOptionEnum 值。如果设置为 adConnectAsync，则异步打开连接。adConnectUnspecified为默认选项，表示同步打开连接
_RecordsetPtr Execute ( _bstr_t CommandText, VARIANT * RecordsAffected, long Options ) 
参数
1.CommandText是命令字符串，通常是SQL命令，也可以是表名、存储过程等
2.RecordsAffected 可选，是操作完成后所影响的行数
3.Options 可选，解释CommandText参数的方式，Options可以是CommandTypeEnum或ExecuteOptionEnum枚举类型值
|类型|说明|
|----|----|
|adCmdUnspecifed=-1|未描述CommandType属性|
|adCmdText=1|指示提供者应将 CommandText 赋值为命令的文本定义。|
|adCmdTableDirect|指示提供者应从 CommandText 命名的表中返回所有行。|
|adCmdTable=2|指示提供者应将 CommandText 赋值为表名。|
|adCmdStoredProc=4|指示提供者应将 CommandText 赋值为存储过程。|
|adCmdUnknown=8|指示 CommandText 参数中的命令类型未知。|
|adExecuteAsync|指示命令应该异步执行。|
|adFetchAsync|指示 CacheSize 属性指定的初始数量之后的行应异步提取。|
Command方法
_ RecordsetPtr  Execute (VARIANT * RecordsAffected, VARIANT * Parameters,long Options)
参数
1.RecordsAffected 可选，返回受操作影响的记录数
2.Parameters 可选，包含所使用的具体参数，是一个VARIANT 类型数组
3.Options可选，指示提供程序如何解释CommandText属性，请参见Connection对象的Execute 方法
ParameterPtr CreateParameter(_bstr_t Nmae,enum DataTypeEnum Type,enum ParameterDirectionEnum Direction,ADO_LONGPTR size,const _variant_t &value)
Recordset方法
HRESULT Open ( const _variant_t & Source, const _variant_t & ActiveConnection, enum CursorTypeEnum CursorType, enum LockTypeEnum LockType, long Options ) 
参数
1.Source可选，指定记录集的源，可以是命令对象，SQL语句，表，存储过程等
2.ActiveConnection可选，可以是一个已经建立好的连接对象（我们需要用Connection对象指针来构造一个_variant_t对象) 或是一个包含连接信息的字符串
3.CursorType可选，游标类型，它可以是以下值之一,请看CursorTypeEnum这个枚举结构:
|枚举值|说明|
|----|----|
|adOpenUnspecified = -1|不作特别指定|
|adOpenForwardOnly = 0|向前滚静态游标。这种游标只能向前浏览记录集，比如用MoveNext向前滚动,这种方式可以提高浏览速度。但诸如BookMark,RecordCount,AbsolutePosition,AbsolutePage都不能使用|
|adOpenKeyset = 1|采用这种游标的记录集看不到其它用户的新增、删除操作，但对于更新原有记录的操作对你是可见的。|
|adOpenDynamic = 2|动态游标。所有数据库的操作都会立即在各用户记录集上反应出来。|
|adOpenStatic = 3|静态游标。它为你的记录集产生一个静态备份，但其它用户的新增、删除、更新操作对你的记录集来说是不可见的。|
4.LockType锁定类型，它可以是以下值之一，请看LockTypeEnum这个枚举结构：
|枚举值|说明|
|----|----|
|adLockUnspecified = -1|未指定|
|adLockReadOnly = 1|只读记录集|
|adLockPessimistic = 2|悲观锁定方式。数据在更新时锁定其它所有动作，这是最安全的锁定机制|
|adLockOptimistic = 3|乐观锁定方式。只有在你调用Update方法时才锁定记录。在此之前仍然可以做数据的更新、插入、删除等动作|
|adLockBatchOptimistic = 4|乐观分批更新。编辑时记录不会锁定，更改、插入及删除是在批处理模式下完成。|
5.Options请参考本文中对Connection对象或Command对象的Execute方法的介绍
HRESULT Move (ADO_LONGPTR NumRecords,const _variant_t &start)
nadBookmarkCurrent
nadBookmardFirst
nadBookmarkLast
_RecordsetPtr NextRecordset(VARIANT *RecordsAffected)
HRESULT AddNew(const _varinat_t & FieldList,const _variant_t &values)
HRESULT Delete(enum AffectedEnum AffectRecords)
nadAffectCurrent
nadAffectGroup
nadAffectAll
HRESULT Update(const _variant_t &Fields,const _variant_t &values)
_variant_t GetRows(long Rows,const _variant_t &start,const _variant_t & Fields)
HRESULT Requery(long Options)
VARIANT_BOOL Supports(enum CursorOptionEnum CursorOptions)
|常量|说明|
|----|----|
|adAddNew|可使用AddNew方法添加新记录。|
|adApproxPosition|可读取并设置AbsolutePosition和AbsolutePage属性。|
|adBookmark|可使用Bookmark属性访问特定记录。|
|adDelete|可以使用Delete方法删除记录。|
|adHoldRecords|可以检索多个记录或者更改下一个检索位置而不必提交所有挂起的更改。|
|adMovePrevious|可使用MoveFirst和MovePrevious方法，以及Move或GetRows方法将当前记录位置向后移动而不必请求书签。|
|adResync|通过Resync方法，使用在现行数据库中可见的数据更新游标。|
|adUpdate|可使用Update方法修改现有的数据。|
|adUpdateBatch|可以使用批更新（UpdateBatch和CancelBatch方法）将更改按组传送给提供者。|
在正式进入本主题前，让我们先来看一下COM API编程
#include <windows.h> 
#include <objbase.h>
#include <initguid.h> 
#include "adoid.h" 
#include "adoint.h" 
struct InitOle
{
         InitOle(){::CoInitialize(NULL);}
         ~InitOle(){::CoUninitialize();}
}_init_InitOle_;
void main(int argc, char* argv[])
{
         HRESULT hr = S_OK; 
         ADORecordset *Rs1 = NULL; 
         VARIANT Source; 
         VARIANT Connect; 
         VariantInit(&Source); 
         VariantInit(&Connect); 
         Source.vt = VT_BSTR; 
         Source.bstrVal = ::SysAllocString(L"SELECT * FROM Customers"); 
         Connect.vt = VT_BSTR; 
         Connect.bstrVal = ::SysAllocString(L"Provider=SQLOLEDB; data source=127.0.0.1;initial catalog=Northwind; user id=sa; password=;"); 
         hr = CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_INPROC_SERVER, IID_IADORecordset, (LPVOID *)&Rs1 ); 
         if(SUCCEEDED(hr))
         {
                   Rs1->put_CursorLocation(adUseClient);
                   hr = Rs1->Open(Source, Connect, adOpenForwardOnly, adLockReadOnly,-1); 
                   long n;
                   Rs1->get_RecordCount(&n);
                   Rs1->Close(); 
                   Rs1->Release(); Rs1 = NULL; 
                   ::MessageBox(NULL, "Success!", "", MB_OK); 
                   printf("%d",n);
         }
}
本例完成的功能就是取得数据库中某表的记录数。
好了，现在我们正式进入本主题，Let's go
n新建一个基于对话的应用程序zjy，在对话框上添加一个按钮和列表控件，并把列表控件的样式设置为报表视图，还有用classwizard给列表控件添加一个控件变量m_list
n在StdAfx.h头文件中添加一条如下语句
#import "c:/program files/common files/system/ado/msado15.dll" no_namespace rename("EOF","adoEOF")
n在zjy.cpp开始处添加自动初始化COM库的代码
struct InitOle
{
         InitOle(){::CoInitialize(NULL);}
         ~InitOle(){::CoUninitialize();}
}_init_InitOle_;
n在zjyDlg.h中添加三个成员变量
         bool m_IsConnectionOpen;
         _ConnectionPtr m_pConnection;
         _RecordsetPtr m_pRecordset;
n在zjyDlg.cpp的BOOL CZjyDlg::OnInitDialog()的      return TRUE;前面添加如下代码
// TODO: Add extra initialization here
         HRESULT hr;
         _bstr_t bstrQuery("select * from customers");
         _bstr_t strConnect="Provider=SQLOLEDB; data source=127.0.0.1;initial catalog=Northwind;"; 
         _variant_t vRecsAffected(0L);
         _variant_t vNULL;
         vNULL.vt=VT_ERROR;
         vNULL.scode =DISP_E_PARAMNOTFOUND;
         try
         {
                   hr=m_pConnection.CreateInstance(__uuidof(Connection));
                   if(SUCCEEDED(hr))
                   {
                            hr=m_pConnection->Open(strConnect,_bstr_t(L"sa"),_bstr_t(L""),adConnectUnspecified);
                            if(SUCCEEDED(hr))
                            {
                                     hr=m_pRecordset.CreateInstance(__uuidof(Recordset));
                                     if(SUCCEEDED(hr))
                                     {
                                               m_pRecordset->PutRefActiveConnection(m_pConnection);
                                               hr=m_pRecordset->Open(_variant_t(bstrQuery),vNULL,adOpenDynamic,adLockOptimistic,adCmdText);
                                               if(SUCCEEDED(hr))
                                               {
                                                         m_IsConnectionOpen=true;
                                               }
                                     }
                            }
                   }
         }
         catch(_com_error &e)
         {
                   _bstr_t bstrSource(e.Source());
                   _bstr_t bstrDescription(e.Description());
                   TRACE("Exception throw for classes generated by #import");
                   TRACE("/tCode=%08lx/n", e.Error);
                   TRACE("/tCode meaning = %s/n", e.ErrorMessage);
                   TRACE("/tSource = %s/n", (LPCTSTR)bstrSource);
                   TRACE("/tDescription = %s/n", (LPCTSTR)bstrDescription);
         }
         catch(...)
         {
                   TRACE("Unhandled Exception");
         }
n添加对话框的WM_Destroy消息响应函数
void CZjyDlg::OnDestroy() 
{
         CDialog::OnDestroy();
         // TODO: Add your message handler code here
         if(m_IsConnectionOpen)
         {
                    m_IsConnectionOpen=false;
                    m_pConnection->Close();
                    m_pConnection=NULL;
                    m_pRecordset=NULL;
         }
}
n最后添加按钮的消息响应函数如下
void CZjyDlg::OnButton1() 
{
         // TODO: Add your control notification handler code here
         VARIANT_BOOL valEof;
         HRESULT hr;
         CString strColName;BSTR bstrColName;
         CString strValue;VARIANT varValue;
         long ColCount,i; int RowCount=0;
         Fields * fields=NULL;Field * field=NULL;
         VARIANT varCounter;
         varCounter.vt=VT_I4;
         varCounter.lVal=0;
         try
         {
                   m_list.DeleteAllItems();
                   while(m_list.DeleteColumn(0));
                   hr=m_pRecordset->get_Fields(&fields);
                   if(SUCCEEDED(hr)) hr=fields->get_Count(&ColCount);
                   for(i=0;i<ColCount;i++)
                   {        
                            hr=fields->Item[i]->get_Name(&bstrColName);
                            strColName=bstrColName;
                            int nWidth=m_list.GetStringWidth(strColName)+15;
                            m_list.InsertColumn(i,strColName,LVCFMT_LEFT,nWidth); 
                   }
if(SUCCEEDED(hr)) fields->Release();
                   m_pRecordset->get_adoEOF(&valEof);
                   while(true)
                   {
                            if(valEof)break;
                            m_pRecordset->Fields->get_Item(varCounter,&field);
                            field->get_Value(&varValue);
                            strValue=VariantToString(varValue);
                            m_list.InsertItem(RowCount,strValue);
                            for(i=1;i<ColCount;i++)
                            {
varCounter.lVal =i;
                                     m_pRecordset->Fields->get_Item(varCounter,&field);
                                     field->get_Value(&varValue);
                                     strValue=VariantToString(varValue);
                                     m_list.SetItemText(RowCount,i,(LPCTSTR)strValue);
                            }
                            RowCount++;
                            m_pRecordset->MoveNext();
                            m_pRecordset->get_adoEOF(&valEof);
varCounter.lVal=0;
                   }
                   m_pRecordset->Close(); 
         }
         catch(_com_error &e)
         {
                   _bstr_t bstrSource(e.Source());
                   _bstr_t bstrDescription(e.Description());
                   TRACE("/tCode=%08lx/n", e.Error);
                   TRACE("/tCode meaning = %s/n", e.ErrorMessage);
                   TRACE("/tSource = %s/n", (LPCTSTR)bstrSource);
                   TRACE("/tDescription = %s/n", (LPCTSTR)bstrDescription);
         }
         catch(...)
         {
         }
}
n还有还有，在对话框类添加一个数据类型转换函数
CString CZjyDlg::VariantToString(VARIANT var)
{
CString strValue;
         _variant_t var_t;
         _bstr_t bstr_t;
         time_t cur_time;
         CTime time_value;
         COleCurrency var_currency;
         switch(var.vt)
         {
         case VT_EMPTY:
         case VT_NULL:strValue=_T("");break;
         case VT_UI1:strValue.Format("%d",var.bVal);break;
         case VT_I2:strValue.Format("%d",var.iVal);break;
         case VT_I4:strValue.Format("%d",var.lVal);break;
         case VT_R4:strValue.Format("%f",var.fltVal);break;
         case VT_R8:strValue.Format("%f",var.dblVal);break;
         case VT_CY:
                   var_currency=var;
                   strValue=var_currency.Format(0);break;
         case VT_BSTR:
                   var_t =var;
                   bstr_t=var_t;
                   strValue.Format("%s",(const char *)bstr_t);break;
         case VT_DATE:
                   cur_time=var.date;
                   time_value=cur_time;
                   strValue.Format("%A,%B,%d,%Y");break;
         case VT_BOOL:strValue.Format("%d",var.boolVal);break;
         default:strValue=_T("");break;
         }
         return strValue;
}
(完)
下一篇介绍如何对记录进行增删改操作。
转自:http://blog.csdn.net/callzjy/category/12932.aspx
