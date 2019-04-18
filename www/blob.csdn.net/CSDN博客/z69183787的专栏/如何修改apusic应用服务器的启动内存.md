# 如何修改apusic应用服务器的启动内存 - z69183787的专栏 - CSDN博客
2013年11月12日 18:34:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1354
    tomcat应用服务器可以通过在catalina.bat中添加“set JAVA_OPTS=-Xms128m -Xmx350m”这样的设置，来修改tomcat的启动内存。
    apusic应用服务器也可以设置启动内存。
一、windows下设置启动内存
    到%apusic_home%\domains\mydomain\bin目录下，找到startapusic.cmd文件，用记事本或其他程序打开，找到
**[cpp]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7885015#)[copy](http://blog.csdn.net/shenxueshi/article/details/7885015#)
- set JVM_OPTS=-server -Xms128m -Xmx512m -XX:MaxPermSize=128m  
    把“-Xms128m -Xmx512m”适当调大，而且两个参数最好保持一致，保存退出。
    具体在set JVM_OPTS处可以设置哪些参数，代表什么含义，可以百度“JVM启动参数”等类似关键字，此处不赘述。
    打开命令行窗口，进入%apusic_home%\domains\mydomain\bin目录，运行如下命令，以product模式启动apusic应用服务器：
**[cpp]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7885015#)[copy](http://blog.csdn.net/shenxueshi/article/details/7885015#)
- startapusic.cmd -p  
    这样apusic启动时设置的启动内存就可以生效了。
    有人可能觉得这样从命令行窗口启动麻烦，想从开始-程序启动，这样默认是以normal方式启动，也可以设置启动内存。
    这时除了需要在startapusic.cmd中如上所述修改“set JVM_OPTS=-server”参数外，还需要在这个文件中找到“:START_NORMAL”，在这行下面约10行处有：
**[cpp]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7885015#)[copy](http://blog.csdn.net/shenxueshi/article/details/7885015#)
- %JAVA_RUN% -Dcom.apusic.domain.home="%DOMAIN_HOME%" com.apusic.server.Main -root "%APUSIC_HOME%"
修改成
**[cpp]**[view
 plain](http://blog.csdn.net/shenxueshi/article/details/7885015#)[copy](http://blog.csdn.net/shenxueshi/article/details/7885015#)
- %JAVA_RUN% -Dcom.apusic.domain.home="%DOMAIN_HOME%" %JVM_OPTS% com.apusic.server.Main -root "%APUSIC_HOME%"
    这个时候从开始-程序里启动apusic，设置的启动内存也可以生效。
二、linux下设置启动内存
    linux下设置启动内存，跟windows下类似，到%apusic_home%\domains\mydomain\bin目录下，找到startapusic文件，用vi命令打开编辑，找到“JVM_OPTS=-server”处，进行修改，保存退出后，使用“./startapusic -p”命令启动apusic即可。
