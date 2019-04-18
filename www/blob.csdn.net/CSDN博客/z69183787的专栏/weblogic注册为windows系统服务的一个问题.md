# weblogic注册为windows系统服务的一个问题 - z69183787的专栏 - CSDN博客
2013年02月20日 10:26:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1352
  之前在win64系统下，想要将weblogic注册为系统服务，系统中的菜单采用js实现，其中有汉字编码采用的是UTF-8，注册成系统服务后，发现有UTF-8编码的那个模块显示不正常，但是从startweblogic.cmd启动，一切显示正常，所以怀疑是win系统服务默认的编码有关，所以在weblogic启动嗲用的脚本中设置weblogic采用的编码UTF-8，修改文件如下Middleware/user_projects/domains/SHEDW/bin的setDomainEnv.cmd中添加      
    
-Dfile.encoding=UTF-8(改成你用的相应编码)
上下文如下：
set JAVA_OPTIONS=%JAVA_OPTIONS% %JAVA_PROPERTIES% -Dwlw.iterativeDev=%iterativeDevFlag% -Dfile.encoding=UTF-8 -Dwlw.testConsole=%testConsoleFlag% -Dwlw.logErrorsToConsole=%logErrorsToConsoleFlag%
