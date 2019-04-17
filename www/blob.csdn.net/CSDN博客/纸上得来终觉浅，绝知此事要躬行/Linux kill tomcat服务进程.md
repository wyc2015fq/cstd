# Linux kill tomcat服务进程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月29日 11:42:32[boonya](https://me.csdn.net/boonya)阅读数：3572








## PID杀死进程

查询tomcat进程命令：ps -ef |grep tomcat

![](https://img-blog.csdn.net/20161229113325665)


```
[root@RD-CD-APP122 ~]# ps -ef |grep tomcat
root     11209 11028 50 11:25 pts/2    00:01:33 /usr/lib/jvm/java-7-sun/bin/java -Djava.util.logging.config.file=/usr/lib/tomcat/apache-tomcat-7.0.57/conf/logging.properties -Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager -Xms512m -Xmx1024m -Xss1024K -XX:PermSize=512m -XX:MaxPermSize=1024m -Djava.endorsed.dirs=/usr/lib/tomcat/apache-tomcat-7.0.57/endorsed -classpath /usr/lib/tomcat/apache-tomcat-7.0.57/bin/bootstrap.jar:/usr/lib/tomcat/apache-tomcat-7.0.57/bin/tomcat-juli.jar -Dcatalina.base=/usr/lib/tomcat/apache-tomcat-7.0.57 -Dcatalina.home=/usr/lib/tomcat/apache-tomcat-7.0.57 -Djava.io.tmpdir=/usr/lib/tomcat/apache-tomcat-7.0.57/temp org.apache.catalina.startup.Bootstrap start
root     11332 10275  0 11:28 pts/0    00:00:00 grep tomcat
[root@RD-CD-APP122 ~]# kill -9 11209
[root@RD-CD-APP122 ~]# ps -ef |grep tomcat
root     11334 10275  0 11:28 pts/0    00:00:00 grep tomcat
[root@RD-CD-APP122 ~]#
```


我们将获取到的进程的PID进行kill处理：

**kill -9   pid     #pid 为相应的进程号**

 kill -9 11209


## Shell脚本处理



```
#!/bin/bash
#
# Firstly find the process of the tomcat....
TOMCAT_PROCESS_STR=`ps aux | grep 'java.*tomcat' | grep -v grep`
PROCESS_ARRAY=(${TOMCAT_PROCESS_STR// / })
TOMCAT_PROCESS_ID=${PROCESS_ARRAY[1]}
echo $TOMCAT_PROCESS_ID
#
# Secondly send stop tomcat to see if we can kill it
stoptomcat

sleep 12
#
# Last we'll use kill tomcat process in a looking up loop
for ((a=1;a<=100;a++))
do
  CHECK_STR_LENGTH=0
  TOMCAT_PROCESS_CHECK_STR=`ps aux | grep 'java.*tomcat' | grep -v grep`
  CHECK_STR_LENGTH=${#TOMCAT_PROCESS_CHECK_STR}
  if [ $CHECK_STR_LENGTH != 0 ]
  then
    kill $TOMCAT_PROCESS_ID
    sleep 5
    echo Try to kill tomcat once more...
  else
    echo Tomcat is already killed
    break
  fi
done
```





