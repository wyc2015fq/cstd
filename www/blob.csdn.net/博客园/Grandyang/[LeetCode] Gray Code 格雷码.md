# [LeetCode] Gray Code 格雷码 - Grandyang - 博客园







# [[LeetCode] Gray Code 格雷码](https://www.cnblogs.com/grandyang/p/4315649.html)







The gray code is a binary numeral system where two successive values differ in only one bit.

Given a non-negative integer *n* representing the total number of bits in the code, print the sequence of gray code. A gray code sequence must begin with 0.

For example, given *n* = 2, return `[0,1,3,2]`. Its gray code sequence is:
00 - 0
01 - 1
11 - 3
10 - 2

**Note:**

For a given *n*, a gray code sequence is not uniquely defined.


For example, `[0,2,3,1]` is also a valid gray code sequence according to the above definition.

For now, the judge is able to judge based on one instance of gray code sequence. Sorry about that.



这道题是关于[格雷码](http://zh.wikipedia.org/wiki/%E6%A0%BC%E9%9B%B7%E7%A0%81)的，猛地一看感觉完全没接触过[格雷码](http://zh.wikipedia.org/wiki/%E6%A0%BC%E9%9B%B7%E7%A0%81)，但是看了维基百科后，隐约的感觉原来好像哪门可提到过，哎全还给老师了。这道题如果不了解格雷码，还真不太好做，幸亏脑补了维基百科，上面说格雷码是一种循环二进制单位距离码，主要特点是两个相邻数的代码只有一位二进制数不同的编码，格雷码的处理主要是位操作 Bit Operation，LeetCode中关于位操作的题也挺常见，比如 [Repeated DNA Sequences 求重复的DNA序列](http://www.cnblogs.com/grandyang/p/4284205.html)，[Single Number 单独的数字](http://www.cnblogs.com/grandyang/p/4130577.html), 和 [Single Number II 单独的数字之二](http://www.cnblogs.com/grandyang/p/4263927.html) 等等。三位的格雷码和二进制数如下：



```
Int    Grey Code    Binary
 0  　　  000        000
 1  　　  001        001
 2   　 　011        010
 3   　 　010        011
 4   　 　110        100
 5   　 　111        101
 6   　 　101        110
 7   　　 100        111
```



其实这道题还有多种解法。首先来看一种最简单的，是用到格雷码和二进制数之间的相互转化，可参见我之前的博客 [Convertion of grey code and binary 格雷码和二进制数之间的转换](http://www.cnblogs.com/grandyang/p/4315607.html) ，明白了转换方法后，这道题完全没有难度，代码如下：



解法一：

```
// Binary to grey code
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res;
        for (int i = 0; i < pow(2,n); ++i) {
            res.push_back((i >> 1) ^ i);
        }
        return res;
    }
};
```



然后我们来看看其他的解法，参考维基百科上关于格雷码的性质，有一条是说[镜面排列](http://zh.wikipedia.org/wiki/%E6%A0%BC%E9%9B%B7%E7%A0%81)的，n位元的格雷码可以从n-1位元的格雷码以上下镜射后加上新位元的方式快速的得到，如下图所示一般。

![](http://upload.wikimedia.org/wikipedia/commons/thumb/c/c1/Binary-reflected_Gray_code_construction.svg/250px-Binary-reflected_Gray_code_construction.svg.png)

有了这条性质，我们很容易的写出代码如下：



解法二：

```
// Mirror arrangement
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res{0};
        for (int i = 0; i < n; ++i) {
            int size = res.size();
            for (int j = size - 1; j >= 0; --j) {
                res.push_back(res[j] | (1 << i));
            }
        }
        return res;
    }
};
```



维基百科上还有一条格雷码的性质是[直接排列](http://zh.wikipedia.org/wiki/%E6%A0%BC%E9%9B%B7%E7%A0%81)，以二进制为0值的格雷码为第零项，第一项改变最右边的位元，第二项改变右起第一个为1的位元的左边位元，第三、四项方法同第一、二项，如此反复，即可排列出n个位元的格雷码。根据这条性质也可以写出代码，不过相比前面的略微复杂，代码如下：

0 0 0
0 0 1
0 1 1
0 1 0
1 1 0
1 1 1
1 0 1
1 0 0



解法三：

```
// Direct arrangement 
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res{0};
        int len = pow(2, n);
        for (int i = 1; i < len; ++i) {
            int pre = res.back();
            if (i % 2 == 1) {
                pre = (pre & (len - 2)) | ((~pre) & 1);
            } else {
                int cnt = 1, t = pre;
                while ((t & 1) != 1) {
                    ++cnt;
                    t >>= 1;
                }
                if ((pre & (1 << cnt)) == 0) pre |= (1 << cnt);
                else pre &= ~(1 << cnt);
            }
            res.push_back(pre);
        }
        return res;
    }
};
```



上面三种解法都需要事先了解格雷码及其性质，假如我们之前并没有接触过格雷码，那么我们其实也可以用比较笨的方法来找出结果，比如下面这种方法用到了一个set来保存已经产生的结果，我们从0开始，遍历其二进制每一位，对其取反，然后看其是否在set中出现过，如果没有，我们将其加入set和结果res中，然后再对这个数的每一位进行遍历，以此类推就可以找出所有的格雷码了，参见代码如下：



解法四：

```
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res;
        unordered_set<int> s;
        helper(n, s, 0, res);
        return res;
    }
    void helper(int n, unordered_set<int>& s, int out, vector<int>& res) {
        if (!s.count(out)) {
            s.insert(out);
            res.push_back(out);
        }
        for (int i = 0; i < n; ++i) {
            int t = out;
            if ((t & (1 << i)) == 0) t |= (1 << i);
            else t &= ~(1 << i);
            if (s.count(t)) continue;
            helper(n, s, t, res);
            break;
        }
    }
};
```



既然递归方法可以实现，那么就有对应的迭代的写法，当然需要用stack来辅助，参见代码如下：



解法五：

```
class Solution {
public:
    vector<int> grayCode(int n) {
        vector<int> res{0};
        unordered_set<int> s;
        stack<int> st;
        st.push(0);
        s.insert(0);
        while (!st.empty()) {
            int t = st.top(); st.pop();
            for (int i = 0; i < n; ++i) {
                int k = t;
                if ((k & (1 << i)) == 0) k |= (1 << i);
                else k &= ~(1 << i);
                if (s.count(k)) continue;
                s.insert(k);
                st.push(k);
                res.push_back(k);
                break;
            }
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/8557/an-accepted-three-line-solution-in-java](https://discuss.leetcode.com/topic/8557/an-accepted-three-line-solution-in-java)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












