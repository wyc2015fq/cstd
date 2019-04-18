# shell局部变量和全局变量 - nosmatch的专栏 - CSDN博客
2011年08月12日 19:10:49[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：54490标签：[shell																[centos																[bash																[java																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
                
 一、变量分类
局部变量和环境变量，局部变量只适用于当前shell，而环境变量是全局的，它适用于所有当前shell以及其派生出来的任意子进程，有些变量是用户创建的，其他的则是专用的shell变量。
二、局部变量
局部变量的作用域被限定在创建它们的shell中。local函数可以用来创建局部变量，但仅限于函数内使用。局部变量可以通过简单的赋予它一个值或一个变量名来设置，用declare内置函数来设置，或者省略也可。
格式：
变量名=值
declare 变量名=值
以下来示范一个局部变量的例子：
[root@centos ~]# echo $$
8836
[root@centos ~]# round=world
[root@centos ~]# echo $round
world
[root@centos ~]# bash
[root@centos ~]# echo $$
8920
[root@centos ~]# echo $round
[root@centos ~]# exit
exit
[root@centos ~]# echo $$
8836
[root@centos ~]# echo $round
world
在上面的岩石中可以看出，当启动一个bash程序，相当于创建一个子shell后，round变在这个子shell中没有被赋值，exit退出子shell后，可以看到父shell中变量round仍有值。
三、环境变量
环境变量通常又称全局变量，以区别于局部变量，通常，环境变量应该大写，环境变量是已经用export内置命令导出的变量。子shell继承当前父shell的环境变量，并能一直传承下去，但是不可逆向传递。
格式：
export 变量=值
declare -x 变量=值
我们做个示范：
[root@centos ~]# export NAME=”yuan faxiang”
[root@centos ~]# echo $NAME
yuan faxiang
[root@centos ~]# echo $$
8836
[root@centos ~]# bash
[root@centos ~]# echo $$
8958
[root@centos ~]# echo $NAME
yuan faxiang
[root@centos ~]# declare -x NAME=”Arvin”
[root@centos ~]# echo $NAME
Arvin
[root@centos ~]# exit
exit
[root@centos ~]# echo $$
8836
[root@centos ~]# echo $NAME
yuan faxiang
上述例子中，父shell中定义的NAME环境变量传递到了子shell中，在子shell中定义的NAME环境变量没有被带到父shell中。
环境变量按生存周期可分为：
1. 永久的：需要修改配置文件，export变量永久生效。
2. 临时的：使用export命令行声明即可，变量在关闭shell时失效。
2.设置变量的三种方法
1) 在/etc/profile文件中添加变量【对所有用户生效(永久的)】
用VI在文件/etc/profile文件中增加变量，该变量将会对Linux下所有用户有效，并且是“永久的”。
例如：编辑/etc/profile文件，添加CLASSPATH变量
# vi /etc/profile
export CLASSPATH=./JAVA_HOME/lib;$JAVA_HOME/jre/lib
注：修改文件后要想马上生效还要运行# source /etc/profile不然只能在下次重进此用户时生效。
2) 在用户目录下的.bash_profile文件中增加变量【对单一用户生效(永久的)】
用VI在用户目录下的.bash_profile文件中增加变量，改变量仅会对当前用户有效，并且是“永久的”。
例如：编辑guok用户目录(/home/guok)下的.bash_profile
$ vi /home/guok/.bash.profile
添加如下内容：
export CLASSPATH=./JAVA_HOME/lib;$JAVA_HOME/jre/lib
注：修改文件后要想马上生效还要运行$ source /home/guok/.bash_profile不然只能在下次重进此用户时生效。
3) 直接运行export命令定义变量【只对当前shell(BASH)有效(临时的)】
在shell的命令行下直接使用[export变量名=变量值]定义变量，该变量只在当前的shell(BASH)或其子shell(BASH)下是有效的，shell关闭了，变量也就失效了，再打开新shell时就没有这个变量，需要使用的话还需要重新定义。
