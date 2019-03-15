# STL之deque实现详解

2018年03月13日 14:56:27

 

有时需要偏执狂

 

阅读数：1798

更多

所属专栏： [STL源代码剖析](https://blog.csdn.net/column/details/20280.html)



 版权声明：本文为博主原创文章，未经博主允许随意转载。	https://blog.csdn.net/u010710458/article/details/79540505

# 何为deque

deque是双向开口的连续线性空间（动态将多个连续空间通过指针数组接合在一起），随时可以增加一段新的空间，所以数据想vector里面的分配，复制，释放操作不会发生。deque头尾两端分别做插入和删除操作都是常数时间。**能用vector尽量使用vector，通过deque用来做队列即可，十分方便的。**

不浮躁，慢慢看，就会理解其中的奥妙。 
花了半天的时间仔细看了一遍SGI-STL`deque`的实现，需要理解其原理，最重要的就是理解其内存模型是咋样的。一切东西，只要理解了内存模型，那么代码看起来都是那么那么的清晰，明了，感叹大神对内存如神的管理。在理解代码之前，必须理解**指针加上数字代表啥意思**，例如`int *a[10];`这是一个指针数组，a的类型是`int **`，那么`a+3`表示指向第三个元素，对应地址的数值实际上等于`(long)a + sizeof(int *)`。这点理解了，那么很多东西都可以理解很通透了。。。。因为毕竟数据结构就是探讨如何在内存上面合理的组织数据。

# deque的Demo

```
//这里以侯jj的代码为例子讲解，实际上，
//现在的libxx上面关于缓存设定这部分有了变化。
//为了和侯jj书上面的图形对应，直接用它的代码。
//理解了原理之后，就会明白一切那么简单。
int main(void)
{
    deque<int , alloc , 8> ideq(20,9)；//一段连续缓冲区可以存放8个int=32字节初始化内存，并将20个元素初始化为9.
    cout <<"size = " << ideq.size() << endl;//20 deque上面存储的元素个数

    for(int i = 0; i < ideq.size() ; ++i)//插入+输出
        ideq[i] = i;
    for(int i = 0; i < ideq.size() ; ++i)
        cout << ideq[i] = ' ';
    cout << endl;//0 1 2 3 ... 19

    for(int i = 0 ; i < 3 ; i++)//插入
        ideq.push_back(i);

    for(int i = 0; i < ideq.size() ; ++i)//输出
        cout << ideq[i] = ' ';
    cout << endl;//0 1 2 3 ... 19 0 1 2 
    cout <<"size = " << ideq.size() << endl;//23 deque上面存储的元素个数

    ideq.push_back(3)
    for(int i = 0; i < ideq.size() ; ++i)//输出
        cout << ideq[i] = ' ';
    cout << endl;//0 1 2 3 ... 19 0 1 2 3
    cout <<"size = " << ideq.size() << endl;//24 deque上面存储的元素个数

    ideq.push_front(99)
    for(int i = 0; i < ideq.size() ; ++i)//输出
        cout << ideq[i] = ' ';
    cout << endl;//99 0 1 2 3 ... 19 0 1 2  3
    cout <<"size = " << ideq.size() << endl;//25 deque上面存储的元素个数

    ideq.push_front(98)
    ideq.push_front(97)
    for(int i = 0; i < ideq.size() ; ++i)//输出
        cout << ideq[i] = ' ';
    cout << endl;// 97 98 99 0 1 2 3 ... 19 0 1 2   3
    cout <<"size = " << ideq.size() << endl;//27 deque上面存储的元素个数

    deque<int , alloc , 32>::iterator ite = find(ideq.begin() , ideq.end() , 99);
    cout << *ite << endl;
    cout << *(ite.cur) << endl;
}12345678910111213141516171819202122232425262728293031323334353637383940414243444546
```

这里有点说明，侯jj里面是`deque<int , alloc , 32> ideq(20,9)；`然后画出了书本上面对应的图形，这个例子有点小错误，实际上这里只有初始化为`deque<int , alloc , 8> ideq(20,9)；`才是对应与书本上面的内存分配图形，这样每个缓冲区才对应32字节。通过下面几个函数解释：

```
inline size_t __deque_buf_size(size_t n, size_t sz)
{
  return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}1234
```

这里当n不等于0时候，返回是n。

```
static size_type buffer_size() {//返回
    return __deque_buf_size(BufSiz, sizeof(value_type));
 }123
```

这里BufSiz != 0的时候，直接返回buffer_size。

```
//将内存接口包装，以满足STL标准接口
template<class T, class Alloc>
class simple_alloc {
public:
    static T *allocate(size_t n)
                { return 0 == n? 0 : (T*) Alloc::allocate(n * sizeof (T)); }
    static T *allocate(void)
                { return (T*) Alloc::allocate(sizeof (T)); }
    static void deallocate(T *p, size_t n)
                { if (0 != n) Alloc::deallocate(p, n * sizeof (T)); }
    static void deallocate(T *p)
                { Alloc::deallocate(p, sizeof (T)); }
};
typedef simple_alloc<value_type, Alloc> data_allocator;
allocate_node() { return data_allocator::allocate(buffer_size()); }123456789101112131415
```

而分配节点使通过调用`data_allocator::allocate(buffer_size())`一次分配`buffer_size()*sizeof(T)`字节，这里相当于分配`BufSiz*sizeof(int)`字节，只有当模板参数`BufSiz=8`时候，一个`Node`才是32字节，才和剖析书本绘图一致。假如模板参数`BufSiz=0`，那就缓冲区就是分配了512字节，`buffer_size()`的返回值是`512/sizeof(value_type)`,也就是缓冲区可以存储多少个value_type。

```
template <class T, class Alloc = alloc, size_t BufSiz = 0> 
class deque {
            .....
            }1234
```

总体来说在SGI-STL中，BufSiz是表示一个缓冲节点可以存储T的个数，而缓冲区大小等于BufSiz*sizeof(T)。

# deque内存模型

上述Demo中`deque<int , alloc , 4> ideq(20,9)；`之后，ideq的内存模型如下图，依据下图我们分析内存空间是如何构造以及迭代器是如何工作的。 
![这里写图片描述](https://img-blog.csdn.net/20180315094926763?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

首先可以明白`deque`模板中肯定需要`map，start，finish`三个数据用来管理整个内存空间。其一，`map`是指针数组，里面成员是分配空间Node的地址，如何明白如何动态分配二维数组，那么这个map就很容易理解；其二，迭代器，迭代器里面含有4个成员，连续空间开始地址（first），结束地址（last），空间中当前元素的地址（cur）以及连续空间地址在map中的位置（node），上图可以看很清楚。

## 1、迭代器设计

### 一些类型定义，以及三个重要的变量：

```
  typedef random_access_iterator_tag iterator_category;
  typedef T value_type;
  typedef Ptr pointer;
  typedef Ref reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T** map_pointer;

  typedef __deque_iterator self;

  T* cur;//指向当前元素，类型是指针
  T* first;//指向连续内存片段头，指针
  T* last;//指向连续内存片段尾，指针
  map_pointer node;//指针的指针，因为存放的是node的地址，之所以需要是因为map是从中间开始填充的。这种方式在设计迭代器的时候更加方便。1234567891011121314
```

### 构造函数以及操作符重载

`deque`的迭代器必须支持`++ -- * += -=`等等操作，但是deque迭代器是类不是普通指针，纯天然不支持这些操作，那么为了让迭代器支持这些操作，就只有通过操作符重载来实现了，使得其操作和普通指针之间没有差别，这种机制在C++里面广泛只用，将本来完全不一样的东西，在操作上面做到了统一，统一的代价就是没一个操作都对应了函数函数的调用。

```
  __deque_iterator(T* x, map_pointer y)//构造函数，当前指针，y初始化 
    : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
  __deque_iterator() : cur(0), first(0), last(0), node(0) {}
  __deque_iterator(const iterator& x)
    : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

  reference operator*() const { return *cur; }//为了支持*ite
  pointer operator->() const { return &(operator*()); }//为了支持ite->cur

  difference_type operator-(const self& x) const {//为了支持ite1 - ite2，返回元素个数
    return difference_type(buffer_size()) * (node - x.node - 1) +
      (cur - first) + (x.last - x.cur);
  }

  self& operator++() {//++ite
    ++cur;
    if (cur == last) {
      set_node(node + 1);
      cur = first;
    }
    return *this; 
  }
  self operator++(int)  {//ite++
    self tmp = *this;
    ++*this;
    return tmp;
  }

  self& operator--() {//--ite
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }
  self operator--(int) {//ite--
    self tmp = *this;
    --*this;
    return tmp;
  }

  void set_node(map_pointer new_node) {//设定节点信息
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }
   //实现随机存储。迭代器可以直接跳跃n个距离
  //要么直接在cur上面加，当碰到当前片段last，则重新换内存片段，再在cur上面加
  self& operator+=(difference_type n) {//支持ite+=n
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < difference_type(buffer_size()))
      cur += n;//如果还在当前节点，直接加
    else {//否则跳到下个节点
      difference_type node_offset =
        offset > 0 ? offset / difference_type(buffer_size())
                   : -difference_type((-offset - 1) / buffer_size()) - 1;
      set_node(node + node_offset);
      cur = first + (offset - node_offset * difference_type(buffer_size()));
    }
    return *this;//返回当前对象引用
  }

  self operator+(difference_type n) const {//重载const重载+号。
    self tmp = *this;
    return tmp += n;
  }

  self& operator-=(difference_type n) { return *this += -n; }//ite -=n通过+ -n实现。

  self operator-(difference_type n) const {//重载-
    self tmp = *this;
    return tmp -= n;
  }

    //实现随机存储，迭代器调用operator* 和 operator+
  reference operator[](difference_type n) const { return *(*this + n); }//重载ite[]操作，通过+实现

  bool operator==(const self& x) const { return cur == x.cur; }//重载ite1 == ite2
  bool operator!=(const self& x) const { return !(*this == x); }//重载ite1 != ite2
  bool operator<(const self& x) const {
    return (node == x.node) ? (cur < x.cur) : (node < x.node);
  }//重载ite1 < ite21234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768697071727374757677787980818283
```

上述重载函数，可以完全实现类似指针的操作，总结起来就是通过三个成员变量，在已经分配的连续内存片段上面取值或者存数值。

```
  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + difference_type(buffer_size());
  }12345
```

当迭代器++或–的时候，一旦遇到缓冲区边缘，使前进后退而定，需要换取内存片段的时候，那么需用`set_node`更新迭代器三个成员变量的值，挑一个缓冲区。

## deque的数据结构设计

```
template <class T, class Alloc = alloc, size_t BufSiz = 0> 
class deque {
public:                         // Basic types
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

public:                         // Iterators
#ifndef __STL_NON_TYPE_TMPL_PARAM_BUG
  typedef __deque_iterator<T, T&, T*, BufSiz>              iterator;
  typedef __deque_iterator<T, const T&, const T&, BufSiz>  const_iterator;
#else /* __STL_NON_TYPE_TMPL_PARAM_BUG */
  typedef __deque_iterator<T, T&, T*>                      iterator;
  typedef __deque_iterator<T, const T&, const T*>          const_iterator;
#endif /* __STL_NON_TYPE_TMPL_PARAM_BUG */

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
  typedef reverse_iterator<const_iterator> const_reverse_iterator;
  typedef reverse_iterator<iterator> reverse_iterator;
#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */
  typedef reverse_iterator<const_iterator, value_type, const_reference, 
                           difference_type>  
          const_reverse_iterator;
  typedef reverse_iterator<iterator, value_type, reference, difference_type>
          reverse_iterator; 
#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */

protected:                      // Internal typedefs
  typedef pointer* map_pointer;
  typedef simple_alloc<value_type, Alloc> data_allocator;
  typedef simple_alloc<pointer, Alloc> map_allocator;

  static size_type buffer_size() {//返回
    return __deque_buf_size(BufSiz, sizeof(value_type));
  }
  static size_type initial_map_size() { return 8; }

protected:                      // Data members
  iterator start;//开始迭代器，其中cur指向头部元素
  iterator finish;//结束迭代器，其中cur指向尾部元素后面的一个元素

  map_pointer map;//指向指针数组
  size_type map_size;//指针数组元素个数

  typedef __deque_iterator<T, T&, T*, BufSiz>    iterator;//迭代器类
  typedef simple_alloc<value_type, Alloc> data_allocator;//连续内存块 内存器
  typedef simple_alloc<pointer, Alloc> map_allocator;//指针数组 内存分配器
  iterator start;//开始迭代器，其中cur指向头部元素
  iterator finish;//结束迭代器，其中cur指向尾部元素后面的一个元素
  map_pointer map;//指向指针数组
  size_type map_size;//指针数组元素个数12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455
```

这些数据成员，看上图就直到是必须的。一个`deque`需要一个指向指针数组的变量，用来管理已经分配的连续内存片段，还需要开始和结束迭代器，用来实现连续片段上面单个变量的存储和修改（迭代器里面全部都是指针，修改对应的cur指针即可实现，这里用到很多操作符重载计数）。

## 通过例子一步一步讲解内存模型

1、首先调用`deque<int , alloc , 4> ideq(20,9)；`函数，对应于下面构造函数及其调用函数

```
deque(size_type n, const value_type& value)
    : start(), finish(), map(0), map_size(0)
  {
    fill_initialize(n, value);
  }

template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::fill_initialize(size_type n,
                                               const value_type& value) {
  create_map_and_nodes(n);
  map_pointer cur;
  __STL_TRY {
    for (cur = start.node; cur < finish.node; ++cur)
      uninitialized_fill(*cur, *cur + buffer_size(), value);
    uninitialized_fill(finish.first, finish.cur, value);//尾部可能有多余空间
  }
}
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::create_map_and_nodes(size_type num_elements) {
  size_type num_nodes = num_elements / buffer_size() + 1;
  /*
    相当于20/8 + 1 = 3。刚好整除，则多分配一个节点
*/

  map_size = max(initial_map_size(), num_nodes + 2);//map至少管理8个节点，最多是所需节点+2，前后各预留一个位置，扩充时候使用。
  map = map_allocator::allocate(map_size);//分配指针数组

  //先使用map指针数组中间的位置，方便前后扩充
  map_pointer nstart = map + (map_size - num_nodes) / 2;
  map_pointer nfinish = nstart + num_nodes - 1;

  map_pointer cur;
  __STL_TRY {
    for (cur = nstart; cur <= nfinish; ++cur)
      *cur = allocate_node();//初始化指针数组成员
  }
  start.set_node(nstart);//存储开始node
  finish.set_node(nfinish);//存储结束node
  start.cur = start.first;//指向第一个元素
  finish.cur = finish.first + num_elements % buffer_size();//指向最后元素的后面一个元素形成[start , finish)左闭右开空间。
}1234567891011121314151617181920212223242526272829303132333435363738394041
```

对应于下面过程`fill_initialize`首先调用`create_map_and_nodes`分配指针数组对应的内存和节点连续空间对应的内存，然后调用`uninitialized_fill`通过对应的构造函数初始化节点内存块。尾部设定稍微不同，因为可能有剩余空间。最后设定起始和终止迭代器。上述完成之后，内存空间布局如下。 
![这里写图片描述](https://img-blog.csdn.net/20180315152501393?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、接下来调用`ideq[i]` 
先调用deque类里面的操作操作符重载函数`reference operator[](size_type n) { return start[difference_type(n)];`，进而调用迭代器类里面的操作符重载函数`reference operator[](difference_type n) const { return *(*this + n); }`，进而通过迭代器访问了元素。这里一个小小看似简单的操作其实涉及大量的重载运算符也就是大量的函数调用过程，访问异常复杂。在使用中给了人方便，但是相应的代价就是设计大量函数调用降低了效率。注意这里通过`start[i]`访问元素，其中`start`迭代器里面的内容没有变化，因为最后是通过operator+

```
  self operator+(difference_type n) const {//对象重载+号
    self tmp = *this;
    return tmp += n;
  }1234
```

上述返回了对象self，注意看effective上面全部有说明。执行完毕后对应的内存模型如下： 
![这里写图片描述](https://img-blog.csdn.net/20180315155419294?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
3、调用`push_back(0);push_back(1);push_back(2)` 
看看下面函数注释就可以，具体考虑的细节不用明白，知道架构即可。后面有多余空间，直接构造，否则添加节点，再构造。

```
  //尾部添加元素
  void push_back(const value_type& t) {
    if (finish.cur != finish.last - 1) {//尾部还有多余空间，一个以上的空间
      construct(finish.cur, t);//直接构造
      ++finish.cur;//调整缓冲区状态finish的cur+1
    }
    else
      push_back_aux(t);//没有或者只剩下一个，添加node，然后构造
  }

// Called only if finish.cur == finish.last - 1.
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
  value_type t_copy = t;
  reserve_map_at_back();//加入后是否大于map内存空间
  *(finish.node + 1) = allocate_node();//分配节点，node
  __STL_TRY {
    construct(finish.cur, t_copy);//构造元素
    finish.set_node(finish.node + 1);
    finish.cur = finish.first;//设定finish
  }
  __STL_UNWIND(deallocate_node(*(finish.node + 1)));//释放返回
}1234567891011121314151617181920212223
```

![这里写图片描述](https://img-blog.csdn.net/20180315160251147?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4、再次调用`push_back(3)` 
此时尾端仅仅只可以容纳一个元素，于是调用`push_back_aux`，再调用`allocate_node`分配一个节点空间。 
![这里写图片描述](https://img-blog.csdn.net/2018031519324778?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

5、调用`push_front(99)` 
前面没有位置了，然后前面需要动态在添加一个位置，不像vector一样，需要移动再添加。这个就是deque的方便之处。这里调用了`push_front_aux`增加了一个节点。

```
  void push_front(const value_type& t) {
    if (start.cur != start.first) {
      construct(start.cur - 1, t);
      --start.cur;
    }
    else
      push_front_aux(t);
  }

  // Called only if start.cur == start.first.
template <class T, class Alloc, size_t BufSize>
void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
  value_type t_copy = t;
  reserve_map_at_front();//是否需要重新分配map
  *(start.node - 1) = allocate_node();//分配node
  __STL_TRY {
    start.set_node(start.node - 1);
    start.cur = start.last - 1;
    construct(start.cur, t_copy);
  }
} 123456789101112131415161718192021
```

![这里写图片描述](https://img-blog.csdn.net/20180315193446380?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

6、再次调用`push_front(98)，push_front(97)` 
内存模型如下： 
![这里写图片描述](https://img-blog.csdn.net/20180315193604210?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTA3MTA0NTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

7、当节点个数太多，8个节点map无法满足的时候，那么就需要动态分配map，设计map内存分配，数据拷贝，然后释放原来。

```
  void reserve_map_at_back (size_type nodes_to_add = 1) {
    if (nodes_to_add + 1 > map_size - (finish.node - map))
      reallocate_map(nodes_to_add, false);
  }
  void reserve_map_at_front (size_type nodes_to_add = 1) {
    if (nodes_to_add > start.node - map)
      reallocate_map(nodes_to_add, true);
  }12345678
```

功能就是增加指针数组维度。至此deque分析到处结束，再用起来[得心应手](https://www.baidu.com/s?wd=%E5%BE%97%E5%BF%83%E5%BA%94%E6%89%8B&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)，知道了其内存布局之后，那么就清晰明了了。