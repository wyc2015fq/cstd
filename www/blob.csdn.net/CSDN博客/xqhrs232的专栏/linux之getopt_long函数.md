# linux之getopt_long函数 - xqhrs232的专栏 - CSDN博客
2016年05月25日 16:38:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：289
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/mr_jj_lian/article/details/6835352](http://blog.csdn.net/mr_jj_lian/article/details/6835352)
相关文章
1、[getopt和getopt_long函数](http://blog.csdn.net/cashey1991/article/details/7942809)----[http://blog.csdn.net/cashey1991/article/details/7942809](http://blog.csdn.net/cashey1991/article/details/7942809)
2、[linux之getopt 函数](http://blog.csdn.net/mr_jj_lian/article/details/6835137)----[http://blog.csdn.net/mr_jj_lian/article/details/6835137](http://blog.csdn.net/mr_jj_lian/article/details/6835137)
getopt_long是GNU C的一个常用函数，我们在linux下使用各种命令的时候经常会输入各种选项，长选项或短选项，如果让我们自己解析这些选项，不仅工作繁琐，而且也违背软件复用的思想（毕竟选项是linux命令最普遍的一种用法），因此GNU的家伙们就开发出来这样一个函数帮助我们处理选项。
getopt_long(int argc,char * const argv[],const char * shortopts,const struct option *longopts,int *longindex)
下面介绍各个参数的含义：
argc、argv是main函数的两个同名的参数。
shortopts一般为一个字符串常量，代表所有的短选项，就是一般以“-”开头的选项，如果选项后带参数，则必须在相应的字符后面加“：”，如“ab:cde:”。
longopts是一个struct option结构体的数组，option结构体的定义如下：
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)struct option{ 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)constchar *name; 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)int has_arg; 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)int *flag; 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)int val; 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)};
name是长选项的名字，has_arg值为0、1、2，分别表示没有参数、有参数、参数可选，flag如果为NULL，函数返回val的值，否则将val的值写入flag指向的变量中，一般情况下，如果flag为NULL，则val的值为该长选项对应的短选项。如
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)conststruct option
 long_options[] = { 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)     { "help",                0, NULL, 'h' }, 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)     { "output",            1, NULL, 'o' }, 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)     { "version",         0, NULL, 'v' }, 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)     { NULL,                    0, NULL, 0     } 
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)};
longindex参数如果没有设置为NULL，那么它就指向一个变量，这个变量会被赋值为寻找到的长选项在longopts中的索引值，这可以用于错误诊断。
该函数的几种返回值
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)0  getopt_long()设置一个标志，它的值与option结构中的val字段的值一样    
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)1     每碰到一个命令行参数，optarg都会记录它    
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)'?'    无效选项    
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)':'    缺少选项参数    
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)'x'    选项字符'x'    
![](http://byrlx.blog.51cto.com/images/editer/InBlock.gif)-1    选项解析结束    
一般的用法为while(getopt_long() != -1){}，可以通过switch语句来对该函数不同的返回值（即不同的短选项）调用不同的处理函数。
#include <stdio.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <getopt.h>
int main(int argc,char *argv[])
{
  int opt;
  struct option longopts[] = {
    {"initialize",0,NULL,'1'},
    {"file",1,NULL,'f'},
    {"list",0,NULL,'1'},
    {"restart",0,NULL,'r'},
    {0,0,0,0}}:
  while((opt = getopt_long(argc, argv, "if:lr", longopts, NULL)) != -1){
    switch(opt){
    case 'i':
    case 'l':
    case 'r':
      printf("option: %c\n",opt);
      break;
    case 'f':
      printf("filename: %s\n",optarg);
      break;
    case ':':
      printf("option needs a value\n");
      break;
    case '?':
      printf("unknown option: %c\n",optopt);
      break;
  }
    for(; optind < argc; optind++){
      printf("argument: %s\n",argv[optind]);
    }
    exit(0);
}
