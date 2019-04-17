# Leetcode 75 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
记录一下常熟复杂度的双指针写法：


- 定义red指针指向开头位置，blue指针指向末尾位置



- 从头开始遍历原数组，如果遇到0，则交换该值和red指针指向的值，并将red指针后移一位。若遇到2，则交换该值和blue指针指向的值，并将blue指针前移一位。若遇到1，则继续遍历。


class Solution {
public:
    void sortColors(int A[], int n) {int red = 0, blue = n - 1;
        for (int i = 0; i <= blue; ++i) {
            if (A[i] == 0) {
                swap(A[i], A[red++]);
            } else if (A[i] == 2) {
                swap(A[i--], A[blue--]);
            } 
        }
    }
};
```













