# SQLite之C++封装库CppSQLite使用方法 - 深之JohnChen的专栏 - CSDN博客

2018年06月03日 11:16:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：2200


SQLite是一个超轻量级的开源数据库，从官网上下载的source是一个用c写的文件，在C++(VC)中调用时难免会碰到一些问题，这时就可使用CppSQLite。 CppSQLite（最近版本为CppSQLite3）是对SQLite进行二次封装后的C++类库。

SQLite官网：http://www.sqlite.org/

cppsqlite下载地址：https://github.com/lmmir/CppSQLite3

一、使用前需要包含sqlite3头文件和库文件。

（1）CppSQLite3.h 和CppSQLite3.cpp

（2）sqlite3.h、sqlite3.lib和sqlite3.dll

在使用这个类的时候，你需要确保几件事情：首先你要下载上面5个文件。其次就是在你的工程中引入，sqlite3.lib，最后要将CppSQLite3.h 和CppSQLite3.cpp添加到你的工程中。

二、使用方法

1、打开数据库

```cpp
CppSQLite3DB db;  
db.open("data.db");  
db.close();
```

CppSQLite3DB是一个核心类，之后的查询等数据库操作都要借助这个类。打开，关闭就像操作一个文件。

2、查询

```cpp
const char *sRoute = "x://...";  //设置sqlite数据库文件的路径
CppSQLite3DB db;
try
{
    db.open(sRoute);  //打开数据库
    CppSQLite3Query q = db.execQuery("select * from t_table");  //执行查询
    for(int i = 0; i <= q.numFields() - 1; i++)	//遍历并打印表头
    {
        printf("%s\t", q.fieldName(i));
    }
    printf("\n");
    while(!q.eof())		//遍历所有行
    {
         for(int i = 0; i<=q.numFields()-1; i++)
         {
              printf("%s\t", q.getStringField(i));
         }
         printf("\n");
         q.nextRow();
}
q.finalize();//结束查询,释放内存
}
catch(CppSQLite3Exception& e)
{
    printf("%s",e.errorMessage());
}
db.close()
```

CppSQLite3Query是一个查询返回对象，查询完后可以利用此类。这里就使用了CppSQLite3DB的一个函数execQuery，只要将查询sql传入即可。

eof函数：判断是否还有数据；

nextRow函数：移到下一条记录；

getStringField函数：将获得相应字段的内容，以字符串形式返回；

getIntField函数：将获得相应字段的内容，以整形形式返回。

注意的是这个类产生之后要finalize。

3、数据库更改操作

```cpp
db.execDML("insert into t_table(name,age)values('xiaomi',25)");  
db.execDML("update t_table set age = 21 where name = 'xiaomi'");
```

执行insert、update、delete sql语句，无需取返回结果。

4、statement对象

```cpp
CppSQLite3Statementsmt = db.compileStatement("insert into t_table(name,age) values(?,?)");  
    for (int i = 0 ; i < 10; ++i)  
    {   
        smt.bind(1,"test_");  
        smt.bind(2,i);  
        smt.execDML();  
    }  
    smt.finalize();
```

CppSqlite也提供Statement对象，用法也相当简单。注意的是，CppSQLite3Statement和CppSQLite3Query一样，最后也要finalize。

5、SQLite一条SQL语句插入多条记录,批量插入

用SQLite才发现这个语法并非标准SQL，故而SQLite并不支持。网络上推荐的方法：
INSERT INTO TABLE(col1, col2) SELECT val11, val12 UNION ALL SELECT val21, val22 ;
这样的写法是属于复合SQL语句，表示先把两个SELECT的结果集进行无删减的联合，再把联合结果插入到TABLE中。

6、异常处理

```cpp
try  
    {  
       code……
    }  
    catch(CppSQLite3Exception & e)   
    {  
         
    }
```

将操作代码放入try里面，使用CppSQLite3Exception & 来catch。

