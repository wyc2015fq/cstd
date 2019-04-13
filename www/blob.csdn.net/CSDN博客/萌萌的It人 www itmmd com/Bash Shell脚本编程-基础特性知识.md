
# Bash Shell脚本编程-基础特性知识 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月06日 13:31:08[Jlins](https://me.csdn.net/dyllove98)阅读数：1360


**Bash Shell****的基础特性：**
命令历史：
history：shell内建命令
环境变量HISTFILE定义用户退出命令历史列表的存储位置
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|\#echo $HISTFILE
|/root/.bash_history
|\# echo $HISTFILESIZE   HISTFILE指向的文件持久留存的历史命令条目
|1000
|\# echo $HISTSIZE           当前bash进程所留存的历史命令条目数
|1000
|\# history -d|123|删除历史列表中的第|123|条命令
|\# history -c                   清空历史命令
|\# history|20|显示最近的|20|条命令
|\# history -a                   追加当前bash进程的新历史命令至命令历史文件中
|122|echo $HISTFILESIZE
|123|history
|124|history -d|123
|125|history
|126|history|20
|\# !|122|调用历史列表中的命令
|echo$HISTFILESIZE
|1000
|\# !!                                         再次执行上一条命令
|echo$HISTFILESIZE
|1000
|\# !lsb                                     快速调用历史命令中执行的命令
|lsb_release
|LSBVersion:   :base-|4.0|-amd64:base-|4.0|-noarch:core-|4.0|-amd64:core-|4.0|-noarch:graphics-|4.0|-amd64:graphics-|4.0|-noarch:printing-|4.0|-amd64:printing-|4.0|-noarch
|

|1
|2
|3
|4
|5
|6
|\# ls/etc/passwd
|/etc/passwd
|\# cat 这里按esc键，然后按.就会调用/etc/passwd
|\#!$
|/etc/passwd
|-bash:/etc/passwd: Permission denied
|

控制命令历史记录的方式：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|环境变量：HISTCONTROL
|\# echo $HISTCONTROL           忽略重复的命令（连续且相同）
|ignoredups
|ignorespace                         忽略以空格开头的命令
|\#export HISTCONTROL=ignorespace
|\#echo $HISTCONTROL
|ignoredups
|ignoreboth：上面两个功能同时生效
|使用方法：
|exportHISTCONTROL=参数
|撤销HISTCONTROL的控制：
|unsetHISTCONTROL
|


