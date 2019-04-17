# 鸟哥的私房菜：Linux文件与目录管理 - 在思索中前行！ - CSDN博客





2014年09月24日 20:04:59[_Tham](https://me.csdn.net/txl16211)阅读数：608标签：[linux文件																[linux目录																[linux文件权限](https://so.csdn.net/so/search/s.do?q=linux文件权限&t=blog)
个人分类：[鸟哥的私房菜学习](https://blog.csdn.net/txl16211/article/category/2590995)





## 1. 目录与路径 
1.1 相对路径与绝对路径

    绝对路径：路径的写法『一定由根目录 / 写起』，如：/usr/share/doc 这个目录。
    相对路径：路径的写法『不是由 / 写起』，例如由 /usr/share/doc 要到 /usr/share/man 底下时，可以写成： 『cd ../man』这就是相对路径的写法啦！相对路径意指『相对于目前工作目录的路径！』

1.2 目录的相关操作： cd, pwd, mkdir, rmdir

    . 代表此层目录
    .. 代表上一层目录
    - 代表前一个工作目录
    ~ 代表『目前使用者身份』所在的家目录
    ~account 代表 account 这个使用者的家目录

    命令的使用，mkdir有两个参数m和p分别可以设置新建文档的权限和递归, rmdir有一个参数p，递归删除该目录下的文档。 略。

1.3 关于执行文件路径的变量： $PATH

    shell命令之所以可以执行，是由于PATH环境变量。
    列出path路径：echo $PATH
    假设有个新命令X在一个新的路径/xxx/test下，为了使用该命令，可以有2种方法：
　　1)将新路径加入PATH：PATH = "$PATH":/xxx/test,而后就可以使用X命令
　　2)使用绝对路径使用命令x:/xxx/test/X

---------------------------------------------------------------------------------------------------------------

## 2. 档案与目录管理 
2.1 档案与目录的检视： ls

    常用的参数-l显示长格式，-a显示隐藏档案。

2.2 复制、移动与删除： cp, mv, rm

1) 复制cp：

    A)  cp [-adfilprsu] 来源档(source) 目的檔(destination)
    B)  cp [options] source1 source2 source3 ....directory

    cp 指令加上参数可以做很多事情， 除了单纯的复制之外，还可以建立连结档 (就是快捷方式)，比对两档案的新旧而予以更新， 以及复制整个目录等等的功能，具体参见参数。

2) 移动mv

    A)  mv [-fiu] source destination
    B)  mv [options] source1 source2 source3 .... directory

参数：
-f ：force 强制的意思，强制直接移动而不询问；
-i ：若目标档案 (destination) 已经存在时，就会询问是否覆盖！
-u ：若目标档案已经存在，且 source 比较新，才会更新 (update)


3) 删除rm:

rm [-fir] 档案或目录

参数：
-f ：就是 force 的意思，强制移除；
-i ：互动模式，在删除前会询问使用者是否动作
-r ：递归删除啊！最常用在目录的删除了

2.3 取得路径的文件名称与目录名称(basename和dirname)

[root@linux ~]# basename /etc/sysconfig/network
    network <== 很简单！就取得最后的档名～

[root@linux ~]# dirname /etc/sysconfig/network
    /etc/sysconfig <== 取得的变成目录名了！

---------------------------------------------------------------------------------------------------------------

## 3. 档案内容查阅： 

    我们要查阅一个档案的内容时，这里有相当多有趣的指令可以来分享一下： 最常使用的显示档案内容的指令可以说是 cat 与 more 及 less 了！此外，如果我们要查看一个很大型的档案 ( 好几百MB时 )，但是我们只需要后端的几行字而已，那么该如何是好？呵呵！用
 tail 呀，此外， tac 这个指令也可以达到！好了，说说各个指令的用途吧:

 cat     由第一行开始显示档案内容
    tac     从最后一行开始显示，可以看出 tac 是 cat 的倒着写！
    nl     显示的时候，顺道输出行号！
    more     一页一页的显示档案内容
    less     与 more 类似，但是比 more 更好的是，他可以往前翻页！
    head     只看头几行
    tail     只看尾巴几行
    od     以二进制的方式读取档案内容！


3.1 直接检视档案内容： cat, tac, nl

1) cat

[root@linux ~]# cat [-AEnTv]

