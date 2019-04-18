# STL学习小结 - 深之JohnChen的专栏 - CSDN博客

2009年10月05日 14:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：73134


STL就是Standard Template Library，标准模板库。这可能是一个历史上最令人兴奋的工具的最无聊的术语。从根本上说，STL是一些“容器”的集合，这些“容器”有list, vector,set,map等，STL也是算法和其它一些组件的集合。这里的“容器”和算法的集合指的是世界上很多聪明人很多年的杰作。是C＋＋标准库的一个重要组成部分，它由Stepanov and Lee等人最先开发，它是与C＋＋几乎同时开始开发的；一开始STL选择了Ada作为实现语言，但Ada有点不争气，最后他们选择了C＋＋，C＋＋中已经有了模板。STL又被添加进了C＋＋库。1996年，惠普公司又免费公开了STL，为STL的推广做了很大的贡献。STL提供了类型安全、高效而易用特性的STL无疑是最值得C++程序员骄傲的部分。每一个C＋＋程序员都应该好好学习STL。大体上包括container（容器）、algorithm（算法）和iterator（迭代器），容器和算法通过迭代器可以进行无缝连接。

一、基础知识

1、泛型技术

泛型技术的实现方法有多种，比如模板，多态等。模板是编译时决定，多态是运行时决定，其他的比如RTTI也是运行时确定。多态是依靠虚表在运行时查表实现的。比如一个类拥有虚方法，那么这个类的实例的内存起始地址就是虚表地址，可以把内存起始地址强制转换成int*，取得虚表，然后(int*)*(int*)取得虚表里的第一个函数的内存地址，然后强制转换成函数类型，即可调用来验证虚表机制。

泛型编程（generic programming，以下直接以GP称呼）是一种全新的程序设计思想，和OO，OB，PO这些为人所熟知的程序设计想法不同的是GP抽象度更高，基于GP设计的组件之间偶合度底，没有继承关系，所以其组件间的互交性和扩展性都非常高。我们都知道，任何算法都是作用在一种特定的数据结构上的，最简单的例子就是快速排序算法最根本的实现条件就是所排序的对象是存贮在数组里面，因为快速排序就是因为要用到数组的随机存储特性，即可以在单位时间内交换远距离的对象，而不只是相临的两个对象，而如果用联表去存储对象，由于在联表中取得对象的时间是线性的即O[n]，这样将使快速排序失去其快速的特点。也就是说，我们在设计一种算法的时候，我们总是先要考虑其应用的数据结构，比如数组查找，联表查找，树查找，图查找其核心都是查找，但因为作用的数据结构不同将有多种不同的表现形式。数据结构和算法之间这样密切的关系一直是我们以前的认识。泛型设计的根本思想就是想把算法和其作用的数据结构分离，也就是说，我们设计算法的时候并不去考虑我们设计的算法将作用于何种数据结构之上。泛型设计的理想状态是一个查找算法将可以作用于数组，联表，树，图等各种数据结构之上，变成一个通用的，泛型的算法。

2、四种类型转换操作符

static_cast将一个值以符合逻辑的方式转换。应用到类的指针上，意思是说它允许子类类型的指针转换为父类类型的指针（这是一个有效的隐式转换），同时，也能够执行相反动作：转换父类为它的子类。

例如：float x;

Count<<static_cast<int>(x);//把x作为整型值输出

dynamic_cast将多态类型向下转换为其实际静态类型。只用于对象的指针和引用。当用于多态类型时，它允许任意的隐式类型转换以及相反过程。dynamic_cast会检查操作是否有效。也就是说，它会检查转换是否会返回一个被请求的有效的完整对象。检测在运行时进行。如果被转换的指针不是一个被请求的有效完整的对象指针，返回值为NULL.

例如：class Car;

class Cabriolet:public Car{

…

};

class Limousline:public Car{

…

};

void f(Car *cp)

{

Cabriolet *p = dynamic_cast< Cabriolet > cp;

}

reinterpret_cast转换一个指针为其它类型的指针。它也允许从一个指针转换为整数类型。反之亦然。这个操作符能够在非相关的类型之间转换。操作结果只是简单的从一个指针到别的指针的值的二进制拷贝。在类型之间指向的内容不做任何类型的检查和转换。

例如：

class A {};
class B {};
A * a = new A;
B * b = reinterpret_cast<B *>(a);

const_cast一般用于强制消除对象的常量性。

例如：

class C {};
const C *a = new C;
C *b = const_cast<C *>(a);
其它三种操作符是不能修改一个对象的常量性的。

3、explicit修饰的构造函数不能担任转换函数。在很多情况下，隐式转换是有意的，并且是正当的。但有时我们不希望进行这种自动的转换。

