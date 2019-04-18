# Linux下Tomcat开机自动启动 - z69183787的专栏 - CSDN博客
2016年07月08日 13:52:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：559
个人分类：[服务器-Tomcat](https://blog.csdn.net/z69183787/article/category/2175825)
sudo service tomcat restart
Linux下tomcat开机自动启动有两种方法，一种是简单，一种是复杂而又专业的，使用shell脚本要实现，我们一般推荐shell脚本启动方式。下面我们分别介绍这两种方法。
## 1.shell脚本启动
众所周知，在Linux中设置开机自启动的服务，需要在/etc/rcX.d下挂载。除此之外还需要在/etc/init.d下些启动脚本。其实很简单，先export出所需要的环境变量，例如JAVA_HOME，JRE_HOME, CATALINA_HOME, CATALINA_BASE什么的，然后再直接调用$CATALINA_HOME/bin/startup.sh就能成功启动。
第一步：我们在/etc/init.d下新建一个文件tomcat（需要在root权限下操作）
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- vi /etc/init.d/tomcat  
写下如下代码，tomcat自启动脚本：
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- #!/bin/sh  
- # chkconfig: 3459910
- # description: Auto-starts tomcat  
- # /etc/init.d/tomcatd  
- # Tomcat auto-start  
- # Source function library.  
- #. /etc/init.d/functions  
- # source networking configuration.  
- #. /etc/sysconfig/network  
- RETVAL=0
- export JAVA_HOME=/usr/java/jdk1.7.0_60  
- export JRE_HOME=/usr/java/jdk1.7.0_60/jre  
- export CATALINA_HOME=/usr/local/tomcat  
- export CATALINA_BASE=/usr/local/tomcat  
- start()  
- {  
- if [ -f $CATALINA_HOME/bin/startup.sh ];  
-           then  
-             echo $"Starting Tomcat"
-                 $CATALINA_HOME/bin/startup.sh  
-             RETVAL=$?  
-             echo " OK"
- return $RETVAL  
-         fi  
- }  
- stop()  
- {  
- if [ -f $CATALINA_HOME/bin/shutdown.sh ];  
-           then  
-             echo $"Stopping Tomcat"
-                 $CATALINA_HOME/bin/shutdown.sh  
-             RETVAL=$?  
-             sleep 1
-             ps -fwwu root | grep tomcat|grep -v grep | grep -v PID | awk '{print $2}'|xargs kill -9
-             echo " OK"
-             # [ $RETVAL -eq 0 ] && rm -f /var/lock/...  
- return $RETVAL  
-         fi  
- }  
- 
- case"$1" in  
-  start)   
-         start  
-         ;;  
-  stop)    
-         stop  
-         ;;  
- 
-  restart)  
-          echo $"Restaring Tomcat"
-          $0 stop  
-          sleep 1
-          $0 start  
-          ;;  
-  *)  
-         echo $"Usage: $0 {start|stop|restart}"
-         exit 1
-         ;;  
- esac  
- exit $RETVAL  
这里特别提醒注意这一句ps -fwwu root | grep tomcat|grep -v grep | grep -v PID | awk '{print $2}'|xargs kill -9，熟悉Linux命令的人应该都清楚这句话的意义，这里就简单说下前半部分，查询root用户下tomcat的进程PID，个人根据实际情况修改。
第二步：保存退出之后，给其增加可执行权限
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- chmod +x /etc/init.d/tomcat  
第三步：挂载
 将这个shell文件的link连到/etc/rc2.d/目录下。linux的/etc/rcX.d/目录中的数字代表开机启动时不同的run level，也就是启动的顺序，Ubuntu9.10下有0-5六个level，不能随便连到其他目录下，可能在那个目录中的程序启动时Tomcat所需要的一些库尚未被加载，用ln命令将tomcat的链接链过去：sudo
 ln -s /etc/init.d/tomcat /etc/rc2.d/S16Tomcat。rcX.d目录下的命名规则是很有讲究的，更具不同需要可能是S开头，也可能是K开头，之后的数字代表他们的启动顺序，详细看各自目录下的Readme文件。
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- ln -s /etc/init.d/tomcat /etc/rc2.d/S16Tomcat  
第四步：设置脚本开机自启动
把这个脚本设置成系统启动时自动执行，系统关闭时自动停止，使用如下命令:
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- chkconfig --add tomcat  
第五步：验证
执行 sudo reboot。重启之后就会发现，Tomcat已经成功运行了。
题外话：添加这个脚本之后我们启动，停止，重启tomcat可以直接用命令
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- service tomcat start  
- service tomcat stop  
- service tomcat restart  
另外一些tomcat设置命令有： chkconfig --list tomcat      chkconfig  tomcat on，大家可以试试看有什么效果。
## 2.修改系统文件
1.修改/etc/rc.d/rc.local   
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- vi /etc/rc.d/rc.local   
2.添加下面两行脚本，记住是两行，仅仅第二行不行，必须加第一行。在/etc/rc.d/rc.local文件最后加上：
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- export JAVA_HOME=/usr/java/jdk1.7.0_60  
- /usr/local/tomcat/bin/startup.sh start  
说明：/usr/java/jdk1.7.0_60 是jdk安装目录
         /usr/local/tomcat是tomcat安装的目录
3.注意:修改rc.local文件为可执行
**[java]**[view
 plain](http://blog.csdn.net/wangli61289/article/details/37924785#)[copy](http://blog.csdn.net/wangli61289/article/details/37924785#)
[print](http://blog.csdn.net/wangli61289/article/details/37924785#)[?](http://blog.csdn.net/wangli61289/article/details/37924785#)
- chmod +x  rc.local  
第一种shell脚本的方式实现tomcat的开机自启动已经实践过，第二种没用过，只是在网上搜到有这方面的资料，所以记录下来，方便以后学习。如果想了解更多请参考文档：
[http://www.cnblogs.com/gnorts/archive/2010/11/24/1886771.html](http://http//www.cnblogs.com/gnorts/archive/2010/11/24/1886771.html)
[http://jingyan.baidu.com/article/1612d50079cfe5e20f1eee71.html](http://jingyan.baidu.com/article/1612d50079cfe5e20f1eee71.html)
