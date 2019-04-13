
# MySQL基础：修改root密码 - 知行合一 止于至善 - CSDN博客

2019年03月17日 20:12:11[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：268标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[忘记密码																](https://so.csdn.net/so/search/s.do?q=忘记密码&t=blog)[root																](https://so.csdn.net/so/search/s.do?q=root&t=blog)[密码重置																](https://so.csdn.net/so/search/s.do?q=密码重置&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=root&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=忘记密码&t=blog)个人分类：[数据库																](https://blog.csdn.net/liumiaocn/article/category/6328292)
[
																								](https://so.csdn.net/so/search/s.do?q=忘记密码&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)

![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果忘记了root密码的情况下，需要修改root密码可以考虑使用如下方式进行，首先在mysql的设定文件中设定跳过权限认证，然后重启mysql之后重新设定密码，之后再将权限认证跳过的部分去除，之后再此重启即恢复使用新密码
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# Step 1: 设定权限认证跳过
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# grep skip-grant-tables /etc/mysql/mysql.conf.d/mysqld.cnf
```
```python
# echo "skip-grant-tables" >>/etc/mysql/mysql.conf.d/mysqld.cnf
```
```python
# grep skip-grant-tables /etc/mysql/mysql.conf.d/mysqld.cnf
```
```python
skip-grant-tables
```
```python
#
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# Step 2: 重启mysql服务并设定新密码
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)重启mysql服务，此处由于使用的是mysql的官方镜像，所以重启容器即可。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)重新设定mysql的新密码
由于已经设定了权限认证跳过，所以此处无需输入密码，输入mysql -uroot -p之后，密码无需输入，既可以以root身份登入mysql
\# mysql -uroot -pEnter password: 
Welcometothe MySQL monitor.Commandsendwith;or\g.Your MySQL connection idis3Server version:5.7.16-log MySQL Community Server(GPL)Copyright(c)2000,2016,Oracleand/orits affiliates.Allrights reserved.Oracleisa registered trademarkofOracle Corporationand/orits
affiliates.Other names may be trademarksoftheir respective
owners.Type'help;'or'\h'forhelp.Type'\c'toclear thecurrentinput statement.mysql>selectuser();+--------+|user()|+--------+|root@|+--------+1rowinset(0.00sec)mysql>重新设定mysql的root用户的密码
mysql>usemysql;Readingtableinformationforcompletionoftableandcolumnnames
You can turnoffthis featuretoget a quicker startupwith-ADatabasechanged
mysql>updateusersetauthentication_string=password("liumiaocn")whereuser="root";Query OK,1rowaffected,1warning(0.03sec)Rowsmatched:1Changed:1Warnings:1mysql>Step 3: 去除权限跳过设定
\# grep skip-grant-tables /etc/mysql/mysql.conf.d/mysqld.cnfskip-grant-tables\# sed -i s/skip-grant-tables//g /etc/mysql/mysql.conf.d/mysqld.cnf\# grep skip-grant-tables /etc/mysql/mysql.conf.d/mysqld.cnf\#Step 4: 重启mysql服务并以新密码登入
重启mysql服务之后，此时再使用空密码试图进入，则会提示如下错误信息。
\# mysql -uroot -pEnter password: 
ERROR1045(28000): Access deniedforuser'root'@'localhost'(usingpassword:NO)\#使用新修改的密码liumiaocn则可正确登入了
\# mysql -uroot -pliumiaocnmysql:[Warning]Usinga passwordonthe command line interface can be insecure.Welcometothe MySQL monitor.Commandsendwith;or\g.Your MySQL connection idis3Server version:5.7.16-log MySQL Community Server(GPL)Copyright(c)2000,2016,Oracleand/orits affiliates.Allrights reserved.Oracleisa registered trademarkofOracle Corporationand/orits
affiliates.Other names may be trademarksoftheir respective
owners.Type'help;'or'\h'forhelp.Type'\c'toclear thecurrentinput statement.mysql>总结
修改root密码主要是需要将权限认证的部分进行跳过，在这种情况下重设密码，当然root的密码还是妥善保存为好，另外在实际的环境中使用这种方式如同开了一个后门，audit信息也没有得到很好的保存，需要慎重操作。
[
						](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[
	](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