例如：为了避免这样的隐式转换，应该象下面这样显式声明该带单一参数的构造函数：

class String {
int size;
char *p;
//..
public:
       //不要隐式转换
       explicit String (int sz);
       String (const char *s, int size n = 0); //隐式转换
};
void f ()
{
    String s(10);
    s = 100; //现在编译时出错；需要显式转换：
    s = String(100); //好；显式转换
    s = "st";        //好；此时允许隐式转换
}

4、命名空间namespace

解决在使用不同模块和程序库时，出现名称冲突问题。

5、C++标准程序库中的通用工具。由类和函数构成。这些工具包括：

数种通用类型

一些重要的C函数

数值极值

二、STL六大组件

容器（Container）

算法（Algorithm）

迭代器（Iterator）

仿函数（Function object）

适配器（Adaptor）

空间配置器（allocator）

1、容器

作为STL的最主要组成部分－－容器，分为向量（vector），双端队列(deque)，表(list)，队列（queue），堆栈(stack)，集合(set)，多重集合(multiset)，映射(map)，多重映射(multimap)。
|容器|特性|所在头文件|
|----|----|----|
|向量vector|可以用常数时间访问和修改任意元素，在序列尾部进行插入和删除时，具有常数时间复杂度，对任意项的插入和删除就有的时间复杂度与到末尾的距离成正比，尤其对向量头的添加和删除的代价是惊人的高的|<vector>|
|双端队列deque|基本上与向量相同，唯一的不同是，其在序列头部插入和删除操作也具有常量时间复杂度|<deque>|
|表list|对任意元素的访问与对两端的距离成正比，但对某个位置上插入和删除一个项的花费为常数时间。|<list>|
|队列queue|插入只可以在尾部进行，删除、检索和修改只允许从头部进行。按照先进先出的原则。|<queue>|
|堆栈stack|堆栈是项的有限序列，并满足序列中被删除、检索和修改的项只能是最近插入序列的项。即按照后进先出的原则|<stack>|
|集合set|由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，没有两个不同的元素能够拥有相同的次序，具有快速查找的功能。但是它是以牺牲插入删除操作的效率为代价的|<set>|
|多重集合multiset|和集合基本相同，但可以支持重复元素具有快速查找能力|<set>|
|映射map|由{键，值}对组成的集合，以某种作用于键对上的谓词排列。具有快速查找能力|<map>|
|多重集合multimap|比起映射，一个键可以对应多了值。具有快速查找能力|<map>|
STL容器能力表：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20091005/1.JPG)

2、算法

算法部分主要由头文件<algorithm>，<numeric>和<functional>组成。< algorithm>是所有STL头文件中最大的一个，它是由一大堆模版函数组成的，可以认为每个函数在很大程度上都是独立的，其中常用到的功能范 围涉及到比较、交换、查找、遍历操作、复制、修改、移除、反转、排序、合并等等。<numeric>体积很小，只包括几个在序列上面进行简单数学运算的模板函数，包括加法和乘法在序列上的一些操作。<functional>中则定义了一些模板类，用以声明函数对象。

STL的算法也是非常优秀的，它们大部分都是类属的，基本上都用到了C＋＋的模板来实现，这样，很多相似的函数就不用自己写了，只要用函数模板就可以了。

我们使用算法的时候，要针对不同的容器，比如：对集合的查找，最好不要用通用函数find（），它对集合使用的时候，性能非常的差，最好用集合自带的find（）函数，它针对了集合进行了优化，性能非常的高。

3、迭代器

它的具体实现在<itertator>中，我们完全可以不管迭代器类是怎么实现的，大多数的时候，把它理解为指针是没有问题的（指针是迭代器的一个特例，它也属于迭代器），但是，决不能完全这么做。

随机迭代器

Random access iterator

随机读写

Read and Write with random

access

vector,deque,array,string

4、仿函数

仿函数，又或叫做函数对象，是STL六大组件之一；仿函数虽然小，但却极大的拓展了算法的功能，几乎所有的算法都有仿函数版本。例如，查找算法find_if就是对find算法的扩展，标准的查找是两个元素相等就找到了，但是什么是相等在不同情况下却需要不同的定义，如地址相等，地址和邮编都相等，虽然这些相等的定义在变，但算法本身却不需要改变，这都多亏了仿函数。仿函数(functor)又称之为函数对象（function object），其实就是重载了()操作符的struct，没有什么特别的地方。

如以下代码定义了一个二元判断式functor：

struct IntLess
{
bool operator()(int left, int right) const
{
   return (left < right);
};
};

为什么要使用仿函数呢？

1).仿函数比一般的函数灵活。

2).仿函数有类型识别，可以作为模板参数。

