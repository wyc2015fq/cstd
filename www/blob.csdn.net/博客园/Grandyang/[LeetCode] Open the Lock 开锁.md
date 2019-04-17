# [LeetCode] Open the Lock 开锁 - Grandyang - 博客园







# [[LeetCode] Open the Lock 开锁](https://www.cnblogs.com/grandyang/p/8449211.html)








You have a lock in front of you with 4 circular wheels. Each wheel has 10 slots: `'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'`. The wheels can rotate freely and wrap around: for example we can turn `'9'` to be `'0'`, or `'0'` to be `'9'`. Each move consists of turning one wheel one slot.

The lock initially starts at `'0000'`, a string representing the state of the 4 wheels.

You are given a list of `deadends` dead ends, meaning if the lock displays any of these codes, the wheels of the lock will stop turning and you will be unable to open it.

Given a `target` representing the value of the wheels that will unlock the lock, return the minimum total number of turns required to open the lock, or -1 if it is impossible.

Example 1:
Input: deadends = ["0201","0101","0102","1212","2002"], target = "0202"
Output: 6
Explanation:
A sequence of valid moves would be "0000" -> "1000" -> "1100" -> "1200" -> "1201" -> "1202" -> "0202".
Note that a sequence like "0000" -> "0001" -> "0002" -> "0102" -> "0202" would be invalid,
because the wheels of the lock become stuck after the display becomes the dead end "0102".



Example 2:
Input: deadends = ["8888"], target = "0009"
Output: 1
Explanation:
We can turn the last wheel in reverse to move from "0000" -> "0009".



Example 3:
Input: deadends = ["8887","8889","8878","8898","8788","8988","7888","9888"], target = "8888"
Output: -1
Explanation:
We can't reach the target without getting stuck.



Example 4:
Input: deadends = ["0000"], target = "8888"
Output: -1



Note:
- The length of `deadends` will be in the range `[1, 500]`.
- `target` will not be in the list `deadends`.
- Every string in `deadends` and the string `target` will be a string of 4 digits from the 10,000 possibilities `'0000'` to `'9999'`.




这道题说有一种可滑动的四位数的锁，貌似行李箱上比较常见这种锁。给了我们一个目标值，还有一些死锁的情况，就是说如果到达这些死锁的位置，就不能再动了，相当于迷宫中的障碍物。然后问我们最少多少步可以从初始的0000位置滑动到给定的target位置。如果各位足够老辣的话，应该能发现其实本质就是个迷宫遍历的问题，只不过相邻位置不再是上下左右四个位置，而是四位数字每个都加一减一，总共有八个相邻的位置。遍历迷宫问题中求最短路径要用BFS来做，那么这道题也就是用BFS来解啦，和经典BFS遍历迷宫解法唯一不同的就是找下一个位置的地方，这里我们要遍历四位数字的每一位，然后分别加1减1，我们用j从-1遍历到1，遇到0跳过，也就是实现了加1减1的过程。然后我们要计算要更新位上的数字，为了处理9加1变0，和0减1变9的情况，我们统一给该位数字加上个10，然后再加或减1，最后再对10取余即可，注意字符和整型数之间通过加或减'0'来转换。我们用结果res来记录BFS遍历的层数，如果此时新生成的字符串等于target了，直接返回结果res，否则我们看如果该字符串不在死锁集合里，且之前没有遍历过，那么加入队列queue中，之后将该字符串加入visited集合中即可。注意这里在while循环中，由于要一层一层的往外扩展，一般的做法是会用一个变量len来记录当前的q.size()，博主为了简洁，使用了一个trick，就是从q.size()往0遍历，千万不能反回来，因为在计算的过程中q的大小会变化，如果让k < q.size() 为终止条件，绝b会出错，而我们初始化为q.size()就没事，参见代码如下：



解法一：

```
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> deadlock(deadends.begin(), deadends.end());
        if (deadlock.count("0000")) return -1;
        int res = 0;
        unordered_set<string> visited{{"0000"}};
        queue<string> q{{"0000"}};
        while (!q.empty()) {
            ++res;
            for (int k = q.size(); k > 0; --k) {
                auto t = q.front(); q.pop();
                for (int i = 0; i < t.size(); ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (j == 0) continue;
                        string str = t;
                        str[i] = ((t[i] - '0') + 10 + j) % 10 + '0';
                        if (str == target) return res;
                        if (!visited.count(str) && !deadlock.count(str)) q.push(str);        
                        visited.insert(str);
                    }
                }
            }
        }
        return -1;
    }
};
```



下面这种方法也是用的BFS遍历，不同之处在于生成新字符串的方法，这里我们采用拼接法来生成新字符串，而不是像上面那样使用置换字符串的方法。我们对于加一和减一分别进行拼接，注意处理9加1变0，和0减1变9的情况。然后剩下的部分就和经典的BFS遍历写法没有什么太大的区别了，参见代码如下：



解法二：

```
class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        unordered_set<string> deadlock(deadends.begin(), deadends.end());
        if (deadlock.count("0000")) return -1;
        int res = 0;
        unordered_set<string> visited{{"0000"}};
        queue<string> q{{"0000"}};
        while (!q.empty()) {
            ++res;
            for (int k = q.size(); k > 0; --k) {
                auto t = q.front(); q.pop();
                for (int i = 0; i < t.size(); ++i) {
                    char c = t[i];
                    string str1 = t.substr(0, i) + to_string(c == '9' ? 0 : c - '0' + 1) + t.substr(i + 1);
                    string str2 = t.substr(0, i) + to_string(c == '0' ? 9 : c - '0' - 1) + t.substr(i + 1);
                    if (str1 == target || str2 == target) return res;
                    if (!visited.count(str1) && !deadlock.count(str1)) q.push(str1);
                    if (!visited.count(str2) && !deadlock.count(str2)) q.push(str2);
                    visited.insert(str1);
                    visited.insert(str2);
                }
            }
        }
        return -1;
    }
};
```



参考资料：

[https://leetcode.com/problems/open-the-lock](https://leetcode.com/problems/open-the-lock)

[https://leetcode.com/problems/open-the-lock/discuss/110230/BFS-solution-C++](https://leetcode.com/problems/open-the-lock/discuss/110230/BFS-solution-C++)

[https://leetcode.com/problems/open-the-lock/discuss/110237/Regular-java-BFS-solution-and-2-end-BFS-solution-with-improvement](https://leetcode.com/problems/open-the-lock/discuss/110237/Regular-java-BFS-solution-and-2-end-BFS-solution-with-improvement)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












