# Leetcode 88 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int k = nums1.size() - m;
        for(int i=0;i < k;i++){
            nums1.pop_back();
        }
        k = nums2.size() - n;
        for(int i=0;i < k;i++){
            nums2.pop_back();
        }
        
        nums1.push_back(INT_MAX);
        for(int i=0;i < n;i++){
            for(int j=0;j <= nums1.size();j++){
                if(nums2[i] <= nums1[j]){
                    nums1.insert(nums1.begin()+j,nums2[i]);
                    break;
                }
            }
        }
        nums1.pop_back();
    }
};
```