3).执行速度上仿函数比函数和指针要更快的。

怎么使用仿函数？

除了在STL里，别的地方你很少会看到仿函数的身影。而在STL里仿函数最常用的就是作为函数的参数，或者模板的参数。

在STL里有自己预定义的仿函数，比如所有的运算符，=，-，*，、比如'<'号的仿函数是less

template<class _Ty>
struct less   : public binary_function<_Ty, _Ty, bool>
{ // functor for operator<
        bool operator()(const _Ty& _Left, const _Ty& _Right) const
                   { // apply operator< to operands
                              return (_Left < _Right);
                   }
};

从上面的定义可以看出，less从binary_function<...>继承来的，那么binary_function又是什么的？

template<class _Arg1, class _Arg2, class _Result>
struct binary_function
{ // base class for binary functions
        typedef _Arg1 first_argument_type;
        typedef _Arg2 second_argument_type;
      typedef _Result result_type;
};

其实binary_function只是做一些类型声明而已，别的什么也没做，但是在STL里为什么要做这些呢？如果你要阅读过STL的源码，你就会发现，这样的用法很多，其实没有别的目的，就是为了方便，安全，可复用性等。但是既然STL里面内定如此了，所以作为程序员你必须要遵循这个规则,否则就别想安全的使用STL。

比如我们自己定一个仿函数。可以这样：

template <typename type1,typename type2>
class func_equal :public binary_function<type1,type2,bool>
{
        inline bool operator()(type1 t1,type2 t2) const//这里的const不能少
            {
                 return t1 == t2;//当然这里要overload==

             }
}

我们看这一行： inline bool operator()(type1 t1,type2 t2) const//这里的const不能少
inline是声明为内联函数，我想这里应该不用多说什么什么了，关键是为什么要声明为const的？要想找到原因还是看源码，加入如果我们这里写一行代码，find_if(s.begin(),s.end(),bind2nd(func_equal(),temp)),在bind2nd函数里面的参数是const类型的，const类型的对象，只能访问cosnt修饰的函数！

与binary_function(二元函数)相对的是unary_function(一元函数),其用法同binary_function

struct unary_function {
typedef _A argument_type; 
typedef _R result_type; 
}; 

注：仿函数就是重载()的class，并且重载函数要为const的，如果要自定义仿函数，并且用于STL接配器，那么一定要从binary_function或者，unary_function继承。

5、适配器

适配器是用来修改其他组件接口的STL组件，是带有一个参数的类模板（这个参数是操作的值的数据类型）。STL定义了3种形式的适配器：容器适配器，迭代器适配器，函数适配器。

容器适配器：包括栈（stack）、队列(queue)、优先(priority_queue)。使用容器适配器，stack就可以被实现为基本容器类型（vector,dequeue,list）的适配。可以把stack看作是某种特殊的vctor,deque或者list容器，只是其操作仍然受到stack本身属性的限制。queue和priority_queue与之类似。容器适配器的接口更为简单，只是受限比一般容器要多。

迭代器适配器：修改为某些基本容器定义的迭代器的接口的一种STL组件。反向迭代器和插入迭代器都属于迭代器适配器，迭代器适配器扩展了迭代器的功能。

函数适配器：通过转换或者修改其他函数对象使其功能得到扩展。这一类适配器有否定器（相当于"非"操作）、绑定器、函数指针适配器。函数对象适配器的作用就是使函数转化为函数对象，或是将多参数的函数对象转化为少参数的函数对象。

例如：

在STL程序里，有的算法需要一个一元函数作参数，就可以用一个适配器把一个二元函数和一个数值，绑在一起作为一个一元函数传给算法。
例如：
find_if(coll.begin(), coll.end(), bind2nd(greater <int>(), 42)); 
这句话就是找coll中第一个大于42的元素。
greater <int>()，其实就是">"号，是一个2元函数
bind2nd的两个参数，要求一个是2元函数，一个是数值，结果是一个1元函数。
bind2nd就是个函数适配器。

6、空间配置器

**STL的内存配置器在我们的实际应用中几乎不用涉及，但它却在STL的各种容器背后默默做了大量的工作，STL内存配置器为容器分配并管理内存。统一的内存管理使得STL库的可用性、可移植行、以及效率都有了很大的提升。**

SGI-STL的空间配置器有2种，一种仅仅对c语言的malloc和free进行了简单的封装，而另一个设计到小块内存的管理等，运用了内存池技术等。在SGI-STL中默认的空间配置器是第二级的配置器。

SGI使用时std::alloc作为默认的配置器。

