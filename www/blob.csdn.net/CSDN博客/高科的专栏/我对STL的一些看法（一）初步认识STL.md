
# 我对STL的一些看法（一）初步认识STL - 高科的专栏 - CSDN博客

2013年12月09日 15:11:48[高科](https://me.csdn.net/pbymw8iwm)阅读数：6041


后面一段时间我将会给大家分享我自己学到STL以及应用的时候遇到的问题还有他的一些精髓，可能开始的逻辑会有些乱吧，不过后面还会不断的整理和优化，让自己看明白也让更多的读者看的清楚。
最近刚闲下来，先说说什么是STL： 不知道你是否有过这样的经历。在大学，你准备着手完成数据结构老师所布置的家庭作业时，或者在你为你所负责的某个软件项目中添加一项新功能时，你发现需要用到一个链表(List)或者是映射表（Map）之类的东西，但是手头并没有现成的代码。于是在你开始正式考虑程序功能之前，手工实现List或者Map是不可避免的。于是……，最终你顺利完成了任务。或许此时，作为一个具有较高素养的程序员的你还不肯罢休（或者是一个喜欢偷懒的优等生:），因为你会想到，如果以后还遇到这样的情况怎么办？没有必要再做一遍同样的事情吧！
如果说上述这种情形每天都在发生，或许有点夸张。但是，如果说整个软件领域里，数十年来确实都在为了一个目标而奋斗--可复用性（reusability），这看起来似乎并不夸张。从最早的面向过程的函数库，到面向对象的程序设计思想，到各种组件技术（如：COM、EJB），到设计模式（designpattern）等等。而STL也在做着类似的事情，同时在它背后蕴涵着一种新的程序设计思想--泛型化设计（generic programming）。
说了半天，到底STL是什么东西呢？
STL（StandardTemplate Library），即标准模板库，是一个具有工业强度的，高效的C++程序库。
STL 提供六大组件，彼此可以组合套用：
1.      容器（containers）：各种数据结构，如 vector, list, deque, set, map用来存放数据。，STL 容器是种 class template
2.      算法（algorithms）：各种常用算法如 sort, search, copy, erase…，从实作的角度看，STL 算法是种 functiontemplate。
3.      迭代器（iterators）：扮演容器与算法之间的胶着剂，是所谓的「泛型指标」，有五种类型，以及其它衍生变化。从实作的角度看，迭代器是㆒种将 operator*, operator->, operator++, operator-- 等指标相关操作予以多载化的class template。所有 STL 容器都附带有自己专属的迭代器 — 是的，只有容器设计者才知道如何巡访自己的元素。原生指标（nativepointer）也是㆒种迭代器。
4.      仿函数（functors）：行为类似函式，可做为算法的某种策略（policy），仿函式是㆒种重载了 operator()的 class 或class template。㆒般函式指标可视为狭义的仿函式。
5.      配接器（adapters）：用来修饰容器（containers）或仿函式（functors）或迭代器（iterators）接口的东西。例如 STL 提供的 queue 和stack，虽然看似容器，其实只能算是㆒种容器配接器，因为它们的底部完全借重 deque，所有动作都由底层的 deque 供应。改变 functor 接口者，称为 function adapter，改变 container 接口者，称为 container adapter，改变iterator 界面者，称为 iterator
 adapter。配接器的实作技术很难㆒言以蔽之，必须逐㆒分析。
6.      配置器（allocators）：负责空间配置与管理，配置器是㆒个实现了动态空间配置、空间管理、空间释放的 class template。
http://www.cplusplus.com/reference/stl/更加详细的资料
STL 提供给程序员以下三类数据结构的实现：
标准容器类
顺序性容器
vector 从后面快速的插入与删除，直接访问任何元素
deque 从前面或后面快速的插入与删除，直接访问任何元素
list 双链表，从任何地方快速插入与删除
关联容器
set 快速查找，不允许重复值
multiset 快速查找，允许重复值
map 一对多映射，基于关键字快速查找，不允许重复值
multimap 一对多映射，基于关键字快速查找，允许重复值
容器适配器
stack 后进先出
queue 先进先出
priority_queue 最高优先级元素总是第一个出列
程序员使用复杂数据结构的最困难的部分已经由STL完成. 如果程序员想使用包含int数据的stack, 他只要写出如下的代码:
stack<int> myStack;
接下来, 他只要简单的调用 push()和 pop()函数来操作栈. 借助 C++模板的威力, 他可以指定任何的数据类型，不仅仅是int类型. STL stack实现了栈的功能，而不管容纳的是什么数据类型
## 容器元素的初始化
### 将一个容器初始化为另一个的副本
将一个容器复制给另外一容器的时候,类型必须匹配,容器类型和元素类型必须相同,为什么呢？请大家思考一下。例如下面的例子就是错误的:
intmain () {
std::vector<char> vec1(10,1);
std::vector<float> vec2(vec1);
return 0;
}
我认为用一个容器去初始化另一个容器的时候调用的是构造函数，既然是拷贝构造函数，那么穿的参数容器和该容器本身应该是同一个容器类型吧

### 初始化为一段元素的副本
尽管不能直接将一种容器内的元素复制给另外一个容器,但是系统还是允许通过传递一个迭代器来初始化,不过这里的迭代器不需要他们的容器类型相同,容器内的元素类型也可以不相同,只要能相互兼容.
比如
**Test3.cpp**
\#include <iostream>     // std::cout
\#include <algorithm>    //std::replace
\#include <vector>       // std::vector
\#include <list>
using namespace std;
int main () {
vector<int>vec1(10,1);
list<int>vec2(vec1.begin(),vec1.end());
return 0;
}
**Test3.cpp**
\#include <iostream>     // std::cout
\#include <algorithm>    // std::replace
\#include <vector>       // std::vector
\#include <list>
using namespace std;
int main () {
vector<char> vec1(10,1);
vector<double> vec2(vec1.begin(),vec1.end());
return 0;
}
g++ -o test Test3.cpp
为什么呢？先别着急，后面的环节将会说道迭代器的细节。我们之暂时保留对他的用法认识
## 容器内元素类型约束
大多数类型都可以作为容器的元素类型。元素类型必须满足一下2个约束：
元素类型必须支持赋值运算（因为要牵扯到容器内元素的计算修改）
元素类型的对象必须可以复制
此外关联容器还有其他约束。
因为引用和IO库类型的不支持赋值运算
int main () {
floata=12;
vector<float&>vec1(10,a);
return 0;
}
这段代码有什么问题吗？（相信聪明的你已经发现了他的问题：因为这个容器里的每个元素类型都是引用类型的，但是我们又知道引用类型的不支持赋值预算，所以他是不合法的代码）

## 迭代器的运算符
迭代器之间的关系操作符比如>,<,>=,<=适用于vector和deque，想想为什么？因为他们的内存布局，他们的内存块都是连续分配的，所以可以直接想数组一样的操作对地址进行比较，但是像list，map就不一样的，你无法确定按顺序存放的元素的地址也是不是按顺序分配的内存呢。
因此底下的代码看看有什么问题呢：
list<int> l;
list<int>::iterator  it1=l.begin(), it2=l.end();
while(it1<it2){
//…..
}
如果你是一个高手，一眼就能看出问题来，因为我们所认识的list其实它就相当于链表，其内存并不是我们想象的那样连续分配的，所以对it1和it2的大小，我们不可而知，这将会产生什么后果，你懂得。下一章节我将会从顺序容器讲起。


