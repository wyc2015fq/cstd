# leetcode_650. 2 Keys Keyboard - wjheha的博客 - CSDN博客
2018年01月08日 12:31:16[wjheha](https://me.csdn.net/wjheha)阅读数：66
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Initially on a notepad only one character ‘A’ is present. You can perform two operations on this notepad for each step:
Copy All: You can copy all the characters present on the notepad (partial copy is not allowed). 
Paste: You can paste the characters which are copied last time. 
Given a number n. You have to get exactly n ‘A’ on the notepad by performing the minimum number of steps permitted. Output the minimum number of steps to get n ‘A’.
Example 1: 
Input: 3 
Output: 3 
Explanation: 
Intitally, we have one character ‘A’. 
In step 1, we use Copy All operation. 
In step 2, we use Paste operation to get ‘AA’. 
In step 3, we use Paste operation to get ‘AAA’. 
Note: 
The n will be in the range [1, 1000].
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/2-keys-keyboard/description/](https://leetcode.com/problems/2-keys-keyboard/description/)
### 思路：
这道题目的意思是给你一个字母”A”，然后你使用***复制***和***粘贴***这两种操作，来生成指定数量的”A………”，问所需要的最少的操作数是多少，这里复制和粘贴都算一次操作。和平时使用习惯一样，复制一次，可以反复粘贴多次。 
***注意***：题目中，规定了我们的复制操作，必须是把已有的A全部复制，这是至关重要的一点，把问题的难度直接降低了一半，因为要求复制全部的A，那么在生成最终奇数（n为奇数）个A时，只能通过复制一次A，然后粘贴n-1次来完成。针对偶数个A（n为偶数），把n进行因式分解来完成。
- 输入的n是1，最少需要操作0次
- 输入的n是2，最少需要操作2次（复制1次A，粘贴1次A）
- 输入的n是3，最少需要操作3次（复制1次A，粘贴2次A）
- 输入的n是3，最少需要操作4次（复制1次A，粘贴一次A，复制一次AA，粘贴一次AA）
- 。。。。。。。。。。。。。
- 。。。。。。。。。。。。。
- 。。。。。。。。。。。。。 
输入的n是12，最少需要多少次呢？找出12的所有因子：如果我们已经得到了6个A，那么复制一次AAAAAA,粘贴一次AAAAAA，就可以完成。如果我们已经得到了4个A，那么复制一次AAAA,粘贴两次AAAA，就可以完成。如果我们已经得到了3个A，那么复制一次AAA,粘贴三次AAA，就可以完成。如果我们已经得到了2个A，那么复制一次AA,粘贴五次AA，就可以完成。比较这些方案，选取最少的次数即可。参考下面几个链接： 
[https://www.cnblogs.com/grandyang/p/7439616.html](https://www.cnblogs.com/grandyang/p/7439616.html)
[https://www.cnblogs.com/syjbupt/p/7440824.html](https://www.cnblogs.com/syjbupt/p/7440824.html)
代码实现：
```
package dailyAAlgorithm;
public class TwoKeysKeyboard_650 {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        int n = 100;
        int res1 = twoKeysKeyboaed1(n);
        System.out.println(res1);
    }
    public static int twoKeysKeyboaed1(int n) {
        if(n == 1) return 0;
        int res = n;
        for(int i = n-1; i > 1; i--) {
            if(n % i == 0) {
                res = Math.min(res, twoKeysKeyboaed1(n / i) + i);
            }
        }
        return res;
    }
}
```
