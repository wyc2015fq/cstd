# [LeetCode] Employee Importance 员工重要度 - Grandyang - 博客园







# [[LeetCode] Employee Importance 员工重要度](https://www.cnblogs.com/grandyang/p/7639798.html)







You are given a data structure of employee information, which includes the employee's **unique id**, his **importance value** and his **direct** subordinates' id.

For example, employee 1 is the leader of employee 2, and employee 2 is the leader of employee 3. They have importance value 15, 10 and 5, respectively. Then employee 1 has a data structure like [1, 15, [2]], and employee 2 has [2, 10, [3]], and employee 3 has [3, 5, []]. Note that although employee 3 is also a subordinate of employee 1, the relationship is **not direct**.

Now given the employee information of a company, and an employee id, you need to return the total importance value of this employee and all his subordinates.

**Example 1:**

**Input:** [[1, 5, [2, 3]], [2, 3, []], [3, 3, []]], 1
**Output:** 11
**Explanation:**
Employee 1 has importance value 5, and he has two direct subordinates: employee 2 and employee 3. They both have importance value 3. So the total importance value of employee 1 is 5 + 3 + 3 = 11.

**Note:**

- One employee has at most one **direct** leader and may have several subordinates.
- The maximum number of employees won't exceed 2000.



这道题定义了一种员工类，有id，重要度，和direct report的员工，让我们求某个员工的总重要度。我们要明白的是就算某个员工不直接向你汇报工作，而是向你手下人汇报，这个人的重要度也会算进你的重要度中，想到了狗家的统计直接汇报个数系统就是这样的。这其实就是之前那道 [Nested List Weight Sum](http://www.cnblogs.com/grandyang/p/5340305.html) 的变化形式，我们可以用DFS来做。首先我们想，为了快速的通过id来定位到员工类，需要建立一个id和员工类的映射，然后我们根据给定的员工id来算其重要度。计算方法当然是其本身的重要度加上其所有手下人的重要度，对于手下人，还要累加其手下人的重要度。需要注意的是，像这种类似有向图遍历的问题都需要用一个HashSet来记录访问过的结点，以免有环存在，从而陷入无限死循环。但是由于这道题的场景比较特殊，一个人是不可能给自己的下属汇报的，所以不会有环存在，我们也乐得省事。建立一个结果res变量，加上当前员工的重要度，然后遍历其所有手下，对其每个手下人调用递归函数加到res上，最后返回res即可，参见代码如下：



解法一：

```
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        unordered_map<int, Employee*> m;
        for (auto e : employees) m[e->id] = e;
        return helper(id, m);
    }
    int helper(int id, unordered_map<int, Employee*>& m) {
        int res = m[id]->importance;
        for (int num : m[id]->subordinates) {
            res += helper(num, m);
        }
        return res;
    }
};
```



我们也可以用BFS来做，使用一个queue来辅助运算，开始将给定员工id放入，然后当queue不为空进行循环，每次取出队首员工，累加上当前员工的复杂度到结果res，然后将其所有手下人加入队列等待遍历，参见代码如下：



解法二：

```
class Solution {
public:
    int getImportance(vector<Employee*> employees, int id) {
        int res = 0;
        queue<int> q{{id}};
        unordered_map<int, Employee*> m;
        for (auto e : employees) m[e->id] = e;
        while (!q.empty()) {
            auto t = q.front(); q.pop();
            res += m[t]->importance;
            for (int num : m[t]->subordinates) {
                q.push(num);
            }
        }
        return res;
    }
};
```



类似题目：

[Nested List Weight Sum](http://www.cnblogs.com/grandyang/p/5340305.html)



参考资料：

[https://leetcode.com/problems/employee-importance/](https://leetcode.com/problems/employee-importance/)

[https://leetcode.com/problems/employee-importance/discuss/112587/Java-HashMap-bfs-dfs](https://leetcode.com/problems/employee-importance/discuss/112587/Java-HashMap-bfs-dfs)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












