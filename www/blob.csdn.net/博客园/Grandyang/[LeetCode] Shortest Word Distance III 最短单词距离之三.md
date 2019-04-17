# [LeetCode] Shortest Word Distance III 最短单词距离之三 - Grandyang - 博客园







# [[LeetCode] Shortest Word Distance III 最短单词距离之三](https://www.cnblogs.com/grandyang/p/5192426.html)







This is a follow up of Shortest Word Distance. The only difference is now word1 could be the same as word2.

Given a list of words and two words word1 and word2, return the shortest distance between these two words in the list.

word1 and word2 may be the same and they represent two individual words in the list.

For example,
Assume that words = ["practice", "makes", "perfect", "coding", "makes"].

Given word1 = “makes”, word2 = “coding”, return 1.
Given word1 = "makes", word2 = "makes", return 3.

Note:
You may assume word1 and word2 are both in the list. 



这道题还是让我们求最短单词距离，有了之前两道题[Shortest Word Distance II](http://www.cnblogs.com/grandyang/p/5187640.html)和[Shortest Word Distance](http://www.cnblogs.com/grandyang/p/5187041.html)的基础，就大大降低了题目本身的难度。这道题增加了一个条件，就是说两个单词可能会相同，所以在第一题中的解法的基础上做一些修改，我最先想的解法是基于第一题中的解法二，由于会有相同的单词的情况，那么p1和p2就会相同，这样结果就会变成0，显然不对，所以我们要对word1和word2是否的相等的情况分开处理，如果相等了，由于p1和p2会相同，所以我们需要一个变量t来记录上一个位置，这样如果t不为-1，且和当前的p1不同，我们可以更新结果，如果word1和word2不等，那么还是按原来的方法做，参见代码如下：



解法一：

```
class Solution {
public:
    int shortestWordDistance(vector<string>& words, string word1, string word2) {
        int p1 = -1, p2 = -1, res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            int t = p1;
            if (words[i] == word1) p1 = i;
            if (words[i] == word2) p2 = i;
            if (p1 != -1 && p2 != -1) {
                if (word1 == word2 && t != -1 && t != p1) {
                    res = min(res, abs(t - p1));
                } else if (p1 != p2) {
                    res = min(res, abs(p1 - p2));
                }
            }
        }
        return res;
    }
};
```



上述代码其实可以优化一下，我们并不需要变量t来记录上一个位置，我们将p1初始化为数组长度，p2初始化为数组长度的相反数，然后当word1和word2相等的情况，我们用p1来保存p2的结果，p2赋为当前的位置i，这样我们就可以更新结果了，如果word1和word2不相等，则还跟原来的做法一样，这种思路真是挺巧妙的，参见代码如下：



解法二：

```
class Solution {
public:
    int shortestWordDistance(vector<string>& words, string word1, string word2) {
        int p1 = words.size(), p2 = -words.size(), res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i] == word1) p1 = word1 == word2 ? p2 : i;
            if (words[i] == word2) p2 = i;
            res = min(res, abs(p1 - p2));
        }
        return res;
    }
};
```



我们再来看一种更进一步优化的方法，只用一个变量idx，这个idx的作用就相当于记录上一次的位置，当前idx不等-1时，说明当前i和idx不同，然后我们在word1和word2相同或者words[i]和words[idx]相同的情况下更新结果，最后别忘了将idx赋为i，参见代码如下；



解法三：

```
class Solution {
public:
    int shortestWordDistance(vector<string>& words, string word1, string word2) {
        int idx = -1, res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i] == word1 || words[i] == word2) {
                if (idx != -1 && (word1 == word2 || words[i] != words[idx])) {
                    res = min(res, i - idx);
                }
                idx = i;
            }
        }
        return res;
    }
};
```



类似题目：

[Shortest Word Distance II](http://www.cnblogs.com/grandyang/p/5187640.html)

[Shortest Word Distance](http://www.cnblogs.com/grandyang/p/5187041.html)



参考资料：

[https://leetcode.com/discuss/69493/c-o-n-solution](https://leetcode.com/discuss/69493/c-o-n-solution)

[https://leetcode.com/discuss/70327/short-solution-lines-modified-from-shortest-word-distance](https://leetcode.com/discuss/70327/short-solution-lines-modified-from-shortest-word-distance)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)














