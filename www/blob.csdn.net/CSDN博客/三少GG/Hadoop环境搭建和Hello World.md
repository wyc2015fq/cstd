# Hadoop环境搭建和Hello World - 三少GG - CSDN博客
2013年08月23日 11:08:42[三少GG](https://me.csdn.net/scut1135)阅读数：2062

安装过程：
**************************************************************
一、安装Linux操作系统                                        *
二、在Ubuntu下创建hadoop用户组和用户           *
三、在Ubuntu下安装JDK                                      *
四、修改机器名
五、安装ssh服务
六、建立ssh无密码登录本机
七、安装hadoop
八、在单机上运行hadoop
***************************************************************
**二、在Ubuntu下创建hadoop用户组和用户**
这里考虑的是以后涉及到hadoop应用时，专门用该用户操作。用户组名和用户名都设为：hadoop。可以理解为该hadoop用户是属于一个名为hadoop的用户组，这是linux操作系统的知识，如果不清楚可以查看linux相关的书籍。
1、创建hadoop用户组，如图（3）
![](http://dl.iteye.com/upload/attachment/0062/9806/fa6b488c-9b4d-3341-a786-7bb34ae818c7.png)
2、创建hadoop用户，如图（4）
![](http://dl.iteye.com/upload/attachment/0062/9810/f0554440-2f21-3d3b-bee1-b819bf402861.png)
3、给hadoop用户添加权限，打开/etc/sudoers文件，如图（5）
![](http://dl.iteye.com/upload/attachment/0062/9812/4a78d42f-622b-32cd-b622-f3c4602df056.png)
按回车键后就会打开/etc/sudoers文件了，给hadoop用户赋予root用户同样的权限。在root   ALL=(ALL:ALL)   ALL下添加hadoop   ALL=(ALL:ALL)  ALL，如图（6）
![](http://dl.iteye.com/upload/attachment/0062/9814/00c4ea19-ff04-3b9f-8af7-c7173b5e37d9.png)
**三、在Ubuntu下安装JDK（[http://weixiaolu.iteye.com/blog/1401786](http://weixiaolu.iteye.com/blog/1401786)） 四、修改机器名**
每当ubuntu安装成功时，我们的机器名都默认为：ubuntu ，但为了以后集群中能够容易分辨各台服务器，需要给每台机器取个不同的名字。机器名由 /etc/hostname文件决定。
1、打开/etc/hostname文件，如图（7）
![](http://dl.iteye.com/upload/attachment/0062/9816/3546515c-fbca-3679-8b52-3b087897c763.png)
2、回车后就打开/etc/hostname文件了，将/etc/hostname文件中的ubuntu改为你想取的机器名。这里我取“s15“。重启系统后才会生效。
**五、安装ssh服务**
这里的ssh和三大框架:spring,struts,hibernate没有什么关系，ssh可以实现远程登录和管理，具体可以参考其他相关资料。
1、安装openssh-server，如图（8）
![](http://dl.iteye.com/upload/attachment/0062/9820/bca8c79d-6988-3d7e-ad1a-187276f750cc.png)
（注：自动安装openssh-server时，可能会进行不下去，可以先进行如下操作：)
![](http://dl.iteye.com/upload/attachment/0062/9822/dbeadcef-3a3f-31bf-834c-8bfade93d43e.png)
2、更新的快慢取决于您的网速了，如果中途因为时间过长您中断了更新（Ctrl+z），当您再次更新时，会更新不了，报错为：“Ubuntu无法锁定管理目录（/var/lib/dpkg/），是否有其他进程占用它？“需要如下操作，如图（10）
![](http://dl.iteye.com/upload/attachment/0062/9824/6acf87ab-a59e-3b9d-88e4-88e032f9b07e.png)
操作完成后继续执行第1步。
这时假设您已经安装好了ssh，您就可以进行第六步了哦~
**六、 建立ssh无密码登录本机**
ssh生成密钥有rsa和dsa两种生成方式，默认情况下采用rsa方式。
1、创建ssh-key，，这里我们采用rsa方式，如图（11）
![](http://dl.iteye.com/upload/attachment/0062/9826/660298b1-bd18-3be3-bc08-a6a69bad5758.png)
（注：回车后会在~/.ssh/下生成两个文件：id_rsa和id_rsa.pub这两个文件是成对出现的）
2、进入~/.ssh/目录下，将id_rsa.pub追加到authorized_keys授权文件中，开始是没有authorized_keys文件的，如图（12）
![](http://dl.iteye.com/upload/attachment/0062/9828/d8dc62de-85e3-3c75-9a11-5ae866b0cffc.png)
（完成后就可以无密码登录本机了。）
3、登录localhost，如图（13）
![](http://dl.iteye.com/upload/attachment/0062/9830/d5bd8515-0064-3905-9140-ec47196b2543.png)
( 注：当ssh远程登录到其它机器后，现在你控制的是远程的机器，需要执行退出命令才能重新控制本地主机。)
4、执行退出命令，如图（14）
![](http://dl.iteye.com/upload/attachment/0062/9831/9a6b5eb4-5122-32e8-88d8-168f9881b720.png)
**七、安装hadoop**
我们采用的hadoop版本是：hadoop-0.20.203（[http://apache.etoak.com/hadoop/common/hadoop-0.20.203.0/](http://apache.etoak.com/hadoop/common/hadoop-0.20.203.0/) ），因为该版本比较稳定。
1、假设hadoop-0.20.203.tar.gz在桌面，将它复制到安装目录 /usr/local/下，如图（15）
![](http://dl.iteye.com/upload/attachment/0062/9834/dc07213d-e8b2-3b51-bbc7-40d3efe1c0ec.png)
2、解压hadoop-0.20.203.tar.gz，如图（16）
![](http://dl.iteye.com/upload/attachment/0062/9836/68d12023-2165-370d-ba44-c2a76b2ceed9.png)
3、将解压出的文件夹改名为hadoop，如图（17）
![](http://dl.iteye.com/upload/attachment/0062/9838/ad0d0b72-efb4-3e12-87c9-c4d27b70ad31.png)
4、将该hadoop文件夹的属主用户设为hadoop，如图（18）
![](http://dl.iteye.com/upload/attachment/0062/9840/8e7d952a-40d6-39c1-b949-683cb75bce67.png)
5、打开hadoop/conf/hadoop-env.sh文件，如图（19）
![](http://dl.iteye.com/upload/attachment/0062/9843/dab2cdaa-cc22-3645-9d32-5fb73c3e4716.png)
6、配置conf/hadoop-env.sh（找到#export JAVA_HOME=...,去掉#，然后加上本机jdk的路径）
![](http://dl.iteye.com/upload/attachment/0062/9842/9d009733-6325-34e6-9ae7-3a292195df44.png)
7、打开conf/core-site.xml文件，编辑如下：
Java代码  ![收藏代码](http://weixiaolu.iteye.com/images/icon_star.png)
- <?xml version="1.0"?>  
- <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>  
- 
- <!-- Put site-specific property overrides in this file. -->  
- 
- <configuration>  
- <property>    
-   <name>fs.default.name</name>    
-   <value>hdfs://localhost:9000</value>   
-  </property>    
- </configuration>     
8、打开conf/mapred-site.xml文件，编辑如下：
Java代码  ![收藏代码](http://weixiaolu.iteye.com/images/icon_star.png)
- <?xml version="1.0"?>  
- <?xml-stylesheet type="text/xsl" href="configuration.xsl"?>  
- 
- <!-- Put site-specific property overrides in this file. -->  
- 
-     <configuration>    
-      <property>     
-       <name>mapred.job.tracker</name>    
-       <value>localhost:9001</value>     
-      </property>    
-     </configuration>    
9、打开conf/hdfs-site.xml文件，编辑如下：
Java代码  ![收藏代码](http://weixiaolu.iteye.com/images/icon_star.png)
- <configuration>  
- <property>  
- <name>dfs.name.dir</name>  
- <value>/usr/local/hadoop/datalog1,/usr/local/hadoop/datalog2</value>  
- </property>  
- <property>  
- <name>dfs.data.dir</name>  
- <value>/usr/local/hadoop/data1,/usr/local/hadoop/data2</value>  
- </property>  
- <property>  
- <name>dfs.replication</name>  
- <value>1</value>  
- </property>  
- </configuration>  
10、打开conf/masters文件，添加作为secondarynamenode的主机名，作为单机版环境，这里只需填写localhost就Ok了。
11、打开conf/slaves文件，添加作为slave的主机名，一行一个。作为单机版，这里也只需填写localhost就Ok了。
**八、在单机上运行hadoop**
1、进入hadoop目录下，格式化hdfs文件系统，初次运行hadoop时一定要有该操作，如图（21）
![](http://dl.iteye.com/upload/attachment/0062/9846/20e98b30-370d-3faf-a4fa-5e7dd3f84d0a.png)
当你看到下图时，就说明你的hdfs文件系统格式化成功了。
![](http://dl.iteye.com/upload/attachment/0062/9848/e224d34a-44bf-33c2-900d-887851f13f36.png)
3、启动bin/start-all.sh，如图（23）
![](http://dl.iteye.com/upload/attachment/0062/9850/8a8721e1-cfb7-3248-a1fa-630dde729f72.png)
4、检测hadoop是否启动成功，如图（24）
![](http://dl.iteye.com/upload/attachment/0062/9852/c133beda-84b7-3268-a985-4fb7f059c05d.png)
如果有Namenode，SecondaryNameNode，TaskTracker，DataNode，JobTracker五个进程，就说明你的hadoop单机
版环境配置好了，呵呵，多么宏伟的工程呀！
**九、 Linux下的快捷键：**
Ctrl+Alt+t：弹出终端
Ctrl+空格：中英文输入法切换
*************************************************************************************************************************
WordCount示例：
新建几个文件夹：cd hadoop-1.0.0
mkdir playhadoop
mkdir playhadoop/src
mkdir playhadoop/class
编译：
cp ./src/examples/org/apache/hadoop/examples/WordCount.java ./playhadoop/src
cd playhadoop
建立一个编译脚本
vim compile.sh
javac -classpath ../hadoop-core-1.0.0.jar:../lib/commons-cli-1.2.jar -d class/ src/WordCount.java
chmod a+x compile.sh
./compileme.sh
4, 打成jar包
jar -cvf wordcount.jar -C class/ .
5,运行
#在hadoop分布式文件系统下建立一个目录input
./bin/hadoop fs -mkdir input
#从本地文件系统中复制一个要处理的文件到hadoop分布式文件系统中：
./bin/hadoop fs -copyFromLocal ./idata  input
#运行
./bin/hadoop jar playhadoop/wordcount.jar org.apache.hadoop.examples.WordCount input/idata output
注意：这里不要创建output文件夹，输出文件夹会自己创建
查看一下output文件夹
./bin/hadoop fs -ls output
Found 3 items
-rw-r--r--   2 hadoop supergroup          0 2012-04-28 22:08 /user/hadoop/output/_SUCCESS
drwxr-xr-x   - hadoop supergroup          0 2012-04-28 22:07 /user/hadoop/output/_logs
-rw-r--r--   2 hadoop supergroup       1306 2012-04-28 22:08 /user/hadoop/output/part-r-00000
这个 part-r-00000 文件中就是结果
cat一下这个文件：
./bin/hadoop fs -cat output/part-r-00000
有如下输出：
performing      1
permitted.      1
please  2
policies        1
possession,     2
project 1
provides        1
re-export       2
regulations     1
reside  1
restrictions    1
security        1
see     1
software        2
software,       2
software.       2
software:       1
*****************************************************************************************************************************************************************
**error:**
```java
```
hduser@saket-K53SM:/usr/local/hadoop$ jps
  The program 'jps' can be found in the following packages:
  * openjdk-6-jdk
  * openjdk-7-jdk
 Try: sudo apt-get install <selected package>
```
```
solution:
That is actually not a Hadoop problem. Hadoop does not use JPS.
If JPS can't be found, you have to put it into your path or create an alias. The JPS executable can be found under `$JAVA_HOME/bin/jps`.
The alias for example could be:
```java
`alias jps='/usr/lib/jvm/jdk1.6.0_43/bin/jps'`
```
Or if you don't care about using JPS, you could instead do a
```java
`ps aux | grep java`
```
which will approx. give you the same result ;)
