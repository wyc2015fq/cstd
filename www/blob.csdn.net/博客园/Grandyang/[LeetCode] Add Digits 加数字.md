# [LeetCode] Add Digits 加数字 - Grandyang - 博客园







# [[LeetCode] Add Digits 加数字](https://www.cnblogs.com/grandyang/p/4741028.html)







Given a non-negative integer `num`, repeatedly add all its digits until the result has only one digit.

Example:
Input: `38`
Output: 2 
Explanation: The process is like: `3 + 8 = 11`, `1 + 1 = 2`. 
             Since `2` has only one digit, return it.

Follow up:
Could you do it without any loop/recursion in O(1) runtime?

**Hint:**
- A naive implementation of the above process is trivial. Could you come up with other methods? 
- What are all the possible results?
- How do they occur, periodically or randomly?
- You may find this [Wikipedia article](https://en.wikipedia.org/wiki/Digital_root) useful.



这道题让我们求[数根](https://zh.wikipedia.org/zh/%E6%95%B8%E6%A0%B9)，所谓树根，就是将大于10的数的各个位上的数字相加，若结果还大于0的话，则继续相加，直到数字小于10为止。那么根据这个性质，我们可以写出一个解法如下：



解法一：

```
class Solution {
public:
    int addDigits(int num) {
        while (num / 10 > 0) {
            int sum = 0;
            while (num > 0) {
                sum += num % 10;
                num /= 10;
            }
            num = sum;
        }
        return num;
    }
};
```



但是这个解法在出题人看来又trivial又naive，需要想点高逼格的解法，一行搞定碉堡了，那么我们先来观察1到20的所有的树根：

1    1
2    2
3    3
4    4
5    5
6    6
7    7
8    8    
9    9    
10    1
11    2
12    3    
13    4
14    5
15    6
16    7
17    8
18    9
19    1
20    2



根据上面的列举，我们可以得出规律，每9个一循环，所有大于9的数的树根都是对9取余，那么对于等于9的数对9取余就是0了，为了得到其本身，而且同样也要对大于9的数适用，我们就用(n-1)%9+1这个表达式来包括所有的情况。还有个特殊情况需要考虑一下，当num为0的时候，那么就会出现 -1 % 9 的情况，这个其实挺烦人的，因为C++和Java会给出商0余-1的结果，而Python会给出商-1余8的结果，博主搜了一下，好像是说当有一个负数存在的时候，C++/Java会尽可能让商大一些，而Python会让商小一些，所以结果不统一就神烦，那么只好做个额外判断了，特殊处理一下0的情况就OK了，所以解法如下：



解法二：

```
class Solution {
public:
    int addDigits(int num) {
        return (num == 0) ? 0 : (num - 1) % 9 + 1;
    }
};
```



类似题目：

[Happy Number](http://www.cnblogs.com/grandyang/p/4447233.html)



参考资料：

[https://leetcode.com/problems/add-digits/](https://leetcode.com/problems/add-digits/)

[https://leetcode.com/problems/add-digits/discuss/68580/Accepted-C%2B%2B-O(1)-time-O(1)-space-1-Line-Solution-with-Detail-Explanations](https://leetcode.com/problems/add-digits/discuss/68580/Accepted-C%2B%2B-O(1)-time-O(1)-space-1-Line-Solution-with-Detail-Explanations)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












