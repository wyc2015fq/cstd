# oracle连接串的一种写法 - 左直拳的马桶_日用桶 - CSDN博客
2016年07月15日 20:22:26[左直拳](https://me.csdn.net/leftfist)阅读数：5755
我在.NET项目里访问oracle，向来是规规矩矩地这样写：
`DATA SOURCE=PDBGZFBC;PASSWORD=test;PERSIST SECURITY INFO=True;USER ID=123;`
那么这个 PDBGZFBC 只是一个名称，它定义在oracle客户端的文件tnsnames.ora里：
```
PDBGZFBC =
  (DESCRIPTION =
    (ADDRESS_LIST =
      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.22)(PORT = 1522))
    )
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = GZFBC)
    )
  )
```
我对oracle并不十分了解，虽然许多年以前，有用过一两年，但当时也不怎么懂，记性又不好，早忘得光当当的了。去年来到现在公司以后，反正见大家都这么写，也很乖地一直这么写。认为天经地义。
因为这次开发win form桌面程序，也同样需要访问oracle。问题是，本机操作系统64位，而oracle客户端是32位，在没有IIS支持的情况下，只好下载了64位的OCI.dll来支撑。但是，tnsnames.ora是属于32位oracle客户端的，所以，上面这个连接串 
`DATA SOURCE=PDBGZFBC;PASSWORD=test;PERSIST SECURITY INFO=True;USER ID=123;`
在运行的时候就报错了。
是否我！真的要装一个64位的oracle客户端？！
在网上寻寻觅觅，得到灵感。直接将连接串改成酱紫：
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
呵呵，搞定。
