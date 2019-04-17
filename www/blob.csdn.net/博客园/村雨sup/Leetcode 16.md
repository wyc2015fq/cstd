# Leetcode 16 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//一次AC 有点爽的
class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        int res = 0;
        int minal = INT_MAX;
        sort(nums.begin(),nums.end());
        for(int i=0;i < nums.size()-2;i++){
            int cnt = target-nums[i];
            for(int j=i+1,k=nums.size()-1;j < k;){
                int sum = nums[j]+nums[k];
                if(sum < cnt){j++;}
                else if(sum > cnt){k--;}
                else{
                    res = target;
                    return res;
                }
                if(minal > abs(sum-cnt)){minal = abs(sum-cnt);res = sum+nums[i];}
            }
        }
        return res;
    }
};
```













