# LeetCode笔记：462. Minimum Moves to Equal Array Elements II - weixin_33985507的博客 - CSDN博客
2017年11月23日 09:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
## 问题：
> 
Given a non-empty integer array, find the minimum number of moves required to make all array elements equal, where a move is incrementing a selected element by 1 or decrementing a selected element by 1.
You may assume the array's length is at most 10,000.
Example:
> 
Input:
[1,2,3]
Output:
2
Explanation:
Only two moves are needed (remember each move increments or decrements one element):
[1,2,3]  =>  [2,2,3]  =>  [2,2,2]
## 大意：
> 
给出一个非空整型数组，找到需要移动的最小值来让数组的所有元素都相等，一次移动是指将指定元素加一或者减一。
你可以假设数组的长度不超过10000。
例子：
> 
输入：
[1,2,3]
输出：
2
解释：
只需要两次移动（记住每次移动是指增减一个元素）：
[1,2,3]  =>  [2,2,3]  =>  [2,2,2]
## 思路：
题目的描述有一点误导性，主要是用了“移动”这个词，而且给出的例子也不够明确，一开始我误以为是要将元素进行位移，导致想的很复杂，后来才发现是对元素进行加减。
只是加减就很简单了，我们要通过最小的加减数来使所有的元素都相同，最快的方式是往中间靠拢，这就需要先给数组排序，然后取其中间的数，由于每次“移动”都只能加一或者减一，所以“移动”的次数其实就是两数之间的差值。这样遍历一次都进行一次减法就行了，当然要记得取绝对值。
## 代码（Java）：
```java
public class Solution {
    public int minMoves2(int[] nums) {
        Arrays.sort(nums);
        int middle = nums[nums.length/2];
        int result = 0;
        for (int i = 0; i < nums.length; i++) {
            result += Math.abs(middle - nums[i]);
        }
        return result;
    }
}
```
## 他山之石：
```java
public class Solution {
    public int minMoves2(int[] nums) {
        Arrays.sort(nums);
        int i = 0, j = nums.length-1;
        int count = 0;
        while(i < j){
            count += nums[j]-nums[i];
            i++;
            j--;
        }
        return count;
    }
}
```
同样的思路，这种做法理想情况下会快一半。
合集：[https://github.com/Cloudox/LeetCode-Record](https://link.jianshu.com?t=https://github.com/Cloudox/LeetCode-Record)
[查看作者首页](https://www.jianshu.com/users/9ec19ab8c802/timeline)
