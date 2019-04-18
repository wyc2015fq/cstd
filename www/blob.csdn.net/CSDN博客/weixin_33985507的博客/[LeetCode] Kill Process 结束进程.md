# [LeetCode] Kill Process 结束进程 - weixin_33985507的博客 - CSDN博客
2017年07月10日 06:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
Given n processes, each process has a unique PID (process id) and its PPID (parent process id).
Each process only has one parent process, but may have one or more children processes. This is just like a tree structure. Only one process has PPID that is 0, which means this process has no parent process. All the PIDs will be distinct positive integers.
We use two list of integers to represent a list of processes, where the first list contains PID for each process and the second list contains the corresponding PPID.
Now given the two lists, and a PID representing a process you want to kill, return a list of PIDs of processes that will be killed in the end. You should assume that when a process is killed, all its children processes will be killed. No order is required for the final answer.
Example 1:
Input: 
pid =  [1, 3, 10, 5]
ppid = [3, 0, 5, 3]
kill = 5
Output: [5,10]
Explanation: 
           3
         /   \
        1     5
             /
            10
Kill 5 will also kill 10.
Note:
- The given kill id is guaranteed to be one of the given PIDs.
- n >= 1.
这道题让我们结束进程，一直不想翻译程杀死进程，感觉进程很可怜的样子，还要被杀死。题目给了我们两个数组，一个是进程的数组，还有一个是进程数组中的每个进程的父进程组成的数组。题目中说结束了某一个进程，其所有的子进程都需要结束，由于一个进程可能有多个子进程，所以我们首先要理清父子进程的关系。所以我们使用一个哈希表，建立进程和其所有子进程之间的映射，然后我们首先把要结束的进程放入一个队列queue中，然后while循环，每次取出一个进程，将其加入结果res中，然后遍历其所有子进程，将所有子进程都排入队列中，这样我们就能结束所有相关的进程，参见代码如下：
解法一：
```
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        vector<int> res;
        queue<int> q{{kill}};
        unordered_map<int, vector<int>> m;
        for (int i = 0; i < pid.size(); ++i) {
            m[ppid[i]].push_back(pid[i]);
        }
        while (!q.empty()) {
            int t = q.front(); q.pop();
            res.push_back(t);
            for (int p : m[t]) {
                q.push(p);
            }
        }
        return res;
    }
};
```
我们也可以使用递归的写法，思路都一样，只不过用递归函数来代替队列，参见代码如下：
解法二：
```
class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        vector<int> res;
        unordered_map<int, vector<int>> m;
        for (int i = 0; i < pid.size(); ++i) {
            m[ppid[i]].push_back(pid[i]);
        }
        helper(kill, m, res);
        return res;
    }
    void helper(int kill, unordered_map<int, vector<int>>& m, vector<int>& res) {
        res.push_back(kill);
        for (int p : m[kill]) {
            helper(p, m, res);
        }
    }
};
```
参考资料：
[https://discuss.leetcode.com/topic/89293/c-clean-code-2-solution-dfs-bfs](https://discuss.leetcode.com/topic/89293/c-clean-code-2-solution-dfs-bfs)
[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)
