# 【各种调试】种种Bug - 三少GG - CSDN博客
2011年04月25日 11:47:00[三少GG](https://me.csdn.net/scut1135)阅读数：885
1. G:**Win7 下 vs2008** 调试**MFC winsocket**时遇到此问题：
VC++ 2008 连接器问题 1>LINK : fatal error LNK1000: Internal error during IncrBuildImage
每次编译都会出现这个问题
在Windows 7下
1>LINK : fatal error LNK1000: Internal error during IncrBuildImage
解答：
返回错误：LINK : fatal error LNK1000: Internal error during IncrBuildImage 解决办法： **项目->属性->链接器->常规 下面的“启用增量链接”，将“是(/INCREMENTAL)”改为“否(/INCREMENTAL:NO)”。**不过这又引入了另外一个警 告：FormatCom.obj : warning LNK4075: 忽略“/EDITANDCONTINUE”(由于“/INCREMENTAL:NO”规范) 选择项目,**属性->配置属性 ->C/C++，修改“调试信息格式”为“程序数据库(/Zi)”即可**。 到此问题解决，据说这是VC的一个内部BUG，可以下载KB948127补丁来解决，但我并没有能正常安装该补丁
 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2. error C2628: followed by 'ClfDecisionStump' is illegal (did you forget a ';'?)   (2011.6.6)
error C2628: followed by 'ClfDecisionStump' is illegal (did you forget a ';'?)
出现如上的错误，
查找csdn 
5 楼jiangsheng（蒋晟.Net[MVP]）回复于 2005-01-24 01:02:15 得分 *20*
包含的最后一个头文件末尾如果有语法错误的话，有可能造成第一行可执行代码编译出错。
发现确实如此，在最后一个#include 的文件里面
**定义class最后没有加上分号。**
**造成这种错误。**
注意注意。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
