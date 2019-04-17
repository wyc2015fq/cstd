# [LeetCode] Find And Replace in String 在字符串中查找和替换 - Grandyang - 博客园







# [[LeetCode] Find And Replace in String 在字符串中查找和替换](https://www.cnblogs.com/grandyang/p/10352323.html)







To some string `S`, we will perform some replacement operations that replace groups of letters with new ones (not necessarily the same size).

Each replacement operation has `3` parameters: a starting index `i`, a source word `x` and a target word `y`.  The rule is that if `x` starts at position `i` in the original string `S`, then we will replace that occurrence of `x` with `y`.  If not, we do nothing.

For example, if we have `S = "abcd"` and we have some replacement operation `i = 2, x = "cd", y = "ffff"`, then because `"cd"` starts at position `2` in the original string `S`, we will replace it with `"ffff"`.

Using another example on `S = "abcd"`, if we have both the replacement operation `i = 0, x = "ab", y = "eee"`, as well as another replacement operation `i = 2, x = "ec", y = "ffff"`, this second operation does nothing because in the original string `S[2] = 'c'`, which doesn't match `x[0] = 'e'`.

All these operations occur simultaneously.  It's guaranteed that there won't be any overlap in replacement: for example, `S = "abc", indexes = [0, 1], sources = ["ab","bc"]` is not a valid test case.

Example 1:
Input: S = "abcd", indexes = [0,2], sources = ["a","cd"], targets = ["eee","ffff"]
Output: "eeebffff"
Explanation: "a" starts at index 0 in S, so it's replaced by "eee".
"cd" starts at index 2 in S, so it's replaced by "ffff".

Example 2:
Input: S = "abcd", indexes = [0,2], sources = ["ab","ec"], targets = ["eee","ffff"]
Output: "eeecd"
Explanation: "ab" starts at index 0 in S, so it's replaced by "eee". 
"ec" doesn't starts at index 2 in the original S, so we do nothing.

Notes:
- `0 <= indexes.length = sources.length = targets.length <= 100`
- `0 < indexes[i] < S.length <= 1000`
- All characters in given inputs are lowercase letters.



这道题给了我们一个字符串S，并给了一个坐标数组，还有一个源字符串数组，还有目标字符串数组，意思是若某个坐标位置起，源字符串数组中对应位置的字符串出现了，将其替换为目标字符串。题目真的是好长，但好在给了两个例子可以帮助我们很好的理解题意。此题的核心操作就两个，查找和替换，需要注意的是，由于替换操作会改变原字符串，但是我们查找始终是基于最初始的S，比如例子2中，当完成了第一次替换后，S变为了 "eeecd"，好像此时 "ec" 出现了，但仍然不能替换，因为一切查找都是基于最原始的那个S。那么正向的替换可能会产生这样的问题，我们注意到题目中有个限制条件，就是说不会有重叠产生，比如 "abc"，如果让在0位置上查找 "ab" 了，就不会让在1位置上查找 "bc"，这样的话，其实我们可以从后往前开始查找替换，因为不会有重叠，所以后面替换了的字符不会影响到前面。首先我们需要给indexes数组排个序，因为可能不是有序的，但是却不能直接排序，这样会丢失和sources，targets数组的对应关系，这很麻烦。所以我们新建了一个保存pair对儿的数组，将indexes数组中的数字跟其位置坐标组成pair对儿，加入新数组v中，然后给这个新数组按从大到小的方式排序，这样我们既排了序，又保存了对应关系，岂不美哉！

下面就要开始遍历新数组v了，对于遍历到的pair对儿，取出第一个数字，保存到i，表示S中需要查找的位置，取出第二个数字，然后根据这个位置分别到sources和targets数组中取出源字符串和目标字符串，然后我们在S中的i位置，向后取出和源字符串长度相同的子串，然后比较，若正好和源字符串相等，则将其替换为目标字符串即可，参见代码如下：



解法一：

