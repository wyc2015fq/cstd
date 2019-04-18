# ACE网管平台移植Linux - ljx0305的专栏 - CSDN博客
2008年05月15日 21:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：768
我们的网管平台基于ACE, ODBC，SNMP ++的，都是可以跨平台的库,但一直未进行linux下的编译测试，这是去年移植测试时记录的日志。
注意移植前先要编译好ACE, unixODBC，SNMP ++的库。
在从VC下移植到linux（用gcc编译）过程中记录的一些更改项：
1）微软windef.h文件中定义的内容linux下不可用，包括
#define MAX_PATH          260
别的还有
#define DWORD unsigned int
#define LPVOID void*
2）用到的ace中和NT有关的
ACE_NT_Service
这些东西即使是win32下其实也没有起作用,直接屏蔽
3）微软winsvc.h文件中定义的内容
#define SERVICE_CONTROL_SHUTDOWN       0x00000005
.....
这些东西即使是win32下其实也没有起作用,直接屏蔽
4）有些大小写有关系了, 还有include后面必须用/而不是/
#include "ace/OS.H"
#include "ace/Task.h"
#include "dbaccess1.h"
#include "include/v3.h"
#include "include/snmpmsg.h"       // asn serialization class
#include "include/vb.h"
#include "include/oid_def.h"
5）有些语法细节gcc不允许
ACE_DEBUG((LM_INFO, ACE_TEXT("%D Preparing to rebuild DB connection "
                    "pool(%d)! : %t/n"), iConnPoolSize));
那个换行的第二个"必须放到第二行
6）每个文件结尾都要回车，不然编译会告警
warning: no newline at end of file
7）gcc不允许过期绑定(VC下是可以通过编译的)
for (int i = 0;i < 6;i++)
。。。
for (i = 0;i < 128;i++)
8）warning: "NULL" redefined
9）linux（gcc）下函数至少返回void，VC下不是必须的
10) 注册表部分只能在windows中用（查询数据库类型）
改用SQLGetInfo读数据库类型
11)FormatMessage这是MFC CString的东东(comm.cpp 里面得debug_info函数)
12)linux下用真实ip得出来的主机名和用127.0.0.1或0.0.0.0(默认)得出来得不一样
ACE_INET_Addr addr1(port, szAddr);//初始化,注意第二个参数填真实ip和"127.0.0.1"的区别
addr1.get_host_name();//获得主机名
13)常量字符串不能直接转给char*
sztemp = (sztemp==0||strlen(sztemp)<1)?" ":sztemp;
14)linux不支持：itoa   strrev   strnicmp（不管大小写的比较） stricmp strset Sleep
其中itoa用ACE_OS::itoa代替(strrev也就不用了)，可以用以下函数模拟实现或转换成linux下的函数
#if defined(__linux__)
#define MAX_PATH 260
#define DWORD unsigned int
#define LPVOID void*
#define strnicmpstrncasecmp
#define stricmpstrcasecmp
typedef unsigned charBYTE;
typedef unsigned shortWORD;
#define __strrevstrrev
char* strrev(char* szT)
{
    if ( !szT )                 // 处理传入的空串.
        return "";
inti = strlen(szT);
intt = !(i%2)? 1 : 0;      // 检查串长度.
    for(intj = i-1 , k = 0 ; j > (i/2 -t) ; j-- )
    {
charch = szT[j];
szT[j]   = szT[k];
szT[k++] = ch;
    }
    return szT;
}
#define _itoaitoa
char* itoa(intvalue, char* str, intradix)
{
intrem = 0;
intpos = 0;
charch = '!' ;
    do
    {
rem    = value % radix ;
value /= radix;
        if ( 16 == radix )
        {
            if( rem >= 10 && rem <= 15 )
            {
                switch( rem )
                {
                case 10:
ch = 'a' ;
                    break;
                case 11:
ch ='b' ;
                    break;
                case 12:
ch = 'c' ;
                    break;
                case 13:
ch ='d' ;
                    break;
                case 14:
ch = 'e' ;
                    break;
                case 15:
ch ='f' ;
                    break;
                }
            }
        }
        if( '!' == ch )
        {
str[pos++] = (char) ( rem + 0x30 );
        }
        else
        {
str[pos++] = ch ;
        }
    }while( value != 0 );
str[pos] = '/0' ;
    return strrev(str);
}
inline void Sleep(unsigned intuseconds )
{
// 1 毫秒（milisecond） = 1000 微秒（microsecond）.
// Windows 的 Sleep 使用毫秒（miliseconds）
// Linux 的 usleep 使用微秒（microsecond）
// 由于原来的代码是在 Windows 中使用的，所以参数要有一个毫秒到微秒的转换。
    usleep( useconds * 1000 );
}
#endif
预设库路径怎么不起作用呢（bvrde里面）？
ACE_ROOT=/mnt/hgfs/ACE_wrappers
export ACE_ROOT
C_INCLUDE_PATH=$ACE_ROOT
export C_INCLUDE_PATH
共享库LD_LIBRARY_PATH=$ACE_ROOT/ace;$LD_LIBRARY_PATH
export LD_LIBRARY_PATH
静态库LIBRARY_PATH=./snmp_lib/lib
export LIBRARY_PATH
后来在makefile里面明确指定库文件(给ld选项使用,编译多项目文件的单个文件时只需指定-I路径即可)才行：
LIBS          = ./snmp_lib/lib/libsnmp++.a /
                     ./snmp_lib/libdes/libdes.a /
       /mnt/hgfs/igoview/ACE_wrappers/ace/libACE.so /
       /usr/lib/libodbc.so
转自:http://blog.csdn.net/stephenxu111/archive/2008/05/14/2446384.aspx
