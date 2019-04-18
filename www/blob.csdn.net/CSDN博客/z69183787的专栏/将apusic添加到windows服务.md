# 将apusic添加到windows服务 - z69183787的专栏 - CSDN博客
2013年11月12日 18:35:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：961
  apusic应用服务器安装后，默认可以通过%domain_home%\bin\startapusic.cmd启动apusic服务，也可以通过开始-程序里的快捷方式启动apusic服务。
    上述两种方式都需要手工启动，有的时候可能需要让apusic服务开机自动启动，我们可以将apusic服务添加到windows服务中，并设置自动启动。
    apusic应用服务器自带了apusicsvc.exe来完成将apusic服务添加到windows服务的工作，在apusic6.0中，这个文件是在%domain_home%\bin\目录下。
1.默认安装
    在命令行窗口，进入%domain_home%\bin目录，执行如下命令：
**[plain]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- apusicsvc.exe -install   
   查看windows服务列表，可以看到名为“ApusicServer”的服务，可以对此服务启动、停止等操作，也可以将服务设置成自动启动。
    已经添加到windows服务的apusic，也可以从服务列表移除，通过如下命令：
**[plain]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- apusicsvc.exe -uninstall  
   可以看到apusic服务已经从windows服务列表移除。
2.指定服务名称
    默认安装的apusic服务在windows服务列表中的名称为“ApusicServer”，也可以在安装时指定服务名称：
**[plain]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- apusicsvc.exe -install -name "apusic"  
安装的apusic服务在windows服务列表中的名称为“apusic”，如果要移除，需要如下命令：
**[plain]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- apusicsvc.exe -uninstall apusic  
3.指定jvm参数
    安装apusic服务到windows服务列表时，还可以指定JVM参数，如：
**[plain]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- apusicsvc.exe -install -Xms256M -Xmx256M  
4.可用参数
    命令行输入：apusicsvc.exe，回车，可以查看可用的参数：
**[cpp]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7732020#)[copy](http://blog.csdn.net/shenxueshi/article/details/7732020#)
- C:\Apusic-6.0\domains\mydomain\bin>apusicsvc.exe    
- Usage: apusicsvc [-options]    
- 
- where options include:    
-     -install [Apusic args...]    
-                  install Apusic as windows service    
-     -uninstall [Service Name]    
-                  uninstall the Apusic service    
- where Apusic args include:    
-     -cp -classpath <directories and zip/jar files separated by ;>    
-                  set search path for application classes and resources    
-     -D<name>=<value>    
-                  set a system property    
-     -X    
-                  JVM non-standard options    
-     -name <service name>    
-                  set the service name of server    
-     -root <directory>    
-                  set the root directory of server    
- 
- for example: apusicsvc -install    
-     install Apusic as windows service without any startup params    
- 
- C:\Apusic-6.0\domains\mydomain\bin>  
