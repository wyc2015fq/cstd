# 用SQLConfigDataSource设置odbc - ljx0305的专栏 - CSDN博客
2008年07月09日 14:40:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3936
10.9.1   自动注册DSN   
    
  　　无论是用ODBC还是DAO类，在访问ODBC数据源以前，都必须先注册DSN。通过调用函数SQLConfigDataSource，可以实现自动注册DSN。当然，用DAO可以直接访问一些常用的数据库，而不必通过ODBC来访问（参见10.8.3）。   
    
  　　清单10.14的代码演示了注册DSN的过程。该段代码先用SQLConfigDataSource注册一个名为MYDB的FoxPro   2.5数据源，然后调用CDatabase::Open函数与该数据源连接。注意在使用这段代码时，要包含afxdb.h头文件，读者可以把该文件放到stdafx.h中。   
    
  　   
    
  清单10.14   自动注册DSN   
    
  #include   “afxdb.h”   
    
  .   .   .   
    
  CDatabase   db;   
    
  if(!SQLConfigDataSource(NULL,ODBC_ADD_DSN,   "Microsoft   FoxPro   Driver   (*.dbf)",     
    
  "DSN=MYDB/0"   
    
  "DefaultDir=c://mydir/0"   
    
  "FIL=FoxPro   2.5/0"   
    
  "DriverId=280/0"))   
    
  {   
    
  AfxMessageBox("Can't   add   DSN!");   
    
  return   ;   
    
  }   
    
  TRY   
    
  {   
    
  db.Open("MYDB");   
    
  }   
    
  CATCH(CDBException,   e)   
    
  {   
    
  AfxMessageBox(e->m_strError);   
    
  return;   
    
  }   
    
  END_CATCH   
    
  　　在注册DSN时，SQLConfigDataSource函数的第二个参数应该是ODBC_ADD_DSN，第三个参数指定了ODBC驱动程序，它的写法可以参照ODBC管理器的驱动程序页。第四个参数说明了数据源的各种属性，它是由一系列子串构成，每个子串的末尾必须有一个“/0”。最重要的属性是“DSN＝数据源名”，其它属性包括缺省目录以及驱动程序版本信息。在上例中，使用FoxPro   2.5的版本，所以DriverId应该是280，对应地，FoxPro   2.6的DriverId是536，FoxPro   2.0的是24。   
    
  　　如果读者对SQLConfigDataSource函数的第四个参数的设置方法不清楚，那么可以打开Windows的注册表看一看已注册过的DSN的各项属性。运行RegEdit可以打开注册表，然后依次打开HKEY_CURRENT_USER->Software->ODBC->ODBC.INI，就可以看到已注册的DSN，打开各DSN，则可以看到该DSN的各项属性，读者可以仿照DSN属性来设置第四个参数。   
    
  DSN的名字必须唯一，因此如果要注册的DSN已被注册过，那么SQLConfigDataSource就修改原来DSN的属性。   
（1）在StdAfx.h文件中添加   
  #include   <afxdb.h>   
  #include   <afxtempl.h>   
  #include   <odbcinst.h>   
  （2）在适当位置添加   
  //创建ODBC数据源   
    
  //创建ODBC数据源   
  if   (AfxMessageBox("选择是否创建数据源sss",   MB_YESNO)   !=   IDYES)   
  return   ;   
    
  //创建方法3   
  if(SQLConfigDataSource(m_pMainWnd->m_hWnd,ODBC_ADD_DSN,"Microsoft   Access   Driver   (*.mdb)",   
  "DSN=Personnel/0"   
  "DBQ=C://My   Documents//dq.mdb/0"   
  "Description=ODBC数据源/0"   
  "DataDirectory=C://My   Documents/0"   
  "/0"))   
  AfxMessageBox("创建成功!");   
  else   
  AfxMessageBox("取消创建或创建失败!");
把/0换成/1，完成整个字符串后，再把/1换成/0
