# 把JFinal项目上传到服务器 - 一个跳popping的quant的博客 - CSDN博客





2017年08月30日 12:28:04[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：706








```
最近用JFinal框架写了个学生管理系统的后台，然后打算把它上传到linux服务器上，然后遇到了很多问题。
 一开始，就是按照网上的说明：https://my.oschina.net/jfinal/blog/353062
```

1：创建一个目录:   /var/www

2：为将要部署的项目创建一个目录， /var/www/my_project

3：将项目打成 war 包， 然后解压到 /var/www/my_project

4：最终的目录结构为：/var/www/my_project/WEB-INF，也即 WEB-INF 在项目名称目录之下

5：修改  TOMCAT_HOME/conf/ server.xml 文件找到 Host 标记，在其中添加如下子标记：

```
<Context path="" docBase="/var/www/my_project" reloadable="false" />
```

6：注意 Host 标记中的 appBase 属性不要去修改，让其为默认值 “webapps”

7：启动 tomcat

```
另外要注意两点：

这里没有把项目放在webapps中的目的就是防止项目加载两次，因为在配置文件中配置了会加载一次，而webapps中还会加载一次。所以记住第一点，永远不要将项目放在 TOMCAT_HOME/webapps 目录下面。
```

以上配置中的 Context 标记的 path 属性一定要设置为 “” 而不是 “/”，否则也会跳入一个坑。 docBase 属性后面放项目名称的绝对地址。

除此之外，在打包war的时候，要把war项目的jdk编译环境和服务器中的jdk版本一致，否则也会报错。还有就是把war上传到了指定的位置后，要改下WEB-INF中关于和数据库连接的properties文件来和服务器中数据库连接的帐号密码保持一致。另外可以通过查看logs下的日志来看报了什么错误，可以对症下药。

最后，还遇到了MySQL for Linux错误 ERROR 1045 (28000): Access denied for user ‘root’@‘localhost’ (using password: YES的问题，原因就是我在安装mysql的时候没有设置登录的密码。借鉴了这篇文章http://www.cnblogs.com/gumuzi/p/5711495.html

通过在免密的情况下进行修改密码解决了这个问题。

1.重置密码的第一步就是跳过MySQL的密码认证过程，方法如下：

vim /etc/my.cnf(注：windows下修改的是my.ini  一开始我就是找的my.ini但是没有找到，才发现my.ini是windows中的，linux中mysql要改my.ini)在文档内搜索mysqld定位到[mysqld]文本段：

/mysqld(在vim编辑状态下直接输入该命令可搜索文本内容)
在[mysqld]后面任意一行添加“skip-grant-tables”用来跳过密码验证的过程，如下图所示：

保存文档并退出：wq

2.接下来我们需要重启MySQL：

/etc/init.d/mysql restart(有些用户可能需要使用/etc/init.d/mysqld restart)

3.重启之后输入#mysql即可进入mysql。

4.接下来就是用sql来修改root的密码

```
mysql> use mysql;
mysql> update user set password=password("你的新密码") where user="root";
mysql> flush privileges;
mysql> quit
```

到这里root账户就已经重置成新的密码了。

5.编辑my.cnf,去掉刚才添加的内容，然后重启MySQL。大功告成！

最后，这样发布后在外网直接访问http://xxx.xxx.xxx.xx:8080/action就可以了。




