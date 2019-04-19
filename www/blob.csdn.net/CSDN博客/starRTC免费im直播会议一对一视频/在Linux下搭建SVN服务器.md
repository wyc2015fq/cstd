# 在Linux下搭建SVN服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:25:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：244
svn不仅仅可以用于程序开发，还可以做很多事情，例如备份文档。
## CentOS下：安装
**yum install **-y subversion
Ubuntu下：
**sudo ****apt-get install** subversion
检查是否安装成功
**svnserve **--version
## 建立项目
**mkdir **-p ~**/****svn            ****//**建立**svn**根目录，用于存放多个项目，其中“~”表示home目录
新建项目proj1
**mkdir **-p ~**/**svn**/**proj1  **//**其中-p表示没有父目录则新建
**svnadmin** create ~**/**svn**/**proj1
命令完成后，会在proj1目录下生成相应的文件和文件夹：
再建立1个项目proj2
**mkdir**-p ~**/**svn**/**proj2
**svnadmin** create ~**/**svn**/**proj2
## 配置SVN
将proj1/conf下的authz、passwd和svnserve.conf拷贝至~/svn/根目录下，结果如下所示：
### 添加用户
注意：不需要在linux中添加用户
vi ~/svn/passwd，添加形如“username=password”的条目即可
### 权限控制
用于设置哪些用户可以访问哪些目录
vi ~/svn/authz，添加
**[****groups****]**admin=test1
devteam1=test2,test3 **//**开发组一
devteam2=test4
 
**[****/****]****@**admin=rw
*****=**[**proj1:**/****]****@**devteam1=rw
**[**proj2:**/****]****@**devteam2=rw
注： 此处的用户名必须是“passwd”文件中已经定义的用户名
1个用户组可以包含1个或多个用户，用户间以逗号分隔
*=表示除了有权限的用户之外，其他任何人都禁止访问本目录。
进入proj1/conf，配置svnserve.conf（左侧不能留空格），让用户和权限控制策略生效
**[**general**]**
anon-access=none  *# 使非授权用户无法访问*
auth-access=**write***#使授权用户有写权限 *
password-db=~**/**svn**/****passwd***#密码文件 *
authz-db=~**/**svn**/**authz*#访问控制文件*
以后建立的其它项目可以直接使用此配置，如：
**cp** ~**/**svn**/**proj1**/**conf**/**svnserve.conf  ~**/**svn**/**proj2**/**conf**/**svnserve.conf**或cp** ~**/**svn**/**svnserve.conf  ~**/**svn**/**proj2**/**conf**/**svnserve.conf
## 启动
**svnserve **-d -r ~**/****svn** （注意不是~**/**svn**/**proj1）
-d表示以守护模式运行
svn默认监听3690端口，如果已经有svn在运行了，可以使用“--listen-port=”指定其它端口
**svnserve**-d-r ~**/**svn**/**--listen-port3391
这样同一台服务器便可以运行多个svnserver了
检查端口
**netstat**-ntlp**|****grep**3690
注：如果修改了svn配置，需要重启svn服务
*# ps -aux|grep svnserve**# kill -9 ID号 //  -9表示强制终止**# svnserve -d -r ~/svn*
开启3690端口
**/**sbin**/**iptables -I INPUT -p tcp --dport3690-j ACCEPT
**/**etc**/**init.d**/**iptables save   **//**保存
service iptables restart    **//**重启防火墙
可通过cat /etc/sysconfig/iptables查看是否将3690端口添加成功
## windows下测试
需要安装TortoiseSVN，有32位和64位
安装完成后，可以右键打开Repo-browser,（url填svn://ip/proj1）建立一个目录后拖动代码进去，然后在另一个地方就可以check out了
svn目录无绿勾，绿色标记
没有这种
只能不断重新安装和重启了。呵呵。
[SVN Check out时需要一直输入密码的问题](http://www.blogjava.net/DLevin/archive/2012/11/02/390686.html)
home/<user>/.subversion/目录下的config文件找到一下配置：
# store-passwords = no
将“store-passwords = no” 注释掉或将其值设置成yes
在~/.subversion/servers的global域下设置store-plaintext-passwords = yes  