参数：
-A ：相当于 -vET 的整合参数，可列出一些特殊字符～
-E ：将结尾的断行字符 $ 显示出来；
-n ：打印出行号；
-T ：将 [tab] 按键以 ^I 显示出来；
-v ：列出一些看不出来的特殊字符

2) tac(反向列示)

功能跟 cat 相反啦， cat 是由『第一行到最后一行连续显示在屏幕上』，而 tac 则是『 由最后一行到第一行反向在屏幕上显示出来 』

3) nl(添加行号打印)

[root@linux ~]# nl [-bnw] 档案

参数：
-b ：指定行号指定的方式，主要有两种：
    -b a ：表示不论是否为空行，也同样列出行号；
    -b t ：如果有空行，空的那一行不要列出行号；

-n ：列出行号表示的方法，主要有三种：
    -n ln ：行号在屏幕的最左方显示；
    -n rn ：行号在自己字段的最右方显示，且不加 0 ；
    -n rz ：行号在自己字段的最右

3.2 可翻页检视： more, less

1) more指令

[root@linux ~]# more /etc/man.config
#
# Generated automatically from man.conf.in by the
# configure script.
#
# man.conf from man-1.5p
#
.......中间省略.......
--More--(28%) <== 重点在这一行喔！

    仔细的给他看到上面的范例，如果 more 后面接的档案长度大于屏幕输出的行数时， 就会出现类似上面的图示。重点在最后一行，最后一行会显示出目前显示的百分比， 而且还可以在最后一行输入一些有用的指令喔！在 more 这个程序的运作过程中，你有几个按键可以按的：

    空格键 (space)：代表向下翻一页；
    Enter ：代表向下翻『一行』；
 /字符串 ：代表在这个显示的内容当中，向下搜寻『字符串』；
    :f ：立刻显示出文件名以及目前显示的行数；
q ：代表立刻离开 more ，不再显示该档案内容。

    要离开 more 这个指令的显示工作，可以按下 q 就能够离开了。而要向下翻页，就使用空格键即可。 比较有用的是搜寻字符串的功能，举例来说，我们使用『 more /etc/man.config 』来观察该档案， 若想要在该档案内搜寻 MANPATH 这个字符串时，可以这样做：

[root@linux ~]# more /etc/man.config
#
# Generated automatically from man.conf.in by the
# configure script.
#
# man.conf from man-1.5p
#
.......中间省略.......

    /MANPATH <== 输入了 / 之后，光标就会自动跑到最底下一行等待输入！如同上面的说明，输入了 / 之后，光标就会跑到最底下一行，并且等待您的输入， 您输入了字符串之后，more 就会开始向下搜寻该字符串～而重复搜寻同一个字符串可以直接按下 n 即可啊！最后，不想要看了，就按下
 q 即可离开 more啦。

2) less指令

[root@linux ~]# less /etc/man.config
#
# Generated automatically from man.conf.in by the
# configure script.
#
# man.conf from man-1.5p
......中间省略........
: <== 这里可以等待您输入指令！


    less 的用法比起 more 又更加的有弹性，在 more 的时候，我们并没有办法向前面翻， 只能往后面看，但若使用了 less 时，呵呵！就可以使用 [pageup] [pagedown] 等按键的功能来往前往后翻看文件，您瞧，是不是更容易使用来观看一个档案的内容了呢！？
 除此之外，在 less 里头可以拥有更多的『搜寻』功能喔！不止可以向下搜寻，也可以向上搜寻～ 实在是很不错用～基本上，可以输入的指令有：

    空格键 ：向下翻动一页；
    [pagedown]：向下翻动一页；
    [pageup] ：向上翻动一页；
    /字符串 ：向下搜寻『字符串』的功能；
    ?字符串 ：向上搜寻『字符串』的功能；
n ：重复前一个搜寻 (与 / 或 ? 有关！)
N ：反向的重复前一个搜寻 (与 / 或 ? 有关！)
    q ：离开 less 这个程序；


3.3 资料撷取： head, tail

head 与 tail 都是以『行』为单位来进行数据撷取的。

1) head(取头几行)

[root@linux ~]# head [-n number] 档案

参数：
-n ：后面接数字，代表显示几行的意思

范例：
[root@linux ~]# head /etc/man.config
# 预设的情况中，显示前面十行！若要显示前 20 行，就得要这样：
[root@linux ~]# head -n 20 /etc/man.config

