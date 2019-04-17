# ijkplayer 详尽编译过程 - DEVELOPER - CSDN博客





2018年01月29日 13:03:20[学术袁](https://me.csdn.net/u012827205)阅读数：5254
所属专栏：[Android-开发](https://blog.csdn.net/column/details/20398.html)









## 摘要

```
对于比较热门新颖的技术，我还是比较喜欢的。尤其是短视频、直播应用的兴起，直接拉起 ffmpeg 和 ijkplayer 在技术开发界的声望。个人觉得学习一门技术，很多方面是要循序渐进，慢慢积累知识。通过前段时间对这方面进行了恶补，简单的了解一些，因此后来就对这块尝试了一下编译。不过你的“尝试”并不会很顺利，只有在不断尝试、学习、查阅、纠正和再尝试的过程中，才能有新的知识沉淀和突破。因此，这里我所有的一个博客记录，不是要展示什么，而是为了记录一下我的一个学习过程。并分享到更多的朋友，让朋友们在顺利编译的过程中，能够认识到和学习到更多，并了解自己应该在这方面怎么进行改进。
```

## 内容摘要

```
准备linux系统 ubuntu14.04
安装 git
安装 vim(vimcdoc-1.5.0.tar.gz)
安装 jdk(jdk-8u151-linux-x64.tar.gz)
安装 ndk(android-ndk-r10e-linux-x86_64.bin)
配置jdk、sdk和ndk环境
ijkplayer 编译
遇到的问题处理
将已编译源码导入 android studio
```

## Ubuntu14.04配置

### 开启root用户并设置密码

系统刚安装之后，打开终端`Ctrl + Alt + t`  输入命令 `ztman@ubuntu:~$ sudo passwd` 然后输入密码就能成功设置了！

### os的简体中文环境支持

通过这样的操作步骤 `System Setting -> Language Support -> Language -> install/remove language -> Chinese(Simplified)`

![这里写图片描述](https://img-blog.csdn.net/20180129144822804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个 [汉语（中国）] 是从底部位置拖动到这里，然后重启系统实现了系统语言的更改！

### 使用Windouws上的软件 WinSCP 连接 Ubuntu

首先通过命令 `sudo apt-get update` 当然，也可以在终端使用命令 `sudo -s` 这样之后，在当前的终端页面可以不再使用 sudo 作为命令的前缀。

![这里写图片描述](https://img-blog.csdn.net/20180129145503406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后通过命令`sudo apt-get install openssh-server`安装ssh， 然后启动ta命令`sudo service ssh start`。

![这里写图片描述](https://img-blog.csdn.net/20180129150054365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

之后，使用 WinSCP 进行连接。（注：以防在使用连接的时候你搞不清怎么写，自己安装的系统关于什么用户名什么的都填写一致的名称更好）结果是这样的，这样后我们就能在windows在linux之间进行数据交互！
![这里写图片描述](https://img-blog.csdn.net/20180129150635925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 安装 git

使用安装命令`sudo apt-get install git`进行git进行安装,

![这里写图片描述](https://img-blog.csdn.net/20180129151036582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

安装之后，使用命令进行最初的设置，然后进行配置信息的查看： 

```
git config --global user.name "your name" 

  git config --global user.email "youre mail" 

  git config --list 






## 安装vim



通过命令安装vim `sudo apt-get install vim`

![这里写图片描述](https://img-blog.csdn.net/20180129154121130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 安装 jdk

通过以下步骤进行顺序执行

**创建目录、加压、重定制指向**

```
root@ubuntu:/usr/lib/jvm# mkdir -p /usr/lib/jvm 

root@ubuntu:/usr/lib/jvm# sudo mvjdk-8u151-linux-x64.tar.gz /usr/lib/jvm 

root@ubuntu:/usr/lib/jvm# cd /usr/lib/jvm 

root@ubuntu:/usr/lib/jvm# sudo tar jdk-8u151-linux-x64.tar.gz.tar.gz 

root@ubuntu:/usr/lib/jvm# sudo ln -sjdk1.8.0_151 java-8


**配置jdk的环境变量** 

`root@ubuntu:~#vim ~/.bashrc`

**环境设置代码**
![这里写图片描述](https://img-blog.csdn.net/20180129161816119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
export JAVA_HOME=/usr/lib/jvm/java-8 

export JRE_HOME=${JAVA_HOME}/jre 

export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib 

export PATH=${JAVA_HOME}/bin:$PATH  

 

**执行立即生效** 

`root@ubuntu:~#source ~/.bashrc`
### 安装 ndk

首先对ndk的包进行权限的更改，然后进行解压。 
`sudo chmod 777 android-ndk-r10e-linux-x86_64.bin`
`./android-ndk-r10e-linux-x86_64.bin`
![这里写图片描述](https://img-blog.csdn.net/20180129160418995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在已经看到执行成功！
### 配置ndk的环境变量

**环境设置代码**
![这里写图片描述](https://img-blog.csdn.net/20180129162635921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
export ANDROID_NDK=/home/ztman/opt/android-ndk-r10e 

export PATH=$ANDROID_NDK:$PATH
```

**执行立即生效**
`root@ubuntu:~#source ~/.bashrc`

**命令查看ndk配置是否成功**

![这里写图片描述](https://img-blog.csdn.net/20180129163001673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样的提示状态说明已经成功！
### 安装sdk并配置环境

![这里写图片描述](https://img-blog.csdn.net/20180129163856369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20180129163902568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用命令`wget http://dl.google.com/android/android-sdk_r24.2-linux.tgz` 进行 sdk 的下载。 

然后使用命令`tar -xvf android-sdk_r24.2-linux.tgz`进行解压。 

敲入命令`cd android-sdk-linux/tools`进入到该目录， 

使用命令`./android update sdk --no-ui`安装sdk的包。 
![这里写图片描述](https://img-blog.csdn.net/20180129170506236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后使用代码
```java
export PATH=${PATH}:$HOME/sdk/android-sdk-linux/platform-tools:$HOME/sdk/android-sdk-linux/tools:$HOME/sdk/android-sdk-linux/build-tools/22.0.1/
```

，配置sdk环境，打开方式依然使用vim。共同看一下结果： 
![这里写图片描述](https://img-blog.csdn.net/20180129170718949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## ijkplayer 编译

### 克隆ijkplayer

![这里写图片描述](https://img-blog.csdn.net/20180129171426148?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用命令`git clone https://github.com/Bilibili/ijkplayer.git ijkplayer-android` 拷贝ijkplayer到当前目录。 

切换到工程的最新分支目录，执行命令 
![这里写图片描述](https://img-blog.csdn.net/20180129171904639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

执行命令 

```
cd ijkplayer-android 

git checkout -B latest k0.8.4 

 

初始化需要进行编译的代码，使用命令`./init-android.sh`。 内容较多，我贴多一些初始化的过程图片： 
![这里写图片描述](https://img-blog.csdn.net/20180129172912177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180129173951220?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 进入ijkplayer编译阶段

执行以下命令 

```
cd android/contrib 

./compile-ffmepg.sh clean 

./compile-ffmpeg.sh all 

 

![这里写图片描述](https://img-blog.csdn.net/20180129173532839?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 

![这里写图片描述](https://img-blog.csdn.net/20180130094459892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 

![这里写图片描述](https://img-blog.csdn.net/20180130094520341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



根据成功编译后的提示，继续编译ijkplayer的so文件



执行以下命令 

``cd .. 

./compile-ijk.sh 

![这里写图片描述](https://img-blog.csdn.net/20180130095343265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180130095352238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

到此就全部编译成功了。对于图片的截取，是对部分编译过程的一个快照，这样可以对自己的编译信息做一个对比。遇到的问题处理

然后在编译过程中容易遇到的问题： 

1，就是sdk和ndk的环境配置问题； 

2，就是yasm版本过旧，需要更新然后再次接着终端时候的命令重新编译；sdk和ndk的环境配置问题

这个就不多说了，因为上面已经聊的很清楚。yasm版本过旧

这个问题的出现会导致什么问题呢？请看：

![这里写图片描述](https://img-blog.csdn.net/20180130100027742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此我们需要对其进行处理：在我的操作中，我首先把目录切换到``/home/ztman/opt/下，然后进行操作。 

在此目录下依次执行命令： 
``curl http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz >yasm.tar.gz 

tar zxvf yasm-1.3.0.tar.gz 

cd yasm-1.3.0/ 

./configure --prefix=/usr/local/yasm 

make 

make install 


然后，在``~/.bashrc 文件中添加环境变量：``export PATH=$PATH:/usr/local/yasm/bin 保存之后执行命令：``source ~/.bashrc

![这里写图片描述](https://img-blog.csdn.net/20180130100412143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

到此就解决了编译过程出现的那个问题，然后就可以继续中断的编译。将已编译源码导入 android studio

首先表明一下我的实践结果，对于编译的源代码来讲，导入android studio的方式很普通，跟我们平时导入一个 existing project 是一样的。只需把 ./ijkplayer-android/android 目录下的 project ijkplayer导入就ok。 
然后主要是怎么进行精简项目代码？！像这样。。。

![这里写图片描述](https://img-blog.csdn.net/20180130101858977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
减去那些 module 形如 ijkplayer-armXX、ijkplayer-x86XX。怎么使用他们的.so文件替代这些module？！

例如，为实现这种精简效果，这个时候把 module ijkplayer-armv7a下的armabi-v7a文件夹下的so文件导入到 module ijkplayer-java中； 
![这里写图片描述](https://img-blog.csdn.net/20180130102247470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在 module ijkplayer-java 的 src目录下新建一个 jniLibs 文件夹，并把armeabi-v7a导入。

![这里写图片描述](https://img-blog.csdn.net/20180130102041581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

关键的地方是在 module ijkplayer-java 的 builder.gradle 文件中添加这几行代码，能够让其找到并引用 jniLibs 下的so文件。 
![这里写图片描述](https://img-blog.csdn.net/20180130102828156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
[](https://gitee.com/yuan1530702811/ijkplayer)这样之后，就能干掉那些不用的moudle了。
```
```
```
```
```





