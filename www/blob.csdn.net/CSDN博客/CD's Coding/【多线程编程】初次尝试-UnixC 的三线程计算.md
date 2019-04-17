# 【多线程编程】初次尝试 - UnixC 的三线程计算 - CD's Coding - CSDN博客





2014年10月26日 22:10:29[糖果天王](https://me.csdn.net/okcd00)阅读数：569








老师布置了个作业的说……

然后就试试看写咯~




![](https://img-blog.csdn.net/20141026222502686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







```cpp
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
//use "gcc test01.c -o thread -lpthread" to compile
int n=0;
int mode=0;
int arr[1024]={0};

void maxv(void* args)
{
        sleep(1);
        int _max=-1,i=0;
        for(i=0;i<n;i++)_max=(_max<arr[i]?arr[i]:_max);
        if(mode) while(1) {sleep(1);printf("thr_1:the maximal value is: %d\n",_max);}
        else printf("the maximal value is: %d\n",_max);
}

void minv(void* args)
{
        sleep(2);
        int _min=9999,i=0;
        for(i=0;i<n;i++)_min=(_min>arr[i]?arr[i]:_min);
        if(mode) while(1) {sleep(2);printf("thr_2:the minimal value is: %d\n",_min);}
        else printf("the minimal value is: %d\n",_min);
}

void avgv(void* args)
{
        sleep(3);
        int sum=0,i=0;
        for(i=0;i<n;i++)sum+=arr[i];
        if(mode) while(1) {sleep(3);printf("thr_3:the average value is: %d\n",sum/n);}
        else printf("the average value is: %d\n",sum/n);
}

int main()
{
        int tmp=0;
        printf("Please Input numbers split by blanks(zero means end)\n");
        while(scanf("%d",&tmp) && tmp) arr[n++]=tmp;
        printf("If you want to show how thread works input '1',otherwise '0'\n");
        scanf("%d",&mode);
        pthread_t pid1,pid2,pid3;
        printf("Author: Chendian 20125209 Class:IOT01\n");
        printf("=====Multi Pthread Program Test01=====\n");
        if(pthread_create(&pid1,NULL,maxv,NULL)){printf("pid1 is wrong\n");return -1;}
        if(pthread_create(&pid2,NULL,minv,NULL)){printf("pid2 is wrong\n");return -1;}
        if(pthread_create(&pid3,NULL,avgv,NULL)){printf("pid3 is wrong\n");return -1;}
        while(1){sleep(3);}
        printf("=====Multithread Program Test End=====\n");
        return 0;
}
```








需要注意的其实也不多…… 使用UnixC编的东西在Linux编译的时候需要使用gcc -c xxxxx这样的指令，

然而我们使用的是多线程，所以还少了个库需要添加，所以要多一个参数，
`gcc test01.c -o thread -lpthread`


这样来编译即可，作用为把 test01.c 的文件使用lpthread编译，输出为thread文件，运行的时候就



```cpp
./thread
```
就可以啦~~~






运行结果： （啊对了average老师要求用int的，不是我不愿意用double哦~）


![](https://img-blog.csdn.net/20141026222657689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141026222735703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




