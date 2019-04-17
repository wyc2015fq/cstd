# CentOS 6.2 Eclipse CDT 开发环境搭建 - DoubleLi - 博客园






一、安装中文语言支撑

  我当初安装CentOS6.2时选择了中文语言支持，有iBus和拼音输入法，有gnome和kde

  没有安装的可以参考：[CentOS英文环境下使用中文输入法](http://blog.csdn.net/g1036583997/article/details/8904869)  安装中文语言支持和输入法

二、配置XWindow

1 /etc/sysconfig/i18n 文件的内容：

   LANG="zh_CN.UTF-8" 

2 .bashrc 文件加入：

   export LANG="en_US.UTF-8"

   export LC_ALL="en_US.UTF-8"

3 XWindow不能启动的问题解决：

   删除用户home目录下所有与XWindow有关的文件和目录，只需保留如下文件： .bash*  .gvimrc  .ssh  .subversion  .vim  .viminfo  .vimrc  你的其它工作文件及目录。

   尤其是修改了语言设置后，gnome卡死的问题，用这个办法可以解决。

4 配置输入法

   将输入法文件复制到英文环境下

   #cp /usr/share/locale/zh_CN/LC_MESSAGES/ibus* /usr/share/locale/en_US/LC_MESSAGES 

   将ibus设置为英文环境的开机启动项

   #vi /etc/X11/xinit/xinitrc.d/50-xinput.sh 

   找到_im_language_list=”as bn gu hi ja kn ko ml mr ne or pa si ta te th ur vi zh” 

   在列表中增加 en

5 启动XWindow，添加中文输入法

   在登录窗口的最下面部分，语言选择 English(United States) ，会话选择 GNOME

   点菜单：system->preferences->input method 

   点按钮：Input Method Preferences...    选择拼音输入法并添加

![](https://images2015.cnblogs.com/blog/382515/201510/382515-20151023145246567-2130554792.png)![](https://images2015.cnblogs.com/blog/382515/201510/382515-20151023145540270-1630392835.png)

![](file:///C:/Users/yucheng/AppData/Roaming/Tencent/Users/3109206108/QQ/WinTemp/RichOle/OHXVNJN%6076W%7B]GAZ0WXV8KR.png)

![](file:///C:/Users/yucheng/AppData/Roaming/Tencent/Users/3109206108/QQ/WinTemp/RichOle/OHXVNJN%6076W%7B]GAZ0WXV8KR.png)

![](file:///C:/Users/yucheng/AppData/Roaming/Tencent/Users/3109206108/QQ/WinTemp/RichOle/OHXVNJN%6076W%7B]GAZ0WXV8KR.png)

   打开编辑器，按“Control+space”，即可输入中文。(如果没有，先注销用户，再进入)

![](file:///C:/Users/yucheng/AppData/Roaming/Tencent/Users/3109206108/QQ/WinTemp/RichOle/OHXVNJN%6076W%7B]GAZ0WXV8KR.png)

 三、安装JDK （[参考URL](http://www.centoscn.com/image-text/install/2015/0217/4721.html)）

1 下载[jdk-7u80-linux-x64.gz](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

2 mv文件到/usr/local目录下

   cd /usr/local 

   tar -xzvf jdk-7u80-linux-x64.gz

   mv  jdk-7u80-linux-x64  jdk7

3 修改 .bash_profile文件，加入：

   export JAVA_HOME=/usr/local/jdk7

   export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar

   export PATH=$JAVA_HOME/bin:$PATH

4 退出用户，重新登录

   $ java -version

   java version "1.7.0_80"

   Java(TM) SE Runtime Environment (build 1.7.0_80-b15)

   Java HotSpot(TM) 64-Bit Server VM (build 24.80-b11, mixed mode)

四、安装eclipse  ([参考URL](http://segmentfault.com/a/1190000002651775))

1 下载 [eclipse-cpp-luna-SR2-linux-gtk-x86_64.tar.gz](http://www.eclipse.org/downloads/packages/release/luna/sr2)

2 以root用户操作：

   mkdir /opt/eclipse

   cd /opt/eclipse 

   tar -zxvf eclipse-cpp-luna-SR2-linux-gtk-x86_64.tar.gz 

   mv eclipse  luna-SR2

   ln -s /opt/eclipse/luna-SR2/eclipse /usr/local/bin/eclipse-luna 

3 创建桌面启动

   vim /usr/share/applications/eclipse-luna.desktop

   添加如下代码：

   [Desktop Entry]

   Type=Application

   Name=Eclipse 4.4.2 Luna

   Comment=Eclipse Luna C/C++

   Icon=/opt/eclipse/luna-SR2/icon.xpm

   Exec=/usr/local/bin/eclipse-luna

   Terminal=false

   Categories=Development;IDE;C/C++;

4 检查eclipse是否被添加到应用程序中

![](https://images2015.cnblogs.com/blog/382515/201510/382515-20151023161326755-420489825.png)









