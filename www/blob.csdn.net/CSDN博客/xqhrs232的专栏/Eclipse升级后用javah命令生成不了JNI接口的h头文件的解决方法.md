# Eclipse升级后用javah命令生成不了JNI接口的h头文件的解决方法 - xqhrs232的专栏 - CSDN博客
2012年03月06日 16:59:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：695标签：[jni																[eclipse](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
相关网帖
1.[使用javah出现无法访问的错误！(转载)](http://blog.csdn.net/zhhaogen/article/details/6266545)----  
[http://blog.csdn.net/zhhaogen/article/details/6266545](http://blog.csdn.net/zhhaogen/article/details/6266545)
2.[javah生成jni头文件](http://blog.csdn.net/roadog2006/article/details/6414583)----[http://blog.csdn.net/roadog2006/article/details/6414583](http://blog.csdn.net/roadog2006/article/details/6414583)
没升级Eclipse前，只要切到对应的工程目录下执行如下的命令就可以生成JNI接口的h头文件，但升级后不行，通过比较以前可以的工程发现升级Eclipse后bin目录下改变了，多了一级classes目录，所以用下面格式的命令就不行了。要加一级classes目录。也要注意系统的环境变量CLASSPATH的设置值！！！
javah -classpath  bin -d jni xxx.xxx.xxx.abc
其中xxx.xxx.xxx.abc中xxx.xxx.xxx为工程的包名，abc是要实现为JNI接口的类。
//------------------------------------------------------------------
javah -classpath  bin\classes  -d  jni   xxx.xxx.xxx.abc
