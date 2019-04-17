# [LeetCode] Reorganize String 重构字符串 - Grandyang - 博客园







# [[LeetCode] Reorganize String 重构字符串](https://www.cnblogs.com/grandyang/p/8799483.html)







Given a string `S`, check if the letters can be rearranged so that two characters that are adjacent to each other are not the same.

If possible, output any possible result.  If not possible, return the empty string.

Example 1:
Input: S = "aab"
Output: "aba"

Example 2:
Input: S = "aaab"
Output: ""

Note:
- `S` will consist of lowercase letters and have length in range `[1, 500]`.



这道题给了我们一个字符串，让我们重构这个字符串，使得相同的字符不会相邻，如果无法做到，那么就返回空串，题目中的例子很好的说明了这一点。那么，如果先不考虑代码实现，让你来手动重构的话，该怎么做呢？我们要做的就是把相同的字符分开。比如例子1中，两个a相邻了，所以我们把第二个a和后面的b交换位置，这样分开了相同的字符，就是最终答案了。我们再来看一个例子，比如"aaabbc"，那么其实我们发现第二个字符也是‘a’的时候，就需要往后遍历找到第一个不是‘a’的字符，即‘b’，然后交换‘a’和‘b’即可，然后继续往后面进行同样的处理，当无法找到不同的字符后就返回空串。这种方法对有序的字符串S是可以的，虽然题目给的两个例子中字符串S都是有序的，实际上不一定是有序的。所以博主最先的想法是给数组排序呗，但是博主的这个解法跪在了这个例子上"vvvlo"，我们发现排序后就变成"lovvv"，这样上面提到的解法就跪了。我们希望次数出现多的字符串再前面，这样才好交换嘛。那么我们还是要统计每个字符串出现的次数啊，这里使用HashMap来建立字母和其出现次数之间的映射。由于我们希望次数多的字符排前面，可以使用一个最大堆，C++中就是优先队列Priority Queue，将次数当做排序的key，那么就把次数和其对应的字母组成一个pair，放进最大堆中自动排序。这里其实有个剪枝的trick，如果某个字母出现的频率大于总长度的一半了，那么必然会有两个相邻的字母出现。这里博主就不证明了，感觉有点像抽屉原理。所以我们在将映射对加入优先队列时，先判断下次数，超过总长度一半了的话直接返回空串就行了。

好，我们的最大堆建立好以后，我们想，此时难道还是应该使用上面所说的交换的方法吗？其实直接构建新的字符串要更加简单一些。接下来，我们每次从优先队列中取队首的两个映射对儿处理，因为我们要拆开相同的字母，这两个映射对儿肯定是不同的字母，我们可以将其放在一起，之后我们需要将两个映射对儿中的次数自减1，如果还有多余的字母，即减1后的次数仍大于0的话，将其再放回最大堆。由于我们是两个两个取的，所以最后while循环退出后，有可能优先队列中还剩下了一个映射对儿，此时将其加入结果res即可。而且这个多余的映射对儿一定只有一个字母了，因为我们提前判断过各个字母的出现次数是否小于等于总长度的一半，按这种机制来取字母，不可能会剩下多余一个的相同的字母，参见代码如下：



解法一：

```
class Solution {
public:
    string reorganizeString(string S) {
        string res = "";
        unordered_map<char, int> m;
        priority_queue<pair<int, char>> q;
        for (char c : S) ++m[c];
        for (auto a : m) {
            if (a.second > (S.size() + 1) / 2) return "";
            q.push({a.second, a.first});
        }
        while (q.size() >= 2) {
            auto t1 = q.top(); q.pop();
            auto t2 = q.top(); q.pop();
            res.push_back(t1.second);
            res.push_back(t2.second);
            if (--t1.first > 0) q.push(t1);
            if (--t2.first > 0) q.push(t2);
        }
        if (q.size() > 0) res.push_back(q.top().second);
        return res;
    }
};
```



下面这种解法的原理和上面的很类似，就是写法上很秀，堪比陈独秀。这里使用了一个长度为26的一位数组cnt来代替上面的HashMap进行统计字母的出现次数，然后比较秀的一点是，把上面的映射对儿压缩成了一个整数，做法是将次数乘以了100，再加上当前字母在一位数字中的位置坐标i，这样一个整数就同时encode了次数和对应字母的信息了，而且之后decode也很方便。数组cnt更新好了后，需要排个序，这一步就是模拟上面解法中最大堆的自动排序功能。不过这里是数字小的在前面，即先处理出现次数少的字母。这里除了和上面一样检测次数不能大于总长度的一半的操作外，还有一个小trick，就是构建字符串的时候，是从第二个位置开始的。这里我们构建的字符串是直接对原字符串S进行修改的，因为cnt数组建立了之后，字符串S就没啥用了。我们用一个变量idx来表示当前更新字母的位置，初始化为1，表示我们要从第二个位置开始更新。因为出现次数最多的字母一定要占据第一个位置才行，这就是我们留出第一个位置的原因。这里很叼的一点，就是隔位更新，这样能保证相同的字母不相邻，而且当idx越界后，拉回到起始位置0，这就有点遍历循环数组的感觉。举个栗子来说吧，比如"aaabbc"，我们的更新顺序为：

_ c _ _ _ _

_ c _ b _ _

_ c _ b _ b

a c _ b _ b

a c a b _ b

a c a b a b



解法二：

```
class Solution {
public:
    string reorganizeString(string S) {
        int n = S.size(), idx = 1;
        vector<int> cnt(26, 0);
        for (char c : S) cnt[c - 'a'] += 100;
        for (int i = 0; i < 26; ++i) cnt[i] += i;
        sort(cnt.begin(), cnt.end());
        for (int num : cnt) {
            int t = num / 100;
            char ch = 'a' + (num % 100);
            if (t > (n + 1) / 2) return "";
            for (int i = 0; i < t; ++i) {
                if (idx >= n) idx = 0;
                S[idx] = ch;
                idx += 2;
            }
        }
        return S;
    }
};
```



类似题目：

[Rearrange String k Distance Apart](http://www.cnblogs.com/grandyang/p/5586009.html)

[Task Scheduler](http://www.cnblogs.com/grandyang/p/7098764.html)



参考资料：

[https://leetcode.com/problems/reorganize-string/solution/](https://leetcode.com/problems/reorganize-string/solution/)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












