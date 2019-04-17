# STL 的基本使用 —— 容器与迭代器 - Alex_McAvoy的博客 - CSDN博客





2019年03月25日 20:01:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：40
个人分类：[—————STL 的使用—————																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7515461)








# 【容器】

容器主要分为三类：序列式容器（vector、list、deque）、适配器容器（stack、queue、priority_queue）、关联式容器（set、multiset、map、multimap）

其特点如下：
- 向量(vector)：连续存储的元素，位于 <vector>
- 列表(list)：由节点组成的双向链表，每个结点包含着一个元素，位于 <list>
- 双端队列(deque)：连续存储的指向不同元素的指针所组成的数组，位于 <deque>
- 栈(stack)：后进先出的值的排列，位于 <stack>
- 队列(queue)：先进先出的值的排列，位于 <queue>
- 优先队列(priority_queue)：元素的次序是由作用于所存储的值对上的某种谓词决定的的一种队列，位于 <queue>
- 集合(set)：由节点组成的红黑树，每个节点都包含着一个元素，节点之间以某种作用于元素对的谓词排列，没有两个不同的元素能够拥有相同的次序，位于 <set>
- 多重集合(multiset)：允许存在两个次序相等的元素的集合，位于 <set>
- 映射(map)：由键-值对组成的集合，以某种作用于键对上的谓词排列，位于 <map>
- 多重映射(multimap)：允许键对有相等的次序的映射，位于 <map>

容器的具体使用：
- pair（元素对）：[点击这里](https://blog.csdn.net/u011815404/article/details/86376405)
- vector（向量）：[点击这里](https://blog.csdn.net/u011815404/article/details/86417722)
- list（列表）、stack（栈）、queue（队列）与 deque(双端队列)：[点击这里](https://blog.csdn.net/u011815404/article/details/86419939)
- priority_queue（优先队列）：[点击这里](https://blog.csdn.net/u011815404/article/details/86421623)
- set（集合）与 multiset（多重集合）：[点击这里](https://blog.csdn.net/u011815404/article/details/86434707)
- map（映射）与 multimap（多重映射）：[点击这里](https://blog.csdn.net/u011815404/article/details/86435538)

# 【迭代器的使用】

C++ 的 STL 为每一种容器都定义了一种迭代器类型，迭代器是一种检查容器内元素并遍历元素的数据类型，C++ 中，对容器的访问操作更趋向于是用迭代器而非下标操作，只有少数容器（如 vector）支持下标操作访问容器元素。

迭代器类型可以用 * 操作符来访问迭代器所指向的元素，以 *iter = 0 为例，假设 it 指向 vector 对象 v 的第一个元素，那么 *iter 与 v[0] 就是指向同一个元素，那么 *iter = 0 就是将这个元素赋值为 0，同数组类似，迭代器可以使用自增、自减操作符向前、向后移动迭代器指向容器中的下一个元素。

迭代器的常用操作：
- *iter：对 iter 进行解引用，返回迭代器 iter 指向的元素的引用
- iter->men：对 iter 进行解引用，获取指定元素中名为 men 的成员，等效于 (*iter).men
- ++iter、iter++：给 iter 加 1，使其指向容器的下一个元素
- --iter、iter--：给 iter 减 1，使其指向容器的前一个元素
- iter1==iter2：比较两个迭代器是否相等
- iter1!=iter2：比较两个迭代器是否不等      

几乎每种容器都定义了一对 begin()、end() 函数，用于返回相应的迭代器，如果容器中有元素的话，由 begin() 返回的迭代器指向第一个元素，而由 end() 返回的迭代器指向容器中最后一个元素的下一个位置，其没有指向任何实际的元素，它只是起一个标志的作用，表示已处理完容器中的所有元素。由于 end() 返回的迭代器不指向任何元素，因此不能对它进行解引用(*)或自增(++)操作。

假设已经声明一个 vector<int> 的 ivec 容器，可以用迭代器来遍历 ivec 容器，把其每个元素重置为0：

```cpp
vector<int> v;
vector<int>::iterator iter=ivec.begin();
for(;iter!=ivec.end();iter++)
    *iter=0;
```

#  【例题】

## 1.栈
- 后缀表达式的值**（信息学奥赛一本通-T1331）(stack 计算后缀表达式)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012110)
- 计算**（信息学奥赛一本通-T1356）(stack 计算中缀表达式)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012213)
- 中缀表达式值**（信息学奥赛一本通-T1358）(stack 计算中缀表达式)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012354)
- 扩号匹配问题**（信息学奥赛一本通-T1203）(stack 匹配括号)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80275408)
- 表达式括号匹配**（信息学奥赛一本通-T1353）(stack 匹配括号)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012160)
- 括弧匹配检验**（信息学奥赛一本通-T1354）(stack 匹配括号)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012165)
- 字符串匹配问题**（信息学奥赛一本通-T1355）(stack 匹配括号)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012196)
- Vasya and Books（CF-1073B）**(stack+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/83590145)
- 车厢调度（**信息学奥赛一本通-T1355）(stack+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80012260)
- 炫酷雪花（2019牛客寒假算法基础集训营 Day5-H）**(stack+贪心)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86810311)

## 2.队列与优先队列
- 围圈报数（信息学奥赛一本通-T1334）**(queue+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/80628903)
- Reorder the Array（CF-1008C）**(priority_queue****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81986965)
- Fence Repair（POJ-3253）**(priority_queue****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81081908)
- Stall Reservations（POJ-3190）**(priority_queue****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81147281)
- The Average（POJ-2833）**(两次 priority_queue****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86894255)
- 玩具（BZOJ-1307）**(queue+priority_queue)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86907136)

## 3.其他
- Walking Between Houses（CF-1015D）**(vector+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81450540)
- Registration system（NYOJ-991）**(map+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86989839)
- Equal Sums（CF-988C）**(map + vector)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81486629)
- Summarize to the Power of Two（CF-1005C）**(map + 贪心)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81434910)
- Points and Powers of Two（CF-988D）**(map +数学推导)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81486905)
- 炫酷五子棋（2019牛客寒假算法基础集训营 Day5-B）**(map+搜索)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86763042)
- Equalize the Remainders（CF-999D）**(set+模拟)**：[点击这里](https://blog.csdn.net/u011815404/article/details/81487180)
- 找连续数（HDU-5247）**(set+打表)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86484460)
- Lines（HDU-5124）**(pair+前缀和****)**：[点击这里](https://blog.csdn.net/u011815404/article/details/86545842)
- 循环数组最大字段和（51Nod-1050）**(deque+前缀和)**：[点击这里](https://blog.csdn.net/u011815404/article/details/89321459)




