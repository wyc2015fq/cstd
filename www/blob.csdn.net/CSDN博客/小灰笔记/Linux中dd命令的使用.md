# Linux中dd命令的使用 - 小灰笔记 - CSDN博客





2017年08月20日 19:38:40[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：225








       使用了一次dd命令修改一个编译后的程序，最初抄命令的时候就觉得有点不确定。因为选项参数的形式让我看着有点像程序结构，特别担心因此而导致系统的错误。

       之前的命令以及结果如下：

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls -l

total 9

-rwxrwxrwx 1 rootroot  544 Aug 20 18:55 boot

-rwxrwxrwx 1 rootroot  232 Aug 20 18:41 boot.o

-rwxrwxrwx 1 rootroot  554 Aug 20 16:05 boot.s

-rwxrwxrwx 1 rootroot 4940 Aug 20 19:28 dd.txt

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$dd bs=32 if=boot of=new_boot skip=1

16+0 records in

16+0 records out

512 bytes (512 B)copied, 0.005296 s, 96.7 kB/s

grey@DESKTOP-3T80NPQ:/mnt/e/01_workspace/15_linux/01_kernal/03_my_test_code$ls -l

total 10

-rwxrwxrwx 1 rootroot  544 Aug 20 18:55 boot

-rwxrwxrwx 1 rootroot  232 Aug 20 18:41 boot.o

-rwxrwxrwx 1 rootroot  554 Aug 20 16:05 boot.s

-rwxrwxrwx 1 rootroot 4940 Aug 20 19:28 dd.txt

-rwxrwxrwx 1 rootroot  512 Aug 20 19:35 new_boot

       之前进这个操作的时候，看到if 和 of我就错认为是程序了。其实，这两个字母分别输输入文件和输出文件的缩写。而bs指定的则是一次读写的块大小，skip则指定跳过多少个块。

       其实，这仅仅是dd的一个功能。除此以外，dd还能够进行文件格式的转换以及字母大小写的转换等等功能。还能够方便地定制出逗号分隔符文件等格式。具体的描述倒是可以直接看dd问文档手册了！



