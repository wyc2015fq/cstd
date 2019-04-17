# Leetcode 15 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//用类似双指针的方法，确定第一个i的位置后，j和k向左向右移动使nums[j]+nums[k] = -nums[i];注意特判

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        if (nums.empty()||nums.size() < 3) return {};
        set<vector<int>> res;
        sort(nums.begin(),nums.end());
        for(int i=0;i < nums.size()-2;i++){
            for(int j=i+1,k=nums.size()-1;j < k;){
                int sum = nums[j] + nums[k];
                if(sum < (-nums[i])){j++;}
                else if(sum > (-nums[i])){k--;}
                else{
                    vector<int> cnt;
                    cnt.push_back(nums[i]);
                    cnt.push_back(nums[j]);
                    cnt.push_back(nums[k]);
                    res.insert(cnt);
                    j++;k--;
                }
            }
        }
        return vector<vector<int>>(res.begin(),res.end());
    }
};
```













