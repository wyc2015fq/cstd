# VC6.0连接SqlServer2000数据库 - xqhrs232的专栏 - CSDN博客
2017年10月17日 11:20:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：207
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[http://blog.csdn.net/xdhywj/article/details/7775715](http://blog.csdn.net/xdhywj/article/details/7775715)
相关文章
1、[vc通过ADO连接sql server 2000的核心代码](http://www.cppblog.com/Lee7/archive/2008/06/18/53823.html)----[http://www.cppblog.com/Lee7/archive/2009/09/23/53823.html](http://www.cppblog.com/Lee7/archive/2009/09/23/53823.html)
2、用MFC(VC6.0)连接sql server2000----[http://blog.csdn.net/qq369846642/article/details/42833919](http://blog.csdn.net/qq369846642/article/details/42833919)
3、VC连接SQL2000数据库----[http://blog.sina.com.cn/s/blog_6748cd1f0100k29q.html](http://blog.sina.com.cn/s/blog_6748cd1f0100k29q.html)
4、VC++环境下连接SQL_Server数据库方法指导----[https://wenku.baidu.com/view/ad14292e647d27284b7351a0.html](https://wenku.baidu.com/view/ad14292e647d27284b7351a0.html)
5、vc 使用ado 连接使用sql server 2000数据库----[http://download.csdn.net/download/mcd_23/3194123](http://download.csdn.net/download/mcd_23/3194123)
6、VC++与SQLServer2000连接----[https://wenku.baidu.com/view/5d2be87352ea551810a687b6.html](https://wenku.baidu.com/view/5d2be87352ea551810a687b6.html)
7、VC6.0 ADO连接sql server2000时连接不上，不存在或拒绝访问----[http://bbs.csdn.net/topics/390341628?page=1](http://bbs.csdn.net/topics/390341628?page=1)
1、首先安装好SqlServer2000数据库
2、本文将数据库的连接与释放封装成一个类，具体实现如下所示：
1）创建DataBaseLink.h文件：
//该类用来实现数据库的连接
class DataBaseLink
{
public:
//构造函数
DataBaseLink();
//析构函数
~DataBaseLink();
//打开数据库函数
void OpenDataBase();
//关闭数据库函数
void CloseDataBase();
//打开记录集
_RecordsetPtr& OpenRecordset(CString sql);
//关闭记录集
void CloseRecordset();
//返回记录集个数
UINT GetRecordsetCount(_RecordsetPtr pRecordset);
public:
//申明指针
_ConnectionPtr m_pConnection;
_RecordsetPtr m_pRecordset;
};
2）实现头文件中的函数
#include "stdafx.h"
#include "DataBaseLink.h"
//构造函数
DataBaseLink::DataBaseLink()
{
}
//析构函数
DataBaseLink::~DataBaseLink()
{
}
//打开数据库函数
void DataBaseLink::OpenDataBase()
{
//初始化操作
::CoInitialize(NULL);
//创建ADO连接操作
//AfxOleInit();
HRESULT hr;
hr = m_pConnection.CreateInstance(_uuidof(Connection));
//判断是否为空
if(FAILED(hr))
{
AfxMessageBox("字符数据为空");
return;
}
try
{
//打开本地数据库student
CString str = "Provider=SQLOLEDB.1;Server=GCXY01;Database=student;UID=xdhywj;PWD=12345";
//打开数据库
m_pConnection->Open((_bstr_t)str,"","",-1);
//测试是否连接成功
AfxMessageBox("数据库连接成功！！");
}
//异常捕获
catch(_com_error *e)
{
AfxMessageBox("数据库连接失败！！");
AfxMessageBox(e->ErrorMessage());
return;
}
}
//关闭数据库函数
void DataBaseLink::CloseDataBase()
{
//关闭连接
if(m_pConnection != NULL)
{
m_pConnection->Close();
}
::CoUninitialize();
}
//打开记录集
_RecordsetPtr& DataBaseLink::OpenRecordset(CString sql)
{
ASSERT(!sql.IsEmpty());
try
{
m_pRecordset.CreateInstance(__uuidof(Recordset));
m_pRecordset->Open(_bstr_t(sql),m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
}
catch(_com_error e)
{
AfxMessageBox(e.Description());
}
return m_pRecordset;
}
//关闭记录集
void DataBaseLink::CloseRecordset()
{
if(m_pRecordset->GetState() == adStateOpen)
{
m_pRecordset->Close();
}
}
//返回记录集个数
UINT DataBaseLink::GetRecordsetCount(_RecordsetPtr pRecordset)
{
int nCount = 0;
try
{
//移动记录集
pRecordset->MoveFirst();
}
catch(...)
{
return 0;
}
if(pRecordset->adoEOF)
{
return 0;
}
while(!pRecordset->adoEOF)
{
pRecordset->MoveNext();
nCount = nCount + 1;
}
pRecordset->MoveFirst();
return nCount;
}

