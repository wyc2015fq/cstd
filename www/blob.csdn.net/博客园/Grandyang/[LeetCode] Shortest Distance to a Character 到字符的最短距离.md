# [LeetCode] Shortest Distance to a Character 到字符的最短距离 - Grandyang - 博客园







# [[LeetCode] Shortest Distance to a Character 到字符的最短距离](https://www.cnblogs.com/grandyang/p/10113218.html)









Given a string `S` and a character `C`, return an array of integers representing the shortest distance from the character `C` in the string.

Example 1:
Input: S = "loveleetcode", C = 'e'
Output: [3, 2, 1, 0, 1, 0, 0, 1, 2, 2, 1, 0]



Note:
- `S` string length is in `[1, 10000].`
- `C` is a single character, and guaranteed to be in string `S`.
- All letters in `S` and `C` are lowercase.






这道题给了我们一个字符串S，和一个字符C，让我们求字符串中每个字符到字符C到最短距离，这里的字符C可能有多个。题目中给的例子中就是有多个e，每个e的位置一定是0，其他位置的值是到其最近的e的距离。最原始粗犷的方法肯定是对于每个非e的字符，左右两个方向来查找e，令博主感到意外的是，这种暴力搜索的方法居然可以通过OJ，太仁慈了啊，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> shortestToChar(string S, char C) {
        int n = S.size();
        vector<int> res(n, n);
        for (int i = 0; i < n; ++i) {
            if (S[i] == C) {res[i] = 0; continue;}
            int j = i + 1;
            while (j < n && S[j] != C) ++j;
            if (j < n) res[i] = j - i;
            j = i - 1;
            while (j >= 0 && S[j] != C) --j;
            if (j >= 0) res[i] = min(res[i], i - j);
        }
        return res;
    }
};
```



一般来说，优化线性搜索的方法就是用二分搜索法，这里我们先把所有为字符C的位置存入数组idx，因为是按顺序遍历的，所以idx数组也是有序的，这为二分搜索创造了条件。然后对于数组中的每一个位置，我们都在idx数组中二分查找不小于该位置的数，这时候要分情况讨论一下，如果找不到这样的数的时候，说明所有字符C的位置都在当前位置的左边，那么我们取idx数组中最后一个数，就是左边最近的一个字符C，求出距离即可。如果返回的是idx数组中的首数字，说明当前的位置是字符C，或者最近的字符C在右边，那么只要用这个首数字减去当前位置就是最近距离了。对于其他情况，左右两边都有字符C，所以我们都要各自计算一下距离，然后取较小的那个即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> shortestToChar(string S, char C) {
        vector<int> res, idx;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == C) idx.push_back(i);
        }
        for (int i = 0; i < S.size(); ++i) {
            auto it = lower_bound(idx.begin(), idx.end(), i);
            if (it == idx.end()) res.push_back(i - *(--it));
            else if (it == idx.begin()) res.push_back(*it - i);
            else {
                int d1 = *it - i, d2 = i - *(--it);
                res.push_back(min(d1, d2));
            }
        }
        return res;
    }
};
```



还有一种类似距离场的解法，与解法一不同的是，这里是对于每个是字符C的位置，然后分别像左右两边扩散，不停是更新距离，这样当所有的字符C的点都扩散完成之后，每个非字符C位置上的数字就是到字符C的最短距离了，参见代码如下：



解法三：

```
class Solution {
public:
    vector<int> shortestToChar(string S, char C) {
        int n = S.size();
        vector<int> res(n, n);
        for (int i = 0; i < n; ++i) {
            if (S[i] != C) continue;
            res[i] = 0;
            for (int j = i + 1; j < n && S[j] != C; ++j) {
                res[j] = min(res[j], j - i);
            }
            for (int j = i - 1; j >= 0 && S[j] != C; --j) {
                res[j] = min(res[j], i - j);
            }
        }
        return res;
    }
};
```



下面这种方法也是建立距离场的思路，不过更加巧妙一些，只需要正反两次遍历就行。首先进行正向遍历，若当前位置是字符C，那么直接赋0，否则看如果不是首位置，那么当前位置的值等于前一个位置的值加1。这里不用和当前的值进行比较，因为这个算出来的值不会大于初始化的值。然后再进行反向遍历，要从倒数第二个值开始往前遍历，用后一个值加1来更新当前位置的值，此时就要和当前值做比较，取较小的那个，参见代码如下：



解法四：

```
class Solution {
public:
    vector<int> shortestToChar(string S, char C) {
        vector<int> res(S.size(), S.size());
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == C) res[i] = 0;
            else if (i > 0) res[i] = res[i - 1] + 1;
        }
        for (int i = (int)S.size() - 2; i >= 0; --i) {
            res[i] = min(res[i], res[i + 1] + 1);
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/shortest-distance-to-a-character/](https://leetcode.com/problems/shortest-distance-to-a-character/)


[https://leetcode.com/problems/shortest-distance-to-a-character/discuss/125788/C%2B%2BJavaPython-2-Pass-with-Explanation](https://leetcode.com/problems/shortest-distance-to-a-character/discuss/125788/C%2B%2BJavaPython-2-Pass-with-Explanation)

[https://leetcode.com/problems/shortest-distance-to-a-character/discuss/156216/2-pass-solution-in-C%2B%2B-(beats-100-submissions).](https://leetcode.com/problems/shortest-distance-to-a-character/discuss/156216/2-pass-solution-in-C%2B%2B-(beats-100-submissions).)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