2) tail(取尾几行)

[root@linux ~]# tail [-n number] 档案

参数：
-n ：后面接数字，代表显示几行的意思

范例：
[root@linux ~]# tail /etc/man.config
# 预设的情况中，显示最后的十行！若要显示最后的 20 行，就得要这样：
[root@linux ~]# tail -n 20 /etc/man.config

3.4 非纯文字文件： od

[root@linux ~]# od [-t TYPE] 档案

参数：
-t ：后面可以接各种『类型 (TYPE)』的输出，例如：
a ：利用预设的字符来输出；
c ：使用 ASCII 字符来输出
d[size] ：利用十进制(decimal)来输出数据，每个整数占用 size bytes ；
f[size] ：利用浮点数值(floating)来输出数据，每个数占用 size bytes ；
o[size] ：利用八进位(octal)来输出数据，每个整数占用 size bytes ；
x[size] ：利用十六进制(hexadecimal)来输出数据，每个整数占用 size bytes ；

范例：
[root@linux ~]# od -t c /usr/bin/passwd
0000000 177 E L F 001 001 001 \0 \0 \0 \0 \0 \0 \0 \0 \0
0000020 002 \0 003 \0 001 \0 \0 \0 260 225 004 \b 4 \0 \0 \0

3.5 修改档案时间与建置新档： touch

1) 档案的3种时间

    我们在 ls 这个指令的介绍时，有稍微提到每个档案在 linux 底下都会记录三个主要的变动时间：
    modification time (mtime)：当该档案的『内容数据』变更时，就会更新这个时间！ 内容数据指的是档案的内容，而不是档案的属性喔！
    status time (ctime)：当该档案的『状态 (status)』改变时，就会更新这个时间，举例来说， 像是权限与属性被更改了，都会更新这个时间啊～
    access time (atime)：当『该档案的内容被取用』时，就会更新这个读取时间 (access)。 举例来说，我们使用 cat 去读取 ~/.bashrc ，就会更新 atime 了。

举例:
[root@linux ~]# ls -l /etc/man.config
-rw-r--r-- 1 root root 4506 Apr 8 19:11 /etc/man.config
[root@linux ~]# ls -l --time=atime /etc/man.config
-rw-r--r-- 1 root root 4506 Jul 19 17:53 /etc/man.config
[root@linux ~]# ls -l --time=ctime /etc/man.config
-rw-r--r-- 1 root root 4506 Jun 25 08:28 /etc/man.config

在预设的情况下， ls 显示出来的是该档案的 mtime.

2) touch指令

[root@linux ~]# touch [-acdmt] 档案
参数：
-a ：仅修订 access time；
-c ：仅修改时间，而不建立档案；
-d ：后面可以接日期，也可以使用 --date="日期或时间"
-m ：仅修改 mtime ；
-t ：后面可以接时间，格式为[YYMMDDhhmm]
---------------------------------------------------------------------------------------------------------------
4. 档案与目录的预设权限与隐藏权限 
4.1 档案预设权限：umask



umask 以补码的形式指定 『目前使用者在建立档案或目录时候的属性默认值』


[root@linux ~]# umask
0022
[root@linux ~]# umask -S
u=rwx,g=rx,o=rx

    查阅的方式有两种，一种可以直接输入 umask 以数字型态的权限设定分数， 一种则是加入 -S (Symbolic) 这个参数，以符号类型的方式来显示出权限！ 奇怪的是，怎么 umask 会有四组数字啊？不是只有三组吗？是没错啦，第一组是特殊权限用的，先看后面三组即可。 在预设权限的属性上，目录与档案是不一样的。由于档案我们不希望他具有可执行的权力，
 预设情况中，档案是没有可执行 (x) 权限的。因此：
    1) 若使用者建立为『档案』则预设『没有可执行 ( x ) 项目』，也就是最大为 666 分，预设属性如下： -rw-rw-rw-
    2) 若使用者建立为『目录』，则由于 x 与是否可以进入此目录有关，因此预设为所有权限均开放，亦即为 777 分，预设属性如下： drwxrwxrwx

    那么 umask 指定的是『该默认值需要减掉的权限！』,以上面的例子来说明的话，因为 umask 为 022 ，所以 user 并没有被拿掉属性，不过 group 与 others 的属性被拿掉了 2 ( 也就是 w 这个属性 )，那么022针对档案和目录分别代表：

    (-rw-rw-rw-) – (-----w--w-) ==> -rw-r--r--
    (drwxrwxrwx) – (d----w--w-) ==> drwxr-xr-x

