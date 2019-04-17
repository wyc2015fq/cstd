# [LeetCode] Accounts Merge 账户合并 - Grandyang - 博客园







# [[LeetCode] Accounts Merge 账户合并](https://www.cnblogs.com/grandyang/p/7829169.html)








Given a list `accounts`, each element `accounts[i]` is a list of strings, where the first element `accounts[i][0]` is a *name*, and the rest of the elements are *emails* representing emails of the account.

Now, we would like to merge these accounts. Two accounts definitely belong to the same person if there is some email that is common to both accounts. Note that even if two accounts have the same name, they may belong to different people as people could have the same name. A person can have any number of accounts initially, but all of their accounts definitely have the same name.

After merging the accounts, return the accounts in the following format: the first element of each account is the name, and the rest of the elements are emails in sorted order. The accounts themselves can be returned in any order.

Example 1:
Input: 
accounts = [["John", "johnsmith@mail.com", "john00@mail.com"], ["John", "johnnybravo@mail.com"], ["John", "johnsmith@mail.com", "john_newyork@mail.com"], ["Mary", "mary@mail.com"]]
Output: [["John", 'john00@mail.com', 'john_newyork@mail.com', 'johnsmith@mail.com'],  ["John", "johnnybravo@mail.com"], ["Mary", "mary@mail.com"]]
Explanation: 
The first and third John's are the same person as they have the common email "johnsmith@mail.com".
The second John and Mary are different people as none of their email addresses are used by other accounts.
We could return these lists in any order, for example the answer [['Mary', 'mary@mail.com'], ['John', 'johnnybravo@mail.com'], 
['John', 'john00@mail.com', 'john_newyork@mail.com', 'johnsmith@mail.com']] would still be accepted.



Note:
- The length of `accounts` will be in the range `[1, 1000]`.
- The length of `accounts[i]` will be in the range `[1, 10]`.
- The length of `accounts[i][j]` will be in the range `[1, 30]`.



这道题给了我们一堆人名和邮箱，一个名字可能有多个邮箱，但是一个邮箱只属于一个人，让我们把同一个人的邮箱都合并到一起，名字相同不一定是同一个人，只有当两个名字有相同的邮箱，才能确定是同一个人，题目中的例子很好说明了这个问题，输入有三个 John，最后合并之后就只有两个了。这道题博主最开始尝试使用贪婪算法来做，结果发现对于下面这个例子不适用：

["John", "a@gmail.com", "b@gmail.com"]

["John", "c@gmail.com", "d@gmail.com"]

["John", "a@gmail.com", "c@gmail.com"]

