
# C++ STL 数据结构与算法 —— 排序 - Zhang's Wikipedia - CSDN博客


2018年07月07日 17:43:49[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：1311个人分类：[数据结构](https://blog.csdn.net/lanchunhui/article/category/6062855)



## 1. Top k 大的数
排序后直接索引输出：O(n\log n)
std::sort(v.rbegin(), r.rend());return v[k-1];
使用优先队列（O(k\log n)，每 pop 一个元素，都需重排大顶堆）：
std::priority_queue<int>PQ(v.rbegin(), v.rend());for(size_t i =0; i < k-1; ++i) {PQ.pop();
}returnPQ.top();
使用自带函数（随机选择算法，期望耗时O(n)，最坏O(n^2)）：
std::nth_element(v.rbegin(), v.rbegin()+k-1, v.rend());return v[k-1];

