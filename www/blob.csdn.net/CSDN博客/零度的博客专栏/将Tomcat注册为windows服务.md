# 将Tomcat注册为windows服务 - 零度的博客专栏 - CSDN博客
2016年05月13日 10:37:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：342
1. 首先检查系统是不是之前有安装过之前的版本的tomcat，如果有的话，请将其删除，删除的办法是，到windows的控制面板--->计算机管理--->服务 
在服务列表里查看有没有Apache 或tomcat相关的服务， 
如果有的话，请在上面点鼠标右键--->属性，记下此服务的名称，假如为Tomcat6 
此时进入windows的命令行，开始菜单--->运行--->cmd 
在命令行里输入 
sc delete Tomcat6 
此时就能删除。上面一行命令行的Tomcat6就是系统服务里的属性名称 
2. 注册新服务 
在命令行里进入到下载的免安装版的tomcat目录下的bin目录 
在此bin目录下运行 
service.bat install 
此时就能把免安装版的tomcat注册为系统服务， 
你再到windows的控制面板--->计算机管理--->服务，在服务列表里就能看到你刚新加的服务 
其实上面说了这么多，原理很简单，下面把这个基本原理讲一下， 
Tomcat/bin目录下自带了一个service.bat文件，我们执行的就是这个bat文件， 
打开service.bat文件，你会发现以下代码，重要代码用红色标记： 
set EXECUTABLE=%CATALINA_HOME%\bin\tomcat5.exe 
rem Set default Service name 
set SERVICE_NAME=Tomcat5 
set PR_DISPLAYNAME=Apache Tomcat
if "%1" == "" goto displayUsage 
if "%2" == "" goto setServiceName 
set SERVICE_NAME=%2 
set PR_DISPLAYNAME=Apache Tomcat %2 
:setServiceName 
if %1 == install goto doInstall 
if %1 == remove goto doRemove 
if %1 == uninstall goto doRemove
echo Unknown parameter "%1" 
:displayUsage 
echo. 
echo Usage: service.bat install/remove [service_name] 
goto end 
:doRemove
rem Remove the service 
"%EXECUTABLE%" //DS//%SERVICE_NAME% 
echo The service '%SERVICE_NAME%' has been removed 
goto end 
:doInstall
rem Install the service 
echo Installing the service '%SERVICE_NAME%' ... 
echo Using CATALINA_HOME:    %CATALINA_HOME% 
echo Using CATALINA_BASE:    %CATALINA_BASE% 
echo Using JAVA_HOME:        %JAVA_HOME% 
1.红色代码1: 
set SERVICE_NAME=Tomcat5 
set PR_DISPLAYNAME=Apache Tomcat
如果把tomcat设成服务成功后，就要会系统服务里面看到，服务名就是在这里设的 
你如果改一下服务名的话，可以把上面的SERVICE_NAME，PR_DISPLAYNAME的值换掉。 
2.红色代码2: 
:doRemove和:doInstall这段代码就相当于这个bat文件有两个方法，一个叫doRemove，一个doInstall，看名字很好理解，doInstall就表示安装服务，doRemove表示删除服务 
3.红色代码3: 
if %1 == install goto doInstall 
if %1 == remove goto doRemove 
if %1 == uninstall goto doRemove
但是我有在cmd命令行里却用的是： 
service.bat install 
是如何映射的呢? 
主要是通过如下代码来实现通过上面可以看到,%1就表示我们在service.bat install输入的第一个参数就是install,也就是%1=install,发现if %1 == install goto doInstall 
于是就执行doInstall方法，安装服务到系统， 
同样删除服务，一个最简单的方法就是： 
service.bat remove 
或 
service.bat uninstall 
OK.一切就这么简单 
