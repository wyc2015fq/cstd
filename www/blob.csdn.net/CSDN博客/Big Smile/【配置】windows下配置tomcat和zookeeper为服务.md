# 【配置】windows下配置tomcat和zookeeper为服务 - Big Smile - CSDN博客
2018年02月27日 18:31:46[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1286
最近在电脑上配置dubbo的环境，由于使用的windows的系统，所以想让dubbo控制台和依赖的zookeeper都常驻系统后台，并且在我开机的时候能够启动，这就不得不让我想到了吧他们作为系统的服务来启动，这样系统在开机的时候就可以自动运行这些服务了。
# 一、tomcat配置为服务
        把tomcat配置为服务是比较简单的，因为tomcat自带了一个service.bat，我们可以直接把它设置为服务，不过需要配置一下：
         1、由于我们还要在电脑上跑别的项目，所以需要修改tomcat的端口号，使他们互不干扰。
![](https://img-blog.csdn.net/20180227170922660)
找到tomcat目录，conf文件夹下的server.xml修改如图的三个地方的端口号为自己能记住的端口。
      2、然后配置一下运行自带service.bat所需的环境变量：
PATH：配置到jdk的bin目录下： 
Path后添加：;%JAVA_HOME%\bin;%CATALINA_HOME%\bin
CLASSPATH:配置到jdk的lib目录下：
CLASSPATH：.;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib\tools.jar;
JAVA_HOME:配置到jdk的根目录：
JAVA_HOME：C:\Program Files\Java\jdk1.8.0_21
CATALINA_HOME:配置到tomcat的根目录； 
CATALINA_HOME：D:\tools\apache-tomcat-8.5.23
配置好以后我们就可以运行service.bat了：
     3、打开cmd命令提示符，cd到tomcat所在目录的bin文件夹下，然后执行如下命令：
![](https://img-blog.csdn.net/20180227171731126)
最后出现 The service 'Tomcat8' has been installed.  就说明成功了，我们在服务中可以看到：
win+r，然后输入services.msc，进入系统服务：
![](https://img-blog.csdn.net/20180227171926849)
我们可以看到这个服务就已经在了，只要手动启动一下，就可以用了，重启电脑的话也会自己启动的。
        有的同志可能会问了，这个服务名字这么长，还是默认的，如果我想自己起名字，或者发布多个的时候会不会重复呢？这里我们也可以通过修改bin目录下的service.bat来实现修改服务名称：
       我们可以在tomcat的bin目录下找到service.bat文件，然后打开，找到：
![](https://img-blog.csdn.net/20180227172742136)
然后修改成自己想要的服务名就行了。（注：我这个是tomcat版本是8.5.23，不同版本可能有差异，但是7.0的也有SERVICE_NAME 这个项）
# 二、ZooKeeper服务配置
       zookeeper的服务配置可能有些难度，我们在配置之前需要先下载一个Apache的插件才可以，因为像是tomcat一类的Apache的工具都是通过这个来实现服务启动的：
1、下载
       Zookeeper的下载地址：[http://zookeeper.apache.org/](http://zookeeper.apache.org/)
       prunsrv 的下载地址：[http://archive.apache.org/dist/commons/daemon/binaries/windows/](http://archive.apache.org/dist/commons/daemon/binaries/windows/)
以上两个东西都下载了之后，我们需要做一些操作：
       64位机器用amd64/prunsrv.exe 
　　a. 复制 commons-daemon-1.0.15-bin-windows/amd64/prunsrv.exe 至 zookeeper-3.4.8\bin目录下
　　这是做服务的执行程序 
　　b. 复制 commons-daemon-1.0.15-bin-windows/prunmgr.exe 至 zookeeper-3.4.8\bin目录下
　　监控服务配置及运行的程序 
2、我们需要在windows中配置环境变量：
   添加ZOOKEEPER_SERVICE， ZOOKEEPER_HOME两个环境变量
     ZOOKEEPER_SERVICE：服务名称（不要有中文）
     ZOOKEEPER_HOME：设置到zookeeper-3.4.8目录下
3、我们需要配置zookeeper，把zookeeper-3.4.8\conf目录下复制一份zoo_sample.cfg文件，并重命名为zoo.cfg。（默认配置）
4、我们需要建立服务部署文件，就是两个批处理文件
在zookeeper-3.4.8\bin目录下添加一个 zkServerStop.cmd 文件：
```
@echo off
setlocal
TASKLIST /svc | findstr /c:"%ZOOKEEPER_SERVICE%" > %ZOOKEEPER_HOME%\zookeeper_svc.pid
FOR /F "tokens=2 delims= " %%G IN (%ZOOKEEPER_HOME%\zookeeper_svc.pid) DO (
    @set zkPID=%%G
)
taskkill /PID %zkPID% /T /F
del %ZOOKEEPER_HOME%/zookeeper_svc.pid
endlocal
```
然后在相同目录下建一个批处理安装文件：install.bat
```
prunsrv.exe "//IS//%ZOOKEEPER_SERVICE%" ^
        --DisplayName="Zookeeper (%ZOOKEEPER_SERVICE%)" ^
        --Description="Zookeeper (%ZOOKEEPER_SERVICE%)" ^
        --Startup=auto --StartMode=exe ^
        --StartPath=%ZOOKEEPER_HOME% ^
        --StartImage=%ZOOKEEPER_HOME%\bin\zkServer.cmd ^
        --StopPath=%ZOOKEEPER_HOME%\ ^
        --StopImage=%ZOOKEEPER_HOME%\bin\zkServerStop.cmd ^
        --StopMode=exe --StopTimeout=5 ^
        --LogPath=%ZOOKEEPER_HOME% --LogPrefix=zookeeper-wrapper ^
        --PidFile=zookeeper.pid --LogLevel=Info --StdOutput=auto --StdError=auto
```
最后在以管理员的身份运行一下install.bat就可以了，然后我们在服务里面就可以看到他了：
![](https://img-blog.csdn.net/20180227182838330)
