# 数据结构：循环队列（C语言实现） - 数据之美的博客 - CSDN博客
2017年08月15日 03:19:02[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：178
个人分类：[数据结构](https://blog.csdn.net/oppo62258801/article/category/6551324)
版权声明：本文为博主原创文章，未经博主允许不得转载。
生活中有很多队列的影子，比如打饭排队，买火车票排队问题等，可以说与时间相关的问题，一般都会涉及到队列问题；从生活中，可以抽象出队列的概念，队列就是一个能够实现“先进先出”的存储结构。队列分为链式队列和静态队列；静态队列一般用数组来实现，但此时的队列必须是循环队列，否则会造成巨大的内存浪费；链式队列是用链表来实现队列的。这里讲的是循环队列，首先我们必须明白下面几个问题
一、循环队列的基础知识
1.循环队列需要几个参数来确定
循环队列需要2个参数，front和rear
2.循环队列各个参数的含义
（1）队列初始化时，front和rear值都为零；
（2）当队列不为空时，front指向队列的第一个元素，rear指向队列最后一个元素的下一个位置；
（3）当队列为空时，front与rear的值相等，但不一定为零；
3.循环队列入队的伪[算法](http://lib.csdn.net/base/datastructure)
（1）把值存在rear所在的位置；
（2）rear=（rear+1）%maxsize ，其中maxsize代表数组的长度；
程序代码：
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- bool Enqueue(PQUEUE Q, int val)  
- {  
- if(FullQueue(Q))  
- returnfalse;  
- else
-     {  
-         Q->pBase[Q->rear]=val;  
-         Q->rear=(Q->rear+1)%Q->maxsize;  
- returntrue;  
-     }  
- }  
4.循环队列出队的伪算法
（1）先保存出队的值；
（2）front=（front+1）%maxsize ，其中maxsize代表数组的长度；
程序代码：
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- bool Dequeue(PQUEUE Q, int *val)  
- {  
- if(EmptyQueue(Q))  
-     {  
- returnfalse;  
-     }  
- else
-     {  
-         *val=Q->pBase[Q->front];  
-         Q->front=(Q->front+1)%Q->maxsize;  
- returntrue;  
-     }  
- }  
5.如何判断循环队列是否为空
if（front==rear）
队列空；
else
队列不空；
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- bool EmptyQueue(PQUEUE Q)  
- {  
- if(Q->front==Q->rear)    //判断是否为空
- returntrue;  
- else
- returnfalse;  
- }  
6.如何判断循环队列是否为满
![](https://img-blog.csdn.net/20140307190145546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHBwMDkwMDMyMDEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    这个问题比较复杂，假设数组的存数空间为7，此时已经存放1，a，5,7,22,90六个元素了，如果在往数组中添加一个元素，则rear=front；此时，队列满与队列空的判断条件front=rear相同，这样的话我们就不能判断队列到底是空还是满了；
解决这个问题有两个办法：一是增加一个参数，用来记录数组中当前元素的个数；第二个办法是，少用一个存储空间，也就是数组的最后一个存数空间不用，当（rear+1）%maxsiz=front时，队列满；
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- bool FullQueue(PQUEUE Q)  
- {  
- if(Q->front==(Q->rear+1)%Q->maxsize)    //判断循环链表是否满，留一个预留空间不用
- returntrue;  
- else
- returnfalse;  
- }  
附录：
queue.h文件代码：
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- #ifndef __QUEUE_H_
- #define __QUEUE_H_
- typedefstruct queue   
- {  
- int *pBase;  
- int front;    //指向队列第一个元素
- int rear;    //指向队列最后一个元素的下一个元素
- int maxsize; //循环队列的最大存储空间
- }QUEUE,*PQUEUE;  
- 
- void CreateQueue(PQUEUE Q,int maxsize);  
- void TraverseQueue(PQUEUE Q);  
- bool FullQueue(PQUEUE Q);  
- bool EmptyQueue(PQUEUE Q);  
- bool Enqueue(PQUEUE Q, int val);  
- bool Dequeue(PQUEUE Q, int *val);  
- #endif
queue.c文件代码：
**[cpp]**[view
 plain](http://blog.csdn.net/lpp0900320123/article/details/20694409#)[copy](http://blog.csdn.net/lpp0900320123/article/details/20694409#)
- #include<stdio.h>
- #include<stdlib.h>
- #include"malloc.h"
- #include"queue.h"
- /***********************************************
- Function: Create a empty stack;
- ************************************************/
- void CreateQueue(PQUEUE Q,int maxsize)  
- {  
-     Q->pBase=(int *)malloc(sizeof(int)*maxsize);  
- if(NULL==Q->pBase)  
-     {  
-         printf("Memory allocation failure");  
-         exit(-1);        //退出程序
-     }  
-     Q->front=0;         //初始化参数
-     Q->rear=0;  
-     Q->maxsize=maxsize;  
- }  
- /***********************************************
- Function: Print the stack element;
- ************************************************/
- void TraverseQueue(PQUEUE Q)  
- {  
- int i=Q->front;  
-     printf("队中的元素是:\n");  
- while(i%Q->maxsize!=Q->rear)  
-     {  
-         printf("%d ",Q->pBase[i]);  
-         i++;  
-     }  
-     printf("\n");  
- }  
- bool FullQueue(PQUEUE Q)  
- {  
- if(Q->front==(Q->rear+1)%Q->maxsize)    //判断循环链表是否满，留一个预留空间不用
- returntrue;  
- else
- returnfalse;  
- }  
- bool EmptyQueue(PQUEUE Q)  
- {  
- if(Q->front==Q->rear)    //判断是否为空
- returntrue;  
- else
- returnfalse;  
- }  
- bool Enqueue(PQUEUE Q, int val)  
- {  
- if(FullQueue(Q))  
- returnfalse;  
- else
-     {  
-         Q->pBase[Q->rear]=val;  
-         Q->rear=(Q->rear+1)%Q->maxsize;  
- returntrue;  
-     }  
- }  
- 
- bool Dequeue(PQUEUE Q, int *val)  
- {  
- if(EmptyQueue(Q))  
-     {  
- returnfalse;  
-     }  
- else
-     {  
-         *val=Q->pBase[Q->front];  
-         Q->front=(Q->front+1)%Q->maxsize;  
- returntrue;  
-     }  
- }  
