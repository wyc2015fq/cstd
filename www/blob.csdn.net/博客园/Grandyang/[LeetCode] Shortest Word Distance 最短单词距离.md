# [LeetCode] Shortest Word Distance 最短单词距离 - Grandyang - 博客园







# [[LeetCode] Shortest Word Distance 最短单词距离](https://www.cnblogs.com/grandyang/p/5187041.html)







Given a list of words and two words *word1* and *word2*, return the shortest distance between these two words in the list.

For example,
Assume that words = `["practice", "makes", "perfect", "coding", "makes"]`.

Given *word1* = `“coding”`, *word2* = `“practice”`, return 3.
Given *word1* = `"makes"`, *word2* = `"coding"`, return 1.

Note:
You may assume that *word1* does not equal to *word2*, and *word1* and *word2* are both in the list.



这道题让我们给了我们一个单词数组，又给定了两个单词，让我们求这两个单词之间的最小距离，限定了两个单词不同，而且都在数组中。我最先想到的方法比较笨，我首先想的是要用哈希表来做，建立每个单词和其所有出现位置数组的映射，但是后来想想，反正建立映射也要遍历一遍数组，我们还不如直接遍历一遍数组，直接把两个给定单词所有出现的位置分别存到两个数组里，然后我们在对两个数组进行两两比较更新结果，参见代码如下：



解法一：

```
class Solution {
public:
    int shortestDistance(vector<string>& words, string word1, string word2) {
        vector<int> idx1, idx2;
        int res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i] == word1) idx1.push_back(i);
            else if (words[i] == word2) idx2.push_back(i);
        }
        for (int i = 0; i < idx1.size(); ++i) {
            for (int j = 0; j < idx2.size(); ++j) {
                res = min(res, abs(idx1[i] - idx2[j]));
            }
        }
        return res;
    }
};
```



上面的那种方法并不高效，我们其实需要遍历一次数组就可以了，我们用两个变量p1,p2初始化为-1，然后我们遍历数组，遇到单词1，就将其位置存在p1里，若遇到单词2，就将其位置存在p2里，如果此时p1, p2都不为-1了，那么我们更新结果，参见代码如下：



解法二：

```
class Solution {
public:
    int shortestDistance(vector<string>& words, string word1, string word2) {
        int p1 = -1, p2 = -1, res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i] == word1) p1 = i;
            else if (words[i] == word2) p2 = i;
            if (p1 != -1 && p2 != -1) res = min(res, abs(p1 - p2));
        }
        return res;
    }
};
```



下面这种方法只用一个辅助变量idx，初始化为-1，然后遍历数组，如果遇到等于两个单词中的任意一个的单词，我们在看idx是否为-1，若不为-1，且指向的单词和当前遍历到的单词不同，我们更新结果，参见代码如下：



解法三：

```
class Solution {
public:
    int shortestDistance(vector<string>& words, string word1, string word2) {
        int idx = -1, res = INT_MAX;
        for (int i = 0; i < words.size(); ++i) {
            if (words[i] == word1 || words[i] == word2) {
                if (idx != -1 && words[idx] != words[i]) {
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

[Shortest Word Distance III](http://www.cnblogs.com/grandyang/p/5192426.html)



参考资料：

[https://leetcode.com/discuss/50234/ac-java-clean-solution](https://leetcode.com/discuss/50234/ac-java-clean-solution)

[https://leetcode.com/discuss/61820/java-only-need-to-keep-one-index](https://leetcode.com/discuss/61820/java-only-need-to-keep-one-index)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












