# Ubuntu之JDK安装 - 零度的博客专栏 - CSDN博客
2017年06月09日 15:34:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：281标签：[ubuntu																[jdk](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Linux](https://blog.csdn.net/zmx729618/article/category/6225640)
1.从官网（[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)）下载安装文件(如：jdk-6u45-linux-x64.bin)。复制该文件到/usr/lib/java下，如果没有java文件夹，则创建它。
sudo mkdir  -p  /usr/lib/java
安装文件默认是没有执行权限的，所以首先第一步是要将安装文件赋予可执行权限，可以通过命令 $ sudo chmod u+x /usr/lib/java/jdk-6u45-linux-x64.bin来实现。
2、成功修改安装文件权限后，通过命令 $ cd /usr/lib/java/，进入java目录，然后执行命令 sudo ./jdk-6u33-linux-x64.bin ，屏幕将会出现安装进行树，最后需要输入回车，当按下回车键提示安装完成。当前目录下将会多出一个jdk1.6.0_45的目录,这也意味jdk已成功安装。
   注：如果下载是tar.gz包使用 sudo tar -zxvf jdk-8u131-linux-x64.tar.gz解压即可。
3、成功安装完JDK后，接下来我们将要进行JDK的配置，使用命令 sudo gedit /etc/environment对environment文件进行修改并添加以下代码：
export JAVA_HOME=/usr/lib/java/jdk1.6.0_45
export JRE_HOME=${JAVA_HOME}/jre  
export CLASSPATH=.:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib  
export PATH=$PATH:${JAVA_HOME}/bin
$ source /etc/environment     #使环境变量生效  
若PATH已存在，则用冒号作间隔，将jdk的 bin目录地址加上，这样java的环境变量将配置成功了，为了使默认使用的是我们安装的jdk，还需执行如下命令:
sudoupdate-alternatives --install /usr/bin/java java /usr/lib/java/jdk1.6.0_45/bin/java 300
sudo update-alternatives --install /usr/bin/javac javac/usr/lib/java/jdk1.6.0_45/bin/javac 300
然后执行通设置系统默认的JDK代码：
sudo update-alternatives --config java
4、成功执行命令，我们安装的JDK就是系统默认的了，然后执行命令java -version就可以成功看到JDK的相关信息：
java version "1.6.0_33"
java(TM)SE Runtime Environment (build 1.6.0_33-b04)
javaHotSpot(TM) 64-Bit Server VM (build 20.8-b03, mixed mode)
此时显示的系统中的java就是刚刚安装的java。
5、配置classpath，修改所有用户的环境变量
$ sudo gedit /etc/profile
在文件最后添加
#set java environment
export JAVA_HOME=/usr/lib/java/jdk1.6.0_45
export JRE_HOME=/usr/lib/java/jdk1.6.0_45/jre
export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/lib/dt.jar:$CLASSPATH 
export PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin
PS：以上红色部分依据你解压得到的版本来进行配置
（今天早上再次启动UBUNTU的时候发现了无法进入桌面，原因是启动的时候去加载这个变量，发生了错误，我在JAVA变量里多输入了一个字符，导致变量无法正确识别，就进不去桌面，停留在登录界面，解决方法如下：
1：开机到登录界面，切换到字符界面：CTRL+ALT+F1（或者CTRL+ALT+F2....） 进入字符模式，
2：用ROOT身份登录，
3：此时连VI命令都无法识别，需要先修改设置PATH路径:
export PATH=$PATH:/usr/bin:/usr/sbin:/etc:/usr/sbin:/usr/bin/X11:/sbin
4： 然后 vi /etc/environment  编辑为正确的变量保存，退出
5： reboot    即可解决）
6：写个HelloUbuntu测试下：
$sudo gedit HelloUbuntu.java
public class HelloUbuntu{
public static void main(String[] args){
System.out.println("HELLO Ubuntu 10.04 LTS ");
}
}
运行JAVA命令如下：
javac HelloUbuntu.java
java HelloUbuntu
------------------------------------------------
输出：HELLO Ubuntu 10.04 LTS
