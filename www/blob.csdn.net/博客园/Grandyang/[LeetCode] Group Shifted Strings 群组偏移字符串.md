# [LeetCode] Group Shifted Strings 群组偏移字符串 - Grandyang - 博客园







# [[LeetCode] Group Shifted Strings 群组偏移字符串](https://www.cnblogs.com/grandyang/p/5204770.html)












Given a string, we can "shift" each of its letter to its successive letter, for example: `"abc" -> "bcd"`. We can keep "shifting" which forms the sequence:
"abc" -> "bcd" -> ... -> "xyz"
Given a list of strings which contains only lowercase alphabets, group all strings that belong to the same shifting sequence.

For example, given: `["abc", "bcd", "acef", "xyz", "az", "ba", "a", "z"]`, 
Return:
[
  ["abc","bcd","xyz"],
  ["az","ba"],
  ["acef"],
  ["a","z"]
]


Note: For the return value, each *inner* list's elements must follow the lexicographic order.








这道题让我们重组偏移字符串，所谓偏移字符串，就是一个字符串的每个字符按照字母顺序表偏移相同量得到的另一个字符串，两者互为偏移字符串，注意相同字符串是偏移字符串的一种特殊情况，因为偏移量为0。现在给了我们一堆长度不同的字符串，让我们把互为偏移字符串的归并到一起，我最开始想的是建立字符度和该长度的所有偏移字符串的映射，但是很明显的错误是相同长度的不一定都是偏移字符串，比如'ab'和'ba‘，所以只能用哈希表来建立一个字符串和所有和此字符串是偏移字符串的集合之间的映射，由于题目要求结果是按字母顺序的，所以用multiset来保存结果，一来可以保存重复字符串，二来可以自动排序。然后我还写了一个判断二个字符串是否互为偏移字符串的函数，注意在比较两个字母距离时采用了加26，再对26取余的trick。我们遍历给定字符串集，对于遍历到的字符串，我们再遍历哈希表，和每个关键字调用isShifted函数来比较，如果互为偏移字符串，则加入其对应的字符串集，并标记flag，最后遍历完哈希表，没有跟任何关键字互为偏移，那么就新建一个映射，最后要做的就是把multiset转换为vector即可，参见代码如下：



解法一：

```
// Correct but complicated
class Solution {
public:
    vector<vector<string>> groupStrings(vector<string>& strings) {
        vector<vector<string> > res;
        unordered_map<string, multiset<string>> m;
        for (auto a : strings) {
            bool b = false;
            for (auto it = m.begin(); it != m.end(); ++it) {
                if (isShifted(it->first, a)) {
                    it->second.insert(a);
                    b = true;
                }
            }
            if (!b) m[a] = {a};
        }
        for (auto it = m.begin(); it != m.end(); ++it) {
            res.push_back(vector<string>(it->second.begin(), it->second.end()));
        }
        return res;
    }
    bool isShifted(string s1, string s2) {
        if (s1.size() != s2.size()) return false;
        int diff = (s1[0] + 26 - s2[0]) % 26;
        for (int i = 1; i < s1.size(); ++i) {
            if ((s1[i] + 26 - s2[i]) % 26 != diff) return false;
        }
        return true;
    }
};
```



上面那个方法挺复杂的，其实有更好的方法，网友的智慧无穷啊，上面那个方法的不高效之处在于对于每个遍历到的字符串，都要和哈希表中所有的关键字都比较一次，而其实我们可以更加巧妙的利用偏移字符串的特点，那就是字符串的每个字母和首字符的相对距离都是相等的，比如abc和efg互为偏移，对于abc来说，b和a的距离是1，c和a的距离是2，对于efg来说，f和e的距离是1，g和e的距离是2。再来看一个例子，az和yx，z和a的距离是25，x和y的距离也是25(直接相减是-1，这就是要加26然后取余的原因)，那么这样的话，所有互为偏移的字符串都有个unique的距离差，我们根据这个来建立映射就可以很好的进行单词分组了，这个思路真实太赞了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<string>> groupStrings(vector<string>& strings) {
        vector<vector<string> > res;
        unordered_map<string, multiset<string>> m;
        for (auto a : strings) {
            string t = "";
            for (char c : a) {
                t += to_string((c + 26 - a[0]) % 26) + ",";
            }
            m[t].insert(a);
        }
        for (auto it = m.begin(); it != m.end(); ++it) {
            res.push_back(vector<string>(it->second.begin(), it->second.end()));
        }
        return res;
    }
};
```



参考资料：

[https://leetcode.com/discuss/70907/c-concise-solution-using-unordered_map](https://leetcode.com/discuss/70907/c-concise-solution-using-unordered_map)

[https://leetcode.com/discuss/81897/12-lines-java-solution-with-explanation](https://leetcode.com/discuss/81897/12-lines-java-solution-with-explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












