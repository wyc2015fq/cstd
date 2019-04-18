# VC2008下配置boost库使用正则表达式 - 深之JohnChen的专栏 - CSDN博客

2009年08月20日 18:11:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4202标签：[正则表达式																[microsoft																[regex																[mfc																[cmd																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=regex&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)
个人分类：[boost](https://blog.csdn.net/byxdaz/article/category/6890413)


**VC2008****下配置****boost****库使用正则表达式**

最近开发项目要用到数据搜索、检验问题，打算使用正则表达式，因为它简单、高效。支持正则表达式的库有多大概有以下几种：boost，微软发布的greta。

　　本文介绍使用boost库正则表达式来解决字符匹配、搜索、替换等应用。现在介绍一下VC2008中怎样配置与使用boost库正则表达式。

1、下载与安装boost库，下载地址：[http://sourceforge.net/projects/boost/files/boost/1.39.0/](http://sourceforge.net/projects/boost/files/boost/1.39.0/)，将其解压到E:/boost目录。

有关boost的使用可以参考[http://www.stlchina.org/twiki/bin/view.pl/Main/BoostEnterBoost](http://www.stlchina.org/twiki/bin/view.pl/Main/BoostEnterBoost)

2、编译动态库，本文只要支持正则表达式就可以，因此只需要编译E:/boost/libs/regex/build目录下的文件。这步比较关键。

具体操作：

　　在“运行”里输入“cmd”，打开命令提示符；

　　输入命令“cd E:/boost/libs/regex/build”，设置当前目录

　　设置VC2008的配置环境变量，找到Microsoft Visual Studio 9.0/VC/bin/VCVARS32.BAT文件，把VCVARS32.BAT文件用鼠标拖到打开的cmd窗口（"F:/Program Files/Microsoft Visual Studio/VC/bin/VCVARS32.BAT"），然后回车。

　　输入命令“nmake –f vc9.mak”

　　等待几分钟，E:/boost/libs/regex/build多了一个vc9的文件夹，里面是很多lib和dll文件。

3、配置VC2008 “VC++目录”，使它编译使时候能找到正则库。

　　打开vc2008，选择“工具->选项->项目与解决方案->显示以下内容的目录-> 包含文件”，加入一行“E:/BOOST”
选择“工具->选项->项目与解决方案->显示以下内容的目录-> 库文件”，加入一行“E:/BOOST /libs/regex/build/vc90”

4、编写程序测试
VC2008中默认的是UNICODE编码字符集,要使用regex的UNICODE版本tregex.关于正则表达式的应用可以查看使用说明和实例,分别放在libs/regex/doc和libs/regex/example目录中.注意在编译连接的时候可能会报告libboost_regex-vc90-mt-gd-1_39.lib文件无法找到,需要把libboost_regex-vc90-mt-gd-1_38.lib改成libboost_regex-vc90-mt-gd-1_39.lib

　　新建一个对话框的MFC工程，
　　加入头文件
#include<boost/regex/mfc.hpp>
　　在按钮鼠标单击事件响应函数中加入

boost::tregexexpression(_T("^select ([a-zA-Z]*) from ([a-zA-Z]*)"));

CStringsRet;

CStringin = _T("select gm from tab");

boost::tmatchwhat;

if(boost::regex_match(in, what, expression))//CString转string

{

for(inti=0;i<what.size();i++)

{

sRet =CString(what[i].first, what.length(i));

MessageBox(sRet);

}

}

else

{

MessageBox(_T("Error Input"));

}
　　输出的结果如下:

select gm from tab

gm

tab