我们就来测试看看吧
[root@linux ~]# umask
0022
[root@linux ~]# touch test1
[root@linux ~]# mkdir test2
[root@linux ~]# ll
-rw-r--r-- 1 root root 0 Jul 20 00:36 test1
drwxr-xr-x 2 root root 4096 Jul 20 00:36 test2

    那么如何设定 umask 呢？假如我们想要让与使用者同群组的人也可以存取档案，我们希望制作出来的档案应该是 -rw-rw-r-- 的模样， umask 应该是要 002 才好 (仅拿掉 others 的 w 权限)。 简单的很，直接umask 后面输入 002
 就好了！

[root@linux ~]# umask 002
[root@linux ~]# touch test3
[root@linux ~]# mkdir test4
[root@linux ~]# ll
-rw-rw-r-- 1 root root 0 Jul 20 00:41 test3
drwxrwxr-x 2 root root 4096 Jul 20 00:41 test4

Tips: 在预设的情况中， root 的 umask 会拿掉比较多的属性，root 的 umask 预设是 022 ， 这是基于安全的考虑啦。至于一般身份使用者，通常他们的
 umask 为 002 ，亦即保留同群组的写入权力！

4.2 档案隐藏属性： chattr, lsattr

档案还有隐藏属性？光是那九个权限就快要疯掉了，竟然还有隐藏属性。但是没办法，就是有档案的隐藏属性存在啊！隐藏的属性确实对于系统有很大的帮助的，尤其是在系统安全 (Security) 上面。底下我们就来谈一谈如何设定与检查这些隐藏的属性吧：

1) chattr (设定档案隐藏属性)

[root@linux ~]# chattr [+-=][ASacdistu] 档案或目录名称

参数：
+ ：增加某一个特殊参数，其它原本存在参数则不动。
- ：移除某一个特殊参数，其它原本存在参数则不动。
= ：设定一定，且仅有后面接的参数

A ：当设定了 A 这个属性时，这个档案(或目录)的存取时间 atime (access)将不可被修改，可避免例如手提式计算机容易有磁盘 I/O 错误的情况发生！
S ：这个功能有点类似 sync 的功能！就是会将数据同步写入磁盘当中！可以有效的避免数据流失！
a ：当设定 a 之后，这个档案将只能增加数据，而不能删除，只有 root才能设定这个属性。
c ：这个属性设定之后，将会自动的将此档案『压缩』，在读取的时候将会自动解压缩，但是在储存的时候，将会先进行压缩后再储存(对于大档案似乎蛮有用的！)
d ：当dump(备份)程序被执行的时候，设定 d 属性将可使该档案(或目录)不具有dump功能
i ：这个 i 可就很厉害了！他可以让一个档案『不能被删除、改名、设定连结,也无法写入或新增资料！』对于系统安全性有相当大的帮助！
j ：当使用 ext3 这个档案系统格式时，设定 j 属性将会使档案在写入时先记录在
journal 中！但是当 filesystem 设定参数为 data=journalled 时，由于已经设定了日志了，所以这个属性无效！
s ：当档案设定了 s 参数时，他将会被完全的移除出这个硬盘空间。
u ：与 s 相反的，当使用 u 来设定档案时，则数据内容其实还存在磁盘中，可以使用来 undeletion.

注意：这个属性设定上面，比较常见的是 a 与 i 的设定值，而且很多设定值必须要身为root 才能够设定的喔！

范例：
[root@linux ~]# cd /tmp
[root@linux tmp]# touch attrtest
[root@linux tmp]# chattr +i attrtest
[root@linux tmp]# rm attrtest
rm: remove write-protected regular empty file `attrtest'? y
rm: cannot remove `attrtest': Operation not permitted

# 看到了吗？呼呼！连 root 也没有办法将这个档案删除呢！赶紧解除设定！
[root@linux tmp]# chattr -i attrtest

