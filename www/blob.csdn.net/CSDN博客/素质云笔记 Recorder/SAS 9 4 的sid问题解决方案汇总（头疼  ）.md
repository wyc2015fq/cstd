# SAS 9.4 的sid问题解决方案汇总（头疼...） - 素质云笔记/Recorder... - CSDN博客





2016年10月13日 18:44:40[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：19130标签：[SID																[SAS 9.4																[解决																[报错																[error](https://so.csdn.net/so/search/s.do?q=error&t=blog)
个人分类：[SAS](https://blog.csdn.net/sinat_26917383/article/category/6461415)









**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


因为经常出现sid出现问题，所以问题很多。最常规的方式就是直接到网上搜索sid直接添加，寻找sid比较好的地方有：[经管之家的sas版块](http://bbs.pinggu.org/forum-68-1.html)

当然如果有sid还是报错，也很经常，一般情况下会修改系统时间。




报错方式



```
ERROR:  WYNEVER FATAL ERROR:  WRCODE=ffffffff80000803, MODULE='HIINIT': PATH OPTION UNAVAILABLE
ERROR:  SAS Is Terminating
```

**方式一：把你现在在用的SID里面的Order、Setnumid复制粘贴替换掉到我附件中有效的SID里面就好**





参考来源：http://bbs.pinggu.org/thread-4733787-1-1.html

说明：先确认你的SAS是什么版本、多少位的
然后找到我附件中对应的有效SID
把你现在在用的SID里面的Order、Setnumid复制粘贴替换掉到我附件中有效的SID里面就好


结果：失败




**方式二：单独运行**



|1.右键单击SAS图标，选择以管理员身份运行。2.打开SAS后把代码复制粘贴到编辑器里。3.运行。你把图中红框部分 从你许可文件中复制粘贴到增强型编辑器中提交运行就可以了，还不行的话，试试以管理员权限运行sas再提交上述代码参考链接：http://bbs.pinggu.org/thread-2969973-1-1.html![](https://img-blog.csdn.net/20161013181523471)结果：失败说明：其中增强型编辑器就是SAS自带的编辑器，不过有可能没下载。可以参考：# [SAS9.3安装后遇到增强型程序编辑器未安装错误的解决方法](http://bbs.pinggu.org/thread-2187693-1-1.html)**方式三：修改日期**修改日期是最基本的笨办法，但是的确好用。```ERROR: 当前日期 Thursday, October 13, 2016 超过了您的 SAS 系统的最后截止ERROR: 日期 Wednesday, May 13, 2015。ERROR: 请与 SAS 安装代表联系以获得更新的 SAS 安装数据 (SID) 文件，ERROR: 该文件包含 SETINIT 信息。```出现了上述的问题，我将日期改在提醒的那一天就可以，改成其他日期好像没啥用。**方式四：组合方式三+方式二**因为出现了以下的情况：我已管理员身份都打不开SAS的时候，就可以用这个办法：1、先调整系统时间到SAS 提示的时间；2、打开SAS 9.4 然后把以下代码写入编辑器（来源：http://bbs.pinggu.org/thread-4795266-1-1.html）：```PROC SETINIT RELEASE='9.4';SITEINFO NAME='NATIONAL PINGTUNG UNIV OF SCIENCE & TECHNOLOGY'SITE=12001462 OSNAME='WX64_WKS' RECREATE WARN=30 GRACE=30BIRTHDAY='24MAR2016'D  EXPIRE='30APR2017'D PASSWORD=730951611;CPU MODEL=' ' MODNUM=' ' SERIAL=' ' NAME=CPU000;EXPIRE 'PRODNUM000' 'PRODNUM001' 'PRODNUM002' 'PRODNUM003''PRODNUM004' 'PRODNUM005' 'PRODNUM006' 'PRODNUM007''PRODNUM008' 'PRODNUM010' 'PRODNUM013' 'PRODNUM015''PRODNUM025' 'PRODNUM035' 'PRODNUM050' 'PRODNUM070''PRODNUM075' 'PRODNUM094' 'PRODNUM095' 'PRODNUM117''PRODNUM119' 'PRODNUM123' 'PRODNUM164' 'PRODNUM165''PRODNUM166' 'PRODNUM167' 'PRODNUM192' 'PRODNUM194''PRODNUM204' 'PRODNUM208' 'PRODNUM209' 'PRODNUM215''PRODNUM219' 'PRODNUM225' 'PRODNUM448' 'PRODNUM535''PRODNUM538' 'PRODNUM550' 'PRODNUM555' 'PRODNUM557''PRODNUM560' 'PRODNUM677' 'PRODNUM678' '30APR2017'D/ CPU=CPU000;SAVE; RUN;```3、调整时间到当下系统时间，4、然后运行复制好的代码，就可以了。以上的SID来源于经管之家，有效期至2017年4月。|
|----|

**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————](https://so.csdn.net/so/search/s.do?q=报错&t=blog)](https://so.csdn.net/so/search/s.do?q=解决&t=blog)](https://so.csdn.net/so/search/s.do?q=SAS 9.4&t=blog)](https://so.csdn.net/so/search/s.do?q=SID&t=blog)




