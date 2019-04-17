# leetcode-55. Jump Game 跳跃游戏 - 别说话写代码的博客 - CSDN博客





2019年01月10日 20:55:14[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：30标签：[跳跃游戏																[leetcode																[Jump Game](https://so.csdn.net/so/search/s.do?q=Jump Game&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given an array of non-negative integers, you are initially positioned at the first index of the array.Each element in the array represents your maximum jump length at that position.Determine if you are able to reach the last index.**Example 1:****Input:** [2,3,1,1,4]**Output:** true**Explanation:** Jump 1 step from index 0 to 1, then 3 steps to the last index.**Example 2:****Input:** [3,2,1,0,4]**Output:** false**Explanation:** You will always arrive at index 3 no matter what. Its maximum             jump length is 0, which makes it impossible to reach the last index.|给定一个非负整数数组，你最初位于数组的第一个位置。数组中的每个元素代表你在该位置可以跳跃的最大长度。判断你是否能够到达最后一个位置。**示例 1:****输入:** [2,3,1,1,4]**输出:** true**解释:** 从位置 0 到 1 跳 1 步, 然后跳 3 步到达最后一个位置。**示例 2:****输入:** [3,2,1,0,4]**输出:** false**解释:** 无论怎样，你总会到达索引为 3 的位置。但该位置的最大跳跃长度是 0 ， 所以你永远不可能到达最后一个位置。|
|----|----|

思路：

**第一种**方法：用max表示i+nums[i]表示当前位置最大能达到的位置，reach表示所有位置最大能达到的位置。若所有位置能达到的最大位置大于等于n-1，也就是reach>=n-1，则true。若reach不能达到i，也就是前面位置的最大能达到的位置<i，返回失败，比如说 示例2中的当i=4时，前面最大能达到的位置是reach=3；所以结束

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n=nums.size(),reach=0,max=0;
        for(int i=0;i<n;++i)
        {
            if(reach>=n-1) return true;
            if(reach<i) return false;
            max = i+nums[i];
            if(reach<max) reach=max; 
        }
    }
};
```

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int n=nums.size(),reach=0;
        for(int i=0;i<n;++i)
        {
            if(reach>=n-1) return true;
            if(reach<i) return false;
            reach=max(reach,i+nums[i]);  //一步代替两步
        }
    }
};
```

**第二种**方法：DP。我们维护一个一位数组dp，其中dp[i]表示达到i位置时剩余的步数，那么难点就是推导状态转移方程啦。我们想啊，到达当前位置的剩余步数跟什么有关呢，其实是跟上一个位置的剩余步数和上一个位置的跳力有关，这里的跳力就是原数组中每个位置的数字，因为其代表了以当前位置为起点能到达的最远位置。所以当前位置的剩余步数（dp值）和当前位置的跳力中的较大那个数决定了当前能到的最远距离，而下一个位置的剩余步数（dp值）就等于当前的这个较大值减去1，因为需要花一个跳力到达下一个位置，所以我们就有状态转移方程了：dp[i] = max(dp[i - 1], nums[i - 1]) - 1，如果当某一个时刻dp数组的值为负了，说明无法抵达当前位置，则直接返回false，最后循环结束后直接返回true即可

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        vector<int> dp(nums.size(),0);
        for(int i=1;i<nums.size();++i)
        {
            dp[i]=max(dp[i-1],nums[i-1])-1;
            if(dp[i]<0) return false;
        }return true;
    }
};
```

参考：[http://www.cnblogs.com/grandyang/p/4371526.html](http://www.cnblogs.com/grandyang/p/4371526.html)](https://so.csdn.net/so/search/s.do?q=跳跃游戏&t=blog)




