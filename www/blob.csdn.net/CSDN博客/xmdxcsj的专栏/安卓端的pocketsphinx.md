# 安卓端的pocketsphinx - xmdxcsj的专栏 - CSDN博客





2015年01月20日 22:33:00[xmucas](https://me.csdn.net/xmdxcsj)阅读数：921








[http://cmusphinx.sourceforge.net/wiki/tutorialandroid](http://cmusphinx.sourceforge.net/wiki/tutorialandroid)

一、安装android版的demo

1.安装jdt

*Help→Install New Software*.

Work with输入"[http://download.eclipse.org/releases/juno"](http://download.eclipse.org/releases/juno%26quot;)

选择*Programming Languages* 里面的Eclipse Java Development Tools安装

2.安卓项目中使用库文件

在libs目录下面

if you are building application for ARM architecture on Eclipse, you will have to include “libs/pocketsphinx-android-0.8-nolib.jar” and “libs/armeabi/libpocketsphinx_jni.so” into your project.

3.模型文件的存放位置

存放在目录assets/下面。

assets.xml文件：ant脚本，用于产生assets.lst和.md5文件

assets.lst文件：记录模型文件名的路径

.md5文件：文件的Mmd5值

pocketsphinx-android里面的Assets类有方法可以自动将assets 文件复制到外部sd卡上面，只有当assets文件夹下面的文件和对应的sd卡上面的文件md5不一致或者sd卡上面没有该文件的时候才会发生自动拷贝。

二、构建lib的编译环境

1.将ant的环境变量添加到PATH

D:\Program Files\adt-bundle-windows-x86-20140702\eclipse\plugins\org.apache.ant_1.8.3.v201301120609\bin

2.安装swig

下载：http://www.swig.org/download.html

解压以后即可

添加PATH

D:\Program Files\swigwin-3.0.4

3.安装android ndk

下载：http://developer.android.com/tools/sdk/ndk/index.html

安装

添加PATH

D:\android-ndk-r10d

4.安装cygwin

[http://www.cnblogs.com/baronzhao/archive/2012/07/10/2585181.html](http://www.cnblogs.com/baronzhao/archive/2012/07/10/2585181.html)

Cygwin的下载地址：[http://www.cygwin.com/](http://www.cygwin.com/)




5.下载pocketsphinx代码

下载三个软件放在同一目录pocketsphinx/下面




svn checkout svn://svn.code.sf.net/p/cmusphinx/code/trunk/sphinxbase

svn checkout svn://svn.code.sf.net/p/cmusphinx/code/trunk/pocketsphinx

svn checkout svn://svn.code.sf.net/p/cmusphinx/code/trunk/pocketsphinx-android




在pocketsphinx-android目录新建build.properties文件，添加以下内容：注意斜线不能使用\

sdk.dir=D:/Program Files/adt-bundle-windows-x86-20140702/sdk

ndk.dir=D:/android-ndk-r10d

sdk.version=19




在当前目录运行“ant jar”

5.报错信息

**Class not found: javac1.8**

今天用Ant打包Android，apk，运行出现了batch_build.xml  Class not found: [java](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2FProgram%2FAndroid%2F201405%2F199102.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=java&k0=android&kdi0=8&k1=java&kdi1=8&k2=eclipse&kdi2=8&sid=e5c27f7be8bc6e66&ch=0&tu=u1682280&jk=027c1a061ba707d5&cf=29&fv=11&stid=9&urlid=0&luki=2&seller_id=1&di=128)c1.8的错误，后来上网查了一下是Ant版本太低，低于1.9，无法和java1.8兼容,因为我的java运行环境是1.8的，Ant版本也是1.8，所以就只有升级Ant版本了。升级Ant版本方法是：

   1.进入http://ant.apache.org/manualdownload.cgi下载新版本的Ant，下载之后解压。

   2.进入你eclipse下的插件路径找出Ant插件，如我的就是D:/Android/eclipse/plugins/org.apache.ant_1.8.4.v201303080030，打开该路径，将Ant新版本解压出来的文件替换到该目录下，不过要保留META-INF目录 和plugin.properties文件。

   3.可以通过cmd进入该插件的位置之后运行ant -version可以查看到ant的版本号，更新成功会显示新的版本号

   4.重启[eclipse](http://cpro.baidu.com/cpro/ui/uijs.php?rs=1&u=http%3A%2F%2Fwww.th7.cn%2FProgram%2FAndroid%2F201405%2F199102.shtml&p=baidu&c=news&n=10&t=tpclicked3_hc&q=smileking_cpr&k=eclipse&k0=android&kdi0=8&k1=java&kdi1=8&k2=eclipse&kdi2=8&sid=e5c27f7be8bc6e66&ch=0&tu=u1682280&jk=027c1a061ba707d5&cf=29&fv=11&stid=9&urlid=0&luki=3&seller_id=1&di=128)，之后用ant打包就能正常兼容jdk1.8了。

**ERROR: Cannot find 'make' program.**

没有安装cygwin

**问题：ndk-build”: CreateProcess error=193 **

解决：该问题表明，调用了非windows程序，在build.xml中将ndk-build修改为ndk-build.cmd即可ant编译

将pocketsphinx-android文件夹下面的build.xml文件ndk.ext变量赋值为.cmd，如下：、

  <property name="ndk.ext" value=".cmd" />

**NDK path cannot contain any spaces!**

ndk的路径不能包含空格，比如不能将ndk文件安装在D:\Program Files\



