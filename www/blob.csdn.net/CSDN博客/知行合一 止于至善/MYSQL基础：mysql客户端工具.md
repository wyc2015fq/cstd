
# MYSQL基础：mysql客户端工具 - 知行合一 止于至善 - CSDN博客

2019年03月16日 17:20:57[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：57


![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
关于mysql客户端工具，和sqlplus一样通过为用户提供一个命令行接口来操作和管理MYSQL服务器。 本文总结一下常用的选项和基础的几个设定技巧。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 常用选项
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|选项缩写|选项|选项说明|
|---|---|---|
|-e|–execute=name|执行指定命令后退出|
> 常用场景：

> 在脚本后者程序中嵌入使用，不希望使用交互式的方式使用msql客户端，执行一条指定的语句后退出。

> 使用例：

> \# mysql -uroot -pliumiaocn -e "desc mysql.time_zone"
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> +
> ------------------+------------------+------+-----+---------+----------------+
> |
> Field
> |
> Type
> |
> Null
> |
> Key
> |
> Default
> |
> Extra
> |
> +
> ------------------+------------------+------+-----+---------+----------------+
> |
> Time_zone_id
> |
> int
> (
> 10
> )
> unsigned
> |
> NO
> |
> PRI
> |
> NULL
> |
> auto_increment
> |
> |
> Use_leap_seconds
> |
> enum
> (
> 'Y'
> ,
> 'N'
> )
> |
> NO
> |
> |
> N
> |
> |
> +
> ------------------+------------------+------+-----+---------+----------------+
> \#
> 选项缩写
> 选项
> 选项说明
|
|---|---|---|
> -E
> –vertical
> 以纵列方式显示显示结果，类似\G的作用
|
> 使用例：将见一个例子以纵列方式显示

> \# mysql -uroot -pliumiaocn -E -e "desc mysql.time_zone"
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> 1.
> row
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> Field: Time_zone_id
> Type
> :
> int
> (
> 10
> )
> unsigned
> Null
> :
> NO
> Key
> : PRI
> Default
> :
> NULL
> Extra:
> auto_increment
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> 2.
> row
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> *
> Field: Use_leap_seconds
> Type
> :
> enum
> (
> 'Y'
> ,
> 'N'
> )
> Null
> :
> NO
> Key
> :
> Default
> : N
  Extra:
> \#
> 选项缩写
> 选项
> 选项说明
|
|---|---|---|
> -H
> –html
> 结果以HTML方式进行保存
|
> -X
> –xml
> 结果以XML方式进行保存
|
> 常用场景：

> 以HTML页面或者XML格式方式将执行的过程直接生成HTML文件时可以使用

> 使用例：

> \# mysql -uroot -pliumiaocn -H -e "desc mysql.time_zone"
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> <
> TABLE
> BORDER
> =
> 1
> >
> <
> TR
> >
> <
> TH
> >
> Field
> <
> /
> TH
> >
> <
> TH
> >
> Type
> <
> /
> TH
> >
> <
> TH
> >
> Null
> <
> /
> TH
> >
> <
> TH
> >
> Key
> <
> /
> TH
> >
> <
> TH
> >
> Default
> <
> /
> TH
> >
> <
> TH
> >
> Extra
> <
> /
> TH
> >
> <
> /
> TR
> >
> <
> TR
> >
> <
> TD
> >
> Time_zone_id
> <
> /
> TD
> >
> <
> TD
> >
> int
> (
> 10
> )
> unsigned
> <
> /
> TD
> >
> <
> TD
> >
> NO
> <
> /
> TD
> >
> <
> TD
> >
> PRI
> <
> /
> TD
> >
> <
> TD
> >
> NULL
> <
> /
> TD
> >
> <
> TD
> >
> auto_increment
> <
> /
> TD
> >
> <
> /
> TR
> >
> <
> TR
> >
> <
> TD
> >
> Use_leap_seconds
> <
> /
> TD
> >
> <
> TD
> >
> enum
> (
> 'Y'
> ,
> 'N'
> )
> <
> /
> TD
> >
> <
> TD
> >
> NO
> <
> /
> TD
> >
> <
> TD
> >
> <
> /
> TD
> >
> <
> TD
> >
> N
> <
> /
> TD
> >
> <
> TD
> >
> <
> /
> TD
> >
> <
> /
> TR
> >
> <
> /
> TABLE
> >
> \#
> \#
> 将此输出的HTML内容进行保存，用浏览器打开，可以看到效果如下所示
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190316152117747.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 选项缩写
> 选项
> 选项说明
|
|---|---|---|
> -
> –prompt=name
> 执行提示信息
|
> 常用场景：

> 修改默认的mysql>的提示信息，同Linux上的prompt一样，可以用于提示一些具体的特定信息，比如当前用户，使用的数据库信息，时间信息等，可以对操作者进行一定的提醒或者警示作用

> 使用例：将默认的mysql提示改成 hello to liumiaocn mysql。

> \# mysql -uroot -pliumiaocn --prompt="hello to liumiaocn mysql>"
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> Welcome
> to
> the MySQL monitor
> .
> Commands
> end
> with
> ;
> or
> \g
> .
> Your MySQL connection id
> is
> 45618
> Server version:
> 5.7
> .16
> MySQL Community Server
> (
> GPL
> )
> Copyright
> (
> c
> )
> 2000
> ,
> 2016
> ,
> Oracle
> and
> /
> or
> its affiliates
> .
> All
> rights reserved
> .
> Oracle
> is
> a registered trademark
> of
> Oracle Corporation
> and
> /
> or
> its
affiliates
> .
> Other names may be trademarks
> of
> their respective
owners
> .
> Type
> 'help;'
> or
> '\h'
> for
> help
> .
> Type
> '\c'
> to
> clear the
> current
> input statement
> .
> hello
> to
> liumiaocn mysql
> >
> 更常见的使用方式比如如下所示：

> \# mysql -uroot -pliumiaocn --prompt="[\\u@\\h(\\d) \\R:\\m:\\s] mysql>"
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> Welcome
> to
> the MySQL monitor
> .
> Commands
> end
> with
> ;
> or
> \g
> .
> Your MySQL connection id
> is
> 47427
> Server version:
> 5.7
> .16
> MySQL Community Server
> (
> GPL
> )
> Copyright
> (
> c
> )
> 2000
> ,
> 2016
> ,
> Oracle
> and
> /
> or
> its affiliates
> .
> All
> rights reserved
> .
> Oracle
> is
> a registered trademark
> of
> Oracle Corporation
> and
> /
> or
> its
affiliates
> .
> Other names may be trademarks
> of
> their respective
owners
> .
> Type
> 'help;'
> or
> '\h'
> for
> help
> .
> Type
> '\c'
> to
> clear the
> current
> input statement
> .
> [
> root
> @localhost
> (
> (
> none
> )
> )
> 17
> :
> 24
> :
> 04
> ]
> mysql
> >
> use
> mysql
Reading
> table
> information
> for
> completion
> of
> table
> and
> column
> names
You can turn
> off
> this feature
> to
> get a quicker startup
> with
> -
> A
> Database
> changed
> [
> root
> @localhost
> (
> mysql
> )
> 17
> :
> 24
> :
> 11
> ]
> mysql
> >
> 时间相关信息

> 转义序列
> 说明
|
|---|---|
> \D
> 完整的日期和时间
|
> \y
> YY格式的年
|
> \Y
> YYYY格式的年
|
> \o
> 以数字方式显示的月份
|
> \O
> 以字母方式显示的月份
|
> \w
> 以字母显示的星期信息
|
> \P
> am/pm标志
|
> \r
> 12小时制时间
|
> \R
> 24小时制时间
|
> \m
> 分
|
> \s
> 秒
|
> 其他设定信息

> 转义序列
> 说明
|
|---|---|
> \u
> 当前用户名
|
> \U
> 包含主机名的用户名
|
> \h
> 当前主机
|
> \d
> 当前使用的数据库
|
> \v
> 服务器的版本号
|
> \p
> 当前端口号或者socket文件名
|
> \c
> 当前输入行数
|
> 特殊字符

> 转义序列
> 说明
|
|---|---|
> \t
> 制表符
|
> \l
> 当前分隔符
|
> \S
> 分号
|
> ’
> 单引号
|
> "
> 双引号
|
> _
> 空格字符
|
> \
> 反斜线
|
> \n
> 换行符
|

