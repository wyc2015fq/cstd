# 更改Mysql的默认字符集！ - 三少GG - CSDN博客
2011年01月05日 02:38:00[三少GG](https://me.csdn.net/scut1135)阅读数：1524

   今天在使用mysql中发现在查询中文记录时不是很准确，后来查了一些资料，原来是mysql默认的字符集latin。于是准备更改mysql默认的字符集。
      其实更改字符集也很容易
打开my.ini文件，在[client]、[mysqld]下面加上default-character-set=gb2312，其含义就是把gb2312设置为数据库的默认字符集。设置完毕后需要重新启动myqsl服务。 
同理，可以把其他字符集设置成默认字符集。
两篇推荐好文章：
1. [http://www.blogjava.net/wldandan/archive/2007/09/04/142669.html](http://www.blogjava.net/wldandan/archive/2007/09/04/142669.html)
2. [http://linux.chinaunix.net/techdoc/database/2008/03/23/986386.shtml](http://linux.chinaunix.net/techdoc/database/2008/03/23/986386.shtml)
3. [http://www.power-bbs.com/zixun/jishuwenzhang2/20070903/16343.html](http://www.power-bbs.com/zixun/jishuwenzhang2/20070903/16343.html)
=======================================================================
的VA是试用期 没有用破解版 卸载后问题依旧 重装VS问题依旧 经过测试 发现
只要头文件 有下面这句话就崩溃 。但是 把#error 注释起来就没问题！！我真是郁闷 
#ifndef __AFXWIN_H__
  #error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif<span style="font-family:Verdana,Arial,Helvetica,sans-serif"><span style="white-space:normal">
</span></span>
问题签名:
  问题事件名称:APPCRASH
  应用程序名:devenv.exe
  应用程序版本:10.0.30319.1
  应用程序时间戳:4ba1fab3
  故障模块名称:KERNELBASE.dll
  故障模块版本:6.1.7600.16385
  故障模块时间戳:4a5bdaae
  异常代码:e0434352
  异常偏移:00009617
  OS 版本:6.1.7600.2.0.0.256.1
  区域设置 ID:2052
  其他信息 1:0a9e
  其他信息 2:0a9e372d3b4ad19135b953a78882e789
  其他信息 3:0a9e
  其他信息 4:0a9e372d3b4ad19135b953a78882e789
应该是要包含预编译头文件“stdafx.h”，在代码的前面#include"stdafx.h".
或者如果是使用VC6.0在工程->设置->c/c++选项卡->（分类）预编译的头文件
->不使用预编译头文件->确定。
========
liujie1992回复于25日23点31分　
没有包含预编译头文件，在出错的那个文件最上面#include "stdafx.h" 然后Build-Clean Solution再F7重新编译||海洋之星1a回复于25日23点37分　在你出错的文件中，#include "stdafx.h"/ 如果不行，那么就在出错的文件右键属性中，选择不是用预编译|
||孙祥军回复于25日23点44分　win32不支持MFC，低级的调用高级的?|
==========================================================================
最初感觉匪夷所思，后来上网一问，原来需要一个动态链接库文件（lib）文件，文件名为ws2_32.lib。
为了把这个ws2_32.lib加载到项目当中，有两种方法。
第一种：
在菜单 project ->settings -> link   -> object/library modules 下面输入ws2_32.lib   然后确定即可
第二种：
在头文件中加入语句#pragma comment( lib, "ws2_32.lib" )   来显式加载。 即：
#include <winsock2.h> 
#pragma comment(lib, "WS2_32")
