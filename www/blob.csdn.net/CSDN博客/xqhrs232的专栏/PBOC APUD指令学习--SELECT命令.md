# PBOC APUD指令学习--SELECT命令 - xqhrs232的专栏 - CSDN博客
2016年10月19日 00:37:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：625
原文地址::[http://blog.csdn.net/tenfyguo/article/details/40590301](http://blog.csdn.net/tenfyguo/article/details/40590301)
相关文章
1、[PBOC规范研究之四、文件结构及访问](http://blog.csdn.net/tenfyguo/article/details/40590819)----[http://blog.csdn.net/tenfyguo/article/details/40590819](http://blog.csdn.net/tenfyguo/article/details/40590819)
一，SELECT命令的定义和范围
       SELECT命令通过文件名或AID来选择IC卡中的PSE、DDF或ADF。命令执行成功后，PSE、DDF或ADF的路径被设定。后续命令作用于与用SFI选定的PSE、DDF或ADF相联系的AEF。从IC卡的响应报文应由回送FCI组成。
二，实际例子
     1，终端发送如下指令： 00 A4 04 00  0E 31 50 41 59 2E 53 59 53 2E 44 44 46 30 31
     指令分析：
     CLA: 00
     INS:  A4
     P1:  04
     P2:  00
     Lc:  0E，表示终端发送给卡片数据域的长度，这里0x0E=15，即15个字节。
     data: 31 50 41 59 2E 53 59 53 2E 44 44 46 30 31，（即“1PAY.SYS.DDF01”的ASCII编码）终端用PSE名“1PAY.SYS.DDF01”向卡片发送SELECT命令。
     Le: 没有该字段
     2，指令的返回  
      卡片回应的数据：  **6F** 1E **84** 0E 31 50 41 59 2E 53 59 53 2E 44 44 46 30 31**A5** 0C **88** 01 01 **5F 2D** 02 7A 68 **9F 11** 0101 
解释见下表表 B.26。 成功选择PSE后回送的FCI。
表 A：SELECT命令报文
|代码|值|
|----|----|
|CLA|‘00’|
|INS|‘A4’|
|P1|引用控制参数(见表B-24)|
|P2|选择选项(见表B-25)|
|Lc|‘05’ - ‘10’|
|Data|文件名|
|Le|‘00’|
下表定义了SELECT命令报文的引用控制参数P1：
表 B.：SELECT命令引用控制参数
|B8|b7|b6|b5|b4|b3|b2|b1|含义|
|----|----|----|----|----|----|----|----|----|
|0|0|0|0|0|||| |
||1|通过名称选择| | | | | | |
||0|0|| | | | | |
下表定义了SELECT命令报文的选择选项P2：
表 B.25：SELECT命令的可选参数
|b8|b7|b6|b5|b4|b3|b2|b1|含义|
|----|----|----|----|----|----|----|----|----|
||0|0|第一个有或仅有一个| | | | | |
|1|0|下一个| | | | | | |
### 命令报文数据域
 应答报文中数据域应包括所选择的PSE、DDF或ADF的FCI。
下表定义了成功选择PSE后回送的FCI：
表 C：选择PSE的应答报文 (FCI)
|标识|值|存在性| |
|----|----|----|----|
|‘6F’|FCI模板|M| |
||‘84’|DF名（1PAY.SYS.DDF01）|M|
|‘A5’|FCI数据专用模板|M| |
||‘88’|目录基本文件的SFI|M|
|‘5F2D’|语言选择|O| |
|‘9F11’|发卡行代码表索引|O| |
|‘BF0C’|发卡行自定义数据(FCI)|O| |
||‘XXXX’(第3册规定的标签)|来自从应用提供商、发卡行或IC卡供应商的1个或多个附加(专用)数据元。|O|
[](http://blog.csdn.net/tenfyguo/article/details/40590301#)[](http://blog.csdn.net/tenfyguo/article/details/40590301#)[](http://blog.csdn.net/tenfyguo/article/details/40590301#)[](http://blog.csdn.net/tenfyguo/article/details/40590301#)[](http://blog.csdn.net/tenfyguo/article/details/40590301#)[](http://blog.csdn.net/tenfyguo/article/details/40590301#)
- 
顶
0- 
踩
