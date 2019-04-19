# STL - LC900730的博客 - CSDN博客
2017年05月23日 15:19:36[lc900730](https://me.csdn.net/LC900730)阅读数：181标签：[stl](https://so.csdn.net/so/search/s.do?q=stl&t=blog)
个人分类：[c语言](https://blog.csdn.net/LC900730/article/category/6901419)
## Vector
标准库类型vector表示对象的集合，其中所有对象类型都相同。集合中的每个对象都有一个与之对应的索引，索引用于访问对象。因为vector“容纳着其他对象”，所以它也常被称作容器。
```cpp
#include<vector>
using std::vector;
```
vector是一个类模板； 
模板本身不是类或者函数，它可以将模板看做为编译器生成类或函数编写的一份说明。编译器根据模板创建类或函数的过程称为实例化(instanitiation)。当使用模板时，需要指出编译器应把类或函数实例化成何种类型。 
对应类模板来说，我们通过提供一些额外信息来指定模板到底实例化成什么样的类，需要提供哪些信息由模板决定。提供信息的方式总是这样：即在模板名字后面跟一对尖括号，在括号内放上信息。
vector v1{“a”,”an”,”the”};   //列表初始化 
vector v2(“a”,”an”,”the”);   //错误
如果初始化的时候使用了花括号的形式但是提供的值又不能用来列表初始化，就要考虑用这样的值来构造vector对象。 
vector v7{10};//   尖括号提供的10不能给v7初始化，因此是v7有10个默认初始化的元素
### 向vector对象中添加元素
|v.push_back|压入元素到vector|
|----|----|
|v.empty()|如果v不包含任何元素，返回真；否则返回假|
|v.size()|返回v中元素的个数|
|v[n]|返回v中第n个位置上元素的引用|
|v1=v2|用v2中元素的拷贝替换v1中的元素|
|v1={a,b,c…}|用列表中元素的拷贝替换v1中的元素|
|v1==v2|v1和v2相等当且仅当它们元素数量相同且对应位置元素值相同|
|v1=v2|用v2中元素的拷贝替换v1中的元素|
|v1！=v2|判断是否相等|
|| |
```cpp
vector<int> v{1,2,3,4,5,6,7,8,9};
for(auto &i:v)
    i*=i;
for(auto i:v)
    cout<<i<<" ";
cout<<endl;
```
### 计算vector内对象的索引
string::size_type,作用域操作符表明名字是size_type是在类string中定义的。 
vector对象的下标也是0开始，下标类型是相应的size_type类型。只要vector对象不是一个常量，就能向下标运算符返回的元素赋值。 
不能用下标形式添加元素
ivec[ix]=ix;                 //❌，因为ivec不包含任何元素
### 3.迭代器
迭代器的对象是容器中的元素或者是string对象中的字符。使用迭代器可以访问某个元素，迭代器也能从一个元素移动到另外一个元素。
#### 3.4使用迭代器
这些都有begin和end成员，其中begin成员负责返回指向第一个元素的迭代器。
### 迭代器类型
string 和vector的size_type成员类型未知，实际上，那些拥有迭代器的标准库类型使用iterator和const_iterator来表示迭代器的类型； 
vector::iterator it ;   //it能读写vector的元素 
string::iterator it2;
vector::const_iterator it3;           //it3只能读元素，不能写元素 
string::const_iterator it4;                //it4只能读字符，不能写字符
如果vector对象或者string对象是一个常量，只能使用const_iterator;如果vector对象或string对象不是常量，那么既然使用iterator也能使用const_iterator。 
begin和end的返回具体类型由对象是否是常量决定，如果对象是常量，begin和end返回const_iterator;如果对象不是常量，返回iterator 
为了便于专门得到const_iterator类型的返回值，c++11新标准引入了两个新函数，分别是cbegin和cend；
解引用迭代器可以获得迭代器所指向的对象，如果该对象的类型恰好是类，就有可能希望进一步访问它的成员。 
凡是使用了迭代器的循环体，都不要向迭代器所属的容器中添加元素。
## STL的概念
## Iterator(迭代器)
## Container(容器)
## Algorithm(算法)
## Adaptors(适配器)
