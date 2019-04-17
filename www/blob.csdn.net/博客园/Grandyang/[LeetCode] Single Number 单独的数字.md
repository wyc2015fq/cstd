# [LeetCode] Single Number  单独的数字 - Grandyang - 博客园







# [[LeetCode] Single Number  单独的数字](https://www.cnblogs.com/grandyang/p/4130577.html)







Given an array of integers, every element appears *twice* except for one. Find that single one.

Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?



本来是一道非常简单的题，但是由于加上了时间复杂度必须是O(n)，并且空间复杂度为O(1)，使得不能用排序方法，也不能使用map数据结构。那么只能另辟蹊径，需要用位操作Bit Operation来解此题，这个解法如果让我想，肯定想不出来，因为谁会想到用[**逻辑异或**](http://zh.wikipedia.org/wiki/%E9%80%BB%E8%BE%91%E5%BC%82%E6%88%96)来解题呢。逻辑异或的真值表为：

 异或运算![A \oplus B](http://upload.wikimedia.org/math/0/2/1/021a6af6071cb77c364718edc0ca959b.png)的[真值表](http://zh.wikipedia.org/wiki/%E7%9C%9F%E5%80%BC%E8%A1%A8)如下：
|A|B|⊕|
|----|----|----|
|F|F|F|
|F|T|T|
|T|F|T|
|T|T|F|

由于数字在计算机是以二进制存储的，每位上都是0或1，如果我们把两个相同的数字异或，0与0异或是0,1与1异或也是0，那么我们会得到0。根据这个特点，我们把数组中所有的数字都异或起来，则每对相同的数字都会得0，然后最后剩下来的数字就是那个只有1次的数字。这个方法确实很赞，但是感觉一般人不会忘异或上想，绝对是为CS专业的同学设计的好题呀，赞一个~~ 



C++ 解法：

```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for (auto num : nums) res ^= num;
        return res;
    }
};
```



Java 解法：

```
public class Solution {
    public int singleNumber(int[] nums) {
        int res = 0;
        for (int num : nums) res ^= num;
        return res;
    }
}
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












