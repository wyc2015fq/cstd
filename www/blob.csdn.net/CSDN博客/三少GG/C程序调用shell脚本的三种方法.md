# C程序调用shell脚本的三种方法 - 三少GG - CSDN博客
2013年07月31日 17:42:42[三少GG](https://me.csdn.net/scut1135)阅读数：1245

C程序调用shell脚本共有三种法子 ：system()、popen()、exec系列函数call_exec1.c ，内容为：
system() 不用你自己去产生进程，它已经封装了，直接加入自己的命令
exec 需要你自己 fork 进程，然后exec 自己的命令
popen() 也可以实现执行你的命令，比system 开销小
1）system(shell命令或shell脚本路径);
   system() 会调用fork()产生 子历程，由子历程来调用/bin/sh-c string来履行 参数string字符串所代表的命令，此命令履行 完后随即返回原调用的历程。在调用system()期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被漠视 。
   返回值：如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-1。若参数string为空指针(NULL)，则返回非零值。 如果 system()调用成功 则最后会返回履行 shell命令后的返回值，但是此返回值也有可能为system()调用/bin/sh失败所返回的127，因 此最好能再反省 errno 来确认履行 成功 。
   system命令以其简略 高效的作用得到很很广泛的利用 ，下面是一个例子。
例：在/tmp/testDir/目录下有shell脚本abc.sh，内容为
```
#!/bin/bash
wget $1
echo "Done!"
```
在同层目录下新建一个c文件(以下代码 可带参数)
```
#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
int main(int argc ,char *argv[])
{
    char arg[300]="/tmp/testDir/abc.sh ";
    if ( argv[1] != NULL )
    {
        strcat(arg,argv[1]);
        
        system(arg);
        printf("\ndone message in program\n");
        return 1;
    }
    else
    {
        printf("Error: Empty input\n");
        return 0;
    }
    
}
```
执行效果 如下：
```
运行输出
[root@localhost testDir]#gcc call_exec1.c -o call_exec1
[root@localhost testDir]#./call_exec1 http://www.baidu.com/img/logo-yy.gif
--2011-01-21 17:02:22--  http://www.baidu.com/img/logo-yy.gif
正在解析主机 www.baidu.com... 61.135.169.105, 61.135.169.125
Connecting to www.baidu.com|61.135.169.105|:80... 已连接。
已发出 HTTP 请求，正在等待回应... 200 OK
长度：1618 (1.6K) [image/gif]
Saving to: `logo-yy.gif'
100%[======================================>] 1,618       --.-K/s   in 0.001s  
2011-01-21 17:02:34 (3.05 MB/s) - `logo-yy.gif' saved [1618/1618]
Done!
done message in program
```
2）popen(char *command,char *type)
    popen() 会调用fork()产生 子历程，然后从子历程中调用/bin/sh -c来履行 参数command的指令。参数type可应用 “r”代表读取，“w”代表写入。遵循此type值，popen()会建立 管道连到子历程的标准 输出设备 或标准 输入设备 ，然后返回一个文件指针。随后历程便可利用
 此文件指针来读取子历程的输出设备 或是写入到子历程的标准 输入设备 中。此外，所有应用 文 件指针(FILE*)操作的函数也都可以应用 ，除了fclose()以外。
    返回值：若成功 则返回文件指针，否则返回NULL，差错 原因存于errno中。注意：在编写具SUID/SGID权限的程序时请尽量避免应用 popen()，popen()会继承环境变量，通过环境变量可能会造成系统安全的问题。
例：C程序popentest.c内容如下：
```
#include<stdio.h>
    main
    {
        FILE * fp;
        charbuffer[80];
        fp=popen(“~/myprogram/test.sh”,”r”);
        fgets(buffer,sizeof(buffer),fp);
        printf(“%s”,buffer);
        pclose(fp);
    }
```
执行效果 如下：
```
运行输出
 #include<stdio.h>
    main
    {
        FILE * fp;
        charbuffer[80];
        fp=popen(“~/myprogram/test.sh”,”r”);
        fgets(buffer,sizeof(buffer),fp);
        printf(“%s”,buffer);
        pclose(fp);
    }
```
更好的例子：
[http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c)
|```#include <string>#include <iostream>#include <stdio.h>std::string exec(char* cmd) {    FILE* pipe = popen(cmd, "r");    if (!pipe) return "ERROR";    char buffer[128];    std::string result = "";    while(!feof(pipe)) {    	if(fgets(buffer, 128, pipe) != NULL)    		result += buffer;    }    pclose(pipe);    return result;}```Replace popen and pclose with _popen and _pclose for Windows.|[share](http://stackoverflow.com/a/478960)|[improve this answer](http://stackoverflow.com/posts/478960/edit)|answered Jan 26 '09 at 6:12[](http://stackoverflow.com/users/58008/waqas)![](https://www.gravatar.com/avatar/2daec0855071d1f016f9a3899139c8ac?s=32&d=identicon&r=PG)[waqas](http://stackoverflow.com/users/58008/waqas)2,262188||[share](http://stackoverflow.com/a/478960)|[improve this answer](http://stackoverflow.com/posts/478960/edit)|answered Jan 26 '09 at 6:12[](http://stackoverflow.com/users/58008/waqas)![](https://www.gravatar.com/avatar/2daec0855071d1f016f9a3899139c8ac?s=32&d=identicon&r=PG)[waqas](http://stackoverflow.com/users/58008/waqas)2,262188|
|----|----|----|
|[share](http://stackoverflow.com/a/478960)|[improve this answer](http://stackoverflow.com/posts/478960/edit)|answered Jan 26 '09 at 6:12[](http://stackoverflow.com/users/58008/waqas)![](https://www.gravatar.com/avatar/2daec0855071d1f016f9a3899139c8ac?s=32&d=identicon&r=PG)[waqas](http://stackoverflow.com/users/58008/waqas)2,262188| |
||| |
|||7|||7||Be aware that this will only grab stdout and not stderr. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)||----|----|----|----||7|| | |||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)||3|| | |||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)||1|| | |||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)| | |||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)||1|| | |||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)| | |||7|||7||Be aware that this will only grab stdout and not stderr. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||7|||7||Be aware that this will only grab stdout and not stderr. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)| | | | | | | | | | | | | | | | | |
|7|| | | | | | | | | | | | | | | | | | | |
||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)| | | | | | | | | | | | | | | | | |
|3|| | | | | | | | | | | | | | | | | | | |
||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)| | | | | | | | | | | | | | | | | |
|1|| | | | | | | | | | | | | | | | | | | |
||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)| | | | | | | | | | | | | | | | | | | |
||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)| | | | | | | | | | | | | | | | | |
|1|| | | | | | | | | | | | | | | | | | | |
||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)| | | | | | | | | | | | | | | | | | | |
|||7|||7||Be aware that this will only grab *stdout* and not *stderr*. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)||----|----|----|----||7|| | |||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)||3|| | |||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)||1|| | |||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)| | |||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)||1|| | |||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)| | |||7|||7||Be aware that this will only grab *stdout* and not *stderr*. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
||7|||7||Be aware that this will only grab *stdout* and not *stderr*. – [kalaxy](http://stackoverflow.com/users/188610/kalaxy)[Oct 31 '11 at 23:53](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment9732820_478960)| | | | | | | | | | | | | | | | | |
|7|| | | | | | | | | | | | | | | | | | | |
||3|||3||You can redirect stderr to stdout by appending "2>&1" to the command. Docs:[microsoft.com/resources/documentation/windows/xp/all/proddocs/…](http://www.microsoft.com/resources/documentation/windows/xp/all/proddocs/en-us/redirection.mspx?mfr=true) – [waqas](http://stackoverflow.com/users/58008/waqas)[Dec 17 '11 at 4:22](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment10582909_478960)| | | | | | | | | | | | | | | | | |
|3|| | | | | | | | | | | | | | | | | | | |
||1|||1||Also be aware that an exception can occur in ```result += buffer```, so the pipe might not be properly closed. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[May 19 '12 at 20:27](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment13840116_478960)| | | | | | | | | | | | | | | | | |
|1|| | | | | | | | | | | | | | | | | | | |
||@larsmans What kind of exceptions? – [Janis](http://stackoverflow.com/users/99904/janis)[Mar 14 at 16:05](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21795275_478960)| | | | | | | | | | | | | | | | | | | |
||1|||1||@Janis: `bad_alloc`. – [larsmans](http://stackoverflow.com/users/166749/larsmans)[Mar 14 at 22:00](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21807581_478960)| | | | | | | | | | | | | | | | | |
|1|| | | | | | | | | | | | | | | | | | | |
||@larsmans But it is because of a code error out of this function, right, not something that may be expected?– [Janis](http://stackoverflow.com/users/99904/janis)[Mar 15 at 11:23](http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c#comment21824782_478960)| | | | | | | | | | | | | | | | | | | |
