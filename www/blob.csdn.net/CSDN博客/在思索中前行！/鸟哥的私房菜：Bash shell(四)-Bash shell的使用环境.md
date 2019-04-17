# 鸟哥的私房菜：Bash shell(四)-Bash shell的使用环境 - 在思索中前行！ - CSDN博客





2014年09月24日 20:44:24[_Tham](https://me.csdn.net/txl16211)阅读数：703








## [Bash shell(四)-Bash shell的使用环境](http://www.cnblogs.com/kkgreen/archive/2012/03/25/2416993.html)



　   是否记得我们登入主机的时候，屏幕上头会有一些说明文字，告知我们的 Linux 版本啊什么的， 还有，登入的时候，我们还可以给予使用者一些讯息或者欢迎文字呢。此外，
 我们习惯的环境变量、命令别名等等的，是否可以登入就主动的帮我设定好？ 这些都是需要来注意的。另外，这些设定值又可以分为系统整体设定值与各人喜好设定值， 仅是一些档案放置的地点不同啦！这我们后面也会来谈一谈的！



## 一  绝对路径与相对路径
这个议题说到快要烂掉了～从一开始到现在，这个绝对路径与相对路径的问题我们就提到不知道多少次了， 因为他实在很重要～这与 PATH 这个变量关系很大！老实说， 万一你的 PATH 没有设定完整的时候，下达指令就必须要以『 一长列的指令连带根目录都要列出来
 』，呵呵那就是绝对路径的设定法啦！ 
基本上，这个『 绝对路径』与『相对路径 』的观念是很重要的！否则你将常常会找不到档案说！ 所谓的『绝对路径』就是以根目录开始写入到档案的一种命令写定方法，举例来说，我目前在 /home/test 这个 test 使用者的家目录中，我想要看看里面的
 .bashrc 这个档案的数据，使用的是 more 这个指令，而这个指令在 /bin/more 当中，则正确的下达指令的方法为：
[root@linux ~]# /bin/more .bashrc

我在的目录为 /home/test ！这是绝对路径写法！ 而如果你还记得我们在 Linux 档案与目录管理 那一篇文章中提到的观念的话，那么应该记得使用 ls -al 时会出现两个一定存在的目录，分别是『.』与『..』，分别代表是『这个路径』，与『上一层路径』！
[root@linux ~]# ls -al
drwxrwxr-x 2 root root 4096 Aug 15 11:05 .
drwxrwxr-x 2 root root 4096 Aug 14 23:26 ..
所以说，要执行上一层目录中的命令，可以下达『../command 』那个 command 

指的是存在的可执行档！那么我因为在 /home/test 里面，距离 /bin 有两层上层目录，所以我要使用 /bin/more 这个执行档，并且使用相对路径的方法，就必须使用：
[root@linux ~]# /bin/more .bashrc

这种相对路径的方法相当广泛的被运用于 script 当中，这是因为如前面提到的， 每个人的安装预设的目录都不相同，则使用相对路径的话， 很容易就可以找到套件之间相依软件或者是设定档案的相关性！
     关于路径搜寻的问题！为何不执行目前所在目录下的档案？
　　咦！刚刚不是提到『.』与『..』吗？那么那个『 . 』是干嘛用的？！眼尖的朋友应该已经发现了，就是『我在执行档案的时候，基本上，并不会主动搜寻目前目录下的档案』举个例子来说，我安装的
 squid 这个执行档在 /usr/local/squid/bin/squid 这个档案，然而我在 /usr/local/squid/bin 下达 squid
 的时候，系统会告诉你『查不到这个档案！』真是见鬼了！明明有这个档案的呀！这是因为系统预设的 PATH (路径) 并没有执行目前目录下的设定，也就是『.』这个路径！你可以使用『
 echo $PATH 』看看，就可以知道为什么了！ 那么为何不要设定这个路径呢？这是因为『 安全』的考虑。由于系统预设是允许任何人在 /tmp 底下写入任何档案的，那么万一有居心不良的使用者或者是
 Cracker 入侵你的计算机，并在你的 /tmp 里头埋了一个小木马，并取名为 ls ，好了，改天你以 root 身份登入后，到 /tmp 底下，并执行 ls ，你看会有什么结果？！这个 /tmp/ls 由其它身份的人来执行或许没有问题，但是由
 root 来执行却可能会导致 Cracker 所乐意见到的结果！那晓得为何了吧？！
　　由于系统预设并不主动搜寻目前目录下的执行文件，那么你应该如何执行『目前目录下的执行文件』呢？ 很简单呀！就是以相对路径的观念，由于『 .. 』是上层，而『
 . 』是这一层，所以要执行这一层目录的命令就使用『 ./command 』即可！例如你的 /usr/local/squid/bin 底下执行 squid 则可以写成：
　　[root@linux ~]# ./squid







## 二  登录讯息显示数据： /etc/issue, /etc/motd
还记得我们在终端机接口 (tty1 ~ tty6) 登入的时候，会有几行提示的字符串吗？ 那个字符串写在哪里啊？呵呵！在 /etc/issue 里面啊！先来看看：
[root@linux ~]# cat /etc/issue
Fedora Core release 4 (Stentz)
Kernel \r on an \m
在 FC4 里面预设有三行，这个在我们本机登入时就会显示在 title 的地方呢～ 咦！那么那个 \r 及 \m 是啥？您可以使用 man issue 配合 man mingetty 
就能够知道：issue 内的各代码意义

\d 本地端时间的日期；

\l 显示第几个终端机接口；

\m 显示硬件的等级 (i386/i486/i586/i686...)；

\n 显示主机的网络名称；

\o 显示 domain name；

\r 操作系统的版本 (相当于 uname -r) 

\t 显示本地端时间的时间； 
\s 操作系统的名称； 

\v 操作系统的版本。

所以，如果您想要显示终端机的号码，就可以加上 \l 在 /etc/issue 档案内啰～就能够修改登入字符。 咦！但是还有个 /etc/issue.net 呢！这是啥？没
啥啦！这个是提供给 telnet 这个远程登入程序用的。 当我们使用 telnet 连接到主机时，主机的登入画面就会显示 /etc/issue.net 而不是 /etc/issue呢！

至于如果您想要让使用者登入后取得一些讯息，例如您想要让大家都知道的讯息， 那么可以将讯息加入 /etc/motd 里面去！例如：当登入后，告诉登入者，
 系统将会在某个固定时间进行维护工作，可以这样做：
[root@linux ~]# vi /etc/motd
Hello everyone,
Our server will be maintained at 2005/10/10 0:00 ~ 24:00.
Please don't login at that time. ^_^

那么当你的使用者登入主机后，就会显示这样的讯息出来：
Last login: Mon Aug 15 10:17:10 2005 from 127.0.0.1
Hello everyone,
Our server will be maintained at 2005/10/10 0:00 ~ 24:00.
Please don't login at that time. ^_^






## 三  环境设定档： 
bashrc, ~/.bashrc, ~/.profile, profile...,/etc/inputrc,source

关于取得 bash 的环境变量等数据，其实可以有系统规划与各人喜好， 一般来说，建议使用者直接修改个人设定值即可，不需要更动到系统啦～
 底下我们分别来谈一谈几个有趣的设定档喔！要注意的是，在指令列输入的变量也好、命令别名也罢，
 都是针对该次登入的设定而已，所以只要您一注销，那么上次的设定值就会不见去！ 因此，我们需要有几个档案来帮助我们，每次登入的时候，就已经帮我们搞定了环境的设定啰！

1) 系统设定值

