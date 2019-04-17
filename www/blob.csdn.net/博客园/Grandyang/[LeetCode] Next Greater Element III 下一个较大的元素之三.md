# [LeetCode] Next Greater Element III 下一个较大的元素之三 - Grandyang - 博客园







# [[LeetCode] Next Greater Element III 下一个较大的元素之三](https://www.cnblogs.com/grandyang/p/6716130.html)







Given a positive 32-bit integer n, you need to find the smallest 32-bit integer which has exactly the same digits existing in the integer n and is greater in value than n. If no such positive 32-bit integer exists, you need to return -1.

Example 1:
Input: 12
Output: 21



Example 2:
Input: 21
Output: -1


这道题给了我们一个数字，让我们对各个位数重新排序，求出刚好比给定数字大的一种排序，如果不存在就返回-1。这道题给的例子的数字都比较简单，我们来看一个复杂的，比如12443322，这个数字的重排序结果应该为13222344，如果我们仔细观察的话会发现数字变大的原因是左数第二位的2变成了3，细心的童鞋会更进一步的发现后面的数字由降序变为了升序，这也不难理解，因为我们要求刚好比给定数字大的排序方式。那么我们再观察下原数字，看看2是怎么确定的，我们发现，如果从后往前看的话，2是第一个小于其右边位数的数字，因为如果是个纯降序排列的数字，做任何改变都不会使数字变大，直接返回-1。知道了找出转折点的方法，再来看如何确定2和谁交换，这里2并没有跟4换位，而是跟3换了，那么如何确定的3？其实也是从后往前遍历，找到第一个大于2的数字交换，然后把转折点之后的数字按升序排列就是最终的结果了。最后记得为防止越界要转为长整数型，然后根据结果判断是否要返回-1即可，参见代码如下：



解法一：

```
class Solution {
public:
    int nextGreaterElement(int n) {
        string str = to_string(n);
        int len = str.size(), i = len - 1;
        for (; i > 0; --i) {
            if (str[i] > str[i - 1]) break;
        }
        if (i == 0) return -1;
        for (int j = len - 1; j >= i; --j) {
            if (str[j] > str[i - 1]) {
                swap(str[j], str[i - 1]);
                break;
            }
        }
        sort(str.begin() + i, str.end());
        long long res = stoll(str);
        return res > INT_MAX ? -1 : res;
    }
};
```



下面这种解法博主感觉有些耍赖了，用到了STL的内置函数next_permutation，该数字实现的就是这样一个功能，找下一个全排序，刚好比当前的值大，贴上来权当好玩：



解法二：

```
class Solution {
public:
    int nextGreaterElement(int n) {
        string str = to_string(n);
        next_permutation(str.begin(), str.end());
        long long res = stoll(str);
        return (res > INT_MAX || res <= n) ? -1 : res;
    }
};
```



类似题目：

[Next Greater Element II](http://www.cnblogs.com/grandyang/p/6442861.html)

[Next Greater Element I](http://www.cnblogs.com/grandyang/p/6399855.html)



参考资料：

[https://discuss.leetcode.com/topic/85740/c-4-lines-next_permutation](https://discuss.leetcode.com/topic/85740/c-4-lines-next_permutation)

[https://discuss.leetcode.com/topic/86049/simple-java-solution-4ms-with-explanation](https://discuss.leetcode.com/topic/86049/simple-java-solution-4ms-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