```
class Solution {
public:
    string findReplaceString(string S, vector<int>& indexes, vector<string>& sources, vector<string>& targets) {
        vector<pair<int, int>> v;
        for (int i = 0; i < indexes.size(); ++i) {
            v.push_back({indexes[i], i});
        }
        sort(v.rbegin(), v.rend());
        for (auto a : v) {
            int i = a.first;
            string s = sources[a.second], t = targets[a.second];
            if (S.substr(i, s.size()) == s) {
                S = S.substr(0, i) + t + S.substr(i + s.size());
            }
        }
        return S;
    }
};
```



我们也可以使用TreeMap来代替需要排序的数组，由于TreeMap默认的是最小堆，而我们需要的是最大堆，只要在定义上加一个greater就行了，其他部分基本没有任何的区别，参见代码如下：



解法二：

```
class Solution {
public:
    string findReplaceString(string S, vector<int>& indexes, vector<string>& sources, vector<string>& targets) {
        map<int, int, greater<int>> m;
        for (int i = 0; i < indexes.size(); ++i) {
            m[indexes[i]] = i;
        }
        for (auto a : m) {
            int i = a.first;
            string s = sources[a.second], t = targets[a.second];
            if (S.substr(i, s.size()) == s) {
                S = S.substr(0, i) + t + S.substr(i + s.size());
            }
        }
        return S;
    }
};
```



再来看一种稍有不同的解法，之前的两种解法都是直接在S上替换，这里我们新建一个结果res字符串，这里还是使用HashMap来保存映射对，但是稍有不同的是，我们并不是无脑的添加所有的映射对儿，而是先做个check，只要当发现可以查找到源字符串位置的时候，才添加映射对儿，这样就排除了所有不能替换的情况。然后我们遍历原字符串S，对于每个遍历到的位置，我们都到HashMap中查找，如果发现需要替换，我们就把目标字符串提取出来，加入结果res中，注意此时i也需要加上源字符串的长度。若不需要替换，则直接将字符加入结果res中，然后i移动到下一个位置，参见代码如下：



解法三：

```
class Solution {
public:
    string findReplaceString(string S, vector<int>& indexes, vector<string>& sources, vector<string>& targets) {
        string res = "";
        unordered_map<int, int> m;
        for (int i = 0; i < indexes.size(); ++i) {
            if (S.substr(indexes[i], sources[i].size()) == sources[i]) {
                m[indexes[i]] = i;
            }        
        }
        for (int i = 0; i < S.size();) {
            if (m.count(i)) {
                res += targets[m[i]];
                i += sources[m[i]].size();
            } else {
                res.push_back(S[i]);
                ++i;
            }
        }
        return res;
    }
};
```



我们也可以使用STL自带的replace函数来完成替换操作，有点偷懒啊～



解法四：

```
class Solution {
public:
    string findReplaceString(string S, vector<int>& indexes, vector<string>& sources, vector<string>& targets) {
        map<int, pair<int, string>, greater<int>> m;
        for (int i = 0; i < indexes.size(); ++i) {
            if (S.substr(indexes[i], sources[i].size()) == sources[i]) {
                m[indexes[i]] = {sources[i].size(), targets[i]};
            }        
        }
        for (auto a : m) {
            S.replace(a.first, a.second.first, a.second.second);
        }
        return S;
    }
};
```



参考资料：

[https://leetcode.com/problems/find-and-replace-in-string/](https://leetcode.com/problems/find-and-replace-in-string/)

[https://leetcode.com/problems/find-and-replace-in-string/discuss/134758/Java-O(n)-solution](https://leetcode.com/problems/find-and-replace-in-string/discuss/134758/Java-O(n)-solution)

[https://leetcode.com/problems/find-and-replace-in-string/discuss/130577/C%2B%2B-5-lines-6-ms-bucket-sort-O(n)](https://leetcode.com/problems/find-and-replace-in-string/discuss/130577/C%2B%2B-5-lines-6-ms-bucket-sort-O(n))

[https://leetcode.com/problems/find-and-replace-in-string/discuss/130587/C%2B%2BJavaPython-Replace-S-from-right-to-left](https://leetcode.com/problems/find-and-replace-in-string/discuss/130587/C%2B%2BJavaPython-Replace-S-from-right-to-left)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












