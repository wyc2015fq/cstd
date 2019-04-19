# wince 6.0下使用Performance monitor - xqhrs232的专栏 - CSDN博客
2013年03月19日 17:39:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1032
原文地址::[http://blog.csdn.net/whygosofar/article/details/4456830](http://blog.csdn.net/whygosofar/article/details/4456830)
相关网帖：：
1.[关于VS2005远程工具的一点说明](http://www.cnblogs.com/we-hjb/archive/2010/08/24/1807656.html)----[http://www.cnblogs.com/we-hjb/archive/2010/08/24/1807656.html](http://www.cnblogs.com/we-hjb/archive/2010/08/24/1807656.html)
2.远程性能监视器(Remote Performance Monitor)----[http://hi.baidu.com/timematter/item/619a4f488152bce1bcf4513a](http://hi.baidu.com/timematter/item/619a4f488152bce1bcf4513a)
1.打开performance monitor添加设备 ，取个设备名称。采用active sync连接方式。
2.把这个C:/Program Files/Common Files/Microsoft Shared/Windows CE Tools/Platman/target/armV4文件夹
拷贝到如下目录
C:/Program Files/Common Files/Microsoft Shared/Windows CE Tools/Platman/target/wce600
即可使用了。
//=============================
备注::
1>我的怎么是在C:/Program Files/Common Files/Microsoft Shared/Windows CE Tools/Platman/target/wce600下增加一个armV4文件夹，直接把armV4i文件夹COPY过来再修改为armV4，这样就可以连上了！！！
