# STL 的基本使用 —— vector 的使用 - Alex_McAvoy的博客 - CSDN博客





2019年01月13日 15:04:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：86








# 【概述】

vector 是定义在 <vector> 头文件中的向量容器模版类，其以连续数组的方式存储元素序列，可以将 vector 看作是以顺序结构实现的线性表。

当在程序中需要使用动态数组时，vector 是一个理想的选择，其在使用过程中动态地增长存储空间，存取任何元素都能在参数时间内完成，但若在结尾插入，效率比较高，若往中间某个位置插入，则其插入位置后的元素都要后移，效率相对较低。

定义：**vector<type> vectorName**

参数：type 代表 vector 中存储元素的类型

# 【构造函数】
- vector <type> c：产生一个空 vector，其中没有任何元素
- vector <type> c(n)：生成一个大小为 n 的 vector
- vector <type> c(n, elem)：产生一个大小为 n 的 vector，每个元素值都是 elem 
- vector <type> c1(c2)：产生另一个同型 vector 的副本，所有元素都被复制

```cpp
vector<int> v1;
vector<double> v2(10);
vector<string> v3(10,"abc");
vector<string> v4(v3);
```

# 【基本操作】

对 vector<type> v 的基本操作有：
- v[i]：访问第 i 个元素
- v.front()：返回首元素
- v.back()：返回尾元素
- v.size()：返回容器中实际的元素个数
- v.clear()：删除容器中所有元素
- v.empty()：判断容器是否为空，若为空返回真，否则返回假
- v.push_back(elem)：向表尾插入元素 elem
- v.pop_back()：删除表尾元素
- v1.swap(v2)、swap(v1,v2)：将 v1 和 v2 元素互换

# 【迭代器操作】

对 vector<type> v 的迭代器 it 的操作有：
- v.bedin()：返回迭代器指向的第一个元素的位置
- v.end()：返回迭代器指向的最后一个元素后的位置
- v.insert(it, elem)：向迭代器 it 指向的元素前插入新元素 elem
- v.insert(it, n, elem)：向迭代器 it 指向的元素前插入 n 个 elem
- v.insert(it, first, last)：将由迭代器 first 和 last 所指定的序列 [first, last) 插入到迭代器 it 指向的元素前面
- v.erase(it)：删除由迭代器 it 所指向的元素
- v.erase(first, last)：删除由迭代器 first 和 last 所指定的序列 [first, last)