命令补全：
bash可以根据内建命令或外部命令的查找方式查找以用户指定的字符串开头的命令并直接予以补充完整的机制。
给出的打头的串已经可以唯一标识某命令，直接补全
不能唯一标识单个命令，再次键入TAB键就可以列出所有相关命令。
在PATH环境变量所指定的路径下找可执行文件名进行补全
**路径补全：****TAB****键**
把用户给出的字符串开头的做路径进行文件名称补全的机制
在指定路径下进行补全操作
命令行展开
~：展开为用户的家目录
｛｝：可承载一个以逗号分隔的列表，并将其展开为多个路径
/tmp/{a,b}
/tmp/a,/tmp/b
/tmp/{a,b}/hello:
/tmp/a/hello
/tmp/a/hello
练习：如何创建/tmp/x/y1, /tmp/x/y2, /tmp/x/y1/a,/tmp/x/y1/b, /tmp/x/y2/a, /tmp/x/y2/b
|1
|2
|3
|4
|5
|6
|7
|8
|\# mkdir -pv /tmp/x/{y1,y2}/{a,b}/
|mkdir:created directory `/tmp/x'
|mkdir:created directory `/tmp/x/y1'
|mkdir:created directory `/tmp/x/y1/a/'
|mkdir:created directory `/tmp/x/y1/b/'
|mkdir:created directory `/tmp/x/y2'
|mkdir:created directory `/tmp/x/y2/a/'
|mkdir:created directory `/tmp/x/y2/b/'
|


练习：如何创建a_d, a_c, b_d, b_c四个目录？
|1
|2
|3
|4
|5
|\#mkdir -pv /tmp/{a,b}_{c,d}/
|mkdir:created directory `/tmp/a_c/'
|mkdir:created directory `/tmp/a_d/'
|mkdir:created directory `/tmp/b_c/'
|mkdir:created directory `/tmp/b_d/'
|

|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|\# tree /tmp/
|/tmp/
|├──a3
|├──a_c
|├──a_d
|├──b_c
|├──b_d
|├──ks-script-ILxZQr
|├──ks-script-ILxZQr.log
|├──test
|├──test1
|├──x
|│   ├── y1
|│   │   ├── a
|│   │   └── b
|│   └── y2
|│       ├── a
|│       └── b
|└──yum.log
|14directories,|3|files
|

|1
|\#tree /path/to/somedir
|

-L \#：显示多少级内容
练习：创建
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|\# tree /tmp/mylinux/
|/tmp/mylinux/
|├──bin
|├──etc
|│   └── sysconfig
|│       └── etwork-scripts
|├──sbin
|├──usr
|│   ├── bin
|│   ├── lib
|│   ├── lib64
|│   ├── local
|│   │   ├── bin
|│   │   ├── lib
|│   │   ├── lib64
|│   │   └── sbin
|│   ├── sbin
|│   └── share
|└──|var
|├── lib
|├── log
|└── run
|20directories,|0|files
|

|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|\# mkdir -pv /tmp/mylinux/{bin,sbin,etc/sysconfig/etwork-scripts,usr/{bin,sbin,local/{bin,sbin,lib,lib64},share,lib,lib64},|var|/{log,run,lib}}
|mkdir:created directory `/tmp/mylinux'
|mkdir:created directory `/tmp/mylinux/bin'
|mkdir:created directory `/tmp/mylinux/sbin'
|mkdir:created directory `/tmp/mylinux/etc'
|mkdir:created directory `/tmp/mylinux/etc/sysconfig'
|mkdir:created directory `/tmp/mylinux/etc/sysconfig/etwork-scripts'
|mkdir:created directory `/tmp/mylinux/usr'
|mkdir:created directory `/tmp/mylinux/usr/bin'
|mkdir:created directory `/tmp/mylinux/usr/sbin'
|mkdir:created directory `/tmp/mylinux/usr/local'
|mkdir:created directory `/tmp/mylinux/usr/local/bin'
|mkdir:created directory `/tmp/mylinux/usr/local/sbin'
|mkdir:created directory `/tmp/mylinux/usr/local/lib'
|mkdir:created directory `/tmp/mylinux/usr/local/lib64'
|mkdir:created directory `/tmp/mylinux/usr/share'
|mkdir:created directory `/tmp/mylinux/usr/lib'
|mkdir:created directory `/tmp/mylinux/usr/lib64'
|mkdir:created directory `/tmp/mylinux/|var|'
|mkdir:created directory `/tmp/mylinux/|var|/log'
|mkdir:created directory `/tmp/mylinux/|var|/run'
|mkdir:created directory `/tmp/mylinux/|var|/lib'
|

