# Effective STL 学习笔记 - xiaoyu15407的博客 - CSDN博客
2015年06月07日 23:50:10[xiaoyu15407](https://me.csdn.net/xiaoyu15407)阅读数：241
### 条款1 : 仔细选择你的容器
- 标准STL序列容器：vector、string、deque和list.
- 标准STL关联容器：set、multiset、map和multimap.
- 非标准序列容器slist和rope.slist是一个单向链表,rope本质上是一个重型字符串
- 非标准关联容器hash_set、hash_multiset、hash_map和hash_multimap
- vector可以作为string的替代品
- vector作为标准关联容器的替代品
- 几种标准非STL容器,包括数组、bitset、valarray、stack、queue和priority_queue
- 连续内存容器 
- 如果一个新元素被插入或者已存元素被删除,其他在同一个内存块的元素就必须向上或者向下移动来为新元素提供空间或者填充原来被删除的元素所占的空间.这种移动影响了效率和异常安全.标准的连续内存容器是vector,string和deque.非标准的rope也是连续内存容器.
- 基于节点的容器 
- 在每个内存块 (动态分配) 中只保存一个元素.容器元素的插入或删除只影响指向节点的指针,而不是节点自己的内容.所以当有东西插入或删除时,元素值不需要移动,表现为链表的容器.list和slist——是基于节点的,所有的标准关联容器也是,它们的典型实现是平衡树,非标准的散列容器使用不同的基于节点的实现
- 容器选择的一些标准 
- 是否需要可以在容器的任意位置插入一个新元素？如果是选择序列容器,关联容器做不到.
- 是否关心元素在容器中的顺序？如果不散列容器就是可行的选择.否则要避免使用散列容器.
- 必须使用标准C++中的容器吗？如果是,就可以除去散列容器、slist和rope.
- 你需要哪一类迭代器？如果必须是随机访问迭代器,你就只能限于vector、deque和string如果需要双向迭代器, 
你就用不了slist和散列容器的一般实现
- 当插入或者删除数据时,是否非常在意容器内现有元素的移动？如果是,你就必须放弃连续内存容器
- 容器中的数据的内存布局需要兼容C吗？如果是,你就只能用vector
- 查找速度很重要吗？如果是你就应该看看散列容器,排序的vector和标准的关联容器,大概是这个顺序.
- 是否介意容器的底层使用引用计数？如果是你就得避开string,因为很多string的实现是基于引用计数的,可以考虑使用vector
- 是否需要把迭代器、指针和引用的失效次数减到最少？如果是就应该使用基于节点的容器,因为在这些容器上进行插入和删除不会使迭代器、指针和引用失效（除非它们指向你删除的元素）.一般来说,在连续内存容器上插入和删除会使所有指向容器的迭代器、指针和引用失效.
### 条款2：小心对“容器无关代码”的幻想
- 不要写容器无关的代码,这根本没有必要.不同的容器是不同的,而且它们的优点和缺点有重大不同.它们并不被设计成可互换的,而且你做不了什么包装的工作
### 条款3：使容器里对象的拷贝操作轻量而正确
- 容器容纳了对象,但不是你给它们的那个对象,当你从容器中获取一个对象时,你所得到的对象不是容器里的那个对象.取而代之的是,当你向容器中添加一个对象,进入容器的是你指定的对象的拷贝.拷进去,拷出来.这就是STL的方式.
- 由于继承的存在,拷贝会导致分割,如果你以基类对象建立一个容器,而你试图插入派生类对象,那么当对象拷入容器的时候对象的派生部分会被删除：
- 一个使拷贝更高效、正确而且对分割问题免疫的简单的方式是建立指针的容器而不是对象的容器
### 条款4：用empty来代替检查size()是否为0
- 对于所有的标准容器,empty是一个常数时间的操作,但对于一些list实现,size花费线性时间.
### 条款5：尽量使用区间成员函数代替它们的单元素兄弟
- 一般来说使用区间成员函数可以输入更少的代码.
- 区间成员函数会导致代码更清晰更直接了当.
- 当处理标准序列容器时,应用单元素成员函数比完成同样目的的区间成员函数需要更多地内存分配,更频繁地拷贝对象或者造成多余操作
### 条款6：警惕C++最令人恼怒的解析
下面是参数为函数指针的三种声明形式:
```cpp
int g(double (*pf)());//最一般的方式
int g(double pf());   // 同上;pf其实是一个指针
int g(double ());     // 同上;参数名省略
```
假设你有一个int的文件,你想要把那些int拷贝到一个list中.这看起来像是一个合理的方式：
```cpp
ifstream dataFile("ints.dat");
list<int> data(istream_iterator<int>(dataFile),istream_iterator<int>());
```
这声明了一个函数data,它的返回类型是list.这个函数data带有两个参数： 
* 第一个参数叫做dataFile.它的类型是istream_iterator.dataFile左右的括号是多余的而且被忽略. 
* 第二个参数没有名字.它的类型是指向一个没有参数而且返回istream_iterator的函数的指针. 
这符合C++里的一条通用规则—几乎任何东西都可能被分析成函数声明 
例如：
```cpp
class Widget {...};
Widget w();
```
这并没有声明一个叫做w的Widget,它声明了一个叫作w的没有参数且返回Widget的函数 
解决方法：通过增加一对括号,我们强迫编译器以我们的方式看事情：
```cpp
list<int> data((istream_iterator<int>(dataFile)),istream_iterator<int>());
```
### 条款7：当使用new得指针的容器时,记得在销毁容器前delete那些指针
下面代码直接导致一个内存泄漏
```cpp
void doSomething()
{
    vector<Widget*> vwp;
    for (int i = 0; i < SOME_MAGIC_NUMBER; ++i)
    vwp.push_back(new Widget);
    ... // 使用vwp
} // Widgets在这里泄漏！
```
可以简单这样
```cpp
void doSomething()
{
    vector<Widget*> vwp;
    ... // 同上
    for (vector<Widget*>::iterator i = vwp.begin();i != vwp.end(),++i) 
    {
        delete *i;
    }
}
```
也可以调用for_each把delete转入一个函数对象中
```cpp
template<typename T>
struct DeleteObject :public unary_function<const T*, void> 
{
    void operator()(const T* ptr) const
    {
        delete ptr;
    }
};
void doSomething()
{
    ... // 同上
    for_each(vwp.begin(), vwp.end(), DeleteObject<Widget>);
}
```
不幸的是,这让你指定了DeleteObject将会删除的对象的类型那是很讨厌的,vwp是一个vector
```cpp
struct DeleteObject 
{   // 删除这里的模板化和基类
    template<typename T> // 模板化加在这里
    void operator()(const T* ptr) const
    {
        delete ptr;
    }
};
void doSomething()
{
    deque<SpecialString*> dssp;
    //...
    for_each(dssp.begin(), dssp.end(),DeleteObject());
}
```
但不是异常安全的,要异常安全可以使用智能指针下面是使用boost库shared_ptr的代码
```cpp
void doSomething()
{
    typedef boost::shared_ ptr<Widget> SPW; //SPW = "shared_ptr
    // to Widget"
    vector<SPW> vwp;
    for (int i = 0; i < SOME_MAGIC_NUMBER; ++i)
    vwp.push_back(SPW(new Widget)); // 从一个Widget建立SPW,
    // 然后进行一次push_back
    ... // 使用vwp
} // 这里没有Widget泄漏,甚至在上面代码中抛出异常
```
### 条款8：永不建立auto_ptr的容器
auto_ptr的特性： 
当你拷贝一个auto_ptr时,auto_ptr所指向对象的所有权被转移到拷贝的auto_ptr,而被拷贝的auto_ptr被设为NULL
### 条款9：在删除选项中仔细选择
假定你有一个标准STL容器,c,容纳int,Container c;而你想把c中所有值为1963的对象都去掉. 
如果你有一个连续内存容器（vector、deque或string）,最好的方法是erase-remove
```cpp
c.erase(remove(c.begin(), c.end(), 1963),c.end());
```
这方法也适合于list,list的成员函数remove更高效：c.remove(1963); 
当c是标准关联容器（即,set、multiset、map或multimap）时,使用任何叫做remove的东西都是完全错误的.这样的容器没有叫做remove的成员函数 
对于关联容器,解决问题的适当方法是调用erase：
```cpp
c.erase(1963); // 当c是标准关联容器时erase成员函数是去除特定值的元素的最佳方法
```
关联容器的erase成员函数有基于等价而不是相等的优势 
让我们现在稍微修改一下这个问题.不是从c中除去每个有特定值的物体,让我们消除下面判断式返回真的每个对象：
```cpp
bool badValue(int x); // 返回x是否是“bad”
```
对于序列容器（vector、string、deque和list）,我们要做的只是把每个remove替换为remove_if,然后就完成了： 
// 当c是vector、string或deque时这是去掉badValue返回真的对象的最佳方法
```cpp
// 当c是list时这是去掉badValue返回真的对象的最佳方法
c.erase(remove_if(c.begin(), c.end(), badValue), c.end()); 
c.remove_if(badValue);
```
对于标准关联容器,它不是很直截了当.有两种方法处理该问题,一个更容易编码,另一个更高效.“更容 
易但效率较低”的解决方案用remove_copy_if把我们需要的值拷贝到一个新容器中,然后把原容器的内容和 
新的交换 
方法1:
```cpp
AssocContainer<int> c; // c现在是一种标准关联容器
//... 
AssocContainer<int> goodValues; // 用于容纳不删除的值的临时容器
//从c拷贝不删除的值到goodValues
remove_copy_if(c.begin(), c.end(),inserter(goodValues,goodValues.end()),badValue);
c.swap(goodValues); // 交换c和goodValues
```
方法2：
```cpp
AssocContainer<int> c;
//…
// for循环的第三部分是空的；i现在在下面自增
for (AssocContainer<int>::iterator i = c.begin();i != c.end();)
{
    if (badValue(*i)) 
        c.erase(i++);//注意在这里为了避免迭代器失效使用i++
    else 
        ++i; 
}
```
总结 
* 去除一个容器中有特定值的所有对象： 
如果容器是vector、string或deque,使用erase-remove惯用法. 
如果容器是list,使用list::remove. 
如果容器是标准关联容器,使用它的erase成员函数. 
* 去除一个容器中满足一个特定判定式的所有对象： 
如果容器是vector、string或deque,使用erase-remove_if惯用法. 
如果容器是list,使用list::remove_if. 
如果容器是标准关联容器,使用remove_copy_if和swap,或写一个循环来遍历容器元素,当你把迭代 
器传给erase时记得后置递增它. 
* 在循环内做某些事情（除了删除对象之外）： 
如果容器是标准序列容器,写一个循环来遍历容器元素,每当调用erase时记得都用它的返回值更新你 
的迭代器. 
如果容器是标准关联容器,写一个循环来遍历容器元素,当你把迭代器传给erase时记得后置递增它.
### 条款10：注意分配器的协定和约束
标准允许STL实现认为所有相同类型的分配器对象都是等价的而且比较起来总是相等 
考虑这段代码：
```cpp
template<typename T> // 一个用户定义的分配器
class SpecialAllocator {...}; // 模板
typedef SpecialAllocator<Widget> SAW; // SAW = “SpecialAllocator
// for Widgets”
list<Widget, SAW> L1;
list<Widget, SAW> L2;
...
L1.splice(L1.begin(), L2); // 把L2的节点移到
// L1前端
```
记住当list元素从一个list被接合到另一个时,没有拷贝什么.取而代之的是,调整了一些指针,曾经在一个list 
中的节点发现他们自己现在在另一个list中.这使接合操作既迅速又异常安全.在上面的例子里,接合前在L2 
里的节点接合后出现在L1中.当L1被销毁时,当然,它必须销毁它的所有节点（以及回收它们的内存）,而因为它现在包含最初是L2一部分的节点,L1的分配器必须回收最初由L2的分配器分配的节点.现在清楚为什么标准允许STL实现认为相同类型的分配器等价.所以由一个分配器对象（比如L2）分配的内存可以安全地被另一个分配器对象（比如L1）回收.如果没有这样的认为,接合操作将更难实现.显然它们不能像现在一样高效. 
STL实现可以认为相同类型的分配器等价意味着可移植的分配器不能有任何非静态数据成员 
allocator::allocate分配n个T对象大小的内存,这对于内存连续容器是试用的,但对于list和标准关联容器这些基于结点的容器是不适用的,因为关联容器分配的内存除了T外还有其他指针,对于list来说它需要的分配器不是allocator而是allocator 
如果你想要写自定义分配器,让我们总结你需要记得的事情. 
* 把你的分配器做成一个模板,带有模板参数T,代表你要分配内存的对象类型. 
* 提供pointer和reference的typedef,但是总是让pointer是T*,reference是T&. 
* 决不要给你的分配器每对象状态.通常,分配器不能有非静态的数据成员. 
* 记得应该传给分配器的allocate成员函数需要分配的对象个数而不是字节数.也应该记得这些函数返回 
T*指针（通过pointer typedef）,即使还没有T对象被构造. 
* 一定要提供标准容器依赖的内嵌rebind模板.
### 条款13：尽量使用vector和string来代替动态分配的数组
使用new来进行动态分配,你需要肩负下列职责： 
1. 你必须确保有的人以后会delete这个分配.如果后面没有delete,你的new就会产生一个资源泄漏. 
2. 你必须确保使用了delete的正确形式.对于分配一个单独的对象,必须使用“delete”.对于分配一个 
数组,必须使用“delete []”.如果使用了delete的错误形式,结果会未定义.在一些平台上,程序在 
运行期会当掉.另一方面,它会默默地走向错误,有时候会造成资源泄漏,一些内存也随之而去. 
3. 你必须确保只delete一次.如果一个分配被删除了不止一次,结果也会未定义. 
vector和string消除了上面的负担,因为 
它们管理自己的内存.当元素添加到那些容器中时它们的内存会增长,而且当一个vector或string销毁时,它 
的析构函数会自动销毁容器中的元素,回收存放那些元素的内存
### 条款14：使用reserve来避免不必要的重新分配
有vector和string提供的几个函数 
* size()告诉你容器中有多少元素.它没有告诉你容器为它容纳的元素分配了多少内存. 
* capacity()告诉你容器在它已经分配的内存中可以容纳多少元素.那是容器在那块内存中总共可以容纳 
多少元素,而不是还可以容纳多少元素.如果你想知道一个vector或string中有多少没有被占用的内 
存,你必须从capacity()中减去size().如果size和capacity返回同样的值,容器中就没有剩余空间了,而 
下一次插入（通过insert或push_back等）会引发上面的重新分配步骤. 
* resize(Container::size_type n)强制把容器改为容纳n个元素.调用resize之后,size将会返回n.如果n小于 
当前大小,容器尾部的元素会被销毁.如果n大于当前大小,新默认构造的元素会添加到容器尾部. 
如果n大于当前容量,在元素加入之前会发生重新分配. 
* reserve(Container::size_type n)强制容器把它的容量改为至少n,提供的n不小于当前大小.这一般强迫 
进行一次重新分配,因为容量需要增加.（如果n小于当前容量,vector忽略它,这个调用什么都不 
做,string可能把它的容量减少为size()和n中大的数,但string的大小没有改变.在我的经验中,使用 
reserve来从一个string中修整多余容量一般不如使用“交换技巧” 
这个简介明确表示了只要有元素需要插入而且容器的容量不足时就会发生重新分配避免重新分配的关键是使用reserve尽快把容器的容量设置为足够大,最好在容器被构造之后立刻进行
### 条款16: 如何将vector和string的数据传给遗留的API
给定一个vector v; 
如果我们想要传递v给这样的C风格的API：void doSomething(const int* pInts, size_t numInts); 
我们可以这么做：
```cpp
if (!v.empty()) 
{
    doSomething(&v[0], v.size());
}
```
类似从vector上获取指向内部数据的指针的方法,对string不是可靠的,因为 
（1）string中的数据并没有保证被存储在独立的一块连续内存中 
（2）string的内部表示形式并没承诺以一个null字符结束.这解释了string的成 
员函数c_str存在的原因,它返回一个按C风格设计的指针,指向string的值.因此我们可以这样传递一个string对象s给这个函数,
```cpp
void doSomething(const char *pString);
```
像这样:
```cpp
doSomething(s.c_str());
```
### 条款17：使用“交换技巧”来修整过剩容量
要避免你的vector持有它不再需要的内存,你需要有一种方法来把它从曾经最大的容量减少到它现在需要的 
容量例如
```cpp
vector<Contestant>(contestants).swap(contestants);
```
表达式建立一个临时vector,它是contestants的一份拷贝：vector的拷贝构造函数做了这个工作.但是,vector的拷贝构造函数只分配拷贝的元素需要的内存,所以这个临时vector没有多余的容量.然后我们让临时vector和contestants交换数据,这时我们完成了,contestants只有临时变量的修整过的容量,而这个临时变量则持有了曾经在contestants中的发胀的容量.在这里（这个语句结尾）,临时vector被销毁,因此释放了以前contestants使用的内存.同样的技巧可以应用于string：同样方法可以用来清空vector和string
### 条款18：避免使用vector 
vector不是stl容器,不容纳bool 
一个东西要成为STL容器就必须满足所有在C++标准列出的容器必要条件如果c是一个T类型对象的容器,且c支持operator[], 
那么以下代码必须能够编译:
```cpp
T *p = &c[0]; // 无论operator[]返回什么 都可以用这个地址初始化一个T*
```
换句话说,如果你使用operator[]来得到Container中的一个T对象,你可以通过取它的地址而获得指向那个对象的指针 
所以如果vector是一个容器,这段代码必须能够编译：
```cpp
vector<bool> v;
bool *pb = &v[0]; // 用vector<bool>::operator[]返回的东西的地址初始化一个bool*
```
但它不能编译.因为vector是一个伪容器,并不保存真正的bool,而是打包bool以节省空间.在一个典型的实现中,每个保存在“vector”中的“bool”占用一个单独的比特,而一个8比特的字节将容纳8个“bool”.在内部,vector使用了与位域（bitfield）等价的思想来表示它假装容纳的bool.正如bool,位域也只表现为两种可能的值,但真的bool和化装成bool的位域之间有一个重要的不同：你可以创建指向真的bool的指针,但却禁止有指向单个比特的指针. 
vector::operator[]需要返回指向 一个比特的引用,而并不存在这样的东西.为了解决这个难题vector::operator[]返回一个对象,其行为类似于比特的引用,也称为代理对象 
vector看起来像这样：
```cpp
template <typename Allocator>
vector<bool, Allocator> 
{
    public:
    class reference {...}; // 用于产生引用独立比特的代理类
    reference operator[](size_type n); // operator[]返回一个代理
    ...
}
```
现在,这段代码不能编译的原因就很明显了：
```cpp
vector<bool> v;
bool *pb = &v[0]; // 错误！右边的表达式是vector<bool>::reference*类型,不是bool*
```
### 条款19：了解相等和等价的区别
相等的概念是基于operator==的,如果表达式x==y返回true,那么x和y有相等的值 
等价是基于在一个有序区间中对象值的相对位置.等价一般在每种标准关联容器（比如,set、multiset、map和multimap）的一部分排序顺序方面有意义.两个对象x和y如果在关联容器c的排序顺序中没有哪个排在另一个之前,那么它们关于c使用的排序顺序有等价的值.在一般情况下用于关联容器的比较函数是用户定义的判断式,每个标准关联容器通过它的key_comp成员函数来访问排序判断式,所以如果下式求值为真,两个对象x和y关于一个关联容器c的排序标准有等价的值：
```cpp
//在排序标准key_comp条件下x和y都不在彼此之前
!c.key_comp()(x, y) && !c.key_comp()(y, x)
```
例如考虑一个忽略大小写的set这样的比较函数会认为“STL”和“stL”是等价的,为此我们写一个比较的仿函数类
```cpp
struct CIStringCompare:public binary_function<string, string, bool> 
{ 
    bool operator()(const string& lhs,const string& rhs) const
    {
        return ciStringCompare(lhs, rhs); 
    } 
};
```
给定CIStringCompare,要建立一个忽略大小写的set就很简单了:
```cpp
set<string, CIStringCompare> ciss; // ciss = “case-insensitive string set”
```
如果我们向这个set中插入“Persephone”和“persephone”,只有第一个字符串加入了,因为第二个等价于第一个:
```cpp
ciss.insert("Persephone"); // 一个新元素添加到set中
ciss.insert("persephone"); // 没有新元素添加到set中
```
如果我们现在使用set的find成员函数搜索字符串“persephone”,搜索会成功
```cpp
if (ciss.find("persephone") != ciss.end())// 这个测试会成功
//但如果我们用非成员的find算法,搜索会失败：
//下面这个测试会失败
if (find(ciss.begin(), ciss.end(),"persephone") != ciss.end())
```
那是因为“persephone”等价于“Persephone”（关于比较仿函数CIStringCompare）,但不等于它（因为string(“persephone”) != string(“Persephone”)）.这个例子演示了为什么你应该优先选择成员函数（就像set::find）而不是非成员兄弟（就像find）的一个理由.
### 条款20：为指针的关联容器指定比较类型
无论何时你建立一个指针的标准关联容器,你必须记住容器会以指针的值排序.这基本上不是你想要的,所以你几乎总是需要建立自己的仿函数类作为比较类型 
条款21: 永远让比较函数,对相等的值返回false
### 条款22：避免原地修改set和multiset的键
所有标准关联容器,set和multiset,map和multimap保持它们的元素有序,这些容器的正确行为依赖于它们保持有序. 如果你改了关联容器里的一个元素的值,新值可能不在正确的位置,而且那将破坏容器的有序性,这对于map和multimap特别简单,因为试图改变这些容器里的一个键值的程序将不能编译,那是因为map
### 条款23：考虑用有序vector代替关联容器
标准关联容器的典型实现是平衡二叉查找树.一个平衡二叉查找树是一个对插入、删除和查找的混合操作优 
化的数据结构 
在很多应用中,使用数据结构并没有那么混乱.它们对数据结构的使用可以总结为这样的三个截然不同的阶段： 
* 建立.通过插入很多元素建立一个新的数据结构.在这个阶段,几乎所有的操作都是插入和删除.几乎没有或根本没有查找. 
* 查找.在数据结构中查找指定的信息片.在这个阶段,几乎所有的操作都是查找.几乎没有或根本没有插入和删除. 
* 重组.修改数据结构的内容,也许通过删除所有现有数据和在原地插入新数据.从动作上说,这个阶段等价于阶段1.一旦这个阶段完成,应用程序返回阶段2. 
对于这么使用它们的数据结构的应用来说,一个vector可能比一个关联容器能提供更高的性能（时间和空间上都是）.但不是任意的vector都会,只有有序vector.因为只有有序容器才能正确地使用查找算法binary_search lower_bound equal_range 
但为什么一个（有序的）vector的二分法查找比一个二叉树的二分法查找提供了更好的性能? 
1.关联容器占用更多的内存空间,不仅存放对象还存放相关的指针,造成内存空间浪费如果使用虚拟内存会造成更多的页面错误 
2.关联容器是基于结点的,内存空间不是连续的,分散的结点不具有引用局部性,因此会造成更多的页面错误
### 条款24：当关乎效率时应该在map::operator[]和map-insert之间仔细选择
当要插入元素时使用insert,当要更新元素值时使用operator[]函数 
map的operator[]函数与vector、deque和string的operator[]函数无关,也和内建的数组operator[]无关相反map::operator[] 被设计为简化“添加或更新”功能.即,给定map
```cpp
map<int, Widget> m;
m[1] = 1.50;
```
在这里,m里面还没有任何东西,所以键1在map里没有入口.因此operator[]默认构造一个Widget来作为关联到1的值,然后返回到那个Widget的引用.最后,Widget成为赋值目标：被赋值的值是1.50. 
功能上等价于
```cpp
typedef map<int, Widget> IntWidgetMap; // 方便的typedef
pair<IntWidgetMap::iterator, bool> result = m.insert(IntWidgetMap::value_type(1, Widget())); 
result.first->second = 1.50;
```
使用insert来插入元素
```cpp
m.insert(IntWidgetMap::value_type(1, 1.50));
```
这与上面的那些代码有相同的最终效果,除了它通常节省了三次函数调用：一个建立临时的默认构造Widget对象,一个销毁那个临时的对象和一个对Widget的赋值操作.那些函数调用越昂贵,你通过使用map-insert代替map::operator[]就能节省越多
```cpp
//使用operator[]更新元素
m[k] = v;
//使用insert来更新元素
m.insert(IntWidgetMap::value_type(k, v)).first->second = v;
```
insert的调用需要IntWidgetMap::value_type类型的实参（即pair
### 条款25：熟悉非标准散列容器
非标准散列容器：hash_set、hash_multiset、hash_map和hash_multimap 
标准散列容器：unordered_set、unordered_multiset、unordered_map和unordered_multimap散列关联容器,不像它们在标准中的（通常基于树）兄弟,不需要保持有序
