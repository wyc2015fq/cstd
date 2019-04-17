# tftp在嵌入式中应用 - V__KING__的专栏 - CSDN博客





2017年01月11日 09:22:28[v__king__](https://me.csdn.net/V__KING__)阅读数：467








# tftp在嵌入式中应用

tftp在一般的linux系统中都有，但是在一些低配置中，可能没有scp，ftp这些工具，只有tftp工具。 
[http://blog.sina.com.cn/s/blog_7d7e9d0f0101ifoz.html](http://blog.sina.com.cn/s/blog_7d7e9d0f0101ifoz.html)

1. tftp服务在windows上(当然linux下肯定也是有的) 

    下载windows下的tftp软件： tftppd32 

2. arm9板子上不跑tftp服务器而是作为客户端来用的，可能是资源limit原因吧 

```
tftp -g -r 806_irp_llrp 192.168.1.73 

    tftp -p -r 806_irp_llrp 192.168.1.73 






# ssh远程执行



[http://www.jb51.net/article/64229.htm](http://www.jb51.net/article/64229.htm)
```




