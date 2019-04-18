# exec命令详解与source区别 - nosmatch的专栏 - CSDN博客
2011年08月11日 15:33:59[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1078
                
 exec和source都属于bash内部命令（builtins commands），在bash下输入man exec或man source可以查看所有的内部命令信息。
bash shell的命令分为两类：外部命令和内部命令。外部命令是通过系统调用或独立的程序实现的，如sed、awk等等。内部命令是由特殊的文件格式（.def）所实现，如cd、history、exec等等。
在说明exec和source的区别之前，先说明一下fork的概念。
fork是linux的系统调用，用来创建子进程（child process）。子进程是父进程(parent process)的一个副本，从父进程那里获得一定的资源分配以及继承父进程的环境。子进程与父进程唯一不同的地方在于pid（process id）。
环境变量（传给子进程的变量，遗传性是本地变量和环境变量的根本区别）只能单向从父进程传给子进程。不管子进程的环境变量如何变化，都不会影响父进程的环境变量。
shell script:
有两种方法执行shell scripts，一种是新产生一个shell，然后执行相应的shell scripts；一种是在当前shell下执行，不再启用其他shell。
新产生一个shell然后再执行scripts的方法是在scripts文件开头加入以下语句
> #!/bin/sh
一般的script文件(.sh)即是这种用法。这种方法先启用新的sub-shell（新的子进程）,然后在其下执行命令。
另外一种方法就是上面说过的source命令，不再产生新的shell，而在当前shell下执行一切命令。
source:
source命令即点(.)命令。
在bash下输入man source，找到source命令解释处，可以看到解释"Read and execute commands from filename in the current shell environment and ..."。从中可以知道，source命令是在当前进程中执行参数文件中的各个命令，而不是另起子进程(或sub-shell)。source filename or .filename 执行filename中的命令。
exec:
在bash下输入man exec，找到exec命令解释处，可以看到有"No new process is created."这样的解释，这就是说exec命令不产生新的子进程。那么exec与source的区别是什么呢？
exec命令在执行时会把当前的shell process关闭，然后换到后面的命令继续执行。
此外。exec还能用于文件描述符和find命令！
            
