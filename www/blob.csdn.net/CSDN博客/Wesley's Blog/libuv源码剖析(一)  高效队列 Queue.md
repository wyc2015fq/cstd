
# libuv源码剖析(一): 高效队列 Queue - Wesley's Blog - CSDN博客


2018年08月26日 22:23:17[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：352


## Introduction
libuv中的高效队列是用c语言, 只使用**宏定义**封装而成, 据说linux内核都是这个风格的, 因为我没有读过linux内核源码, 所以一开始理解这一段代码还是有些困难的.
libuv中使用的是**双向循环队列,**实际上每个节点是两个`void*`指针, 分别保存`next`,`prev`指针.
## 实现原理
libuv 的 queue 实现了`QUEUE_NEXT`,`QUEUE_PREV`,`QUEUE_PREV_NEXT`,`QUEUE_NEXT_PREV`,`QUEUE_DATA`,`QUEUE_FOREACH`,`QUEUE_EMPTY`,`QUEUE_HEAD`,`QUEUE_INIT`,`QUEUE_ADD`,`QUEUE_SPLIT`,`QUEUE_INSERT_HEAD`,`QUEUE_INSERT_TAIL`,`QUEUE_REMOVE`几个方法
### QUEUE_NEXT, QUEUE_PREV, QUEUE_PREV_NEXT, QUEUE_NEXT_PREV
首先来说一下上面这几个最基本的宏定义
`#include <stddef.h>
typedef void *QUEUE[2];
/* Private macros. */
#define QUEUE_NEXT(q)       (*(QUEUE **) &((*(q))[0]))
#define QUEUE_PREV(q)       (*(QUEUE **) &((*(q))[1]))
#define QUEUE_PREV_NEXT(q)  (QUEUE_NEXT(QUEUE_PREV(q)))
#define QUEUE_NEXT_PREV(q)  (QUEUE_PREV(QUEUE_NEXT(q)))`正如之前所说, QUEUE被定义成了两个`void*`指针
![这里写图片描述](https://img-blog.csdn.net/20180826222218351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
关于这段代码有些难以理解的地方就是`(*(QUEUE **) &((*(q))[0]))`, 其实这个的意思简单来说就是`(*q)[0]`, 而其他的那些操作是为了**类型保持、成为左值。**
使用的时候, 用法如下:
`static QUEUE queue;
QUEUE_NEXT(&queue);`QUEUE_NEXT(&queue) 扩展之后相当于：(*(QUEUE **)&((*(&queue))[0]))，我们将其拆开来看(如下图所示)，共分为四个部分：
先对数组取地址（&）再对其解引用（*），最后再作[0]运算，就相当于queue[0]，这里补充一下知识：假设有一个数组int a[10]，当访问数组时，a[1]相当于*(a+1)，而数组名相当于数组首元素首地址，而&a在数值上虽然与a的值相同，但是&a从含义上讲是代表整个数组的首地址（类型为整个数组），因此&a + 1操作将跨域整个数组的长度，因此(&a)[1]并不是访问a[1]，(*(&a))[1]才是访问a[1]
对数组首元素queue[0]取地址。
对第二部分取得的地址进行强制类型转换，将其强转为QUEUE **，因为QUEUE的元素类型本身为void *，而实际中每一个元素都需要指向QUEUE地址，因此对于&queue[0](二级指针)，就需要将其强转为QUEUE **。
对上文强转后的地址进行“解引用”操作，也就是对&queue[0]解引用之后相当于queue[0]，为什么要这么做呢？这是为了使其成为左值，左值的简单定义是：占用实际的内存、可以对其进行取地址操作的变量都是左值，而c语言中（其实其他语言也是一样），对于一个变量（或者表达式）进行强制类型转换时，其实并不是改变该变量本身的类型，而是产生一个变量的副本，而这个副本并不是左值（因为并不能对其取地址），它是一个右值，举个例子：int a = 1; (char) a = 2;这样会报错。而如果改成这样：int a = 1; ((char)(&a)) = 2;就正确了。
![这里写图片描述](https://img-blog.csdn.net/20180826222246319?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
至此，这个稍微有点复杂的表达式算是分析清楚了，对于QUEUE_PREV原理类似，在此不再赘述。
再然后的基本的`INSERT`,`EMPTY`等函数也较为基础, 不再介绍
### QUEUE_DATA
以前用c++的同学可能会有疑问了, 队列的节点当然不能只有 next, prev 信息, 那其他的用户数据放在哪呢? 只有一个链, 没有用户数据的话有什么用?
实际上, libuv是通过指针偏移来达到目的的:
`/* Public macros. */
#define QUEUE_DATA(ptr, type, field)    \      
  ((type *) ((char *) (ptr) - offsetof(type, field)))`这种队列并不限制你的用户数据类型，你需要做的，只是将QUEUE节点内嵌到自己定义的数据类型中即可, 然后让它们串起来.
从这里也联想到**c++ 的继承**, 本来这种方式可以用 c++ 的继承来实现, 也更加的清晰明了, 就定义一个基类 QueueNode, 里面包含了基本的两个`void*`指针, 然后还有各种方法, 然后其他需要加各种用户数据的再通过继承加入自己的字段. 这样的代码更加清晰明了. (脑补c++的继承如何实现的)
![这里写图片描述](https://img-blog.csdn.net/20180826222233139?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 实例代码
为了进一步熟悉, 还写了一个实例代码:
`#include <iostream>
#include <cstring>
#include "queue.h"
#include <stdio.h>
using namespace std;
struct Student{
    int age;
    char *name;
    QUEUE node;
};
int main() {
    Student user_s1;
    Student user_s2;
    user_s1.age = 10;
    user_s1.name = new char[10];
    strcpy(user_s1.name, "wesley");
    user_s2.age = 15;
    user_s2.name = new char[10];
    strcpy(user_s2.name, "Bob");
    QUEUE queue;
    QUEUE_INIT(&queue);
    QUEUE_INIT(&user_s1.node);
    QUEUE_INIT(&user_s2.node);
    QUEUE_INSERT_TAIL(&queue, &user_s1.node);
    QUEUE_INSERT_TAIL(&queue, &user_s2.node);

    QUEUE *p;
    p = QUEUE_HEAD(&queue);
    /**
    * Should retrieve the user behind the "p" pointer.
    */
    Student *first_stu = QUEUE_DATA(p, struct Student, node);
    /**
    * Should output the name of wesley.
    */
    printf("Received first inserted Student: %s who is %d.\n",
           first_stu->name, first_stu->age);
    QUEUE_FOREACH(p, &queue){
        Student *tmp = QUEUE_DATA(p, struct Student, node);
        cout<<"name: "<<tmp->name<<" age: "<<tmp->age<<endl;
    }
    return 0;
}`输出是:
`Received first inserted Student: wesley who is 10.
name: wesley age: 10
name: Bob age: 15`非常感谢大牛的博客, 参考链接:
[Nodejs事件引擎libuv源码剖析之：高效队列（queue）的实现](https://my.oschina.net/fileoptions/blog/1036629)
[libuv queue的实现](https://www.jianshu.com/p/6373de1e117d)

