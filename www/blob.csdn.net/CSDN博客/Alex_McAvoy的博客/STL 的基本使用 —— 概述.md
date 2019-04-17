# STL 的基本使用 —— 概述 - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:07:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：230








# 【概述】

STL 是指 C++ 标准模板库，是 C++ 语言标准中的重要组成部分，其以模板类和模版函数的形式提供了各种数据结构与算法的精巧实现，如果能充分使用 STL，可以在代码空间、执行时间、编码效率上得到极大的提高。

STL 大致可以分为三大类：容器(Container)、迭代器(iterator)、算法(algorithm)。

STL 容器是一些模板类，提供了多种组织数据的常用方法，例如：vector(向量)、list(列表)、deque(双向队列)、set(集合)、map(映象)、stack(栈)、queue(队列)、priority_queue(优先队列) 等，通过模版的参数可以指定容器中元素类型。

STL 算法是一些模板函数，提供了相当多的有用算法和操作，从简单如 for_each(遍历) 到复杂如 stable_sort(稳定排序)。

STL 迭代器是对 C 中的指针的一般化，用来将算法和容器联系起来，几乎所有的 STL 算法都是通过迭代器来存取元素序列进行工作的，而STL 中的每一个容器也都定义了其本身所专有的迭代器，用以存取容器中的元素。

# 【头文件】

在 C++ 标准中，STL 被组织为以下的 13 个头文件：

<algorithm>、<map>、<list>、<queue>、<set>、<stack>、<vector>、<iterator>、<deque>、<functional>、<memory>、<numeric>、<utility>

当需要使用 STL 的某个功能时，需要嵌入相应的头文件，要注意的是，在 C++ 标准中，STL 是被定义在 std 命名空间中的，因此使用时需要使用命名空间。

# 【具体使用】
- 容器与迭代器：[点击这里](https://blog.csdn.net/u011815404/article/details/88804594)
- 算法：[点击这里](https://blog.csdn.net/u011815404/article/details/88804938)



