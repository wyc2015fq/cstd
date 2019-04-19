# python subprocess Popen (转) - 宇宙浪子的专栏 - CSDN博客
2015年01月05日 16:24:08[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1307
# [python subprocess Popen (转)](http://blog.csdn.net/sunnypotter/article/details/17248159)
- 01  subprocess.Popen(args  
- 02                  ,bufsize=0
- 03                  ,executable=None
- 04                  ,stdin=None
- 05                  ,stdout=None
- 06                  ,stderr=None
- 07                  ,preexec_fn=None
- 08                  ,close_fds=False
- 09                  ,shell=False
- 10                  ,cwd=None
- 11                  ,env=None
- 12                  ,universal_newlines=False
- 13                  ,startupinfo=None
- 14                  ,creationflags=0)  
- 参数args可以是字符串或者序列类型(如:list,元组),用于指定进程的可执行文件及其参数.如果是序列类型,第一个元素通常是可执行文 件的路径.我们也可以显式的使用executeable参数来指定可执行文件的路径.在windows操作系统上,Popen通过调用 CreateProcess()来创建子进程,CreateProcess接收一个字符串参数,如果args是序列类型,系统将会通过 list2cmdline()函数将序列类型转换为字符串。  
- 
- 参数bufsize:指定缓冲.我到现在还不清楚这个参数的具体含义,望各个大牛指点.  
- 
- 参数executable用于指定可执行程序.一般情况下我们通过args参数来设置所要运行的程序.如果将参数shell设为True,executable将指定程序使用的shell.在windows平台下,默认的shell由COMSPEC环境变量来指定.  
- 
- 参数stdin,stdout,stderr分别表示程序的标准输入、输出、错误句柄.他们可以是PIPE,文件描述符或文件对象,也可以设置为None,表示从父进程继承.  
- 
- 参数preexec_fn只在Unix平台下有效,用于指定一个可执行对象(callable object),它将在子进程运行之前被调用.  
- 
- 参数Close_sfs:在windows平台下,如果close_fds被设置为True,则新创建的子进程将不会继承父进程的输入、输出、 错误管 道.我们不能将close_fds设置为True同时重定向子进程的标准输入、输出与错误(stdin, stdout, stderr).  
- 
- 如果参数shell设为true,程序将通过shell来执行.  
- 
- 参数cwd用于设置子进程的当前目录.  
- 
- 参数env是字典类型,用于指定子进程的环境变量.如果env = None,子进程的环境变量将从父进程中继承.  
- 
- 参数Universal_newlines:不同操作系统下,文本的换行符是不一样的.如:windows下用'\r\n'表示换,而Linux下用'\n'.如果将此参数设置为True,Python统一把这些换行符当作'\n'来处理.  
- 
- 参数startupinfo与createionflags只在windows下用效,它们将被传递给底层的CreateProcess()函数,用于设置子进程的一些属性,如:主窗口的外观,进程的优先级等等.  
- 
- subprocess.PIPE   
- 在创建Popen对象时,subprocess.PIPE可以初始化stdin,stdout或stderr参数.表示与子进程通信的标准流.   
- 
- subprocess.STDOUT   
- 创建Popen对象时,用于初始化stderr参数,表示将错误通过标准输出流输出.   
- 
- Popen的方法:   
- Popen.poll()   
- 用于检查子进程是否已经结束.设置并返回returncode属性.  
- 
- Popen.wait()   
- 等待子进程结束.设置并返回returncode属性.   
- 
- Popen.communicate(input=None)   
- 与子进程进行交互.向stdin发送数据,或从stdout和stderr中读取数据.可选参数input指定发送到子进程的参 数.Communicate()返回一个元组:(stdoutdata, stderrdata).注意:如果希望通过进程的stdin向其发送数据,在创建Popen对象的时候,参数stdin必须被设置为PIPE.同样,如 果希望从stdout和stderr获取数据,必须将stdout和stderr设置为PIPE.   
- 
- Popen.send_signal(signal)   
- 向子进程发送信号。   
- 
- Popen.terminate()   
- 停止(stop)子进程.在windows平台下,该方法将调用Windows API TerminateProcess()来结束子进程.   
- 
- Popen.kill()   
- 杀死子进程.   
- 
- Popen.stdin   
- 如果在创建Popen对象是,参数stdin被设置为PIPE,Popen.stdin将返回一个文件对象用于策子进程发送指令.否则返回None.   
- 
- Popen.stdout   
- 如果在创建Popen对象是,参数stdout被设置为PIPE,Popen.stdout将返回一个文件对象用于策子进程发送指令.否则返回None.   
- 
- Popen.stderr   
- 如果在创建Popen对象是,参数stdout被设置为PIPE,Popen.stdout将返回一个文件对象用于策子进程发送指令.否则返回None.   
- 
- Popen.pid   
- 获取子进程的进程ID.   
- 
- Popen.returncode   
- 获取进程的返回值.如果进程还没有结束,返回None.  
另一个：[http://blog.csdn.net/menglei8625/article/details/7494094](http://blog.csdn.net/menglei8625/article/details/7494094)
### [Python_cmd的各种实现方法及优劣（subprocess.Popen, os.system和commands.getstatusoutput）](http://blog.csdn.net/menglei8625/article/details/7494094)
分类： [Python](http://blog.csdn.net/menglei8625/article/category/1105173)2012-04-24
 16:154611人阅读[评论](http://blog.csdn.net/menglei8625/article/details/7494094#comments)(0)收藏[举报](http://blog.csdn.net/menglei8625/article/details/7494094#report)
[cmd](http://www.csdn.net/tag/cmd)[python](http://www.csdn.net/tag/python)[windows](http://www.csdn.net/tag/windows)[shell](http://www.csdn.net/tag/shell)[subprocess](http://www.csdn.net/tag/subprocess)[signal](http://www.csdn.net/tag/signal)
目前我使用到的python中执行cmd的方式有三种：
**1. 使用os.system("cmd")**
这是最简单的一种方法，特点是执行的时候程序会打出cmd在linux上执行的信息。使用前需要import os。
**[python]**[view
 plain](http://blog.csdn.net/menglei8625/article/details/7494094#)[copy](http://blog.csdn.net/menglei8625/article/details/7494094#)
- os.system("ls")  
**2. 使用Popen模块产生新的process**
现在大部分人都喜欢使用Popen。Popen方法不会打印出cmd在linux上执行的信息。的确，Popen非常强大，支持多种参数和模式。使用前需要from subprocess import Popen, PIPE。但是Popen函数有一个缺陷，就是它是一个阻塞的方法。如果运行cmd时产生的内容非常多，函数非常容易阻塞住。解决办法是不使用wait()方法，但是也不能获得执行的返回值了。
Popen原型是：
**[python]**[view
 plain](http://blog.csdn.net/menglei8625/article/details/7494094#)[copy](http://blog.csdn.net/menglei8625/article/details/7494094#)
- subprocess.Popen(args, bufsize=0, executable=None, stdin=None, stdout=None, stderr=None, preexec_fn=None, close_fds=False, shell=False, cwd=None, env=None, universal_newlines=False, startupinfo=None, creationflags=0)  
参数bufsize：指定缓冲。我到现在还不清楚这个参数的具体含义，望各个大牛指点。
参数executable用于指定可执行程序。一般情况下我们通过args参数来设置所要运行的程序。如果将参数shell设为 True，executable将指定程序使用的shell。在windows平台下，默认的shell由COMSPEC环境变量来指定。
参数stdin, stdout, stderr分别表示程序的标准输入、输出、错误句柄。他们可以是PIPE，文件描述符或文件对象，也可以设置为None，表示从父进程继承。
参数preexec_fn只在Unix平台下有效，用于指定一个可执行对象（callable object），它将在子进程运行之前被调用。
参数Close_sfs：在windows平台下，如果close_fds被设置为True，则新创建的子进程将不会继承父进程的输入、输出、错误管 道。我们不能将close_fds设置为True同时重定向子进程的标准输入、输出与错误(stdin, stdout, stderr)。
如果参数shell设为true，程序将通过shell来执行。
参数cwd用于设置子进程的当前目录。
参数env是字典类型，用于指定子进程的环境变量。如果env = None，子进程的环境变量将从父进程中继承。
参数Universal_newlines:不同操作系统下，文本的换行符是不一样的。如：windows下用’/r/n’表示换，而Linux下用 ‘/n’。如果将此参数设置为True，Python统一把这些换行符当作’/n’来处理。
参数startupinfo与createionflags只在windows下用效，它们将被传递给底层的CreateProcess()函数，用 于设置子进程的一些属性，如：主窗口的外观，进程的优先级等等。
subprocess.PIPE
在创建Popen对象时，subprocess.PIPE可以初始化stdin, stdout或stderr参数，表示与子进程通信的标准流。
subprocess.STDOUT
创建Popen对象时，用于初始化stderr参数，表示将错误通过标准输出流输出。
Popen的方法：
Popen.poll() 
用于检查子进程是否已经结束。设置并返回returncode属性。
Popen.wait() 
等待子进程结束。设置并返回returncode属性。
Popen.communicate(input=None)
与子进程进行交互。向stdin发送数据，或从stdout和stderr中读取数据。可选参数input指定发送到子进程的参数。 Communicate()返回一个元组：(stdoutdata, stderrdata)。注意：如果希望通过进程的stdin向其发送数据，在创建Popen对象的时候，参数stdin必须被设置为PIPE。同样，如 果希望从stdout和stderr获取数据，必须将stdout和stderr设置为PIPE。
Popen.send_signal(signal) 
向子进程发送信号。
Popen.terminate()
停止(stop)子进程。在windows平台下，该方法将调用Windows API TerminateProcess（）来结束子进程。
Popen.kill()
杀死子进程。
Popen.stdin 
如果在创建Popen对象是，参数stdin被设置为PIPE，Popen.stdin将返回一个文件对象用于策子进程发送指令。否则返回None。
Popen.stdout 
如果在创建Popen对象是，参数stdout被设置为PIPE，Popen.stdout将返回一个文件对象用于策子进程发送指令。否则返回 None。
Popen.stderr 
如果在创建Popen对象是，参数stdout被设置为PIPE，Popen.stdout将返回一个文件对象用于策子进程发送指令。否则返回 None。
Popen.pid 
获取子进程的进程ID。
Popen.returncode 
获取进程的返回值。如果进程还没有结束，返回None。
例如：
**[python]**[view
 plain](http://blog.csdn.net/menglei8625/article/details/7494094#)[copy](http://blog.csdn.net/menglei8625/article/details/7494094#)
- p = Popen("cp -rf a/* b/", shell=True, stdout=PIPE, stderr=PIPE)  
- p.wait()  
- if p.returncode != 0:  
- print"Error."
- return -1
**3. 使用commands.getstatusoutput方法**
这个方法也不会打印出cmd在linux上执行的信息。这个方法唯一的优点是，它不是一个阻塞的方法。即没有Popen函数阻塞的问题。使用前需要import commands。
例如：
**[python]**[view
 plain](http://blog.csdn.net/menglei8625/article/details/7494094#)[copy](http://blog.csdn.net/menglei8625/article/details/7494094#)
- status, output = commands.getstatusoutput("ls")  
还有只获得output和status的方法：
**[python]**[view
 plain](http://blog.csdn.net/menglei8625/article/details/7494094#)[copy](http://blog.csdn.net/menglei8625/article/details/7494094#)
- commands.getoutput("ls")  
- commands.getstatus("ls")  
- 
再一：[http://blog.csdn.net/dbzhang800/article/details/6879239](http://blog.csdn.net/dbzhang800/article/details/6879239)
在熟悉了Qt的QProcess以后，再回头来看python的subprocess总算不觉得像以前那么恐怖了。
和QProcess一样，subprocess的目标是启动一个新的进程并与之进行通讯。
## subprocess.Popen
这个模块主要就提供一个类Popen：
class subprocess.Popen( args, 
      bufsize=0, 
      executable=None,
      stdin=None,
      stdout=None, 
      stderr=None, 
      preexec_fn=None, 
      close_fds=False, 
      shell=False, 
      cwd=None, 
      env=None, 
      universal_newlines=False, 
      startupinfo=None, 
      creationflags=0)
这堆东西真让人抓狂：
|args|字符串或者列表|
|----|----|
|bufsize|0 无缓冲1 行缓冲其他正值 缓冲区大小负值 采用默认系统缓冲(一般是全缓冲)|
|executable|一般不用吧，args字符串或列表第一项表示程序名|
|stdinstdoutstderr|None 没有任何重定向，继承父进程PIPE 创建管道文件对象文件描述符(整数)stderr 还可以设置为 STDOUT|
|preexec_fn|钩子函数， 在fork和exec之间执行。(unix)|
|close_fds|unix 下执行新进程前是否关闭0/1/2之外的文件windows下不继承还是继承父进程的文件描述符|
|shell|为真的话unix下相当于args前面添加了 "/bin/sh“ ”-c”window下，相当于添加"cmd.exe /c"|
|cwd|设置工作目录|
|env|设置环境变量|
|universal_newlines|各种换行符统一处理成 '\n'|
|startupinfo|window下传递给CreateProcess的结构体|
|creationflags|windows下，传递CREATE_NEW_CONSOLE创建自己的控制台窗口|
- 当初最感到困扰的就是 args 参数。可以是一个字符串，可以是一个列表。
subprocess.Popen(["gedit","abc.txt"])
subprocess.Popen("gedit abc.txt")
这两个之中，后者将不会工作。因为如果是一个字符串的话，必须是程序的路径才可以。(考虑unix的api函数 exec，接受的是字符串列表)
- 但是下面的可以工作
subprocess.Popen("gedit abc.txt", shell=True)
这是因为它相当于
subprocess.Popen(["/bin/sh", "-c", "gedit abc.txt"])
都成了sh的参数，就无所谓了
- 在Windows下，下面的却又是可以工作的
subprocess.Popen(["notepad.exe", "abc.txt"])
subprocess.Popen("notepad.exe abc.txt")
这是由于windows下的api函数CreateProcess接受的是一个字符串。即使是列表形式的参数，也需要先合并成字符串再传递给api函数。
- 类似上面
subprocess.Popen("notepad.exe abc.txt" shell=True)
等价于
subprocess.Popen("cmd.exe /C "+"notepad.exe abc.txt" shell=True)
## subprocess.call*
模块还提供了几个便利函数（这本身也算是很好的Popen的使用例子了）
- call() 执行程序，并等待它完成
def call(*popenargs, **kwargs):
    return Popen(*popenargs, **kwargs).wait()- check_call() 调用前面的call，如果返回值非零，则抛出异常
def check_call(*popenargs, **kwargs):
    retcode = call(*popenargs, **kwargs)
    if retcode:
        cmd = kwargs.get("args")
        raise CalledProcessError(retcode, cmd)
    return 0- check_output() 执行程序，并返回其标准输出
def check_output(*popenargs, **kwargs):
    process = Popen(*popenargs, stdout=PIPE, **kwargs)
    output, unused_err = process.communicate()
    retcode = process.poll()
    if retcode:
        cmd = kwargs.get("args")
        raise CalledProcessError(retcode, cmd, output=output)
    return output
## Popen对象
该对象提供有不少方法函数可用。而且前面已经用到了wait()/poll()/communicate()
|poll()|检查是否结束，设置返回值|
|----|----|
|wait()|等待结束，设置返回值|
|communicate()|参数是标准输入，返回标准输出和标准出错|
|send_signal()|发送信号 (主要在unix下有用)|
|terminate()|终止进程，unix对应的SIGTERM信号，windows下调用api函数TerminateProcess()|
|kill()|杀死进程(unix对应SIGKILL信号)，windows下同上|
|stdinstdoutstderr|参数中指定PIPE时，有用|
|pid|进程id|
|returncode|进程返回值|
## 参考
- 
[http://docs.python.org/library/subprocess.htcml](http://docs.python.org/library/subprocess.htcml)
- 
[http://www.python.org/dev/peps/pep-0324/](http://www.python.org/dev/peps/pep-0324/)