这个指令是重要的，尤其是在系统的安全性上面！由于这些属性是隐藏的性质，所以需要以 lsattr 才能看到该属性呦！其中，个人认为最重要的当属 +i 这个属性了，因为他可以让一个档案无法被更动，对于需要强烈的系统安全的人来说， 真是相当的重要的！里头还有相当多的属性是需要
 root 才能设定的呢！此外，如果是 log file 这种的登录档，就更需要 +a 这个可以增加，但是不能修改旧有的数据与删除的参数了！ 

2) lsattr (显示档案隐藏属性)

[root@linux ~]# lsattr [-aR] 档案或目录

参数：
-a ：将隐藏文件的属性也秀出来；
-R ：连同子目录的数据也一并列出来！
范例：
[root@linux tmp]# chattr +aij attrtest
[root@linux tmp]# lsattr
----ia---j--- ./attrtest

使用 chattr 设定后，可以利用 lsattr 来查阅隐藏的属性。

4.4 档案特殊权限：SUID/SGID/Sticky Bit

我们前面一直提到关于档案的重要权限，那就是 rwx 这三个读、写、执行的权限。 但是，眼尖的朋友们一定注意到了一件事，那就是，怎么我们的 /tmp 权限怪怪的？ 还有，那个 /usr/bin/passwd 也怪怪的？怎么回事啊？看看先：

[root@linux ~]# ls -ld /tmp ; ls -l /usr/bin/passwd
drwxrwxrwt 5 root root 4096 Jul 20 10:00 /tmp
-r-s--x--x 1 root root 18840 Mar 7 18:06 /usr/bin/passwd

不是只有 rwx 吗？还有其它的特殊权限啊？我们这里先不谈这两个权限，只是先介绍一下而已。 因为要了解这几个特殊的权限，必须要先具有账号的 ID 概念，以及程序的程序 (process) 概念后， 才能够进一步的了解这个特殊权限所代表的意义。

1) Set UID

制作出 s 与 t 的权限，是为了让一般使用者在执行某些程序的时候， 能够暂时的具有该程序拥有者的权限。 举例来说好了，我们知道账号与密码的存放档案其实是 /etc/passwd 与 /etc/shadow 。 而 /etc/shadow 这个档案的权限是什么？是『-r--------』。且他的拥有者是
 root 。在这个权限中，仅有 root 可以『强制』储存，其它人是连看都没有办法看的吶！ 但是偏偏鸟哥使用 dmtsai 这个一般身份使用者去更新自己的密码时，使用的就是 /usr/bin/passwd 这个程序， 却是可以更新自己的密码的，这就是 s 这个权限的帮助啦！ 

当 s 这个权限在 user 的 x 时，也就是类似上表的 -r-s--x--x ，称为 Set UID ，简称为 SUID ， 这个
 UID 代表的是 User 的 ID ，而 User 代表的则是这个程序 (/usr/bin/passwd) 的拥有者 (root 啊！)。 那么由上面的定义中，我们知道了，当 dmtsai 这个使用者执行/usr/bin/passwd时，他就会『暂时』的得到档案拥有人 root 的权限。 

SUID 仅可用在『二进制制档案(binary file)』上， SUID 因为是程序在执行的过程中拥有档案拥有者的权限，因此，他仅可用于
 binary file ， 不能够用在批次档 (shell script) 上面的！当然，SUID 对于目录也是无效的。

2) Set GID
    进一步来说，如果 s 的权限是在 group 时，那么就是 Set GID ，简称为 SGID。 SGID 可以用在两个部分：
    档案：如果 SGID 是设定在 binary file 上面，则不论使用者是谁，在执行该程序的时候， 他的有效群组 (effective group) 将会变成该程序的群组所有人 (group id)。
    目录：如果 SGID 是设定在 A 目录上面，则在该 A 目录内所建立的档案或目录的 group ，将会是 此 A 目录的 group ！

    一般来说， SGID 应该是比较多用在特定的多人团队的项目开发上， 在系统当中应该会比较少这个设定才对～

3) Sticky Bit

