# linux下运行matlab脚本方法 - qq_32515081的博客 - CSDN博客
2017年12月24日 20:11:27[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：3117
**​一般运行方法：**
**matlab -nodesktop nosplash **-r 'cd /home/teng/data;hellowrld;'****
**后台运行方法：**
**nohup matlab -nosplash -nodesktop -r 'cd /home/teng/data;helloworld;quit;'  >/dev/null  2>&1  &**
​-r为matlab命令，后面不可以直接跟脚本，‘ ’内容为**在matlab中实际运行的命令，且不能加.m后缀**。
cd 为根据脚本文件位置来切换matlab工作区的搜索路径；
nohup ……&为一程序组合，使程序后台运行，而不用使客户端进入到matlab命令行中；
quit为执行完命令直接
 我们还可以通过修改.bashrc文件来把常用命令简化缩短：
$ vim ~/.bashrc
添加如下：
# Add an "mrun" alias for running matlab in the terminal.
alias mrun="matlab -nodesktop -nosplash -logfile `date +%Y_%m_%d-%H_%M_%S`.log -r"
保存后，进入.m文件所在目录，运行
$ mrun 
matlabfile
如果使用 -nodisplay -r 选项运行，当程序中有figure()命令时会出错，可以使用如下选项抑制图形的显示：
`[figure](http://www.mathworks.com/access/helpdesk/help/techdoc/ref/figure.html)('visible','off');`
并使用如下命令将图形输出到文件：
`[print](http://www.mathworks.com/access/helpdesk/help/techdoc/ref/print.html)('-deps','example.eps');`
或者
`hgsave('example.fig');`
``
>/dev/null 2>&1  一般在shell包括dos下都可能会这样使用。
null是一个名叫null小桶的东西，将输出重定向到它的好处是不会因为输出的内容过多而导致文件大小不断的增加。其实，你就认为null就是什么都没有，也就是，将命令的输出扔弃掉了。
1表示标准输出，2表示标准错误输出，2>&1表示将标准错误输出重定向到标准输出，这样，程序或者命令的正常输出和错误输出就可以在标准输出输出。
一般来讲标准输出和标准错误输出都是屏幕，那为什么还要这么用呢？原因是标准输出的重定向。你的例子是重定向到了null，如果重定向到文件，例如：
dir > out.txt 
表示标准输出重定向到out.txt文件。此时如果dir命令出错，那么错误信息不会输出到out.txt文件，错误信息仍然会输出到屏幕——标准错误输出。为了使正确的信息和错误的信息都重定向到out.txt文件，那么需要将错误信息的标准错误输出重定向到标准输出。即命令如下：
dir > out.txt 2>&1
重定向到null是一个道理。
dir > null 2>&1
**0:表示键盘输入(stdin)1:表示标准输出(stdout),系统默认是1 2:表示错误输出(stderr)**
**command >/dev/null 2>&1 &  == command 1>/dev/null 2>&1 &**
**1)command:表示shell命令或者为一个可执行程序2)>:表示重定向到哪里 3)/dev/null:表示Linux的空设备文件 4)2:表示标准错误输出5)&1:****&表示等同于的意思,2>&1,表示2的输出重定向等于于1****6)&:表示后台执行,即这条指令执行在后台运行 **
**1>/dev/null:表示标准输出重定向到空设备文件,也就是不输出任何信息到终端,不显示任何信息。2>&1:表示标准错误输出重定向等同于标准输出,因为之前标准输出已经重定向到了空设备文件,所以标准错误输出也重定向到空设**备**文****件。**
**这条命令的意思就是在后台执行这个程序,并将错误输出2重定向到标准输出1,然后将标准输出1全部放到/dev/null文件,也就是清空.所以可以看出" >/dev/null 2>&1 "常用来避免shell命令或者程序等运行中有内容输出**
使用man bash命令查看到的联机帮助文件中的相关解释如下：
.bashrc - The individual per-interactive-shell startup file.
这个文件主要保存个人的一些个性化设置，如命令别名、路径等。下面是个例子：
 # User specific aliases and functions
 PATH="/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin"
 LANG=zh_CN.GBK
 export PATH LANG
 alias rm='rm -i'
 alias ls='/bin/ls -F --color=tty --show-control-chars'
例子中定义了路径，语言，命令别名（使用rm删除命令时总是加上-i参数需要用户确认，使用ls命令列出文件列表时加上颜色显示）。
每次修改.bashrc后，使用source ~/.bashrc（或者 . ~/.bashrc）就可以立刻加载修改后的设置，使之生效。
一般会在.bash_profile文件中显式调用.bashrc。登陆linux启动bash时首先会去读取~/.bash_profile文件，这样~/.bashrc也就得到执行了，你的个性化设置也就生效了。
