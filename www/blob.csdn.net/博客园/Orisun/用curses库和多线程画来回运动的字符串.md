# 用curses库和多线程画来回运动的字符串 - Orisun - 博客园







# [用curses库和多线程画来回运动的字符串](https://www.cnblogs.com/zhangchaoyang/articles/2302293.html)





使用curses库可以设置光标的位置和终端屏幕上显示的字符串样式。下面是一个使用curses库的简单示例：



```
#include<stdio.h>
#include<curses.h>
void main(){
    initscr();  //初始化curses库和tty
    clear();    //清屏
    int i;
    for(i=0;i<LINES;i++){
        move(i,i+1); //把光标移到(10,20)的位置
        if(i%2==1)
            standout();//启动standout模式，一般使屏幕反色
        addstr("Hello World");  //在光标添加字符串
        if(i%2==1)
            standend(); //关闭standout模式
        sleep(1);    //sleep和alarm提供的时间精度为秒
        refresh();  //curses保存了两个屏幕：内部屏幕和工作屏幕，必须调用refresh才能用工作屏幕去替换真实屏幕
        move(i,i+1);
        addstr("");    //擦掉字符串
    }
    //getch();    //等待用户按下任意一个键
    endwin();   //关闭curses并重置tty
}
```

编译时注意加上-lcurses选项。

现在我们要在屏幕上下一场雪（武汉今年一直没下雪），其步骤大概是这样的：初始化一些雪花，它们的位置和速度都是随机的；让每片雪花运动一个步长后刷新屏幕。如果刷新的速度足够快，就会模拟的非常逼真。关键问题就在于：如果雪花很多，要一次完成对它们的位置更新需要的时间会比较长，即屏幕刷新的不是足够快，看起来根本不像是在下雪。

用多线程可以解决这个问题。每个线程只负责更新一片雪花的位置。更新前它获得互斥锁，更新后刷新屏幕，最后释放锁。这样每次刷新屏幕的间隔只是更新一片雪花所需要的时间，由于线程占用CPU是随机的，所以可能有的雪花移动了几次，而有的雪花一次都没移动，但人眼是觉察不出来哪片雪花没动的。

下面的程序由用户输入一些字符串，让这些字符串在屏幕上按水平方向来回的移动。看起来它们的运动同时进行但又相互独立。

```
#include<stdio.h>
#include<string.h>
#include<curses.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define MAXMSG 10       //最多允许用户输入的字符串个数，超过的部分会忽略
#define TUNIT 20000
struct propset{
    char *str;
    int row;
    int delay;
    int dir;
};
int setup(int,char **,struct propset*);
pthread_mutex_t mx=PTHREAD_MUTEX_INITIALIZER;
int main(int argc,char *argv[]){
    int c;
    pthread_t thrds[MAXMSG];
    struct propset props[MAXMSG];
    void *animate();
    int num_msg;
    int i;
    if(argc==1){
        printf("usage:%s string ...\n",argv[0]);
        exit(1);
    }
    num_msg=setup(argc-1,argv+1,props);
    for(i=0;i<num_msg;i++){     //用户输入几个字符串就创建几个线程
        if(pthread_create(&thrds[i],NULL,animate,&props[i])){   //若成功创建线程会返回0
            fprintf(stderr,"error creating thread");
            endwin();
            exit(0);
        }
    }
    while(1){
        c=getch();
        if(c=='Q' || c=='q')
            break;      //退出
        if(c==' '){     //全部反向
            for(i=0;i<num_msg;i++)
                props[i].dir=-props[i].dir; 
        }
        if(c>='0' && c<='9'){       //指定某个字符串反向
            i=c-'0';
            if(i<num_msg)
                props[i].dir=-props[i].dir; 
        }
    }
    pthread_mutex_lock(&mx);    //上锁，然后取消所有的线程
    for(i=0;i<num_msg;i++)
        pthread_cancel(thrds[i]);
    endwin();
    return 0;
}
int setup(int nstring,char *strings[],struct propset props[]){
    int num_msg=(nstring>MAXMSG?MAXMSG:nstring);
    int i;
    srand(getpid());
    for(i=0;i<num_msg;i++){
        props[i].str=strings[i];
        props[i].row=i;
        props[i].delay=1+(rand()%15);
        props[i].dir=((rand()%2)?1:-1);
    }
    initscr();
    crmode();
    noecho();
    clear();
    mvprintw(LINES-1,0,"'Q' to quit,'0'..'%d' to bounce",num_msg-1);    //在屏幕底端打印字条串
    return num_msg;
}
void *animate(void *arg){
    struct propset *info=arg;
    int len=strlen(info->str)+2;        //+2 for padding
    int col=rand()%(COLS-len-3);
    while(1){
        usleep(info->delay*TUNIT);
        pthread_mutex_lock(&mx);
        move(info->row,col);
        addch(' ');
        addstr(info->str);
        addch(' ');
        move(LINES-1,COLS-1);
        refresh();
        pthread_mutex_unlock(&mx);
        col+=info->dir;
        if(col<=0 && info->dir==-1)
            info->dir=1;
        else if(col+len>=COLS && info->dir==1)
            info->dir=-1;
    }
}
```












