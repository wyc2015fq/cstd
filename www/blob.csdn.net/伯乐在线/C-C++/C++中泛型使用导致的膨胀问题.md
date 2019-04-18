# C++中泛型使用导致的膨胀问题 - 文章 - 伯乐在线
原文出处： [临峰不畏，2016-03-21](https://my.oschina.net/hevakelcj/blog/643893)
前几天，博主看了一篇文章抨击C++的泛型会导致生成的可执行文件代码臃肿。
博主从事C++软件开发多年，由于之前的开发环境都是资源充足的服务器，不用考虑磁盘空间的问题。最近打算在智能家居主机的嵌入式平台上使用C++进行开发。FLASH存储空间有限，这是必须要考虑的因素，一定要重视。
如下定义两个list，元素类型不同：


```
list<int> l1;
list<string> l2;
```
如果是用C语来做应该怎么办？它会对应list写一套代码，再对list写一套。每套都有相同的成员函数，只是变量类型各自不同罢了。
下面是list<int>的C语言实现方式：


```
//! code-1
struct list_int_item {
    int value;
    struct list_int_item *next;
};
struct list_int {
    struct list_int_item *head;
    size_t size;
};
void list_int_insert(struct list_int *p, int value);
int  list_int_sort(struct list_int *p);
bool list_int_empty(struct list_int *p);
...
```
下面是list<string>的C语言实现方式：


```
//! code-2
struct list_string_item {
    string value;
    struct list_string_item *next;
};
struct list_string {
    struct list_string_item *head;
    size_t size;
};
void list_string_insert(struct list_int *p, string value);
int  list_string_sort(struct list_int *p);
bool list_string_empty(struct list_int *p);
...
```
两者之间就是类型的差别。所以很多时间，在C语言中我们就用宏来替代它的类型，如下：


```
//! code-3
#define LIST_DECLARE(TYPE) 
    struct list_##TYPE##_item { 
        TYPE## value; 
        struct list_##TYPE##_item *next; 
    }; 
    
    struct list_##TYPE { 
        struct list_##TYPE##_item *head; 
        size_t size; 
    }; 
    
    void list_##TYPE##_insert(struct list_##TYPE *p, ##TYPE## value); 
    int  list_##TYPE##_sort(struct list_##TYPE *p); 
    bool list_##TYPE##_empty(struct list_##TYPE *p); 
    ...
```
然后在头文件中是这样定义list<double>的：


```
//! code-4
LIST_DECLARE(double)
```
所以，泛型产生冗余代码是无法避免的，至少用C来做这样的泛型也是无法避免的。
既然无法避免的，那就看看怎么尽可能以避免上述的问题。在《Effective C++》中有一章节专门提到：不要在模板中使用不必要的参数。因为每一个不同的参数编译器都会为之生成一套相应的代码。
如果代码中只有一种数据类型，就算用该类型定义了多个变量，编译器是不是只会生成一套相关的代码？（应该是这样的）。
写个例子对比一下：（省略不必要的代码）
test1.cpp，里面只有map<int, string>，但定义了m1, m2, m3。


```
//! code-5
    map<int, string> m1;
    map<int, string> m2;
    map<int, string> m3;
    m1.insert(std::make_pair(1, "hello"));
    m2.insert(std::make_pair(1, "hi"));
    m3.insert(std::make_pair(1, "lichunjun"));
```
test2.cpp，与test1.cpp相比，里面有三个类型：


```
//! code-6
    map<int, string> m1;
    map<int, double> m2;
    map<int, int> m3;
    m1.insert(std::make_pair(1, "hello"));
    m2.insert(std::make_pair(1, 1.2));
    m3.insert(std::make_pair(1, 44));
```
结果，编译出来的可执行文件大小比较：


```
[hevake_lcj@Hevake tmp]$ ll test1 test2
-rwxrwxr-x. 1 18784 Mar 19 22:01 test1
-rwxrwxr-x. 1 35184 Mar 19 22:03 test2
```
test2比test1大一倍，原因不用多说。
还有一个问题：指针是不是被认为是一个类型？
上面的list<int>与list<string>不能共用同一套代码，根据的原因是因为int与string这两种类型在空间大小与赋值的方式上都是不同的。所以，必须生成两套代码来实现。
而指针，不管是什么指针，它们都是一样的。我们可以用void*代表所有的指针类型。
于是我们将上面的代码改改，再测试一下：


```
//! code-7
    map<int, string*> m1;
    map<int, string*> m2;
    map<int, string*> m3;
    m1.insert(std::make_pair(1, new string("hello")));
    m2.insert(std::make_pair(1, new string("hi")));
    m3.insert(std::make_pair(1, new string("lichunjun")));
```
与


```
//! code-8
    map<int, string*> m1;
    map<int, double*> m2;
    map<int, int*> m3;
    m1.insert(std::make_pair(1, new string("hello")));
    m2.insert(std::make_pair(1, new double(1.2)));
    m3.insert(std::make_pair(1, new int(44)));
```
结果是这样的：


```
-rwxrwxr-x. 1 18736 Mar 19 23:05 test1
-rwxrwxr-x. 1 35136 Mar 19 23:05 test2
```
预期的结果test1与test2相差不多，但从结果上看并没有什么优化，结果有点令人失望～
思考：C++有没有什么参数可以优化这个？
如果没有，为了节省空间，我们只能将所有的指针统一定义成void*类型了，在使用时再强制转换。


```
//! code-9
    map<int, void*> m1;
    map<int, void*> m2;
    map<int, void*> m3;
    m1.insert(std::make_pair(1, new string("hello")));
    m2.insert(std::make_pair(1, new double(1.2)));
    m3.insert(std::make_pair(1, new int(44)));
    cout << *static_cast<string*>(m1[1]) << endl;
    cout << *static_cast<double*>(m2[1]) << endl;
    cout << *static_cast<int*>(m3[1]) << endl;
```
如上代码是将code-8的基础上，将所有的指定都定义成了void*，在使用的时候用static_cast进行强制转换成对应的指针类型。
如此得到的代码大小与code-7的比较，只多了16个字节。
但这种做法是很不可取的，必须用void*指针之后，编译器不再对类型进行检查，很容易把类型搞混淆。
最好还是编译器支持指针泛型的优化吧！
