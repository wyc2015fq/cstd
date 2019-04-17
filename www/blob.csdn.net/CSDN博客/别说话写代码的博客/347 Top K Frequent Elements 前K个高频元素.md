# 347. Top K Frequent Elements 前K个高频元素 - 别说话写代码的博客 - CSDN博客





2019年02月25日 20:26:12[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：21标签：[前K个高频元素																[Top K Frequent Elements																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Top K Frequent Elements&t=blog)



|Given a non-empty array of integers, return the ***k*** most frequent elements.**Example 1:****Input: **nums = [1,1,1,2,2,3], k = 2**Output: **[1,2]**Example 2:****Input: **nums = [1], k = 1**Output: **[1]**Note:**- You may assume *k* is always valid, 1 ≤ *k* ≤ number of unique elements.- Your algorithm's time complexity **must be** better than O(*n* log *n*), where *n* is the array's size.|给定一个非空的整数数组，返回其中出现频率前 ***k ***高的元素。**示例 1:****输入: **nums = [1,1,1,2,2,3], k = 2**输出: **[1,2]**示例 2:****输入: **nums = [1], k = 1**输出: **[1]**说明：**- 你可以假设给定的 *k *总是合理的，且 1 ≤ k ≤ 数组中不相同的元素的个数。- 你的算法的时间复杂度**必须**优于 O(*n* log *n*) , *n *是数组的大小。|
|----|----|

思路：先利用哈希表统计每个数字出现的次数。然后将 出现的次数和数字值 打包放入堆中，只需要在堆中找到前k 大的数对应的值即可。现成的 堆就是priority_queue。

```cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int> m;
        vector<int> res;
        priority_queue<pair<int,int>> q;
        for(int i=0;i<nums.size();++i) m[nums[i]]++;  //先统计所有元素出现的次数
        for(auto it:m) q.push({it.second,it.first});     //将次数和对应的值放入堆中，遍历哈希表要用迭代器
        for(int i=0;i<k;++i)  //从堆中拿出k个元素
        {
            res.push_back(q.top().second);  //每次取堆顶元素对应的key
            q.pop();   //删掉堆顶元素
        }return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=前K个高频元素&t=blog)




