
# Linux执行定时任务（crontab）遇到的坑 - Magician的博客 - CSDN博客


2018年11月23日 15:37:09[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：878



### 文章目录
[前言：](#_1)
[1、建立定时任务的两种方式](#1_14)
[1.1、crontab -e](#11crontab_e_18)
[1.2、vi /etc/ crontab](#12vi_etc_crontab_26)
[2、两种方法的区别](#2_39)
[2.1、用户级](#21_43)
[2.2、系统级](#22_55)
[3、解决办法](#3_81)

## 前言：
之前第一次要在生产环境部署定时任务，无奈的是，博主对定时任务这块还是个小白，但是任务却需要部署一连串的定时任务，前后的定时任务之间还有关联。。。由于数据采集的时间有特殊要求，所以整个流程就挺恶心的更别说部署了，部署之后有想过用Azkaban来串联多个定时任务，但是由于其他原因也没来得及实践，这里记录下期间遇到的坑。
刚开始实现第一个crontab定时任务时，就遇到很多问题。这里参考了网上的一些文章，如：
`第一步：写cron脚本文件,命名为crontest.cron。 
15,30,45,59 * * * * echo “xgmtest…..” >> xgmtest.txt 表示，每隔15分钟，执行打印一次命令
第二步：添加定时任务。执行命令 “crontab crontest.cron”。搞定
第三步：”crontab -l” 查看定时任务是否成功或者检测/var/spool/cron下是否生成对应cron脚本`
# 1、建立定时任务的两种方式
但是差点把之前老员工部署的定时任务给搞丢（由于不懂原理差点铸成大错）。。所以就先来说下定时任务的两种添加方式：
## 1.1、crontab -e
`#直接写入定时任务
*/10 17-18 * * * root tcpdump -i eth0 tcp port 80 -s 0 -w sohu1.txt
*/10 17-18 * * * root tcpdump -i eth0 tcp port 80 -s 0 -w sohu1.cap`
## 1.2、vi /etc/ crontab
`vi /etc/ crontab
#添加定时任务 
30 10 * * * root /sbin/reboot
重新加载配置
/sbin/service crond reload
重启cron
/sbin/service crond restart`
# 2、两种方法的区别
**crontab -e对应的是用户级，vi /etc/ crontab对应的系统级的**
## 2.1、用户级
使用crontab -e 这个命令会自动打开vim然后编辑定时脚本文件，编写后保存，在ubuntu下会被写到/var/spool/cron/crontabs目录下，生成一个和用户名一致的文件,我们可以直接用crontab -l查看内容,
`ml@linux:~$ crontab -e
ml@linux:~$ crontab -l
30 * * * * /home/ml/tools/login.sh
ml@linux:~$ sudo cat /var/spool/cron/crontabs/ml
30 * * * * /home/ml/tools/login.sh`
## 2.2、系统级
cronotab -e是针对用户来设计的，如果是系统的例行性任务，需要编辑/etc/crontab这个文件，编辑完所有用户都会受其影响：
`SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
HOME=/
# run-parts
01 * * * * root run-parts /etc/cron.hourly
02 4 * * * root run-parts /etc/cron.daily
22 4 * * 0 root run-parts /etc/cron.weekly
42 4 1 * * root run-parts /etc/cron.monthly
0 * * * *  root /home/ml/tools/login.sh`每列分别是分、时、日、月及周进行一次的工作！但是在五个字段后面接的并不是命令，而是执行命令的身份！这个用户的crontab -e不相同。由于用户自己的crontab并不需要指定身份，但/etc/crontab里面需指定身份。
OK，添加了定时任务就要看这个定时任务是否生效，但是实际操作中执行定时的python脚本时，如下
`#40 15 * * * /usr/local/bin/python3  /opt/FileTransclation.py（每天在15:40自动上传文件）`却报各种错误，基本上全是和环境相关，但问题是，我再Linux下手动执行改脚本时，却没有任何问题，这里大胆怀疑定时任务在执行这个python脚本时用的别的环境变量，后来查了资料发现，怀疑果然成立，crontab有自己的用户，在执行脚本的时候会用自己的环境变量，而我们用的是我们自己的环境变量（装有python及所需环境），所以也就能解释为什么执行python脚本会失败了。根据网上的文章一顿操作，修改了crontab的启动时的环境变量，但是发现问题依旧存在。。。但是，这就真的没有其他的解决办法了吗？怎么会，既然没法直接启动python脚本，我们可以间接启吗！既然crontab的环境变量和用户的环境变量有差异，那我们就利用两者的环境变量相同的地方。
# 3、解决办法
通过自定义shell脚本，然后用定时任务来启shell脚本（shell环境变量两者肯定都有吧），在shell脚本里面来启Python脚本，但前提是你要指定对应的环境变量
模板如下：
**第一步：添加定时任务**
`crontab -e 
#添加定时任务
40 09 * * * cd /root/auto_call; ./start_auto_call.sh >> auto_call.log 2>&1`**第二步：创建shell脚本**
vi  start_auto_call.sh
`#!/bin/bash
. /etc/profile
. ~/.bash_profile
set -x
python auto_call.py`这样只要能在你当前环境下运行的任务，也一定可以在crontab中运行！
参考：[https://blog.csdn.net/idkevin/article/details/72948962](https://blog.csdn.net/idkevin/article/details/72948962)

