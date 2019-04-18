# Centos下Web中间件Jboss应用发布start和stop脚本范例 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月03日 09:01:38[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2216








在centos环境部署jboss发布web应用，摸索了发布具体应用的start和stop脚本，范例如下：




1、start脚本：${JBOSS_HOME}/bin/start-app.sh



```
export LANG=zh_CN.GBK
cd /home/jboss/bin
date=1_`date +%Y-%m-%d\ %H:%M`.log

nohup ./run.sh -b 127.0.0.1 -c app > "logs/app.logs" &
```


2、stop脚本：${JBOSS_HOME}/bin/stop-app.sh





```
cd /home/jboss/bin
echo `date +%F\ %T` >> logs/shutdown.log
nohup ./shutdown.sh -S -s 127.0.0.1:1199 -u admin -p admin >> "logs/shutdown.log" 2>&1 &
```


stop脚本最主要是端口和用户名密码，其中服务器配置的JNDI端口（默认端口为1099）在${JBOSS_HOME}/server/app/conf/jboss-service.xml文件中：





```
<mbean code="org.jboss.naming.NamingService"
            name="jboss:service=Naming"
      xmbean-dd="resource:xmdesc/NamingService-xmbean.xml">
      <!-- The call by value mode. true if all lookups are unmarshalled using
         the caller's TCL, false if in VM lookups return the value by reference.
      -->
      <attribute name="CallByValue">false</attribute>
      <!-- The listening port for the bootstrap JNP service. Set this to -1
         to run the NamingService without the JNP invoker listening port.
      -->
      <attribute name="Port">1099</attribute>
      <!-- The bootstrap JNP server bind address. This also sets the default
         RMI service bind address. Empty == all addresses
      -->
```


用户名和密码在${JBOSS_HOME}/server/app/conf/props/jmx-console-users.properties文件中，也可以新增，如admin=admin










