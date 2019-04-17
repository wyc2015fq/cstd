# Leetcode 18 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        if(nums.size() < 4) return {};
        set<vector<int>> res;
        sort(nums.begin(),nums.end());
        for(int i=0;i < nums.size()-3;i++){
            for(int j=i+1;j < nums.size()-2;j++){
                int cnt = target - nums[i] - nums[j];
                for(int m=j+1,n=nums.size()-1;m < n;){
                    int sum = nums[m]+nums[n];
                    if(sum < cnt){m++;}
                    else if(sum > cnt){n--;}
                    else{
                        res.insert({nums[i],nums[j],nums[m],nums[n]});
                        m++;n--;
                    }
                }
            }
        }
        return vector<vector<int>>(res.begin(),res.end());
    }
};
```













