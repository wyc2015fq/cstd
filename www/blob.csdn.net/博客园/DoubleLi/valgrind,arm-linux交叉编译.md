# valgrind,arm-linux交叉编译 - DoubleLi - 博客园






1. 下载及解压valgrind-3.9.0

2.CC=/opt/hisi-linux/x86-arm/arm-hisiv200-linux/target/bin/arm-hisiv200-linux-gcc CPP=/opt/hisi-linux/x86-arm/arm-hisiv200-linux/target/bin/arm-hisiv200-linux-cpp CXX=/opt/hisi-linux/x86-arm/arm-hisiv200-linux/target/bin/arm-hisiv200-linux-g++ ./configure --host=armv7-a-linux --prefix=/nfsroot/valgrind

记住--prefix这里的路径要与你在板子上放的路径一样。否则会出现valgrind: failed to start tool 'memcheck' for platform 'arm-linux': No such file or directory”错误。 host=armv7-a-linux 这可以改成arm-linux, 修改configure里面的armv7* 为 armv7*|arm*

3. 放到板子上目录是--prefix的路径。

valgrind: failed to start tool 'memcheck' for platform 'arm-linux': No such file or directory”，这个错误纠结了好久，最后在[http://blog.csdn.net/jiazhen/article/details/18966423?reload#reply](http://blog.csdn.net/jiazhen/article/details/18966423?reload#reply) 大神这里找到解决方法了。 十分感谢。。。









