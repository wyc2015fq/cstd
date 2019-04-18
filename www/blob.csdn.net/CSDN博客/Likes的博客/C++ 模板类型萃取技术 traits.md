# C++ 模板类型萃取技术 traits - Likes的博客 - CSDN博客
2018年10月11日 09:39:35[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：17
当函数，类或者一些封装的通用算法中的某些部分会因为数据类型不同而导致处理或逻辑不同（而我们又不希望因为数据类型的差异而修改算法本身的封装时），traits会是一种很好的解决方案。（类型测试发生在编译期）
自从C++中引入了template后，以泛型技术为中心的设计得到了长足的进步。STL就是这个阶段杰出的产物。STL的目标就是要把数据和算法分开，分别对其进行设计，之后通过一种名为iterator的东西，把这二者再粘接到一起。设计模式中，关于iterator的描述为：一种能够顺序访问容器中每个元素的方法，使用该方法不能暴露容器内部的表达方式。可以说，类型萃取技术就是为了要解决和iterator有关的问题的，下面，我们就来看看整个故事。
应该说，迭代器就是一种智能指针，因此，它也就拥有了一般指针的所有特点——能够对其进行*和->操作。但是在遍历容器的时候，不可避免的要对遍历的容器内部有所了解，所以，设计一个迭代器也就自然而然的变成了数据结构开发者的一个义务，而这些iterators的表现都是一样的，这种内外的差异，对用户来说，是完全透明的，
## 第一部分 为什么要有萃取技术
既然是一种智能指针，iterator也要对一个原生指针进行封装，而问题就源于此，当我们需要这个原生指针所指对象的类型的时候（例如声明变量），怎么办呢？
**Case1 对于函数的局部变量**
这种情况我们可以采用模版的参数推导，例如：
template <class T> void func(T iter)
如果T是某个指向特定对象的指针，那么在func中需要指针所指向对象类型的变量的时候，怎么办呢？这个还比较容易，模板的参数推导机制可以完成任务，如下：
```cpp
template <class T, class U>
void func_impl(T t, U u) {
    U temp; // OK, we’ve got the type
            // The rest work of func…
}
template <class T>
void func(T t) {
    func_impl(t, *t); // forward the task to func_impl
}
```
通过模板的推导机制，我们轻而易举的或得了指针所指向的对象的类型，但是事情往往不那么简单。例如，如果我想把传递给func的这个指针参数所指的类型作为返回值，显然这个方法不能凑效了，这就是我们的case 2。
**Case2 对于函数的返回值**
尽管在func_impl中我们可以把U作为函数的返回值，但是问题是用户需要调用的是func，于是，你不可能写出下面的代码：
```cpp
template <class T, class U>
U func_impl(T t, U u) {
    U temp; // OK, we’ve got the type
            // The rest work of func…
}
template <class T>
(*T) func(T t) { // !!!Wrong code
    return func_impl(t, *t); // forward the task to func_impl
}
int i  =10;
cout<<func(&i)<<endl; // !!! Can’t pass compile
```
**（*T）概念上如此正确，目的在于解引用迭代器**，不过所有的编译器都会让你失望。这个问题解决起来也不难，只要做一个iterator，然后在定义的时候为其指向的对象类型制定一个别名，就好了，像下面这样：
```cpp
template <class T>
struct MyIter {
    typedef T value_type; // A nested type declaration, important!!!
    T* ptr;
    MyIter(T* p = 0) : ptr(p) {}
    T& operator*() const { return *ptr; }
};
```
而后只要需要其指向的对象的类型，只要直接引用就好了，例如：
```cpp
template <class I>
typename I::value_type func(I iter) { return *iter; }
```
很漂亮的解决方案，看上去一切都很完美。但是，实际上还是有问题，因为func如果是一个泛型算法，那么它也绝对要接受一个原生指针作为迭代器，但是显然，你无法让下面的代码编译通过：
```cpp
int *p = new int(52);
cout<<func(p)<<endl; // !!!Is there a int::value_type?? Wrong Code here
```
我们的func无法支持原生指针，这显然是不能接受的。此时，template partial specialization就派上了用场。
**Solution：template partial specialization是救世主**
既然刚才的设计方案仍不完美，那我们就再加一个间接层，把智能指针和原生指针统统的封装起来。在讨论之前，先要澄清一下template partial specialization的含义。所谓的partial specialization和模板的默认参数是完全不同的两件事情，前者指的是当参数为某一类特定类型的时候，采用特殊的设计，也就是说是“针对template参数更进一步的条件限制所设计出来的一个特化版本”；而默认参数则是当不提供某些参数的时候，使用的一个缺省。
参考：partial specialization的语法
Template <typename T> class C<T*> {} // 为所有类型为T*的参数而准备的特殊版本
好了，下面我们就找一个专职的负责人，用来封装迭代器封装的对象类型。首先，把我们刚才的MyIter重新包装一下：
template <class I>
struct iterator_traits {
    Typedef I::value_type value_type;
}
现在，我们的func又有了新的面貌：
template <class I>
typename iterator_traits<I>::value_type func(I ite) {
    return *ite;
}
尽管这次我们的函数返回值的长度有些吓人，但是，我们的确为原生指针找到了好的解决方案。只要为原生指针提供一个偏特化的iterator_traits就OK了。如下：
template <class I>
struct iterator_traits<T*> {
    typedef T value_type;
};
下面，我们终于可以让func同时支持智能指针和原生指针了：
template <class I>
struct iterator_traits {
    Typedef I::value_type value_type;
}
template <class T>
struct iterator_traits<T*> {
    typedef T value_type;
};
template <class I>
typename iterator_traits<I>::value_type func(I ite) {
    return *ite;
}
int main() {
    MyIter<int> iter = new int(520);
    int *p = new int(520);
    // This time the following two statements will success
    cout<<func(iter)<<endl;
    cout<<func(p)<<endl;
    return 0;
}
但是，我们离最后的成功还有最后一步，如果，我们需要声明一个value_type类型的左值，但是却给iterator_traits传递了一个const int*，显然结果有问题，于是，为const T*也另起炉灶，准备一份小炒：
template<class T>
struct iterator_traits<const T*> {
    typedef T value_type;
}
OK，现在万事大吉，无论是正宗迭代器，原生指针，const原生指针，我们都可以利用iterator_traits萃取出其封装的对象的类型，萃取技术由此而来。
第二部分 基于泛型的类型萃取技术
总结一下，我们之所以要萃取迭代器相关的类型，无非是要把迭代器相关的类型用于声明局部变量、用作函数的返回值等一系列行为。对于原生指针和point-to-const类型的指针，采用模板偏特化技术对其进行特殊处理，另外，对于point-to-const类型的指针，为了保证声明左值时语义正确，特化时按照普通原生指针处理。
实际上，把我们刚才的例子提炼一下，迭代器相应类型不仅仅有迭代器封装的对象类型，STL中对这些类型作了整理，有如下几种：
template <class I>
struct iterator_traits {
    typedef typename I::iterator_category iterator_category;
    typedef typename I::value_type value_type;
    typedef typename I::difference_type difference_type;
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
}
当然，也如你所想，对于原生pointer和pointer-to-const这两种情况，STL分别对其进行了特化处理。如果你看了上面的代码却不知所云，也属正常，在去了解特化版本之前，我们先来看看这五种类型的含义。
Type 1 value_type
这个类型和我们在第一部分谈到的vlaue_type的含义是一样的，不多说了。
Type 2 difference_type
用来表示两个迭代器之间的最大距离。这个类型用来对某种算法提供计数功能，例如：
template <class I, class T>
typename iterator_traits<I>::difference_type
count(I first, I last, const T& value){
    typename iterator_traits<I>::difference_type n = 0;
    for(; first != last; first++) {
        if(*first == value)
            n++;
}
return n;
}
也许这个例子最足以说明问题，任何的解释都没必要了。这里需要说明的是。对于原生指针，由于不存在int::difference_type的情况，所以，iterator_traits对其进行特化：
template <class I>
class iterator_traits<I*> {
    typedef ptrdiff_t difference_type;
}
这里，ptrdiff_t是定义在cstddef中的一个C++内置类型，在GNU gcc中，定义如下：
typedef long int ptrdiff_t;
同样，对于pointer-to-const，也要入法炮制：
template <class I>
class iterator_traits<const I*> {
    typedef ptrdiff_t difference_type;
}
再一次，偏特化技术帮了大忙，现在count可以处理所有类型迭代器的difference_type了。
Type 3 reference
这里，reference type指的是迭代器封装对象的类型的引用。这个类型的出现主要是为了解决对指针进行解引用的时候，返回什么样的对象的问题。我们希望：
 MyIter<int> iter(new int(10));
*iter = 52;
和
Int *p = new int(10);
*p = 52;
是一样的。于是，reference_type一般用在迭代器的*运算符重载上，让所有的“指针家族”有同样的表现形式。于是，如果value_type是T，那么reference_type就是T&，如果value_type是const T，reference_type就是const T&。
Type 4 pointer
C++中指针和引用总是有着密切的关系。如果我们想返回迭代器封装的对象的地址，就需要用到这里的pointer_type，主要用在迭代器中对->运算符重载的问题。对于一个智能指针来说，通常我们都需要下面的两个运算符重载：
T& operator*() const { return *ptr; } // T& is reference type
T* operator->() const { return ptr; } // T* is pointer type
同样，为了能够对迭代器和原生指针都能够在算法上有统一的表现形式，在iterator_traits中加入了下面的类型
template <class T>
struct iterator_traits {
    typedef typename I::pointer pointer;
    typedef typename I::reference reference;
}
同样，对于原生指针和point-to-const类型的指针作了特化：
template<class T>
struct iterator_traits<T*> {
    typedef typename T* pointer;
    typedef typename T& reference;
}
而这次，对于point-to-const类型的指针，则有些特别：
template<class T>
struct iterator_traits<const T*> {
    typedef typename const T* pointer;
    typedef typename const T& reference;
}
也就是说，当我们解引用一个封装了常量对象的迭代器的时候，返回的类型应该是const T&，取一个封装了常量对对象的迭代器中的元素的地址，返回的应该是const T*。最终的结果，就是所有的算法都有了一个统一的表达方式：
template <class T>
typename iterator_traits<T>::reference func() {}
template <class T>
typename iterator_traits<T>::pointer func() {}
Type 5 iterator_category
这个类型的作用是按照迭代器的移动特性和能够在该迭代器上实施的操作对迭代器进行分类，之所以这样做，完全是为了效率的考量。不过，在我看来，对其分类的因素实际上只有迭代器的移动特性，而分类，也非常简单：一步步向前挪的类型和一步跨到位的类型。
在STL中，共有以下5种迭代器类型：
l         单向移动只读迭代器 Input Iterator
l         单向移动只写迭代器 Output Iterator
l         单向移动读写迭代器 Forward Iterator
l         双向移动读写迭代器 Bidirectional Iterator
以上4种属于单步向前挪型的迭代器，还有一种双向移动读写迭代器属于一步跨到位型：
l         随机访问迭代器 Random Access Iterator
按照强化关系，上面5种迭代器的关系如下：
Input Iterator        Output Iterator
     |                      |
     +-----------+----------+
                 |
          Forward Iterator
                 |
       Bidirectional Iterator
                 |
       Random Access Iterator
在STL的各种算法中，遍历元素是很常用的，于是我们就用advance()这个函数作个例子，看看每个迭代器的类型，这个函数负责把迭代器移动特定的长度：
// The input iterator version, an O(N) algorithm
template <class InputIterator, class Distance>
void Advance_II(InputIteraotr& i, Distance n) {
    while(n--) i++; // This is step by step moving
}
其实，Output和Forward类型的迭代器在移动上和Input类型是一样的。不再熬述，来看看Bidirectional类型：
// The bidirectional iterator version, an O(N) algorithm
template <class BidirectionalIterator, class Distance>
void Advance_BI(BidirectionalIterator& i, Distance n) {
    if(n >= 0)
        while(n--) i++;
    else
        while(n++) i++;
}
加入了双向移动，但仍然要单步进行。最后，看看随机访问类型：
// The random access version, an O(1) algorithm
template <class RandomAccessIterator, class Distance>
void Advance_RAI(RandomAccessIterator& i, Distance n) {
    i += n;
}
最后，我们可以构想一个把这3个函数封装起来的函数advance，专门负责迭代器的移动。
template <class InputIterator, class Distance>
void advance(InputIterator& I, Distance n) {
    if(is_ramdom_access_iterator(i)) // How to judge?
        advance_RAI(I, i);
    else if(is_bidirectional_iterator(i)) // How to judge?
        Advance_BI(I, i);
    else
        Advance_II(I, i);
}
但是，在程序运行时决定函数调用，显然效率不彰，最好能够让编译器在程序编译的时候决定函数调用，于是，我们要想方设法利用函数重载，让编译器帮助我们决策函数调用。这样，就需要我们对于迭代器的类型做一个统一的规划，OO正好能帮助我们解决这个问题，设计下面的继承结构，这和我们上面画的那张图是一样的：
// five tag classes
struct input_iterator_tag { }
struct output_iterator_tag { }
struct forward_iterator_tag : public input_iterator_tag { }
struct bidirectional_iterator_tag : public forward_iterator_tag { }
struct random_access_tag : public bidirectional_iterator_tag { }
之后，重新设计__advance，给它加上第3个参数——用以表明此迭代器类型的标签，根据此标签来决定不同的__advance操作（此时，type_traits技术派上了用场）。而对外开放的advance仍然不变:
template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n) {
    // Forward the correct messages
    __advance(i, n, type_traits<i>::iterator_category());
}
说到这里，你也就应该明白iterator_category的作用了，同样，为poiner准备了两个特化版本：
template <class T>
struct iterator_traits<T*> {
typedef random_access_iterator_tag iterator_category;
}
template <class T>
struct iterator_traits<const T*> {
typedef random_access_iterator_tag iterator_category;
}
道理很简单，所有的原生指针都支持随机访问。
第三部分 杂项
在STL中，所有的迭代器都遵从上面的设计原则，都要提供上面说过的五种类型，但是，人总会有挂一漏万的时候，为了设计上的方便，STL提供了一个标准的迭代器壳：
template <class Category,
class T,
class Distance = ptrdiff_t
class Pointer = T*
class Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T       value_type;
    typedef Distance difference_type;
    typedef Pointer  pointer;
    typedef Reference reference;
};
这样就免去了声明这些类型的麻烦，当你想自定义一个迭代器的时候：
template <class Item>
struct MyIter : public std::iterator<std::forward_iterator_tag, Item> { … }
就万事大吉了。
/////////////////////////////////////////////////////////////////////////////////////////////////
Traits技术可以用来获得一个 类型 的相关信息的。 首先假如有以下一个泛型的迭代器类，其中类型参数 T 为迭代器所指向的类型：
template <typename T>
class myIterator
{
 ...
};
当我们使用myIterator时，怎样才能获知它所指向的元素的类型呢？我们可以为这个类加入一个内嵌类型，像这样：
template <typename T>
class myIterator
{
      typedef  T value_type; 
...
};
这样当我们使用myIterator类型时，可以通过 myIterator::value_type来获得相应的myIterator所指向的类型。
现在我们来设计一个算法，使用这个信息。
template <typename T>
typename myIterator<T>::value_type Foo(myIterator<T> i)
{
 ...
}
这里我们定义了一个函数Foo，它的返回为为  参数i 所指向的类型，也就是T，那么我们为什么还要兴师动众的使用那个value_type呢？ 那是因为，当我们希望修改Foo函数，使它能够适应所有类型的迭代器时，我们可以这样写：
template <typename I> //这里的I可以是任意类型的迭代器
typename I::value_type Foo(I i)
{
 ...
}
现在，任意定义了 value_type内嵌类型的迭代器都可以做为Foo的参数了，并且Foo的返回值的类型将与相应迭代器所指的元素的类型一致。至此一切问题似乎都已解决，我们并没有使用任何特殊的技术。然而当考虑到以下情况时，新的问题便显现出来了：
原生指针也完全可以做为迭代器来使用，然而我们显然没有办法为原生指针添加一个value_type的内嵌类型，如此一来我们的Foo()函数就不能适用原生指针了，这不能不说是一大缺憾。那么有什么办法可以解决这个问题呢？ 此时便是我们的主角：类型信息榨取机 Traits 登场的时候了
....drum roll......
我们可以不直接使用myIterator的value_type，而是通过另一个类来把这个信息提取出来：
template <typename T>
class Traits
{
      typedef typename T::value_type value_type;
};
这样，我们可以通过 Traits<myIterator>::value_type 来获得myIterator的value_type，于是我们把Foo函数改写成：
template <typename I> //这里的I可以是任意类型的迭代器
typename Traits<I>::value_type Foo(I i)
{
 ...
}
然而，即使这样，那个原生指针的问题仍然没有解决，因为Trait类一样没办法获得原生指针的相关信息。于是我们祭出C++的又一件利器--偏特化(partial specialization)：
template <typename T>
class Traits<T*> //注意 这里针对原生指针进行了偏特化
{
      typedef typename T value_type;
};
通过上面这个 Traits的偏特化版本，我们陈述了这样一个事实：一个 T* 类型的指针所指向的元素的类型为 T。
如此一来，我们的 Foo函数就完全可以适用于原生指针了。比如：
int * p;
....
int i = Foo(p);
Traits会自动推导出 p 所指元素的类型为 int，从而Foo正确返回。
