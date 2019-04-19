# 如何写出优美的 C 代码 - 文章 - 伯乐在线
原文出处： [邱俊涛（@正反反长）](http://www.ibm.com/developerworks/cn/linux/l-cn-cobject/index.html)
面向对象的语言更接近人的思维方式，而且在很大程度上降低了代码的复杂性，同时提高了代码的可读性和可维护性，传统的 C 代码同样可以设计出比较易读，易维护，复杂度较低的优美代码，本文将通过一个实际的例子来说明这一点。
## 基础知识
### 结构体
除了提供基本数据类型外，C 语言还提供给用户自己定制数据类型的能力，那就是结构体，在 C 语言中，你可以用结构体来表示任何实体。结构体正是面向对象语言中的类的概念的雏形，比如：


```
typedef struct{ 
    float x; 
    float y; 
 }Point;
```
定义了一个平面坐标系中的一个点，点中有两个域，x 坐标和 y 坐标。
结构体中的域称为结构体的*成员*。结构体中的数据类型可以是简单数据类型，也可以是其他的结构体，甚至结构体本身还可以嵌套，比如，一个标准的链表结构可以进行如下定义：

C
```
typedef struct node{ 
    void *data;// 数据指针
    int dataLength;// 数据长度
    struct node *next;// 指向下一个节点
 }Node;
```
可以看到，结构体 node 中的 next 指针的类型又是 node 类型。
### 函数指针
指针是 C 语言的灵魂，是 C 比其他语言更灵活，更强大的地方。所以学习 C 语言必须很好的掌握指针。函数指针，即指向函数在内存映射中的首地址的指针，通过函数指针，可以将函数作为参数传递给另一个函数，并在适当的时候调用，从而实现异步通信等功能。
比如， UNIX/Linux 系统中的信号注册函数，其原型如下：

C
```
void (*signal(int signo,void (*func)(int))) (int)
```
使用的时候，需要自己在外部定义一个信号处理函数 (signal handler), 然后使用 signal(sigNo, handler) 将处理程序注册在进程上，当信号发生时，进程就可以回调信号处理函数。
### 将函数指针作为结构体的成员
正如前面提到的，结构体的成员可以是简单的数据结构，也可以是其他的结构体，当然，也可以是指针。当将函数指针作为结构体的成员，并且这些函数只用来操作*本*结构体中的数据时，就可以形成一个独立的实体，这个实体中既有数据，也有对数据的操作，这样自然就可以引出**类**`**(class)**`的概念。
## 面向对象语言的特性
一般而言，继承，封装和多态被认为是面向对象语言所必须支持的三种特征，也正是通过这三种特征才可以体现出面向对象在哪些方面优于面向过程。由于语言开发商的宣传或其他的各种原因，使的表面上面向对象的思想要通过语言为载体而得以实现，然而实际上，面向对象是一种软件设计思想，完全是可以与具体实现无关的。
虽然如此，但是不可否认，这些所谓的纯面向对象的语言，在其代码的可读性以及与人的自然思维的匹配方面，比面向过程的语言要好的多。
### 语言层次的面向对象
我们一般要描述一个对象，一般需要描述这个对象的一些属性，比如盒子`(box) 是一个实体，它有 6 个面，有颜色，重量，是否为空等属性，并且可以``*放*``东西进去，可以``*取*``东西出来。在面向对象的语言中，通常将这样的对象抽象成一个类 (class):`


C
```
class Box{ 
    clolr color; 
    int weight; 
    boolean empty; 
    put(something); 
    something get(); 
 }
```
对盒子进行操作时，可以做一下动作：


C
```
Box.put(cake); 
 Box.get();// 取到某个东西，从盒子中。
```
而面向过程的语言中，通常是将实体传递给一个贯穿全局的函数来进行的，同样以 Box 为例，对 Box 进行操作时，往往是这样：


C
```
Put(Box, cake);// 将一个蛋糕放到盒子中
 Get(Box);// 从盒子中取出某个东西来
```
而显然，第一种代码形式更符合常理，所以面向对象的语言大都提供这种语言层面的细节的支持，使得代码的可读性，可理解性大大增加。 C 语言，作为一个灵活而简单的语言，我们完全可以通过 C 提供的简单机制，实现这样的比较优美的代码形式。
## C 语言的面向对象
如前所说，面向对象是一种软件设计的思想，是语言无关的。在本节中，我举一个链表`(list)`的例子来说明如何在 C 语言中的设计出有面向对象风格的代码。
### 定义接口
接口是面向对象语言中的一个比较重要的概念，接口只对外部承诺实现该接口的实体可以完成什么样的功能，但是不暴露实现的方式。这样的好处是，实现者可以在不接触接口使用者的代码的情况下，对实现进行调整。
我们来看看链表的接口定义：
##### 清单 1. 链表的接口定义


C
```
#ifndef _ILIST_H 
 #define 	 _ILIST_H 
 // 定义链表中的节点结构
 typedef struct node{ 
    void *data; 
    struct node *next; 
 }Node; 
 // 定义链表结构
 typedef struct list{ 
    struct list *_this; 
    Node *head; 
    int size; 
    void (*insert)(void *node);// 函数指针
    void (*drop)(void *node); 
    void (*clear)(); 
    int (*getSize)(); 
    void* (*get)(int index); 
    void (*print)(); 
 }List; 
 void insert(void *node); 
 void drop(void *node); 
 void clear(); 
 int getSize(); 
 void* get(int index); 
 void print(); 
 #endif 	 /* _ILIST_H */
```
IList 接口中，可以清晰的看到，对于一个 list 实体 ( 也就是对象 ) 来说，可以在*其上*进行 insert, drop, clear, getSize, get(index) 以及 print 等操作。
### 接口的实现
##### 清单 2. 构造方法


C
```
Node *node = NULL; 
 List *list = NULL; 
 void insert(void *node); 
 void drop(void *node); 
 void clear(); 
 int getSize(); 
 void print(); 
 void* get(int index); 
 List *ListConstruction(){ 
    list = (List*)malloc(sizeof(List)); 
    node = (Node*)malloc(sizeof(Node)); 
    list->head = node; 
    list->insert = insert;// 将 insert 函数实现注册在 list 实体上
    list->drop = drop; 
    list->clear = clear; 
    list->size = 0; 
    list->getSize = getSize; 
    list->get = get; 
    list->print = print; 
    list->_this = list;// 用 _this 指针将 list 本身保存起来
    return (List*)list; 
 }
```
需要注意的是此处的 _this 指针，_this 指针可以保证外部对 list 的操作映射到对 _this 的操作上，从而使得代码得到简化。
##### 清单 3. 插入及删除


C
```
// 将一个 node 插入到一个 list 对象上
 void insert(void *node){ 
    Node *current = (Node*)malloc(sizeof(Node)); 
    current->data = node; 
    current->next = list->_this->head->next; 
    list->_this->head->next = current; 
    (list->_this->size)++; 
 } 
 // 删除一个指定的节点 node 
 void drop(void *node){ 
    Node *t = list->_this->head; 
    Node *d = NULL; 
    int i = 0; 
    for(i;i < list->_this->size;i++){ 
        d = list->_this->head->next; 
        if(d->data == ((Node*)node)->data){ 
            list->_this->head->next = d->next; 
            free(d); 
            (list->_this->size)--; 
            break; 
        }else{ 
            list->_this->head = list->_this->head->next; 
        } 
    } 
    list->_this->head = t; 
 }
```
其他的实现代码可以参看下载部分，这里限于篇幅就不再意义列举出来。
### 测试
**测试代码**
好了，前面做的一切工作都是为了保证我们的暴露给使用者的 API 可以尽量的简洁，优美，现在到测试的时候了：
##### 清单 4. 测试代码


C
```
int main(int argc, char** argv) { 
    List *list = (List*)ListConstruction();// 构造一个新的链表
 // 插入一些值做测试
    list->insert("Apple"); 
    list->insert("Borland"); 
    list->insert("Cisco"); 
    list->insert("Dell"); 
    list->insert("Electrolux"); 
    list->insert("FireFox"); 
    list->insert("Google"); 
    list->print();// 打印整个列表
    printf("list size = %d\n",list->getSize()); 
    Node node; 
    node.data = "Electrolux"; 
    node.next = NULL;  
    list->drop(&node);// 删除一个节点
    node.data = "Cisco"; 
    node.next = NULL; 
    list->drop(&node);// 删除另一个节点
    list->print();// 再次打印
    printf("list size = %d\n",list->getSize()); 
    list->clear();// 清空列表
    return 0; 
 }
```
##### 图 1. 运行结果
![运行结果](http://ww3.sinaimg.cn/mw690/0064cTs2jw1ezvcsyh916g309y0740sj.gif)
## 结束语
C 语言所诞生的`*UNIX*`平台提倡这样一种设计哲学：尽量进行简单的设计，让使用者如同搭积木一样的将这些简单的工具连接成强大的，完整的应用。 应该说，C 比较好的继承了这一点，C 语言非常简洁，非常强大，而由于 C 语言诞生的比较早，当时的面向对象的思想还不成熟，所以出现了大量的过程式的 C 应用，从而给人们一种 C 语言是面向过程的语言的错觉，其实 C 只是提供了一些简单，强大而通用的能力，至于你想将其搭成什么样的积木，则全靠你自己了。
