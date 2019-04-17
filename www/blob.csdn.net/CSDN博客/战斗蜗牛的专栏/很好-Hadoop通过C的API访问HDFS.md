# 很好-Hadoop通过C的API访问HDFS - 战斗蜗牛的专栏 - CSDN博客





2017年04月28日 14:01:09[vbskj](https://me.csdn.net/vbskj)阅读数：494









在通过Hadoop的C的API 访问HDFS的时候，编译和运行出现了不少问题，在这边，做个总结吧：



系统：Ubuntu11.04，Hadoop-0.20.203.0


样例代码就是参考官方文档中提供到：






**[cpp]**[view
 plain](http://blog.csdn.net/deqingguo/article/details/7004898#)[copy](http://blog.csdn.net/deqingguo/article/details/7004898#)



- #include "hdfs.h" 
- 
- int main(int argc, char **argv) {  
- 
-     hdfsFS fs = hdfsConnect("default", 0);  
- constchar* writePath = "/tmp/testfile.txt";  
-     hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY|O_CREAT, 0, 0, 0);  
- if(!writeFile) {  
-           fprintf(stderr, "Failed to open %s for writing!\n", writePath);  
-           exit(-1);  
-     }  
- char* buffer = "Hello, World!";  
-     tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);  
- if (hdfsFlush(fs, writeFile)) {  
-            fprintf(stderr, "Failed to 'flush' %s\n", writePath);   
-           exit(-1);  
-     }  
-    hdfsCloseFile(fs, writeFile);  
- }  


编译：官网这样描述



See the Makefile for hdfs_test.c in the libhdfs source directory (${HADOOP_HOME}/src/c++/libhdfs/Makefile) or something like:

gcc above_sample.c -I${HADOOP_HOME}/src/c++/libhdfs -L${HADOOP_HOME}/libhdfs -lhdfs -o above_sample


但是我两个方法都试了，不行，后面发现原来是要少了：





**[plain]**[view
 plain](http://blog.csdn.net/deqingguo/article/details/7004898#)[copy](http://blog.csdn.net/deqingguo/article/details/7004898#)



- LIB = -L$(HADOOP_INSTALL)/c++/Linux-i386-32/lib/  
- libjvm=/usr/lib/jvm/java-6-openjdk/jre/lib/i386/client/libjvm.so  





所以完整到makefile就是：






**[plain]**[view
 plain](http://blog.csdn.net/deqingguo/article/details/7004898#)[copy](http://blog.csdn.net/deqingguo/article/details/7004898#)



- HADOOP_INSTALL=/home/fzuir/hadoop-0.20.203.0  
- PLATFORM=Linux-i386-32  
- JAVA_HOME=/usr/lib/jvm/java-6-openjdk/  
- CPPFLAGS= -I$(HADOOP_INSTALL)/src/c++/libhdfs  
- LIB = -L$(HADOOP_INSTALL)/c++/Linux-i386-32/lib/  
- libjvm=/usr/lib/jvm/java-6-openjdk/jre/lib/i386/client/libjvm.so  
- LDFLAGS += -lhdfs  
- 
- testHdfs: testHdfs.c  
-     gcc testHdfs.c  $(CPPFLAGS) $(LIB) $(LDFLAGS) $(libjvm) -o testHdfs  
- 
- clean:  
-     rm testHdfs  




好了，编译通过，但是运行的时候出现以下错误信息：


1.


./testHdfs: error while loading shared libraries: xxx.so.0:cannot open shared object file: No such file or directory


解决方法：把xxx.so.0所在的目录添加到/etc/ld.so.conf中，然后/sbin/ldconfig –v下就可以了。


2.



Exception in thread "main" java.lang.NoClassDefFoundError: org/apache/commons/configuration/Configuration


...


Call to org.apache.hadoop.fs.Filesystem::get(URI, Configuration) failed!

Exception in thread "main" java.lang.NullPointerException

Call to get configuration object from filesystem failed!



解决方法，修改/etc/profile,添加相应的CLASSPATH：





**[plain]**[view
 plain](http://blog.csdn.net/deqingguo/article/details/7004898#)[copy](http://blog.csdn.net/deqingguo/article/details/7004898#)



- HADOOP_HOME=/<span><span>home/fzuir/hadoop-0.20.203.0</span></span>  
- export PATH=$HADOOP_HOME/bin:$PATH  
- export CLASSPATH=.:$HADOOP_HOME/lib/commons-lang-2.4.jar:$HADOOP_HOME/hadoop-cor  
- e-1.0.1.jar:$HADOOP_HOME/lib/commons-logging-api-1.0.4.jar:$HADOOP_HOME/lib/comm  
- ons-configuration-1.6.jar:$JAVA_HOME/lib:$JRE_HOME/lib:$HADOOP_HOME/contrib/stre  
- aming/hadoop-streaming-1.0.1.jar:$CLASSPATH  

![](http://static.blog.csdn.net/images/save_snippets.png)






最后，恭喜你，问题解决了。







