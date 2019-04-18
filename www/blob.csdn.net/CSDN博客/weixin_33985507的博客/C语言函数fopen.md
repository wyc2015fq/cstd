# C语言函数fopen - weixin_33985507的博客 - CSDN博客
2012年03月22日 22:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
### 函数简介
**函数功能：**打开一个文件
**函数原型：**FILE * fopen(const char * path,const char * mode);
**相关函数：**[open](http://baike.baidu.com/view/26337.htm)，[fclose](http://baike.baidu.com/view/656642.htm)，fopen_s[1]，_wfopen
**所需库：**<[stdio.h](http://baike.baidu.com/view/538727.htm)>
**返回值：**文件顺利打开后，指向该流的文件指针就会被返回。如果文件打开失败则返回NULL，并把错误代码存在errno 中。
　　一般而言，打开文件后会作一些文件读取或写入的动作，若打开文件失败，接下来的读写动作也无法顺利进行，所以一般在fopen()后作错误判断及处理。
**参数说明：**
　　参数path字符串包含欲打开的文件路径及文件名，参数mode字符串则代表着流形态。
　　mode有下列几种形态字符串:
　　r 以[只读](http://baike.baidu.com/view/1060618.htm)方式打开[文件](http://baike.baidu.com/view/345685.htm)，该文件必须存在。
　　r+ 以可读写方式打开文件，该文件必须存在。
　　rb+ 读写打开一个二进制文件，允许读数据。
　　rw+ 读写打开一个文本文件，允许读和写。
　　w 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
　　w+ 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
　　a 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
　　a+ 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。 （原来的EOF符不保留）
　　wb 只写打开或新建一个二进制文件；只允许写数据。
　　wb+ 读写打开或建立一个二进制文件，允许读和写。
　　wt+ 读写打开或着建立一个文本文件；允许读写。
　　at+ 读写打开一个文本文件，允许读或在文本末追加数据。
　　ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
　　上述的形态字符串都可以再加一个b字符，如rb、w+b或ab+等组合，加入b 字符用来告诉函数库打开的文件为二进制文件，而非纯文字文件。不过在POSIX系统，包含Linux都会忽略该字符。由fopen()所建立的新文件会具有S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH(0666)权限，此文件权限也会参考umask 值。
　　有些C编译系统可能不完全提供所有这些功能，有的C版本不用"r+","w+","a+",而用"rw","wr","ar"等，读者注意所用系统的规定。
### 程序示例
**程序示例1**
　　#include <stdio.h>
　　#include <stdlib.h> //为了使用exit()
　　int main()
　　{
　　char ch;
　　FILE* fp;
　　char fname[50]; //用于存放文件名
　　printf("输入文件名：");
　　scanf("%s",fname);
　　fp=fopen(fname,"r"); //只供读取
　　if(fp==NULL) //如果失败了
　　{
　　printf("错误！");
　　exit(1); //中止程序
　　}
　　//getc()用于在打开文件中获取一个字符
　　while((ch=getc(fp))!=EOF)
　　putchar(ch);
　　fclose(fp); //关闭文件
　　return 0;
　　}
　　注意！初学者往往会犯一个错误，即在输入文件名时不加后缀名，请注意加上！
**程序示例2[2]**
　　#include <stdio.h>
　　FILE *stream, *stream2;
　　int main( void )
　　{
　　int numclosed;
　　// Open for read (will fail if file "crt_fopen.c" does not exist)
　　if( (stream = fopen( "crt_fopen.c", "r" )) == NULL ) // C4996
　　// Note: fopen is deprecated; consider using fopen_s instead
　　printf( "The file 'crt_fopen.c' was not opened\n" );
　　else
　　printf( "The file 'crt_fopen.c' was opened\n" );
　　// Open for write
　　if( (stream2 = fopen( "data2", "w+" )) == NULL ) // C4996
　　printf( "The file 'data2' was not opened\n" );
　　else
　　printf( "The file 'data2' was opened\n" );
　　// Close stream if it is not NULL
　　if( stream)
　　{
　　if ( fclose( stream ) )
　　{
　　printf( "The file 'crt_fopen.c' was not closed\n" );
　　}
　　}
　　// All other files are closed:
　　numclosed = _fcloseall( );
　　printf( "Number of files closed by _fcloseall: %u\n", numclosed );
　　}
