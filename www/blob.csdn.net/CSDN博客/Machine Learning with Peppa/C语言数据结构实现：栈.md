# C语言数据结构实现：栈 - Machine Learning with Peppa - CSDN博客





2018年01月10日 13:16:04[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：112标签：[C																[栈																[数据结构																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[***Data Science***																[编程之美：C/C++																[算法之数据结构](https://blog.csdn.net/qq_39521554/article/category/7389781)
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)](https://blog.csdn.net/qq_39521554/article/category/7294876)




# 1.什么是栈

栈是一种只能在一端进行插入或者删除操作的线性表（说明栈还是线性表结构，只是操作受限而已）。其中允许进行插入或者删除操作的一端称为栈顶。栈的插入和删除一般叫入栈和出栈。栈的顺序存储结构叫做顺序栈，栈的链式存储结构叫做链栈。

# 2.栈的特点

栈的特点是后进先出。老师都喜欢举那个将盘子压入箱子的例子来解释栈的特点。举个例子：很多车开进死胡同，先进去的必须得等之前所有的车全出去才可以出去，所以第一个进去的车最后一个出来。最后进去的车第一个出来，所以这个就类似于栈，先进后出。

# 3.顺序栈

## 顺序栈的结构：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- #define maxsize 100  
- typedef struct SqStack{  
-     int data[maxsize];  
-     int top;  
- }SqStack;  




## 顺序栈的状态：

### （1）栈空状态：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- st.top=-1;  




### （2）栈满状态：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- st.top=maxsize-1;  




## 顺序栈的操作：

### （1）进栈：先开辟空间，然后元素入栈





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- ++st.top;  
- st.data[st.top]=x;  




### （2）出栈：元素先出栈，然后改变栈顶指针。





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- x=st.data[st.top];  
- --st.top;  




## 顺序栈的实现：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- #include<stdio.h>
- #include<stdlib.h>
- #define maxsize 100  
- typedef struct SqStack{  
-     int data[maxsize];  
-     int top;  
- }SqStack;  
- //初始化顺序栈  
- void initSqStack(SqStack *st){  
-     st->top=-1;  
- }  
- //判断栈是否为空  
- int SqStackEmpty(SqStack *st){  
-     return (st->top==-1?1:0);  
- }  
- //进栈  
- int push(SqStack *st,int x){  
-     if(st->top==maxsize-1){  
-         return 0;  
-     }  
-     st->data[++st->top]=x;  
-     return 1;  
- }  
- //出栈  
- int pop(SqStack *st,int *x){  
-     if(st->top ==-1){  
-         return 0;  
-     }  
-     *x=st->data[st->top--];  
-     return 1;  
- }  
- //打印栈元素  
- void printStack(SqStack *st){  
-     while(st->top !=-1){  
-         printf("栈元素：%d\n",st->data[st->top--]);  
-     }  
- }  
- void main(){  
-     int x;  
-     SqStack st={{1,2,3,4},3};  
-     push(&st,5);  
-     pop(&st,&x);  
-     printf("出栈元素：%d\n",x);  
-     printStack(&st);  
- }  

结果：


![](https://img-blog.csdn.net/20170220210847375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFfMTI0OTQ2MzMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注意：在出栈前一定要先判断栈是否为空，在入栈前一定要判断栈是否为满栈。


# 4.链栈

## 链栈的结构：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- typedef struct Lnode{  
-     int data;  
-     struct Lnode *next;  
- }Lnode;  




## 链栈的状态：

### （1）栈空：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- ln->next==NULL;  




## 链栈的操作：

### （1）进栈：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- p->next=ln->next;  
- ln->next=p;  




### （2）出栈：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- p=ln->next;  
- *x=p->data;  
- ln->next=p->next;  
- free(p);  




## 链栈的实现：





**[html]**[view
 plain](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[copy](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

[print](http://blog.csdn.net/ya_1249463314/article/details/56013305#)[?](http://blog.csdn.net/ya_1249463314/article/details/56013305#)

- #include<stdio.h>
- #include<stdlib.h>
- typedef struct Lnode{  
-     int data;  
-     struct Lnode *next;  
- }Lnode;  
- //初始化链栈  
- void initStack(Lnode *ln){  
- ln=(Lnode *)malloc(sizeof(Lnode));  
-     ln->next=NULL;  
- }  
- //判断链栈是否为空  
- int StackEmpty(Lnode *ln){  
-     return (ln->next==NULL?1:0);  
- }  
- //进栈  
- void push(Lnode *ln,int x){  
-     Lnode *p;  
- p=(Lnode *)malloc(sizeof(Lnode));  
-     if(p ==NULL){  
-         printf("ERROR");  
-         exit(0);  
-     }  
-     p->next=NULL;  
-     p->data=x;  
-     p->next=ln->next;  
-     ln->next=p;  
- }  
- //出栈  
- int pop(Lnode *ln,int *x){  
-     Lnode *p=ln->next;  
-     if(p ==NULL){  
-         return 0;  
-     }  
-     *x=p->data;  
-     ln->next=p->next;  
-     free(p);  
-     return 1;  
- }  
- void printStack(Lnode *ln){  
-     Lnode *p=ln->next;  
-     while(p!=NULL){  
-         printf("%d\n",p->data);  
- p=p->next;  
-     }  
- }  
- void main(){  
-     Lnode ln;  
-     int x;  
-     initStack(&ln);  
-     push(&ln,2);  
-     push(&ln,3);  
-     push(&ln,4);  
-     push(&ln,5);  
-     pop(&ln,&x);  
-     printf("出栈元素为：%d\n",x);  
-     printStack(&ln);  
- }  

结果：


![](https://img-blog.csdn.net/20170220215541021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWFfMTI0OTQ2MzMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)](https://blog.csdn.net/qq_39521554/article/category/7293623)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=栈&t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)




