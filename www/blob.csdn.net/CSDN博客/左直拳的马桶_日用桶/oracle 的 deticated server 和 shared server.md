# oracle 的 deticated server 和 shared server - 左直拳的马桶_日用桶 - CSDN博客
2018年10月20日 11:28:47[左直拳](https://me.csdn.net/leftfist)阅读数：84
在配置oracle的tsname的时候，发觉有的有这个"SERVER = DEDICATED"，有的又没有。如下：
```
pdbnhzhzs =
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.231)(PORT = 1521))
    (CONNECT_DATA =
      (SERVICE_NAME = pdbnhzhzs)
    )
  )
JZCWHD =
  (DESCRIPTION =
    (ADDRESS_LIST =
      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.22)(PORT = 1521))
    )
    (CONNECT_DATA =
      (SERVER = DEDICATED) #<----------------------------
      (SERVICE_NAME = JZCWHD)
    )
  )
```
这是啥东东？
我的英语很差，当年考英语四级，考了三次，最后才以60分勉强通过。但搞IT，焉能不懂英语，所以我也装一回逼，装模作样的看了下oracle的文档。以后也尽量先看英文文档。果然，不大懂，先记录一下，以后再从此出发。
这应该是oracle对数据库的两种访问方式。顾名思义，一个是专有访问（默认），一个是共享访问。所谓专有和共享，大概是对资源的利用吧，将各种资源加载到内存里。
1、专有方式
![在这里插入图片描述](https://img-blog.csdn.net/20181020112352730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、共享方式
![在这里插入图片描述](https://img-blog.csdn.net/20181020112401594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、两种方式比较
我猜，
1）专有方式性能总体来说比较好，但早期加载东西可能要耗点时间；处理起来也比较简单
2）共享方式稍复杂，但资源利用率高，也较节省
通常，像“(SERVER = DEDICATED) ”这种方式是不需要写的。但当数据库是共享访问方式，但你又想以“专有方式”来访问，就要声明了。
参考文章：
[https://docs.oracle.com/cd/B28359_01/server.111/b28310/manproc001.htm#ADMIN11166](https://docs.oracle.com/cd/B28359_01/server.111/b28310/manproc001.htm#ADMIN11166)
