# Sweet Snippet 系列之 有序列表 - tkokof1的专栏 - CSDN博客

2018年07月26日 20:27:43[tkokof1](https://me.csdn.net/tkokof1)阅读数：42


> 
工作中常常遇到需要使用有序列表的情况,这篇文章简单讨论一下相关实现(以 C# 中的 List<T> 为例)

##### 使用 List<T>.Sort

很朴素的一种想法,为了维持 List 有序,我们可以在 Add 操作之后进行 Sort 操作(Remove 操作后不需要重新 Sort):

```
list.Add(value);
list.Sort();
```

该方案的缺点是时间消耗比较大,每次 Add 操作之后都要执行费时的 Sort 操作

##### 借助平台库中的 SortedList<Tkey, TValue> etc.

使用平台库内建的 SortedList<Tkey, TValue>,我们可以立即实现有序列表功能,这也应该是我们大部分情况下的选择,稍有缺陷的是,平台库中的 SortedList 需要指定 TKey 和 TValue,这在存储非映射类数据时(譬如存储单一的 int 数值)显得有些内存浪费~ 
*(类似的还有 SortedDictionary<TKey, TValue>)*

##### 那 SortedSet<T> 怎么样?

比起内部使用数组实现的 List 而言,目前默认使用[红黑树](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree)实现的 SortedSet 会有更多的内存消耗,而且也不提供索引形式的访问,不过在插入和删除操作上,他更有时间优势~

##### 其实我们可以自己封装

基于 List 有序这个前提,每次进行 Add 时,我们可以使用[插入排序](https://en.wikipedia.org/wiki/Insertion_sort)来添加元素,这样我们便可以省去之后的 Sort 操作,而 List 本身提供的 BinarySearch(二分查找)功能正好可以帮助我们实现插入排序~

```
// simple sorted list implementation using insert sort
// maintainer hugoyu

using System.Collections.Generic;

namespace Util
{
    public class SortedList<T>
    {
        public SortedList(IComparer<T> comparer = null)
        {
            m_comparer = comparer;
        }

        public int Count
        {
            get
            {
                return m_elementList.Count;
            }
        }

        public T this[int index]
        {
            get
            {
                return m_elementList[index];
            }
        }

        public bool Contains(T item)
        {
            return m_elementList.BinarySearch(item, m_comparer) >= 0;
        }

        public void Add(T item)
        {
            var index = m_elementList.BinarySearch(item, m_comparer);
            if (index < 0)
            {
                m_elementList.Insert(~index, item);
            }
            else
            {
                m_elementList.Insert(index, item);
            }
        }

        public bool Remove(T item)
        {
            var index = m_elementList.BinarySearch(item, m_comparer);
            if (index >= 0)
            {
                m_elementList.RemoveAt(index);
                return true;
            }

            return false;
        }

        public void Clear()
        {
            m_elementList.Clear();
        }

        IComparer<T> m_comparer;
        List<T> m_elementList = new List<T>();
    }
}
```

完整的代码在[这里(gist)](https://gist.github.com/tkokof/848be24521be16e4bc6d7852d5e761a8)

软件开发的核心就是**权衡**,下次如果你需要使用有序列表,会选择怎么实现呢?