创建一个月日年小时分钟秒的文件名
|1
|2
|3
|4
|\# mkdir -pv $(date|"+%Y-%m-%d-%H-%M-%S"|)
|mkdir:created directory `|2014|-|11|-|11|-|19|-|11|-|29|'
|[root@localhosttmp]\# ls
|2014|-|11|-|11|-|19|-|11|-|29
|


命令执行结果
每个命令执行完成后隶属于两种状态：
成功0
失败1-127
bash通过特殊变量$?保存了最近执行完成的这条命令的状态
echo  $?
\#echo $?
0
程序自身返回的数据
程序执行完成后的状态结果
\#rpm -rf /tmp/mylinux
rpm:arguments to --root (-r) must begin with a /
\#echo $?
1
命令别名
\#alias
|1
|2
|3
|4
|5
|6
|7
|8
|\# alias
|aliascp=|'cp -i'
|aliasl.=|'ls -d .* --color=auto'
|aliasll=|'ls -l --color=auto'
|aliasls=|'ls --color=auto'
|aliasmv=|'mv -i'
|aliasrm=|'rm -i'
|aliaswhich=|'alias | /usr/bin/which --tty-only --read-alias --show-dot --show-tilde'
|

使用原始命令：\command
注意：在命令行中定义的别名，只对当前shell进程有效
持续有效需要编辑配置文件：
仅对当前用户有效：~/.bashrc
对全局用户有效：/etc/profile
注：直接编辑配置文件不会对当前shell进程有效
取消别名：unaliasNAME

**globbing****通配**
文件名通配：元字符
*：匹配任意长度的任意字符
Pa*，*pa*，*pa，p*a，*p*a(文件名以a结尾且中间包含p的）
？：匹配任意单个字符
\#ls -d  /etc/p??
[]：匹配指定集合内的任意单个字符
[0-9]
[a-z]
[A-Z]
[0-9a-zA-Z]
\#ls   -d    /etc/p[mn]*

[^]：匹配指定集合外的任意单个字符
\#ls  -d    /etc/p[^m-z]*
字符集合：
[:space:]：所有的空白字符
[[:space:]]：除去空白字符的所有
[:punct:]：所有标点符号
[:lower:]：所有小写字母
[:upper:]：所有大写字母
[:digit:]：所有数字
[:alpha:]：所有字母
[:alnum:]：所有字母和数字

练习1：显示/var目录下所有l开头，以一个小写字母结尾，且中间出现一位数字的文件或目录；
|1
|\# ls-d /|var|/l*[[:digit:]]*[[:lower:]]
|

练习2：显示/etc/目录下，以任意一位数字开头，且以非数字结尾的文件或目录；
|1
|\# ls-d /etc/[|0|-|9|]*[^|0|-|9|]
|

练习3:显示/etc/目录下，以非字母开头，后面跟了一个字母及其它任意长度任意字符的文件或目录；
|1
|\# ls-d /etc/[^[:alpha:]][[:alpha:]]*
|

练习4：复制/etc/下，所有以m开头，以非数字结尾的文件或目录至/tmp/test1目录中；
|1
|2
|\#mkdir test1
|\# cp-a /etc/m*[^|0|-|9|] /tmp/test1
|

练习5：复制/etc目录下，所有以.d结尾的文件或目录至/tmp/test2目录中
|1
|2
|\#mkdir test2
|\# cp-r /etc/*.d /tmp/test2
|

练习6：复制/etc目录下，所有以.conf结尾，以m,n,r,p开头的文件或目录至/tmp/test3目录中；
|1
|2
|\#mkdir test3
|\# cp-r /etc/[mnrp]*.conf /tmp/test3
|


快捷键
\#cdd /etc/sysconfig/n
\#Ctrl + a：跳转至命令首部
\#Ctrl + e：跳转至命令尾部
\#Ctrl + l：清屏
\#Ctrl + c：中止或取消
\#Ctrl + u：删除命令行首至光标所在处所有字符
\#Ctrl + k：删除光比所在处至尾部的所有字符
输入输出重定向
程序有输入输出的需求
程序：指令+数据
程序处理数据后产生的结果保存的位置的需求是输出
标准输入：键盘，0
标准输出：监视器，1
错误输出：显示器，2
fd：file descriptor，文件描述符，windows称为句柄
0，1，2
输出重定向：把文件输出到其他地方
>：目标文件中原有内容被清除
>>：新内容被追加至文件尾部
\#cat   issue >/tmp/issue.out
\#ls  /var  >> /tmp/issue.out
使用set  -C来关闭覆盖重定向，set 
 +C开启
\#ls /etc>|/tmp/issue这是进行强制覆盖
错误输出进行输出重定向：把错误信息输出到其他地方
|1
|2
|3
|\#ls  /|var|r|2|> /tmp/|var|.err
|\#ls /|var|2|> /tmp/|var|.out
|\#ls  /|var|r|2|>>/tmp/|var|/err
|

合并标准输出和错误输出的数据流至同一处：&>
|1
|2
|3
|\#ls   /|var|&> /tmp/|var|.out
|\#ls   /|var|&>> /tmp/|var|.out
|COMMAND > /path/to/somefile|2|>  &|1
|


输入重定向：<
<<：此处生成文档
2<<
2<

