# Web服务器移植--boa和cgic - Orisun - 博客园







# [Web服务器移植--boa和cgic](https://www.cnblogs.com/zhangchaoyang/articles/1934386.html)






1.可以去这个上面下载boa-0.94.14rc18.tar.gz和cgic205.tar.gz

  http://labs.nevion.com/open-source/

2.去Chinaunix上下载m4-1.4.13,bison-2.2和flex-2.5.33

  上面三个存在依赖关系，要依次安装

  ./configure

  make

  sudo make install

  当然如果你的网络很给力，sudo apt-get install bison flex是可以的

  如果不安装bison和flex,在编译boa时会出现如下错误

　make: yacc：命令未找到

　make: *** [y.tab.c] 错误 127

　make: lex：命令未找到

　make: *** [lex.yy.c] 错误 127

3.配置boa

  cd boa-0.94.13/src

  ./configure

  vi Makefile

  在 31 行和 32 行 , 到 : = gcc 和 CPP = gcc -E, 后 改 成 : = arm-linux-gcc 和  CPP =arm-linux-g++ -E,保存退出。

  修改 src/boa.c 文件:

  #vi src/boa.c

  在行 225 到行 227 间有如下内容,注释掉。原因后面会解释:

  if (setuid(0) != -1) {

DIE("icky Linux kernel bug!");

  }

  保存退出。

  修改 src/compat.h 文件:

  #vi src/compat.h

  把 120 行改为如下内容:

  #define TIMEZONE_OFFSET(foo) foo->tm_gmtoff

4.编译并优化boa

  编译,会在 boa-0.94.13 目录下生成 boa 的可执行文件(大小 232K 左 右 )

  #make

  优化:

  #arm-linux-strip boa

  这里的优化就是去除 boa 中的调试信息。经过此操作,boa 会由 232K 变成 62K 左右,这个优化是比较常用的做法。













