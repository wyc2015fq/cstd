# 对轻量级C++日志类的增强 - ljx0305的专栏 - CSDN博客
2008年05月15日 20:53:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1301标签：[c++																[struct																[file																[ios																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=ios&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
对轻量级C++日志类的增强
这是网上找到的一个轻量级C++日志类的实现，主要应用了Singleton模式，我增加一个函数tmWrite，主要用来进行前面带时间的行输出。
//log.h
//hujinshan@2004.1.3 Airforce Engineering University
/*
//CLog* CLog::_instance = NULL;
CLog::GetObj().Write( "******* 初始化完成 *******" ) <<endl;
CLog::GetObj().tmWrite( "******* 开始渲染No1. *******" ) <<endl ;
CLog::GetObj()<< "******* 注销钩子 *******" <<endl;
在Clog文件中有 #define CLog /##/ 的语句，
这是为了使日志在release版无效，
使用单行注释，所以如果调用时如果要换行，
请务必在换行最后加上 / 符号,
*/
/*#ifndef _DEBUG
#define CLog /##/
#define _CLOG_H
#endif*/
#ifndef _CLOG_H
#define _CLOG_H
#pragma once
#include <fstream.h>
#include <ctime>
classCLog
{
CLog()
         {       
pf= newofstream ("inmstrace.log", ios::app);
         }
         static CLog* _instance;
ofstream* pf;
public:
         ~CLog()
         {
_instance=0;                   
pf->close();
         }
         static CLog* GetPtr()
         {
                  if(!_instance)
_instance=newCLog;
                  return(_instance);
         }
         static CLog& GetObj()
         {
                  if(!_instance)
_instance=newCLog;
                  return(*_instance);
         }
         template<classT> inline CLog& Write(Tval)
         {
                   (*pf) << val ;
pf->flush(); 
                   return *this;
         }
         template<classT> inline CLog& tmWrite(Tval)
         {
charstrBuf[50];        
time_tltime;
time( &ltime );
        struct tm *today = localtime( &ltime );
sprintf(strBuf, "%4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d ",
today->tm_year+1900, today->tm_mon+1, today->tm_mday, today->tm_hour,
today->tm_min, today->tm_sec);
                   (*pf) << strBuf << val << "/r/n";
pf->flush(); 
                   return *this;
         }
         template<classT> inline CLog& operator<< (Tval)
         {
                   (*pf) << val ;
pf->flush(); 
                   return *this;
         }
};
#endif
//end of file
转自:http://blog.csdn.net/stephenxu111/archive/2008/05/13/2442825.aspx
