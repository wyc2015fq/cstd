# linux 查看 *.a *.so 符号表(zt) - DoubleLi - 博客园






objdump -tT libName.so | grep symbel symbolName

nm -D libName.so | grep symbel symbolName



很多非常熟悉windows 进程依赖那个dll，使用depends，linux使用ldd命令。
查看函数windows使用dumpbin，linux使用
objdump -tT libCavium4J.so |grep generateKey
nm -D libCavium4J.so  |grep generateKey



转自：[http://blog.csdn.net/jernymy/article/details/7258337](http://blog.csdn.net/jernymy/article/details/7258337)