我们可以看到其实这三个 John 是同一个人，但是贪婪算法遍历完前两个 John，还是认为其是两个不同的人，当遍历第三个 John 时，就直接加到第一个 John 中了，而没有同时把第二个 John 加进来，也可能博主写的是假的贪婪算法，反正不管了，还是参考大神们的解法吧。这个归组类的问题，最典型的就是岛屿问题(例如 [Number of Islands II](http://www.cnblogs.com/grandyang/p/5190419.html))，很适合使用 Union Find 来做，LeetCode 中有很多道可以使用这个方法来做的题，比如 [Friend Circles](http://www.cnblogs.com/grandyang/p/6686983.html)，[Graph Valid Tree](http://www.cnblogs.com/grandyang/p/5257919.html)，[Number of Connected Components in an Undirected Graph](http://www.cnblogs.com/grandyang/p/5166356.html)，和 [Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html) 等等。都是要用一个 root 数组，每个点开始初始化为不同的值，如果两个点属于相同的组，就将其中一个点的 root 值赋值为另一个点的位置，这样只要是相同组里的两点，通过 find 函数得到相同的值。在这里，由于邮件是字符串不是数字，所以 root 可以用 HashMap 来代替，我们还需要一个 HashMap 映射owner，建立每个邮箱和其所有者姓名之前的映射，另外用一个 HashMap 来建立用户和其所有的邮箱之间的映射，也就是合并后的结果。

首先我们遍历每个账户和其中的所有邮箱，先将每个邮箱的 root 映射为其自身，然后将 owner 赋值为用户名。然后开始另一个循环，遍历每一个账号，首先对帐号的第一个邮箱调用 find 函数，得到其父串p，然后遍历之后的邮箱，对每个遍历到的邮箱先调用 find 函数，将其父串的 root 值赋值为p，这样做相当于将相同账号内的所有邮箱都链接起来了。我们下来要做的就是再次遍历每个账户内的所有邮箱，先对该邮箱调用 find 函数，找到父串，然后将该邮箱加入该父串映射的集合汇总，这样就我们就完成了合并。最后只需要将集合转为字符串数组，加入结果 res 中，通过 owner 映射找到父串的用户名，加入字符串数组的首位置，参见代码如下：



解法一：

```
class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        vector<vector<string>> res;
        unordered_map<string, string> root;
        unordered_map<string, string> owner;
        unordered_map<string, set<string>> m;
        for (auto account : accounts) {
            for (int i = 1; i < account.size(); ++i) {
                root[account[i]] = account[i];
                owner[account[i]] = account[0];
            }
        }
        for (auto account : accounts) {
            string p = find(account[1], root);
            for (int i = 2; i < account.size(); ++i) {
                root[find(account[i], root)] = p;
            }
        }
        for (auto account : accounts) {
            for (int i = 1; i < account.size(); ++i) {
                m[find(account[i], root)].insert(account[i]);
            }
        }
        for (auto a : m) {
            vector<string> v(a.second.begin(), a.second.end());
            v.insert(v.begin(), owner[a.first]);
            res.push_back(v);
        }
        return res;
    }
    string find(string s, unordered_map<string, string>& root) {
        return root[s] == s ? s : find(root[s], root);
    }
};
```



下面这种方法是使用 BFS 来解的，建立了每个邮箱和其所有出现的账户数组之间的映射，比如还是这个例子：

["John", "a@gmail.com", "b@gmail.com"]

["John", "c@gmail.com", "d@gmail.com"]

["John", "a@gmail.com", "c@gmail.com"]

那么建立的映射就是：

"a@gmail.com" -> [0, 2]

"b@gmail.com" -> [0]

"c@gmail.com" -> [1, 2]

"d@gmail.com" -> [1]

然后我们还需要一个 visited 数组，来标记某个账户是否已经被遍历过，0表示为未访问，1表示已访问。在建立好 HashMap 之后，我们遍历所有的账户，如果账户未被访问过，将其加入队列 queue，新建一个 TreeSet，此时进行队列不为空的 while 循环，取出队首账户，将该该账户标记已访问1，此时将该账户的所有邮箱取出来放入数组 mails 中，然后遍历 mails 中的每一个邮箱，将遍历到的邮箱加入 TreeSet 中，根据映射来找到该邮箱所属的所有账户，如果该账户未访问，则加入队列中并标记已访问。当 while 循环结束后，当前账户的所有合并后的邮箱都保存在 TreeSet 中，将其转为字符串数组，并且加上用户名在首位置，最后加入结果 res 中即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        vector<vector<string>> res;
        int n = accounts.size();
        unordered_map<string, vector<int>> m;
        vector<int> visited(n, 0);
        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < accounts[i].size(); ++j) {
                m[accounts[i][j]].push_back(i);
            }
        }
        for (int i = 0; i < n; ++i) {
            if (visited[i] != 0) continue;
            queue<int> q{{i}};
            set<string> s;
            while (!q.empty()) {
                int t = q.front(); q.pop();
                visited[t] = 1;
                vector<string> mails(accounts[t].begin() + 1, accounts[t].end());
                for (string mail : mails) {
                    s.insert(mail);
                    for (int user : m[mail]) {
                        if (visited[user] != 0) continue;
                        q.push(user);
                        visited[user] = 1;
                    }
                }
            }
            vector<string> out(s.begin(), s.end());
            out.insert(out.begin(), accounts[i][0]);
            res.push_back(out);
        }
        return res;
    }
};
```




类似题目：

[Redundant Connection](http://www.cnblogs.com/grandyang/p/7628977.html)

[Sentence Similarity II](http://www.cnblogs.com/grandyang/p/8053934.html)

[Sentence Similarity](http://www.cnblogs.com/grandyang/p/8016251.html)



参考资料：

[https://leetcode.com/problems/accounts-merge/](https://leetcode.com/problems/accounts-merge/)

[https://leetcode.com/problems/accounts-merge/discuss/109189/simple-c-sol](https://leetcode.com/problems/accounts-merge/discuss/109189/simple-c-sol)

[https://leetcode.com/problems/accounts-merge/discuss/109157/javac-union-find](https://leetcode.com/problems/accounts-merge/discuss/109157/javac-union-find)

[https://leetcode.com/problems/accounts-merge/discuss/109158/java-solution-build-graph-dfs-search](https://leetcode.com/problems/accounts-merge/discuss/109158/java-solution-build-graph-dfs-search)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













