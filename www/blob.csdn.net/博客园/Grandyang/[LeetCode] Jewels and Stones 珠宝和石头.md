# [LeetCode] Jewels and Stones 珠宝和石头 - Grandyang - 博客园







# [[LeetCode] Jewels and Stones 珠宝和石头](https://www.cnblogs.com/grandyang/p/8910994.html)









You're given strings `J` representing the types of stones that are jewels, and `S` representing the stones you have.  Each character in `S`is a type of stone you have.  You want to know how many of the stones you have are also jewels.

The letters in `J` are guaranteed distinct, and all characters in `J` and `S` are letters. Letters are case sensitive, so `"a"` is considered a different type of stone from `"A"`.

Example 1:
Input: J = "aA", S = "aAAbbbb"
Output: 3

Example 2:
Input: J = "z", S = "ZZ"
Output: 0

Note:
- `S` and `J` will consist of letters and have length at most 50.
- The characters in `J` are distinct.





这道题给了我们两个字符串，珠宝字符串J和石头字符串S，其中J中的每个字符都是珠宝，S中的每个字符都是石头，问我们S中有多少个珠宝。这道题没什么难度，高于八成的Accept率也应证了其Easy难度实至名归。那么先来暴力搜索吧，就将S中的每个字符都在J中搜索一遍，搜索到了就break掉，参见代码如下：



解法一：

```
class Solution {
public:
    int numJewelsInStones(string J, string S) {
        int res = 0;
        for (char s : S) {
            for (char j : J) {
                if (s == j) {
                    ++res; break;
                }
            }
        }
        return res;
    }
};
```



我们用HashSet来优化时间复杂度，将珠宝字符串J中的所有字符都放入HashSet中，然后遍历石头字符串中的每个字符，到HashSet中查找是否存在，存在的话计数器自增1即可，参见代码如下：



解法二：

```
class Solution {
public:
    int numJewelsInStones(string J, string S) {
        int res = 0;
        unordered_set<char> s;
        for (char c : J) s.insert(c);
        for (char c : S) {
            if (s.count(c)) ++res;
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/jewels-and-stones/solution/](https://leetcode.com/problems/jewels-and-stones/solution/)

[https://leetcode.com/problems/jewels-and-stones/discuss/113553/C++JavaPython-Easy-and-Concise-Solution-O(M+N)](https://leetcode.com/problems/jewels-and-stones/discuss/113553/C++JavaPython-Easy-and-Concise-Solution-O(M+N))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












