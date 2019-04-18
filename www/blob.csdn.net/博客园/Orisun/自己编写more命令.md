# 自己编写more命令 - Orisun - 博客园







# [自己编写more命令](https://www.cnblogs.com/zhangchaoyang/articles/2293910.html)





more命令可以实现的功能是很丰富的，包括模式查找。今天我们只是实现它的一个基本功能：
- more  文件列表
- 在屏幕正文显示已读的百分比
- 按下回车显示下一行
- 按下空格显示下一屏
- 按下q退出
- 按回车、空格、q时没有回显

这里有几个问题：
- 要在屏幕正文显示已读的百分比，那我们必须先获取文件列表中各个文件的长度，这个可以使用系统调用lstat
#include<sys/stat.h>
struct stat *buf;
lstat("filename",buf) //获取文件状态
filesize+=buf->st_size; //获取文件大小 
- 要显示整屏内容，我们就需要知道一屏可以显示多少行，然后从文件中读出这么多行显示在屏幕上。

#include<termios.h>
struct winsize *ws;
ws=(struct winsize*)malloc(sizeof(struct winsize));
memset(ws,0x00,sizeof(struct winsize));
ioctl(STDIN_FILENO,TIOCGWINSZ,ws);
int width=ws->ws_col;
int height=ws->ws_row;

- 禁用回显

#include<termios.h>
struct termios ts; //终端属性
tcgetattr(STDIN_FILENO,&ts); //获取终端属性 
ts.c_lflag &= (~ECHO); //阻止回显
tcsetattr(STDIN_FILENO,TCSAFLUSH,&ts); //设置终端的新属性
 备注：如果要开启回显，可以用ts.c_lflag |= ECHO;

- 从终端读取单个字符后立即返回，不需要按回车
#include<termios.h>
struct termios ts; //终端属性
tcgetattr(STDIN_FILENO,&ts); //获取终端属性 
ts.c_lflag &= (~ICANON); //设置终端为非标准模式，在非标准模式下终端驱动程序没有缓冲区，输入立即交换程序处理
ts.c_cc[VMIN]=1; //VMIN的值告诉驱动程序一次可以读取多少个字符。如果想一次读取3个字符就设为3
ts.c_cc[VTIME]=0;　　//VTIME设置超时
tcsetattr(STDIN_FILENO,TCSAFLUSH,&ts); //设置终端的新属性 
备注：

终端有三种模式：

　　规范模式：驱动程序输入的字符保存在缓冲区中，仅在接收到Enter键后才发送给程序，在此之前支持对输入的编辑。

　　非规范模式：没有缓冲区，输入一个字母就立即交给程序。

　　raw模式：每个处理步骤都被一个独立的位控制。例如，ISIG位控制Ctrl-C键是否用于终止一个程序。程序可以随意地关闭这些处理步骤。

 默认情况下打开的文件都处于阻塞模式，当调用getchar()时如果用户一直不输入，程序就会一直等待。使用fcntl或open文件时可以为文件描述符指定非阻塞模式O_NDELAY或O_NONBLOCK。关闭文件描述符的阻塞状态并调用read，如果没有输入字符，read就返回0。

终端驱动程序不仅是一行一行地缓冲输入，而且是一行一行有缓冲输出。可以调用fflush(stdout)强制缓冲输出。


完整代码：

```
#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<termios.h>
void gettermsize(int *w,int *h);     //获取终端尺寸
void do_more(FILE *);   //根据see_more获取的用户输入，显示更多的文件内容或退出
int see_more();   //捕获用户的输入指令
void settermattr();    //设置终端属性
int width;      //终端屏幕宽度
int height;     //终端屏幕高度
int filesize;   //文件的大小
int readsize;   //已经显示的内容长度
struct termios ts,ots;     //终端属性
int main(int argc,char *argv[]){
    gettermsize(&width,&height);
    settermattr();
    FILE *fp;
    struct stat *buf;
    filesize=0;
    readsize=0;
    buf=(struct stat*)malloc(sizeof(struct stat));
    memset(buf,0x00,sizeof(struct stat));
    if(argc==1)     //如果没有指定文件，则从标准输入读取内容
        do_more(stdin);
    else{        //从文件列表中读取内容
        int i;
        for(i=1;i<argc;i++){       //more命令后面可能跟多个文件
            if((fp=fopen(*(argv+i),"r"))!=0){     //以只读形式打开文件
                if(lstat((const char*)*(argv+i),buf)==0){   //获取文件状态
                    filesize+=buf->st_size;     //获取文件大小
                    printf("fs=%ld\n",buf->st_size);
                }
                else{
                    exit(1);
                }
                fclose(fp);      //关闭文件
            }
            else{        //打开文件失败,退出程序
                exit(1);
            }
        }
        printf("size=%d\n",filesize);
        for(i=1;i<argc;i++){       //more命令后面可能跟多个文件
            if((fp=fopen(*(argv+i),"r"))!=0){     //以只读形式打开文件
                do_more(fp);        //显示文件内容
                fclose(fp);      //关闭文件
            }
            else{        //打开文件失败,退出程序
                exit(1);
            }
        }
    }
    tcsetattr(STDIN_FILENO,TCSANOW,&ots);
    return 0;
}
void gettermsize(int *w,int *h){
    struct winsize *ws;
    ws=(struct winsize*)malloc(sizeof(struct winsize));
    memset(ws,0x00,sizeof(struct winsize));
    ioctl(STDIN_FILENO,TIOCGWINSZ,ws);
    *w=ws->ws_col;
    *h=ws->ws_row;
}
void settermattr(){
    tcgetattr(STDIN_FILENO,&ts);    //获取终端属性 
    ots=ts;         //备份终端属性
    ts.c_lflag &= (~ECHO);    //阻止回显
    ts.c_lflag &= (~ICANON);    //设置终端为非标准模式
    ts.c_cc[VMIN]=1;        //这两行当需要从终端获取字符时立即返回，不需要按回车键
    ts.c_cc[VTIME]=0;
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&ts);    //设置终端的新属性。TCSAFLUSH表示输出队列空了以后才生效，生效之前的输出队列被flush
}
void do_more(FILE *fp){
    int linesize=width;
    char line[linesize];
    int num_of_lines=0;     //记录本次读了多少行
    int reply;      //记录see_more()的返回值
    FILE *fp_tty;
    while(fgets(line,linesize,fp)){    //从文件中读取一行内容
        readsize+=strlen(line);     //读出的内容长度
        if(num_of_lines==height){   //如果满屏了
            reply=see_more(); //从键盘获取用户输入的命令
            if(reply==0){        //用户不需要显示更多内容了，要退出
                tcsetattr(STDIN_FILENO,TCSANOW,&ots);
                exit(0);
            }
            num_of_lines-=reply;
        }
        if(fputs(line,stdout)==EOF){ //把这行内容显示在用户屏幕上
            tcsetattr(STDIN_FILENO,TCSANOW,&ots);        //TCSANOW表示修改立即生效
            perror("fputs");
            exit(1);
        }
        num_of_lines++;
    }
}
int see_more(){
    int c;
    printf("\033[7m--more--(%2.0f%%)\033[m",(double)readsize/filesize*100);
    while((c=getc(stdin))!=EOF){            //注意加括号，赋值操作符是右结合的。如果不加括号会把getchar()!=EOF的结果赋给c
        if(c=='q'){
            return 0;
        }
        if(c==' ')      //空格
            return height;    
        if(c=='\n')     //回车
            return 1;
    }
    return 0;
}
```












