# POSITION用法解释 - L_Andy的专栏 - CSDN博客

2013年12月23日 18:55:38[卡哥](https://me.csdn.net/L_Andy)阅读数：8582



POSITION是MFC模板类库中经常使用的一个数据类型，我们从它的定义可以看出，其实，它就是一个指针。

// abstract iteration position

struct __POSITION { };

typedef __POSITION* POSITION;

MFC给出的注释是：一个抽象的迭代位置，既然是抽象，也就是没有给定具体的数据类型，以满足不同的模板参数的需要。

在CList中，经常会用到POSITION作为引用参数，或者作为返回值。下面，我们就举2个简单的例子，来帮助大家理解这个数据类型，不过在此之前，我要啰嗦一下，因为有很多人对__POSITION这个空结构体，表示怀疑，似乎这成为了他们理解的障碍。既然是一个空的结构体，怎么能用它来定义指针呢？

其实，C++编译器，不会将一个空的结构体完全看成是空的，编译器会为它分配1byte的内存，其实说白了__POSITION这个结构体，就相当于一个unsigned char的类型，所以__POSITION也就相当于是unsigned char，也就是Windows里面的BYTE类型，从而POSITION也就有了一个可以等价的形式，那就是BYTE*。

我们举CList中2个典型的成员函数作为讲解的重点，当然，大家应该知道CList是一个链表数据结构。下面看这两个函数：

1. POSITION GetHeadPosition() const;

显而易见，这个函数，是用来获取链表头的，它的返回值是一个POSITION，其实也就是一个指针。这个指针代表了谁？当然，就是你一直想要的链表头在链表中的指针，这个指针是new出来的，你绝对不能擅自的将这个指针delete掉。

2. TYPE& GetAt(POSITION position);

这个函数，看起来，比较怪。它的唯一的入参是一个指针，其实，有可能就是你刚刚用GetHeadPosition获得的头节点指针。这个函数的意思，也就是，需要你提供一个节点的地址，然后，CList会遍历整条链，去寻找并返回符合这个地址的节点数据。

废话了那么多，一言以蔽之，毕其功于一役，换句话说就是：

POSITION，你可以把它看成是一个学号，有了它，你就可以找到任何一个班里的学生。

POSITION在MFC里用来存放各种List或Array对象的索引，便于在遍历时标识元素的位置。其实就是一个32位值，其内容可能是一个指针，或是数组的Index。


