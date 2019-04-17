# 鸟哥的私房菜：Bash shell(二)-变量的丰富功能 - 在思索中前行！ - CSDN博客





2014年09月24日 20:32:08[_Tham](https://me.csdn.net/txl16211)阅读数：876










##       一  Shell变量的取用与设定，变量规则
　　由于系统需要一些变量来提供他数据的存取，所以就有一些所谓的『环境变量』 需要来读入系统中了！这些环境变量例如 PATH、HOME、MAIL、SHELL。为了区别与自订变量的不同，环境变量通常以大写字符来表示。 
　　利用 echo 这个指令来取用变量， 变量在被取用时，前面必须要加上 $ 才行：


[root@linux ~]# echo $variable
     [root@linux ~]# echo $PATH

　　/bin:/sbin:/usr/sbin:/usr/bin:/usr/local/sbin:/usr/local/bin:/usr/X11R6/bin
    [root@linux ~]# echo ${PATH}
    利用 ehco 就能够读出，只是需要在变量名称前面加上 $ ， 或者是以 ${variable} 的方式来取用！

    例题一：请在屏幕上面显示出您的环境变量 HOME 与 MAIL：
    echo $HOME echo $MAIL
    举例来说： 我要将 myname 这个变量名称的内容设定为 VBird ，那么：
    [root@linux ~]# echo $myname
<==这里并没有任何数据～因为这个变量尚未被设定！是空的！
    [root@linux ~]# myname=VBird
    [root@linux ~]# echo $myname
    VBird <==出现了！因为这个变量已经被设定了！
    当一个变量名称尚未被设定时，预设的内容是『空』的。 另外，变量在设定时，还是需要符合某些规定的:（非常重要的规则）

   1. 变量与变量内容以等号『=』来连结；
   2. 等号两边不能直接接空格符；
   3. 变量名称只能是英文字母与数字，但是数字不能是开头字符；
4. 若有空格符可以使用双引号『 " 』或单引号『 ' 』来将变量内容结合起来，但须要特别留意， 双引号内的特殊字符可以保有变量特性，但是单引号内的特殊字符则仅为一般字符；
   5. 必要时需要以跳脱字符『 \ 』来将特殊符号 ( 如 Enter, $, \, 空格符, ' 等 ) 变成一般符号；
   6. 在一串指令中，还需要藉由其它的指令提供的信息，可以使用 quote 『 ` command` 』；(特别特别注意，那个 ` 是键盘上方的数字键 1 左边那个按键，而不是单引号！)
   7. 若该变量为扩增变量内容时，则需以双引号及 $变量名称 如：『 "$PATH":/home』继续累加内容；
   8. 若该变量需要在其它子程序执行，则需要以 export 来使变量变成环境变量， 如『export PATH』；
   9. 通常大写字符为系统预设变量，自行设定变量可以使用小写字符，方便判断 ( 纯粹依照使用者兴趣与嗜好 ) ；
   10. 取消变量的方法为：『unset 变量名称』。

   范例一：设定一变量 name ，且内容为 VBird 。
[root@linux ~]# 12name=VBird
-bash: 12name=VBird: command not found <==屏幕会显示错误！因为不能以数字开头！
[root@linux ~]# name = VBird <==还是错误！因为有空白！
[root@linux ~]# name=VBird <==OK 的啦！
   范例二：承上题，若变量内容为 VBird's name 呢？
[root@linux ~]# name=VBird's name
# 因为单引号可以将 Enter 这个特殊字符取消，所以，您可以继续在下一行输入内容～
# 不过，这与我们要达到的功能不同，所以，算是失败的啦！
[root@linux ~]# name="VBird's name" <==OK 的啦！
[root@linux ~]# name=VBird\'s\ name
# 利用反斜线 (\) 跳脱特殊字符，例如单引号与空格键，这也是 OK 的啦！
   范例三：我要在 PATH 这个变量当中『累加』:/home/dmtsai/bin 这个目录
[root@linux ~]# PATH=$PATH:/home/dmtsai/bin
[root@linux ~]# PATH="$PATH":/home/dmtsai/bin
# 上面这两种格式在 PATH 里头的设定都是 OK 的！但是底下的例子就不见得啰！
   范例四：呈范例三，我要将 name 的内容多出 "yes" 呢？
[root@linux ~]# name=$nameyes
# 知道了吧？如果没有双引号，那么变量成了啥？name 的内容是 $nameyes 这个变量！
# 呵呵！我们可没有设定过 nameyes 这个变量吶！所以，应该是底下这样才对！
[root@linux ~]# name="$name"yes
[root@linux ~]# name=${name}yes
**   范例五：如何让我刚刚设定的 name=VBird 可以用在下个 shell 的程序？**
[root@linux ~]# name=VBird
[root@linux ~]# bash <==进入到所谓的子程序
[root@linux ~]# echo $name <==嘿嘿！并没有刚刚设定的内容喔！
[root@linux ~]# exit <==离开刚刚的子程序
[root@linux ~]# export name
[root@linux ~]# bash <==进入到所谓的子程序
[root@linux ~]# echo $name <==出现了设定值了！
[root@linux ~]# exit <==离开刚刚的子程序
# 什么是『子程序』呢？就是说，在我目前这个 shell 的情况下，
# 去启用另一个新的 shell ，新的那个 shell 就是子程序啦！在一般的状态下，
# 父程序的自订变量是无法在子程序内使用的。但是透过 export 将变量变成
# 环境变量后，就能够在子程序底下应用了！

**   范例六：如何进入到您目前核心的模块目录？**
[root@linux ~]# cd /lib/modules/`uname -r`/kernel
# 每个操作系统核心版本都不相同，以 FC4 为例，他的预设核心版本是
# 2.6.11-1.1369_FC4 所以，他的模块目录在 /lib/modules/2.6.11-1.1369_FC4/kernel 。
# 因为每个 distributions 的这个值都不相同，但是我们却可以利用 uname -r 这个指令
# 先取得版本信息，所以啰，就可以透过上面指令当中的内含指令 `uname -r`
# 先取得版本输出到 cd .. 那个指令当中，就能够顺利的进入目前核心的驱动程序所放置
# 的目录啰！很方便吧！

范例七：取消刚刚设定的 name 这个变量内容
[root@linux ~]# unset name
根据上面的案例你可以试试看！就可以了解变量的设定啰！这个是很重要的呦！请勤加练习！！ 其中，较为重要的一些特殊符号的使用啰！例如单引号、双引号、跳脱字符、钱字号、quote 符号等等，底下的例题想一想吧！

例题二：在变量的设定当中，单引号与双引号的用途有何不同？ 
单引号与双引号的最大不同在于双引号仍然可以保有变量的内容，但单引号内仅能是一般字符 ，而不会有特殊符号。我们以底下的例子做说明：假设您定义了一个变量， name=VBird ，现在想以 name 这个变量的内容定义出 myname 显示 VBird its me 这个内容，要如何订定呢？
[root@linux ~]# name=VBird 
[root@linux ~]# echo $name 
       　　　　　　 VBird 
[root@linux ~]# myname="$name its me" 
[root@linux ~]# echo $myname 
        　　　　　　VBird its me 
[root@linux ~]# myname='$name its me' 
[root@linux ~]# echo $myname 
        　　　　　　$name its me
使用了单引号的时候，那么 $name 将失去原有的变量内容， 仅为一般字符的显示型态而已！这里必需要特别小心在意！

例题三：在指令下达的过程中， quote ( ` ) 这个符号代表的意义为何？
在一串指令中，在 ` 之内的指令将会被先执行，而其执行出来的结果将做为外部的输入信息！例如 uname -r 会显示出目前的核心版本，，你可以先执行 uname -r 找出核心版本，然后再以『 cd 目录』到该目录下，当然也可以执行如同上面范例六的执行内容啰。 
另外再举个例子，我们也知道， locate 指令可以列出所有的相关档案档名，但是， 如果我想要知道各个档案的权限呢？举例来说，我想要知道每个 crontab 相关档名的权限：
[root@linux ~]# ls -l `locate crontab`
如此一来，先以 locate 将文件名数据都列出来，再以 ls 指令来处理的意思啦！





## 二    环境变量的功能，一些重要的环境变量，set，export
环境变量可以帮我们达到很多功能～包括家目录的变换啊、提示字符的显示啊、执行文件搜寻的路径啊等等的， 目前我的 shell 环境中， 有多少变量啊？我们可以利用两个指令来查阅，分别是 env 与 export 

1) 一些环境变量的说明： env

范例一：列出目前的 shell 环境下的所有环境变量与其内容。

[root@linux ~]# env
HOSTNAME=linux.dmtsai.tw <== 这部主机的主机名称
SHELL=/bin/bash <== 目前这个环境下，使用的 Shell 是哪一个程序？
TERM=xterm <== 这个终端机使用的环境是什么类型
HISTSIZE=1000 <== 这个就是『记录指令的笔数』在 FC4 预设可记录 1000 笔

USER=root <== 使用者的名称啊！



LS_COLORS=no=00:fi=00:di=00;34:ln=00;36:pi=40;33:so=00;35:bd=40;33;01:cd=40;33;01:or=01;05;37;41:mi=01;05;37;41:ex=00;32:*.cmd=00;32:*.exe=00;32:*.com=00;32:*.btm=00;32:*.bat=00;32:*.sh=00;32:*.csh=00;32:*.tar=00;31:*.tgz=00;31:*.arj=00;31:*.taz=00;31:*.lzh=00;31:*.zip=00;31:*.z=00;31:*.Z=00;31:*.gz=00;31:*.bz2=00;31:*.bz=00;3

1:*.tz=00;31:*.rpm=00;31:*.cpio=00;31:*.jpg=00;35:*.gif=00;35:*.bmp=00;35:*.xbm=00;35:*.xpm=00;35:*.png=00;35:*.tif=00;35: <== 一些颜色显示



ENV=/root/.bashrc <== 使用的个人环境设定档
MAIL=/var/spool/mail/root <== 这个使用者所取用的 mailbox 位置
PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/X11R6/bin:/usr/local/bin:/usr

/local/sbin:
/root/bin <== 不再多讲啊！是执行文件指令搜寻路径
INPUTRC=/etc/inputrc <== 与键盘按键功能有关。可以设定特殊按键！
PWD=/root <== 目前使用者所在的工作目录 (利用 pwd 取出！)
LANG=en_US.UTF-8 <== 这个与语系有关，底下会再介绍！
HOME=/root <== 这个使用者的家目录啊！
_=/bin/env <== 上一次使用的指令的最后一个参数(或指令本身)


    env 是 environment (环境) 的简写啊～ 上面的例子当中，是列出来所有的环境变量。当然，如果使用 export 也会是一样的内容～ 只不过， export 还有其它额外的功能就是了，我们等一下再提这个 export 指令。 那么上面这些变量有些什么功用呢？底下我们就一个一个来分析分析
    HOME ： 代表使用者的家目录。还记得我们可以使用 cd ~ 去到使用者的家目录吗？或者利用 cd 就可以直接回到使用者家目录了。 有很多程序都可能会取用到这个变量的值喔！
SHELL ： 告知我们，目前这个环境使用的 SHELL 是哪支程序？ 如果是 bash 的话，预设是 /bin/bash 的啦！
 HISTSIZE ： 这个与『历史命令』有关，亦即是， 我们曾经下达过的指令可以被系统记录下来，而记录的『笔数』则是由这个值来设定的。
 ENV ： 这个使用者所使用的个人化环境设定档的读取档案。
  MAIL ： 当我们使用 mail 这个指令在收信时，系统会去读取的邮件信箱档案 (mailbox)。
 PATH ： 就是执行文件搜寻的路径啦～目录与目录中间以冒号(:)分隔， 由于档案的搜寻是依序由 PATH 的变量内的目录来查询，所以，目录的顺序也是重要的喔。
 LANG ： 这个重要！就是语系档案啰～很多数据都会用到他
RANDOM ： 这个玩意儿就是『随机随机数』的变量啦！ 我们可以透过这个随机数档案相关的变量 ($RANDOM)来随机取得随机数值喔。

   2) 其它所有的变量说明： set

   而除了这些环境变量之外，还有没有什么重要的变量呢？我们在 bash 的环境下，其实还有一些挺重要的变量，这些变量是『在这个 shell 环境下有效』的， 如果是在『子程序』，这些变量值就不会相同了。 set 这个指令除了会将环境变量列出来之外，其它我们的自订变量，与所有的变量，都会被列出来:

[root@linux ~]# set

BASH=/bin/bash <== bash 的主程序放置路径
BASH_VERSINFO=([0]="3" [1]="00" [2]="16" [3]="1" [4]="release"
[5]="i386-redhat-linux-gnu") <== bash 的版本啊！
BASH_VERSION='3.00.16(1)-release' <== bash 的版本啊！
COLORS=/etc/DIR_COLORS.xterm <== 使用的颜色纪录档案
COLUMNS=115 <== 在目前的终端机环境下，使用的字段有几个字符长度
HISTFILE=/root/.bash_history <== 历史命令记录的放置档案，隐藏档
HISTFILESIZE=1000 <== 存起来(与上个变量有关)的档案之指令的最大纪录笔数。
HISTSIZE=1000 <== 目前环境下，可记录的历史命令最大笔数。
HOSTTYPE=i386 <== 主机安装的软件主要类型。我们用的是 i386 兼容机器软件
IFS=$' \t\n' <== 预设的分隔符
LINES=35 <== 目前的终端机下的最大行数
MACHTYPE=i386-redhat-linux-gnu <== 安装的机器类型
MAILCHECK=60 <== 与邮件有关。每 60 秒去扫瞄一次信箱有无新信！
OLDPWD=/home <== 上个工作目录。我们可以用 cd - 来取用这个变量。
OSTYPE=linux-gnu <== 操作系统的类型！
PPID=20046 <== 父程序的 PID (会在后续章节才介绍)
PROMPT_COMMAND='echo -ne "\033]0;${USER}@${HOSTNAME%%.*}:${PWD/#$HOME/~}\007"'
<== 上面这个是命令提示字符！与底下也有关。
PS1='[\u@\h \W]\$ ' <== PS1 就厉害了。这个是命令提示字符，也就是我们常见的
[root@linux ~]# 或 [dmtsai ~]$ 的设定值啦！可以更动的！
RANDOM=13586 <== 随机数啊！上面已经提过啰～
SUPPORTED=zh_TW.UTF-8:zh_TW:zh:en_US.UTF-8 <== 本系统所支持的语系
name=VBird <== 刚刚设定的自订变量也可以被列出来喔！
$ <== 目前这个 shell 所使用的 PID
? <== 刚刚执行完指令的回传值。

    一般来说，不论是否为环境变量，只要跟我们目前这个 shell 的操作接口有关的变量， 通常都会被设定为大写字符，也就是说，『基本上，在 Linux 预设的情况中，使用{大写的字母}来设定的变量一般为系统内定需要的变量』。 使用 set 除了会将系统的默认值秀出来之外，连带的所有的你自己设定的变量也会被秀出来！ 同时需要注意的是，若当时有相当多人同时在在线的话，那么 你的变量只能给自己使用 ( 除非改的是系统的预设参数档，如 /etc/profile
 )，而不会干扰到别人的！就如同前面所说的， 由于你登入 Linux 之后会取得一个 PID ，而你的设定将只对这个 PID 与子程序有关！此外， 这次登入所进行的变量设定，如果没有更动到设定档， 那么这次设定的变量在下次登入时将被取消掉 ( 因为程序 PID 不见啰！ ) ！所以啰， 如果你想要你的变量每次都能在你登入的时候自动就设定好了，那么就必须将你的设定写入登入时加载的设定档！ ( 更多的程序相关的说明，不要急～我们会在后面的 程序与资源管理 当中好好的提一提的！ ) OK！OK！那么上头那些变量当中，有哪些是比较重要：

PS1：(提示字符的设定) 

　这个东西就是我们的『命令提示字符』啊！ 当我们每次按下 [Enter] 按键去执行某个指令后，最后要再次出现提示字符时， 就会主动去读取这个变数值了。上头 PS1 内显示的是一些特殊符号，每个版本 bash 的 PSI 变量内的特殊符号可能有些许的差异， 你应该主动的以 man bash 去查询一下相关的变数。底下我列出 FC4 的环境下， 预设的bash 的 PS1 变量内的特殊符号代表意义：

o \d ：代表日期，格式为 Weekday Month Date，例如 "Mon Aug 1"
o \H ：完整的主机名称。举例来说，鸟哥的练习机 linux.dmtsai.tw ，那么这个主机名称就是 linux.dmtsai.tw
o \h ：仅取主机名称的第一个名字。以上述来讲，就是 linux 而已， .dmtsai.tw 被省略。
o \t ：显示时间，为 24 小时格式，如： HH:MM:SS
o \T ：显示时间，12 小时的时间格式！
o \A ：显示时间，24 小时格式， HH:MM
o \u ：目前使用者的账号名称；
o \v ：BASH 的版本信息；
o \w ：完整的工作目录名称。家目录会以 ~ 取代；
o \W ：利用 basename 取得工作目录名称，所以仅会列出最后一个目录名。
o \# ：下达的第几个指令。
o \$ ：提示字符，如果是 root 时，提示字符为 # ，否则就是 $ 啰～

OK！所以，由预设的 PS1 内容为： '\[\u@\h \W\]\$ ' 就可以了解为何我们的提示字符会是： [root@linux ~]# 了吧！好了，那么假设我想要有类似底下的提示字符：

　　[root@linux /home/dmtsai 16:50 #12]#
        那个 # 代表第 12 次下达的指令。 那么应该如何设定 PS1 呢？可以这样啊：

[root@linux home]# PS1='[\u@\h \w \A #\#]\$ '
[root@linux /home 17:02 #85]#

# 看到了吗？提示字符变了！变的很有趣吧！其中，那个 #85 比较有趣，
# 如果您按下 [Enter] 后，该数字就会增加喔！为啥？上面有说明

$：(关于本 shell 的 PID) 
　其实这个咚咚代表的是『目前这个 Shell 的执行绪代号』，亦即是所谓的 PID (Process ID)。 更多的程序观念，我们会在第四章的时候提及。想要知道我们的 shell 的 PID ，就可以： echo $$ 即可！

?：(关于上个执行指令的回传码) 
　问号也是一个特殊的变数？没错！在 bash 里面这个变量可重要的很！ 这个变数是：『上个执行的指令所回传的值』， 上面这句话的重点是『上一个指令』与『回传值』两个地方。当我们执行某些指令时， 这些指令都会回传一个执行后的代码。一般来说，如果成功的执行该指令，
 则会回传一个 0 值，如果执行过程发生错误，就会回传『错误代码』才对！一般就是以非为 0 的数值来取代。 我们以底下的例子来看看：
[root@linux ~]# echo $SHELL
/bin/bash
[root@linux ~]# echo $?
0
# 因为上个指令执行过程中，并没有错误，为成功的执行完毕，所以回传 0 。
[root@linux ~]# 12name=VBird
-bash: 12name=VBird: command not found
[root@linux ~]# echo $?
127
# 发生错误啦！所以 echo $? 时，就会出现错误的代码！
# 我们可以利用这个代码来搜寻错误的原因喔！
[root@linux ~]# echo $?
0
# 咦！怎么又变成正确了？这是因为 "?" 只与『上一个执行指令』有关，
# 所以，我们上一个指令是执行『 echo $? 』，当然没有错误，所以是 0 没错！

OSTYPE, HOSTTYPE, MACHTYPE：(主机硬件与核心的等级)

　这几个东西与程序的安装有关。我们在『Linux 主机规划』 里面提到过关于主机的等级方面的问题，当我们在安装软件的时候， 需要透过编译器来将原始码编译成为二进制的档案 (binary file)。但是， 我们可以针对硬件的配备来进行编译的最佳化，此时，这些参数就可以被用到了！ 基本上，目前主要的 distribution 都是针对 i386 亦即最低等级的机器进行最佳化， 这样才能够安装在较高阶的机器上，如果以 686 的机型来最佳化，
 那么，可就无法向下兼容的喔！(早期的 OpenLinux 是针对 686 机器来释出软件， 所以，当时的 OpenLinux 是无法安装在 P-166 的机器上的。 )

3) 自订变量转成环境变量： export

好了，上面我们环境变量也提过了，一些自订变量也提过了，那么，这两者有啥不同？ 他的不同处，我们在 变量设定规则 当中稍微提过， 主要是由于变量可否被子程序所引用。 当你取得一个 bash 之后，亦即得到了一个程序了，但是若你再次的执行一次 bash ，那么你将进入『子程序』，这个程序的概念我们在资源管理章节中再详谈，这里您先有个概念即可。
 那么由于您已经进入了该子程序，所以在父程序中的自订变量设定将不再继续的存在。 会存在子程序中的，仅有『环境变量』。

export 变数

这个东西用在『引用他人的档案或者其它程序』时，相当的重要的！ 尤其像鸟哥常常两三个档案互相引用来引用去的，如果忘记设定 export 的话，那么不同的档案中的相同变量值，将需要一再地重复设定才行！所以，我只要在头一个档案使用 export 的话，那么后续的档案引用时，将会把该变量内容读进来！好用的很，如果仅下达 export 而没有接变量时，那么此时将会把所有的『环境变量』秀出来喔！例如：
[root@linux ~]# export
declare -x ENV="/root/.bashrc"
declare -x HISTSIZE="1000"
declare -x HOME="/root"
declare -x HOSTNAME="linux.dmtsai.tw"
declare -x INPUTRC="/etc/inputrc"
declare -x LANG="en_US.UTF-8"
declare -x MAIL="/var/spool/mail/root"
declare -x SHELL="/bin/bash"
# 很多都直接省略了！不然....重复性太高，浪费版面～ ^_^




## 三　语系档案的变量 (locale)
那么我们的 Linux 到底支持了多少的语系呢？这可以由 locale 这个指令来查询到喔！
[root@linux ~]# locale -a
aa_DJ
aa_DJ.iso88591
en_US
en_US.iso88591
en_US.iso885915
en_US.utf8
zh_TW
zh_TW.big5
zh_TW.euctw
zh_TW.utf8
# 其实输出的内容有很多，鸟哥将一些信息舍弃了～
# 从上面的输出中，我们也不难看出，系统是有支持 big5, utf8 等中文语系数据的！中文语系至少支持了两种以上的编码，一种是目前还是很常见的 big5 ，另一种则是越来越热门的 utf-8 编码。 那么我们如何修订这些编码呢？其实可以透过底下这些变量的说：
[root@linux ~]# LANG <==主语言的环境
[root@linux ~]# LC_CTYPE <==字符辨识的编码
[root@linux ~]# LC_NUMERIC <==数字系统的显示讯息
[root@linux ~]# LC_TIME <==时间系统的显示数据
[root@linux ~]# LC_COLLATE <==字符串的比较与排序等
[root@linux ~]# LC_MONETARY <==币值格式的显示等
[root@linux ~]# LC_MESSAGES <==讯息显示的内容，如菜单、错误讯息等
[root@linux ~]# LC_ALL <==语言环境的整体设定。
基本上，你可以逐一设定每个与语系有关的变量数据，当我们使用 locale 时，系统是列出目前 Linux 主机内保有的语系档案， 这些语系档案都在：/usr/lib/locale/ 这个目录中。 但是，目前的这个 shell 环境所支持的语系，则是要看 SUPPORTED这个变数才对喔！ 那么，如果我想要修订系统的语系支持呢？/etc/sysconfig/i18n 这个档案呢:

[root@linux ~]# vi /etc/sysconfig/i18n
LANG="en_US.UTF-8"
SYSFONT="latarcyrheb-sun16"
SUPPORTED="zh_TW.UTF-8:zh_TW:zh:en_US.UTF-8"






## 四   变量的有效范围
      如果在跑程序的时候，有父程序与子程序的不同程序关系时， 则『变量』可否被引用是 export 有关。被 export 后的变量，我们可以称他为『环境变量』！ 环境变量可以被子程序所引用，但是其它的自订变量内容就不会存在于子程序中。也就是说： 我们自行设定的变量，只在目前这个 shell 环境当中存在， 在子程序中将不会存在此一变量。除非使用 export 将自订变量变成环境变量。
　   其实除了 shell 的父、子程序外，在脚本( scripts )的编写当中，由于有的软件会使用到 2 个以上的 scripts 做为一个完整的套件！也就是说，假如你有两支程序，一支为 scripts1.sh 以及 scripts2.sh ，而 scripts2.sh 会去引用 scripts1.sh 的变数，这个时候，嘿嘿！你在 scripts1.sh 当中设定的变量请『千万记得以 export 设定』， 否则你的变量将无法在两个
 scripts 之间互相被引用喔！当这个 scripts 执行完毕之后，刚刚在 scripts 当中设定的变量也就『失效了！』。 其实，要了解不同程序之间变量的变换，应该要先了解『程序』的概念比较好， 但是我们还没有讲到.....没关系～等你念到程序章节后，还可以再回来好好的看一看。 基本上，环境变量可以让子程序继续引用的原因，是因为：
     当启动一个 shell ，操作系统分配一记忆区块给 shell 使用，此区域之变量可以让子程序存取；利用 export 功能，可以让变量的内容写到上述的记忆区块当中(环境变量)；当加载另一个 shell 时 (亦即启动子程序，而离开原本的父程序了)子shell可以将父 shell 的环境变量所在的记忆区块导入自己的环境变量区块当中。






## 五    变量键盘读取、数组与宣告： read, array, declare
      我们上面提到的变量设定功能，都是直接由指令列直接设定的，那么，可不可以让使用者能够经由键盘输入？ 什么意思呢？是否记得某些程序执行的过程当中，会等待使用者输入 "yes/no" 之类的讯息啊！？ 在 bash 里面也有相对应的功能喔！此外，我们还可以宣告这个变量的属性， 例如：数组或者是数字等等的。底下就来看看吧！
read
要读取来自键盘输入的变量，就是用 read 这个指令了。这个指令最常被用在 shell script 的撰写当中， 以跟使用者进行对谈。关于 script 的写法，我们会在后面章节介绍:
[root@linux ~]# read [-pt] variable
参数：
-p ：后面可以接提示字符！
-t ：后面可以接等待的『秒数！』这个比较有趣～不会一直等待使用者啦！

范例一：让使用者由键盘输入一内容，将该内容变成 atest 变量
[root@linux ~]# read atest
This is a test
[root@linux ~]# echo $atest
This is a test

范例二：提示使用者 30 秒内输入自己的大名，将该输入字符串做成 named 变量
[root@linux ~]# read -p "Please keyin your name: " -t 30 named
Please keyin your name: VBird Tsai
[root@linux ~]# echo $named
VBird Tsai

read 之后不加任何参数，直接加上变量名称，那么底下就会主动出现一个空白行，等待您输入。 如果加上 -t 后面接秒数之后，例如上面的范例当中，那么 30 秒之内没有任何动作时， 该指令就会自动略过了～如果是加上 -p ，嘿嘿！后面就会有比较多可以用的提示字符给我们参考！ 在指令的下达里面，比较美观啦！ ^_^

declare / typeset

declare 或 typeset 是一样的功能，就是在宣告变量的属性。如果使用 declare 后面并没有接任何参数， 那么 bash 就会主动的将所有的变量名称与内容通通叫出来，就好像使用 set 一样啦！ 那么 declare 还有什么语法呢？看看先：
[root@linux ~]# declare [-aixr] variable
参数：
-a ：将后面的 variable 定义成为数组 (array)
-i ：将后面接的 variable 定义成为整数数字 (integer)
-x ：用法与 export 一样，就是将后面的 variable 变成环境变量；
-r ：将一个 variable 的变量设定成为 readonly ，该变量不可被更改内容，也不能 unset

范例一：让变量 sum 进行 100+300+50 的加总结果
[root@linux ~]# sum=100+300+50
[root@linux ~]# echo $sum
100+300+50 <==咦！怎么没有帮我计算加总？因为这是文字型态的变量属性啊！
[root@linux ~]# declare -i sum=100+300+50
[root@linux ~]# echo $sum
450 <==瞭乎？？

范例二：将 sum 变成环境变量
[root@linux ~]# declare -x sum

范例三：让 sum 变成只读属性，不可更动！
[root@linux ~]# declare -r sum
[root@linux ~]# sum=tesgting
-bash: sum: readonly variable <==老天爷～不能改这个变数了！
declare 也是个很有用的功能～尤其是当我们需要使用到底下的数组功能时， 他也可以帮我们宣告数组的属性喔！不过，老话一句，数组也是在 shell script 比较常用的啦！

数组属性 array 说明

在 bash 里头，数组的设定方式是：
var[index]=content
范例：设定上面提到的 var[1] ～ var[3] 的变数。
[root@linux ~]# var[1]="small min"
[root@linux ~]# var[2]="big min"
[root@linux ~]# var[3]="nice min"
[root@linux ~]# echo "${var[1]}, ${var[2]}, ${var[3]}"
比较有趣的地方在于『读取』，一般来说，建议直接以 ${数组} 的方式来读取， 比较正确无误的啦！






## 六    与档案系统及程序的限制关系： ulimit
想象一个状况：我的 Linux 主机里面同时登入了十个人，这十个人不知怎么搞的， 同时开启了 100 个档案，每个档案的大小约 10MBytes ，请问一下， 我的 Linux 主机的内存要有多大才够？ 10*100*10 = 10000为了要预防这个情况的发生， bash 是可以『限制使用者的某些系统资源』的，包括可以开启的档案数量， 可以使用的 CPU 时间，可以使用的内存总量等等。

[root@linux ~]# ulimit [-SHacdflmnpstuv] [配额]
参数：
-H ：hard limit ，严格的设定，必定不能超过设定的值；
-S ：soft limit ，警告的设定，可以超过这个设定值，但是会有警告讯息，并且，还是无法超过 hard limit 的喔！也就是说，假设我的 soft limit为 80 ， hard limit 为 100 ，那么我的某个资源可以用到 90 ，可以超过 80 ，还是无法超过 100 ，而且在 80~90 之间，会有警告讯息的意思。
-a ：列出所有的限制额度；
-c ：可建立的最大核心档案容量 (core files)
-d ：程序数据可使用的最大容量
-f ：此 shell 可以建立的最大档案容量 (一般可能设定为 2GB)单位为 Kbytes
-l ：可用于锁定 (lock) 的内存量
-p ：可用以管线处理 (pipe) 的数量
-t ：可使用的最大 CPU 时间 (单位为秒)
-u ：单一使用者可以使用的最大程序(process)数量。

范例一：列出所有的限制数据
[root@linux ~]# ulimit -a

范例二：限制使用者仅能建立 1MBytes 以下的容量的档案
[root@linux ~]# ulimit -f 1024
若改天你一直无法建立一个大容量的档案，记得瞧一瞧 ulimit 的信息喔！( 不过，要注意的是，一般身份使用者如果以 ulimit 设定了 -f 的档案大小， 那么他『只能减小档案大小，不能增加档案大小喔！』)






## 七   其他额外的变量设定功能
刚刚我们提到了两种变量取用的方法，分别是这样：
[root@linux ~]# echo $HOME
[root@linux ~]# echo ${HOME}

那么，在那个 ${variable} 的使用方法中，其实，我们还可以将变量进行一些修订的工作喔！ 只要加上一些字符标志，后面再接着使用比对字符串，就能够修改变量的内容了！ 我们取底下的例子来说明：在底下的例子中，假设我的变量名称为 vbird ，且内容为 /home/vbird/testing/testing.x.sh。

1. 完整呈现 vbird 这个变量的内容；
[root@linux ~]# vbird="/home/vbird/testing/testing.x.sh"
[root@linux ~]# echo ${vbird}
/home/vbird/testing/testing.x.sh

2. 在 vbird 变量中，从最前面开始比对，若开头为 / ，则删除两个/之间的
所有数据，亦即 /*/
[root@linux ~]# echo ${vbird##/*/}
testing.x.sh <==删除了 /home/vbird/testing/
[root@linux ~]# echo ${vbird#/*/}
vbird/testing/testing.x.sh <==仅删除 /home/ 而已
# 这两个小例子有趣了～变量名称后面如果接了两个 ## ，表示在 ##
# 后面的字符串取『最长的』那一段；如果仅有一个 # ，表示取『最小的那一段』喔！

3. 承上题，如果是从后面开始，删除 /* 呢？
[root@linux ~]# echo ${vbird%%/*/}
/home/vbird/testing/testing.x.sh <==都没被删除
[root@linux ~]# echo ${vbird%%/*}
<==被删除光了！
[root@linux ~]# echo ${vbird%/*}
/home/vbird/testing <==只删除 /testing.x.sh 部分
# 这个例子当中需要特别注意，那个 % 比对的是『最后面那个字符』的意思，
# 所以啰，第一个方式当然不对～因为 vbird 这个变量的内容最后面是 h 而不是 / 啊！
# 至于 %%/* 则是删除『最长的那个 /* 』，当然就是全部喔！而 %/* 则是最短的那个！

4. 将 vbird 变数中的 testing 取代为 TEST
[root@linux ~]# echo ${vbird/testing/TEST}
/home/vbird/TEST/testing.x.sh
[root@linux ~]# echo ${vbird//testing/TEST}
/home/vbird/TEST/TEST.x.sh

# 如果变量后面接的是 / 时，那么表示后面是进行『取代』的工作～而且仅取代『第一个』
# 但如果是 // ，则表示全部的字符串都取代啊！

这里您稍微留意一下就好了～反正就是变量后面可以接 #, ##, %, %%, /, // ， 而他们存在的意义并不相同的啦～ 


另外，几个不同的变量内容还可以进行判断呢！ 举例来说，目前我需要用到两个变量，分别是 var 与 str ， 那我想要针对 str 这个变量内容是否有设定成一个字符串，亦即 "expr" 来决定 var 的内容。 那我可以使用什么方法来进行判断呢？如果您会写 shell script 的话， 直接用 shell script 就好了，如果不会写，那么我们就透过简单的变量判断吧！

Tips: 底下的例子当中，那个 var 与 str 为变量，我们想要针对 str 是否有设定来决定 var 的值喔！ 一般来说， str: 代表『str 没设定或为空的字符串时』；至于 str 则仅为『没有该变数』。

　　变量设定方式          　　str没有设定             　　str为空字符串             　　str为非空字符串


![](http://pic002.cnblogs.com/images/2012/282432/2012032520190771.jpg)


根据上面这张表，我们来进行几个范例的练习吧！ ^_^

范例一：若 str 这个变量内容存在，则 var 设定为 str ，否则 var 设定为 "newvar"
[root@linux ~]# unset str; var=${str-newvar}
[root@linux ~]# echo var="$var", str="$str"
var=newvar, str= <==因为 str 不存在，所以 var 为 newvar
[root@linux ~]# str="oldvar"; var=${str-newvar}
[root@linux ~]# echo var="$var", str="$str"
var=oldvar, str=oldvar <==因为 str 存在，所以 var 等于 str 的内容

范例二：若 str 不存在，则 var 与 str 均设定为 newvar，否则仅 var 为 newvar
[root@linux ~]# unset str; var=${str=newvar}
[root@linux ~]# echo var="$var", str="$str"
var=newvar, str=newvar <==因为 str 不存在，所以 var/str 均为 newvar
[root@linux ~]# str="oldvar"; var=${str=newvar}
[root@linux ~]# echo var="$var", str="$str"
var=oldvar, str=oldvar <==因为 str 存在，所以 var 等于 str 的内容

范例三：若 str 这个变量存在，则 var 等于 str ，否则输出 "novar"
[root@linux ~]# unset str; var=${str?novar}
-bash: str: novar <==因为 str 不存在，所以输出错误讯息
[root@linux ~]# str="oldvar"; var=${str?novar}
[root@linux ~]# echo var="$var", str="$str"
var=oldvar, str=oldvar <==因为 str 存在，所以 var 等于 str 的内容
# 上面这三个案例都没有提到当 str 有设定，且为空字符串的情况喔！
# 您可以自行测试一下哩！

虽然猛一看，觉得变量没有什么奇特的地方，但是，如果仔细瞧一瞧，嘿！一堆环境变量与系统资源方面的变量， 可是会影响到我们在 bash 里头是否能够顺利作业的呢！例如 PATH 啊、ulimit 之类的～ 所以，您还是得要了解变量这个玩意才行喔！ ^_^



