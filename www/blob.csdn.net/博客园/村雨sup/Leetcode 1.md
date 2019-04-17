# Leetcode 1 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> m;
        vector<int> res;
        for(int i=0;i < nums.size();i++){
            m[nums[i]] = i;
        }
        for(int i=0;i < nums.size();i++){
            int t = target - nums[i];
            if(m.count(t) !=0 && m[t] != i){
                res.push_back(i);
                res.push_back(m[t]);
                break;
            }
        }
        return res;
    }
};
```

hash map 构建查找表，用target减去第一个找第二个，要注意如果target是自己的两倍也是要排除的，即：

if(m.count(t) !=0 && m[t] != i)











