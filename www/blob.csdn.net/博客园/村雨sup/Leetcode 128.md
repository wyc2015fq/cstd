# Leetcode 128 * - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        int res = 0;
        unordered_map<int,int> m;
        for(int i=0;i < nums.size();i++){
            if(m.count(nums[i])) continue;
            int left = (m.count(nums[i]-1) > 0 ? m[nums[i]-1]:0);
            int right = (m.count(nums[i]+1) > 0 ? m[nums[i]+1]:0);
            int sum = left + right + 1;
            m[nums[i]] = sum; // 为什么要加这个，不是改变两端么
            res = max(res,sum);
            m[nums[i]-left] = sum;
            m[nums[i]+right] = sum;
        }
        return res;
    }
};
```

还有bug，太晚了不想想了，下次再补











