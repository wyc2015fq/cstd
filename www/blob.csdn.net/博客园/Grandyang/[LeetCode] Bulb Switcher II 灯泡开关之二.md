# [LeetCode] Bulb Switcher II 灯泡开关之二 - Grandyang - 博客园







# [[LeetCode] Bulb Switcher II 灯泡开关之二](https://www.cnblogs.com/grandyang/p/7595595.html)







There is a room with `n` lights which are turned on initially and 4 buttons on the wall. After performing exactly `m` unknown operations towards buttons, you need to return how many different kinds of status of the `n` lights could be.

Suppose `n` lights are labeled as number [1, 2, 3 ..., n], function of these 4 buttons are given below:
- Flip all the lights.
- Flip lights with even numbers.
- Flip lights with odd numbers.
- Flip lights with (3k + 1) numbers, k = 0, 1, 2, ...



Example 1:
Input: n = 1, m = 1.
Output: 2
Explanation: Status can be: [on], [off]



Example 2:
Input: n = 2, m = 1.
Output: 3
Explanation: Status can be: [on, off], [off, on], [off, off]



Example 3:
Input: n = 3, m = 1.
Output: 4
Explanation: Status can be: [off, on, off], [on, off, on], [off, off, off], [off, on, on].



Note: `n` and `m` both fit in range [0, 1000].



这道题是之前那道[Bulb Switcher](http://www.cnblogs.com/grandyang/p/5100098.html)的拓展，但是关灯的方式改变了。现在有四种关灯方法，全关，关偶数灯，关奇数灯，关3k+1的灯。现在给我们n盏灯，允许m步操作，问我们总共能组成多少种不同的状态。博主开始想，题目没有让列出所有的情况，而只是让返回总个数。那么博主觉得应该不能用递归的暴力破解来做，一般都是用DP来做啊。可是想了半天也没想出递推公式，只得作罢。只好去参考大神们的做法，发现这道题的结果并不会是一个超大数，最多情况只有8种。转念一想，也是，如果结果是一个超大数，一般都会对一个超大数10e7来取余，而这道题并没有，所以是一个很大的hint，只不过博主没有get到。博主应该多列几种情况的，说不定就能找出规律。下面先来看一种暴力解法，首先我们先做一个小小的优化，我们来分析四种情况：

第一种情况：**1**，**2**，**3**，**4**，**5**，**6**，**7**，**8**，**9**，**10**，**11**，**12**，**13**，**14**，**15**，...

第二种情况：1，**2**，3，**4**，5，**6**，7，**8**，9，**10**，11，**12**，13，**14**，15，...

第三种情况：**1**，2，**3**，4，**5**，6，**7**，8，**9**，10，**11**，12，**13**，14，**15**，...

第四种情况：**1**，2，3，**4**，5，6，**7**，8，9，**10**，11，12，**13**，14，15，...

通过观察上面的数组，我们可以发现以6个为1组，都是重复的pattern，那么实际上我们可以把重复的pattern去掉而且并不会影响结果。如果n大于6，我们则对其取余再加上6，新的n跟使用原来的n会得到同样的结果，但这样降低了我们的计算量。

下面我们先来生成n个1，这里1表示灯亮，0表示灯灭，然后我们需要一个set来记录已经存在的状态，用一个queue来辅助我们的BFS运算。我们需要循环m次，因为要操作m次，每次开始循环之前，先统计出此时queue中数字的个数len，然后进行len次循环，这就像二叉树中的层序遍历，必须上一层的结点全部遍历完了才能进入下一层，当然，在每一层开始前，我们都需要情况集合s，这样每个操作之间才不会互相干扰。然后在每层的数字循环中，我们取出队首状态，然后分别调用四种方法，突然感觉，这很像迷宫遍历问题，上下左右四个方向，周围四个状态算出来，我们将不再集合set中的状态加入queue和集合set。当m次操作遍历完成后，队列queue中状态的个数即为所求，参见代码如下：



解法一：

```
class Solution {
public:
    int flipLights(int n, int m) {
        n == (n <= 6) ? n : (n % 6 + 6);
        int start = (1 << n) - 1;
        unordered_set<int> s;
        queue<int> q{{start}};
        for (int i = 0; i < m; ++i) {
            int len = q.size();
            s.clear();
            for (int k = 0; k < len; ++k) {
                int t = q.front(); q.pop();
                vector<int> next{flipAll(t, n), flipEven(t, n), flipOdd(t, n), flip3k1(t, n)};
                for (int num : next) {
                    if (s.count(num)) continue;
                    q.push(num);
                    s.insert(num);
                }
            }
        }
        return q.size();
    }
    
    int flipAll(int t, int n) {
        int x = (1 << n) - 1;
        return t ^ x;
    }
    
    int flipEven(int t, int n) {
        for (int i = 0; i < n; i += 2) {
            t ^= (1 << i);
        }
        return t;
    }
    
    int flipOdd(int t, int n) {
        for (int i = 1; i < n; i += 2) {
            t ^= (1 << i);
        }
        return t;
    }
    
    int flip3k1(int t, int n) {
        for (int i = 0; i < n; i += 3) {
            t ^= (1 << i);
        }
        return t;
    }
};
```



上面那个方法虽然正确，但是有些复杂了，由于这道题最多只有8中情况，所以很适合分情况来讨论：

- 当m和n其中有任意一个数是0时，返回1

- 当n = 1时

只有两种情况，0和1

- 当n = 2时，

这时候要看m的次数，如果m = 1，那么有三种状态 00，01，10

当m > 1时，那么有四种状态，00，01，10，11

- 当m = 1时，

此时n至少为3，那么我们有四种状态，000，010，101，011

- 当m = 2时，

此时n至少为3，我们有七种状态：111，101，010，100，000，001，110

- 当m > 2时，

此时n至少为3，我们有八种状态：111，101，010，100，000，001，110，011



解法二：

```
class Solution {
public:
    int flipLights(int n, int m) {
        if (n == 0 || m == 0) return 1;
        if (n == 1) return 2;
        if (n == 2) return m == 1 ? 3 : 4;
        if (m == 1) return 4;
        return m == 2 ? 7 : 8;
    }
};
```



下面这种简洁到变态的方法是史蒂芬大神观察规律得到的，他自己也在[帖子中](https://discuss.leetcode.com/topic/102395/2-short-lines-simple-formula)说不清为啥这样可以，但是就是叼，贴上来纯属娱乐吧～



解法三：

```
class Solution {
public:
    int flipLights(int n, int m) {
        n = min(n, 3);
        return min(1 << n, 1 + m * n);
    }
};
```



类似题目：

[Bulb Switcher](http://www.cnblogs.com/grandyang/p/5100098.html)



参考资料：

[https://discuss.leetcode.com/topic/102022/c-concise-code-o-1](https://discuss.leetcode.com/topic/102022/c-concise-code-o-1)

[https://discuss.leetcode.com/topic/102395/2-short-lines-simple-formula](https://discuss.leetcode.com/topic/102395/2-short-lines-simple-formula)

[https://discuss.leetcode.com/topic/102227/short-and-clean-java-o-1-solution](https://discuss.leetcode.com/topic/102227/short-and-clean-java-o-1-solution)

[https://discuss.leetcode.com/topic/102107/easy-to-understand-java-bfs-solution-o-m](https://discuss.leetcode.com/topic/102107/easy-to-understand-java-bfs-solution-o-m)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