这个 Sticky Bit 目前只针对目录有效，对于档案已经没有效果了。 SBit 对于目录的作用是：『在具有
 SBit 的目录下，使用者若在该目录下具有 w 及 x 的权限， 则当使用者在该目录下建立档案或目录时，只有档案拥有者与 root 才有权力删除』。

    换句话说：当甲这个使用者于 A 目录下是拥有 group 或者是 other 的用户，并且拥有 w 的权限， 这表示『甲使用者对该目录内任何人建立的目录或档案均可进行 "删除/更名/搬移" 等动作。』 不过，如果将 A 目录加上了 Sticky bit 的权限项目时，
 则甲只能够针对自己建立的档案或目录进行删除/更名/移动等动作。 举例来说，我们的 /tmp 本身的权限是『drwxrwxrwt』， 在这样的权限内容下，任何人都可以在 /tmp 内新增、修改档案，但仅有该档案/目录建立者与 root 能够删除自己的目录或档案。你可以这样做个简单的测试：

SUID/SGID/SBIT 权限设定

前面介绍过 SUID 与 SGID 的功能，那么如何开启档案使成为具有 SUID 与 SGID 的权限呢？！ 如果在这三个数字之前再加上一个数字的话，那最前的面数字就代表这几个属性了！ ( 注：通常我们使用 chmod xyz filename 的方式来设定 filename
 的属性时，则是假设没有 SUID, SGID 及 Sticky bit  )
4 为 SUID
2 为 SGID
1 为 Sticky bit

假设要将一个档案属性改为『-rwsr-xr-x』时，由于 s 在使用者权限中，所以是 SUID ，因此， 在原先的 755 之前还要加上 4 ，也就是：『 chmod
 4755 filename 』来设定！此外，还有大 S 与大 T 的产生喔！参考底下的范例啦！(注意：底下的范例只是练习而已， 所以鸟哥使用同一个档案来设定，您必须了解 SUID 不是用在目录上，而 SBIT 不是用在档案上的)

[root@linux ~]# cd /tmp
[root@linux tmp]# touch test

[root@linux tmp]# chmod 4755 test; ls -l test
-rwsr-xr-x 1 root root 0 Jul 20 11:27 test

[root@linux tmp]# chmod 6755 test; ls -l test
-rwsr-sr-x 1 root root 0 Jul 20 11:27 test

[root@linux tmp]# chmod 1755 test; ls -l test
-rwxr-xr-t 1 root root 0 Jul 20 11:27 test

[root@linux tmp]# chmod 7666 test; ls -l test
-rwSrwSrwT 1 root root 0 Jul 20 11:27 test

# 这个例子就要特别小心啦！怎么会出现大写的 S 与 T 呢？不都是小写的吗？
# 因为 s 与 t 都是取代 x 这个参数的，但是你有没有发现阿，我们是下达
# 7666 喔！也就是说， user, group 以及 others 都没有 x 这个可执行的标志
# ( 因为 666 嘛！ )，所以，这个 S, T 代表的就是『空的』啦！怎么说？
# SUID 是表示『该档案在执行的时候，具有档案拥有者的权限』，但是档案
# 拥有者都无法执行了，哪里来的权限给其它人使用？当然就是空的啦！ ^_^

4.3 档案类型：file

如果你想要知道某个档案的基本数据，例如是属于 ASCII 或者是 data 档案，或者是 binary ， 且其中有没有使用到动态函式库 (share library) 等等的信息，就可以利用 file 这个指令来检阅喔！ 举例来说：

[root@linux ~]# file ~/.bashrc
/root/.bashrc: ASCII text <== 告诉我们是 ASCII 的纯文字文件啊！

[root@linux ~]# file /usr/bin/passwd
/usr/bin/passwd: setuid ELF 32-bit LSB executable, Intel 80386, version 1
(SYSV), for GNU/Linux 2.2.5, dynamically linked (uses shared libs), stripped
# 数据可多了～包括这个日 Set UID 2的档案，使用 shared libs，
# 适合于 Intel 的 386 以上机种的硬件，很清楚吧！

[root@linux ~]# file /var/lib/slocate/slocate.db
/var/lib/slocate/slocate.db: data <== 这是 data 档案！
透过这个指令，我们可以简单的先判断这个档案的格式为何喔！

---------------------------------------------------------------------------------------------------------------

5. 档案的搜寻：which, whereis, locate, find在 Linux 底下也有相当优异的搜寻系统呦！通常 find 不很常用的！因为速度慢之外，也很操硬盘！通常我们都是先使用 whereis 或者是 locate 来检查，如果真的找不到了，才以 find 来搜寻呦！因为 whereis 与 locate 是利用数据库来搜寻数据，所以相当的快速，而且并没有实际的搜寻硬盘，比较省时间

