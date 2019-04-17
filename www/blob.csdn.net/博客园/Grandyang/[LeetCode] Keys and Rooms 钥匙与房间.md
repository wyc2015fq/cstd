# [LeetCode] Keys and Rooms 钥匙与房间 - Grandyang - 博客园







# [[LeetCode] Keys and Rooms 钥匙与房间](https://www.cnblogs.com/grandyang/p/10415773.html)







There are `N` rooms and you start in room `0`.  Each room has a distinct number in `0, 1, 2, ..., N-1`, and each room may have some keys to access the next room. 

Formally, each room `i` has a list of keys `rooms[i]`, and each key `rooms[i][j]` is an integer in `[0, 1, ..., N-1]` where `N = rooms.length`.  A key `rooms[i][j] = v` opens the room with number `v`.

Initially, all the rooms start locked (except for room `0`). 

You can walk back and forth between rooms freely.

Return `true` if and only if you can enter every room.

Example 1:
Input: [[1],[2],[3],[]]
Output: true
Explanation:  
We start in room 0, and pick up key 1.
We then go to room 1, and pick up key 2.
We then go to room 2, and pick up key 3.
We then go to room 3.  Since we were able to go to every room, we return true.

Example 2:
Input: [[1,3],[3,0,1],[2],[0]]
Output: false
Explanation: We can't enter the room with number 2.

Note:
- `1 <= rooms.length <= 1000`
- `0 <= rooms[i].length <= 1000`
- The number of keys in all rooms combined is at most `3000`.



这道题给了我们一些房间，房间里有一些钥匙，用钥匙可以打开对应的房间，说是起始时在房间0，问我们最终是否可以打开所有的房间。这不由得让博主想起了惊悚片万能钥匙，还真是头皮发麻啊。赶紧扯回来，这是一道典型的有向图的遍历的题，邻接链表都已经帮我们建立好了，我们直接遍历就好了，这里先用BFS来遍历。使用一个HashSet来记录访问过的房间，先把0放进去，然后使用queue来辅助遍历，同样将0放入。之后进行典型的BFS遍历，取出队首的房间，然后遍历其中的所有钥匙，若该钥匙对应的房间已经遍历过了，直接跳过，否则就将钥匙加入HashSet。此时我们看若HashSet中的钥匙数已经等于房间总数了，直接返回true，因为这表示所有房间已经访问过了，否则就将钥匙加入队列继续遍历。最后遍历结束后，就看HashSet中的钥匙数是否和房间总数相等即可，参见代码如下：



解法一：

```
class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        unordered_set<int> visited{{0}};
        queue<int> q{{0}};
        while (!q.empty()) {
            int t = q.front(); q.pop();
            for (int key : rooms[t]) {
                if (visited.count(key)) continue;
                visited.insert(key);
                if (visited.size() == rooms.size()) return true;
                q.push(key);
            }
        }
        return visited.size() == rooms.size();
    }
};
```



我们也可以使用递归的解法来做，还是使用HashSet来记录访问过的房间，递归函数还需要传进当前的房间，还有HashSet，首先将当前房间加入HashSet，然后遍历此房间中的所有钥匙，如果其对应的房间没有访问过，则调用递归函数，参见代码如下：



解法二：

```
class Solution {
public:
    bool canVisitAllRooms(vector<vector<int>>& rooms) {
        unordered_set<int> visited;
        helper(rooms, 0, visited);
        return visited.size() == rooms.size();
    }
    void helper(vector<vector<int>>& rooms, int cur, unordered_set<int>& visited) {
        visited.insert(cur);
        for (int key : rooms[cur]) {
            if (!visited.count(key)) helper(rooms, key, visited);
        }
    }
};
```



参考资料：

[https://leetcode.com/problems/keys-and-rooms/](https://leetcode.com/problems/keys-and-rooms/)

[https://leetcode.com/problems/keys-and-rooms/discuss/133944/Java-8-lines](https://leetcode.com/problems/keys-and-rooms/discuss/133944/Java-8-lines)

[https://leetcode.com/problems/keys-and-rooms/discuss/133855/Straight-Forward](https://leetcode.com/problems/keys-and-rooms/discuss/133855/Straight-Forward)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












