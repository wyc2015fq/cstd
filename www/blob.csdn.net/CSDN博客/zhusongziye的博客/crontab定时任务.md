# crontab定时任务 - zhusongziye的博客 - CSDN博客





2017年11月25日 19:02:27[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：137








                
相信大家在实际工作时，经常碰到这类需求：每天想要定时去备份或者执行某个操作，或者有些任务需要在凌晨某个时间点去执行，那怎么办？总不能等到对应的时间点吧？


祭出神器：crontab！



1. crontab是啥？




crontab命令用于linux操作系统中，用于设置被周期性执行的指令，该命令从标准输入设备中读取指令，并将其存放于crontab文件中。以供之后读取和执行。

crontab存储的指令被守护进程激活，crond通常在后台运行，每一分钟检查是否有预定的作业需要执行，这类作业一般称为cron jobs



2.怎么部署/安装crontab？



以centos系统举例：我们通过yum命令来进行安装

yum –y install vixie-cron

yum –y install crontabs

vixie-cron软件包是cron的主程序

crontabs软件包是用来管理安装、卸载



3.crontab使用相关命令



service crond start    //启动服务

service crond stop     //关闭服务

service crond restart  //重启服务

service crond reload   //重新载入配置

service crond status   //查看服务状态



4.相关配置文件



/var/spool/cron/ 这个目录下存放的是每个用户包括root的crontab任务，每个任务以创建者的名字命名，比如tom建的crontab任务对应的文件就是/var/spool/cron/tom。一般一个用户最多只有一个crontab文件

/etc/crontab 这个文件负责安排由系统管理员制定的维护系统以及其他任务的crontab

/etc/cron.d/ 这个目录用来存放任何要执行的crontab文件或脚本



作为普通用户来讲只需关注/var/spool/cron/对应用户的配置文件即可
![](https://img-blog.csdn.net/20171125190044711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



我们可以使用crontab -e命令去编辑这个配置文件



crontab配置文件格式，我们可以使用more /etc/crontab命令查看crontab配置说明信息
![](https://img-blog.csdn.net/20171125190103734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


minute： 表示分钟，可以是从0到59之间的任何整数。

hour：表示小时，可以是从0到23之间的任何整数。

day：表示日期，可以是从1到31之间的任何整数。

month：表示月份，可以是从1到12之间的任何整数。

week：表示星期几，可以是从0到7之间的任何整数，这里的0或7代表星期日。

command：要执行的命令，可以是系统命令，也可以是自己编写的脚本文件。



特殊字符

星号（）：代表所有可能的值，例如month字段如果是星号，则表示在满足其它字段的制约条件后每月都执行该命令操作。

逗号（,）：可以用逗号隔开的值指定一个列表范围，例如，“1,2,5,7,8,9”。

中杠（-）：可以用整数之间的中杠表示一个整数范围，例如“2-6”表示“2,3,4,5,6”。

正斜线（/）：可以用正斜线指定时间的间隔频率，例如“0-23/2”表示每两小时执行一次。同时正斜线可以和星号一起使用，例如/10，如果用在minute字段，表示每十分钟执行一次。



5.实例
![](https://img-blog.csdn.net/20171125190135635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

完成配置退出保存即可

是不是想要满足我们的需求so easy？（转自：shakebabe）
            


