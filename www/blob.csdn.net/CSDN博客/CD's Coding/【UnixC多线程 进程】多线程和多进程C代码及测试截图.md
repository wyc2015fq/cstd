# 【UnixC多线程\进程】多线程和多进程C代码及测试截图 - CD's Coding - CSDN博客





2014年11月09日 16:28:23[糖果天王](https://me.csdn.net/okcd00)阅读数：554标签：[UnixC																[多线程																[多进程																[Linux](https://so.csdn.net/so/search/s.do?q=Linux&t=blog)
个人分类：[作业](https://blog.csdn.net/okcd00/article/category/1727537)







```cpp
﻿#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//线程1用来求1~n之间的素数
void *MyThread1(void)
{
   int n;
   int h=0,leap=1,k,m,i;
   printf("[pid_prime]input the value of n:\n");//输入n的值
   scanf("%d",&n); 
   printf("the prime number between 1~%d is:\n",n);
   for(m=2;m<=n;m++)
  {
       // k=sqrt(m+1);
        for(i=2;i<=m/2;i++)
        {
            if(m%i==0)
            {
               leap=0;break;
            }  
        }   
      if(leap)
      {
         printf("%4d",m);
         h++;
        if(h%10==0)
           printf("\n");
      }
    leap=1;
  }
  printf("\nthread1 exit!\n");//线程1退出
  pthread_exit(0);
}

//线程2用来求fib序列
void *MyThread2(void)
{
   int fib0=0,fib1=1,fib2,i,N;
    printf("[pid_fib]input the value of N:\n");//输入N的值
   scanf("%d",&N);
   printf("the fib sequence as following:\n");
   for(i=0;i<N;i++)
   {
      if(i==0)
      {
         printf("0 ");
       }
       else if(i==1)
       { 
          printf("1 ");
       }
       else
       {
          fib2=fib0+fib1;
          printf("%d ",fib2);
          fib0=fib1;
          fib1=fib2;
        }
   }
    printf("\nthread2 exit!\n");//线程2退出
    pthread_exit(0);
}

//线程3用来求闰年
void *MyThread3(void)
{
	int year=0;
	printf("[pid_run]input the value of year:\n");//输入n的值
	scanf("%d",&year);
	if(year%4!=0)
	{
	 printf("%d is not a 闰年\n",year);
	}
	else
	{
	 if(year%100!=0) printf("%d is a 闰年\n",year);
	 else 
	 {
	  if(year%400==0) printf("%d is a 闰年\n",year);
	  else printf("%d is not a 闰年\n",year);
	 }
	}
    printf("\nthread3 exit!\n");//线程3退出
    pthread_exit(0);
}

//线程4用来求回文
void *MyThread4(void)
{
	int num=0;
	printf("[pid_palid]input the value of num:\n");//输入num的值
	scanf("%d",&num);
	int a1=num/10000;
	int a2=num/1000%10;
	int a3=num/10%10;
	int a4=num%10;
	if(a1==a4 && a2==a3) printf("number %d is a palindome\n",num);
	else printf("number %d is not a palindome\n",num);
    printf("\nthread4 exit!\n");//线程4退出
    pthread_exit(0);
}

//主线程来创建两个线程
int mainthread()
{
    int ret1=0,ret2=0,ret3=0,ret4=0;
    pthread_t id1,id2,id3,id4;//定义两个线程id

   //调用pthread_create()函数来创建线程1
   ret1=pthread_create(&id1,NULL,(void*)MyThread1,NULL);
   //ret1=0表示创建成功，否则表示出错
   if(ret1)
   {
       //如果出错就打印错误信息
       printf("Create pthread error!\n");
     return 1;
   }

   //调用pthread_create()函数来创建线程2
   ret2=pthread_create(&id2,NULL,(void*)MyThread2,NULL);
   //ret2=0表示创建成功，否则表示出错
   if(ret2)
   {
       //如果出错就打印错误信息
       printf("Create pthread error!\n");
       return 1;
   }

   //调用pthread_create()函数来创建线程3
   ret3=pthread_create(&id3,NULL,(void*)MyThread3,NULL);
   //ret3=0表示创建成功，否则表示出错
    if(ret3)
   {
     //如果出错就打印错误信息
     printf("Create pthread error!\n");
     return 1;
   }

   //调用pthread_create()函数来创建线程4
   ret4=pthread_create(&id4,NULL,(void*)MyThread4,NULL);
   //ret4=0表示创建成功，否则表示出错
    if(ret4)
   {
     //如果出错就打印错误信息
     printf("Create pthread error!\n");
     return 1;
   }


  pthread_join(id1,NULL);//主线程等待线程1结束
  pthread_join(id2,NULL);//主线程等待线程2结束
  pthread_join(id3,NULL);//主线程等待线程3结束
  pthread_join(id4,NULL);//主线程等待线程4结束
  printf("main thread exit!\n");//主线程退出
  return 0; 
 }

//被4号进程创建的两个子进程分别调用下面这个函数来完成不同的任务
//CD：现在是仨进程了
void childfun(int i)
{
    switch(i)
    {

      case 5 : printf("This is NO.5 process,ID is %d,parent ID is %d,will create 2 thread\n",getpid(),getppid());
               mainthread();//调用线程创建的函数
               break;
      case 6 : printf("This is NO.6 process,ID is %d,parent ID is %d,will execute ./proc2 \n",getpid(),getppid());
              //用execl来执行用户写的可执行文件proc2
              execl("proc2","./proc2",NULL);
              break;
	case 7 : printf("This is NO.7 process,ID is %d,parent ID is %d,will execute ./proc2 \n",getpid(),getppid());
		

		break;
    }

    exit(0);

}


int createprocess()  //4号进程来创建两个子进程
{
  int i;
  for(i=5;i<=7;i++)
  {
      pid_t child;
      child=fork();
      //创建失败
      if(child==-1)
    {
      printf("Error happened in fork function!\n");
      return 0;
    }
   //子进程的返回值为0,
   else if(child==0)
   {
      childfun(i);//调用这个函数来分别让进程执行不同的程序
   }
 }
  for(i=0;i<3;i++)
 {
   //进程等待其子进程的退出
   pid_t cpid=wait(NULL);
   printf("the process %d exit\n",cpid);
 }
   return 0;
}

//父进程创建的两个进程分别调用下面函数来完成不同的任务
//CD:现在是三个进程了，多了个2
void fun(int i)
{
    switch(i)
    {
	case 2 : printf("this is NO.2 process,ID is %d,parent ID is %d,will execute command: ls -l:\n",getpid(),getppid());
		// NAME.ID.PID is ok
		break;
      	case 3 : printf("this is NO.3 process,ID is %d,parent ID is %d,will execute command: ls -l:\n",getpid(),getppid());
               //3号进程调用execl函数来执行ls -l命令
               execl("/bin/ls","ls","-l",NULL);
               break;
      	case 4 : printf("I am NO.4 process,ID is %d,parent ID is %d,will create 2 process\n",getpid(),getppid()); 
               createprocess();//4号进程调用这个函数来创建两个子进程
               break;
    }

    exit(0);
}

int main()
{
  int i;
  for(i=2;i<=4;i++)
  {
      pid_t child;
      child=fork();//调用fork函数来创建子进程
     //创建失败
      if(child==-1)
    {
      printf("Error happened in fork function!\n");
      return 0;
    }
   //子进程的返回值为0,
   else if(child==0)
   {
      printf("process ID is %d:\n",getpid());
      fun(i);
   }
 }
 for(i=0;i<3;i++)
 {
   //父进程等待子进程的退出
   pid_t cpid=wait(NULL);
   printf("The process %d exit\n",cpid);
 }
  //最后父进程退出
  printf("The NO.1 parent process ID is %d exit\n",getpid());
   return 0;
}
```



Terminal 调试过程_Logcat

```cpp
cqu@cqu:~$ ls
cd                Linux  公共的  视频  文档  音乐
examples.desktop  lyh    模板    图片  下载  桌面
cqu@cqu:~$ cd cd
cqu@cqu:~/cd$ ls
exp2  exp2.c  proc2  proc2.c  process  process.c
cqu@cqu:~/cd$ gcc exp2.c -lpthread -o exp2
cqu@cqu:~/cd$ ./exp2
process ID is 2713:
this is NO.2 process,ID is 2713,parent ID is 2712,will execute command: ls -l:
The process 2713 exit
process ID is 2714:
this is NO.3 process,ID is 2714,parent ID is 2712,will execute command: ls -l:
总计 52
-rwxr-xr-x 1 cqu cqu 11935 2014-11-09 16:17 exp2
-rwxr--r-- 1 cqu cqu  6390 2014-11-09 16:17 exp2.c
-rwxrw-rw- 1 cqu cqu  7159 2014-11-09 15:04 proc2
-rwxrw-rw- 1 cqu cqu    75 2014-11-09 15:04 proc2.c
-rwxrw-rw- 1 cqu cqu 11890 2014-11-09 15:04 process
-rwxrw-rw- 1 cqu cqu  4466 2014-11-09 15:04 process.c
The process 2714 exit
process ID is 2715:
I am NO.4 process,ID is 2715,parent ID is 2712,will create 2 process
This is NO.6 process,ID is 2717,parent ID is 2715,will execute ./proc2 
Hello world!
the process 2717 exit
This is NO.7 process,ID is 2718,parent ID is 2715,will execute ./proc2 
the process 2718 exit
This is NO.5 process,ID is 2716,parent ID is 2715,will create 2 thread
[pid_fib]input the value of N:
[pid_run]input the value of year:
[pid_prime]input the value of n:
[pid_palid]input the value of num:
5
the fib sequence as following:
0 1 1 2 3 
thread2 exit!
1234
1234 is not a 闰年

thread3 exit!
15
the prime number between 1~15 is:
   2   3   5   7  11  13
thread1 exit!
12321
number 12321 is a palindome

thread4 exit!
main thread exit!
the process 2716 exit
The process 2715 exit
The NO.1 parent process ID is 2712 exit
```






![](https://img-blog.csdn.net/20141109162346782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20141109162514484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20141109162434190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20141109162446546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



¢
1号父进程再创建2号进程。

![](https://img-blog.csdn.net/20141109162456296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

¢
2号进程输出自己的进程信息，包括名称，ID,PID等。

![](https://img-blog.csdn.net/20141109162603000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

¢
4号进程再创建7号进程。

![](https://img-blog.csdn.net/20141109162622531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Thread3：判断某个年份是否是闰年

![](https://img-blog.csdn.net/20141109162627093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Thread4：判断一个5位数是不是回文数。例如12321是回文数，个位与万位相同，十位与千位相同。

![](https://img-blog.csdn.net/20141109162637656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

编译运行编写的程序

![](https://img-blog.csdn.net/20141109162542659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看运行输出

![](https://img-blog.csdn.net/20141109162559866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看各个线程运行结果
![](https://img-blog.csdn.net/20141109162718187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=多进程&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=UnixC&t=blog)




