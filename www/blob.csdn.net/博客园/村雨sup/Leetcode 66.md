# Leetcode 66 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//如何再vector头部插一个数
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int n = digits.size();
        int jinwei = 0;
        digits[n-1] += 1;
        if(digits[n-1] == 10){
            digits[n-1] = 0;
            jinwei = 1;
            int i=n-2;
            while(i >= 0){
                digits[i] += 1;
                if(digits[i] < 10) break;
                digits[i] %= 10;
                i--;
            }
        }
        if(digits.front() == 0)digits.insert(digits.begin(),1);
        return digits;
    }
};
```













