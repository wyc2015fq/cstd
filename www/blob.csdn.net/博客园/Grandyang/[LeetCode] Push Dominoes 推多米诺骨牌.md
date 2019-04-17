# [LeetCode] Push Dominoes 推多米诺骨牌 - Grandyang - 博客园







# [[LeetCode] Push Dominoes 推多米诺骨牌](https://www.cnblogs.com/grandyang/p/10393508.html)







There are`N` dominoes in a line, and we place each domino vertically upright.

In the beginning, we simultaneously push some of the dominoes either to the left or to the right.

![](https://s3-lc-upload.s3.amazonaws.com/uploads/2018/05/18/domino.png)

After each second, each domino that is falling to the left pushes the adjacent domino on the left.

Similarly, the dominoes falling to the right push their adjacent dominoes standing on the right.

When a vertical domino has dominoes falling on it from both sides, it stays still due to the balance of the forces.

For the purposes of this question, we will consider that a falling domino expends no additional force to a falling or already fallen domino.

Given a string "S" representing the initial state. `S[i] = 'L'`, if the i-th domino has been pushed to the left; `S[i] = 'R'`, if the i-th domino has been pushed to the right; `S[i] = '.'`, if the `i`-th domino has not been pushed.

Return a string representing the final state. 

Example 1:
Input: ".L.R...LR..L.."
Output: "LL.RR.LLRRLL.."

Example 2:
Input: "RR.L"
Output: "RR.L"
Explanation: The first domino expends no additional force on the second domino.

Note:
- `0 <= N <= 10^5`
- String `dominoes` contains only `'L`', `'R'` and `'.'`



这道题给我们摆好了一个多米诺骨牌阵列，但是与一般的玩法不同的是，这里没有从一头开始推，而是在很多不同的位置分别往两个方向推，结果是骨牌各自向不同的方向倒下了，而且有的骨牌由于左右两边受力均等，依然屹立不倒，这样的话骨牌就很难受了，能不能让哥安心的倒下去？！生而为骨牌，总是要倒下去啊，就像漫天飞舞的樱花，秒速五厘米的落下，回到最终归宿泥土里。喂，不要给骨牌强行加戏好么！～ 某个位置的骨牌会不会倒，并且朝哪个方向倒，是由左右两边受到的力的大小决定的，那么可以分为下列四种情况：

1）R....R  ->  RRRRRR

这是当两个向右推的操作连在一起时，那么中间的骨牌毫无悬念的都要向右边倒去。

2）L....L  ->  LLLLLL

同理，

当两个向左推的操作连在一起时，那么中间的骨牌毫无悬念的都要向左边倒去。

3）L....R  ->  L....R

当左边界的骨牌向左推，右边界的骨牌向右推，那么中间的骨牌不会收到力，所以依然保持坚挺。

4）R....L  -> RRRLLL   or   R.....L  ->  RRR.LLL

当左边界的骨牌向右推，右边界的骨牌向左推时，就要看中间的骨牌个数了，若是偶数，那么对半分，若是奇数，那么最中间的骨牌保持站立，其余的对半分。 

由于上述四种情况包含了所有的情况，所以我们的目标就是在字符串中找出中间是‘点’的小区间，为了便于我们一次遍历就处理完，我们在dominoes字符串左边加个L，右边加个R，这并不会影响骨牌倒下的情况。我们使用双指针来遍历，其中i初始化为0，j初始化为1，当j指向‘点’时，我们就跳过，目标是i指向小区间的左边界，j指向右边界，然后用 j-i-1 算出中间‘点’的个数，为0表示中间没有点。若此时 i>0，则将左边界加入结果res中。若左右边界相同，那么中间的点都填成左边界，这是上述的情况一和二；若左边界是L，右边界是R，则是上述的情况三，中间还是保持点不变；若左边界是R，右边界是L，则是情况四，那么先加 mid/2 个R，再加 mid%2 个点，最后加 mid/2 个L即可。然后i更新为j，继续循环即可，参见代码如下：



解法一：

```
class Solution {
public:
    string pushDominoes(string dominoes) {
        string res = "";
        dominoes = "L" + dominoes + "R";
        for (int i = 0, j = 1; j < dominoes.size(); ++j) {
            if (dominoes[j] == '.') continue;
            int mid = j - i - 1;
            if (i > 0) res += dominoes[i];
            if (dominoes[i] == dominoes[j]) res += string(mid, dominoes[i]);
            else if (dominoes[i] == 'L' && dominoes[j] == 'R') res += string(mid, '.');
            else res += string(mid / 2, 'R') + string(mid % 2, '.') + string(mid / 2, 'L');
            i = j;
        }
        return res;
    }
};
```



下面这种解法遍历了两次字符串，第一次遍历是先把R后面的点全变成R，同时累加一个cnt数组，其中cnt[i]表示在dominoes数组中i位置时R连续出现的个数，那么拿题目中的例子1来说，第一次遍历之后，原dominoes数组，修改后的dominoes数组，以及cnt数组分别为：

```
.L.R...LR..L..
.L.RRRRLRRRL..
00001230012000
```

我们可以发现cnt数字记录的是R连续出现的个数，第一次遍历只模拟了所有往右推倒的情况，很明显不是最终答案，因为还需要往左推，那么就要把某些点变成L，已经把某些R变成点或者L，这时我们的cnt数组就非常重要，因为它相当于记录了往右推的force的大小。第二次遍历是从右往左，我们找所有L前面的位置，若其为点，则直接变为L。若其为R，那么也有可能变L，此时就要计算往左的force，通过 cnt[i+1] + 1 获得，然后跟往右的force比较，若此位置往右的force大，说明当前骨牌应该往左倒，更新此时cnt[i]为往左的force。若此时左右force相等了，说明当前骨牌不会向任意一遍倒，改为点即可，最终修改后的dominoes数组和cnt数组分别为：

```
LL.RR.LLRRLL..
10001210011000
```



解法二：

```
class Solution {
public:
    string pushDominoes(string dominoes) {
        int n = dominoes.size();
        vector<int> cnt(n);
        for (int i = 1; i < n; ++i) {
            if (dominoes[i - 1] == 'R' && dominoes[i] == '.') {
                dominoes[i] = 'R';
                cnt[i] = cnt[i - 1] + 1;
            }
        }
        for (int i = n - 2, cur = 0; i >= 0; --i) {
            if (dominoes[i + 1] != 'L') continue;
            cur = cnt[i + 1] + 1;
            if (dominoes[i] == '.' || cnt[i] > cur) {
                dominoes[i] = 'L';
                cnt[i] = cur;
            } else if (dominoes[i] == 'R' && cnt[i] == cur) {
                dominoes[i] = '.';
            }
        }
        return dominoes;
    }
};
```



类似题目：

[Shortest Distance to a Character](https://www.cnblogs.com/grandyang/p/10113218.html)



参考资料：

[https://leetcode.com/problems/push-dominoes/](https://leetcode.com/problems/push-dominoes/)

[https://leetcode.com/problems/push-dominoes/discuss/132332/C%2B%2BJavaPython-Two-Pointers](https://leetcode.com/problems/push-dominoes/discuss/132332/C%2B%2BJavaPython-Two-Pointers)

[https://leetcode.com/problems/push-dominoes/discuss/132932/C%2B%2B-2-pass-scan-O(2N)-13ms](https://leetcode.com/problems/push-dominoes/discuss/132932/C%2B%2B-2-pass-scan-O(2N)-13ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