A）．alloc把内存配置和对象构造的操作分开，分别由**alloc::allocate()**和**::construct()**负责，同样内存释放和对象析够操作也被分开分别由**alloc::deallocate()**和::destroy()负责。这样可以保证高效，因为对于内存分配释放和构造析够可以**根据具体类型(type traits)**进行优化。比如一些类型可以直接使用高效的memset来初始化或者忽略一些析构函数。对于内存分配alloc也提供了2级分配器来应对不同情况的内存分配。

B）．第一级配置器直接使用malloc()和free()来分配和释放内存。第二级视情况采用不同的策略：当需求内存超过128bytes的时候，视为足够大，便调用第一级配置器；当需求内存小于等于128bytes的时候便采用比较复杂的memeory pool的方式管理内存。

C）．无论allocal被定义为第一级配置器还是第二级，SGI还为它包装一个接口，使得配置的接口能够符合标准即把配置单位从bytes转到了元素的大小：

template<classT, classAlloc>

classsimple_alloc

{

public:

staticT* allocate(size_tn)

     {

return 0 == n ? 0 : (T*)Alloc::allocate(n *sizeof(T));

     }

staticT* allocate(void)

     {

return (T*) Alloc::allocate(sizeof(T));

     }

staticvoiddeallocate(T* p, size_tn)

     {

if (0 != n) Alloc::deallocate(p,n * sizeof(T));

     }

staticvoiddeallocate(T* p)

     {

Alloc::deallocate(p,sizeof(T));

     }

}   

d）．内存的基本处理工具，它们均具有commt or rollback能力。

template<classInputIterator, classForwardIterator>

ForwardIterator

uninitialized_copy(InputIteratorfirst, InputIteratorlast, ForwardIteratorresult);

template<classForwardIterator, classT>

voiduninitialized_fill(ForwardIteratorfirst, ForwardIteratorlast, constT& x);

template<classForwardIterator, classSize, classT>

ForwardIterator

uninitialized_fill_n(ForwardIteratorfirst, ForwardIteratorlast, constT& x)

三、具体容器、算法

1、所有容器都提供了一个默认的构造函数，一个拷贝构造函数。

例如：

list<int> l;

....

vector<int> ivector(l.begin(),l.end());

int array[]={1,2,3,4};

....

set<int> iset(array,array+sizeof(array)/sizeof(array[0]));

2、与大小相关的函数

size(),empty(),max_size()

3、返回迭代器的函数

begin(),end(),rbegin(),rend()

4、比较操作

==,!=,<,>,>=....

Vector详解：

capacity(),返回vector能够容纳的元素个数。

size(),返回vector内现有元素的个数。

赋值操作：

c1=c2; 把c2的全部元素指派给c1

c.assign(n,elem);复制n个elem，指派给c

c.assign(beg,end);将区间beg，end内的元素指派给c

c1.swap(c2);将c1,c2元素互换

swap(c1,c2);同上

元素存取

c.at(index);

c[index];

c.front();返回第一个元素

c.back();

插入和删除：

c.insert(pos.elem);

c.insert(pos,n.elem); 插入n个elem

c.insert(pos,beg,end); 在pos出插入beg，end区间内的所有元素。

c.push_back(elem);

c.pop_back();

c.erase(pos); 删除pos上的元素，返回下一个元素

c.erase(beg,end);

c.resize(num);将元素数量改为num，如果size变大了，多出来的新元素都要一default方式构建。

c.resize(num,elem);将元素数量改为num，如果size变大了，多出来的新元素是elem的副本。

c.clear();删除所有。

vector的reserve和resize

reserve只分配空间，而不创建对象，size()不变。而resize分配空间而且用空对象填充.

reserve是容器预留空间，但并不真正创建元素对象，在创建对象之前，不能引用容器内的元素，因此当加入新的元素时，需要用push_back()/insert()函数。

resize是改变容器的大小，并且创建对象，因此，调用这个函数之后，就可以引用容器内的对象了，因此当加入新的元素时，用operator[]操作符，或者用迭代器来引用元素对象。

再者，两个函数的形式是有区别的，reserve函数之后一个参数，即需要预留的容器的空间；resize函数可以有两个参数，第一个参数是容器新的大小，第二个参数是要加入容器中的新元素，如果这个参数被省略，那么就调用元素对象的默认构造函数。

vector有而deque无的：capacity(), reserve();

deque有而vector无的：push_front(elem), pop_front(); push_back(elem), pop_back();

STL提供的另两种容器queue、stack，其实都只不过是一种adaptor，它们简单地修饰deque的界面而成为另外的容器类型

List详解：

for_each(.begin(), .end(), “函数”);

count (.begin(), .end(), 100, jishuqi);

返回对象等于100的个数jishuqi值。

count_if() 带一个函数对象的参数(上面“100”的这个参数)。函数对象是一个至少带有一个operator()方法的类。这个类可以更复杂。

