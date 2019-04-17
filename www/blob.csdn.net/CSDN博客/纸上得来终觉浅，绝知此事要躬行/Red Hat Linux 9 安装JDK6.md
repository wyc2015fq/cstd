# Red  Hat Linux 9 安装JDK6 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月12日 22:44:16[boonya](https://me.csdn.net/boonya)阅读数：1059








[红黑联盟]原文地址：http://www.2cto.com/os/201201/117133.html 

1.从java.sun.com[下载](http://www.2cto.com/soft)最新的J2SE
 1.6：jdk-6u17-linux-i586-rpm.bin
2.将JDK安装文件jdk-6u17-linux-i586-rpm.bin拷贝到Redhat任意目录下。例如：/opt/jdk（目录jdk需要手动新建）
3.执行 ＃chmod +x jdk-6u17-linux-i586-rpm.bin
4.执行./jdk-6u17-linux-i586-rpm.bin
此时会出现JDK安装授权协议。可以一路按Enter浏览。如果等的不耐烦可以直接按Ctrl+C,直接会出现Do you agree to the above license terms? [yes or no]的字样。
5.键入yes，同意该授权协议。
此时[系统](http://www.2cto.com/os/)会开始解压jdk-6u17-linux-i586-rpm.bin
6.解压完毕后，回到/opt/jdk目录，键入dir。会发现多出了一个解压好的安装文件：jdk-6u17-linux-i586.rpm
7.执行rpm -ivh jdk-6u17-linux-i586.rpm
此时，系统会开始安装JDK。安装结束后可以在/usr目录下发现新增了一个名为java的文件夹。该文件夹就是安装好的JDK目录。在java目录下，又可找到jdk 1.6.0_17文件夹，该文件夹是设置环境变量时要用到的目录。
8.设置环境变量
a.进入/etc文件夹（具体操作命令忽略），找到文件profile并打开。
[注意：profile是指文件不是指文件夹]
b.找到export PATH USER LOGNAME MAIL HOSTNAME HISTSIZE INPUTRC，在该语句的上面添加以下语句：
export   JAVA_HOME="/usr/java/jdk1.6.0_17"
[注意：“=”两侧不能有空格]
export CLASSPATH="/usr/java/jdk1.6.0_17/lib:/usr/java/jdk1.6.0_17/jre/lib"
[注意：“=”两侧不能有空格；“:”是冒号，同样不要有空格出现]
pathmunge /usr/java/jdk1.6.0_17/bin:/usr/java/jdk1.6.0_17/jre/bin
[注意：无引号]
9.设置完毕后，保存文件。重启RedHat后登录控制终端，键入：java -version;
如果出现java version “1.6.0.17”等字样，说明您的JDK已经安装成功了！祝贺你！
小技巧：
如果您不想重启系统从而使修改过的配置文件profile生效，那么可以按Ctrl+Alt+F1/F2/…/F6进入控制终端。如果您还没有登录过，那么直接登录就可以启用修改过的profile文件了。如果已经登录，那么键入exit退出系统，再重新登录即可。


