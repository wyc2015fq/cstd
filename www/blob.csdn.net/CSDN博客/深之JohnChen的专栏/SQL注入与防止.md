# SQL注入与防止 - 深之JohnChen的专栏 - CSDN博客

2017年10月13日 11:57:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：854


一、SQL注入

SQL注入即是指web应用程序对用户输入数据的合法性没有判断，攻击者可以在web应用程序中事先定义好的查询语句的结尾上添加额外的SQL语句，以此来实现欺骗数据库服务器执行非授权的任意查询，从而进一步得到相应的数据信息。

二、SQL注入漏洞的几种判断方法

①http://www.heetian.com/showtail.asp?id=40'

②http://www.heetian.com/showtail.asp?id=40 and 1=1

③http://www.heetian.com/showtail.asp?id=40 and 1=2

如果执行①后，页面上提示报错或者提示数据库错误的话，说明是存在注入漏洞的。

如果执行②后，页面正常显示，而执行③后，页面报错，那么说明这个页面是存在注入漏洞的。

三、如何防止sql注入式攻击

杜绝用户提交的参数入库并且执行，参数化查询或者做词法分析。

1、过滤前端提交的参数。

JSP页面判断代码： 

使用javascript在客户端进行不安全字符屏蔽 

功能介绍：检查是否含有”‘”,”\\”,”/” 

参数说明：要检查的字符串 

返回值：0：是1：不是 

函数名是 

function check(a) 

{ 

return 1; 

fibdn = new Array (”‘” ,”\\”,”/”); 

i=fibdn.length; 

j=a.length; 

for (ii=0; ii＜i; ii++) 

{ for (jj=0; jj＜j; jj++) 

{ temp1=a.charAt(jj); 

temp2=fibdn[ii]; 

if (tem’; p1==temp2) 

{ return 0; } 

} 

} 

return 1; 

} 

2、后台使用正则表达式过滤传入的参数 

要引入的包： 

import java.util.regex.*; 

正则表达式： 

private String CHECKSQL = “^(.+)\\sand\\s(.+)|(.+)\\sor(.+)\\s$”; 

判断是否匹配： 

Pattern.matches(CHECKSQL,targerStr); 

下面是具体的正则表达式： 

检测SQL meta-characters的正则表达式 ： 

/(\%27)|(\’)|(\-\-)|(\%23)|(#)/ix 

修正检测SQL meta-characters的正则表达式 ：/((\%3D)|(=))[^\n]*((\%27)|(\’)|(\-\-)|(\%3B)|(:))/i

典型的SQL 注入攻击的正则表达式 ：/\w*((\%27)|(\’))((\%6F)|o|(\%4F))((\%72)|r|(\%52))/ix 

检测SQL注入，UNION查询关键字的正则表达式 ：/((\%27)|(\’))union/ix(\%27)|(\’) 

检测MS SQL Server SQL注入攻击的正则表达式： 

/exec(\s|\+)+(s|x)p\w+/ix 

等等….. 

3、后台在执行语句时，使用PreparedStatement

采用预编译语句集，它内置了处理SQL注入的能力，只要使用它的setXXX方法传值即可。 

使用好处： 

(1).代码的可读性和可维护性. 

(2).PreparedStatement尽最大可能提高性能. 

(3).最重要的一点是极大地提高了安全性. 

原理： 

sql注入只对sql语句的准备(编译)过程有破坏作用 

而PreparedStatement已经准备好了,执行阶段只是把输入串作为数据处理, 

而不再对sql语句进行解析,准备,因此也就避免了sql注入问题. 

4、基于词法分析和语法分析的SQL注入攻击检测。

词法lexical+语法grammar＋语义semantic分析

a. [libdejector](http://sourceforge.net/projects/libdejector/), [blackhat
 2005](http://www.blackhat.com/presentations/bh-usa-05/bh-us-05-hansen.pdf), Hanson && Pattresn, 给出了以下观点

"Input validataion needs to be done with a mechanism strong enough to recognize the language"

HTTP与SQL属于CFG(Context-Free Grammars)，正则表达式属于FSM(Finite-state machine)描述语言，用正则来描述规则总会有误报与漏报

b. [libinjection](https://github.com/client9/libinjection/), [blackhat
 2012](http://2015.zeronights.org/assets/files/04-Novikov.pdf) , Nick Galbreath 词法分析

c. [sqlchop](http://blog.chaitin.com/sqlchop-the-sqli-detection-engine/), [blackhat
 2014](http://blog.chaitin.com/sqlchop-the-sqli-detection-engine/)， 词法+语法分析



