# 折腾Ubuntu下的android studio - xcw0754 - 博客园
# [折腾Ubuntu下的android studio](https://www.cnblogs.com/xcw0754/p/5245518.html)
ps：网速很慢，耗时一天多。先吐槽一下搭建很麻烦，毕竟现在在ubuntu上用as的人还不太多，很多步骤最好先自备梯子。
测试环境：
`ubuntu 14.04 LTS 64bit`
安装的东西无非就是androidstudio、SDK、JDK、genymotion、as插件。最痛苦的过程是***，不过还好一切顺利，项目的工作环境成功迁移到Ubuntu了。下面的东西可以按顺序看，最好看完再开始整，别看一点就整一点，很多需要注意的点。
### 最先需要的就是JDK
ubuntu系统所用的默认java是openjdk、jre之类的，我们不需要用它，最好别去碰它。最好别使用apt-get install来安装，麻烦很多。
到如下网址下载必要的JDK（一般在很显眼的地方有JDK字眼）：
`http://www.oracle.com/technetwork/java/javase/downloads/index.html`
全称一般是`Java SE Development Kit 8uxx`，比如我下载的就是`Java SE Development Kit 8u73`，它的下载链接是`http://download.oracle.com/otn-pub/java/jdk/8u73-b02/jdk-8u73-linux-x64.tar.gz`。
下载后解压：
`tar zxvf 文件名.tar.gz`
先为文件夹改个合适的名称（短一点），拉到一个合适的位置（比较短的前缀即可），然后添加以下3行指令到`/etc/profile`中：
```
export JAVA_HOME=你的JDK位置
export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH 
export CLASSPATH=$CLASSPATH:.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
```
当然这3行指令已经对所有用户都生效了，可以换其他方式弄成当前用户的，不过不碍事，算了不弄。
更新一下我们的修改，使得修改生效：
`source /etc/profile`
最后一步操作了，配置以下java环境，让系统使用我们的JDK而不是默认的openjdk。注意自己替换以下指令的后面部分的文件夹路径：
```
$ sudo update-alternatives --install /usr/bin/java java /JDK的路径/bin/java 300
$ sudo update-alternatives --install /usr/bin/javac javac /JDK的路径/bin/javac 300
$ sudo update-alternatives --config java 
$ sudo update-alternatives --config javac
```
确认一下已经安装完毕：
`java -version`
看到版本信息就完毕了。
### 其次是SDK
先到下面的站点找到合适的SDK下载下来：
`http://tools.android-studio.org/`
下载完毕后解压，进入解压后的文件夹tools，里面的android运行起来就是SDK啦。下载完必要的平台再搞studio。下载过程如果有问题（墙），请到如下网址了解：
`https://github.com/inferjay/AndroidDevTools#sdk-tools`
打开SDK先下载几个比较合适的platform，应该需要***，但是我并不需要，速度也有900k的样子。
### 安装android studio的方式有两种（择其一）
### 方式一
安装android studio很简单，只需要以下3条命令，但是是1.5的正式版：
```
sudo apt-add-repository ppa:paolorotolo/android-studio
sudo apt-get update
sudo apt-get install android-studio
```
### 方式二
方法一所安装的IDE比较旧的正式版，我不喜欢，想使用2.0 preview的版本。以下链接提供了多种下载方式，选择喜欢的下载即可。
`http://tools.android-studio.org/`
下载完毕之后，先解压。无需安装，只要JDK对了，切换到解压后的/bin目录下，直接敲：
`./studio.sh`
android studio 就启动起来了，此时就可以进行一些配置，在右下角的configure的地方。
![](https://images2015.cnblogs.com/blog/641737/201603/641737-20160305204225768-405209682.png)
如果还有点问题，可以看链接的FAQ：
`https://github.com/PaoloRotolo/android-studio#faq`
### genymotion模拟器
注意到此模拟器是需要依赖virtual box的，我的virtual box是直接在ubuntu软件重心下载的，搜一搜就可以了。
模拟器比较简单，先到官网下载程序下来（必须注册）：
`https://www.genymotion.com/pricing-and-licensing/`
下载完成之后竟然只是一个名为genymotion-2.6.0-linux_x64.bin的文件，得赋予它执行的权限，然后用root执行它：
`sudo ./genymotion-2.6.0-linux_x64.bin`
这个过程是全程自动的。
接着就是在/opt/genymobile/genymotion/中有一个可运行的genymotion，运行它就是模拟器了。
### 大功告成
我遇到的问题有如下几点：
- as初次打开创建项目后总停留在处理dependency相关的gradle处理界面
解决办法：如果不能联网的话它不会总是在处理。因为我设置了gradle的http和https代理，它就好像正在下载东西，但是又很慢的样子，而且很久后会失败。能取消的先取消，能停止的先停止，再去掉代理信息，信息就在该项目的local.properties文件中的底下几行，直接去掉即可。然后就奇迹般的搞定了。
- genymotion登录时老提示unknown generic error
解决办法：因为用的瑞捷上网，打开genymotion就会出现virtualbox的网卡，导致断网。到合适的wifi环境下，设置里面的proxy，再登录，直接搞定。
### apt-get出现失败的情况
任何安装失败可能导致dpkg工具不可以用了，总是提示xxx依赖问题不能解决。提供以下解决办法：
```
cd /var/lib/dpkg/info/
ls | grep "你的软件名" #查看到的哪些就是必须删除的
rm -rf /var/lib/dpkg/info/软件名* #根据软件名来删，别删错
sudo dpkg --remove --force-remove-reinstreq <准确软件名>
```
如果还搞不定，直接放大招，清空这些东西：
```
sudo mv /var/lib/dpkg/info   /var/lib/dpkg/info.bak #这是备份
sudo mkdir /var/lib/dpkg/info //再创建一个空的
```

