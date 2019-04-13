
# 我对STL的一些看法（二）认识vector容器 - 高科的专栏 - CSDN博客

2013年12月09日 15:35:51[高科](https://me.csdn.net/pbymw8iwm)阅读数：1191标签：[stl																](https://so.csdn.net/so/search/s.do?q=stl&t=blog)个人分类：[STL																](https://blog.csdn.net/pbymw8iwm/article/category/1793843)[C/C++																](https://blog.csdn.net/pbymw8iwm/article/category/910215)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1793843)


先说vector吧。

## C++ Vector（向量容器）
是一个线性顺序结构。相当于数组，但其大小可以不预先指定，并且自动扩展。它可以像数组一样被操作，由于它的特性我们完全可以将vector 看作动态数组。
vector 的数据安排以及操作方式，与 array 非常像似。两者的唯㆒差别在于空间的运用弹性。array是静态空间，㆒旦配置了就不能改变；要换个大（或小）㆒点的房子，可以，㆒切细琐得由客端自己来：首先配置㆒块新空间，然后将元素从旧址㆒㆒搬往新址，然后再把原来的空间释还给系统。vector 是动态空间，随着元素的加入，它的内部机制会自行扩充空间以容纳新元素。因此，vector 的运用对于内存的樽节与运用弹性有很大的帮助，我们再也不必因为害怕空间不足而㆒开始就要求㆒个大块头 array 了，我们可以安心使用vector，吃多少用多少。
vector 的实作技术，关键在于其对大小的控制以及重新配置时的数据搬移效率。㆒旦 vector 旧有空间满载，如果客端每新增㆒个元素，vector 内部只是扩充元素的空间，实为不智，因为所谓扩充空间（不论多大），在创建一个vector 后，它会自动在内存中分配一块连续的内存空间进行数据存储，初始的空间大小可以预先指定也可以由vector 默认指定，这个大小即capacity （）函数的返回值。当存储的数据超过分配的空间时vector 会重新分配一块内存块，但这样的分配是很耗时的，在重新分配空间时它会做这样的动作：
首先，vector 会申请一块更大的内存块；(2的指数级增长)
然后，将原来的数据拷贝到新的内存块中；
其次，销毁掉原内存块中的对象（调用对象的析构函数）；
最后，将原来的内存空间释放掉。
那好,我们来看看这段代码有什么问题呢:
void main( )
{
vector<int> vec;
vec.push_back(1);
int*p = &vec[0];
vec.push_back(1);
vec.push_back(1);
vec.push_back(1);
cout<<*p<<endl;
cout<<sizeof(vec)<<","<<vec.capacity()<<","<<vec.size()<<endl;
}
原来这里的p指向的是vec的第一个元素的地址,那当vec不断的插入数据,内存布局和地址就可能会改变,所以,这里指向的原来的vec[0]的地址已经不是现在的vec[0]的地址了,物是人非,那*p获得的肯定是个非法的数据,里边是什么东西我也不知道哦.因此以后要慎重使用指向vector容器元素的指针.

### vector 的特点：
(1) 指定一块如同数组一样的连续存储，但空间可以动态扩展。即它可以像数组一样操作，并且可以进行动态操作。通常体现在push_back()pop_back() 。
(2) 随机访问方便，它像数组一样被访问，即支持[ ] 操作符和vector.at()
(3) 节省空间，因为它是连续存储，在存储数据的区域都是没有被浪费的，但是要明确一点vector 大多情况下并不是满存的，在未存储的区域实际是浪费的。
(4) 在内部进行插入、删除操作效率非常低，这样的操作基本上是被禁止的。Vector 被设计成只能在后端进行追加和删除操作，其原因是vector 内部的实现是按照顺序表的原理。
(5) 只能在vector 的最后进行push 和pop ，不能在vector 的头进行push 和pop 。
(6) 当动态添加的数据超过vector 默认分配的大小时要进行内存的重新分配、拷贝与释放，这个操作非常消耗性能。 所以要vector 达到最优的性能，最好在创建vector 时就指定其空间大小。
Vectors 包含着一系列连续存储的元素,其行为和数组类似。访问Vector中的任意元素或从末尾添加元素都可以在常量级时间复杂度内完成，而查找特定值的元素所处的位置或是在Vector中插入元素则是线性时间复杂度

### vector 的函数：
1.Constructors 构造函数
vector<int> v1; //构造一个空的vector
vector<int> v1( 5, 42 ); //构造了一个包含5个值为42的元素的Vector
2.Operators 对vector进行赋值或比较
C++ Vectors能够使用标准运算符: ==, !=, <=, >=,<, 和 >.
要访问vector中的某特定位置的元素可以使用 [] 操作符.
两个vectors被认为是相等的,如果:
1.它们具有相同的容量
2.所有相同位置的元素相等.
vectors之间大小的比较是按照词典规则.
3.assign() 对Vector中的元素赋值
语法：
void assign( input_iterator start, input_iterator end );
// 将区间[start, end)的元素赋到当前vector
void assign( size_type num, const TYPE &val );
// 赋num个值为val的元素到vector中,这个函数将会清除掉为vector赋值以前的内容.
4.at() 返回指定位置的元素
语法：
TYPE at( size_type loc );//差不多等同v[i];但比v[i]安全;,到后边有例子说明
5.back() 返回最末一个元素
6.begin() 返回第一个元素的迭代器
7.capacity() 返回vector所能容纳的元素数量(在不重新分配内存的情况下）
8.clear() 清空所有元素
9.empty() 判断Vector是否为空（返回true时为空）
10.end() 返回最末元素的迭代器(译注:实指向最末元素的下一个位置)
11.erase() 删除指定元素
语法：
iterator erase( iterator loc );//删除loc处的元素
iterator erase( iterator start, iterator end );//删除start和end之间的元素
那好,既然删除了元素,那我们来看看他的空间是不是也缩小了或者他的大小
void main( )
{
vector<int> vec1;
vec1.push_back(1);
vec1.push_back(2);
vec1.push_back(3);
cout<<vec1.capacity()<<","<<vec1.size()<<endl;
vec1.erase(vec1.begin());
cout<<vec1.capacity()<<","<<vec1.size()<<endl;
vec1.erase(vec1.begin());
cout<<vec1.capacity()<<","<<vec1.size()<<endl;
cout<<vec1[2];//在这里我们应该看到了，如果用vec1.at(2)访问肯定是非法的数据
}
从这个简单的例子说明了一个问题，在erase删除元素的时候，并不会回收capacity容量的大小的，只是将元素向前移位，并且修改了size的大小，并不是真正意义上的删除，另外为了安全的访问最好使用at()函数而不是下标访问。
12.front() 返回第一个元素的引用
13.get_allocator() 返回vector的内存分配器
14.insert() 插入元素到Vector中
语法：
iterator insert( iterator loc, const TYPE &val );
//在指定位置loc前插入值为val的元素,返回指向这个元素的迭代器,
void insert( iterator loc, size_type num, const TYPE &val );
//在指定位置loc前插入num个值为val的元素
void insert( iterator loc, input_iterator start, input_iterator end );
//在指定位置loc前插入区间[start, end)的所有元素
15.max_size() 返回Vector所能容纳元素的最大数量（上限）
16.pop_back() 移除最后一个元素
17.push_back() 在Vector最后添加一个元素
18.rbegin() 返回Vector尾部的逆迭代器
19.rend() 返回Vector起始的逆迭代器
20.reserve() 设置Vector最小的元素容纳数量



