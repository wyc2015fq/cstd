# leetcode-31.Next Permutation 下一个排列 - 别说话写代码的博客 - CSDN博客





2018年12月23日 20:56:19[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：48标签：[下一个排列																[Next Permutation																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Next Permutation&t=blog)




题目：
|Implement **next permutation**, which rearranges numbers into the lexicographically next greater permutation of numbers.If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).The replacement must be **[in-place](http://en.wikipedia.org/wiki/In-place_algorithm)** and use only constant extra memory.Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.`1,2,3` → `1,3,2``3,2,1` → `1,2,3``1,1,5` → `1,5,1`|实现获取下一个排列的函数，算法需要将给定数字序列重新排列成字典序中下一个更大的排列。如果不存在下一个更大的排列，则将数字重新排列成最小的排列（即升序排列）。必须**[原地](https://baike.baidu.com/item/%E5%8E%9F%E5%9C%B0%E7%AE%97%E6%B3%95)**修改，只允许使用额外常数空间。以下是一些例子，输入位于左侧列，其相应输出位于右侧列。`1,2,3` → `1,3,2``3,2,1` → `1,2,3``1,1,5` → `1,5,1`|
|----|----|

思路：从右往左，如果是顺序的，说明已经是最大的排列，只需要逆序（最小）输出就行。从右往左一直是顺序**直到某元素A是逆序**，那就说明 有更大的 排列，那就再从右往左找第一个比A大的元素B，A和B交换，然后将A之后所有元素逆序就可以了，来看个例子：

1　　2　　7　　4　　3　　1

它的下个排列是

1　　3　　1　　2　　4　　7

观察，若从右往左找到 第一个递减的元素2，这就说明有更大排列，然后再从右 往左找到第一个大于2的元素3，然后交换2和3，最后将交换后3位置以后的 元素全部逆序 就可以了

1　　2　　7　　4　　3　　1

1　　2　　7　　4　　3　　1

1　　3　　7　　4　　2　　1

1　　3　　1　　2　　4　　7

```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int i,j,len=nums.size();
        for(i=len-2;i>=0;--i)
        {
            if(nums[i+1]>nums[i])
            {
                for(j=len-1;j>i;--j)
                    if(nums[j]>nums[i])
                        break;
                swap(nums[j],nums[i]);
                reverse(nums.begin()+i+1,nums.end());
                return ;
            }
        }
        reverse(nums.begin(),nums.end());
    }
};
```

 思路来自：[http://www.cnblogs.com/grandyang/p/4428207.html](http://www.cnblogs.com/grandyang/p/4428207.html)](https://so.csdn.net/so/search/s.do?q=下一个排列&t=blog)




