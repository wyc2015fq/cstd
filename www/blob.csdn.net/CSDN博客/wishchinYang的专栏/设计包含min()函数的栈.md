# 设计包含min()函数的栈 - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:21:27[wishchin](https://me.csdn.net/wishchin)阅读数：250
                
题目：定义栈的数据结构，要求添加一个min函数，能够得到栈的最小元素。要求函数min、push以及pop的时间复杂度都是O(1)。 分析：这是去年google的一道面试题。
我看到这道题目时，第一反应就是每次push一个新元素时，将栈里所有逆序元素排序。这样栈顶元素将是最小元素。但由于不能保证最后push进栈的元素最先出栈，这种思路设计的数据结构已经不是一个栈了。
在栈里添加一个成员变量存放最小元素（或最小元素的位置）。每次push一个新元素进栈的时候，如果该元素比当前的最小元素还要小，则更新最小元素。 
乍一看这样思路挺好的。但仔细一想，该思路存在一个重要的问题：如果当前最小元素被pop出去，如何才能得到下一个最小元素？ 
因此仅仅只添加一个成员变量存放最小元素（或最小元素的位置）是不够的。我们需要一个辅助栈。每次push一个新元素的时候，同时将最小元素（或最小元素的位置。考虑到栈元素的类型可能是复杂的数据结构，用最小元素的位置将能减少空间消耗）push到辅助栈中；每次pop一个元素出栈的时候，同时pop辅助栈。
参考代码： 
```cpp
#include <deque>   
#include <assert.h>   
template <typename T> class CStackWithMin   
{   
public:   
       CStackWithMin(void) {}   
      virtual ~CStackWithMin(void) {}   
       T& top(void);   
      const T& top(void) const;   
  
      void push(const T& value);   
      void pop(void);   
  
      const T& min(void) const;   
  
private:   
      T> m_data;               // the elements of stack   
      size_t> m_minIndex;      // the indices of minimum elements   
};   
  
// get the last element of mutable stack   
template <typename T> T& CStackWithMin<T>::top()   
{   
      return m_data.back();   
}   
  
// get the last element of non-mutable stack   
template <typename T> const T& CStackWithMin<T>::top() const  
{   
      return m_data.back();   
}   
  
// insert an elment at the end of stack   
template <typename T> void CStackWithMin<T>::push(const T& value)   
{   
      // append the data into the end of m_data   
       m_data.push_back(value);   
  
      // set the index of minimum elment in m_data at the end of m_minIndex   
      if(m_minIndex.size() == 0)   
             m_minIndex.push_back(0);   
      else  
       {   
            if(value < m_data[m_minIndex.back()])   
                   m_minIndex.push_back(m_data.size() - 1);   
            else  
                   m_minIndex.push_back(m_minIndex.back());   
       }   
}   
  
// erease the element at the end of stack   
template <typename T> void CStackWithMin<T>::pop()   
{   
      // pop m_data   
       m_data.pop_back();   
      // pop m_minIndex   
       m_minIndex.pop_back();   
}   
  
// get the minimum element of stack   
template <typename T> const T& CStackWithMin<T>::min() const  
{   
       assert(m_data.size() > 0);   
       assert(m_minIndex.size() > 0);   
      return m_data[m_minIndex.back()];   
}
```
- 
举个例子演示上述代码的运行过程： 
   步骤               数据栈             辅助栈 (存小元素的位置)              最小值
1.push 3              3                     0                                                   3
2.push 4             3,4                  0,0                                                  3
3.push 2            3,4,2               0,0,2                                                2
4.push 1            3,4,2,1            0,0,2,3                                              1
5.pop                3,4,2               0,0,2                                                 2
6.pop               3,4                   0,0                                                    3
7.push 0           3,4,0                0,0,2                                                  0 
另一种想法 辅助栈 (存最小元素)    更快
  步骤               数据栈             辅助栈 (最小元素)                       最小值 
1.push 3              3                     3                                                  3 
2.push 4             3,4                 3，3                                               3 
3.push 2            3,4,2                3,3,2                                              2 
4.push 1            3,4,2,1             3，3，2，1                                    1 
5.pop                3,4,2                3,3,2                                              2 
6.pop               3,4                    3,3                                                3 
7.push 0           3,4,0                3,3,0                                               0 
讨论：如果思路正确，编写上述代码不是一件很难的事情。但如果能注意一些细节无疑能在面试中加分。比如我在上面的代码中做了如下的工作： 
·         用模板类实现。如果别人的元素类型只是int类型，模板将能给面试官带来好印象； 
·         两个版本的top函数。在很多类中，都需要提供const和非const版本的成员访问函数； 
·         min函数中assert。把代码写的尽量安全是每个软件公司对程序员的要求； 
·         添加一些注释。注释既能提高代码的可读性，又能增加代码量，何乐而不为？ 
总之，在面试时如果时间允许，尽量把代码写的漂亮一些。说不定代码中的几个小亮点就能让自己轻松拿到心仪的Offer。
