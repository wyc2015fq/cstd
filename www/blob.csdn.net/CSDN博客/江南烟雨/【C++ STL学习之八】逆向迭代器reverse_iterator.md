# 【C++ STL学习之八】逆向迭代器reverse_iterator - 江南烟雨 - CSDN博客
2012年11月09日 10:19:58[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2573
对于逆向迭代器，很重要的一点是需要弄清楚逻辑位置和实际位置二者的区别。
下图显示了逆向迭代器的位置和所指的数值：
![](https://img-my.csdn.net/uploads/201211/09/1352427755_9234.jpg)
可以发现，逆向迭代器所指位置（实际位置）和所代表的的数值（逻辑位置或数值）是不同的。C++这么做是有其原因的。导致这个行为的原因是区间的半开性。为了能够制定容器内的所有元素，我们必须运用最后一个元素的下一个位置。但是对于reverse迭代器而言，这个位置位于第一个元素之前。这时候问题就出现了，这个位置也许并不存在，因为容器并不要求其第一个元素之前的位置合法。
因此，逆向迭代器运用了一个小技巧：实际上倒置了“半开原则”，即逆向迭代器所定义的区间不包括起点，而包括终点。但是逻辑上一如常态。这样就导致了逆向迭代器实际所指的元素位置和逻辑上所指的元素位置就不一致。
下面再看看将一个迭代器转化为逆向迭代器的过程：
![](https://img-my.csdn.net/uploads/201211/09/1352427774_4142.jpg)
可以发现，迭代器的实际位置（元素）不变，但是逻辑位置（元素）发生了变化。图中pos迭代器转化为逆向迭代器rpos后实际位置还是5，但是逻辑位置是4.即逻辑元素位置是实际位置的前一个位置。测试代码：
```cpp
#include <iostream> 
   #include <vector> 
   #include <algorithm> 
   using namespace std; 
    int main()  
   { 
       vector<int> coll; 
 
 
       //insert elements from 1 to 9 
       for (int i=1; i<=9; ++i) { 
            coll.push_back(i); 
       } 
 
 
       //find position of element with value 5 
       vector<int>::iterator pos; 
       pos = find (coll.begin(), coll.end(), 
                   5); 
 
 
       //print value to which iterator pos refers 
       cout << "pos: " << *pos << endl; 
 
 
       //convert iterator to reverse iterator rpos 
       vector<int>::reverse_iterator rpos(pos); 
 
 
       //print value to which reverse iterator rpos refers 
       cout << "rpos: " << *rpos <<endl; 
   }
```
输出结果是（VS2008）：
![](https://img-my.csdn.net/uploads/201211/09/1352427811_4552.jpg)