所谓的系统设定值，也就是说每个使用者进入到 bash shell 之后，会先读取的设定档案！ 预设的设定档案有下列几个：

/etc/sysconfig/i18n
记得我们在几个重要变量内谈到的语系数据吗？！ 那个语系是由 i18n 所维护的，而 FC4 预设的系统语系设定文件就在 /etc/sysconfig/i18n 当中。 这个档案有点像这样：
[root@linux ~]# cat /etc/sysconfig/i18n
LANG="zh_TW.UTF-8"
SYSFONT="latarcyrheb-sun16"
SUPPORTED="zh_TW.UTF-8:zh_TW:zh:en_US.UTF-8"

我预设使用 zh_TW.UTF-8 来作为我的整体语系，当然，我可以在这里修改 LANG 以及其它相关的语系变量， 例如 LC_CTYPE 或者是 LC_TIME 等等的。不过，一般来说，使用者自己个人的设定不建议在这里做更动啦！ 他们可以自行设定他们自己的设定档啊！

/etc/profile
这个档案设定了几个重要的变量，例如：『PATH、USER、MAIL、 HOSTNAME、HISTSIZE、umask』等等，也同时规划出
 /etc/inputrc 这个针对键盘热建设定的档案的数据内容。你可以在这里设定总体的 PATH 等等的信息！ 同时，这个档案也规划出 /etc/profile.d 及 /etc/inputrc 这两个目录与档案！ 总之，你可以了解到刚刚我们学会的变量设定方式，在这个档案中也可以设定呢！
 但是设定上需要特别小心，因为所有的使用者皆会使用到这个档案的信息。通常我都喜欢将 /usr/local/bin 这个路径加成最前面，这是因为通常自己安装的套件自己最喜欢， 所以当然是最先搜寻啰！ ^_^！此外，请注意一下，可以将 HISTSIZE
 的大小改变一下，改成 50 就可以啦！比较安全！( 注：这个档案不论在那个 Linux distributions 当中均存在 /etc/profile 当中，所以，请特别留意此一档案即可！ )。

