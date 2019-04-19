# Ubuntu下定时任务和自启动任务的部署 - =朝晖= - 博客园
# [Ubuntu下定时任务和自启动任务的部署](https://www.cnblogs.com/dhcn/p/7121439.html)
1、定时任务的部署，最简单的方法是执行
- crontab -e  
然后在下面加上世间周期配置和要执行的命令，一般情况下，可以把要执行的任务用bash脚本封装一下，格式如下所示：
- minute   hour   day   month   week   command  
举例如下：
- 1 * * * *  ~/start_run.sh  
上面这个例子是指没到分钟到1的时候执行一次脚本，脚本的执行间隔就是1小时。
2、自启动任务的部署：
Ubuntu下正规的自启动服务是用update-rc.d管理，但是这个命令只适用于正规编写安装的服务启动脚本，一般自己的写的开机启动任务脚本，可以把任务脚本的启动命令加在/etc/rc.local文件中
3、nohup命令的日志输出重定向到文件：
- #!/bin/bash  
- today=`date +%Y%m%d`  
- filename="log_"$today".log"  
- nohup python -u ~/run.py>>/var/log/systemname/$filename 2>&1 &  

