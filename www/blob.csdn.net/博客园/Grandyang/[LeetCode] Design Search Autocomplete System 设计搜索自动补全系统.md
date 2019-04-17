# [LeetCode] Design Search Autocomplete System 设计搜索自动补全系统 - Grandyang - 博客园







# [[LeetCode] Design Search Autocomplete System 设计搜索自动补全系统](https://www.cnblogs.com/grandyang/p/7897166.html)







Design a search autocomplete system for a search engine. Users may input a sentence (at least one word and end with a special character `'#'`). For each character they type except '#', you need to return the top 3historical hot sentences that have prefix the same as the part of sentence already typed. Here are the specific rules:
- The hot degree for a sentence is defined as the number of times a user typed the exactly same sentence before.
- The returned top 3 hot sentences should be sorted by hot degree (The first is the hottest one). If several sentences have the same degree of hot, you need to use ASCII-code order (smaller one appears first).
- If less than 3 hot sentences exist, then just return as many as you can.
- When the input is a special character, it means the sentence ends, and in this case, you need to return an empty list.

Your job is to implement the following functions:

The constructor function:

`AutocompleteSystem(String[] sentences, int[] times):` This is the constructor. The input is historical data. `Sentences` is a string array consists of previously typed sentences. `Times` is the corresponding times a sentence has been typed. Your system should record these historical data.

Now, the user wants to input a new sentence. The following function will provide the next character the user types:

`List<String> input(char c):` The input `c` is the next character typed by the user. The character will only be lower-case letters (`'a'` to `'z'`), blank space (`' '`) or a special character (`'#'`). Also, the previously typed sentence should be recorded in your system. The output will be the top 3 historical hot sentences that have prefix the same as the part of sentence already typed.



Example:
Operation: AutocompleteSystem(["i love you", "island","ironman", "i love leetcode"], [5,3,2,2]) 
The system have already tracked down the following sentences and their corresponding times: 
`"i love you"` : `5` times 
`"island"` : `3` times 
`"ironman"` : `2` times 
`"i love leetcode"` : `2` times 
Now, the user begins another search: 

Operation: input('i') 
Output: ["i love you", "island","i love leetcode"] 
Explanation: 
There are four sentences that have prefix `"i"`. Among them, "ironman" and "i love leetcode" have same hot degree. Since `' '` has ASCII code 32 and `'r'` has ASCII code 114, "i love leetcode" should be in front of "ironman". Also we only need to output top 3 hot sentences, so "ironman" will be ignored. 

Operation: input(' ') 
Output: ["i love you","i love leetcode"] 
Explanation: 
There are only two sentences that have prefix `"i "`. 

Operation: input('a') 
Output: [] 
Explanation: 
There are no sentences that have prefix `"i a"`. 

Operation: input('#') 
Output: [] 
Explanation: 
The user finished the input, the sentence `"i a"` should be saved as a historical sentence in system. And the following input will be counted as a new search. 



Note:
- The input sentence will always start with a letter and end with '#', and only one blank space will exist between two words.
- The number of complete sentences that to be searched won't exceed 100. The length of each sentence including those in the historical data won't exceed 100.
- Please use double-quote instead of single-quote when you write test cases even for a character input.
- Please remember to RESET your class variables declared in class AutocompleteSystem, as static/class variables are persisted across multiple test cases. Please see [here](https://leetcode.com/faq/#different-output) for more details.



这道题让我们实现一个简单的搜索自动补全系统，我们用谷歌或者百度进行搜索时，会有这样的体验，输入些单词，搜索框会弹出一些以你输入为开头的一些完整的句子供你选择，这就是一种搜索自动补全系统。根据题目的要求，补全的句子是按之前出现的频率排列的，高频率的出现在最上面，如果频率相同，就按字母顺序来显示。输入规则是每次输入一个字符，然后返回自动补全的句子，如果遇到井字符，表示完整句子结束。那么我们肯定需要一个哈希map，建立句子和其出现频率的映射，还需要一个字符串data，用来保存之前输入过的字符。在构造函数中，给了我们一些句子，和其出现的次数，那么我们就直接将其加入哈希map，然后data初始化为空字符串。在input函数中，我们首先判读输入字符是否为井字符，如果是的话，那么表明当前的data字符串已经是一个完整的句子，在哈希表中次数加1，并且data清空，返回空集。否则的话我们将当前字符加入data字符串中，现在就要找出包含data前缀的前三高频句子了，我们使用优先队列来做，设计的思路是，始终用优先队列保存频率最高的三个句子，那么我们就应该把频率低的或者是字母顺序大的放在队首，以便随时可以移出队列，所以应该是个最小堆，队列里放句子和其出现频率的pair，并且根据其频率大小进行排序，所以我们要重写优先队列的comparator。然后我们遍历哈希表中的所有句子，我们首先要验证当前data字符串是否是其前缀，没啥好的方法，就逐个字符比较，用标识符matched，初始化为true，如果发现不匹配，则matched标记为false，并break掉。然后判断如果matched为true的话，说明data字符串是前缀，那么就把这个pair加入优先队列中，如果此时队列中的元素大于三个，那把队首元素移除，因为我们设计的是最小堆，所以频率小的句子会被先移除。然后就是将优先队列的元素加到结果res中，由于先出队列的是频率小的句子，所以要加到结果res的末尾，参见代码如下：



```
class AutocompleteSystem {
public:
    AutocompleteSystem(vector<string> sentences, vector<int> times) {
        for (int i = 0; i < sentences.size(); ++i) {
            freq[sentences[i]] += times[i]; 
        }
        data = "";
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            ++freq[data];
            data = "";
            return {};
        }
        data.push_back(c);
        auto cmp = [](pair<string, int>& a, pair<string, int>& b) {
            return a.second > b.second || (a.second == b.second && a.first < b.first);
        };
        priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(cmp) > q(cmp);
        for (auto f : freq) {
            bool matched = true;
            for (int i = 0; i < data.size(); ++i) {
                if (data[i] != f.first[i]) {
                    matched = false;
                    break;
                }
            }
            if (matched) {
                q.push(f);
                if (q.size() > 3) q.pop();
            }
        }
        vector<string> res(q.size());
        for (int i = q.size() - 1; i >= 0; --i) {
            res[i] = q.top().first; q.pop();
        }
        return res;
    }
    
private:
    unordered_map<string, int> freq;
    string data;
};
```



类似题目：

[Implement Trie (Prefix Tree)](http://www.cnblogs.com/grandyang/p/4491665.html)

[Top K Frequent Words](http://www.cnblogs.com/grandyang/p/7689927.html)



参考资料：

[https://leetcode.com/problems/design-search-autocomplete-system/](https://leetcode.com/problems/design-search-autocomplete-system/)

[https://discuss.leetcode.com/topic/96090/straight-forward-hash-table-priority-queue-solution-in-c-no-trie](https://discuss.leetcode.com/topic/96090/straight-forward-hash-table-priority-queue-solution-in-c-no-trie)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












