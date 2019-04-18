# linux(docker)中搭建zookeeper单机版(非常详细) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月19日 22:23:19[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：1045


首先 必备 两个压缩包、一个是jdk的、一个是zookeeper的

1：安装解压软件  yum -y install tar 

2 :  解压jdk   tar -zxvf  jdk压缩文件名

3 :  解压zookeeper    tar -zxvf zookeeper压缩文件名

4 :  给解压后的jdk和zookeeper文件夹名重命名为了后续配置方便    mv 原名 新名

5 :  进入etc下的profile文件夹 cd /etc/profile.d/ ，然后配置环境变量    vi java.sh     打开以后 输入以下内容                          

JAVA_HOME=jdk解压后所在的目录

CLASSPATH=$JAVA_HOME/lib:$CLASSPATH

PATH=$JAVA_HOME/bin:$PATH

export JAVA_HOME CLASSPATH PATH

保存 退出   esc 然后 :wq

6 :执行刚刚编写的shell脚本    source java.sh ，并 java -version 检验是否配置成功，出现java版本信息，表示配置成功

7 : 还在此文件夹下编写一个zookeeper的脚本(配置环境变量)   vi zk.sh 输入以下内容 

ZOOKEEPER=/soft/zk

PATH=$ZOOKEEPER/bin:$PATH

export ZOOKEEPER PATH

保存退出 esc 然后:wq

8 :进入到刚刚解压好的zookeeper文件夹 (zk是我的重命名后的文件夹名)  cd  /soft/zk/conf

里面有一个 zoo_sample.cfg  我们需要copy一份并改名 因为 默认的文件名是 zoo.cfg

cp zoo_sample.cfg  zoo.cfg

9 : 创建两个文件夹，一个存放数据，一个存放目录 

 mkdir -p /tmp/zookeeper/data

 mkdir -p /tmp/zookeeper/log 

10:进入刚刚我们copy的 文件 更改 

![](https://img-blog.csdn.net/20180719205502618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同时在标注的两行下面 有一个 ClientPort=2181，记住这个端口号。

保存退出

11：关闭防火墙    service iptables stop

![](https://img-blog.csdn.net/20180719210522257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后一个失败不用管。

12 : 在windows 操作系统下，打开命里提示符(管理员)，添加路由

输入  route add 172.17.0.0 mask 255.255.255.0 192.168.46.227

三组 数字分别为 

 172.17.0.0                           docker的ip地址前两部分加上两个 .0

 255.255.255.0                     网关

 192.168.46.227                   linux的ip地址

13 : 回到 linux操作系统  输入   zkServer.sh start    启动zookeeper服务

14 : 打开eclipse导入zookeeper包

       在pom文件中添加依赖，zookeeper的版本要与linux下的jar包版本一致，我用的3.4.8

   先导入服务端的，需要去掉slf4j、因为有两个日志jar包冲突

```
<dependency>
		    <groupId>org.apache.zookeeper</groupId>
		    <artifactId>zookeeper</artifactId>
		    <exclusions>
		    	<exclusion>
					<groupId>org.slf4j</groupId>
					<artifactId>slf4j-log4j12</artifactId>
		    	</exclusion>
		    </exclusions>
		</dependency>
```

 再导入客户端的

```
<dependency>
    <groupId>com.github.sgroschupf</groupId>
    <artifactId>zkclient</artifactId>
    <version>0.1</version>
</dependency>
```

测试

```java
//配置连接地址
		ZkClient client = new ZkClient("172.17.0.13:2181");
		//创建一个目录
		client.createPersistent("/hhh");
		//向这个目录中写信息
		client.writeData("/hhh", "fdasjkldfs");
		//从这个目录中读信息
		Object readData = client.readData("/hhh");
		//打印输出
		System.out.println(readData);
```

测试结果

![](https://img-blog.csdn.net/20180719222138779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

