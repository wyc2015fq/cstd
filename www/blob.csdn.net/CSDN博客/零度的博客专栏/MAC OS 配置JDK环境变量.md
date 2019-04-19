# MAC OS 配置JDK环境变量 - 零度的博客专栏 - CSDN博客
2017年05月03日 17:44:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：3488
大家在windows里面配置JDK环境变量很容易，但是如果要在mac里面配置JDK环境变量和windows里面有所不同，具体如下：
第一：
mac OS里面自带jdk，不过是1.6的版本，现在很多人使用的都是1.7甚至更高的版本，所需要替换，自己去 oracle的官网下载最新的对应的jdk版本，网址如下：http://www.oracle.com/technetwork/[Java](http://lib.csdn.net/base/17)/javase/downloads/index-jsp-138363.html;
下载完以后安装，此时会有问题出现，系统自带的jdk和我们的jdk在什么地方。 
mac系统自带的jdk的安装目录在如下目录：/System/Library/Java/JavaVirtualMachines/1.6.0.jdk/Contents/Home ；   在mac系统里面jdk的路径是要到Home这里的，所以我们在后面配置环境变量的时候一点要记住选择到home这个文件这里。
我们自己安装的jdk的目录一般在  /Library/Java/JavaVirtualMachines/jdk1.7.0_79.jdk/Contents/Home  下；  也就是到Library这个目录下面去找应该就可以找到。
找到路径以后就是如何配置环境变量的问题，配置环境变量比较简单：
需要修改两个文件
第一个文件  .bash_profile ，使用如下命令修改：
unix:bin bin$ cd ~ 进入到 **~ **目录
unix:~ bin$ touch .bash_profile  如果.bash_profile不存在则创建这个文件
unix:~ bin$ vi .bash_profile  使用vi编辑器编辑 .bash_profile文件
然后输入   i   ,在vi编辑器里面输入 i  的意思是开始编辑。
vi编辑器里面的内容如下:
JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.7.0_79.jdk/Contents/Home
CLASSPAHT=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
PATH=$JAVA_HOME/bin:$PATH:
export JAVA_HOME
export CLASSPATH
export PATH
添加如上所示的语句就可以了，第一行代表的是jdk的路径，一定要记住选择到 Home 这个文件加下面；
然后退出vi编辑器使用如下命令：
1. 按键 Esc 
2. 输入冒号 : wq
3. 保存退出 
如果以上修改完毕切正确，那么接下来就是让配置的环境变量生效，使用如下命令：
source .bash_profile  
或者可以直接重新启动机器，注销机器都可以。
完毕以后查看下当前的java 版本是否正确输入如下命令：
java -version
如下所示：
unix:~ bin$ java -version
java version "1.7.0_79"
Java(TM) SE Runtime Environment (build 1.7.0_79-b15)
Java HotSpot(TM) 64-Bit Server VM (build 24.79-b02, mixed mode)
如果出现了上面所示的对应的自己的安装的版本那就说明没有问题了，或者使用如下命令查看环境变量是否已经配置成功：
echo $JAVA_HOME
如下所示：
unix:~ bin$ echo $JAVA_HOME
/Library/Java/JavaVirtualMachines/jdk1.7.0_79.jdk/Contents/Home
我这里显示已经配置成功了，如果查看java的版本正确，但是查看是jdk环境变量配置失败，重新打开terminal 查看，前面打开的 终端 可能显示不正确，我的就是如此。
以上的配置对于当前的用户的，如果换了用户就不能使用了。 
还有很多人遇到的情况可能是就算配置了上面的东西，但是查看java的版本还是不对，那么我们去配置另外一个东西，配置的内容和上面的的 .bash_profile文件是一样的，只不过接下来要配置的这个文件是所有的用户都管用，就和我们在windows里面配置的系统和用户的环境变量一样。具体配置如下：
首先进入 /etc目录
cd /etc
修改 etc 目录下的profile文件，此文件是系统级别的文件，修改的时候必须要使用超级用户才能修改，使用如下命令：
sudo vi profile 
具体命令行如下：
unix:~ bin$ cd /etc
unix:etc bin$ sudo vi profile
Password:
如上所示，在Password后面输入超级用户密码就可以了，进入vi编辑器界面
# System-wide .profile for sh(1)
if [ -x /usr/libexec/path_helper ]; then 
        eval `/usr/libexec/path_helper -s`
fi
if [ "${BASH-no}" != "no" ]; then
        [ -r /etc/bashrc ] && . /etc/bashrc
fi
JAVA_HOME="/Library/Java/JavaVirtualMachines/jdk1.7.0_79.jdk/Contents/Home"
export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH
export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
"profile" [readonly] 13L, 372C
如上所示，添加上
JAVA_HOME="/Library/Java/JavaVirtualMachines/jdk1.7.0_79.jdk/Contents/Home"
export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH
export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
这几句话就好。
然后保存退出vi编辑器，和前面操作一样
ese
:wq
如果出现 E45: 'readonly' option is set (add ! to override) 或者其他类似提示不能编辑的提示，需要强制保存退出，就需要在wq的后面加上感叹号，如下所示：
:wq!
然后就可以退出了。如果不能退出，就百度或者谷歌查明原名。在此我没有碰到不能退出的情况.
然后和上面的操作一样，需要上配置的环境变量生效：
source profile
或者重新启动机器 
然后查看java版本：
java -version
或者查看路径是否配置成功：
echo $JAVA_HOME
查看输入的信息是否是自己配制的就可以知道是否配置成功。
经过以上步骤可以配置完 mac下的jdk环境变量。
