# CodeSmith连Oracle - 左直拳的马桶_日用桶 - CSDN博客
2016年08月26日 21:00:13[左直拳](https://me.csdn.net/leftfist)阅读数：2584
据说CodeSmith连Oracle特别麻烦，什么WIN7下不行，64位下不行。之前有个同事为了用上CodeSmith，还特地装了个XP虚拟机。
其实，还是那个连接串的问题。
操作系统64位，就要用64位的驱程。但我们机器上装的是oracle 10g，一个不分64、32位的混蛋，其实就是32位的。反正我们本机出发的连接串，要靠这个32位的oracle客户端来解释。如果服务器的oracle是64位的11g甚至12c,它就没办法解释了。
但是，这个连接串为啥要它来解释？
看看这个连接串，酱紫写的：
`DATA SOURCE=PDBGZFBC;PASSWORD=test;PERSIST SECURITY INFO=True;USER ID=123;`
DATA SOURCE=PDBGZFBC；啥是PDBGZFBC？噢，它是在我机器上32位的oracle 10g客户端那里定义的：
![这里写图片描述](https://img-blog.csdn.net/20160826205445966)
![这里写图片描述](https://img-blog.csdn.net/20160826205605170)
主要是靠10g客户端来解释这个串。其实我们可以直接写在连接串里，无须经过它解释。所以，酱紫写连接串：
```
Server=(DESCRIPTION =
    (ADDRESS_LIST =
      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.22)(PORT = 1522))
    )
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = GZFBC)
    )
  );PASSWORD=test;PERSIST SECURITY INFO=True;USER ID=123;
```
codesmith可以连oracle矣。
============================================ 
原来不全是这样的。64位操作系统仍然需要64位的oracle客户端支持，否则还是不行。
> 
1、首先要有oracle 64位客户端 
  2、CodeSmith要使用这个64位的客户端
首先说第一点。我都忘了自己是怎么装这个客户端的。似乎是不用安装，直接将这个客户端压缩包解压后放在硬盘上即可。比如：
`C:\instantclient-basic-windows.x64-12.1.0.2.0\instantclient_12_1`
然后，怎么才能让CodeSmith使用这个客户端？答案是在系统的PATH里加上这个客户端的路径，并且要加在oracle 32位客户端的前面。这样，codesmith找到这个64位的oci.dll以后，就会心满意足地忽略后面那个32位客户端了。比如路径设置如下：
`PATH=C:\instantclient-basic-windows.x64-12.1.0.2.0\instantclient_12_1;E:\oracle\product\10.2.0\db_1\bin;`
“E:\oracle\product\10.2.0\db_1\bin”就是oracle 10g的安装路径，32位的。
