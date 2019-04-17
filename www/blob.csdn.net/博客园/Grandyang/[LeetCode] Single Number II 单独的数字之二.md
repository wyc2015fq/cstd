# [LeetCode] Single Number II 单独的数字之二 - Grandyang - 博客园







# [[LeetCode] Single Number II 单独的数字之二](https://www.cnblogs.com/grandyang/p/4263927.html)







Given an array of integers, every element appears *three* times except for one. Find that single one.

Note:
Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?



这道题是之前那道 [Single Number 单独的数字](http://www.cnblogs.com/Given%20an%20array%20of%20integers,%20every%20element%20appears%20three%20times%20except%20for%20one.%20Find%20that%20single%20one.%20%20Note:%20Your%20algorithm%20should%20have%20a%20linear%20runtime%20complexity.%20Could%20you%20implement%20it%20without%20using%20extra%20memory)的延伸，那道题的解法就比较独特，是利用计算机按位储存数字的特性来做的，这道题就是除了一个单独的数字之外，数组中其他的数字都出现了三次，那么还是要利用位操作 Bit Operation 来解此题。我们可以建立一个32位的数字，来统计每一位上1出现的个数，我们知道如果某一位上为1的话，那么如果该整数出现了三次，对3去余为0，我们把每个数的对应位都加起来对3取余，最终剩下来的那个数就是单独的数字。代码如下：



解法一：

```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for (int i = 0; i < 32; ++i) {
            int sum = 0;
            for (int j = 0; j < nums.size(); ++j) {
                sum += (nums[j] >> i) & 1;
            }
            res |= (sum % 3) << i;
        }
        return res;
    }
};
```



还有一种解法，思路很相似，用3个整数来表示INT的各位的出现次数情况，one表示出现了1次，two表示出现了2次。当出现3次的时候该位清零。最后答案就是one的值。
- `ones`   代表第ith 位只出现一次的掩码变量
- `twos`  代表第ith 位只出现两次次的掩码变量
- `threes`  代表第ith 位只出现三次的掩码变量

假设现在有一个数字1，那么我们更新one的方法就是‘亦或’这个1，则one就变成了1，而two的更新方法是用上一个状态下的one去‘与’上数字1，然后‘或’上这个结果，这样假如之前one是1，那么此时two也会变成1，这make sense，因为说明是当前位遇到两个1了；反之如果之前one是0，那么现在two也就是0。注意更新的顺序是先更新two，再更新one，不理解的话只要带个只有一个数字1的输入数组看一下就不难理解了。然后我们更新three，如果此时one和two都是1了，那么由于我们先更新的two，再更新的one，two为1，说明此时至少有两个数字1了，而此时one为1，说明了此时已经有了三个数字1，这块要仔细想清楚，因为one是要‘亦或’一个1的，值能为1，说明之前one为0，实际情况是，当第二个1来的时候，two先更新为1，此时one再更新为0，下面three就是0了，那么‘与’上three的相反数1不会改变one和two的值；那么当第三个1来的时候，two还是1，此时one就更新为1了，那么three就更新为1了，此时就要清空one和two了，让它们‘与’上three的相反数0即可，最终结果将会保存在one中，参见代码如下：



解法二：

```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int one = 0, two = 0, three = 0;
        for (int i = 0; i < nums.size(); ++i) {
            two |= one & nums[i];
            one ^= nums[i];
            three = one & two;
            one &= ~three;
            two &= ~three;
        }
        return one;
    }
};
```



下面这种解法思路也十分巧妙，根据上面解法的思路，我们把数组中数字的每一位累加起来对3取余，剩下的结果就是那个单独数组该位上的数字，由于我们累加的过程都要对3取余，那么每一位上累加的过程就是0->1->2->0，换成二进制的表示为00->01->10->00，那么我们可以写出对应关系：

00 (+) 1 = 01

01 (+) 1 = 10

10 (+) 1 = 00 ( mod 3)

那么我们用ab来表示开始的状态，对于加1操作后，得到的新状态的ab的算法如下：

b = b xor r & ~a;

a = a xor r & ~b;

我们这里的ab就是上面的三种状态00，01，10的十位和各位，刚开始的时候，a和b都是0，当此时遇到数字1的时候，b更新为1，a更新为0，就是01的状态；再次遇到1的时候，b更新为0，a更新为1，就是10的状态；再次遇到1的时候，b更新为0，a更新为0，就是00的状态，相当于重置了；最后的结果保存在b中。明白了上面的分析过程，就能写出代码如下；



解法三：

```
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int a = 0, b = 0;
        for (int i = 0; i < nums.size(); ++i) {
            b = (b ^ nums[i]) & ~a;
            a = (a ^ nums[i]) & ~b;
        }
        return b;
    }
};
```



类似题目：

[Single Number](http://www.cnblogs.com/grandyang/p/4130577.html)

[Single Number III](http://www.cnblogs.com/grandyang/p/4741122.html)



参考资料：

[https://discuss.leetcode.com/topic/2031/challenge-me-thx/58](https://discuss.leetcode.com/topic/2031/challenge-me-thx/58)

[https://discuss.leetcode.com/topic/18085/java-bit-manipulation-solution](https://discuss.leetcode.com/topic/18085/java-bit-manipulation-solution)

[https://discuss.leetcode.com/topic/43166/java-o-n-easy-to-understand-solution-easily-extended-to-any-times-of-occurance](https://discuss.leetcode.com/topic/43166/java-o-n-easy-to-understand-solution-easily-extended-to-any-times-of-occurance)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












