# [LeetCode] Shortest Completing Word 最短完整的单词 - Grandyang - 博客园







# [[LeetCode] Shortest Completing Word 最短完整的单词](https://www.cnblogs.com/grandyang/p/8407446.html)








Find the minimum length word from a given dictionary `words`, which has all the letters from the string `licensePlate`. Such a word is said to *complete* the given string `licensePlate`

Here, for letters we ignore case. For example, `"P"` on the `licensePlate` still matches `"p"` on the word.

It is guaranteed an answer exists. If there are multiple answers, return the one that occurs first in the array.

The license plate might have the same letter occurring multiple times. For example, given a `licensePlate` of `"PP"`, the word `"pair"` does not complete the `licensePlate`, but the word `"supper"` does.



Example 1:
Input: licensePlate = "1s3 PSt", words = ["step", "steps", "stripe", "stepple"]
Output: "steps"
Explanation: The smallest length word that contains the letters "S", "P", "S", and "T".
Note that the answer is not "step", because the letter "s" must occur in the word twice.
Also note that we ignored case for the purposes of comparing whether a letter exists in the word.



Example 2:
Input: licensePlate = "1s3 456", words = ["looks", "pest", "stew", "show"]
Output: "pest"
Explanation: There are 3 smallest length words that contains the letters "s".
We return the one that occurred first.



Note:
- `licensePlate` will be a string with length in range `[1, 7]`.
- `licensePlate` will contain digits, spaces, or letters (uppercase or lowercase).
- `words` will have a length in the range `[10, 1000]`.
- Every `words[i]` will consist of lowercase letters, and have length in range `[1, 15]`.




这道题给了我们一个车牌号，还有一些单词，让我们找出包含这个车牌号中所有字母的第一个最短的单词。车牌中的字母有大小写之分，但是单词只是由小写单词组成的，所以需要把车牌号中的所有大写字母都转为小写的，转换方法很简单，ASCII码加上32即可。我们建立车牌中各个字母和其出现的次数之间的映射，同时记录所有字母的个数total，然后遍历所有的单词，对于每个单词都要单独处理，我们遍历单词中所有的字母，如果其在车牌中也出现了，则对应字母的映射减1，同时还需匹配的字母数cnt也自减1，最后遍历字母完成后，如果cnt为0（说明车牌中所有的字母都在单词中出现了），并且结果res为空或长度大于当前单词word的话，更新结果即可，参见代码如下：



解法一：

```
class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        string res = "";
        int total = 0;
        unordered_map<char, int> freq; 
        for (char c : licensePlate) {
            if (c >= 'a' && c <= 'z') {++freq[c]; ++total;}
            else if (c >= 'A' && c <= 'Z') {++freq[c + 32]; ++total;}
        }
        for (string word : words) {
            int cnt = total;
            unordered_map<char, int> t = freq;
            for (char c : word) {
                if (--t[c] >= 0) --cnt;
            }
            if (cnt == 0 && (res.empty() || res.size() > word.size())) {
                res = word;
            }
        }
        return res;
    }
};
```



如果这道题的单词是按长度排序的话，那么上面的方法就不是很高效了，因为其会强制遍历完所有的单词。所以我们考虑给单词排序，博主这里用了TreeMap这个数据结构建立单词长度和包含所有该长度单词的数组之间的映射，其会自动按照单词长度来排序。然后还使用了一个chars数组来记录车牌中的所有字母，这样就可以方便的统计出字母总个数。我们从单词长度等于字母总个数的映射开始遍历，先检验该长度的所有单词。这里检验方法跟上面略有不同，但都大同小异，用一个bool型变量succ，初始化为true，然后建立一个字母和其出现次数的映射，先遍历单词，统计各个字母出现的次数。然后就遍历chars数组，如果chars中某个字母不在单词中，那么succ赋值为false，然后break掉。最后我们看succ，如果仍为true，直接返回当前单词word，之后的单词就不用再检验了，参见代码如下：



解法二：

```
class Solution {
public:
    string shortestCompletingWord(string licensePlate, vector<string>& words) {
        map<int, vector<string>> m;
        vector<char> chars;
        for (string word : words) {
            m[word.size()].push_back(word);
        }
        for (char c : licensePlate) {
            if (c >= 'a' && c <= 'z') chars.push_back(c);
            else if (c >= 'A' && c <= 'Z') chars.push_back(c + 32);
        }
        for (auto a : m) {
            if (a.first < chars.size()) continue;
            for (string word : a.second) {
                bool succ = true;
                unordered_map<char, int> freq;
                for (char c : word) ++freq[c];
                for (char c : chars) {
                    if (--freq[c] < 0) {succ = false; break;}
                }
                if (succ) return word;
            }
        }
        return "";
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/114155/java-c-clean-code](https://discuss.leetcode.com/topic/114155/java-c-clean-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












