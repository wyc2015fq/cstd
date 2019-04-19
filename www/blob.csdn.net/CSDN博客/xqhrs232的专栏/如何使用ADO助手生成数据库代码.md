# 如何使用ADO助手生成数据库代码 - xqhrs232的专栏 - CSDN博客
2017年10月17日 11:43:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：123
个人分类：[SQLServer/Oracle数据库技术](https://blog.csdn.net/xqhrs232/article/category/906969)
原文地址::[http://blog.csdn.net/zyq5945/article/details/7998001](http://blog.csdn.net/zyq5945/article/details/7998001)
ADO助手最新版本为V1.20，[下载地址](http://download.csdn.net/detail/zyq5945/4581254)，[例子Demo](http://blog.csdn.net/zyq5945/article/details/8050386)。
ADO助手是一个获取ADO连接字符串，测试SQL命令的辅助软件。ADO助手还可以用来以插入SQL语句形式导出导入记录，目前提供了插件还可以生成MFC和C#数据库代码，大大简化编写数据库代码复杂度和缩短了编写数据库代码时间。
使用ADO助手生成数据库代码大致有下面几个步骤：
**一.设置数据库的连接字符串**
1.可以在如下图的下拉列表中选择相应的连接字符串
![](https://img-my.csdn.net/uploads/201209/19/1348066449_4524.png)
2.点击“连接字符串”按钮，在“数据链接属性”中编辑连接字符串，在“数据链接属性”中编辑连接字符串可以参考《[自己如何获取ADO连接字符串](http://blog.csdn.net/zyq5945/article/details/5586423)》。
![](https://img-my.csdn.net/uploads/201209/19/1348066075_7868.png)
当然也可以选择相应的连接字符串后，再点击“连接字符串”按钮类进行编辑。
**二.选择数据连接建立的方式**
![](https://img-my.csdn.net/uploads/201209/19/1348066094_9160.png)
**三.建立数据库连接**
![](https://img-my.csdn.net/uploads/201209/19/1348066111_7675.png)
**四.编写需要生成的数据库代码SQL，点击“打开记录集”按钮**
![](https://img-my.csdn.net/uploads/201209/19/1348066128_5889.png)
**五.在数据库代码对话框中的代码类型和配置选项按需求选择生成的数据库代码类型**
![](https://img-my.csdn.net/uploads/201209/19/1348066148_6739.png)
代码类型是根据插件目录下的动态链接库动态生成的，和动态链接库同名，配置选项是在Ini目录下同名配置文件内容。
**六.最后是点击“生成代码”按钮，弹出保存对话框生成最终的数据库代码**
![](https://img-my.csdn.net/uploads/201209/19/1348066161_2630.png)
生成的MFC数据库类格式如下：
**[cpp]**[view
 plain](http://blog.csdn.net/zyq5945/article/details/7998001#)[copy](http://blog.csdn.net/zyq5945/article/details/7998001#)
- /*----------------------------------------------------------------------------------------------------------------
- Version:    AdoAssist 1.20
-    Time:    2012-09-19 22:06:17
-     SQL:    SELECT * FROM tbl_Demo
- ----------------------------------------------------------------------------------------------------------------*/
- #pragma once
- 
- class CDBClassName  
- {  
- public:  
- long            m_lID;  //0 ID  adInteger
-     CString         m_strGuid;  //1 Guid    adVarWChar
-     CString         m_strName;  //2 Name    adVarWChar
- long            m_lAge; //3 Age adInteger
- 
- public:  
- long GetID() const
-     {  
- returnthis->m_lID;  
-     }  
- 
- void GetID(long& tID) const
-     {  
-         tID = this->m_lID;  
-     }  
- 
- void SetID(constlong& tID)  
-     {  
- this->m_lID = tID;  
-     }  
- 
-     CString GetGuid() const
-     {  
- returnthis->m_strGuid;  
-     }  
- 
- void GetGuid(CString& tGuid) const
-     {  
-         tGuid = this->m_strGuid;  
-     }  
- 
- void SetGuid(const CString& tGuid)  
-     {  
- this->m_strGuid = tGuid;  
-     }  
- 
-     CString GetName() const
-     {  
- returnthis->m_strName;  
-     }  
- 
- void GetName(CString& tName) const
-     {  
-         tName = this->m_strName;  
-     }  
- 
- void SetName(const CString& tName)  
-     {  
- this->m_strName = tName;  
-     }  
- 
- long GetAge() const
-     {  
- returnthis->m_lAge;  
-     }  
- 
- void GetAge(long& tAge) const
-     {  
-         tAge = this->m_lAge;  
-     }  
- 
- void SetAge(constlong& tAge)  
-     {  
- this->m_lAge = tAge;  
-     }  
- 
- 
- public:  
- // Database Interface
- bool AdoGetRecord(const CAdoRecordset& rs, DWORD dwFlag = 0);  
- bool AdoSetRecord(const CAdoRecordset& rs, DWORD dwFlag = 0) const;  
- };  
- 
- #pragma warning(disable:4100)// Database Interface
- inlinebool CDBClassName::AdoGetRecord(const CAdoRecordset& rs, DWORD dwFlag)  
- {  
-     ADO_GETFIELDVALUE(_T("ID"), m_lID); //0
-     ADO_GETFIELDVALUE(_T("Guid"), m_strGuid);   //1
-     ADO_GETFIELDVALUE(_T("Name"), m_strName);   //2
-     ADO_GETFIELDVALUE(_T("Age"), m_lAge);   //3
- 
- /*
-     ADO_GETFIELDVALUE((long)0, m_lID);  //ID
-     ADO_GETFIELDVALUE((long)1, m_strGuid);  //Guid
-     ADO_GETFIELDVALUE((long)2, m_strName);  //Name
-     ADO_GETFIELDVALUE((long)3, m_lAge); //Age
-     */
- 
- returntrue;  
- }  
- 
- // Database Interface
- inlinebool CDBClassName::AdoSetRecord(const CAdoRecordset& rs, DWORD dwFlag) const
- {  
- //ADO_SETFIELDVALUE(_T("ID"), m_lID);   //0
-     ADO_SETFIELDVALUE(_T("Guid"), m_strGuid);   //1
-     ADO_SETFIELDVALUE(_T("Name"), m_strName);   //2
-     ADO_SETFIELDVALUE(_T("Age"), m_lAge);   //3
- 
- /*
-     //ADO_SETFIELDVALUE((long)0, m_lID);    //ID
-     ADO_SETFIELDVALUE((long)1, m_strGuid);  //Guid
-     ADO_SETFIELDVALUE((long)2, m_strName);  //Name
-     ADO_SETFIELDVALUE((long)3, m_lAge); //Age
-     */
- 
- returntrue;  
- }  
- #pragma warning(default:4100)
关于配置文件：每个数据库代码插件都有同名的在可执行目录下INI目录配置文件
配置文件格式：数据库字段类型=相应的语言数据变量|数据库类变量前缀
**[plain]**[view
 plain](http://blog.csdn.net/zyq5945/article/details/7998001#)[copy](http://blog.csdn.net/zyq5945/article/details/7998001#)
- adEmpty=Object|m_  
- adTinyInt=Int8|m_  
- adSmallInt=Int16|m_  
- adInteger=Int32|m_  
- adBigInt=Int64|m_  
- adUnsignedTinyInt=UInt8|m_  
- adUnsignedSmallInt=UInt16|m_  
- adUnsignedInt=UInt32|m_  
- adUnsignedBigInt=UInt64|m_  
- adSingle=Single|m_  
- adDouble=Double|m_  
- adCurrency=Object|m_  
- adDecimal=Decimal|m_  
- adNumeric=Decimal|m_  
- adBoolean=Boolean|m_  
- adError=Object|m_  
- adUserDefined=Object|m_  
- adVariant=Object|m_  
- adIDispatch=Object|m_  
- adIUnknown=Object|m_  
- adGUID=Guid|m_  
- adDate=DateTime|m_  
- adDBDate=DateTime|m_  
- adDBTime=DateTime|m_  
- adDBTimeStamp=DateTime|m_  
- adBSTR=String|m_  
- adChar=String|m_  
- adVarChar=String|m_  
- adLongVarChar=String|m_  
- adWChar=String|m_  
- adVarWChar=String|m_  
- adLongVarWChar=String|m_  
- adBinary=Object|m_  
- adVarBinary=Object|m_  
- adLongVarBinary=Object|m_  
- adChapter=Object|m_  
- adFileTime=DateTime|m_  
- adPropVariant=Object|m_  
- adVarNumeric=Object|m_  
- adArray=Object|m_  
一些功能说明以及建议：
1.如果生成的字段有非变量字符（中文等字符），请使用别名查询转换成变量字符。
2.ORACLE中查询的字符名称默认是大写的， 可以用类似的别名FIELDNAME AS "FieldName"进行查询。
3.联合查询的字段名称不能有重复，否则生成的类将可能无法编译过去。
4.当一个查询A是查询B的子集且这两个查询有相互赋值情况时最好是采用继承关系来生成类。
ADO助手版本和发布时间
[2010-09-06](http://download.csdn.net/detail/zyq5945/2675267)
+初始版本发布
[2011-11-09](http://download.csdn.net/detail/zyq5945/3775756)
-修复有时候用DataGrid显示报错“The rowset is not bookmarkable.”
+增加根据SQL生产MFC数据库类的功能
+增加导入和导出记录集的功能
+增加对多记录集的支持功能
+增加常用的ADO连接字符串的下拉选择
[2012-09-19](http://download.csdn.net/detail/zyq5945/4581254)
+增加C#数据库代码生成
ADO助手说明
1."在DataGrid显示"会影响CursorType和LockType的输出
2."在DataGrid显示"状态改变后请重新点击"建立连接",否则该属性不会设置到已经建立的连接中.
有任何bug请给Email:ｚｙｑ５９４５@126.com（请把中文字符改成英文字符）
