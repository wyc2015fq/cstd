# [LeetCode] Top K Frequent Words 前K个高频词 - Grandyang - 博客园







# [[LeetCode] Top K Frequent Words 前K个高频词](https://www.cnblogs.com/grandyang/p/7689927.html)








Given a non-empty list of words, return the *k* most frequent elements.

Your answer should be sorted by frequency from highest to lowest. If two words have the same frequency, then the word with the lower alphabetical order comes first.

Example 1:
Input: ["i", "love", "leetcode", "i", "love", "coding"], k = 2
Output: ["i", "love"]
Explanation: "i" and "love" are the two most frequent words.
    Note that "i" comes before "love" due to a lower alphabetical order.



Example 2:
Input: ["the", "day", "is", "sunny", "the", "the", "the", "sunny", "is", "is"], k = 4
Output: ["the", "is", "sunny", "day"]
Explanation: "the", "is", "sunny" and "day" are the four most frequent words,
    with the number of occurrence being 4, 3, 2 and 1 respectively.



Note:
- You may assume *k* is always valid, 1 ≤ *k* ≤ number of unique elements.
- Input words contain only lowercase letters.



Follow up:
- Try to solve it in *O*(*n* log *k*) time and *O*(*n*) extra space.
- Can you solve it in *O*(*n*) time with only *O*(*k*) extra space?



这道题让我们求前K个高频词，跟之前那道题 [Top K Frequent Elements](http://www.cnblogs.com/grandyang/p/5454125.html) 极其类似，换了个数据类型就又是一道新题。唯一的不同就是之前那道题对于出现频率相同的数字，没有顺序要求。而这道题对于出现频率相同的单词，需要按照字母顺序来排。但是解法都一样，还是用最小堆和桶排序的方法。首先来看最小堆的方法，思路是先建立每个单词和其出现次数之间的映射，然后把单词和频率的pair放进最小堆，如果没有相同频率的单词排序要求，我们完全可以让频率当作pair的第一项，这样priority_queue默认是以pair的第一项为key进行从大到小的排序，而当第一项相等时，又会以第二项由大到小进行排序，这样第一项的排序方式就与题目要求的相同频率的单词要按字母顺序排列不相符，当然我们可以在存入结果res时对相同频率的词进行重新排序处理，也可以对priority_queue的排序机制进行自定义，这里我们采用第二种方法，我们自定义排序机制，我们让a.second > b.second，让小频率的词在第一位，然后当a.second == b.second时，我们让a.first < b.first，这是让字母顺序大的排在前面（这里博主需要强调一点的是，priority_queue的排序机制的写法和vector的sort的排序机制的写法正好顺序相反，同样的写法，用在sort里面就是频率小的在前面，不信的话可以自己试一下）。定义好最小堆后，我们首先统计单词的出现频率，然后组成pair排序最小堆之中，我们只保存k个pair，超过了就把队首的pair移除队列，最后我们把单词放入结果res中即可，参见代码如下：



解法一：

```
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string> res(k);
        unordered_map<string, int> freq;
        auto cmp = [](pair<string, int>& a, pair<string, int>& b) {
            return a.second > b.second || (a.second == b.second && a.first < b.first);
        };
        priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(cmp) > q(cmp);
        for (auto word : words) ++freq[word];
        for (auto f : freq) {
            q.push(f);
            if (q.size() > k) q.pop();
        }
        for (int i = res.size() - 1; i >= 0; --i) {
            res[i] = q.top().first; q.pop();
        }
        return res;
    }
};
```



下面这种解法还是一种堆排序的思路，这里我们用map，来建立次数和出现该次数所有单词的集合set之间的映射，这里也利用了set能自动排序的特性，当然我们还是需要首先建立每个单词和其出现次数的映射，然后将其组成pair放入map种，map是从小到大排序的，这样我们从最后面取pair，就是次数最大的，每次取出一层中所有的单词，如果此时的k大于该层的单词个数，就将整层的单词加入结果res中，否则就取前K个就行了，取完要更更新K值，如果K小于等于0了，就break掉，返回结果res即可，参见代码如下：



解法二：

```
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string> res;
        unordered_map<string, int> freq;
        map<int, set<string>> m;
        for (string word : words) ++freq[word];
        for (auto a : freq) {
            m[a.second].insert(a.first);
        }
        for (auto it = m.rbegin(); it != m.rend(); ++it) {
            if (k <= 0) break;
            auto t = it->second;
            vector<string> v(t.begin(), t.end());
            if (k >= t.size()) {
                res.insert(res.end(), v.begin(), v.end());
            } else {
                res.insert(res.end(), v.begin(), v.begin() + k);
            }
            k -= t.size();
        }
        return res;
    }
};
```



下面这种解法是一种桶排序的思路，我们根据出现次数建立多个bucket，桶的个数不会超过单词的个数，在每个桶中，我们对单词按字符顺序进行排序。我们可以用个数组来表示桶，每一层中放一个集合，利用set的自动排序的功能，使其能按字母顺序排列。我们还是需要首先建立每个单词和其出现次数的映射，然后将其组成pair放入map种，map是从小到大排序的，这样我们倒序遍历所有的桶，这样取pair，就是次数最大的，每次取出一层中所有的单词，如果此时的k大于该层的单词个数，就将整层的单词加入结果res中，否则就取前K个就行了，取完要更更新K值，如果K小于等于0了，就break掉，返回结果res即可，参见代码如下：



解法三：

```
class Solution {
public:
    vector<string> topKFrequent(vector<string>& words, int k) {
        vector<string> res;
        unordered_map<string, int> freq;
        vector<set<string>> v(words.size() + 1, set<string>());
        for (string word : words) ++freq[word];
        for (auto a : freq) {
            v[a.second].insert(a.first);
        }
        for (int i = v.size() - 1; i >= 0; --i) {
            if (k <= 0) break;
            vector<string> t(v[i].begin(), v[i].end());
            if (k >= t.size()) {
                res.insert(res.end(), t.begin(), t.end());
            } else {
                res.insert(res.end(), t.begin(), t.begin() + k);
            }
            k -= t.size();
        }
        return res;
    }
};
```



类似题目：

[Top K Frequent Elements](http://www.cnblogs.com/grandyang/p/5454125.html)

[Design Search Autocomplete System](http://www.cnblogs.com/grandyang/p/7897166.html)



参考资料：

[https://discuss.leetcode.com/topic/106861/o-nlog-k-priority-queue-c-code](https://discuss.leetcode.com/topic/106861/o-nlog-k-priority-queue-c-code)

[https://discuss.leetcode.com/topic/106868/clean-heap-based-solution-o-nlogk-time-and-o-n-space-16ms](https://discuss.leetcode.com/topic/106868/clean-heap-based-solution-o-nlogk-time-and-o-n-space-16ms)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












