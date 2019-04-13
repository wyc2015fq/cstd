
# SAS学习︱逻辑库、数据集创建与查看、数据库链接（SAS与R的code对照） - 素质云笔记-Recorder... - CSDN博客

2016年10月17日 18:00:20[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3557




**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————
入门学习一周，开始写学习笔记。用习惯R之后，发现SAS程序相对python R还是有点繁杂。但是业务需要，不得不学一下。
代码部分大多来源于姚志勇老师的《SAS编程与数据挖掘商业案例》。
——————————————————————————————————————————

# 一、逻辑库、数据集、数据字典
数据字典>逻辑库>数据集
**逻辑库**=工作空间，存储四类内容，test是数据集文件，views是视图（相当于R里面的views点击查看），formats代表文件，sasmacr代表目录内容。
**数据集**，包括数据文件+SAS视图，可以像R中双击获取数据结构图表，不过这样效率较低，可以使用其他方法，譬如数据字典的方式describe view，详细内容见三、数据查看。
**数据字典**，是当年进程中所有内容的逻辑库、数据表的一个平铺、列出来。所以，在全局检索上十分有效。我觉得这个非常有用，不过貌似网上对其的定义比较少。

`libname fv "F:\SAS\SAS 9.4 space";      /*fv是数据库名字*/
data fv.forever;    /*  #forever代表在fv下的forever数据集*/
Input x y z;
Cards;             
1 2 3
4 5 6
;
run;`
code解读：
libname建立逻辑库，相当于R中的workspace中的一个文件夹；
data代表数据表格，data.frame；
与R数据输入不同的是，数据之间不是用逗号，而是空格来进行分割。

## 1、关于libname
如果，没有libname步骤，直接，data，那么就默认存放在SAS中的默认逻辑库之中，默认逻辑库名字叫“work”。
`libname lb clear;       #删除`代表删除libname

## 2、关于数据字典
本节参考[海纳百川](http://www.cnblogs.com/qlp1982/)BLOG的：[SAS数据字典访问](http://www.cnblogs.com/qlp1982/archive/2012/08/20/2647641.html)
访问数据字典方法如下：
1.直接用SQL语句访问Dictionary table
2.访问SAShelp View
|表名
|包含信息
|视图名
|
|DICTIONARY.CATALOGS
|SAS|目录册及所包含的条目
|SASHELP.VCATALG
|
|DICTIONARY.COLUMNS
|列（或变量）及其特征
|SASHELP.VCOLUMN
|
|DICTIONARY.DICTIONARIES
|所有表词典
|SASHELP.VDCTNRY
|
|DICTIONARY.EXTFILES
|文件标识名和外部文件物理存储地址
|SASHELP.VEXTFL
|
|DICTIONARY.INDEXES
|SAS|数据集索引
|SASHELP.VINDEX
|
|DICTIONARY.MEMBERS
|SAS|数据集
|SASHELP.VMEMBER
|
|DICTIONARY.OPTIONS
|目前设定的|SAS|系统选项
|SASHELP.VOPTION
|
|DICTIONARY.STYLES
|ODS|类型
|SASHELP.VSTYLE
|
|DICTIONARY.TABLES
|SAS|数据文件和视图
|SASHELP.VTABLE
|
|DICTIONARY.VIEWS
|SAS|数据视图
|SASHELP.VVIEW
|

`/*数据字典把当前SAS环境中的所有内容进行汇总归类到这个里面，方便检索*/
proc sql flow=10;
title2 'do the columns have formats,informats,or labels';
title3 'dictionary.columns';                                    
/*#所有表的列变量*/
create table colinf as
select name,type,length,label,format,informat                  
/*#变量名称*/
from dictionary.columns
where libname='SASHELP' and memname='CLASS';
/*#注意要大写	*/`
打开explorer window,在SASHELP这个library下，以V开头的视图实际上是Dictionary table内容的复制。如
proc sql;describe view sashelp.voption;quit;

可以看到voption这个视图的内容来自dictionary.options。   而对于View,除了SQL，直接用data步或proc步都可以访问。如
`data test2;
set sashelp.vcolumn;
where libname="WORK" and memname="TEST";
run;`
或者
proc contents data=sashelp.voption;
run;或者在command栏输入viewtable sashelp.voption。
——————————————————————————————————————————

# 二、SAS的数据导入与其他平台接入方式

SAS获取数据的几个方式：外部数据导入、passthrough方式、import方式、input方式。大多通过libname来调入。
## 1、数据库外部接入
`/*#数据库，外部接入*/
libname hsdb db2 user=xxuser password="xxxx" datasrc=datadb;
/*	#在逻辑库hsdb中，db2代表数据库名称，datasrc代表SQL中的数据集名称*/`
还有RODBC访问。
`/*通过RODBC访问 需要设置很多内容*/
libname odb odbc user=*** password=*** datasrc=test;
libname hsdb odbc noprompt="uid=xxxuser;pwd=xxxxx;dsn=datadb;" schema=hsuser`

## 2、外部导入，spss数据
`/*SPSS数据集，外部接入*/
libname sps spss '';`
## 3、oracle数据集，外部接入
`/*oracle数据集，外部接入*/
libname oralib oracle user=xxx pw=xxx path dbmssrv schema=educ;`
## 4、input方式导入数据
input里面有很多小符号，譬如@@ 和@的区别，$&。不过，也正是这些符号，让我觉得... 导个数据也忒费劲了...不过，这个也是数据输入中最复杂的问题了。。**关键就在这指针问题上，**
**@代表指针还在这行，只能罩着下一个input，如果没有两个input，那么跟没有@一样，直接跳到下一行；接下来一个有，则相当于指针在这行；**
**@@代表指针一直在同一行；**
**没有@，代表指针跳到下一行。**
**"$",表示字符型变量，&代表数值型。**

`data test;
    infile cards ;
    input x @;       /*单个@，能H住，有效期到下一个input语句*/
    input y;           /*没有@，H不住，下一个input语句会去读新的一行*/
    input z @@;    /*两个@，H住没问题，有效期延长到Data步下一圈*/
cards;
1 2 3
4 5 6
7
;`@只能在输入数据“1 2 3”之后要换行输入“4 5 6”才能被识别
@@可以无视这个，因为其关注“input xxx”，可以直接在cards;后输入数据“1 2 3 4 5 6”，系统自动识别为两组数据

**参考百度文库：**
**——————————————————————————**
两个都是行控制符(就是SAS阅读你的数据时,在每一行里面的阅读数)
@  表示执行下一个操作时，指针移到下一个记录(也就是下一行)。
@@表示执行下一个操作，指针保持在当前记录(当前行)。
举个例子：(数据录入我使用两种方式,以便于看出@与@@的阅读区别)
（1）：录入数据时占用一行的空间
`data a;
  input x y @;（当仅仅使用一个@时）
  cards;
  1 2 3 4 2 2 3 5 3 2 1 1 （数据占用一行空间）
  ;run;
proc print data=a;
run;`

(阅读结果如下)
data a: (此时SAS仅仅读了1次,显然没有读完，就跳到下一行阅读下一个数据，因为下一行没有数据可读，所以就读了一次就完了)
`The SAS System     17:52 Saturday, October 27, 2014  23
                                         Obs    x    y
                                          1     1    2`
`data b;
  input x y @@;
  cards;
  1 2 3 4 2 2 3 5 3 2 1 1 （数据占用一行空间）
  ;run;
proc print data=b;
run;`

(阅读结果如下)
`data b(此时SAS读了6次,显然这次是正确的，因为使用@@时，数据在当前行连续阅读)
            The SAS System     17:52 Saturday, October 27, 2014  24
                                         Obs    x    y
                                          1     1    2
                                          2     3    4
                                          3     2    2
                                          4     3    5
                                          5     3    2
                                          6     1    1`
（2）：录入数据时占用多行空间
`data a; （当仅仅使用一个@时）
  input x y @;
  cards;  1 2 3 4
2 2 3 5  （数据占用三行空间）
         3 2 1 1
  ;run;
proc print data=a;run;`

（SAS阅读情况如下）
`data a：(此时SAS仅仅读了3次,显然没有读完，因为每一行没有连续读完，就开始阅读新下一个数据，也就是光标就跳到了下一行)
     The SAS System     17:52 Saturday, October 27, 2014  25
                                         Obs    x    y                                          
                                          1     1    2                                          
                                          2     2    2                                          
                                          3     3    2`

`data b;
  input x y @@;
  cards;
  1 2 3 4
  2 2 3 5
  3 2 1 1
  ;run;
proc print data=b;
run;`

（SAS阅读情况如下）
`data b;（此时SAS阅读了6次，显然这次阅读的阅读时正确的，因为每一行都是连续读取之后再读取下一记录，也就是跳到下一行）
           The SAS System     17:52 Saturday, October 27, 2014  26
                                                                                                
                                         Obs    x    y                                          
                                                                                                
                                          1     1    2                                          
                                          2     3    4                                          
                                          3     2    2                                          
                                          4     3    5                                          
                                          5     3    2                                          
                                          6     1    1`
——————————————————————————————————————————

# 三、数据查看方式：content/data/proc sql三种方式

数据查看也有很多方式，譬如content、data/proc sql包括双击也是一种方式，一共四种方式。
## 1、content方式
查询数据的内容（summary）  针对一些头文件里面的隐藏数据集，可以用这样的方式看到，类似summary，不过内容没有那么详细。

`proc contents data=fv.forever out=class;run;`
out代表将查询内容打印出来，并存放在临时逻辑库work之中。
## 2、data方式
`data class/view=class1;
set fv.forever
;
run;`
## 3、sql方式

`proc sql;
create view class2 as
select*from fv.forever;
quit;`
**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)
———————————————————————————


