# [CareerCup] 11.2 Sort Anagrams Array 异位词数组排序 - Grandyang - 博客园







# [[CareerCup] 11.2 Sort Anagrams Array 异位词数组排序](https://www.cnblogs.com/grandyang/p/4882024.html)







11.2 Write a method to sort an array of strings so that all the anagrams are next to each other.



这道题让我们给一个字符串数组排序，让所有的变位词Anagrams排在一起，关于变位词，LeetCode里有两道相关的题目[Anagrams 错位词](http://www.cnblogs.com/grandyang/p/4385822.html)和[Valid Anagram 验证变位词](http://www.cnblogs.com/grandyang/p/4694988.html)。那么对于这道题，我们有两种方法可以实现，先来看第一种方法，来重写sort中的比较函数compare，参见代码如下：



解法一：

```
bool cmp(const string &a, const string &b) {
    string m = a, n = b;
    sort(m.begin(), m.end());
    sort(n.begin(), n.end());
    return !m.compare(n);
}

sort(array.begin(), array.end(), cmp);
```



另一种解法较为复杂一些，用到了哈希表来建立排序后的字符串和其所有的异位词集合的映射，最后在按集合填充原数组，参见代码如下：



解法二：

```
class Solution {
public:
    void sortArray(vector<string> &array) {
        unordered_map<string, vector<string> > m;
        for (auto &a : array) {
            string key = a;
            sort(key.begin(), key.end());
            if (m.find(key) == m.end()) {
                m[key] = vector<string>();
            }
            vector<string> &v = m[key];
            v.push_back(a);
        }
        int idx = 0;
        for (unordered_map<string, vector<string> >::iterator it = m.begin(); it != m.end(); ++it) {
            for (auto &a : it->second) {
                array[idx++] = a;
            }
        }
    }
};
```














