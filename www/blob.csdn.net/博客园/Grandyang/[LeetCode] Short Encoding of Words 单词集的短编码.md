# [LeetCode] Short Encoding of Words 单词集的短编码 - Grandyang - 博客园







# [[LeetCode] Short Encoding of Words 单词集的短编码](https://www.cnblogs.com/grandyang/p/10089792.html)







Given a list of words, we may encode it by writing a reference string `S` and a list of indexes `A`.

For example, if the list of words is `["time", "me", "bell"]`, we can write it as `S = "time#bell#"` and `indexes = [0, 2, 5]`.

Then for each index, we will recover the word by reading from the reference string from that index until we reach a `"#"` character.

What is the length of the shortest reference string S possible that encodes the given words?

Example:
Input: words = `["time", "me", "bell"]`
Output: 10
Explanation: S = `"time#bell#" and indexes = [0, 2, 5`].



Note:
- `1 <= words.length <= 2000`.
- `1 <= words[i].length <= 7`.
- Each word has only lowercase letters.



这道题给了我们一个单词数组，让我们对其编码，不同的单词之间加入#号，每个单词的起点放在一个坐标数组内，终点就是#号，能合并的单词要进行合并，问输入字符串的最短长度。题意不难理解，难点在于如何合并单词，我们观察题目的那个例子，me和time是能够合并的，只要标清楚其实位置，time的起始位置是0，me的起始位置是2，那么根据#号位置的不同就可以顺利的取出me和time。需要注意的是，如果me换成im，或者tim的话，就不能合并了，因为我们是要从起始位置到#号之前所有的字符都要取出来。搞清楚了这一点之后，我们在接着观察，由于me是包含在time中的，所以我们处理的顺序应该是先有time#，然后再看能否包含me，而不是先生成了me#之后再处理time，所以我们可以得出结论，应该先处理长单词，那么就给单词数组按长度排序一下就行，自己重写一个comparator就行。然后我们遍历数组，对于每个单词，我们都在编码字符串查找一下，如果没有的话，直接加上这个单词，再加一个#号，如果有的话，就可以得到出现的位置。比如在time#中查找me，得到found=2，然后我们要验证该单词后面是否紧跟着一个#号，所以我们直接访问found+word.size()这个位置，如果不是#号，说明不能合并，我们还是要加上这个单词和#号。最后返回编码字符串的长度即可，参见代码如下：



解法一：

```
class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        string str = "";
        sort(words.begin(), words.end(), [](string& a, string& b){return a.size() > b.size();});
        for (string word : words) {
            int found = str.find(word);
            if (found == string::npos || str[found + word.size()] != '#') {
                str += word + "#";
            }
        }
        return str.size();
    }
};
```



我们再来看一种不用自定义comparator的方法，根据之前的分析，我们知道其实是在找单词的后缀，比如me就是time的后缀。我们希望将能合并的单词排在一起，比较好处理，而后缀又不好排序。那么我们就将其转为前缀，做法就是给每个单词翻转一下，time变成emit，me变成em，这样我们只要用默认的字母顺序排，就可以得到em，emit的顺序，那么能合并的单词就放到一起了，而且一定是当前的合并到后面一个，那么就好做很多了。我们只要判读当前单词是否是紧跟着的单词的前缀，是的话就加0，不是的话就要加上当前单词的长度并再加1，多加的1是#号。判断前缀的方法很简单，直接在后面的单词中取相同长度的前缀比较就行了。由于我们每次都要取下一个单词，为了防止越界，只处理到倒数第二个单词，那么就要把最后一个单词的长度加入结果res，并再加1即可，参见代码如下：



解法二：

```
class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        int res = 0, n = words.size();
        for (int i = 0; i < n; ++i) reverse(words[i].begin(), words[i].end());
        sort(words.begin(), words.end());
        for (int i = 0; i < n - 1; ++i) {
            res += (words[i] == words[i + 1].substr(0, words[i].size())) ? 0  : words[i].size() + 1;
        }
        return res + words.back().size() + 1;
    }
};
```



接下来的这种方法也很巧妙，用了一个HashSet，将所有的单词先放到这个HashSet中。原理是对于每个单词，我们遍历其所有的后缀，比如time，那么就遍历ime，me，e，然后看HashSet中是否存在这些后缀，有的话就删掉，那么HashSet中的me就会被删掉，这样保证了留下来的单词不可能再合并了，最后再加上每个单词的长度到结果res，并且同时要加上#号的长度，参见代码如下：



解法三：

```
class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        int res = 0;
        unordered_set<string> st(words.begin(), words.end());
        for (string word : st) {
            for (int i = 1; i < word.size(); ++i) {
                st.erase(word.substr(i));
            }
        }
        for (string word : st) res += word.size() + 1;
        return res;
    }
};
```



参考资料：

[https://leetcode.com/problems/short-encoding-of-words/](https://leetcode.com/problems/short-encoding-of-words/)

[https://leetcode.com/problems/short-encoding-of-words/discuss/125825/Easy-to-understand-Java-solution](https://leetcode.com/problems/short-encoding-of-words/discuss/125825/Easy-to-understand-Java-solution)

[https://leetcode.com/problems/short-encoding-of-words/discuss/125822/C%2B%2B-4-lines-reverse-and-sort](https://leetcode.com/problems/short-encoding-of-words/discuss/125822/C%2B%2B-4-lines-reverse-and-sort)

[https://leetcode.com/problems/short-encoding-of-words/discuss/125811/C%2B%2BJavaPython-Easy-Understood-Solution-with-Explanation](https://leetcode.com/problems/short-encoding-of-words/discuss/125811/C%2B%2BJavaPython-Easy-Understood-Solution-with-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