find(*.begin().*end(),“要找的东西”)；

如果没有找到指出的对象，就会返回*.end()的值，要是找到了就返回一个指着找到的对象的iterator

fine_if()；与count_if()类似，是find的更强大版本。

STL通用算法search()用来搜索一个容器，但是是搜索一个元素串，不象find()和find_if() 只搜索单个的元素。

search算法在一个序列中找另一个序列的第一次出现的位置。

search(A.begin(), A.end(), B.begin(), B.end());

在A中找B这个序列的第一次出现。

要排序一个list，我们要用list的成员函数sort()，而不是通用算法sort()。

list容器有它自己的sort算法，这是因为通用算法仅能为那些提供随机存取里面元素 的容器排序。

list的成员函数push_front()和push_back()分别把元素加入到list的前面和后面。你可以使用insert() 把对象插入到list中的任何地方。

insert()可以加入一个对象，一个对象的若干份拷贝，或者一个范围以内的对象。

list成员函数pop_front()删掉list中的第一个元素，pop_back()删掉最后一个元素。函数erase()删掉由一个iterator指出的元素。还有另一个erase()函数可以删掉一个范围的元素。

list的成员函数remove()用来从list中删除元素。

*.remove("要删除的对象");

通用算法remove()使用和list的成员函数不同的方式工作。一般情况下不改变容器的大小。

remove(*.begin(),*.end(),"要删除的对象");

使用STL通用算法stable_partition()和list成员函数splice()来划分一个list。

stable_partition()是一个有趣的函数。它重新排列元素，使得满足指定条件的元素排在不满足条件的元素前面。它维持着两组元素的顺序关系。

splice 把另一个list中的元素结合到一个list中。它从源list中删除元素。

Set Map详解：

STL map和set的使用虽不复杂，但也有一些不易理解的地方，如：

为何map和set的插入删除效率比用其他序列容器高？

为何每次insert之后，以前保存的iterator不会失效？

为何map和set不能像vector一样有个reserve函数来预分配数据？

当数据元素增多时（10000到20000个比较），map和set的插入和搜索速度变化如何？

C++ STL中标准关联容器set, multiset, map, multimap内部采用的就是一种非常高效的平衡检索二叉树：红黑树，也成为RB树(Red-Black Tree)。RB树的统计性能要好于一般的平衡二叉树(AVL-树).

为何map和set的插入删除效率比用其他序列容器高？

大部分人说，很简单，因为对于关联容器来说，不需要做内存拷贝和内存移动。说对了，确实如此。map和set容器内所有元素都是以节点的方式来存储，其节点结构和链表差不多，指向父节点和子节点。这里的一切操作就是指针换来换去，和内存移动没有关系。

为何每次insert之后，以前保存的iterator不会失效？(同解)

为何map和set不能像vector一样有个reserve函数来预分配数据？

究其原理来说时，引起它的原因在于在map和set内部存储的已经不是元素本身了，而是包含元素的节点。

其实你就记住一点，在map和set内面的分配器已经发生了变化，reserve方法你就不要奢望了。

当数据元素增多时（10000和20000个比较），map和set的插入和搜索速度变化如何？

如果你知道log2的关系你应该就彻底了解这个答案。在map和set中查找是使用二分查找，也就是说，如果有16个元素，最多需要比较4次就能找到结果，有32个元素，最多比较5次。那么有10000个呢？最多比较的次数为log10000，最多为14次，如果是20000个元素呢？最多不过15次。

泛型算法：

所有算法的前两个参数都是一对iterators：[first，last)，用来指出容器内一个范围内的元素。

每个算法的声明中，都表现出它所需要的最低层次的iterator类型。

常用算法：

accumulate() 元素累加

adjacent_difference() 相邻元素的差额

adjacent_find() 搜寻相邻的重复元素

binary_search() 二元搜寻

copy() 复制

copy_backward() 逆向复制

count() 计数

count_if() 在特定条件下计数

equal() 判断相等与否

equal_range() 判断相等与否（传回一个上下限区间范围）

fill() 改填元素值

fill_n() 改填元素值，n 次

find() 搜寻

find_if() 在特定条件下搜寻

find_end() 搜寻某个子序列的最后一次出现地点

find_first_of() 搜寻某些元素的首次出现地点

for_each() 对范围内的每一个元素施行某动作

generate() 以指定动作的运算结果充填特定范围内的元素

generate_n() 以指定动作的运算结果充填 n 个元素内容

includes() 涵盖於

inner_product() 内积

inplace_merge() 合并并取代（覆写）

iter_swap() 元素互换

lexicographical_compare() 以字典排列方式做比较

lower_bound() 下限

max() 最大值

max_element() 最大值所在位置

