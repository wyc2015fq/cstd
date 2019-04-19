# oracle 10g 实例用localhost无法访问的处理 - 左直拳的马桶_日用桶 - CSDN博客
2015年08月27日 19:08:13[左直拳](https://me.csdn.net/leftfist)阅读数：1707
我在笔记本上安装了一个Oracle10g，安装好了之后，查看E:\oracle\product\10.2.0\db_1\network\ADMIN\tnsnames.ora文件，发现SID对应的IP地址是我无线网卡的IP地址：
```
PRIMETON =
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.10)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = primeton)
    )
  )
```
靠，这个IP是动态分配的，变了怎么办？就不能改成localhost？我手动改为localhost，结果用plsql developer死活连不上；127.0.0.1，也不行；local，也不行。
“其实可以的”，西蒙说。
只要添加一些监听就行了。
打开 Oracle 10g - 配置移植工具 - Net Manager - 本地 - 监听程序，将localhost、127.0.0.1加进去就可以了 
![这里写图片描述](https://img-blog.csdn.net/20150827190558856)
设置完成之后，记得重启服务里的oracle监听程序，设置才会生效
