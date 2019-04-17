# [LeetCode] Custom Sort String 自定义排序的字符串 - Grandyang - 博客园







# [[LeetCode] Custom Sort String 自定义排序的字符串](https://www.cnblogs.com/grandyang/p/9190143.html)







`S` and `T` are strings composed of lowercase letters. In `S`, no letter occurs more than once.

`S` was sorted in some custom order previously. We want to permute the characters of `T` so that they match the order that `S` was sorted. More specifically, if `x` occurs before `y` in `S`, then `x` should occur before `y` in the returned string.

Return any permutation of `T` (as a string) that satisfies this property.
Example :
Input: 
S = "cba"
T = "abcd"
Output: "cbad"
Explanation: 
"a", "b", "c" appear in S, so the order of "a", "b", "c" should be "c", "b", and "a". 
Since "d" does not appear in S, it can be at any position in T. "dcba", "cdba", "cbda" are also valid outputs.



Note:
- `S` has length at most `26`, and no character is repeated in `S`.
- `T` has length at most `200`.
- `S` and `T` consist of lowercase letters only.



这道题给了我们两个字符串S和T，让我们将T按照S的顺序进行排序，就是说在S中如果字母x在字母y之前，那么排序后的T中字母x也要在y之前，其他S中未出现的字母位置无所谓。那么我们其实关心的是S中的字母，只要按S中的字母顺序遍历，对于遍历到的每个字母，如果T中有的话，就先排出来，这样等S中的字母遍历完了，再将T中剩下的字母加到后面即可。所以我们先用HashMap统计T中每个字母出现的次数，然后遍历S中的字母，只要T中有，就将该字母重复其出现次数个，加入结果res中，然后将该字母出现次数重置为0。之后再遍历一遍HashMap，将T中其他字母加入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    string customSortString(string S, string T) {
        string res = "";
        unordered_map<char, int> m;
        for (char c : T) ++m[c];
        for (char c : S) {
            res += string(m[c], c);
            m[c] = 0;
        }
        for (auto a : m) {
            res += string(a.second, a.first);
        }
        return res;
    }
};
```



下面这种解法的思路和上面的一样，只不过这里没有使用HashMap，而是使用了一个长度为26的数组，因为题目中说了S和T中都是小写的字母，其他部分没有啥太大的区别，参见代码如下：



解法二：

```
class Solution {
public:
    string customSortString(string S, string T) {
        string res = "";
        vector<int> cnt(26, 0);
        for (char c : T) ++cnt[c - 'a'];
        for (char c : S) {
            while (cnt[c - 'a']-- > 0) res.push_back(c);
        }
        for (char c : T) {
            while (cnt[c - 'a']-- > 0) res.push_back(c);
        }
        return res;
    }
};
```



下面这种方法可以说是简洁的让人发指啊，就两行搞定碉堡了。我们自定义了sort的排序的排序方式，对于字符串T中的任意两个字母a和b，按照其在S中的顺序排序，在S中排前面的在T中也排前面，完全符合题意，所以就能很好的work，参见代码如下：



解法三：

```
class Solution {
public:
    string customSortString(string S, string T) {
        sort(T.begin(), T.end(), [&](char a, char b) {return S.find(a) < S.find(b);});
        return T;
    }
};
```



下面这种解法没有用到STL中内置的find函数，而是用了HashMap来建立S中每个字母和其出现位置之间的映射，这样在自定义排序方法的时候，就可以直接从HashMap中取位置了，参见代码如下：



解法四：

```
class Solution {
public:
    string customSortString(string S, string T) {
        unordered_map<char, int> m;
        for (int i = 0; i < S.size(); ++i) {
            m[S[i]] = i + 1;
        }
        sort(T.begin(), T.end(), [&](char a, char b) {return m[a] < m[b];});
        return T;
    }
};
```



类似题目：

[https://leetcode.com/problems/custom-sort-string/solution/](https://leetcode.com/problems/custom-sort-string/solution/)

[https://leetcode.com/problems/custom-sort-string/discuss/116556/Two-Lines-C++](https://leetcode.com/problems/custom-sort-string/discuss/116556/Two-Lines-C++)

[https://leetcode.com/problems/custom-sort-string/discuss/116615/Java-5-ms-10-line-counting-solution-with-comment](https://leetcode.com/problems/custom-sort-string/discuss/116615/Java-5-ms-10-line-counting-solution-with-comment)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












