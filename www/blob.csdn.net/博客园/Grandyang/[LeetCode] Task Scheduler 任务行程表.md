# [LeetCode] Task Scheduler 任务行程表 - Grandyang - 博客园







# [[LeetCode] Task Scheduler 任务行程表](https://www.cnblogs.com/grandyang/p/7098764.html)







Given a char array representing tasks CPU need to do. It contains capital letters A to Z where different letters represent different tasks.Tasks could be done without original order. Each task could be done in one interval. For each interval, CPU could finish one task or just be idle.

However, there is a non-negative cooling interval n that means between two same tasks, there must be at least n intervals that CPU are doing different tasks or just be idle.

You need to return the least number of intervals the CPU will take to finish all the given tasks.

Example 1:
Input: tasks = ['A','A','A','B','B','B'], n = 2
Output: 8
Explanation: A -> B -> idle -> A -> B -> idle -> A -> B.



Note:
- The number of tasks is in the range [1, 10000].
- The integer n is in the range [0, 100].



这道题让我们安排CPU的任务，规定在两个相同任务之间至少隔n个时间点。说实话，刚开始博主并没有完全理解题目的意思，后来看了大神们的解法才悟出个道理来。下面这种解法参考了[大神fatalme的帖子](https://discuss.leetcode.com/topic/92852/concise-java-solution-o-n-time-o-26-space)，由于题目中规定了两个相同任务之间至少隔n个时间点，那么我们首先应该处理的出现次数最多的那个任务，先确定好这些高频任务，然后再来安排那些低频任务。如果任务F的出现频率最高，为k次，那么我们用n个空位将每两个F分隔开，然后我们按顺序加入其他低频的任务，来看一个例子：

AAAABBBEEFFGG 3

我们发现任务A出现了4次，频率最高，于是我们在每个A中间加入三个空位，如下：

A---A---A---A

AB--AB--AB--A   (加入B)

ABE-ABE-AB--A   (加入E)

ABEFABE-ABF-A   (加入F，每次尽可能填满或者是均匀填充)

ABEFABEGABFGA   (加入G)

再来看一个例子：

ACCCEEE 2

我们发现任务C和E都出现了三次，那么我们就将CE看作一个整体，在中间加入一个位置即可：

CE-CE-CE

CEACE-CE   (加入A)

注意最后面那个idle不能省略，不然就不满足相同两个任务之间要隔2个时间点了。

这道题好在没有让我们输出任务安排结果，而只是问所需的时间总长，那么我们就想个方法来快速计算出所需时间总长即可。我们仔细观察上面两个例子可以发现，都分成了(mx - 1)块，再加上最后面的字母，其中mx为最大出现次数。比如例子1中，A出现了4次，所以有A---模块出现了3次，再加上最后的A，每个模块的长度为4。例子2中，CE-出现了2次，再加上最后的CE，每个模块长度为3。我们可以发现，模块的次数为任务最大次数减1，模块的长度为n+1，最后加上的字母个数为出现次数最多的任务，可能有多个并列。这样三个部分都搞清楚了，写起来就不难了，我们统计每个大写字母出现的次数，然后排序，这样出现次数最多的字母就到了末尾，然后我们向前遍历，找出出现次数一样多的任务个数，就可以迅速求出总时间长了，下面这段代码可能最不好理解的可能就是最后一句了，那么我们特别来讲解一下。先看括号中的第二部分，前面分析说了mx是出现的最大次数，mx-1是可以分为的块数，n+1是每块中的个数，而后面的 25-i 是还需要补全的个数，用之前的例子来说明：

AAAABBBEEFFGG 3

A出现了4次，最多，mx=4，那么可以分为mx-1=3块，如下：

A---A---A---

每块有n+1=4个，最后还要加上末尾的一个A，也就是25-24=1个任务，最终结果为13：

ABEFABEGABFGA

再来看另一个例子：

ACCCEEE 2

C和E都出现了3次，最多，mx=3，那么可以分为mx-1=2块，如下：

CE-CE-

每块有n+1=3个，最后还要加上末尾的一个CE，也就是25-23=2个任务，最终结果为8：

CEACE-CE

好，那么此时你可能会有疑问，为啥还要跟原任务个数len相比，取较大值呢？我们再来看一个例子：

AAABBB 0

A和B都出现了3次，最多，mx=3，那么可以分为mx-1=2块，如下：

ABAB

每块有n+1=1个？你会发现有问题，这里明明每块有两个啊，为啥这里算出来n+1=1呢，因为给的n=0，这有没有矛盾呢，没有！因为n表示相同的任务间需要间隔的个数，那么既然这里为0了，说明相同的任务可以放在一起，这里就没有任何限制了，我们只需要执行完所有的任务就可以了，所以我们最终的返回结果一定不能小于任务的总个数len的，这就是要对比取较大值的原因了。

参见代码如下：



解法一：

```
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        vector<int> cnt(26, 0);
        for (char task : tasks) {
            ++cnt[task - 'A'];
        }
        sort(cnt.begin(), cnt.end());
        int i = 25, mx = cnt[25], len = tasks.size();
        while (i >= 0 && cnt[i] == mx) --i;
        return max(len, (mx - 1) * (n + 1) + 25 - i);
    }
};
```



下面这种解法是根据[大神](https://discuss.leetcode.com/topic/92966/java-o-n-time-o-1-space-1-pass-no-sorting-solution-with-detailed-explanation/2)[jinzhou的帖子](https://discuss.leetcode.com/topic/92966/java-o-n-time-o-1-space-1-pass-no-sorting-solution-with-detailed-explanation/2)，优点是代码更容易读懂，而且变量命名很reasonable，前半部分都是一样的，求出最多的次数mx，还有同时出现mx次的不同任务的个数mxCnt。这个解法的思想是先算出所有空出来的位置，然后计算出所有需要填入的task的个数，如果超出了空位的个数，就需要最后再补上相应的个数。注意这里如果有多个任务出现次数相同，那么将其整体放一起，就像上面的第二个例子中的CE一样，那么此时每个part中的空位个数就是n - (mxCnt - 1)，那么空位的总数就是part的总数乘以每个part中空位的个数了，那么我们此时除去已经放入part中的，还剩下的task的个数就是task的总个数减去mx * mxCnt，然后此时和之前求出的空位数相比较，如果空位数要大于剩余的task数，那么则说明还需补充多余的空位，否则就直接返回task的总数即可，参见代码如下：




解法二：

```
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        int mx = 0, mxCnt = 0;
        vector<int> cnt(26, 0);        
        for (char task : tasks) {
            ++cnt[task - 'A'];
            if (mx == cnt[task - 'A']) {
                ++mxCnt;
            } else if (mx < cnt[task - 'A']) {
                mx = cnt[task - 'A'];
                mxCnt = 1;
            }
        }
        int partCnt = mx - 1;
        int partLen = n - (mxCnt - 1);
        int emptySlots = partCnt * partLen;
        int taskLeft = tasks.size() - mx * mxCnt;
        int idles = max(0, emptySlots - taskLeft);
        return tasks.size() + idles;
    }
};
```



下面这种解法是参考的[大神](https://discuss.leetcode.com/topic/92873/c-java-clean-code-priority-queue/2)[alexander的解法](https://discuss.leetcode.com/topic/92873/c-java-clean-code-priority-queue/2)，思路是建立一个优先队列，然后把统计好的个数都存入优先队列中，那么大的次数会在队列的前面。这题还是要分块，每块能装n+1个任务，装任务是从优先队列中取，每个任务取一个，装到一个临时数组中，然后遍历取出的任务，对于每个任务，将其哈希表映射的次数减1，如果减1后，次数仍大于0，则将此任务次数再次排入队列中，遍历完后如果队列不为空，说明该块全部被填满，则结果加上n+1。我们之前在队列中取任务是用个变量cnt来记录取出任务的个数，我们想取出n+1个，如果队列中任务数少于n+1个，那就用cnt来记录真实取出的个数，当队列为空时，就加上cnt的个数，参见代码如下：



解法三：

```
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        int res = 0, cycle = n + 1;
        unordered_map<char, int> m;
        priority_queue<int> q;
        for (char c : tasks) ++m[c];
        for (auto a : m) q.push(a.second);
        while (!q.empty()) {
            int cnt = 0;
            vector<int> t;
            for (int i = 0; i < cycle; ++i) {
                if (!q.empty()) {
                    t.push_back(q.top()); q.pop();
                    ++cnt;
                }
            }
            for (int d : t) {
                if (--d > 0) q.push(d);
            }
            res += q.empty() ? cnt : cycle;
        }
        return res;
    }
};
```



类似题目：

[Rearrange String k Distance Apart](http://www.cnblogs.com/grandyang/p/5586009.html)

[Reorganize String](http://www.cnblogs.com/grandyang/p/8799483.html)



参考资料：

[https://leetcode.com/problems/task-scheduler/](https://leetcode.com/problems/task-scheduler/)

[https://leetcode.com/problems/task-scheduler/discuss/104493/c-java-clean-code-priority-queue](https://leetcode.com/problems/task-scheduler/discuss/104493/c-java-clean-code-priority-queue)

[https://leetcode.com/problems/task-scheduler/discuss/104496/concise-java-solution-on-time-o26-space](https://leetcode.com/problems/task-scheduler/discuss/104496/concise-java-solution-on-time-o26-space)

[https://leetcode.com/problems/task-scheduler/discuss/104500/java-on-time-o1-space-1-pass-no-sorting-solution-with-detailed-explanation](https://leetcode.com/problems/task-scheduler/discuss/104500/java-on-time-o1-space-1-pass-no-sorting-solution-with-detailed-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












