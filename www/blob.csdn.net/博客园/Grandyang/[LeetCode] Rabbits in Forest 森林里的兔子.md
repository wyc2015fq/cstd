# [LeetCode] Rabbits in Forest 森林里的兔子 - Grandyang - 博客园







# [[LeetCode] Rabbits in Forest 森林里的兔子](https://www.cnblogs.com/grandyang/p/9043761.html)







In a forest, each rabbit has some color. Some subset of rabbits (possibly all of them) tell you how many other rabbits have the same color as them. Those `answers` are placed in an array.

Return the minimum number of rabbits that could be in the forest.
Examples:
Input: answers = [1, 1, 2]
Output: 5
Explanation:
The two rabbits that answered "1" could both be the same color, say red.
The rabbit than answered "2" can't be red or the answers would be inconsistent.
Say the rabbit that answered "2" was blue.
Then there should be 2 other blue rabbits in the forest that didn't answer into the array.
The smallest possible number of rabbits in the forest is therefore 5: 3 that answered plus 2 that didn't.

Input: answers = [10, 10, 10]
Output: 11

Input: answers = []
Output: 0

Note:
- `answers` will have length at most `1000`.
- Each `answers[i]` will be an integer in the range `[0, 999]`.



这道题说的是大森林中有一堆成了精的兔子，有着不同的颜色，还会回答问题。每个兔子会告诉你森林中还有多少个和其颜色相同的兔子，当然并不是所有的兔子多出现在数组中，所以我们要根据兔子们的回答，来估计森林中最少有多少只能确定的兔子。例子1给的数字是 [1, 1, 2]，第一只兔子说森林里还有另一只兔子跟其颜色一样，第二只兔子也说还有另一只兔子和其颜色一样，那么为了使兔子总数最少，我们可以让前两只兔子是相同的颜色，可以使其回答不会矛盾。第三只兔子说森林里还有两只兔子和其颜色一样，那么这只兔的颜色就不能和前两只兔子的颜色相同了，否则就会跟前面两只兔子的回答矛盾了，因为根据第三只兔子的描述，森林里共有三只这种颜色的兔子，所有总共可以推断出最少有五只兔子。对于例子2，[10, 10, 10] 来说，这三只兔子都说森林里还有10只跟其颜色相同的兔子，那么这三只兔子颜色可以相同，所以总共有11只兔子。

来看一个比较tricky的例子，[0, 0, 1, 1, 1]，前两只兔子都说森林里没有兔子和其颜色相同了，那么这两只兔子就是森林里独一无二的兔子，且颜色并不相同，所以目前已经确定了两只。然后后面三只都说森林里还有一只兔子和其颜色相同，那么这三只兔子就不可能颜色都相同了，但我们可以让两只颜色相同，另外一只颜色不同，那么就是说还有一只兔子并没有在数组中，所以森林中最少有6只兔子。分析完了这几个例子，我们可以发现，如果某个兔子回答的数字是x，那么说明森林里共有x+1个相同颜色的兔子，我们最多允许x+1个兔子同时回答x个，一旦超过了x+1个兔子，那么就得再增加了x+1个新兔子了。所以我们可以使用一个HashMap来建立某种颜色兔子的总个数和在数组中还允许出现的个数之间的映射，然后我们遍历数组中的每个兔子，如果该兔子回答了x个，若该颜色兔子的总个数x+1不在HashMap中，或者映射为0了，我们将这x+1个兔子加入结果res中，然后将其映射值设为x，表示在数组中还允许出现x个也回答x的兔子；否则的话，将映射值自减1即可，参见代码如下：



解法一：

```
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        int res = 0;
        unordered_map<int, int> m;      
        for (int ans : answers) {
            if (!m.count(ans + 1) || m[ans + 1] == 0) {
                res += ans + 1;
                m[ans + 1] = ans;
            } else {
                --m[ans + 1];
            }
        }
        return res;
    }
};
```



下面这种方法换了个角度，核心还是当某个兔子回答x的时候，那么数组中最多允许x+1个兔子同时回答x，那么我们统计数组中所有回答x的兔子的数量n：

若 n%(x+1)==0，说明我们此时只需要 n/(x+1) 组个数为x+1的兔子。

若 n%(x+1)!=0，说明我们此时只需要 n/(x+1) + 1 组个数为x+1的兔子。

那么这两种情况可以通过 ceil(n/(x+1)) 来整合，而这个值也等于 (n + x) / (x + 1)，参见代码如下：



解法二：

```
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        int res = 0;
        unordered_map<int, int> m;  
        for (int ans : answers) ++m[ans];
        for (auto a : m) {
            res += (a.second + a.first) / (a.first + 1) * (a.first + 1);
        }
        return res;
    }
};
```



下面这种思路也很巧妙，是先统计出不在数组中的兔子的个数，然后再加上回答问题的兔子数。我们使用一个长度为1000的数字代替HashMap，因为题目中限定了最多1000个兔子回答问题，然后我们对于每个回答x的兔子，将 cnt[x] 的值自增1，然后对 x+1 取余，那么余数就是总数为 x+1 的兔子中在数组中出现的个数，所以我们之后用 x+1 减去这个余数，就是不在数组中的兔子的个数。那么你可能会怀疑，当余数为0了，会不会漏掉了兔子啊，答案是不会的，因为我们最终要加上数组中的兔子个数，如果对 x+1 取余为0了，说明这 x+1 个兔子都在数组中出现了，所以我们不会漏掉任何兔子，参见代码如下：



解法三：

```
class Solution {
public:
    int numRabbits(vector<int>& answers) {
        int res = 0;
        vector<int> cnt(1000, 0); 
        for (int ans : answers) cnt[ans] = (cnt[ans] + 1) % (ans + 1);
        for (int i = 0; i < 1000; ++i) {
            if (cnt[i] != 0) res += i + 1 - cnt[i];
        }
        return res + answers.size();
    }
};
```



参考资料：

[https://leetcode.com/problems/rabbits-in-forest/solution/](https://leetcode.com/problems/rabbits-in-forest/solution/)

[https://leetcode.com/problems/rabbits-in-forest/discuss/123206/Java-4-Liner-No-HashMap](https://leetcode.com/problems/rabbits-in-forest/discuss/123206/Java-4-Liner-No-HashMap)

[https://leetcode.com/problems/rabbits-in-forest/discuss/114719/My-easy-Java-HashMap-solution](https://leetcode.com/problems/rabbits-in-forest/discuss/114719/My-easy-Java-HashMap-solution)

[https://leetcode.com/problems/rabbits-in-forest/discuss/114721/Easy-and-Concise-Solution-C++JavaPython](https://leetcode.com/problems/rabbits-in-forest/discuss/114721/Easy-and-Concise-Solution-C++JavaPython)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












