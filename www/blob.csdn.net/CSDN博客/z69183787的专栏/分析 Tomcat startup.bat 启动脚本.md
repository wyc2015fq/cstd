# 分析 Tomcat startup.bat 启动脚本 - z69183787的专栏 - CSDN博客
2014年11月08日 10:53:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：850
个人分类：[服务器-Tomcat](https://blog.csdn.net/z69183787/article/category/2175825)
       闲来无事，平常容器多使用tomcat，所以今天就想打开tomcat启动脚本看看。都说tomcat是纯Java的，这东西只有自己打开看看才能知道。呵呵！
       说来tomcat的脚本确实不难，启动脚本更是没有几行，以下是我解释的tomcat startup.bat脚本。
rem Guess CATALINA_HOME if not defined             确定该命令（脚本）是否在tomcat目录里
set CURRENT_DIR=%cd%                                          设定 CURRENT_DIR 为当前目录
if not "%CATALINA_HOME%" == "" goto gotHome   如果 CATALINA_HOME 不为空，跳转到 gotHome 位置
set CATALINA_HOME=%CURRENT_DIR%              如果为空，CATALINA_HOME设成 CURRENT_DIR（即当前目录）
if exist "%CATALINA_HOME%/bin/catalina.bat" goto okHome  如果存在catalina.bat, 就去 gotHome
cd ..                            否则返回上级目录  （你可以尝试把startup.bat拷贝到上级目录，它一样可以启动）
set CATALINA_HOME=%cd%            把CATALINA_HOME 重新设为当前目录。（应该是tomcat/）
cd %CURRENT_DIR%                       转到 CURRENT_DIR目录，即 tomcat/bin/下
:gotHome                                               gotHome 位置
if exist "%CATALINA_HOME%/bin/catalina.bat" goto okHome     如果存在catalina.bat，就去 okHome
echo The CATALINA_HOME environment variable is not defined correctly   否则输出下面两行，并终结
echo This environment variable is needed to run this program
goto end
:okHome
set EXECUTABLE=%CATALINA_HOME%/bin/catalina.bat     为EXECUTABLE变量设定值
rem Check that target executable exists         查询catalina.bat 命令是否存在
if exist "%EXECUTABLE%" goto okExec         查看 catalina.bat 是否存在，如果存在，就去okExec位置
echo Cannot find %EXECUTABLE%               否则输出下面两行，并终结
echo This file is needed to run this program
goto end
:okExec
rem Get remaining unshifted command line arguments and save them in the  将命令参数存入变量里
set CMD_LINE_ARGS=
:setArgs                                                       前面两行是注释，这里是setArgs 位置的返回点
if ""%1""=="""" goto doneSetArgs            如果没有参数，转到doneSetArgs位置    
set CMD_LINE_ARGS=%CMD_LINE_ARGS% %1        如果有参数，将参数放入CMD_LINE_ARGS里，加在CMD_LINE_ARGS变量的后面
shift                                                              把变量%2 里移到%1
goto setArgs                                               返回setArgs位置，进行循环
:doneSetArgs
call "%EXECUTABLE%" start %CMD_LINE_ARGS%       如果一切顺利调用 "%EXECUTABLE%" start 并加上 结尾参数
:end 
         从上文的分析中我们可以看见，startup.bat脚本实际上并没有做什么实际的工作，主要是查看当前命令所在目录。看是否在tomcat/或tomcat/bin/目录下。以及查看将调用该命令的参数传递给catalina.bat。最后调用catalina.bat start命令。与tomcat书中说startup.bat与catalina.bat
 start功能一样，相吻合。即startup.bat的主要作用是调用catalina.bat start命令。
          我之后，会在另一篇文章上对catalina.bat 脚本进行解析！