/etc/bashrc
这个档案在规划 umask 的功能，也同时规划出提示字符的内容 (就是里头那个 PS1 啦！) 。特别留意的是，这个档案在不同的 Linux distribution 里面摆放的位置可能不太一样呢！ 所以需要查询一下才行呦！

/etc/profile.d/*.sh
/etc/profile.d 是一个目录，里面针对 bash 及 C-shell 规范了一些数据。 

以 FC4 为例，这个目录里面就针对了颜色、语系、vim 及 which 等指令进行一些额外的设定， 例如 alias 之类的规范值。我们的 vim 被用 alias 命名为 vi 就是在这个目录下被设定好的。 

/etc/man.config
这个档案乍看之下好像跟 bash shell 没相关性，但是对于系统管理员来说， 却也是很重要的一个档案！这的档案的内容『规范了使用 man 的时候， man 
page 的路径到哪里去寻找！』所以说的简单一点，这个档案规定了下达 man 的时候，该去哪里查看数据的路径设定！
这就是系统在设定的时候常常会使用的档案！需要特别留意的是，通常设定完了这几个档案之后，都需要先 logout 在 login 之后才会将设定整个启动起来！


2) 个人设定值
那么个人的喜好设定在哪里？嘿嘿嘿嘿！那就是在个人家目录的几个隐藏文件当中啰！ 分别会使用到底下的几个档案啦！(
 注意！底下的档案都是隐藏档需要使用 ls -al 方能显示出来 ) ，另外，注意一下啰！底下那个『 ~ 』代表的是『家目录』的意思：
~/.bash_profile, ~/.bash_login, ~/.profile

这三个档案通常只要一个就够了，一般预设是以 ~/.bash_profile 的檔名存在。 会有这么多的档案，其实是因应其它 shell 转换过来的使用者的习惯而已
。 这个档案可以定义个人化的路径 (PATH) 与环境变量等等。不过，还是有顺位上的差异， bash 启动时，会先去读取 ~/.bash_profile，找不到时，就去读取 ~/.bash_login ，然后才是 ~/.profile。
~/.bashrc

鸟哥一般都是将自己的需要输入在这个档案里面的呢！ 我的个人化设定值都会写在这里说～例如命令别名、路径等等。 这个档案在您每次执行 shell script 的时候都会被重新使用一遍，所以是最完整的。 而上头的 ~/.bash_profile
 则只有在登入的时候会被读取一次。
~/.bash_history

还记得我们在历史命令提到过这个档案吧？！呵呵！没错～预设的情况下， 我们的历史命令就记录在这里啊！而这个档案能够记录几笔数据，则与 HISTSIZE 这个变数有关啊。每次登入
 bash 后，bash 会先读取这个档案，将所有的历史指令读入内存， 因此，当我们登入 bash 后就可以查知上次使用过哪些指令啰。至于更多的历史指令， 请自行回去参考喔！
~/.bash_logout

这个档案则记录了『当我注销 bash 后，系统再帮我做完什么动作后才离开』的意思。 你可以去读取一下这个档案的内容，预设的情况下，注销时， bash 
只是帮我们清掉屏幕的讯息而已。 不过，你也可以将一些备份或者是其它你认为重要的工作写在这个档案中（例如清空暂存盘）， 那么当你离开 Linux 时候，就可以解决一些烦人的事情啰！
好了，我们知道在变量的设定规范当中，后输入的设定值可以取代先输入的设定值， 那么在我们登入 bash 的时候，这些设定档到底是如何读取的呢？他是
这样读取的：
1. 先读取 /etc/profile ，再根据 /etc/profile 的内容去读取其它额外的设定档， 例如 /etc/profile.d 与 /etc/inputrc 等等设定档；
2. 根据不同的使用者，到使用者家目录去读取 ~/.bash_profile 或 ~/.bash_login 或 ~/.profile 等设定档；
3. 根据不同使用者，到他家目录去读取 ~/.bashrc 。


所以啰，当我登入 bash 后，最终读取的设定档竟然是 ~/.bashrc 呢！ 也就是说，在 ~/.bashrc 里面的设定会是最终的设定值！所以啰， 通常鸟哥我喜欢将个人的一些常用
 alias 或 PATH 等环境变量或自订变量都写到这个档案去， 如此一来，不论原来系统帮我们做了什么设定值，我都可以使用属于自己熟悉的环境呢！
 鸟哥的 ~/.bashrc 有点像这样：

[root@linux ~]# vi ~/.bashrc
# .bashrc
# Source global definitions
if [ -f /etc/bashrc ]; then
. /etc/bashrc
fi
# User specific aliases and functions
PATH="/bin:/sbin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin"
PATH="$PATH":/usr/X11R6/bin:/home/dmtsai/bin
LANG=zh_TW.big5
LC_TIME=C
export PATH LC_TIME LANG
umask 022
alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'
alias ll='ls -l'
alias lm='ls -al|more'
alias h='history'

仔细看到上头这个档案，会不会觉得奇怪啊！为什么会有第五行的『 . /etc/bashrc 』呢？ 那个小数点 (.) 代表什么意思啊？？其实 if [ ... ]; then .... fi 是 shell script 当中的程序写法， 这个我们会在下一章当中介绍。不过，那个
 . 则需要好好的谈一谈喔！一般来说，如果修改完了设定档， 通常就是 logout 后再重新 login 到 bash 内，就能够将环境设定档重读了！不过， 我们可以使用底下的方式来让该设定档立即生效：
[root@linux ~]# source file
范例：
[root@linux ~]# source ~/.bashrc
[root@linux ~]# . ~/.bashrc

利用 source 或小数点 (.) 都可以将设定档的内容读进来目前的 shell 环境中！
 举例来说，我修改了 ~/.bashrc ，那么不需要注销，立即以 source 
~/.bashrc 就可以将刚刚最新设定的内容读进来目前的环境中！很不错吧！此外，什么时候会使用到不同的设定档呢？ 最常发生在一个人的工作环境分为多重的时候了！举个例子来说，在我的大型主机中， 我常常需要负责两到三个不同的案子，每个案子所需要处理的环境变量订定并不相同，
 那么我就将这两三个案子分别编写属于该案子的环境变量设定档案，当我需要该环境时，就直接『 source 变量文件 』，如此一来，环境变量的设定就变的更简便而灵活了！





## 四  login shell 与 non-login shell
     事实上，这些环境设定档在读取时，还是有一些差异的，这就得要谈到所谓的『login shell』与 『non-login shell』的差异了。基本上，就字面上的意义来解释的话，所谓的 loign
 shell 指的就是当使用者登入 Linux 系统时，所取得的那个 shell 称为 login shell。 当登入后，再去执行其它的 shell 时，其它的
 shell 就是 non-login shell 了。举例来说，我以 dmtsai 这个使用者身份登入 Linux 后，然后为了要执行一些数值模拟的工作，而去执行 csh 这个 C shell ， 那么此时我就取得了 non-login shell 了。 另外一个例子是，当我以
 X Window 的环境登入 Linux 时，我们不是可以使用『终端机』来开启 shell 吗？当登入 Linux 的时候所取得的那个 X 的环境也可以读入 login shell 的。因此，在 X 环境下所启动的终端机，那些 shell 都是 non-login
 shell 喔！ login 与 non-login shell 的差异除了取得的时机不同之外，其实他们读取的环境设定档也不相同。 我们上头说过一些个人的环境设定档案了吧？那么这两种类型的 shell 该读取什么档案呢？ 当登入 Linux ，亦即是取得
 login shell 时，会读取 ~/.bash_profile, ~/.bash_login, ~/.profile， 这三个档案的优先级已经在上面提过，自行参考一下。至于在取得 login shell 后继续动作的其它 non-login shell ，读取的就是仅有 ~/.bashrc
 啰～。而大部分的 linux distributions 都会将 ~/.bash_profile 的内容指到 ~/.bashrc 去，这样比较简单啰








## 五  终端机的环境设定： stty, set
     什么叫做『终端机环境』啊？！我们在 首次登入 Linux 时就提过，可以在 tty1 ~ tty6 这六个文字接口的终端机 (terminal) 环境中登入，那么登入的
时候我们可以取得一些字符设定的功能喔！ 举例来说，我们可以利用退格键 (backspace，就是那个←符号的按键) 来删除命令列上的字符， 也可以使用[ctrl]+c 来强制终止一个指令的运行，当输入错误时，就会有声音跑出来警告。这是怎么办到的呢？
 很简单啊！因为登入终端机的时候，会自动的取得一些终端机的输入环境的设定啊！事实上，目前我们使用的 Linux distributions 都帮我们作了最棒的使用者环境了， 所以大家可以不用担心操作环境的问题。不过，在某些 Unix like
 的机器中，还是可能需要动用一些手脚， 才能够让我们的输入比较快乐～举例来说，利用 [backspace] 删除，要比利用 [Del] 按键来的顺手吧！ 但是某些 Unix 偏偏是以 [del] 来进行字符的删除啊！所以，这个时候就可以动动手脚啰～
 那么如何查阅目前的一些按键内容呢？可以利用 stty (setting tty 终端机的意思) 呢！ stty 也可以帮助设定终端机的输入按键代表意义喔！
[root@linux ~]# stty [-a]
参数：
-a ：将目前所有的 stty 参数列出来；
范例：
范例一：列出所有的按键与按键内容

[root@linux ~]# stty -a

speed 38400 baud; rows 40; columns 80; line = 0;
intr = ^C; quit = ^\; erase = ^?; kill = ^U; eof = ^D; eol = ;
eol2 = ; start = ^Q; stop = ^S; susp = ^Z; rprnt = ^R; werase
= ^W; lnext = ^V; flush = ^O; min = 1; time = 0;
-parenb -parodd cs8 -hupcl -cstopb cread -clocal -crtscts
-ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -igncr icrnl
ixon -ixoff -iuclc -ixany -imaxbel opost -olcuc -ocrnl onlcr -onocr
-onlret -ofill -ofdel nl0 cr0 tab0 bs0 vt0 ff0 isig icanon iexten
echo echoe echok -echonl -noflsh -xcase -tostop -echoprt echoctl echoke

我们可以利用 stty -a 来列出目前环境中所有的按键列表，在上头的列表当中，需要注意的是特殊字体那几个， 此外，如果出现 ^ 表示 [Ctrl] 那个按键
的意思。举例来说， intr = ^C 表示利用 [ctrl] + c 来达成的。 几个重要的代表意义是：
eof : End of file 的意思，代表『结束输入』。
erase : 向后删除字符，
intr : 送出一个 interrupt (中断) 的讯号给目前正在 run 的程序；
kill : 删除在目前指令列上的所有文字；
quit : 送出一个 quit 的讯号给目前正在 run 的程序；
start : 在某个程序停止后，重新启动他的 output
stop : 停止目前屏幕的输出；
susp : 送出一个 terminal stop 的讯号给正在 run 的程序。

记不记得我们讲过 Linux 底下的几个热键 啊？没错！ 就是这个 stty 设定值内的 intr / eof 啰～至于删除字符，就是 erase 那个设定值啦！ 如果你想
要用 [ctrl]+h 来进行字符的删除，那么可以下达：
[root@linux ~]# stty erase ^h

那么从此之后，你的删除字符就得要使用 [ctrl]+h 啰，按下 [backspace] 则会出现 ^? 字样呢！ 如果想要回复利用 [backspace] ，就下达 stty erase 
^? 即可啊！ 至于更多的 stty 说明，记得参考一下 man stty 的内容喔！除了 stty 之外，其实我们的 bash 还有自己的一些终端机设定值呢！那就是
利用 set 来设定的！ 我们之前提到一些变量时，可以利用 set 来显示，除此之外，其实 set 还可以帮我们设定整个指令输出/输入的环境。 例如记录历史
命令、显示错误内容等等。
[root@linux ~]# set [-uvCHhmBx]
参数：
-u ：预设不启用。若启用后，当使用未设定变量时，会显示错误讯息；
-v ：预设不启用。若启用后，在讯息被输出前，会先显示讯息的原始内容；
-x ：预设不启用。若启用后，在指令被执行前，会显示指令内容(前面有 ++ 符号)
-h ：预设启用。与历史命令有关(下节介绍)；
-H ：预设启用。与历史命令有关(下节介绍)；
-m ：预设启用。与工作管理有关(未来介绍)；
-B ：预设启用。与刮号 [] 的作用有关；
-C ：预设不启用。若使用 > 等，则若档案存在时，该档案不会被覆盖。

范例一：显示目前所有的 set 设定值
[root@linux ~]# echo $-
　　himBH
# 那个 $- 变量内容就是 set 的所有设定啦！ bash 预设是 himBH 喔！

范例二：设定 "若使用未定义变量时，则显示错误讯息"
[root@linux ~]# set -u
[root@linux ~]# echo $vbirding
-bash: vbirding: unbound variable
# 预设情况下，未设定/未宣告 的变量都会是『空的』，不过，若设定 -u 参数，
# 那么当使用未设定的变量时，就会有问题啦！很多的 shell 都预设启用 -u 参数。
# 若要取消这个参数，输入 set +u 即可！

范例三：执行前，显示该指令内容。
[root@linux ~]# set -x
[root@linux ~]# echo $HOME
+ echo /root
/root
++ echo -ne '\033]0;root@linux:~\007'
# 看见否？要输出的指令都会先被打印到屏幕上喔！前面会多出 + 的符号！另外，其实我们还有其它的按键设定功能呢！就是在 /etc/inputrc 这个档案里面设定。

[root@linux ~]# cat /etc/inputrc
# do not bell on tab-completion
#set bell-style none
set meta-flag on
set input-meta on
set convert-meta off
set output-meta on
.....以下省略.....


还有例如 /etc/DIR_COLORS* 与 /etc/termcap 等，也都是与终端机有关的环境设定档案呢！ 不过，事实上，鸟哥并不建议您修改 tty 的环境呢，这是因为 bash 的环境已经设定的很亲和了， 我们不需要额外的设定或者修改，否则反而会产生一些困扰。不过，写在这里的数据，
 只是希望大家能够清楚的知道我们的终端机是如何进行设定的。






## 六   万用字符与特殊符号：
嘿嘿！在 bash 里头还支持一些万用字符喔 (wild card) ！多了这些万用字符， 我们利用 bash 处理数据就更方便了！底下我们列出一些常用的


万用字符



![](http://pic002.cnblogs.com/images/2012/282432/2012032520580714.jpg)

组合按键执行结果
Ctrl + C
终止目前的命令
Ctrl + D
输入结束（EOF），例如邮件结束的时候；
Ctrl + M
就是 Enter 啦！
Ctrl + S
暂停屏幕的输出
Ctrl + Q
恢复屏幕的输出
Ctrl + U
在提示字符下，将整列命令删除
Ctrl + Z
『暂停』目前的命令

在上面的『按键组合』当中，有没有发现跟上个小节很相似的内容啊！？ 呵呵～没错啦！那些组合键都可以在 stty 当中来进行不同的设定的！好玩吧！ 至
于上面的万用字符当中，最常用的就属 *, ?, [] 及 ` 了！我们提几个简单的例子：
[root@linux ~]# ls test* <==那个 * 代表后面不论接几个字符都予以接受
[root@linux ~]# ls test? <==那个 ? 代表后面『一定』要接『一个』字符
[root@linux ~]# ls test??? <==那个 ??? 代表『一定要接三个』字符！
[root@linux ~]# cp test[1-5] /tmp
# 将 test1, test2, test3, test4, test5 若存在的话，就拷贝到 /tmp
[root@linux ~]# cp test[!1-5] /tmp
# 只要不是 test1, test2, test3, test4, test5 之外的其它 test? ，
# 若存在的话，就拷贝到 /tmp
[root@linux ~]# cd /lib/modules/`uname -r`/kernel/drivers
# 被 ` ` 括起来的内容『会先执行』

上面几个例子相当的有趣！尤其是最后面两个！需要注意的是， [1-5] 里面『代表只有一个字符』但是范围可以由 1-5 ，这样来说的话，那么我们如果允许『只要档名里面含有至少一个大写字符』时，就可以将档案 copy 出来的话，可以这样做：
cp *[A-Z]* /tmp

很有趣吧？！也就是说『 [ ] 谨代表一个字符，而这个字符的定义可以是范围(-)， 可以是指定项目，也可以是两者并存。 』举例来说，我想要找出在 
/etc/ 底下所有含有数字的档案， 可以这样：
ls -lda /etc/*[0-9]*
但如果我只想要找出含有 3 及 5 的档名的档案呢？就会是这样：
ls -lda /etc/*[35]*

如果是『不想要』某些范围或者是单字呢？就使用 [!] 即可！例如不想要有小写字符为开头的档案：
ls -lda /etc/[!a-z]*

很好玩吧！至于那个 ` 是啥？在一串指令当中， `command` 内的指令会先被执行， 执行完的讯息再回传到外部指令来处理！也就是说：
1. 系统先执行 uname -r 找出输出的结果；
2. 将结果累加在目录上面，来执行 cd 的功能！


很棒吧！！另外，这个 quot (`) 的功能，也可以利用 $() 来取代喔！例如：cd /lib/modules/$(uname -r)/kernel

更多的使用方式， 在后续的正规表示法当中详谈！






