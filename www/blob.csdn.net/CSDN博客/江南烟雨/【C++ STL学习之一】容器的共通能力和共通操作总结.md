# 【C++ STL学习之一】容器的共通能力和共通操作总结 - 江南烟雨 - CSDN博客
2012年04月08日 20:00:11[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2345标签：[c++																[reference																[优化																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C/C++](https://blog.csdn.net/xiajun07061225/article/category/723127)
**一、C++ STL各容器有一些共通的能力**
其中三个最核心的能力是：
**（1）所有容器提供的是“value语意”而不是“reference语意”。**
容器进行元素的安插操作时，内部实施的是拷贝操作，存放的是元素的副本。**因此容器的每一个元素必须能被拷贝**。
**如果打算存放的对象不具备public 拷贝构造函数或者你要得不是对象的副本，那么容器元素只能是指针**。
**（2）所有元素形成一个次序。**
我们可以以相同顺序一次或者多次遍历每个元素。每个容器都提供可返回迭代器的函数，运用这些迭代器可以遍历元素。
**（3）一般来说，各项操作并非绝对安全。**
调用者必须确保传给操作函数的参数符合需求。违反需求的会导致未定义的行为。而且STL通常不会自己跑出异常。
**二、容器的共通操作**
下表列出容器的共通操作函数：
![](https://img-my.csdn.net/uploads/201204/08/1333882806_1355.jpg)
**（1）初始化**
每个容器类别都提供了一个默认构造函数、一个拷贝构造函数和一个析构函数。
以另一个容器的元素为初值，进行初始化：
list<int> intList;
vector<float> floatVec(intList.begin(),intList.end());
以一个数组的元素为初值进行初始化：
int array[] = {1,2,3,4,5};
set<int> intSet(array,array + sizeof(array) / sizeof(array[0]));
从标准输入获取元素进行初始化：
deque<int> c((istream_iterator<int>(cin)),
(istream_iterator<int>()));
注意：不能遗漏了初始化参数的那对多余的括号：
不能是这样的：
deque<int> c(istream_iterator<int>(cin),
istream_iterator<int>());
否则，表达式被认为是一个函数c的声明。它的一个参数的型别是istream_iterator<int>，参数名是cin。第二个参数无名，型别是istream_iterator<int>。函数的返回值型别是istream_iterator<int>。
但是，我们通过加上一对括号，就可以使参数istream_iterator<int>(cin)不再符合声明语法。
因为C++标准中的语法不接受这样的函数声明形式：
**T name((U), (V));**
**（2）与大小相关的操作函数**
1、size()
返回当前元素的数量
2、empty()
等价于size() == 0.但是其效率可能会高些。所以应该尽量使用empty()。
3、max_size()
返回容器所能容纳的最大元素数量。其值会因实作版本不同而不同。
**（3）比较**
包括常用的==，!=,<,<=,>,>=。
注意：比较操作的两端即两个容器必须属于同一型别。如果容器的所有元素依次序相等，则两个容器相等。采用operator==检查元素是否相等。采用字典式书序比较原则来判断某个容器是否小于另一个容器。
**（4）赋值和swap交换**
当对容器赋值元素时，源容器的所有元素被拷贝到目标容器内，后者的所有元素完全被移除。所以，赋值操作代价较高。
如果两个容器的型别相同，而且拷贝后源容器不在使用，可以使用swap进行优化。它的性能要优异很多。事实上它只交换某些内部指针，所以时间复杂度是常数，而赋值操作的复杂度是线性的。
