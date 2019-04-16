# PHP相关系列 - 安装php环境错误解决 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月15日 09:43:07[initphp](https://me.csdn.net/initphp)阅读数：1981标签：[php																[xml](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)








```
错误: configure: error: libevent >= 1.4.11 could not be found 
解决: yum -y install libevent libevent-devel

错误：configure: error: xml2-config not found. Please check your libxml2 installation. 
解决: yum -y install libxml2 libxml2-devel

错误: configure: error: mcrypt.h not found. Please reinstall libmcrypt.  
 解决: yum install libmcrypt libmcrypt-devel 

错误: configure: error: Please reinstall the libcurl distribution - easy.h should be in <CURL-DIR>/include/curl/  
 解决: yum -y install curl-devel 

错误: If configure fails try --with-jpeg-dir=  configure: error: libpng.(a|so) not found. 
解决: yum -y install libpng-devel

错误: /usr/bin/ld: cannot find -lltdl  collect2: ld returned 1 exit status  
解决: yum -y install libtool-ltdl-devel
```



 主要是一些库没安装的原因。




