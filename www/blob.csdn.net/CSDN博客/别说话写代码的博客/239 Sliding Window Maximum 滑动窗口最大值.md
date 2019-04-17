# 239. Sliding Window Maximum 滑动窗口最大值 - 别说话写代码的博客 - CSDN博客





2019年02月25日 19:54:08[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22标签：[Sliding Window Maximum																[leetcode																[滑动窗口最大值](https://so.csdn.net/so/search/s.do?q=滑动窗口最大值&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given an array *nums*, there is a sliding window of size *k* which is moving from the very left of the array to the very right. You can only see the *k* numbers in the window. Each time the sliding window moves right by one position. Return the max sliding window.**Example:****Input:***nums* = `[1,3,-1,-3,5,3,6,7]`, and *k* = 3**Output: **```[3,3,5,5,6,7] **Explanation: **```Window position                Max---------------               -----[1  3  -1] -3  5  3  6  7       **3** 1 [3  -1  -3] 5  3  6  7       **3** 1  3 [-1  -3  5] 3  6  7      ** 5** 1  3  -1 [-3  5  3] 6  7       **5** 1  3  -1  -3 [5  3  6] 7       **6** 1  3  -1  -3  5 [3  6  7]      **7****Note: **			You may assume *k* is always valid, 1 ≤ k ≤ input array's size for non-empty array.**Follow up:**			Could you solve it in linear time?|给定一个数组 *nums*，有一个大小为 *k *的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口 *k* 内的数字。滑动窗口每次只向右移动一位。返回滑动窗口最大值。**示例:****输入:***nums* = `[1,3,-1,-3,5,3,6,7]`, 和 *k* = 3**输出: **```[3,3,5,5,6,7] **解释: **```  滑动窗口的位置                最大值---------------               -----[1  3  -1] -3  5  3  6  7       3 1 [3  -1  -3] 5  3  6  7       3 1  3 [-1  -3  5] 3  6  7       5 1  3  -1 [-3  5  3] 6  7       5 1  3  -1  -3 [5  3  6] 7       6 1  3  -1  -3  5 [3  6  7]      7**注意：**你可以假设 *k *总是有效的，1 ≤ k ≤ 输入数组的大小，且输入数组不为空。**进阶：**你能在线性时间复杂度内解决此题吗？|
|----|----|

思路：这道题给定了一个数组，还给了一个窗口大小k，让我们每次向右滑动一个数字，每次返回窗口内的数字的最大值，而且要求我们代码的时间复杂度为O(n)。提示我们要用双向队列deque来解题，并提示我们窗口中只留下有用的值，没用的全移除掉。大概思路是用双向队列保存数字的下标，遍历整个数组，如果此时队列的首元素是i - k的话，表示此时窗口向右移了一步，则移除队首元素。然后比较队尾元素和将要进来的值，如果小的话就都移除，然后此时我们把队首元素加入结果中即可，参见代码如下：

具体分析过程如下（参考剑指offer）： 

![](https://img-blog.csdnimg.cn/20190225195319507.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> res;
        deque<int> q;
        for(int i=0;i<nums.size();++i)
        {
            while(!q.empty() && nums[i]>=nums[q.back()])  //当前数组元素大于双端队列后面的元素
                q.pop_back();
            if(!q.empty() && q.front()<=i-k) q.pop_front();  //当窗口滑过最大值后，删掉最大值
            q.push_back(i);
            if(i>=k-1) res.push_back(nums[q.front()]);  //能够构成一个窗口后将每个窗口最大值插入返回值列表
        }
        return res;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Sliding Window Maximum&t=blog)




