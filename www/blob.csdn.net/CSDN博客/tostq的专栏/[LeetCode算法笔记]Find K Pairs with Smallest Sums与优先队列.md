# [LeetCode算法笔记]Find K Pairs with Smallest Sums与优先队列 - tostq的专栏 - CSDN博客





2016年07月13日 14:12:09[tostq](https://me.csdn.net/tostq)阅读数：1692








LeetCode第373题 Find K Pairs with Smallest Sums是一道中等难度的题

题面挺简单的：

给定两组升序排列的整形（int）数组，从中分别挑选k对和最小的数对（pair）

意思就是从两个数组中分别挑选一个数，组成一个数队，使两个数的和最小

**例子：**

输入：nums1:[1,6,7,11]，nums2:[1,2,6,9]，k=4

输出：[[1,1],[1,2],[6,1][1,6]]

这个问题在于如何进行有效的和比较。

暴力的方法就是先计算所有对的和，然后再进行排序。

而优化在于如何利用数组本身已经升序排列的前提，从而减少所需要的排序。




一、利用priority_queue优先队列的解法

C++的解法主要是利用到了priority_queue优先队列：

priority_queue最简单的用法：priority_queue<int>


有点类似于set，会自动将插入队列的值按照优先级大小排序，默认是从大到小。

这题的解法可以采用更经典用法：

priority_queue <TYPE, vector<TYPE>, greater<TYPE> >: 这里的TYPE指各种类型数据，TYPE表示数据类型，vector<TYPE>表示存储数据类型的向量，greater<>表示进行优先级比较的函数，greater指从大到小，less指从小到大

例如：priority_queue <int, vector<int>, greater<int> >: 指的是一个按值从大到小排序的int类型向量。

priority_queue优先队列，也就是原来我们学过的堆，按照自己定义的优先级出队时。默认情况下底层是以Vector实现的heap。既然是队列，也就只有入队、出队、判空、大小的操作，并不具备查找功能。其常用成员函数：

empty() 如果优先队列为空，则返回真


pop() 删除第一个元素 

push() 加入一个元素 

size() 返回优先队列中拥有的元素的个数


top() 返回优先队列中有最高优先级的元素

那么在C++中有了这样一个容器，我们可以将两数组中数的序号构成一对键值，而比较函数设置两数和的


```cpp
class Solution {
public:
    vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<pair<int,int>> res; // record result
        if(nums1.size()==0||nums2.size()==0||k<=0)
            return res;
        
        auto cmp = [&nums1,&nums2](pair<int,int> a,pair<int,int> b){
            return nums1[a.first]+nums2[a.second]>=nums1[b.first]+nums2[b.second];
        };
        
        priority_queue<pair<int,int>,vector<pair<int,int>>,decltype(cmp)> pq(cmp);
        pq.push(make_pair(0,0)); // the first 
        int cnk=0;
        while(cnk<k){
            pair<int,int> t=pq.top();
            pq.pop();

            res.push_back(make_pair(nums1[t.first],nums2[t.second]));
            if(t.first+1<nums1.size())
                pq.push(make_pair(t.first+1,t.second));
            if(t.first==0 && (t.second+1<nums2.size()))
                pq.push(make_pair(t.first,t.second+1));
            if(t.second+1>=nums2.size()&&t.first+1>=nums1.size())
                break;
            cnk++;
        }
        
        return res;
    }
};
```







二、直接的算法

这个算法可以通过C语言来实现，不过LeetCode这道题的C语言版本的接口，我完全搞不清，所以就选择用C++来解决

这个算法的重点在于维持一个位置数组，这个数组保存数组1的某数搜索数组2的位置。


```cpp
class Solution {
public:
    vector<pair<int, int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
            vector<pair<int,int>> res;
            vector<int> pos(nums1.size()); // record the search pos of each num of nums1
            if(nums1.size()==0||nums2.size()==0||k==0)
                return res;
            
            int cnk=0;  // record push_back num
            while(cnk<k){
                int si=-1; // the search pos
                int minsum=INT_MAX; // the min sum
                
                for(int i=0;i<nums1.size();i++){
                    if(nums1[i]+nums2[pos[i]]<minsum&&pos[i]<nums2.size()){
                        minsum=nums1[i]+nums2[pos[i]];
                        si=i;
                    }
                }
                if(si==-1)
                    break;
                res.push_back(make_pair(nums1[si],nums2[pos[si]]));
                pos[si]++;
                cnk++;
            }
            return res;            
        }
};
```







三、Python版本

Python里有一个heapq结构，类似于前面C++的优先队列，这个方法同前面也很类似。


```python
def kSmallestPairs(self, nums1, nums2, k):
    queue = []
    def push(i, j):
        if i < len(nums1) and j < len(nums2):
            heapq.heappush(queue, [nums1[i] + nums2[j], i, j])
    push(0, 0)
    pairs = []
    while queue and len(pairs) < k:
        _, i, j = heapq.heappop(queue)
        pairs.append([nums1[i], nums2[j]])
        push(i, j + 1)
        if j == 0:
            push(i + 1, 0)
    return pairs
```






