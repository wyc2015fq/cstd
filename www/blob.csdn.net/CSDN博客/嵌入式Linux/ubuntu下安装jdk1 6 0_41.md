
# ubuntu下安装jdk1.6.0_41 - 嵌入式Linux - CSDN博客

2013年09月11日 15:28:44[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2542个人分类：[Linux																](https://blog.csdn.net/weiqifa0/article/category/1388863)



1、从网上下载jdk。看好自己的系统是32位还是64位的。用uname -m来查看。
[http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html\#jre-6u43-oth-JPR](http://www.oracle.com/technetwork/java/javasebusiness/downloads/java-archive-downloads-javase6-419409.html#jre-6u43-oth-JPR)
2、运行命令如下。
sudo chmod 777 jdk-6u41-linux-x64.bin
sudo -s ./jdk-6u41-linux-x64.bin /usr/lib/jdk               这个命令安装后。在usr/lib/jdk目录下没有那个文件。所以要复制过去。
sudo cp -r jdk1.6.0_41/ /usr/lib/jdk/
sudo update-alternatives --install /usr/bin/java java /usr/lib/jdk/jdk1.6.0_41/bin/java 300
sudo update-alternatives --config java
sudo update-alternatives --config javac
java -version

sudo gedit ~/.profile
在最后面加上：
export JAVA_HOME=/usr/lib/jdk/jdk1.6.0_41
export  PATH=$JAVA_HOME/bin:$PATH
export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib
source /etc/envrionment              更新做的修改或者重启系统

30  sudo apt-get install vim   安装vim
31  sudo vim helloworld.java    新建测试java程序

```python
public class HelloWorld{
   public static void main(String args[]){
       System.out.println("Hello World in Ubuntu!!");
   }
}
```
32  ls
33  mv helloworld.java HelloWorld.java
34  ls
35  javac HelloWorld.java
java HelloWorld
36  java -version
37  . /etc/environment
38  java -version
39  javac HelloWorld.java





