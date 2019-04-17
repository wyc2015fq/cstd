# [LeetCode] 1-bit and 2-bit Characters 一位和两位字符 - Grandyang - 博客园







# [[LeetCode] 1-bit and 2-bit Characters 一位和两位字符](https://www.cnblogs.com/grandyang/p/7790029.html)







We have two special characters. The first character can be represented by one bit `0`. The second character can be represented by two bits (`10` or `11`).

Now given a string represented by several bits. Return whether the last character must be a one-bit character or not. The given string will always end with a zero.

Example 1:
Input: 
bits = [1, 0, 0]
Output: True
Explanation: 
The only way to decode it is two-bit character and one-bit character. So the last character is one-bit character.



Example 2:
Input: 
bits = [1, 1, 1, 0]
Output: False
Explanation: 
The only way to decode it is two-bit character and two-bit character. So the last character is NOT one-bit character.



Note:
- `1 <= len(bits) <= 1000`.
- `bits[i]` is always `0` or `1`.



这道题说有两种特殊的字符，一种是两位字符，只能是二进制的11和10，另一种是单个位字符，只能是二进制的0。现在给了我们一个只包含0和1的数组，问我们能否将其正确的分割，使得最后一个字符是个单个位字符。这道题可以使用贪婪算法来做，因为两种字符互不干扰，只要我们遍历到了数字1，那么其必定是两位字符，所以后面一位也得跟着，而遍历到了数字0，那么就必定是单个位字符。所以我们可以用一个变量i来记录当前遍历到的位置，如果遇到了0，那么i自增1，如果遇到了1，那么i自增2，我们循环的条件是i < n-1，即留出最后一位，所以当循环退出后，当i正好停留在n-1上，说明最后一位是单独分割开的，因为题目中限定了最后一位一定是0，所以没必要再判断了，参见代码如下：



解法一：

```
class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        int n = bits.size(), i = 0;
        while (i < n - 1) {
            if (bits[i] == 0) ++i;
            else i+= 2;
        }
        return i == n - 1;
    }
};
```



下面这种解法写的更加简洁了，直接用一行代替了if..else..语句，相当巧妙，当bits[i]为0时，i还是相当于自增了1，当bits[i]为1时，i相当于自增了2，最后还是在循环跳出后检测i是否为n-1，参见代码如下：



解法二：

```
class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        int n = bits.size(), i = 0;
        while (i < n - 1) {
            i += bits[i] + 1;
        }
        return i == n - 1;
    }
};
```



下面我们来看递归解法，用的是回溯的思想，首先判断如果bits为空了，直接返回false，因为题目初始给的bits是非空的，在调用递归函数中为空了说明最后一位跟倒数第二位组成了个两位字符，所以不合题意返回false。再判断如果bits大小为1了，那么返回这个数字是否为0，其实直接返回true也行，因为题目中说了最后一个数字一定是0。然后我们新建一个数组t，如果bits的首元素为0，则我们的t赋值为去掉首元素的bits数组；如果bits的首元素是1，则我们的t服之为去掉前两个元素的bits数组，然后返回调用递归函数的结果即可，参见代码如下：



解法三：

```
class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        if (bits.empty()) return false;
        if (bits.size() == 1) return bits[0] == 0;
        vector<int> t;
        if (bits[0] == 0) {
            t = vector<int>(bits.begin() + 1, bits.end());
        } else if (bits[0] == 1) {
            t = vector<int>(bits.begin() + 2, bits.end());
        }
        return isOneBitCharacter(t);
    }
};
```



下面这种解法也是用的递归，递归函数用的不是原函数，这样可以只用位置变量idx来遍历，而不用新建数组t，初始时idx传入0，在递归函数中，如果idx为n了，相当于上面解法中的bits数组为空了情况，返回false；如果idx为n-1，返回true；如果bits[idx]为0，则返回调用递归函数的结果，此时idx加上1；如果bits[idx]为1，则返回调用递归函数的结果，此时idx加上2，参见代码如下：



解法四：

```
class Solution {
public:
    bool isOneBitCharacter(vector<int>& bits) {
        return helper(bits, 0);
    }
    bool helper(vector<int>& bits, int idx) {
        int n = bits.size();
        if (idx == n) return false;
        if (idx == n - 1) return bits[idx] == 0;
        if (bits[idx] == 0) return helper(bits, idx + 1);
        return helper(bits, idx + 2);
    }
};
```



类似题目：

[Gray Code](http://www.cnblogs.com/grandyang/p/4315649.html)



参考资料：

[https://discuss.leetcode.com/topic/108743/java-solution-1-or-2](https://discuss.leetcode.com/topic/108743/java-solution-1-or-2)

[https://discuss.leetcode.com/topic/108766/c-both-iterative-and-recursive-solutions](https://discuss.leetcode.com/topic/108766/c-both-iterative-and-recursive-solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












