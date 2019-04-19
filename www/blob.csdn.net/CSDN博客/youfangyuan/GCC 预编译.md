# GCC 预编译 - youfangyuan - CSDN博客
2012年08月27日 18:00:11[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：376
GCC 预编译 
$gcc -Wall -E hello.c
-E 显示预编译内容，但不保存
$gcc -Wall -c -save-temps hello.c
生成预编译文件并保存为：hello.i(预编译文件), hello.s（汇编文件）, hello.o（目标文件）
分开操作如下：
$cpp hello.c hello.i      //cpp是预编译器，生成预编译文件hello.i
$gcc -Wall -S hello.s     //生成汇编文件hello.s
$as hello.s -o hello.o    //生成目标文件 hello.o
$gcc hello.o -o hello     //生成可执行文件hello