min() 最小值

min_element() 最小值所在位置

merge() 合并两个序列

mismatch() 找出不吻合点

next_permutation() 获得下一个排列组合

泛型演算法（Generic Algorithms）与 Function Obje4 cts

nth_element() 重新安排序列中第n个元素的左右两端

partial_sort() 局部排序

partial_sort_copy() 局部排序并复制到它处

partial_sum() 局部总和

partition() 切割

prev_permutation() 获得前一个排列组合

random_shuffle() 随机重排

remove() 移除某种元素（但不删除）

remove_copy() 移除某种元素并将结果复制到另一个 container

remove_if() 有条件地移除某种元素

remove_copy_if() 有条件地移除某种元素并将结果复制到另一个 container

replace() 取代某种元素

replace_copy() 取代某种元素，并将结果复制到另一个 container

replace_if() 有条件地取代

replace_copy_if() 有条件地取代，并将结果复制到另一个 container

reverse() 颠倒元素次序

reverse_copy() 颠倒元素次序并将结果复制到另一个 container

rotate() 旋转

rotate_copy() 旋转，并将结果复制到另一个 container

search() 搜寻某个子序列

search_n() 搜寻「连续发生 n 次」的子序列

set_difference() 差集

set_intersection() 交集

set_symmetric_difference() 对称差集

set_union() 联集

sort() 排序

stable_partition() 切割并保持元素相对次序

stable_sort() 排序并保持等值元素的相对次序

swap() 置换（对调）

swap_range() 置换（指定范围）

transform() 以两个序列为基础，交互作用产生第三个序列

unique() 将重复的元素摺叠缩编，使成唯一

unique_copy() 将重复的元素摺叠缩编，使成唯一，并复制到他处

upper_bound() 上限

四、注意细节：

1、auto_ptr不能用new[]所生成的array作为初值，因为释放内存时用的是delete，而不是delete[]

2、就搜寻速度而言，hash table通常比二叉树还要快5~10倍。hash table不是C++标准程序库的一员。

3、迭代器使用过程中优先选用前置式递增操作符（++iter）而不是选择后置式递增操作符（iter++）。

3、迭代器三个辅助函数：advance(),distance(),iter_swap()。

advance()可令迭代器前进

distance()可处理迭代器之间的距离。

iter_swap()可交换两个迭代器所指内容。

4、hasp函数 makeheap()、push_heap()、pop_heap()、sort_heap()

5、’/0’在string之中并不具有特殊意义，但是在一般C形式的string中却用来标记字符串结束。在string中，字符‘/0’和其他字符的地位完全相同。string中有三个函数可以将字符串内容转换成字符数组或C形式的string。

data()以字符数组的形式返回字符串内容。但末未追加’/0’字符，返回类型并非有效的C形式string。

c_str()以C形式返回字符串内容（在末尾端添加’/0’字符）。

copy()将字符串内容复制到“调用者提供的字符数组”中，不添加’/0’字符。

6、容器中用empty来代替检查size是否为0；当使用new得到指针的容器时，切记在容器销毁前delete那些指针；千万不要把auto_ptr放入容器中。

7、尽量使用vector和string来代替动态申请的数组；避免使用vector<bool>，vector<bool>有两个问题．第一，它不是一个真正STL容器，第二，它并不保存bool类型。

8、迭代器使用过程中，尽量使用iterator代替const_iterator，reverse_iterator和const_reverse_iterator；使用distance和advance把const_iterators转化成iterators。

typedef deque<int> IntDeque;// 和以前一样

typedef IntDeque::iterator Iter;

typedef IntDeque::const_iterator ConstIter;

IntDequed;

ConstIter ci;

...// 让ci指向d

Iter i(d.begin());// 初始化i为d.begin()

advance(i, distance(i, ci));// 调整i，指向ci位置

9、避免对set和multiset的键值进行修改。

10、永远让比较函数对相同元素返回false。

11、排序选择：

1）如果你需要在vector、string、deque或数组上进行完全排序，你可以使用sort或stable_sort。

2）如果你有一个vector、string、deque或数组，你只需要排序前n个元素，应该用partial_sort。

3）如果你有一个vector、string、deque或数组，你需要鉴别出第n个元素或你需要鉴别出最前的n个元素，而不用知道它们的顺序，nth_element是你应该注意和调用的。

4）如果你需要把标准序列容器的元素或数组分隔为满足和不满足某个标准，你大概就要找partition或stable_partition。

