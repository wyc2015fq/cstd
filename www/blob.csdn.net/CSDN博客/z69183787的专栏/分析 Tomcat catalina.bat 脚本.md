# 分析 Tomcat catalina.bat 脚本 - z69183787的专栏 - CSDN博客
2014年11月08日 10:54:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：975
  Catalina.bat是tomcat所有脚本中最重要的脚本，完成几乎所有的tomcat操作。如启动，关闭等等,都是由catalina.bat脚本来完成的。接下来，我将对Tomcat catalina.bat脚本进行分析。
    首先省去catalina.bat开头诸多注解，这些注解主要是讲解各个变量是干什么的。需要的话，自己看下英文就可以了。这里就不翻译了。
rem Guess CATALINA_HOME if not defined  查看是否在tomcat目录下，与startup.bat里相同，不解释了。需要的话可以看我的另一篇博客。
set CURRENT_DIR=%cd%
if not "%CATALINA_HOME%" == "" goto gotHome
set CATALINA_HOME=%CURRENT_DIR%
if exist "%CATALINA_HOME%/bin/catalina.bat" goto okHome
cd ..
set CATALINA_HOME=%cd%
cd %CURRENT_DIR%
:gotHome
if exist "%CATALINA_HOME%/bin/catalina.bat" goto okHome
echo The CATALINA_HOME environment variable is not defined correctly
echo This environment variable is needed to run this program
goto end
:okHome
rem Get standard environment variables
if exist "%CATALINA_HOME%/bin/setenv.bat" call "%CATALINA_HOME%/bin/setenv.bat" 如果存在setenv.bat脚本，调用它，我的tomcat 没有这个脚本
rem Get standard Java environment variables
if exist "%CATALINA_HOME%/bin/setclasspath.bat" goto okSetclasspath 查看是否存在setclasspath.bat脚本，如果存在，转到okSetclasspath位置
echo Cannot find %CATALINA_HOME%/bin/setclasspath.bat 否则输出下面两行，并退出
echo This file is needed to run this program
goto end
:okSetclasspath    okSetclasspath位置
set BASEDIR=%CATALINA_HOME%   设定BASEDIR变量与CATALINA_HOME变量值相同
call "%CATALINA_HOME%/bin/setclasspath.bat" %1   调用setclasspath.bat脚本并加上参数
if errorlevel 1 goto end      如果存在错误 退出
rem Add on extra jar files to CLASSPATH   设定JSSE_HOME变量，如果存在加入CLASSPATH，不存在跳过
if "%JSSE_HOME%" == "" goto noJsse    检查是否存在JSSE_HOME变量
set CLASSPATH=%CLASSPATH%;%JSSE_HOME%/lib/jcert.jar;%JSSE_HOME%/lib/jnet.jar;%JSSE_HOME%/lib/jsse.jar 如果有加入到CLASSPATH变量后面
:noJsse                                  
set CLASSPATH=%CLASSPATH%;%CATALINA_HOME%/bin/bootstrap.jar  将bootstrap.jar加入到CLASSPATH里
if not "%CATALINA_BASE%" == "" goto gotBase  如果CATALINA_BASE变量不为空，跳过，转到gotBase位置
set CATALINA_BASE=%CATALINA_HOME%  如果为空，将CATALINA_BASE设为CATALINA_HOME变量的值
:gotBase
if not "%CATALINA_TMPDIR%" == "" goto gotTmpdir   CATALINA_TMPDIR不为空，跳过，转到gotTmpdir位置
set CATALINA_TMPDIR=%CATALINA_BASE%/temp   如果为空，将 CATALINA_TMPDIR设为%CATALINA_BASE%/temp变量的值（即tomcat/temp）
:gotTmpdir
if not exist "%CATALINA_HOME%/bin/tomcat-juli.jar" goto noJuli  如果不存在tomcat-juli.jar这个类，转到noJuli位置
set JAVA_OPTS=%JAVA_OPTS% -Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager - Djava.util.logging.config.file="%CATALINA_BASE%/conf/logging.properties"  如果存在，将变量加入到JAVA_OPTS里
:noJuli
set JAVA_OPTS=%JAVA_OPTS% -Xms128m -Xmx512m -Dfile.encoding=UTF8 -Duser.timezone=GMT -Djava.security.auth.login.config=%CATALINA_HOME%/conf/jaas.config  设定JAVA_OPTS变量
echo Using CATALINA_BASE:   %CATALINA_BASE%     输出CATALINA_BASE变量值
echo Using CATALINA_HOME:   %CATALINA_HOME%     输出CATALINA_HOME变量值
echo Using CATALINA_TMPDIR: %CATALINA_TMPDIR% 输出CATALINA_TMPDIR变量值
if ""%1"" == ""debug"" goto use_jdk        如果变量%1里存在debug ，转到use_jdk位置
echo Using JRE_HOME:        %JRE_HOME%     输出JRE_HOME变量值
goto java_dir_displayed      转到java_dir_displayed
:use_jdk
echo Using JAVA_HOME:       %JAVA_HOME%    输出JAVA_HOME变量值
:java_dir_displayed
                        下面几行设定相应变量
