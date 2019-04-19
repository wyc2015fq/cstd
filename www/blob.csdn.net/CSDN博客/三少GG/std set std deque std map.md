# std::set||std::deque||std::map - 三少GG - CSDN博客
2011年10月12日 11:14:40[三少GG](https://me.csdn.net/scut1135)阅读数：2268标签：[vector																[string																[iterator																[insert																[pair																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=pair&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
**std::set**
# [C++标准库：std_set作为一个有序集合](http://blog.sina.com.cn/s/blog_4fc7368a0100q03t.html)
**摘要**：std::set作为标准库的一个关联容器，实现内部元素进行了排序，使用这特性可以对一组元素进行插入排序。std::set最初的设计是完成数学中“集合”的概念，它提供的接口也是如此。本文简单地介绍一下这一个标准库容器。
为了使用std::set，我只需要如下：
#include<set>
//定义一个整型数据的集合
std::set<int> set;
如同所有的STL容器一样，std::set的基本的使用是如此简单，即使使用看起来很高级的功能也并不难，STL是经过设计的调试可用的库。
下面我会展示一些简单的使用示例，这些是示例非常简单，首先介绍那个“排序”的用法：
#include<set>
int array[5]={12,
 34, 10, 98, 3};
constsize_t array_size=sizeof(array)/sizeof(array[0]);
std::set<int> set;
for(size_t i=0; i<array_size;++i)
{
//把数据插入集合，数据自动排序
        set.insert(array[i]);
}
//此时已经排序了，下面依次输出
std::set<int>::const_iterator b=set.begin();
for(; b!=set.end();++b)
{
        std::cout<<*b <<'**\n**';
}
在一般意识中，一个集合并没有太多的操作，所有在STL中，std::set也没有太多操作，它的排序是自动的，我们可以插入一个元素，也可以删除一个元素，也有迭代器。下面简单的示例包括std::set自身的多数的特性：
#include<set>
int array[5]={12,
 34, 10, 98, 3};
constsize_t array_size=sizeof(array)/sizeof(array[0]);
//一个新的定义容器的方法
std::set<int> set(array,
 array+array_size);
//插入一个元素
set.insert(23);
//移除一个元素
set.erase(10);
//移除一个元素不在集合中的元素
//此时什么也不做，n作为返回值将会是0
//指明没有元素移除
size_t n=set.erase(11);
//使用迭代器，找到一个元素
std::set<int>::const_iterator result=set.find(98);
std::set<int> other;
//交换两个集合的内容
std::swap(set,
 other);
//清除所有的内容
other.clear();
## [std::set一些例子程序](http://www.cnblogs.com/kex1n/archive/2010/10/06/1844691.html)
#include<set>
#include<iterator>
#include<iostream>
#include <algorithm>
using namespace std;
int main()
{
    set<int> eg1;
    eg1.insert(1);
    eg1.insert(100);
    eg1.insert(5);
    eg1.insert(1); //元素1因为已经存在所以set中不会再次插入1
    eg1.insert(10);
    eg1.insert(9);
    //遍历set，可以发现元素是有序的
    set<int>::iterator set_iter=eg1.begin();
    cout<<"Set named eg1:"<<endl;
    for(;set_iter!=eg1.end();set_iter++) cout<<*set_iter<<" ";
    cout<<endl;
    //使用size()函数可以获得当前元素个数
    cout<<"Now there are "<<eg1.size()<<" elements in the set eg1"<<endl;
    if(eg1.find(200)==eg1.end())//find()函数可以查找元素是否存在
        cout<<"200 isn't in the set eg1"<<endl;
**std::set成员函数及简要使用方法**
countsize_type count(const key_type& x)计算元素在容器中的个数，对于std::set为1（存在）或者0（不存在）。可用于判断元素是否存在
find查找指定
emptybool empty()判断当前容器是否为空
sizesize_type size()取得当前容器内元素个数
clearvoid clear()清空当前容器
begin声明
end声明
rbegin声明
rend声明
erasevoid erase(iterator position) 
void erase(iterator first, iterator last) 
size_type erase(const key_type& x) 
获取数据
1、std::set不提供下表操作符；
2、如果只是判断元素是否存在，可以使用count函数检查返回值；
3、如果需要获取元素值，可使用迭代器。*iterator就是该迭代器指向的值。
std::set<std::string> set_limit;
set_limit.insert(“User@123”);
set_limit.insert(“User@124”);
set_limit.insert(“User@125”);
//判断"User@124”是否在集合中
if (set_limit.count(“User@124”) == 1)
cout << “User@124存在于集合中"<< std::endl;
else
cout << “User@124不在集合中" << std::endl;
//输出集合中所有的元素值
for (std::set<std::string>::iterator it = set_limit.begin(); it != set_limit.end(); it++)
cout << *it << std::endl;
注意：在获得指向set中某元素的迭代器后，只能对其做读操作，而不能做写操作。
std::set<std::string> set_string; //元素类型是std::string，*迭代器得到的数值类型是std::string
std::set<int> set_int; //元素类型是int，*迭代器得到的数值类型是int
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**std::deque是一个高效的双端队列，可以高效地进行插入和删除操作。**
**[深入研究 STL Deque 容器](http://www.cnblogs.com/me115/archive/2010/10/15/1852432.html)**
本文档深入分析了std::deque，并提供了一个指导思想：当考虑到内存分配和执行性能的时候，使用std::deque要比std::vector好。
**介绍**
本文深入地研究了std::deque 容器。本文将讨论在一些情况下使用deque 比vector更好。读完这篇文章后读者应该能够理解在容量增长的过程中deque 与vector在内存分配和性能的不同表现。由于deque 和vector的用法很相似，读者可以参考[vector](http://www.codeproject.com/vcpp/stl/std_vector.asp) 的文档中介绍如何使用STL容器。
**Deque总览**
deque 和vector一样都是**标准模板库**中的内容，deque 是双端队列，在接口上和vector 非常相似，在许多操作的地方可以直接替换。假如读者已经能够有效地使用vector容器，下面提供deque的成员函数和操作，进行对比参考。
**Deque成员函数**
|函数|描述|
|----|----|
|c.assign(beg,end)c.assign(n,elem)|将[beg; end)区间中的数据赋值给c。将n个elem的拷贝赋值给c。|
|c.at(idx)|传回索引idx所指的数据，如果idx越界，抛出out_of_range。|
|c.back()|传回最后一个数据，不检查这个数据是否存在。|
|c.begin()|传回迭代器重的可一个数据。|
|c.clear()|移除容器中所有数据。|
|deque<Elem> cdeque<Elem> c1(c2)Deque<Elem> c(n)Deque<Elem> c(n, elem)Deque<Elem> c(beg,end)c.~deque<Elem>()|创建一个空的deque。复制一个deque。创建一个deque，含有n个数据，数据均已缺省构造产生。创建一个含有n个elem拷贝的deque。创建一个以[beg;end)区间的deque。销毁所有数据，释放内存。|
|c.empty()|判断容器是否为空。|
|c.end()|指向迭代器中的最后一个数据地址。|
|c.erase(pos)c.erase(beg,end)|删除pos位置的数据，传回下一个数据的位置。删除[beg,end)区间的数据，传回下一个数据的位置。|
|c.front()|传回地一个数据。|
|get_allocator|使用构造函数返回一个拷贝。|
|c.insert(pos,elem) c.insert(pos,n,elem)c.insert(pos,beg,end)|在pos位置插入一个elem拷贝，传回新数据位置。在pos位置插入>n个elem数据。无返回值。在pos位置插入在[beg,end)区间的数据。无返回值。|
|c.max_size()|返回容器中最大数据的数量。|
|c.pop_back()|删除最后一个数据。|
|c.pop_front()|删除头部数据。|
|c.push_back(elem)|在尾部加入一个数据。|
|c.push_front(elem)|在头部插入一个数据。|
|c.rbegin()|传回一个逆向队列的第一个数据。|
|c.rend()|传回一个逆向队列的最后一个数据的下一个位置。|
|c.resize(num)|重新指定队列的长度。|
|c.size()|返回容器中实际数据的个数。|
|C1.swap(c2) Swap(c1,c2)|将c1和c2元素互换。同上操作。|
上面这些特征和vector明显相似，所以我们会提出下面的疑问。
**问题：如果deque和vector可以提供相同功能的时候，我们使用哪一个更好呢？**
回答：如果你要问的话，就使用vector吧。
或者你给个解释？
非常高兴你这样问，的确，这并不是无中生有的，事实上，在C++标准里解释了这个问题，在23.1.1章节有下面一个片断：
**vector在默认情况下是典型的使用序列的方法，对于deque，当使用插入删除操作的时候是一个更好的选择**。
有趣的是，本文就是要非常彻底地理解这句话。
**什么是新的？**
细读上面两张表格，你会发现和vector比较这里增加了两个函数。
1、c.push_front(elem)—— 在头部插入一个数据。
2、c.pop_front() —— 删除头部数据。
调用方法和c.push_back(elem)和c.pop_back()相同，这些将来会告诉我们对于deque 会非常有用，deque可以在前后加入数据。
**缺少了什么？**
同时你也会发现相对于vector 缺少了两个函数，你将了解到deque 不需要它们。
1、 capacity() —— 返回vector当前的容量。
2、 reserve() —— 给指定大小的vector 分配空间。
......
在这篇文章中我们覆盖了多种不同的情况来选择我们到底是该使用vector还是deque。让我们总结一下测试的结果看下面几个结论。
**当执行大数据量的调用push_back()的时候，记住要调用vector::reserve()。**
在实验一中我们研究了vector和deque在插入数据的情况。通过这些假设，我们可以看出deque分配的空间是预先分配好的，deque维持一个固定增长率，在vector实验中我们考虑到应该调用vecor::reserve().然后在下面这个例子验证了我们的假设，在使用vector的时候调用reserve()能够膀子我们预先分配空间，这将是vector一个默认选择的操作。
**当你分配很多内存单元的时候，记住使用deque回收内存要比vector消耗时间多。**
在实验三中我们探讨了vector和deque在回收非邻接内存块上的不同，分别证明了vector在分配内存的时候是线性增长，而deque是指数增长，同样，vector要回收的内存比deque多的多，如果你循环调用了push_back()，那么deque将获取大量的内存，而且是临近的。我们通过测试发现在分配内存单元消耗的时间和vector的时间接近。
**如果你计划使用insert()，或者需要pop_front()，那就使用deque。**
由于vector没有提供pop_front()函数，但在实验四的结果中可以看出没有insert()是非常好的，同时**也告诉我们为什么deque在STL类中要作为单独的一个类划分出来**。
**对于访问数据，vector::at()效率最高。**
在实验五中统计的数据表示，所有访问数据方法的效率是非常接近的，但是vector::at()效率最高。这是因为最优的平衡图访问时间为最低的六个西格玛值。
**最后**
我希望本文能够带你认识deque，而且对它感兴趣或者一个启发，欢迎继续讨论关于vector和deque任何问题和内容。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**vector**    表示一段连续的内存区域，每个元素被顺序存储在这段内存中，对vector的随机访问效率很高，但对非末尾元素的插入和删除则效率非常低。
**deque**    也表示N段连续的内存区域组成，但与vector不同的是它支持高效地在其首部插入和删除元素，它通过两级数组结构来实现，一级表示实际的容器，第二级指向容器的首和尾  
**list**    表示非连续的内存区域并通过一对指向首尾元素的指针双向链接起来，插入删除效率高，随机访问效率低
注意点
1.当执行大数据量的调用push_back()的时候，记住要调用vector::reserve()。
2.如果你计划使用insert()，或者需要pop_front()，那就使用deque。
6.vector还有一个不好的地方,当你往一个vector里添加一个成员的时候,所有指向这个vector的原来成员的指针就不能保证有效了,因为 vector会re-alloc内存.而deque不会,无论从前面还是后面添加新成员,旧的成员都不会移动位置,这一点有时候很有用.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**std::map**
[STL入门教程三：map的使用](http://blog.csdn.net/shining100/article/details/5976924)
    map是C++标准库的关联容器之一，它是一种通过键值(key-value)对组合而成的容器。由于在容器内部对键值对中的键进行了特别的组织，所以我们可以通过搜索容器中的键,来快速的查询到我们所感兴趣的元素。同时，map中的键还具有唯一性。即map中如果存在待插入的键值，则此次的插入操作会失败。了解了这些基本的概念以后，让我们来结合map的具体使用方法，来加深对map的认识。
  接下来让我们来看一下map中的插入操作：
[view
 plain](http://blog.csdn.net/shining100/article/details/5976924#)
- pair<iterator,bool> insert ( const value_type& x );  
      说明：插入操作。需要注意的是map的insert操作和一般容器的insert操作有点不一样，它并不一定会将传入的键值对加入到容器中。它首先会查找容器中是否存在待插入的键值，如果不存在这样的键值，则将传入的键值对插入到map中；如果存在的话，则不会将键值对插入到map中。
      insert和operator[]是map提供的仅有两个能将新的元素插入到容器中的操作。可能有的同学会感到奇怪，与顺序容器的vector和list相比，map为什么没有提供push_back和push_front这样的操作呢？其实这是由关联容器的性质决定的，因为在关联容器中，元素的位置并不是由元素插入到容器中的先后顺序决定的，而是由一个特定的元素间的关系决定的。例如我们在构造一个map对象的时候，可以传入模板参数comp来定义这种关系。因此push_back和push_front这两个操作当然对于map就是没有意义的。
      好了让我们回到正题，来看一下第一个版本的insert操作中的参数和返回类型。
      参数：x。插入的键值对。可能细心的同学会对value_type这个类型感到有点奇怪，如果按照这个名字直译的话，不是一个值类型吗？即只包含键所对应的值，而没有包含键的值。不过，事实并不是这样的，value_type类型就是一个键值对类型。类型的定义如下：
[view
 plain](http://blog.csdn.net/shining100/article/details/5976924#)
- typedef pair<key, value> value_type  
       类型pair是STL中用来表示对的数据结构，我们可以通过它的数据成员first访问key，通过访问它的数据成员second访问value。
       真正的值类型的定义如下：
[view
 plain](http://blog.csdn.net/shining100/article/details/5976924#)
- typdef value mapped_type  
       直译过来为映射类型，跟传统的值类型的翻译有点不一样。不过为了符合大家的习惯，在本篇文章中还是用传统的键值对代表value_type，用值类型代表mapped_type。
       返回：pair<iterator,bool>。返回的是一个键值对。其中iterator表示元素在map中的迭代器，bool表示插入是否成功。如果成功的话为true，失败则为false。需要注意的是，如果插入失败的话，iterator表示该键值元素在map中的迭代器。
       让我们再来看一下第二个版本的插入操作：
    下面再让我们来看一下在map中比较常用的几个操作：
[view
 plain](http://blog.csdn.net/shining100/article/details/5976924#)
- iterator find ( const key_type& x );  
        说明：搜索整个map，判断键值x是否在map中存在。
        参数：x。待搜索的键值。
        返回：成功返回指向键值为x的元素的迭代器，失败返回map::end()。
[view
 plain](http://blog.csdn.net/shining100/article/details/5976924#)
- void erase ( iterator position );  
- size_type erase ( const key_type& x );  
- void erase ( iterator first, iterator last );  
        说明：删除操作。第一个版本在map中删除指向position的元素。第二个版本删除map中key的值为x的元素。第三个版本删除迭代器first(包含)到last(不包含)之间的所有元素。
        参数：position。指向待删除元素的迭代器。
        参数：x。待删除元素的键值。
        参数：first。指向待删除序列头的迭代器。
        参数：last。指向待删除序列尾的迭代器。
        返回：只有第二个版本的删除操作有返回值。其返回值表示在map中删除键值为x的元素个数，该值在map中只能为1。
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.yitsoft.com/chap_study/ch_00029/ch_00029.asp](http://www.yitsoft.com/chap_study/ch_00029/ch_00029.asp)
std map是STL的一个关联容器，它提供一对一（其中第一个可以称为关键字，每个关键字只能在map中出现一次，第二个可能称为该关键字的值）的数据处理能力，由于这个特性，它完成有可能在我们处理一对一数据的时候，在编程上提供快速通道。这里说下std
 map内部数据的组织，std map内部自建一颗红黑树(一种非严格意义上的平衡二叉树)，这颗树具有对数据自动排序的功能，所以在std map内部所有的数据都是有序的，后边我们会见识到有序的好处。
下面举例说明什么是一对一的数据映射。比如一个班级中，每个学生的学号跟他的姓名就存在着一一映射的关系，这个模型用map可能轻易描述，很明显学号用int描述，姓名用字符串描述(本篇文章中不用char *来描述字符串，而是采用STL中string来描述),下面给出map描述代码：
Map<int, string> mapStudent;
1. map的构造函数
map共提供了6个构造函数，这块涉及到内存分配器这些东西，略过不表，在下面我们将接触到一些map的构造方法，这里要说下的就是，我们通常用如下方法构造一个map：
Map<int, string> mapStudent;
2. 数据的插入
在构造map容器后，我们就可以往里面插入数据了。这里讲三种插入数据的方法：
第一种：用insert函数插入pair数据，下面举例说明(以下代码虽然是随手写的，应该可以在VC和GCC下编译通过，大家可以运行下看什么效果，在VC下请加入这条语句，屏蔽4786警告 ＃pragma warning (disable:4786) )
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
pStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第二种：用insert函数插入value_type数据，下面举例说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(map<int, string>::value_type (1, “student_one”));
mapStudent.insert(map<int, string>::value_type (2, “student_two”));
mapStudent.insert(map<int, string>::value_type (3, “student_three”));
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第三种：用数组方式插入数据，下面举例说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent[1] = “student_one”;
mapStudent[2] = “student_two”;
mapStudent[3] = “student_three”;
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
以上三种用法，虽然都可以实现数据的插入，但是它们是有区别的，当然了第一种和第二种在效果上是完成一样的，用insert函数插入数据，在数据的插入上涉及到集合的唯一性这个概念，即当map中有这个关键字时，insert操作是插入数据不了的，但是用数组方式就不同了，它可以覆盖以前该关键字对应的值，用程序说明
mapStudent.insert(map<int, string>::value_type (1, “student_one”));
mapStudent.insert(map<int, string>::value_type (1, “student_two”));
上面这两条语句执行后，map中1这个关键字对应的值是“student_one”，第二条语句并没有生效，那么这就涉及到我们怎么知道insert语句是否插入成功的问题了，可以用pair来获得是否插入成功，程序如下
Pair<map<int, string>::iterator, bool> Insert_Pair;
Insert_Pair = mapStudent.insert(map<int, string>::value_type (1, “student_one”));
我们通过pair的第二个变量来知道是否插入成功，它的第一个变量返回的是一个map的迭代器，如果插入成功的话Insert_Pair.second应该是true的，否则为false。
下面给出完成代码，演示插入成功与否问题
.
.
.
3. map的大小
在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：
Int nSize = mapStudent.size();
4. 数据的遍历
这里也提供三种方法，对map进行遍历
第一种：应用前向迭代器，上面举例程序中到处都是了，略过不表
第二种：应用反相迭代器，下面举例说明，要体会效果，请自个动手运行程序
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::reverse_iterator iter;
for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第三种：用数组方式，程序说明如下
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
int nSize = mapStudent.size()
//此处有误，应该是 for(int nIndex = 1; nIndex <= nSize; nIndex++) 
//by rainfish
for(int nIndex = 0; nIndex < nSize; nIndex++)
{
Cout<<mapStudent[nIndex]<<end;
}
}
5. 数据的查找（包括判定这个关键字是否在map中出现）
在这里我们将体会，map在数据插入时保证有序的好处。
要判定一个数据（关键字）是否在map中出现的方法比较多，这里标题虽然是数据的查找，在这里将穿插着大量的map基本用法。
这里给出三种数据查找方法
第一种：用count函数来判定关键字是否出现，其缺点是无法定位数据出现位置,由于map的特性，一对一的映射关系，就决定了count函数的返回值只有两个，要么是0，要么是1，出现的情况，当然是返回1了
第二种：用find函数来定位数据出现位置，它返回的一个迭代器，当数据出现时，它返回数据所在位置的迭代器，如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器，程序说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::iterator iter;
iter = mapStudent.find(1);
if(iter != mapStudent.end())
{
Cout<<”Find, the value is ”<<iter->second<<endl;
}
Else
{
Cout<<”Do not Find”<<endl;
}
}
第三种：这个方法用来判定数据是否出现，是显得笨了点，但是，我打算在这里讲解
Lower_bound函数用法，这个函数用来返回要查找关键字的下界(是一个迭代器)
Upper_bound函数用法，这个函数用来返回要查找关键字的上界(是一个迭代器)
例如：map中已经插入了1，2，3，4的话，如果lower_bound(2)的话，返回的2，而upper-bound（2）的话，返回的就是3
Equal_range函数返回一个pair，pair里面第一个变量是Lower_bound返回的迭代器，pair里面第二个迭代器是Upper_bound返回的迭代器，如果这两个迭代器相等的话，则说明map中不出现这个关键字，程序说明
.
.
.
6. 数据的清空与判空
清空map中的数据可以用clear()函数，判定map中是否有数据可以用empty()函数，它返回true则说明是空map
7. 数据的删除
这里要用到erase函数，它有三个重载了的函数，下面在例子中详细说明它们的用法
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
//如果你要演示输出效果，请选择以下的一种，你看到的效果会比较好
//如果要删除1,用迭代器删除
map<int, string>::iterator iter;
iter = mapStudent.find(1);
mapStudent.erase(iter);
//如果要删除1，用关键字删除
Int n = mapStudent.erase(1);//如果删除了会返回1，否则返回0
//用迭代器，成片的删除
//一下代码把整个map清空
mapStudent.earse(mapStudent.begin(), mapStudent.end());
//成片删除要注意的是，也是STL的特性，删除区间是一个前闭后开的集合
//自个加上遍历代码，打印输出吧
}
8. 其他一些函数用法
这里有swap,key_comp,value_comp,get_allocator等函数，感觉到这些函数在编程用的不是很多，略过不表，有兴趣的话可以自个研究
9. 排序
这里要讲的是一点比较高深的用法了,排序问题，STL中默认是采用小于号来排序的，以上代码在排序上是不存在任何问题的，因为上面的关键字是int型，它本身支持小于号运算，在一些特殊情况，比如关键字是一个结构体，涉及到排序就会出现问题，因为它没有小于号操作，insert等函数在编译的时候过不去，下面给出两个方法解决这个问题
第一种：小于号重载，程序举例
#include <map>
#include <string>
Using namespace std;
Typedef struct tagStudentInfo
{
Int nID;
String strName;
}StudentInfo, *PStudentInfo; //学生信息
Int main()
{
int nSize;
//用学生信息映射分数
map<StudentInfo, int>mapStudent;
map<StudentInfo, int>::iterator iter;
StudentInfo studentInfo;
studentInfo.nID = 1;
studentInfo.strName = “student_one”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));
studentInfo.nID = 2;
studentInfo.strName = “student_two”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));
for (iter=mapStudent.begin(); iter!=mapStudent.end(); iter++)
cout<<iter->first.nID<<endl<<iter->first.strName<<endl<<iter->second<<endl;
}
以上程序是无法编译通过的，只要重载小于号，就OK了，如下：
Typedef struct tagStudentInfo
{
Int nID;
String strName;
Bool operator < (tagStudentInfo const& _A) const
{
//这个函数指定排序策略，按nID排序，如果nID相等的话，按strName排序
If(nID < _A.nID) return true;
If(nID == _A.nID) return strName.compare(_A.strName) < 0;
Return false;
}
}StudentInfo, *PStudentInfo; //学生信息
第二种：仿函数的应用，这个时候结构体中没有直接的小于号重载，程序说明
#include <map>
#include <string>
Using namespace std;
Typedef struct tagStudentInfo
{
Int nID;
String strName;
}StudentInfo, *PStudentInfo; //学生信息
Classs sort
{
Public:
Bool operator() (StudentInfo const &_A, StudentInfo const &_B) const
{
If(_A.nID < _B.nID) return true;
If(_A.nID == _B.nID) return _A.strName.compare(_B.strName) < 0;
Return false;
}
};
Int main()
{
//用学生信息映射分数
Map<StudentInfo, int, sort>mapStudent;
StudentInfo studentInfo;
studentInfo.nID = 1;
studentInfo.strName = “student_one”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));
studentInfo.nID = 2;
studentInfo.strName = “student_two”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));
}
10. 另外
由于STL是一个统一的整体，map的很多用法都和STL中其它的东西结合在一起，比如在排序上，这里默认用的是小于号，即less<>，如果要从大到小排序呢，这里涉及到的东西很多，在此无法一一加以说明。
还要说明的是，map中由于它内部有序，由红黑树保证，因此很多函数执行的时间复杂度都是log2N的，如果用map函数可以实现的功能，而STL Algorithm也可以完成该功能，建议用map自带函数，效率高一些。
下面说下，map在空间上的特性，否则，估计你用起来会有时候表现的比较郁闷，由于map的每个数据对应红黑树上的一个节点，这个节点在不保存你的数据时，是占用16个字节的，一个父节点指针，左右孩子指针，还有一个枚举值（标示红黑的，相当于平衡二叉树中的平衡因子），我想大家应该知道，这些地方很费内存了吧，不说了……
