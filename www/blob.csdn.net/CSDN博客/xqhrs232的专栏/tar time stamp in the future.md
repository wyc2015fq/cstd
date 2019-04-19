# tar:time stamp in the future - xqhrs232的专栏 - CSDN博客
2016年10月20日 15:01:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：515
原文地址::[http://blog.csdn.net/caspiansea/article/details/17700165](http://blog.csdn.net/caspiansea/article/details/17700165)
相关文章
1、
# [tar时提示time stamp in the future](http://blog.csdn.net/u012700515/article/details/31413655)----[http://blog.csdn.net/u012700515/article/details/31413655](http://blog.csdn.net/u012700515/article/details/31413655)
# 2、[tar:time stamp in the future](http://blog.chinaunix.net/uid-354915-id-4382661.html) ----[http://blog.chinaunix.net/uid-354915-id-4382661.html](http://blog.chinaunix.net/uid-354915-id-4382661.html)

如果解压一个拷贝自一个系统时间在当前系统之后的 包，会提示如题目这样。
**[plain]**[view
 plain](http://blog.csdn.net/caspiansea/article/details/17700165#)[copy](http://blog.csdn.net/caspiansea/article/details/17700165#)
[print](http://blog.csdn.net/caspiansea/article/details/17700165#)[?](http://blog.csdn.net/caspiansea/article/details/17700165#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/135645/fork)
- tar: openssl-1.0.1e/apps/openssl.cnf: time stamp 2013-12-31 15:26:04 is 1360584537.139999998 s in the future  
- openssl-1.0.1e/apps/openssl-vms.cnf  
- tar: openssl-1.0.1e/apps/openssl-vms.cnf: time stamp 2013-12-31 15:26:04 is 1360584537.129999998 s in the future  
- openssl-1.0.1e/apps/passwd.c  
可以用 -m参数来把文件的时间（modify time)设置为当前系统的时间。
**[plain]**[view
 plain](http://blog.csdn.net/caspiansea/article/details/17700165#)[copy](http://blog.csdn.net/caspiansea/article/details/17700165#)
[print](http://blog.csdn.net/caspiansea/article/details/17700165#)[?](http://blog.csdn.net/caspiansea/article/details/17700165#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/135645/fork)
- tar  zxmvf openssl-1.0.1e.tar.gz  
还有一种方法是解压后修改每个文件的时间：
**[plain]**[view
 plain](http://blog.csdn.net/caspiansea/article/details/17700165#)[copy](http://blog.csdn.net/caspiansea/article/details/17700165#)
[print](http://blog.csdn.net/caspiansea/article/details/17700165#)[?](http://blog.csdn.net/caspiansea/article/details/17700165#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/135645/fork)
- mkdir foo  
- mv openssl-1.0.1e.tar.gz foo/  
- cd foo/  
- tar xvvzf openssl-1.0.1e.tar.gz  
- find . -exec touch -am '{}' \;  
- 