set _EXECJAVA=%_RUNJAVA%       
set MAINCLASS=org.apache.catalina.startup.Bootstrap
set ACTION=start
set SECURITY_POLICY_FILE=
set DEBUG_OPTS=
set JPDA=
if not ""%1"" == ""jpda"" goto noJpda
set JPDA=jpda
if not "%JPDA_TRANSPORT%" == "" goto gotJpdaTransport
set JPDA_TRANSPORT=dt_shmem
:gotJpdaTransport
if not "%JPDA_ADDRESS%" == "" goto gotJpdaAddress
set JPDA_ADDRESS=jdbconn
:gotJpdaAddress
if not "%JPDA_SUSPEND%" == "" goto gotJpdaSuspend
set JPDA_SUSPEND=n
:gotJpdaSuspend
if not "%JPDA_OPTS%" == "" goto gotJpdaOpts
set JPDA_OPTS=-Xdebug -Xrunjdwp:transport=%JPDA_TRANSPORT%,address=%JPDA_ADDRESS%,server=y,suspend=%JPDA_SUSPEND%
:gotJpdaOpts
shift
:noJpda
if ""%1"" == ""debug"" goto doDebug    如果%1为debug，转到doDebug，运行debug模式
if ""%1"" == ""run"" goto doRun        如果%1为run，转到doRun，运行正常模式
if ""%1"" == ""start"" goto doStart    如果%1为start，转到doStart，启动tomcat
if ""%1"" == ""stop"" goto doStop      如果%1为stop，转到doStop，关闭tocmat
if ""%1"" == ""version"" goto doVersion 如果%1为version，转到doVersion，显示tomcat的版本号
echo Usage:  catalina ( commands ... ) 如果%1没有上述内容，输出下面几行，并结束
echo commands:
echo   debug             Start Catalina in a debugger
echo   debug -security   Debug Catalina with a security manager
echo   jpda start        Start Catalina under JPDA debugger
echo   run               Start Catalina in the current window
echo   run -security     Start in the current window with security manager
echo   start             Start Catalina in a separate window
echo   start -security   Start in a separate window with security manager
echo   stop              Stop Catalina
echo   version           What version of tomcat are you running?
goto end
:doDebug
shift                    将%2里的值转到%1
set _EXECJAVA=%_RUNJDB%  将变量 _EXECJAVA设为_RUNJDB变量的值
set DEBUG_OPTS=-sourcepath "%CATALINA_HOME%/../../jakarta-tomcat-catalina/catalina/src/share" 
设定DEBUG_OPTS变量
if not ""%1"" == ""-security"" goto execCmd   
如果%1不为-security，转到execCmd位置
shift       将%2里的值转到%1
echo Using Security Manager       输出该行
set SECURITY_POLICY_FILE=%CATALINA_BASE%/conf/catalina.policy
设定SECURITY_POLICY_FILE变量的值
goto execCmd     转到execCmd位置
:doRun
shift        将%2里的值转到%1
if not ""%1"" == ""-security"" goto execCmd   如果%1不为-security，转到execCmd位置
shift        将%2里的值转到%1
echo Using Security Manager   输出该行
set SECURITY_POLICY_FILE=%CATALINA_BASE%/conf/catalina.policy
设定SECURITY_POLICY_FILE变量的值
goto execCmd  转到execCmd位置
:doStart
shift        将%2里的值转到%1
if not "%OS%" == "Windows_NT" goto noTitle  如果OS变量不为Windows_NT，转到noTitle
set _EXECJAVA=start "Tomcat" %_RUNJAVA%       设定_EXECJAVA变量的值
goto gotTitle      转到gotTitle位置
:noTitle
set _EXECJAVA=start %_RUNJAVA%    设定 _EXECJAVA 变量的值
:gotTitle              
if not ""%1"" == ""-security"" goto execCmd   如果%1不为-security，转到execCmd位置
shift                    将%2里的值转到%1
echo Using Security Manager        输出该行
set SECURITY_POLICY_FILE=%CATALINA_BASE%/conf/catalina.policy  
设定SECURITY_POLICY_FILE变量的值
goto execCmd       转到execCmd位置
:doStop
shift                 将%2里的值转到%1
set ACTION=stop       将ACTION的变量设为stop
set CATALINA_OPTS=    设CATALINA_OPTS为空
goto execCmd          转到execCmd位置
:doVersion            显示tomcat版本号
%_EXECJAVA% -classpath "%CATALINA_HOME%/server/lib/catalina.jar" org.apache.catalina.util.ServerInfo   执行该命令
goto end              结束该程序
:execCmd          
rem Get remaining unshifted command line arguments and save them in the
以下几行将命令参数存入CMD_LINE_ARGS变量中
set CMD_LINE_ARGS=
:setArgs
if ""%1""=="""" goto doneSetArgs
set CMD_LINE_ARGS=%CMD_LINE_ARGS% %1
shift
goto setArgs
:doneSetArgs
rem Execute Java with the applicable properties
if not "%JPDA%" == "" goto doJpda        如果JPDA变量不为空，转到doJpda位置
if not "%SECURITY_POLICY_FILE%" == "" goto doSecurity
如果SECURITY_POLICY_FILE变量不为空，转到doSecurity位置
 如果都没有执行下面命令，并结束该程序
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%"
 -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
goto end
:doSecurity        执行下面命令，并结束该程序
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Djava.security.manager -Djava.security.policy=="%SECURITY_POLICY_FILE%"
 -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%" -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
goto end
:doJpda  
如果SECURITY_POLICY_FILE变量不为空，转到doSecurityJpda位置，为空执行下面命令，并结束该程序  
if not "%SECURITY_POLICY_FILE%" == "" goto doSecurityJpda
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %JPDA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%"
 -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
goto end
:doSecurityJpda  执行下面命令，并结束该程序
%_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %JPDA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dirs="%JAVA_ENDORSED_DIRS%" -classpath "%CLASSPATH%" -Djava.security.manager -Djava.security.policy=="%SECURITY_POLICY_FILE%"
 -Dcatalina.base="%CATALINA_BASE%" -Dcatalina.home="%CATALINA_HOME%" -Djava.io.tmpdir="%CATALINA_TMPDIR%" %MAINCLASS% %CMD_LINE_ARGS% %ACTION%
goto end
:end
**终结：**
   catalina.bat咋一看还以为是什么windows程序，可以双击运行，实际上就是调用java命令运行Bootstrap类。从上面代码可以看出tomcat确实是一个纯java的程序，脚本最后都变成直接使用java命令执行程序，与我们普通写的java程序，没有什么不同。只不过由于 tomcat可以使用各种众多的模式（如debug,Security等），以及各种需要各种参数所以不得不使用脚本来执行。
   如果你想看看你到底使用了什么命令可以在“if not "%SECURITY_POLICY_FILE%" == "" goto doSecurity”这行下面添加两行。
       echo  %_EXECJAVA% %JAVA_OPTS% %CATALINA_OPTS% %DEBUG_OPTS% -Djava.endorsed.dir ....（下面那段，是一行但太长，折行了）
       pause
   第一行的命令是打印这具话，系统会将% %里面的变量提换成找到的值并输出。第二行是暂停程序，你可以通过任意键来恢复运行。
   下面是我的程序打印的结果：
start "Tomcat" "C:/Program Files/Java/jdk1.6.0_01/bin/java"  -Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager -Djava.util.logging.config.file="C:/work/tomcat/conf/logging.properties"
 -Xms128m -Xmx512m -Dfile.encoding=UTF8 -Duser.timezone=GMT -Djava.security.auth.login.config=C:/work/tomcat/conf/jaas.config   -Djava.endorsed.dirs="C:/work/tomcat/common/endorsed" -classpath "C:/Program Files/Java/jdk1.6.0_01/lib/tools.jar;C:/work/tomcat/bin/bootstrap.jar"
 -Dcatalina.base="C:/work/tomcat" -Dcatalina.home="C:/work/tomcat" -Djava.io.tmpdir="C:/work/tomcat/temp" org.apache.catalina.startup.Bootstrap  start
    start "tomcat"是另开一个窗口，窗口名是tomcat的意思，你可以去掉这部分，完将你程序打印的拷贝到命令行下，点回车，看是不是一样运行 tomcat了。Catalina.bat startup实际时就将各种系统变量加以总结，输出成这个命令。   呵呵!
