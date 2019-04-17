# Leetcode 526 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    int countArrangement(int N) {
        vector<int> nums;
        for(int i=1;i <= N;i++) nums.push_back(i);
        return DFS(N,nums);
    }
    
    int DFS(int n,vector<int> nums)
    {
        int res = 0;
        if(n <= 0){
            return 1;
        }
        else{
            for(int i=0;i < n;i++){
                if(nums[i]%n == 0||n%nums[i] == 0){//这题关键在此，就是全排列加上特判
                    swap(nums[i],nums[n-1]);
                    res += DFS(n-1,nums);
                    swap(nums[n-1],nums[i]);
                }               
            }
        }
        return res;
    }
};
```

1 2 3 4 5

5 2 3 4 1

4 2 3 5 1

3 2 4 5 1

2 3 4 5 1

发现排列到最后最后一个数字是不变的，当然这和交换顺序有关，所以在判断的时候要选择不变的每次递归最后的数字判断！











