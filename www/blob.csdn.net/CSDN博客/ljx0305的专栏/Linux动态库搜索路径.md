# Linux动态库搜索路径 - ljx0305的专栏 - CSDN博客
2009年07月16日 10:27:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：525标签：[linux																[library																[path																[gcc																[file																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[linux下软件安装																[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/440323)
/usr/lib/usr/lib/libpos.so默认的动态库搜索路径/usr/lib
表1: 程序pos输出结果和动态库的对应关系
创建各个动态库，并放置在相应的目录中。测试环境就准备好了。执行程序pos，并在该命令行中设置环境变量LD_LIBRARY_PATH。 
# LD_LIBRARY_PATH=/root/test/env/lib ./pos  ./
      #
根据程序pos的输出结果可知，最先搜索的是编译目标代码时指定的动态库搜索路径。然后我们把动态库./libpos.so删除了，再运行上述命令试试。
# rm libpos.so
        rm: remove regular file `libpos.so'? y
      # LD_LIBRARY_PATH=/root/test/env/lib ./pos /root/test/env/lib
      #
根据程序pos的输出结果可知，第2个动态库搜索的路径是环境变量LD_LIBRARY_PATH指定的。我们再把/root/test/env/lib/libpos.so删除，运行上述命令。
# rm /root/test/env/lib/libpos.so
        rm: remove regular file `/root/test/env/lib/libpos.so'? y
      # LD_LIBRARY_PATH=/root/test/env/lib ./pos  /root/test/conf/lib
      #
第3个动态库的搜索路径是配置文件/etc/ld.so.conf指定的路径。删除动态库/root/test/conf/lib/libpos.so后再运行上述命令。
# rm /root/test/conf/lib/libpos.so
        rm: remove regular file `/root/test/conf/lib/libpos.so'? y
      # LD_LIBRARY_PATH=/root/test/env/lib ./pos  /lib
      #
第4个动态库的搜索路径是默认搜索路径/lib。我们再删除动态库/lib/libpos.so，运行上述命令。
# rm /lib/libpos.so
        rm: remove regular file `/lib/libpos.so'? y
      # LD_LIBRARY_PATH=/root/test/env/lib ./pos  /usr/lib
      #
最后的动态库搜索路径是默认搜索路径/usr/lib。 
综合以上结果可知，动态库的搜索路径搜索的先后顺序是： 
1.编译目标代码时指定的动态库搜索路径； 
2.环境变量LD_LIBRARY_PATH指定的动态库搜索路径； 
3.配置文件/etc/ld.so.conf中指定的动态库搜索路径； 
4.默认的动态库搜索路径/lib； 
5.默认的动态库搜索路径/usr/lib。 
在上述1、2、3指定动态库搜索路径时，都可指定多个动态库搜索路径，其搜索的先后顺序是按指定路径的先后顺序搜索的。对此本文不再举例说明，有兴趣的读者可以参照本文的方法验证。
原文:[http://www.yuanma.org/data/2007/1212/article_2923.htm](http://www.yuanma.org/data/2007/1212/article_2923.htm)
