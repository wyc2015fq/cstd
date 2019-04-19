# Linux下编写 makefile 详细教程 - 建建的博客 - CSDN博客
2017年03月08日 19:52:01[纪建](https://me.csdn.net/u013898698)阅读数：350
个人分类：[Linux](https://blog.csdn.net/u013898698/article/category/6754383)
Makefile文件格式
         target：依赖文件
         tab空格：命令
makefile包含几种规则
        显示规则
        隐式规则
        变量定义
        文件引用
        注释
Makefile 文件内容
          testlog:testlog.o libxlog.so
  g++ testlog.o -o testlog -lxlog -L./
          testlog.o:testlog.cpp
  g++ testlog.cpp -c -I ../xlog
          libxlog.so:../xlog/xlog.cpp ../xlog/xlog.h
  g++ ../xlog/xlog.cpp -o libxlog.so -shared -fPIC
Linux下面找动态库必须要自己给指定路径
     需要编写脚本
    #!/bin/sh
    LD_LIBRARY_PATH=./
    export LD_LIBRARY_PATH
   ./testlog
    而且这个脚本最好是在linux下面创建
    windows和linux文件有时候创建出来，字符编码不兼容
