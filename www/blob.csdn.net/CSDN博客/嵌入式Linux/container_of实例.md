
# container_of实例 - 嵌入式Linux - CSDN博客

2019年01月14日 17:29:50[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：68个人分类：[C																](https://blog.csdn.net/weiqifa0/article/category/1389385)



### 1 前面说的
container_of 在linux内核代码里面使用非常多，对于喜欢linux编程的同学来说，对这个函数要非常清楚他的使用方法，而且还要知道他的原理，这对以后看内核代码，写内核驱动的帮助都非常大，当然，我不是说了解这个就可以为所欲为了，内核博大精深，先宏观再微观去学习，不要想着一口就能吃成一个胖子，我这篇文章主要剖析一下这个函数的实现原理，希望对大家学习过程中有所帮助。
![](http://plck4ljzb.bkt.clouddn.com/Fu6H9fanTvXg-SHTvu__wn6nw_PJ)

### 2 container_of的作用
container_of的作用的通过结构体成员变量地址获取这个结构体的地址，就比如可以通过你的名字从而知道你爸爸的名字，比如别人叫你“李光明的儿子”，然后别人就可以知道你爸爸叫做“李光明”。
内核函数调用常常给函数传入的是结构体成员地址，然后在函数里面又想使用这个结构体里面的其他成员变量，所以就引发了这样的问题，我认为这个也是用C实现面向对象编程的一种方法。
比如这段代码
`static void sensor_suspend(struct early_suspend *h)                   
{                                                                     
    struct sensor_private_data *sensor =                              
            container_of(h, struct sensor_private_data, early_suspend);                                                                          
    if (sensor->ops->suspend)                                         
        sensor->ops->suspend(sensor->client);                         
}`early_suspend是sensor_private_data 里面的一个成员变量，通过这个成员变量的地址获取sensor_private_data结构体变量的地址，从而调用里面的成员变量client。这个方法非常优雅。在这里我用到了一个比较叼的词，叫“优雅”。
### 3 如何使用container_of
container_of(ptr, type, member)
ptr:表示结构体中member的地址h
type:表示结构体类型struct sensor_private_data
member:表示结构体中的成员early_suspend type里面一定要有这个成员，不能瞎搞啊
返回结构体的首地址
### 4 container_of 用到的知识点 剖析
4.1、({})
({}),第一个先说这个表达式，很多人可能懂，但是可能很多人没见过，这个表达式返回最后一个表达式的值。
举个例子
`#include<stdio.h>
void main(void)
{
    int a=({1;2;4;})+10;
    printf("%d\n",a);//a=14
}`4.2、typeof
这个我们很少看到，这个关键字是C语言关键字的拓展，返回变量的**类型**，具体可以看GCC里面的介绍
https://gcc.gnu.org/onlinedocs/gcc/Typeof.html
例子：
`void main(void)
{
    int a = 6;
    typeof(a) b =9;
    printf("%d %d\n",a,b);
}`4.3. 0的作用
尺子大家应该都用过吧，比如我想用尺子量一本书本的长度，我们第一时间就需要找到尺子的0刻度的位置，然后用这个0刻度的位置去对准书本的边，然后再贴合对齐，在书本的另一边查看尺子刻度就可以知道书本的长度了。
现在我们需要量一个结构体的长度，我们也可以用尺子来量，我们只要找到这个0刻度的位置就可以了。同理，即使我们不知道0刻度位置，我们**首尾刻度相减**一样可以计算出结构体的长度。
`struct st{
    int a;
    int b;
}*p_st,n_st;
void main(void)
{
    printf("%p\n",&((struct st*)0)->b);
}`上面的代码
`(struct st*)0`这个的意思就是把这个结构体放到0刻度上面开始量了，然后量到哪里呢？
`&((struct st*)0)->b)`这个就体现出来了，量到b的位置。所以上面的输出应该是**4**。
看完上面的解释，应该知道下面这两个代码的功能是一样的。
`typeof ((struct st*)0)->b) c; // 取b的类型来声明c
int c;`其实不只是对于0，用其他数字一样是有效的，比如下面的代码，并没有任何问题，编译器关心的是类型，而不在乎这个数字。
`printf("%p\n",&((struct st*)4)->b  -4 );`4.4 offsetof(TYPE, MEMBER)
`#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)`size_t 这个有不懂的可以百度下，就是unsigned 的整数，在32位和64位下长度不同，所以这个offsetof就是获取结构体的**偏移长度**。
### 5 container_of 剖析
`#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
        const typeof( ((type *)0)->member ) *__mptr = (const typeof( ((type *)0)->member ) *)(ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})
//-----分割线
struct st{
    int a;
    int b;
}*pt;
//用这个来举例
container_of(&pt->a,struct st,a)
 const typeof( ((struct st *)0)->a ) *__mptr = (const typeof( ((struct st *)0)->a ) *)(&pt->a);
const int *__mptr = (int *)(&pt->a);//第一句解析完，实际上就是获取a的地址。
(type *)( (char *)__mptr - offsetof(type,member) );
//这个变成
(struct st *)( (char *)__mptr - ((unsigned int) &((struct st*)0)->a));
//这句的意思，把a的地址减去a对结构体的偏移地址长度，那就是结构体的地址位置了。`我画个图
![](http://plck4ljzb.bkt.clouddn.com/FqBwUR_7FNDbUdeCVw20eykNSxPd)

### 6 实例代码
`/*************************************************************************
        > File Name: 4.c
        > Author: 
        > Mail: 
        > Created Time: Mon 14 Jan 2019 04:47:24 PM CST
 ************************************************************************/
#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
/*ptr 成员指针
* type 结构体 比如struct Stu
* member 成员变量，跟指针对应
* */
#define container_of(ptr, type, member) ({          \
        const typeof( ((type *)0)->member ) *__mptr = (const typeof( ((type *)0)->member ) *)(ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})

typedef struct Stu{
        int age;
        char name[10];
        int id;
        unsigned long phone_num;
}*p_stu,str_stu;
void print_all(void *p_str)
{
    p_stu m1p_stu = NULL;
    m1p_stu = container_of(p_str,struct Stu,age);
    printf("age:%d\n",m1p_stu->age);
    printf("name:%s\n",m1p_stu->name);
    printf("id:%d\n",m1p_stu->id);
    printf("phone_num:%d\n",m1p_stu->phone_num);
}

void main(void)
{
    p_stu m_stu = (p_stu)malloc(sizeof(str_stu));
    m_stu->age = 25;
    m_stu->id  = 1;
    m_stu->name[0]='w';
    m_stu->name[1]='e';
    m_stu->name[2]='i';
    m_stu->name[3]='q';
    m_stu->name[4]='i';
    m_stu->name[5]='f';
    m_stu->name[6]='a';
    m_stu->name[7]='\0';
    m_stu->phone_num=13267;
    /*传结构体成员指针进去*/
    print_all(&m_stu->age);
    printf("main end\n");
    if(m_stu!=NULL)
        free(m_stu);
}`
### 程序输出
`age:25
name:weiqifa
id:1
phone_num:13267
main end`

