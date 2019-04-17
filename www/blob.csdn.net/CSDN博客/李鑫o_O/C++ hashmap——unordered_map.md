# C++  hashmap——unordered_map - 李鑫o_O - CSDN博客





2016年05月12日 22:42:44[hustlx](https://me.csdn.net/HUSTLX)阅读数：3514









主要尝试回答下面几个问题：
- 一般情况下，使用 hash 结构，需要有桶的概念，那么 unordered_map 是如何自动管理桶的，这个问题其实再细分的话是这样的：
- 初始的桶是如何设置的
- 当需要扩容的时候，是如何重新分布的

- 对于 string，unordered_map 的默认哈希函数是怎样的

需要注意的是，unorder_map 和 unorder_set，其实都是一个封装而已，底下用的是 hashtable，所以分析也着重分析 hashtable




最根本的区别为底层的实现机制不同，map底层实现为红黑时，hash_map为hash表，所以就有一些其他方面的不同：

1）map存储的时候为排好序的，所以输出时候也是排序的。而hash_map不是的。

2）map具有稳定性，底层存储为树，这种算法差不多相当与list线性容器的折半查找的效率一样，都是O (log2N)。而hash_map使用hash表来排列配对，hash表是使用关键字来计算表位置。当这个表的大小合适，并且计算算法合适的情况下，hash表的算法复杂度为O(1)的，但是这是理想的情况下的，如果hash表的关键字计算与表位置存在冲突，那么最坏的复杂度为O(n)。 map在一次查找中，你可以断定它最坏的情况下其复杂度不会超过O(log2N)。而hash表就不一样，是O(1)，还是O(N)，或者在其之间，你并不能把握。


我觉得如果数量级很小，不到w，那么使用map和hash_map的区别不大，速度，稳定性都相差不大。但是如果数量级很大，就要考虑是要平均效率高，还是稳定性好了，如果用hash_map那么可以自己来根据经验来设定hash函数优化速度。而如果算法对稳定性要求高的话，首选map。



不过gnu hash_map和c++ stl的api不兼容，c++ tr1(C++ Technical Report1)作为标准的扩展，实现了hash map,提供了和stl兼容一致的api,称为unorder_map.在头文件 <tr1/unordered_map>中。另外c++ tr1还提供了正则表达式、智能指针、hash table、 随机数生成器的功能。 

Linux 下的hash_map



**[cpp]**[view
 plain](http://blog.csdn.net/ysu108/article/details/8039794#)[copy](http://blog.csdn.net/ysu108/article/details/8039794#)



- #include <iostream>
- #include <string>
- #include <tr1/unordered_map>
- usingnamespace std;  
- 
- int main(){  
- typedef std::tr1::unordered_map<int,string> hash_map;  
-  hash_map hm;  
-  hm.insert(std::pair<int,std::string>(0,"Hello"));  
-  hm[1] = "World";  
- for(hash_map::const_iterator it = hm.begin(); it != hm.end(); ++it){  
-   cout << it->first << "-> " << it->second << endl;  
-  }  
- return 0;  
- }  



先来看一个典型的操作，[ ] 运算符，在 679 行附近，有这样的代码

```cpp
template<typename K, typename Pair, typename Hashtable>
    typename map_base<K, Pair, extract1st<Pair>, true, Hashtable>::mapped_type&
    map_base<K, Pair, extract1st<Pair>, true, Hashtable>::
    operator[](const K& k)
    {
  Hashtable* h = static_cast<Hashtable*>(this);
  typename Hashtable::hash_code_t code = h->m_hash_code(k);
  std::size_t n = h->bucket_index(k, code, h->bucket_count());

  typename Hashtable::node* p = h->m_find_node(h->m_buckets[n], k, code);
  if (!p)
  return h->m_insert_bucket(std::make_pair(k, mapped_type()),
          n, code)->second;
  return (p->m_v).second;
    }
```


可以看到，这是典型的 hash 操作的写法
- 先对 key 算出 hash code
- 找到这个 hash code 对应的桶
- 在这个桶里面，遍历去找这个 key 对应的节点
- 把节点返回


需要注意的是，如果找不到节点，不是返回空，而是会创建一个新的空白节点，然后返回这个空白节点，这里估计是受到返回值的约束，因为返回值声明了必须为一个引用，所以总得搞一个东西出来才能有的引用


接下来看初始化过程，gdb 跟踪代码可以发现，在 /usr/include/c++/4.1.2/tr1/unordered_map:86，有下面这样的代码，可以看到，初始化的桶大小，被写死为 10。

```cpp
explicit
  unordered_map(size_type n = 10,
        const hasher& hf = hasher(),
        const key_equal& eql = key_equal(),
        const allocator_type& a = allocator_type())
  : Base(n, hf, Internal::mod_range_hashing(),
       Internal::default_ranged_hash(),
       eql, Internal::extract1st<std::pair<const Key, T> >(), a)
  { }
```


但是，我们看一下下面这个代码的输出

```cpp
#include <tr1/unordered_map>
#include <string>
#include <stdio.h>

int main() {
    std::tr1::unordered_map<std::string, int> m;
    printf("%d\n", m.bucket_count());
    return 0;
}
```


输出是 11。为什么呢，这个涉及到 rehash。他是初始化为 10，然后 rehash 为 11 了。


rehash 有两个问题，一个是判断什么时候需要 rehash，一个是什么时候需要 rehash，一个是怎么 rehash。


need_rehash 在 hasttable 的 614 附近：

```cpp
inline std::pair<bool, std::size_t>
  prime_rehash_policy::
  need_rehash(std::size_t n_bkt, std::size_t n_elt, std::size_t n_ins) const
  {
    if (n_elt + n_ins > m_next_resize)
      {
  float min_bkts = (float(n_ins) + float(n_elt)) / m_max_load_factor;
  if (min_bkts > n_bkt)
    {
      min_bkts = std::max(min_bkts, m_growth_factor * n_bkt);
      const unsigned long* const last = X<>::primes + X<>::n_primes;
      const unsigned long* p = std::lower_bound(X<>::primes, last,
                  min_bkts, lt());
      m_next_resize = 
        static_cast<std::size_t>(std::ceil(*p * m_max_load_factor));
      return std::make_pair(true, *p);
    }
  else 
    {
      m_next_resize = 
        static_cast<std::size_t>(std::ceil(n_bkt * m_max_load_factor));
      return std::make_pair(false, 0);
    }
      }
    else
      return std::make_pair(false, 0);
  }
```


来看他是怎么做的，首先是用一个 m_max_load_factor 的因子来判断目前的容量需要多少个哈希桶，如果需要 rehash，那么使用素数表来算出新的桶需要多大。


素数表在 491 行附近：

```cpp
template<int ulongsize>
    const unsigned long X<ulongsize>::primes[256 + 48 + 1] =
    {
      2ul, 3ul, 5ul, 7ul, 11ul, 13ul, 17ul, 19ul, 23ul, 29ul, 31ul,
```


初始的时候，m_max_load_factor(1), m_growth_factor(2), m_next_resize(0)，根据 std::lower_bound 来找到比 10 大的最小素数是 11，于是就分配为 11 个桶。


rehash 就很平淡无奇了，一个一个重算，然后重新填进去，没有什么特别的。

```cpp
template<typename K, typename V, 
     typename A, typename Ex, typename Eq,
     typename H1, typename H2, typename H, typename RP,
     bool c, bool ci, bool u>
    void
    hashtable<K, V, A, Ex, Eq, H1, H2, H, RP, c, ci, u>::
    m_rehash(size_type n)
    {
      node** new_array = m_allocate_buckets(n);
      try
  {
    for (size_type i = 0; i < m_bucket_count; ++i)
      while (node* p = m_buckets[i])
        {
    size_type new_index = this->bucket_index(p, n);
    m_buckets[i] = p->m_next;
    p->m_next = new_array[new_index];
    new_array[new_index] = p;
        }
    m_deallocate_buckets(m_buckets, m_bucket_count);
    m_bucket_count = n;
    m_buckets = new_array;
  }
      catch(...)
  {
    // A failure here means that a hash function threw an exception.
    // We can't restore the previous state without calling the hash
    // function again, so the only sensible recovery is to delete
    // everything.
    m_deallocate_nodes(new_array, n);
    m_deallocate_buckets(new_array, n);
    m_deallocate_nodes(m_buckets, m_bucket_count);
    m_element_count = 0;
    __throw_exception_again;
  }
    }
```


然后就是 hash 函数了。hash 函数位于 /usr/include/c++/4.1.2/tr1/functional:1194，对于 std::string，用的是下面这种 hash 函数

```cpp
template<>
    struct Fnv_hash<8>
    {
      static std::size_t
      hash(const char* first, std::size_t length)
      {
  std::size_t result = static_cast<std::size_t>(14695981039346656037ULL);
  for (; length > 0; --length)
    {
      result ^= (std::size_t)*first++;
      result *= 1099511628211ULL;
    }
  return result;
      }
    };
```


这个叫 FNV hash，[http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function](http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function) ，FNV
 有分版本，例如 FNV-1 和 FNV-1a，区别其实就是先异或再乘，或者先乘在异或，这里用的是 FNV-1a，为什么呢，维基里面说，The small change in order leads to much better avalanche characteristics，什么叫 avalanche characteristics 呢，这个是个密码学术语，叫雪崩效应，意思是说输入的一个非常微小的改动，也会使最终的 hash 结果发生非常巨大的变化，这样的哈希效果被认为是更好的。



