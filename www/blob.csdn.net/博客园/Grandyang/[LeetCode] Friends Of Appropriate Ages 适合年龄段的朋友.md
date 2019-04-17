# [LeetCode] Friends Of Appropriate Ages 适合年龄段的朋友 - Grandyang - 博客园







# [[LeetCode] Friends Of Appropriate Ages 适合年龄段的朋友](https://www.cnblogs.com/grandyang/p/10252913.html)







Some people will make friend requests. The list of their ages is given and `ages[i]` is the age of the ith person. 

Person A will NOT friend request person B (B != A) if any of the following conditions are true:
- `age[B] <= 0.5 * age[A] + 7`
- `age[B] > age[A]`
- `age[B] > 100 && age[A] < 100`

Otherwise, A will friend request B.

Note that if A requests B, B does not necessarily request A.  Also, people will not friend request themselves.

How many total friend requests are made?

Example 1:
Input: [16,16]
Output: 2
Explanation: 2 people friend request each other.

Example 2:
Input: [16,17,18]
Output: 2
Explanation: Friend requests are made 17 -> 16, 18 -> 17.
Example 3:
Input: [20,30,100,110,120]
Output: 
Explanation: Friend requests are made 110 -> 100, 120 -> 110, 120 -> 100.



Notes:
- `1 <= ages.length <= 20000`.
- `1 <= ages[i] <= 120`.



这道题是关于好友申请的，说是若A想要加B的好友，下面三个条件一个都不能满足才行：

1. B的年龄小于等于A的年龄的一半加7。

2. B的年龄大于A的年龄。

3. B大于100岁，且A小于100岁。

实际上如果你仔细看条件3，B要是大于100岁，A小于100岁，那么B一定大于A，这就跟条件2重复了，完全不懂出题者的想法。不管了，强行解题呗。那么由于只能给比自己小的人发送好友请求，那么博主就想到我们可以献给所有人拍个序，然后从后往前遍历，对于每个遍历到的人，再遍历所有比他小的人，这样第二个条件就满足了，前面说了，第三个条件可以不用管了，那么只要看满足第一个条件就可以了，还有要注意的，假如两个人年龄相同，那么满足了前两个条件后，其实是可以互粉的，所以要额外的加1，这样才不会漏掉任何一个好友申请，参见代码如下：



解法一：

```
class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        int res = 0, n = ages.size();
        sort(ages.begin(), ages.end());
        for (int i = n - 1; i >= 1; --i) {
            for (int j = i - 1; j >= 0; --j) {
                if (ages[j] <= 0.5 * ages[i] + 7) continue;
                if (ages[i] == ages[j]) ++res;
                ++res;
            }
        }
        return res;
    }
};
```



我们可以来优化一下上面的解法，根据上面的分析，其实题目给的这三个条件可以归纳成一个条件，若A想加B的好友，那么B的年龄必须在 (A*0.5+7, A] 这个范围内，由于区间要有意义的话，A*0.5+7 < A 必须成立，解出 A > 14，那么A最小就只能取15了。意思说你不能加小于15岁的好友（青少年成长保护？？？）。我们的优化思路是对于每一个年龄，我们都只要求出上面那个区间范围内的个数，就是符合题意的。那么既然是求区域和，建立累加数组就是一个很好的选择了，首先我们先建立一个统计数组numInAge，范围是[0, 120]，用来统计在各个年龄点上有多少人，然后再建立累加和数组sumInAge。这个都建立好了以后，我们就可以开始遍历，由于之前说了不能加小于15的好友，所以我们从15开始遍历，如果某个年龄点没有人，直接跳过。然后就是统计出 (A*0.5+7, A] 这个范围内有多少人，可以通过累计和数组来快速求的，由于当前时间点的人可以跟这个区间内的所有发好友申请，而当前时间点可能还有多人，所以二者相乘，但由于我们但区间里还包括但当前年龄点本身，所以还要减去当前年龄点上的人数，参见代码如下：



解法二：

```
class Solution {
public:
    int numFriendRequests(vector<int>& ages) {
        int res = 0;
        vector<int> numInAge(121), sumInAge(121);
        for (int age : ages) ++numInAge[age];
        for (int i = 1; i <= 120; ++i) {
            sumInAge[i] = numInAge[i] + sumInAge[i - 1];
        }
        for (int i = 15; i <= 120; ++i) {
            if (numInAge[i] == 0) continue;
            int cnt = sumInAge[i] - sumInAge[i * 0.5 + 7];
            res += cnt * numInAge[i] - numInAge[i];
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/friends-of-appropriate-ages/](https://leetcode.com/problems/friends-of-appropriate-ages/)

[https://leetcode.com/problems/friends-of-appropriate-ages/discuss/127341/10ms-concise-Java-solution-O(n)-time-and-O(1)-space](https://leetcode.com/problems/friends-of-appropriate-ages/discuss/127341/10ms-concise-Java-solution-O(n)-time-and-O(1)-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












