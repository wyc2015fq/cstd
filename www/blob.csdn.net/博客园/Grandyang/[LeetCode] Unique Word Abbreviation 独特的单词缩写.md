# [LeetCode] Unique Word Abbreviation 独特的单词缩写 - Grandyang - 博客园







# [[LeetCode] Unique Word Abbreviation 独特的单词缩写](https://www.cnblogs.com/grandyang/p/5220589.html)







An abbreviation of a word follows the form <first letter><number><last letter>. Below are some examples of word abbreviations:
a) it                      --> it    (no abbreviation)

     1
b) d|o|g                   --> d1g

              1    1  1
     1---5----0----5--8
c) i|nternationalizatio|n  --> i18n

              1
     1---5----0
d) l|ocalizatio|n          --> l10n

Assume you have a dictionary and given a word, find whether its abbreviation is unique in the dictionary. A word's abbreviation is unique if no *other* word from the dictionary has the same abbreviation.

Example: 
Given dictionary = [ "deer", "door", "cake", "card" ]

isUnique("dear") -> `false`
isUnique("cart") -> `true`
isUnique("cane") -> `false`
isUnique("make") -> `true`


这道题让我们求独特的单词缩写，但是题目中给的例子不是很清晰，我们来看下面三种情况：

1. dictionary = {"dear"},  isUnique("door") -> false

2. dictionary = {"door", "door"}, isUnique("door") -> true

3. dictionary = {"dear", "door"}, isUnique("door") -> false



从上面三个例子我们可以看出，当缩写一致的时候，字典中的单词均和给定单词相同时，那么返回true。我们需要用哈希表来建立缩写形式和其对应的单词的映射，把所有缩写形式的相同单词放到一个set中，然后我们在判断是否unique的时候只需要看给定单词的缩写形式的set里面该单词的个数是否和set中的元素总数相同，相同的话就是上面的第二种情况，返回true。需要注意的是由于set中不能有重复值，所有上面第二种情况只会有一个door存在set里，但是并不影响判断结果，参见代码如下：



解法一：

```
class ValidWordAbbr {
public:
    ValidWordAbbr(vector<string> &dictionary) {
        for (auto a : dictionary) {
            string k = a[0] + to_string(a.size() - 2) + a.back();
            m[k].insert(a);
        }
    }
    bool isUnique(string word) {
        string k = word[0] + to_string(word.size() - 2) + word.back();
        return m[k].count(word) == m[k].size();
    }
private:
    unordered_map<string, set<string>> m;
};
```



如果我们想省一些空间，也可以不用set，那么我们如何区分上面的第二和第三种情况呢，我们在遇到哈希表中没有当前缩写形式的时候，将该缩写形式和当前单词建立映射，如果该缩写形式应经存在，那么我们看如果映射的单词不是当前单词，我们将映射单词改为空字符串，这样做的原因是，在对于第三种情况dictionary = {"dear", "door"}时，遍历dear时，建立d2r和dear的映射，当遍历到door的时候，由于door和dear不同，我们将映射改为d2r和“”映射，而对于第二种情况 dictionary = {"door", "door"}，保留d2r和door的映射，那么这样在判断door是否unique时，就可以区别第二种和第三种情况了，参见代码如下：



解法二：

```
class ValidWordAbbr {
public:
    ValidWordAbbr(vector<string> &dictionary) {
        for (auto a : dictionary) {
            string k = a[0] + to_string(a.size() - 2) + a.back();
            if (m.find(k) != m.end() && m[k] != a) m[k] = "";
            else m[k] = a;
        }
    }
    bool isUnique(string word) {
        string k = word[0] + to_string(word.size() - 2) + word.back();
        return m.find(k) == m.end() || m[k] == word;
    }
private:
    unordered_map<string, string> m;
};
```



参考资料：

[https://leetcode.com/discuss/80720/7-lines-concise-and-easy-understand-c-solution](https://leetcode.com/discuss/80720/7-lines-concise-and-easy-understand-c-solution)

[https://leetcode.com/discuss/86526/let-me-explain-the-question-with-better-examples](https://leetcode.com/discuss/86526/let-me-explain-the-question-with-better-examples)

[https://leetcode.com/discuss/71652/java-solution-with-hashmap-string-string-beats-submissions](https://leetcode.com/discuss/71652/java-solution-with-hashmap-string-string-beats-submissions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












