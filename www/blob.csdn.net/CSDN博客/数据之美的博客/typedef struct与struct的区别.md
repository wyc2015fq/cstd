# typedef struct与struct的区别 - 数据之美的博客 - CSDN博客
2017年07月22日 13:01:55[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：111
个人分类：[数据结构																[算法](https://blog.csdn.net/oppo62258801/article/category/6551325)](https://blog.csdn.net/oppo62258801/article/category/6551324)
1. 基本解释
　　typedef为C语言的关键字，作用是为一种数据类型定义一个新名字。这里的数据类型包括内部数据类型（int,char等）和自定义的数据类型（struct等）。
　　在编程中使用typedef目的一般有两个，一个是给变量一个易记且意义明确的新名字，另一个是简化一些比较复杂的类型声明。
　　至于typedef有什么微妙之处，请你接着看下面对几个问题的具体阐述。
　　2. typedef & 结构的问题
　　当用下面的代码定义一个结构时，编译器报了一个错误，为什么呢？莫非C语言不允许在结构中包含指向它自己的指针吗？请你先猜想一下，然后看下文说明：
typedef struct tagNode
{
　char *pItem;
　pNode pNext;
} *pNode; 
　　答案与分析：
　　1、typedef的最简单使用
typedef long byte_4;
　　给已知数据类型long起个新名字，叫byte_4。
　　2、 typedef与结构结合使用
typedef struct tagMyStruct
{ 
　int iNum;
　long lLength;
} MyStruct;
　　这语句实际上完成两个操作：
　　1) 定义一个新的结构类型
struct tagMyStruct
{ 
　int iNum; 
　long lLength; 
};
　　分析：tagMyStruct称为“tag”，即“标签”，实际上是一个临时名字，struct 关键字和tagMyStruct一起，构成了这个结构类型，不论是否有typedef，这个结构都存在。
　　我们可以用struct tagMyStruct varName来定义变量，但要注意，使用tagMyStruct varName来定义变量是不对的，因为struct 和tagMyStruct合在一起才能表示一个结构类型。
　　2) typedef为这个新的结构起了一个名字，叫MyStruct。
typedef struct tagMyStruct MyStruct;
　　因此，MyStruct实际上相当于struct tagMyStruct，我们可以使用MyStruct varName来定义变量。
　　答案与分析
　　C语言当然允许在结构中包含指向它自己的指针，我们可以在建立链表等数据结构的实现上看到无数这样的例子，上述代码的根本问题在于typedef的应用。
　　根据我们上面的阐述可以知道：新结构建立的过程中遇到了pNext域的声明，类型是pNode，要知道pNode表示的是类型的新名字，那么在类型本身还没有建立完成的时候，这个类型的新名字也还不存在，也就是说这个时候编译器根本不认识pNode。
　　解决这个问题的方法有多种：
　　1)、
typedef struct tagNode 
{
　char *pItem;
　struct tagNode *pNext;
} *pNode;
　　2)、
typedef struct tagNode *pNode;
struct tagNode 
{
　char *pItem;
　pNode pNext;
};
　　注意：在这个例子中，你用typedef给一个还未完全声明的类型起新名字。C语言编译器支持这种做法。
　　3)、规范做法：
struct tagNode
{
　char *pItem;
　struct tagNode *pNext;
};
typedef struct tagNode *pNode;
