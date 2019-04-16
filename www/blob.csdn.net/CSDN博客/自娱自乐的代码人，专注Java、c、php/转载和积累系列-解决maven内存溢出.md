# 转载和积累系列 - 解决maven内存溢出 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年04月29日 10:02:00[initphp](https://me.csdn.net/initphp)阅读数：709








找到%M2_HOME%\bin\mvn.bat


然后找到@REM set MAVEN_OPTS=-Xdebug -Xnoagent -Djava.compiler=NONE...

在下面加上：

set MAVEN_OPTS= -Xms128m -Xmx512m




