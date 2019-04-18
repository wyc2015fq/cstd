# Weblogic 内存设置 - z69183787的专栏 - CSDN博客
2013年02月20日 13:22:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8108
注：11G中 生产模式默认jdk 为 jrockit
由于WebLogic的配置问题，我们的测试出现了失败情况。原因是为WebLogic分配的内存太少了。通过修改commom\bin\commEnv.cmd文件来增加内存分配。
修改的部分如下：
:bea
if "%PRODUCTION_MODE%" == "true" goto bea_prod_mode
set JAVA_VM=-jrockit
set MEM_ARGS=-Xms768m -Xmx1024m
set JAVA_OPTIONS=%JAVA_OPTIONS% -Xverify:none
goto continue
:bea_prod_mode
set JAVA_VM=-jrockit
set MEM_ARGS=-Xms768m -Xmx1024m//原来是128M~256M，太小了，数据太大
goto continue
结果修改后，没有效果。还是有失败的情况。
发现，原来，在：bea下面还有一段配置信息如下：
:sun
if "%PRODUCTION_MODE%" == "true" goto sun_prod_mode
set JAVA_VM=-client
set MEM_ARGS=-Xms768m -Xmx1024m -XX:MaxPermSize=256m
set JAVA_OPTIONS=%JAVA_OPTIONS% -Xverify:none
goto continue
:sun_prod_mode
set JAVA_VM=-server
set MEM_ARGS=-Xms768m -Xmx1024m -XX:MaxPermSize=256m
goto continue
将这里的内存分配修改后见效。
原因是，上面对第一段代码是为bea自己的JVM设置的，下面的是为Sun的设置的。而WebLogic默认的是Sun的，所以出了毛病。在JDK的选择上，weblogic有两种JDK供选择，一种是Sun的JDK，另外一种是Bea的jrockit。按照bea的网站的说明，sun
 jdk提供更好的兼容性，而使用jrockit可以提供更好的性能。作为weblogic集群我全部采用jrockit作为JDK环境，以达到更高的性能。
在默认启动情况下，jrockit启动时为其窗口配置的内存大小比较小。注意weblogic的启动内存配置-Xms32m -Xmx256m，通过修改commEnv.sh可以修改这个参数，Xms表示启动开始分配的内存，Xmx表示最大能分配的内存，这里我们根据应用情况调整为-Xms1536m -Xmx1536m，这点需要根据自身测试情况和系统配置进行调整，经过周一晚的调试，我们目前应用比较合理的窗口内存大小为1536M（2G× 75％），通过top可以观察到测试中的内存反应，最合理的应该是恰好把物理内存用完。
