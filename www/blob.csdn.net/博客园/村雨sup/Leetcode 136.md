# Leetcode 136 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int,int> m;
        for(int i=0;i < nums.size();i++){
            m[nums[i]]++;
        }
        unordered_map<int,int>::iterator iter;
        for(iter = m.begin();iter != m.end();iter++){
            if(iter->second == 1) return iter->first;
        }
        return 0;
    }
};
```

这个写法只为熟悉一下map的基本操作，复杂度不符合要求。

```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for(int num:nums) res ^= num;
        return res;
    }
};
```

异或操作可还行