7、SQLite 插入大量数据慢(多次insert)的解决方法 
sqlite 插入数据很慢的原因：sqlite在没有显式使用事务的时候会为每条insert都使用事务操作，而sqlite数据库是以文件的形式存在磁盘中，就相当于每次访问时都要打开一次文件，如果对数据进行大量的操作，时间都耗费在I/O操作上，所以很慢。
解决方法是显式使用事务的形式提交：因为我们开始事务后，进行的大量操作的语句都保存在内存中，当提交时才全部写入数据库，此时，数据库文件也就只用打开一次。
[https://www.cnblogs.com/likebeta/archive/2012/06/15/2551466.html](https://www.cnblogs.com/likebeta/archive/2012/06/15/2551466.html)

完整实例：

```cpp
#include "CppSQLite3.h"
 
 Class TestSqlite{
     
     //定义db指针
 private:
     CppSQLite3DB* m_pSqlDb;
     TestSqlite()
     {
         m_pSqlDb = NULL;
         Init();
     }
     
     ~TestSqlite()
     {
         if ( m_pSqlDb )
         {
             m_pSqlDb.Close();
             delete m_pSqlDb;
             m_pSqlDb = NULL;
         }    
     }
     //初始化
     BOOL Init()
     {
         //初始化sqlite指针
         if ( m_pSqlDb || !(m_pSqlDb = new CppSQLite3DB))
         {
             return FALSE;
         }
         
         try
         {
             string strDbFile = "D:\\data.s3db";
             m_pSqlDb->open( strDbFile.c_str() );//打开指定位置的本地数据库
         }
         catch (CppSQLite3Exception& e)//处理sqlite异常
         {
             return FALSE;
         }
         
         return TRUE;
     }
 public:
     //读出db中指定名称的表数据
     void ReadAllLine(map<int,int>& mpDbInfo,const string &TblName)
     {
         try
         {
             char szCmd[256];
             sprintf( szCmd, "SELECT id,testnum FROM %s;",TblName);
             CppSQLite3Query query = m_pSqlDb->execQuery( szCmd );//执行查询语句
             while(!query.eof())
             {
                 int id = query.getIntField( "id");    //列项为id的值
                 int testnum = query.getIntField( "testnum");    //列项testnum的值
                 
                 mpDbInfo.insert(make_pair(id,testnum));//插入map
                 query.nextRow();//继续下一行
             }
             query.finalize();//结束查询,释放内存
         }    
         catch (CppSQLite3Exception& e)
         {        
             return;
         }
     }
     
     //更新指定数据
     BOOL DeleteLine(const string& TblName,const int& id,const int& num)
     {
         try
         {
             char szCmd[256];
             sprintf( szCmd, "update %s set num = %d WHERE id=%d;",TblName,num,id);//更新内容
             m_pSqlDb->execDML( szCmd );
         }
         catch (CppSQLite3Exception& e)
         {        
             return FALSE;
         }
         return TRUE;
     }
     
     //删除指定数据
     BOOL DeleteLine(const string& TblName,const int& id)
     {
         try
         {
             char szCmd[256];
             sprintf( szCmd, "DELETE FROM %d WHERE id=%d;", TblName,id);//删除语句
             m_pSqlDb->execDML( szCmd );
         }
         catch (CppSQLite3Exception& e)
         {        
             return FALSE;
         }
         return TRUE;
     }
 };
```

三、注意事项

1、执行select用execQuery，执行delete，update，insert用execDML。

2、数据库文件名传入utf-8格式字符串，否则文件库文件名包含中文时会打开失败。

Gbk转utf-8函数可以参考下面这篇文章或开源库libiconv

[https://blog.csdn.net/bladeandmaster88/article/details/54800287](https://blog.csdn.net/bladeandmaster88/article/details/54800287)
3、如果发现有的数据库能打开，有的打不开，则到官网下载并替换最新版的sqlite.dll即可。

