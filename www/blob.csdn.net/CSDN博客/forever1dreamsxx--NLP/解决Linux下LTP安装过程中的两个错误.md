
# 解决Linux下LTP安装过程中的两个错误 - forever1dreamsxx--NLP - CSDN博客


2014年01月08日 16:58:20[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1329


转载地址：[http://blog.chinaunix.net/uid-25829053-id-3159236.html](http://blog.chinaunix.net/uid-25829053-id-3159236.html)
安装过程:
./configure prefix=/usr/local/LTP
make
make install
在make过程中，出现两个错误，现将解决方法记录如下。
1.提示./src/__crf++/param.h中有如下错误：
explicit template specialization cannot have a storage class
解决办法：注释掉param.h中的第34行，即static
详见:http://apps.hi.baidu.com/share/detail/48860368
2./usr/bin/ld: cannot find -lboost_regex
这是解决完第一个问题后，重新make时出现的。
解决办法：在/usr/lib下执行 ln -s boost/libboost_regex.so.1.44.0 libboost_regex.so
即在/usr/lib下建立一个名为libboost_regex.so的boost/libboost_regex.so.1.44.0的符号链接
详见：[http://blog.sina.com.cn/s/blog_4156950c0100sfzz.html](http://blog.sina.com.cn/s/blog_4156950c0100sfzz.html)

