# （转载）Hadoop2.7.1配置 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年01月04日 16:58:07[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2373
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)









#### http://my.oschina.net/laigous/blog/478438

#### hadoop伪分布式搭建


    其实官网有比较详实的说明，英语好的可以直接看官网，[地址](http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-common/SingleCluster.html)


#####     1、安装JDK1.7


        这个省略，官网显示1.6可以，但是我用openjdk1.6出了异常，JDK1.6没试，直接用了JDK1.7



        配置好环境变量





[?](http://my.oschina.net/laigous/blog/478438#)

```
`vi``/etc/profile`
```





[?](http://my.oschina.net/laigous/blog/478438#)

```
`export``JAVA_HOME=``/usr/local/jdk1``.7.0_79`

`export``CLASSPATH=.:$JAVE_HOME``/lib``.tools.jar`

`export``PATH=$PATH:$JAVA_HOME``/bin`
```




   添加完后执行命令使配置生效



[?](http://my.oschina.net/laigous/blog/478438#)

```
`source``/etc/profile`
```



#####     2、安装ssh、rsync,（以ubuntu为例）



[?](http://my.oschina.net/laigous/blog/478438#)

```
`  ``$ ``sudo``apt-get ``install``ssh`

`  ``$ ``sudo``apt-get ``install``rsync`
```



#####     3、下载hadoop编译包，[镜像地址](http://www.apache.org/dyn/closer.cgi/hadoop/common/)（国内镜像竟然比US的还慢，不能忍，注意2.7的编译包为64位）


    查是32还是64位的办法



[?](http://my.oschina.net/laigous/blog/478438#)

```
`cd``hadoop-2.7.0``/lib/native`

`file``libhadoop.so.1.0.0`
```





[?](http://my.oschina.net/laigous/blog/478438#)

```
`hadoop-2.7.0``/lib/native/libhadoop``.so.1.0.0: ELF 64-bit LSB shared object, AMD x86-64, version 1 (SYSV), not stripped`
```



#####     4、配置环境变量


    hadoop配置文件指定java路径


etc/hadoop/hadoop-env.sh



[?](http://my.oschina.net/laigous/blog/478438#)

```
`export``JAVA_HOME=``/usr/local/jdk1``.7.0_79`
```



  系统环境变量



[?](http://my.oschina.net/laigous/blog/478438#)

```
`export``HADOOP_HOME=``/usr/local/hadoop-2``.7.0`

`export``PATH=$PATH:$HADOOP_HOME``/bin`

`export``HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_HOME``/lib/native`

`export``HADOOP_OPTS=``"-Djava.library.path=$HADOOP_HOME/lib"`
```




  后两条不加会出现


You have loaded library /usr/hadoop/hadoop-2.7.0/lib/native/libhadoop.so.1.0.0 which might have disabled stack guard. The VM will try to fix the stack guard now.


It's highly recommended that you fix the library with 'execstack -c <libfile>', or link it with '-z noexecstack'.


  添加完后执行命令使配置生效



[?](http://my.oschina.net/laigous/blog/478438#)

```
`source``/etc/profile`
```




  执行命令查看是否成功



[?](http://my.oschina.net/laigous/blog/478438#)

```
`hadoop version`
```



#####     5、修改hadoop配置文件 

etc/hadoop/core-site.xml:



[?](http://my.oschina.net/laigous/blog/478438#)

```
```xml
<
```

```xml
configuration
```

```xml
>
```



```xml

```

```xml
<
```

```xml
property
```

```xml
>
```



```xml

```

```xml
<
```

```xml
name
```

```xml
>fs.defaultFS</
```

```xml
name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
value
```

```xml
>hdfs://localhost:9000</
```

```xml
value
```

```xml
>
```



```xml

```

```xml
</
```

```xml
property
```

```xml
>
```



```xml
</
```

```xml
configuration
```

```xml
>
```
```



 
    etc/hadoop/hdfs-site.xml:



[?](http://my.oschina.net/laigous/blog/478438#)

```
```xml
<
```

```xml
configuration
```

```xml
>
```



```xml

```

```xml
<
```

```xml
property
```

```xml
>
```



```xml

```

```xml
<
```

```xml
name
```

```xml
>dfs.replication</
```

```xml
name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
value
```

```xml
>1</
```

```xml
value
```

```xml
>
```



```xml

```

```xml
</
```

```xml
property
```

```xml
>
```



```xml
</
```

```xml
configuration
```

```xml
>
```
```



#####      6、设置hadoop ssh无密码访问



[?](http://my.oschina.net/laigous/blog/478438#)

```
`  ``$ ``ssh``-keygen -t dsa -P ``''``-f ~/.``ssh``/id_dsa`

`  ``$ ``cat``~/.``ssh``/id_dsa``.pub >> ~/.``ssh``/authorized_keys`

`  ``$ ``export``HADOOP\_PREFIX=``/usr/local/hadoop-2``.7.0`
```



#####     7、格式化节点和启动hadoop



[?](http://my.oschina.net/laigous/blog/478438#)

```
`$ bin``/hdfs``namenode -``format`

`$ sbin``/start-dfs``.sh`
```




    打开浏览器http://localhost:50070/看是否成功


    hdfs配置：username最好和当前用户名相同，不然会可能出现权限问题




[?](http://my.oschina.net/laigous/blog/478438#)

```
`  ``$ bin``/hdfs``dfs -``mkdir``/user`

`  ``$ bin``/hdfs``dfs -``mkdir``/user/``<username>`
```



#####      8、yarn配置


     etc/hadoop/mapred-site.xml:



[?](http://my.oschina.net/laigous/blog/478438#)

```
```xml
<
```

```xml
configuration
```

```xml
>
```



```xml

```

```xml
<
```

```xml
property
```

```xml
>
```



```xml

```

```xml
<
```

```xml
name
```

```xml
>mapreduce.framework.name</
```

```xml
name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
value
```

```xml
>yarn</
```

```xml
value
```

```xml
>
```



```xml

```

```xml
</
```

```xml
property
```

```xml
>
```



```xml
</
```

```xml
configuration
```

```xml
>
```
```



    etc/hadoop/yarn-site.xml:



[?](http://my.oschina.net/laigous/blog/478438#)

```
```xml
<
```

```xml
configuration
```

```xml
>
```



```xml

```

```xml
<
```

```xml
property
```

```xml
>
```



```xml

```

```xml
<
```

```xml
name
```

```xml
>yarn.nodemanager.aux-services</
```

```xml
name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
value
```

```xml
>mapreduce_shuffle</
```

```xml
value
```

```xml
>
```



```xml

```

```xml
</
```

```xml
property
```

```xml
>
```



```xml
</
```

```xml
configuration
```

```xml
>
```
```



    启动yarn




[?](http://my.oschina.net/laigous/blog/478438#)

```
`$ sbin``/start-yarn``.sh`
```



http://localhost:8088/查看是否成功


    至此hadoop单节点伪分布式安装配置完成



