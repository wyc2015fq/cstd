# shell中sudo和su命令 - xqhrs232的专栏 - CSDN博客
2017年03月08日 11:00:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3588
原文地址::[http://blog.csdn.net/liuxigiant/article/details/9090993](http://blog.csdn.net/liuxigiant/article/details/9090993)
sudo command
   sudo命令是以root的身份执行command命令，但是环境变量还是当前用户的，执行目录也仍然是当前目录
  即环境变量和执行目录都不会切换到root
sudo command1 | command2     这种命令只会是command1有root权限，但是管道符后面的command则还是没有root权限。Sudo只会提升紧跟其后的那个命令的权限
 su - username -c "command1;command2"
  su命令是切换到另一个用户，环境变量会切换到username，执行目录会切换到目标用户username的家目录
  提醒：
   假设当前用户为普通用户lx（该用户没有ORACLE_SID这个环境变量），以lx用户执行命令
   su - oracle -c "echo $ORACLE_SID"
   输出会是空
 su - username -c环境变量会切换到username，为什么没打印出oracle用户的ORACLE_SID环境变量呢？
   因为双引号是不屏蔽$这个特殊字符的，在执行su - oracle -c "echo $ORACLE_SID"命令，
   将先在当前用户下替换变量ORACLE_SID（当前用户的ORACLE_SID变量为空），然后发送给oracle执行的命令就成了 echo ""
   我们的意图是将echo $ORACLE_SID这个命令发送给oracle用户，打印出oracle用户用户的环境变量ORACLE_SID，有以下解决方式：
   1、su - oracle -c 'echo $ORACLE_SID'   （单引号会屏蔽所有的特殊字符）
   2、su - oracle -c "echo \$ORACLE_SID"
命令行命令“
 su 与 su - 命令的却别：
  su 命令仅切换用户身份，例如从A切换到B，执行whoami命令，显示的是用户B，但当前目录不会切换，
  环境变量也仍未切换，仍为A用户的环境变量
  su - 命令切换用户，A切换到B，会以登录B的流程执行，不仅会切换用户，还会执行.profile文件，
  更换成B用户的环境变量，目录切换到B的家目录
给普通用户配置sudo权限：
 配置sudo权限后不需要知道root的密码，也可以用root身份执行命令
 1、以root登录，打开/etc/sudoers 文件
  可以输入visudo命令，或者直接 vi /etc/sudoers 
 2、在文件中添加一行，username为你需要配置的用户名
  以下转载自：http://blog.csdn.net/jiaoyanchen/article/details/6376562
  root有所有权限，只要仿照现有root的例子就行，我们在下面加一行（最好用tab作为空白）：
  username      ALL=(ALL) ALL
  只想让username像root那样使用ls和ifconfig，把那一行改为：
　  username      localhost=    /sbin/ifconfig,   /bin/ls
 现在让我们来看一下那三个ALL到底是什么意思。
  第一个ALL是指网络中的主机，我们后面把它改成了主机名，它指明username可以在此主机上执行后面的命令。
  第二个括号里的ALL是指目标用户，也就是以谁的身份去执行命令。
  最后一个ALL当然就是指命令名了。例如，我们想让username用户在linux主机上以jimmy或rene的身份执行kill命令，
  这样编写配置文件：
　　  username    linux=(jimmy,rene)    /bin/kill
　　 但这还有个问题，username到底以jimmy还是rene的身份执行？这时我们应该想到了sudo -u了，它正是用在这种时候。
  username可以使用sudo -u jimmy kill PID或者sudo -u rene kill PID，但这样挺麻烦，其实我们可以不必每次加-u，
  把rene或jimmy设为默认的目标用户即可。
  再在上面加一行：
　　 Defaults:username    runas_default=rene
 Defaults后面如果有冒号，是对后面用户的默认，如果没有，则是对所有用户的默认。就像配置文件中自带的一行：
　　 Defaults    env_reset
　　 另一个问题是，很多时候，我们本来就登录了，每次使用sudo还要输入密码就显得烦琐了。
  我们可不可以不再输入密码呢？当然可以，我们这样修改配置文件：
　　  username localhost=NOPASSWD:     /bin/cat, /bin/ls
　　 当然，你也可以说“某些命令用户username不可以运行”，通过使用!操作符，但这不是一个好主意。
  因为，用!操作符来从ALL中“剔出”一些命令一般是没什么效果的，一个用户完全可以把那个命令拷贝到别的地方，
  换一个名字后再来运行。
  sysopr ALL=(root) NOPASSWD: ALL
  sysopr ALL=(oracle) NOPASSWD: /home/sysopr/sudoscript/sudosqlplus
变为变为 root 环境
 sudo -s
 输入root密码后，用户直接切换到root，当前目录不会切换
 sudo命令格式如下：
　　sudo -K -L -V -h -k -l -vsudo [-HPSb] [-a auth_type] [-c
　　class-] [-p prompt] [-u username#uid] {-e file [...] -i -s command}
　　下面我们再来看一下sudo其它常用的一些参数：
　　选项     含义     作用
　　sudo -h     Help     列出使用方法，退出。
　　sudo -V     Version     显示版本信息，并退出。
　　sudo -l     List     列出当前用户可以执行的命令。只有在sudoers里的用户才能使用该选项。
　　sudo -u username#uid     User     以指定用户的身份执行命令。后面的用户是除root以外的，可以是用户名，也可以是#uid。
　　sudo -k     Kill     清除“入场卷”上的时间，下次再使用sudo时要再输入密码。
sudo -K     Sure kill     与-k类似，但是它还要撕毁“入场卷”，也就是删除时间戳文件。
　　sudo -b command     Background     在后台执行指定的命令。
 sudo -p prompt command     Prompt     可以更改询问密码的提示语，其中%u会代换为使用者帐号名称，%h会显示主机名称。非常人性化的设计。
　　sudo -e file     Edit     不是执行命令，而是修改文件，相当于命令sudoedit。
 -s<shell> 　执行指定的shell。
