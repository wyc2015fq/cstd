# [C++]在程序中高效实用STL - 心纯净，行致远 - CSDN博客





2018年11月16日 09:50:08[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：50标签：[STL](https://so.csdn.net/so/search/s.do?q=STL&t=blog)
个人分类：[【C++/STL】](https://blog.csdn.net/zhanshen112/article/category/8367378)









# 在程序中使用STL1. 算法调用优先于手写循环

使用算法的有如下理由：
-         效率：算法通常比手写循环效率更高
-         正确性：自己写循环容易出错
-         可维护性：使用算法比手写循环更加易懂。

在效率上来说，

例如，在以下代码中，手写循环在每一次循环中都要计算vec.end()， 但在算法调用中只计算了一次vec.end()。因为在循环进行过程中，vec.end()并没有发生改变。

```cpp
for (vector<int>::iterator iter = vec.begin();iter != vec.end();iter++) {
        print(*iter);
    }
    for_each(vec.begin(), vec.end(), print);
```

因为STL的实现者很清楚，begin(), end(), size()这样的函数会经常被调用，所以为了提高他们的效率一般都会被定义为inline的，其次，为了避免在循环中反复计算他们的值，一般都会在循环外调用这个函数。

再就是，STL的实现者很清楚容器中所使用的数据结构，所以很清楚如何对它进行高效操作，而这一般是使用很难做到的。

在正确性来上说，

手写循环最重要的是保证你所使用的迭代器都是有效的，并且指向你所希望的地方。

就比如说一个在deque前部插入元素的方法：

```cpp
deque<int>::iterator iter = deq.begin();
    for (size_t i = 0; i < num; i++) {
        iter = deq.insert(iter, data[i]);
        iter++;
    }
```

这个代码容易想到吗？为了正确地保护iter的有效性已经准确的位置。

如果使用算法，则简单了许多。

```cpp
transform(data, data + num, insert(deq, deq.begin()), bind2nd(plus<int>(), 0);
```

但与此同时，我们可能会发现在很多情况下我们处理很复杂的操作，那么更好的方法是使用C++11中提供的匿名函数，可以更自由地完成相应的操作。

```cpp
int x = 2, y = 3;
    vector<int*>::iterator iter = find_if(v_i.begin(), v_i.end(), [&](int* i) -> bool{
        return *i != x && *i != y;
    });
```

代码的清晰程度仍然比手写代码更好！

# 2. 容器的成员函数优先于同名函数

> 
    首先，使用成员函数往往更快，成员函数通常与容器（特别是关联容器）结合得更好。


成员函数都是准备于特定容器的特定数据结构而设计的，所以他们的实现更加高效。例如对于标准的关联容器，使用成员函数，可以获得对数时间的效率，可以使用等价性来判断两个值是否相同，可以在map和multimap中值关注键的部分。

# 3. 正确使用查找函数

我们先来区分一下count, find, binary_search, lower_bound, upper_bound, equal_range.

    count是简单地查找所有满足条件的元素。

    find找到满足条件的元素，并返回它在哪里。

    binary_search是返回元素在不在。

    lower_bound是返回满足条件的第一个元素的位置。

    upper_bound是返回满足条件的元素后的第一个元素的位置。

    equal_range返回lower_bound和upper_bound。

需要注意的是，count和find都是使用相等性测试，而binary_search, lower_bound, upper_bound, equal_range都使用等价性测试！

这意味着在使用lower_bound时必须确保手工编的等价性测试代码和算法中使用的比较函数是相同的。

```cpp
#include <iostream>
#include <vector>
using namespace std;
int main() {
    vector<int> v_i{1, 2, 3, 4, 2, 3, 4, 5, 1, 4};
    sort(v_i.begin(), v_i.end(), [](int i, int j) -> bool {
        return i < j;
    });
    copy(v_i.begin(), v_i.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    vector<int>::iterator iter = lower_bound(v_i.begin(), v_i.end(), 3, [](int i, int j = 3) -> bool {
        return i != j;
    });
    if (iter != v_i.end() && *iter == 3) { //  判断是否存在
        cout << *iter << endl;
    }
    return 0;
}
```

使用equal_range也可以知道有多少个与之相等的值。通过区间的distance可以判读个数。

简单的总结就是：![这里写图片描述](https://img-blog.csdn.net/20160719103749584)

需要注意的是：对于set和map而言，find并不是第一个满足条件的对象位置，而是其中的任意一个对象。

# 4. 考虑使用函数对象而不是函数作为STL算法的参数

书上说了这么一句话，但我想说，使用函数对象还不如使用匿名函数来的方便。在C++11后，匿名函数几乎可以在所有需要predicate函数上使用，而且更加方便自由。甚至可以解决直写型代码的弊端，把比较的方法直接写在函数调用上。但问题就是，无法重复使用。毕竟匿名函数不可以封装起来。



