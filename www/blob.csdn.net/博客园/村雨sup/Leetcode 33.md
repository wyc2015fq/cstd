# Leetcode 33 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//这题真的很有思维难度啊，前前后后弄了2个小时才写完。
//一定要首先将连续的一段找出来，如果target在里面就在里面找，如果不在连续段里就在另一部分找。
//如果后面二分到都是连续段也是没事了，我们找的另一段也是连续的没有问题的。
//最后面de的两个bug是判断nums[left] <= nums[mid])加的两个等于号，因为到最后会出现left == mid 的情况

class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size()-1;
        while(left <= right){
            int mid = (left+right)/2;
            if(target == nums[mid]) return mid;
            if(nums[left] <= nums[mid]){
                if(target <= nums[mid]&&target >= nums[left])
                    right = mid-1;
                else
                    left = mid+1;
            }
            else if(nums[mid] <= nums[right]){
                if(target <= nums[right]&&target >= nums[mid])
                    left = mid+1;
                else
                    right = mid-1;
            }
        }
        return -1;
    }
};
```