5）如果你的数据是在list中，你可以直接使用partition和stable_partition，你可以使用list的sort来代替sort和stable_sort。如果你需要partial_sort或nth_element提供的效果，你就必须间接完成这个任务。
12、如果你真的想删除东西的话就在类似remove的算法后接上erase。remove从一个容器中remove元素不会改变容器中元素的个数，erase是真正删除东西。
13、提防在指针的容器上使用类似remove的算法，在调用类似remove的算法前手动删除和废弃指针。
14、尽量用成员函数代替同名的算法，有些容器拥有和STL算法同名的成员函数。关联容器提供了count、find、lower_bound、upper_bound和equal_range，而list提供了remove、remove_if、unique、sort、merge和reverse。大多数情况下，你应该用成员函数代替算法。这样做有两个理由。首先，成员函数更快。其次，比起算法来，它们与容器结合得更好（尤其是关联容器）。那是因为同名的算法和成员函数通常并不是是一样的。
15、容器中使用自定义的结构体时，如果用到拷贝与赋值，结构体需要重载operator=符号；比较容器分成相等与不等，相等时重载operator==符号，不等时重载operator<符号。比如set、map、multiset、multimap、priority_queue等容器类要求重载operator<符号。
16、Map/Multimap，Sets/Multisets都不能用push_back,push_front,因为它是自动排序的。

Set内的相同数值的元素只能出现一次，Multisets内可包含多个数值相同的元素。

Map内的相同数值的元素只能出现一次，Multimap内可包含多个数值相同的元素。内部由二叉树实现，便于查找。

17、string 与 数字之间的转换，转换的方法有很多种，一般使用stringstream来实现转换。比如：

#include<iostream>

#include<sstream>

#include<string>

usingnamespacestd;

intmain()

{

inti=0;

stringtemp;

stringstreams;

//string转换为数字

temp = “1234”;

s<<temp;

s>>i;

cout<<i<<endl;

//数字转换为string

i=256;

s<<i;

temp = s.str();

cout<<temp<<end;

system("pause");

return0;

}

18、对于自定义的结构体，放入容器中，最好不要对容器进行内存初始化（不要调用memset,zeromemory函数），否则如果结构体中有指针类型的变量时，就会出现问题。

19、Vector的函数泄漏问题

定义了一个

struct temp

{

char name[256];

int i;

}

Vector<temp> vect;

当对这个vect执行pushback一些temp的结构体后，执行clear这样是否会内存泄露？可以释放掉temp结构体中的name内存吗？

解决方法：

不行，clear只是把那些元素全部删除掉，并不是释放内存。再者，你这样的定义容器是不需要释放内存的，如果你这样定义，std::vector <temp> *pVec。就需要了。先pVec->clear()再 pVec->swap( (std::vector <temp>)(*pVec) )。就能实现内存的释放。

20、stl之map erase方法的正确使用
STL的map表里有一个erase方法用来从一个map中删除掉指令的一个节点，不存在任何问题。
如果删除多一个节点时，需要使用正确的调用方法。比如下面的方法是有问题：
for(ITER iter=mapTest.begin();iter!=mapTest.end();++iter)
{
cout<<iter->first<<":"<<iter->second<<endl;
mapTest.erase(iter);
}
这是一种错误的写法,会导致程序行为不可知.究其原因是map 是关联容器,对于关联容器来说，如果某一个元素已经被删除，那么其对应的迭代器就失效了，不应该再被使用；否则会导致程序无定义的行为。

正确的使用方法：
1).使用删除之前的迭代器定位下一个元素。STL建议的使用方式
for(ITER iter=mapTest.begin();iter!=mapTest.end();)
{
cout<<iter->first<<":"<<iter->second<<endl;
mapTest.erase(iter++);
}

或者
for(ITER iter=mapTest.begin();iter!=mapTest.end();)
{
ITER iterTmp = iter;
iter++;
cout<<iterTmp->first<<":"<<iterTmp->second<<endl;
mapTest.erase(iterTmp);
}

2). erase() 成员函数返回下一个元素的迭代器
for(ITER iter=mapTest.begin();iter!=mapTest.end();)
{
cout<<iter->first<<":"<<iter->second<<endl;
iter=mapTest.erase(iter);
}

 21、boost::bind总结 
bind 是一组重载的函数模板.用来向一个函数(或函数对象)绑定某些参数. bind的返回值是一个函数对象. 
性质：
不是函数，是一个class，是一个多元仿函数

模板参数：
带模板参数，但不需要，会自动推导！

构造函数参数：
格式：_需要绑定类型，_参数1，_参数2，_参数3，_参数4…

_需要绑定类型：可以是普通函数，类成员函数，成员变量

_参数N：可以是一个占位符，或者实际参数。

如果绑定的类型是一个类成员函数或变量，那么第一个参数必须是对象或者对象指针。
仿函数参数：
任意

仿函数返回值
       如果绑定的是函数，返回绑定函数的返回值。

       如果绑定是成员变量，返回成员变量值