1) which (寻找『执行档』)

[root@linux ~]# which [-a] command
参数：
-a ：将所有可以找到的指令均列出，而不止第一个被找到的指令名称
范例：
[root@linux ~]# which passwd
/usr/bin/passwd
[root@linux ~]# which traceroute -a
/usr/sbin/traceroute
/bin/traceroute
这个指令是根据『PATH』这个环境变量所规范的路径，去搜寻『执行档』的档名～ 所以，重点是找出『执行档』而已！且 which 后面接的是『完整档名』喔！若加上 -a 参数，则可以列出所有的可以找到的同名执行文件，而非仅显示第一个而已！

2) whereis (寻找特定档案)

[root@linux ~]# whereis [-bmsu] 档案或目录名
参数：
-b :只找 binary 的档案
-m :只找在说明文件 manual 路径下的档案
-s :只找 source 来源档案
-u :没有说明档的档案！

范例：
[root@linux ~]# whereis passwd
passwd: /usr/bin/passwd /etc/passwd /etc/passwd.OLD
/usr/share/man/man1/passwd.1.gz /usr/share/man/man5/passwd.5.gz
# 任何与 passwd 有关的档名都会被列出来～

[root@linux ~]# whereis -b passwd
passwd: /usr/bin/passwd /etc/passwd /etc/passwd.OLD

[root@linux ~]# whereis -m passwd
passwd: /usr/share/man/man1/passwd.1.gz /usr/share/man/man5/passwd.5.gz

3) locate

[root@linux ~]# locate filename
[root@linux ~]# locate passwd
/lib/security/pam_passwdqc.so
/lib/security/pam_unix_passwd.so
/usr/lib/kde3/kded_kpasswdserver.so
/usr/lib/kde3/kded_kpasswdserver.la
.......中间省略.......

这个 locate 的使用更简单，直接在后面输入『档案的部分名称』后，就能够得到结果。 举上面的例子来说，我输入 locate passwd ，那么在完整文件名 (包含路径名称) 当中，只要有 passwd 在其中， 就会被显示出来的！这也是个很方便好用的指令，如果您忘记某个档案的完整档名时.

4) find

[root@linux ~]# find [PATH] [option] [action]

参数：

1. 与时间有关的参数：

-atime n ：n 为数字，意义为在 n 天之前的『一天之内』被 access 过的档案；
-ctime n ：n 为数字，意义为在 n 天之前的『一天之内』被 change 过状态的档案；
-mtime n ：n 为数字，意义为在 n 天之前的『一天之内』被 modification 过的档案；
-newer file ：file 为一个存在的档案，意思是说，只要档案比 file 还要新，
就会被列出来～


2. 与使用者或群组名称有关的参数：

-uid n ：n 为数字，这个数字是使用者的账号 ID，亦即 UID ，这个 UID 是记录在
/etc/passwd 里面与账号名称对应的数字。这方面我们会在第四篇介绍。
-gid n ：n 为数字，这个数字是群组名称的 ID，亦即 GID，这个 GID 记录在
/etc/group，相关的介绍我们会第四篇说明～
-user name ：name 为使用者账号名称喔！例如 dmtsai
-group name：name 为群组名称喔，例如 users ；
-nouser ：寻找档案的拥有者不存在 /etc/passwd 的人！
-nogroup ：寻找档案的拥有群组不存在于 /etc/group 的档案！

当您自行安装软件时，很可能该软件的属性当中并没有档案拥有者，这是可能的！在这个时候，就可以使用 -nouser 与 -nogroup 搜寻。

3. 与档案权限及名称有关的参数：

