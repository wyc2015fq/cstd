
# cppcheck的安装和使用 - 高科的专栏 - CSDN博客

2015年04月27日 16:06:36[高科](https://me.csdn.net/pbymw8iwm)阅读数：15096


首先从这里下载linux版本的：http://sourceforge.net/projects/cppcheck/files/cppcheck/
然后下载对应的版本，解压，之后安装：
编译：
g++ -o cppcheck -Ilib cli/*.cpp lib/*.cpp
安装：
make install

cppcheck默认安装会报错如下：
cppcheck: unusedFunction check can't be used with '-j' option. Disabling unusedFunction check.
(information) Failed to load std.cfg. Your Cppcheck installation is broken, please re-install. The Cppcheck binary was compiled without CFGDIR set. Either the std.cfg should be available in cfg or the CFGDIR should be configured.
因此我直接不用他们的make，而是直接使用g++参数编译：g++ -o cppcheck -Ilib cli/*.cpp lib/*.cpp
然后就是检查了
cppcheck -j 3 --enable=all src/



使能检查规则：默认：--enable=error
--enable=all
--enable=unusedFuntion path
--enable=style
规则定义：error：出现的错误
warning：为了预防bug防御性编程建议信息
style：编码格式问题（没有使用的函数、多余的代码等）
portablity：移植性警告。该部分如果移植到其他平台上，可能出现兼容性问题
performance：建议优化该部分代码的性能
information：一些有趣的信息，可以忽略不看的。
保存结果到文件中：重定向“>”
-j 3 使用3个线程，如果代码工程太大，可以使用15-20个，自己随意发挥，不过还是参考cpuinfo


如果是Windows的版本呢，就这样使用：
1、将Cppcheck绿色版cppcheck.rar解压；
2、打开cppcheckgui.exe；
3、菜单 check->directory；
4、选择源代码目录，确认；
5、Cppcheck自动开始走查；


