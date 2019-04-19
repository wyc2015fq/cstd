# 在Ubuntu Server 32bit搭建hustoj -- 下 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2016年05月11日 15:17:38[dutong0321](https://me.csdn.net/dutong0321)阅读数：461标签：[数据库																[hustoj																[ubuntu																[Server](https://so.csdn.net/so/search/s.do?q=Server&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=hustoj&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[Server](https://blog.csdn.net/dutong0321/article/category/6222374)
首先，我们假定已经把HUSTOJ安装完毕，上面的过程辣么详细，相信没有人没有安装失败吧！
其实，剩下的就是一些配置问题了。
1.如果导入数据库
 进入mysql，执行以下命令，假设备份文件是/home/acmmanager/jol.sql
```
use jol;
set names utf8;
source /home/acmmanager/jol.sql;
```
2.永久PENDING和RUNNING
修改配置文件。
` sudo vim /home/judge/etc/judge.conf`OJ_HOST_NAME=127.0.0.1 数据库主机地址
OJ_USER_NAME=root 数据库用户名
OJ_PASSWORD=****** 数据库密码
OJ_DB_NAME=jol 数据库名称
OJ_PORT_NUMBER=3306 数据库端口
OJ_RUNNING=8 提升判题的judge_client
OJ_SHM_RUN=1 也是可以提高一定速度
`sudo vim /var/www/JudgeOnline/include/db_info.inc.php`
static  $DB_HOST="localhost"; 数据库地址
static  $DB_NAME="jol"; 数据库民称
static  $DB_USER="root"; 数据库用户名
static  $DB_PASS="********"; 数据库用户密码
static  $OJ_NAME="HUSTOJ"; oj名称
static  $OJ_LANG="cn"; 语言选择
static $OJ_LANGMASK=131060;支持的语言 二进制 0为支持 这个值是C C++ java
重启judged
```
ps -ef | grep judged
sudo kill -9 ****
sudo service judged start
```
嗯，现就这样！有啥想起来再说！