-name filename：搜寻文件名称为 filename 的档案；
-size [+-]SIZE：搜寻比 SIZE 还要大(+)或小(-)的档案。这个 SIZE 的规格有：
c: 代表 byte， k: 代表 1024bytes。所以，要找比 50KB
还要大的档案，就是『 -size +50k 』
-type TYPE ：搜寻档案的类型为 TYPE 的，类型主要有：一般正规档案 (f),
装置档案 (b, c), 目录 (d), 连结档 (l), socket (s),
及 FIFO (p) 等属性。
-perm mode ：搜寻档案属性『刚好等于』 mode 的档案，这个 mode 为类似 chmod
的属性值，举例来说， -rwsr-xr-x 的属性为 4755 ！
-perm -mode ：搜寻档案属性『必须要全部囊括 mode 的属性』的档案，举例来说，
我们要搜寻 -rwxr--r-- ，亦即 0744 的档案，使用 -perm -0744，
当一个档案的属性为 -rwsr-xr-x ，亦即 4755 时，也会被列出来，
因为 -rwsr-xr-x 的属性已经囊括了 -rwxr--r-- 的属性了。
-perm +mode ：搜寻档案属性『包含任一 mode 的属性』的档案，举例来说，我们搜寻
-rwxr-xr-x ，亦即 -perm +755 时，但一个档案属性为 -rw-------
也会被列出来，因为他有 -rw.... 的属性存在！

4. 额外可进行的动作：
-exec command ：command 为其它指令，-exec 后面可再接额外的指令来处理搜寻到
的结果。
-print ：将结果打印到屏幕上，这个动作是预设动作！

范例一：将过去系统上面 24 小时内有更动过内容 (mtime) 的档案列出
[root@linux ~]# find / -mtime 0
# 那个 0 是重点！0 代表目前的时间，所以，从现在开始到 24 小时前，
# 有变动过内容的档案都会被列出来！那如果是三天前的 24 小时内？
# find / -mtime 3 ，意思是说今天之前的 3*24 ~ 4*24 小时之间
# 有变动过的档案都被列出的意思！同时 -atime 与 -ctime 的用法相同。

范例二：寻找 /etc 底下的档案，如果档案日期比 /etc/passwd 新就列出
[root@linux ~]# find /etc -newer /etc/passwd
# -newer 用在分辨两个档案之间的新旧关系是很有用的！

范例三：搜寻 /home 底下属于 dmtsai 的档案
[root@linux ~]# find /home -user dmtsai
# 这个东西也很有用的～当我们要找出任何一个使用者在系统当中的所有档案时，
# 就可以利用这个指令将属于某个使用者的所有档案都找出来喔！

范例四：搜寻系统中不属于任何人的档案
[root@linux ~]# find / -nouser
# 透过这个指令，可以轻易的就找出那些不太正常的档案。
# 如果有找到不属于系统任何人的档案时，不要太紧张，
# 那有时候是正常的～尤其是您曾经以原始码自行编译软件时。

范例五：找出档名为 passwd 这个档案
[root@linux ~]# find / -name passwd
# 利用这个 -name 可以搜寻档名啊！

范例六：搜寻档案属性为 f (一般档案) 的档案
[root@linux ~]# find /home -type f
# 这个 -type 的属性也很有帮助喔！尤其是要找出那些怪异的档案，
# 例如 socket 与 FIFO 档案，可以用 find /var -type p 或 -type s 来找！


范例七：搜寻档案当中含有 SGID/SUID/SBIT 的属性
[root@linux ~]# find / -perm +7000
# 所谓的 7000 就是 ---s--s--t ，那么只要含有 s 或 t 的就列出，
# 所以当然要使用 +7000 ，使用 -7000 表示要含有 ---s--s--t 的所有三个权限，
# 因此，就是 +7000 ～瞭乎？

范例八：将上个范例找到的档案使用 ls -l 列出来～
[root@linux ~]# find / -perm +7000 -exec ls -l {} \;
# 注意到，那个 -exec 后面的 ls -l 就是额外的指令，
# 而那个 {} 代表的是『由 find 找到的内容』的意思～所以， -exec ls -l {}
# 就是将前面找到的那些档案以 ls -l 列出长的数据！至于 \; 则是表示
# -exec 的指令到此为止的意思～意思是说，整个指令其实只有在
# -exec (里面就是指令下达) \;
# 也就是说，-exec 最后一定要以 \; 结束才行！这样了解了吗？！


范例九：找出系统中，大于 1MB 的档案
[root@linux ~]# find / -size +1000k
# 虽然在 man page 提到可以使用 M 与 G 分别代表 MB 与 GB，
# 不过，俺却试不出来这个功能～所以，目前应该是仅支持到 c 与 k 吧！](https://so.csdn.net/so/search/s.do?q=linux目录&t=blog)](https://so.csdn.net/so/search/s.do?q=linux文件&t=blog)




