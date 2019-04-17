# Leetcode 34 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//二分查找后，向两边扩展，二分错了两次，现在是对的。
//还有就是vector可以用{}直接赋值很棒

class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size()-1;
        int mid = -1;
        bool flag = false;
        while(left<=right){
            mid = (left+right)/2;
            if(nums[mid] == target){flag = true;break;}
            else if(nums[mid] < target){left = mid+1;}
            else{right = mid-1;}
        }
        if(flag == false) return {-1,-1};
        
        int begin = mid;
        while(begin >= 0){
            if(nums[begin] != target) break;
            begin--;
        }
        begin++;
        
        int end = mid;
        while(end < nums.size()){
            if(nums[end] != target)break;
            end++;
        }
        end--;
        return {begin,end};
    }
};
```

```
//递归的二分
int search(vector<int>& nums, int left, int right, int target) {
        if (left > right) return -1;
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        else if (nums[mid] < target) return search(nums, mid + 1, right, target);
        else return search(nums, left, mid - 1, target);
    }
```













