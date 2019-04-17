# stl vector源码剖析 - 结构之法 算法之道 - CSDN博客





2011年08月12日 13:18:10[v_JULY_v](https://me.csdn.net/v_JULY_v)阅读数：24482标签：[vector																[iterator																[reference																[insert																[class																[destructor](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)
个人分类：[28.Source analysis](https://blog.csdn.net/v_JULY_v/article/category/865324)





### 前言

     项目组要实现一个算法库，其中涉及到了类似vector的一维数组的实现。特此，对stl中得vector做个学习和了解。有任何问题，欢迎不吝指正。谢谢。

### 一、如何实现vector   

     如果给你一道面试题，如何用数据结构实现STL中vector的功能？聪明的你会怎么做呢?或许你会如下所述：
- 或许，如果不考虑分配效率，只需要两个成员就可以实现了 

template <class   _Ty> 

class   Vector 

{ 

public: 

        Vector(int   nLen=0):m_nLen(nLen),m_Data(NULL) 

        { 

                if(nLen   >   0) 

                { 

                        m_Data   =   new   _Ty[nLen]; 

                } 

        } 

protected: 

        _Ty   *   m_Data; 

        int       m_nLen; 

}; 
- 或许，如下一个简单的思路实现： 


#include   <iostream> 


using   std::ostream; 

using   std::istream; 


class   Array   { 

      friend   ostream   &operator < <(   ostream   &,   const   Array   &   ); 

      friend   istream   &operator> > (   istream   &,   Array   &   ); 


public: 

      Array(   int   =   10   );             

      Array(   const   Array   &   );   

      ~Array();                               

      int   getSize()   const;           


      const   Array   &operator=(   const   Array   &   );   

      bool   operator==(   const   Array   &   )   const;     


      bool   operator!=(   const   Array   &right   )   const     

      {   

            return   !   (   *this   ==   right   );   


      }   


      int   &operator[](   int   );                             

      const   int   &operator[](   int   )   const;     


private: 

      int   size;   

      int   *ptr;   


};
- 或许你会说，应该用模板写。当数组大小变化时，就直接new   当前大小，将旧有的或拷贝或加入新的东西加入，然后删除旧有的m_pData;并更新m_nLen;

当数据大小不变化时，直接使用m_pData;。如果考虑分配效率，则还需要一个成员存储m_nMaxLen;实际的分配大小。 要记住一定删除旧的m_pData就可以。

    很快，你就会意识到，与其这样不知方向的摸着石头过河，不如直接拿来stl里的vector实现代码，来瞧个究竟。ok，下面，咱们来剖析下stl vector的实现。其中的分析借助了侯捷先生的stl源码剖析（大凡研究sgi stl源码，此书都不容忽略），然后再加入一些自己的理解。希望对你有所帮助（下面咱们分析的版本是sgi stl v2.9版）。

### 二、vector的类定义

    以下是vector定义的类中的一些数据成员和部分成员函数：

```cpp
template <class T, class Alloc = alloc>  // 预设使用 alloc 为配置器
class vector {
public:
  // 以下标示 (1),(2),(3),(4),(5)，代表 iterator_traits<I> 所服务的5個型别。
  typedef T value_type;				// (1)
  typedef value_type* pointer; 			// (2)
  typedef const value_type* const_pointer;
  typedef const value_type* const_iterator;
  typedef value_type& reference; 		// (3)
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type; 	// (4)
  // 以下，由于vector 所维护的是一个连续线性空間，所以不论其元素型別为何，
  // 原生指标都可以做为其迭代器而满足所有需求。
  typedef value_type* iterator;
  /* 根据上述写法，如果客户端写出如下的代码：
      vector<Shape>::iterator is;
      is 的型別其实就是Shape*
      而STL 內部运用 iterator_traits<is>::reference 时，获得 Shape&
                 运用iterator_traits<is>::iterator_category 时，获得
                      random_access_iterator_tag		(5)
      （此乃iterator_traits 针对原生指标的特化结果）
  */
  //此处省略了一些与本文主题相关性不大的内容.......
protected:
  // 专属之空间配置器，每次配置一個元素大小
  typedef simple_alloc<value_type, Alloc> data_allocator;

  // vector采用简单的连续线性空间。以两个迭代器start和end分別指向头尾，
  // 并以迭代器end_of_storage指向容量尾端。容量可能比(尾-头)还大，
  // 多余即借用空間。
  iterator start;   //表示目前使用空间的头
  iterator finish;  //表示目前使用空间的尾
  iterator end_of_storage;  //表示目前可用空间的尾

  void insert_aux(iterator position, const T& x);
  void deallocate() {
    if (start)
         data_allocator::deallocate(start, end_of_storage - start);
  }

  void fill_initialize(size_type n, const T& value) {
    start = allocate_and_fill(n, value);  // 配置空间并设初值
    finish = start + n;				// 调整水位
    end_of_storage = finish; 			// 调整水位
  }
```

  下面是另外一些成员操作函数的具体实现，

```cpp
public:
  iterator begin() { return start; }
  const_iterator begin() const { return start; }
  iterator end() { return finish; }
  const_iterator end() const { return finish; }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const { 
    return const_reverse_iterator(end()); 
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const { 
    return const_reverse_iterator(begin()); 
  }
  size_type size() const { return size_type(end() - begin()); }
  size_type max_size() const { return size_type(-1) / sizeof(T); }
  size_type capacity() const { return size_type(end_of_storage - begin()); }
  bool empty() const { return begin() == end(); }
  reference operator[](size_type n) { return *(begin() + n); }
  const_reference operator[](size_type n) const { return *(begin() + n); }

  vector() : start(0), finish(0), end_of_storage(0) {}
  // 以下建模式，允許指定大小 n 和初值 value
  vector(size_type n, const T& value) { fill_initialize(n, value); }
  vector(int n, const T& value) { fill_initialize(n, value); }
  vector(long n, const T& value) { fill_initialize(n, value); }
  explicit vector(size_type n) { fill_initialize(n, T()); }

  vector(const vector<T, Alloc>& x) {
    start = allocate_and_copy(x.end() - x.begin(), x.begin(), x.end());
    finish = start + (x.end() - x.begin());
    end_of_storage = finish;
  }

  template <class InputIterator>
  vector(InputIterator first, InputIterator last) :
    start(0), finish(0), end_of_storage(0)
  {
    range_initialize(first, last, iterator_category(first));
  }

  vector(const_iterator first, const_iterator last) {
    size_type n = 0;
    distance(first, last, n);
    start = allocate_and_copy(n, first, last);
    finish = start + n;
    end_of_storage = finish;
  }
#endif /* __STL_MEMBER_TEMPLATES */
  ~vector() { 
    destroy(start, finish);  // 全域函式，建构/解构基本工具。
    deallocate();   // 先前定义好的成员函式
  }
  vector<T, Alloc>& operator=(const vector<T, Alloc>& x);
  void reserve(size_type n) {
    if (capacity() < n) {
      const size_type old_size = size();
      iterator tmp = allocate_and_copy(n, start, finish);
      destroy(start, finish);
      deallocate();
      start = tmp;
      finish = tmp + old_size;
      end_of_storage = start + n;
    }
  }
```

### 三、vector中insert的实现     

    纷纷扰扰的细节，咱们一概忽略，最后，咱们来具体分析**vector中insert**（插入）一个元素的实现：

```cpp
// 從 position 开始，安插 n 個元素，元素初值为 x
template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x) {
  if (n != 0) { // 当 n != 0  才進行以下所有动作
    if (size_type(end_of_storage - finish) >= n) { 
      // 借用空间大于等于 「新增元素个数」
      T x_copy = x;
      // 以下計算插入点之后的现有元素个数
      const size_type elems_after = finish - position;	
      iterator old_finish = finish;
      if (elems_after > n) { 
        // 「插入点之后的现有元素个数」大于「新增元素个数」
        uninitialized_copy(finish - n, finish, finish);       //finish-n：整体后移
        finish += n;	                                  //将vector 尾端标记后移           
        copy_backward(position, old_finish - n, old_finish);  //插入点元素A后移至A‘，position->old—finish后移至old_finish
        fill(position, position + n, x_copy);                 // 从插入点开始填入新值
      }
      else {	
        // 「插入点之后的现有元素个数」小于等于「新增元素个数」
        uninitialized_fill_n(finish, n - elems_after, x_copy);  //1.新增元素x_copy插入至finish处
        finish += n - elems_after;                          //2.finish后移n_elems_after
        uninitialized_copy(position, old_finish, finish); //3.腾出空间，position->old_finish
        finish += elems_after;                             //4.finish再次后移
        fill(position, old_finish, x_copy);                 //5.插入新元素，（x_copy）position->old_finish
      }
    }
    else {
      // 借用空間小于「新增元素个数」（那就必须配置额外的内存）
      // 首先決定新长度：旧长度的兩倍，或旧长度+新增元素个数。
      const size_type old_size = size();        
      const size_type len = old_size + max(old_size, n);
      // 以下配置新的vector 空間
      iterator new_start = data_allocator::allocate(len);
      iterator new_finish = new_start;
      __STL_TRY {
        // 以下首先将旧vector 的插入点之前的元素复制到新空间。
        new_finish = uninitialized_copy(start, position, new_start);
        // 以下再将新增元素（初值皆为n）填入新空间。
        new_finish = uninitialized_fill_n(new_finish, n, x);
        // 以下再將旧vector 的插入点之后的元素复制到新空间。
        new_finish = uninitialized_copy(position, finish, new_finish);
      }
#         ifdef  __STL_USE_EXCEPTIONS 
      catch(...) {
        // 如有异常发生，实现 "commit or rollback" semantics.
        destroy(new_start, new_finish);
        data_allocator::deallocate(new_start, len);
        throw;
      }
#         endif /* __STL_USE_EXCEPTIONS */
      // 以下清除并释放旧的 vector 
      destroy(start, finish);
      deallocate();
      // 以下調整水位标记
      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + len;
    }
  }
}
```

    我想，如果本文只是单单给出上面的代码，你一定内心非常愤懑，道：晕，又是一篇什么鬼剖析，就一大堆代码加注释，看上去就是一堆乱码，有什么意思嘛。是的，我想，读者肯定并没有看懂上述insert的实现，那么，下面，请允许我引用stl源码剖析一书里面的三张图片，相信，看过图片之后，您就会对vector中insert的实现清晰不少了：

    如下**图4-3b-1**所示的情况是，备用空间为2，新增元素也为2，所以，备用空间>=新增元素个数，而插入点之后的元素个数为3大于新增元素个数2（原有元素个数3个+备用空间为2，共5个存储单位）。此种情况的处理方式是，相当于将插入点之后的原有的3个元素整体向后移2个单位，然后把要新增的2个元素从插入点处插入，刚好满足新增的2个元素加上原有的3个元素共同存储在5个单位的空间中。



![](http://hi.csdn.net/attachment/201108/12/0_1313128982ahw1.gif)

    如下**图4-3b-2**所示，插入点之后的现有元素个数2<=新增元素个数3，此种情况的处理方式为：相当于将插入点之后的原有的3个元素整体向后移三个单位，然后把新增的3个元素从原插入点处插入：

![](http://hi.csdn.net/attachment/201108/12/0_1313128989Jl7C.gif)

    如果原有空间不够，那么vector将实施所谓的动态增加大小，而动态增加大小，并不是指在原空间之后接连续新空间（因为无法保证原空间之后尚有可供配置的空间），而是以原大小的两倍另外配置一块较大空间，然后将原内容拷贝过来，然后才开始在原内容之后构造新元素，并释放原空间，这点可以从上述insert的实现中的第二部分，当借用空間小于「新增元素個數」（那就必须配置额外的内存）可以看出来。

    如下**图4-3b-3**所示（另外，必须提醒的是，经过上述操作后，一旦引起空间重新配置，指向原vector的所有迭代器就都失效了。这是一般人会犯的错误，务必小心。 --侯捷如是说）：

![](http://hi.csdn.net/attachment/201108/12/0_1313129002t8lg.gif)

#### 四、vector的扩展

    最后，我再贴一段代码，相当于是vector的高效应用（或者说是拓展）：

```cpp
/*
    Copyright (c) 2007-2011 iMatix Corporation
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see < http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_ARRAY_INCLUDED__
#define __ZMQ_ARRAY_INCLUDED__

#include <vector>
#include <algorithm>
//w++ 
//从个人风格上来讲，一般要拒绝这种类中成员函数全部内联的用法。

namespace zmq
{

    //  Base class for objects stored in the array. Note that each object can
    //  be stored in at most one array.

    class array_item_t
    {
    public:

        inline array_item_t () :
            array_index (-1)
        {
        }

        //  The destructor doesn't have to be virtual. It is mad virtual
        //  just to keep ICC and code checking tools from complaining.
        inline virtual ~array_item_t ()
        {
        }

        inline void set_array_index (int index_)
        {
            array_index = index_;
        }

        inline int get_array_index ()
        {
            return array_index;
        }

    private:

        int array_index;

        array_item_t (const array_item_t&);
        const array_item_t &operator = (const array_item_t&);
    };

	// stl vector是一种简单高效的容器，在尾端插入和删除元素，算法时间复杂度为O(1)常数阶，其他元素的插入和删除为O(n)线性阶，
	// 其中n为vector容器的元素个数。vector具有自动的内存管理功能，对于元素的插入和删除，可动态调整所占用的内存空间。

    //  Fast array implementation with O(1) access to item, insertion and
    //  removal. Array stores pointers rather than objects. The objects have
    //  to be derived from array_item_t class.

    template <typename T> class array_t
    {
    public:

        typedef typename std::vector <T*>::size_type size_type;

        inline array_t ()
        {
        }

        inline ~array_t ()
        {
        }

        inline size_type size ()
        {
            return items.size ();
        }

        inline bool empty ()
        {
            return items.empty ();
        }

        inline T *&operator [] (size_type index_)
        {
            return items [index_];
        }

        inline void push_back (T *item_)
        {
            if (item_)
                item_->set_array_index (items.size ());
            items.push_back (item_);
        }

        inline void erase (T *item_) {
            erase (item_->get_array_index ());
        }

        inline void erase (size_type index_) {
            if (items.back ())//back函数返回最末一个元素的引用
                items.back ()->set_array_index (index_);
            items [index_] = items.back ();
            items.pop_back ();
        }

        inline void swap (size_type index1_, size_type index2_)
        {
			//交换序号和内容
            if (items [index1_])
                items [index1_]->set_array_index (index2_);
            if (items [index2_])
                items [index2_]->set_array_index (index1_);
            std::swap (items [index1_], items [index2_]);
        }

        inline void clear ()
        {
            items.clear ();
        }

        inline size_type index (T *item_)
        {
            return (size_type) item_->get_array_index ();
        }

    private:

        typedef std::vector <T*> items_t;
        items_t items;

        array_t (const array_t&);
        const array_t &operator = (const array_t&);
    };

}

#endif
```

**说明**：@555，在webkit中的WTF模块中，它里面的vector是直接放弃了STL的vector,它是利用google的tcmalloc来管理内存的，比stl的高效。

**参考**：侯捷先生的stl源码剖析。

    ok，如果有任何问题，欢迎不吝指正。完。](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=iterator&t=blog)](https://so.csdn.net/so/search/s.do?q=vector&t=blog)