占位符：
_1,_2,_3,_4….._9

占位符的数字表示仿函数时对应参数的位置。

一个bind里可以嵌入多个bind,但占位符是相对于这一块的bind是共享。

注意事项
a)如果绑定的是类函数，传入对象时，最好使用对象指针，如果使用对象实例会产生多次对象复制。如果非要传对象而不想多次被复制传在在使用ref或cref(ref的const版)

b) 跟lambda混用时一定要特别小心

第一、   会与lambda的占位符有冲突

第二、   lambda库里有跟同样名字的bind,功能类似，但没有此功能强大

总结
无模板参数，构函数对绑定函数负责，仿函数是任意的。

举例说明
例一：

void nine_arguments(

                       int i1,int i2,int i3,int i4,

                       int i5,int i6,int i7,int i8, int i9) {

                            std::cout << i1 << i2 << i3 << i4 << i5

                                 << i6 << i7 << i8 << i9 << '/n';

}

int main() {

     int i1=1,i2=2,i3=3,i4=4,i5=5,i6=6,i7=7,i8=8,i9=9;

     (boost::bind(&nine_arguments,_9,_2,_1,_6,_3,_8,_4,_5,_7))

         (i1,i2,i3,i4,i5,i6,i7,i8,i9);

}
输出结果921638457

22、std::min error C2059: 语法错误:“::” 的解决方法
下面这段代码：
size_t n = std::min(count_, num_elements);
编译会出现这样的错误：
1> error C2059: 语法错误:“::”
1> error C2589: “(”:“::”右边的非法标记

解决办法:
将std::min用括号括起来，问题解决。
size_t n = (std::min)(count_, num_elements);

23、STL不一定不能在DLL间传递，不能将STL变量作为DLL接口中参数进行数据传递。
STL使用模板生成，当我们使用模板的时候，每一个EXE，和DLL都在编译器产生了自己的代码，导致模板所使用的静态成员不同步，所以出现数据传递的各种问题，下面是详细解释。
原因分析：
如果任何STL类使用了静态变量（无论是直接还是间接使用），那么就不要再写出跨执行单元访问它的代码。?除非你能够确定两个动态库使用的 都是同样的STL实现，比如都使用VC同一版本的STL，编译选项也一样。强烈建议，不要在动态库接口中传递STL容器！！
微软的解释：
http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b172396
微软给的解决办法：
http://support.microsoft.com/default.aspx?scid=kb%3ben-us%3b168958
微软的解释：
大部分C++标准库里提供的类直接或间接地使用了静态变量。由于这些类是通过模板扩展而来的，因此每个可执行映像（通常是.dll或.exe文件）就会存在 一份只属于自己的、给定类的静态数据成员。当一个需要访问这些静态成员的类方法执行时，它使用的是“这个方法的代码当前所在的那份可执行映像”里的静态成 员变量。由于两份可执行映像各自的静态数据成员并未同步，这个行为就可能导致访问违例，或者数据看起来似乎丢失或被破坏了。

推荐书籍：

《C++标准程序库》本书将焦点放在标准模板库（Standard Template Library）身上，检验其中的容器（containers）、迭代器（iterators）、仿函数（functors）和算法（algorithms）。你还可以找到特殊容器、字符串（strings）、数值类别、国际化议题、IOStream。每一个组件都有深刻的呈现，包括其介绍、设计、运用实例、细部解说、陷阱、意想不到的危险，以及相关类别和函数的确切标记（signature）和定义。一份见解深刻的基础概念介绍和一个程序库综合鸟瞰，会对新手带来快速的提升。

《泛型编程与STL》阐述了泛型程序设计的中心观念:concepts,modeling, refinement,并为你展示这些观念如何导出 STL的基础概念:iterators, containers, function objects.循此路线,你可以把 STL 想象为一个由 concepts(而非明确之 functions 或 classes)组成的 library.你将学习其正式结构并因此获得其潜在威力之完整优势.

《Effective STL》阐述了如何有效地使用STL（Standard Template Library, 标准模板库）进行编程。书中讲述了如何将STL组件组合在一起，从而利用库的设计。这些内容会帮助你针对简单的问题开发出简单、直接的解决方案，并且针对复杂的问题开发出精致的解决方案。书中还描述了常见的STL使用错误，并告诉你如何避免这些错误。

《STL源码剖析》了解源码，看到vector的实现、list的实现、heap的实现、deque的实现、RB-tree的实现、hash-table的实现、set/map的实现；你将看到各种算法（排序、搜寻、排列组合、数据移动与复制…）的实现；你甚至将看到底层的memory pool 和高阶抽象的traits机制的实现。

STL China网站：http://www.stlchina.org/

