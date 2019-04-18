# linux下让tomcat以service方式运行 - z69183787的专栏 - CSDN博客
2016年05月11日 17:44:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：833
执行一下步骤，可以使Tomcat以Service方式运行：
一、创建启动/停止脚本文件：
**[plain]**[view
 plain](http://blog.csdn.net/jason5186/article/details/7317253#)[copy](http://blog.csdn.net/jason5186/article/details/7317253#)
[print](http://blog.csdn.net/jason5186/article/details/7317253#)[?](http://blog.csdn.net/jason5186/article/details/7317253#)
- #!/bin/bash  
- # chkconfig: 2345 10 90  
- # description: Starts and Stops the Tomcat daemon.  
- TOMCAT_HOME=/home/apache-tomcat-6.0.32  
- TOMCAT_START=$TOMCAT_HOME/bin/startup.sh  
- TOMCAT_STOP=$TOMCAT_HOME/bin/shutdown.sh  
- # necessary environment variables export  
- CATALINA_HOME=$TOMCAT_HOME  
- export JAVA_HOME=/usr/java/jre1.6.0_10  
- # source function library.  
- . /etc/rc.d/init.d/functions  
- if [ ! -f $TOMCAT_HOME/bin/catalina.sh ]  
- then echo "Tomcat not valilable..."  
- exit  
- fi  
- start(){  
- echo -n "Starting Tomcat: "  
- daemon $TOMCAT_START  
- echo  
- touch /var/lock/subsys/tomcat  
- }  
- stop(){  
- echo -n {1}quot;Shutting down Tomcat: "  
- daemon $TOMCAT_STOP  
- rm -f /var/lock/subsys/tomcat.pid echo  
- }  
- restart(){  
- stop  
- start  
- }  
- status(){  
- ps ax --width=1000 | grep "[o]rg.apache.catalina.startup.Bootstrap start" | awk '{printf $1 " "}' | wc | awk '{print $2}' > /tmp/tomcat_process_count.txt  
- read line < /tmp/tomcat_process_count.txt  
- if [ $line -gt 0 ]; then  
- echo -n "tomcat ( pid "  
- ps ax --width=1000 | grep "org.apache.catalina.startup.Bootstrap start" | awk '{printf $1 " "}'  
- echo -n ") is running..."  
- echo  
- else  
- echo "Tomcat is stopped"  
- fi  
- }  
- case "$1" in  
- start)  
- start ;;  
- stop)  
- stop ;;  
- restart)  
- stop  
- sleep 3  
- start ;;  
- status)  
- status ;;  
- *)  
- echo "Usage: tomcatd {start|stop|restart|status}"  
- exit 1  
- esac  
- exit 0  
二、保存并设置脚本文件
将以上的脚本文件保存在/etc/init.d中，命名为tomcat;
设置tomcat的文件属性
#chmod a+x tomcat
三、设置服务运行级别
最后用chkconfig设置服务运行
#chkconfig --add tomcat
服务就添加成功了。
然后你就可以用 chkconfig --list 查看，在服务列表里就会出现自定义的服务了。
注意：
在tomcat文件的头两行的注释语句中，需要包含chkconfig和description两部分内容(确认不要拼写错误，)，否则在执行“chkconfig --add tomcat”时，会出现“tomcat服务不支持chkconfig”的错误提示。
chkconfig这行表示缺省启动的运行级别以及启动和停止的优先级，如该服务缺省不再任何运行级启动，则以 - 代替运行级别。在tomcat中表示脚本在运行级2、3、4、5启动，启动优先权为10，停止优先权为90。
语　法：chkconfig [--add][--del][--list][系统服务] 或 chkconfig [--level <等级代号>][系统服务][on/off/reset]
description行对服务进行描述，可以用 “\” 跨行注释。
