# Substring with Concatenation of All Words——解题报告（窗口移动法） - bigfacesafdasgfewgf - CSDN博客





2015年05月21日 21:44:53[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1028











    【题目】




You are given a string, s, and a list of words, words, that are all of the same length. Find all starting indices of substring(s) in s that is a concatenation
 of each word in wordsexactly once and without any intervening characters.


For example, given:
s: `"barfoothefoobarman"`
words: `["foo", "bar"]`


You should return the indices: `[0,9]`.

(order does not matter).


    【分析】

    本题目解法有两个，第一个是常规思路，第二个是窗口移动法，这个方法需要掌握。




    解法一：先把words存在一个map中，key是单词字符串，value是出现的次数。然后逐个位置遍历字符串s（注意遍历结束位置不必到最后，剩余长度小于单词总长度即停止），判断其后面的和档次总长度相同的子串中的每个单词是否和words一样。如果一样，这push_back进去；否则，遍历下一个字符。时间复杂度：O(LNw)，L是s的长度，N是words的个数，w是word的长度。

    代码：



```cpp
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector <int> res;
        int N = words.size(); // number of words
        if(N == 0)
            return res;
        int len = words[0].length();  // length of each word
        int strLen = s.length();  // length of string
        
        map<string, int> countWords;  // get the wordCount map
        for(int i = 0; i < N; i ++)
        {
            if(countWords.count(words[i]))
                countWords[words[i]]++;
            else
                countWords[words[i]] = 1;
        }
        
        map<string, int> counting;
        for(int i = 0; i <= strLen - len * N; i++)  // first loop
        {
            counting.clear();
            bool flag = true;
            for(int j = i; j < i + N*len; j += len)  // second loop
            {
                string w = s.substr(j, len);
                if(countWords.count(w) == 0)  //  if not exist in countWords, break directly
                {
                    flag = false; 
                    break;
                }
                else
                {
                    if(counting.count(w))  // if not new
                        counting[w]++;
                    else
                        counting[w] = 1;
                }
            }
            if(!flag)
                continue;
            else
            {
                if(compare(counting, countWords))
                    res.push_back(i);
                else
                    continue;
            }
        }
        
    }
    bool compare(map<string, int> counting, map<string, int> countWords)
    {
        map<string, int>::iterator iter;
        for(iter = countWords.begin(); iter != countWords.end(); iter++)
        {
            if(counting[iter->first] != iter->second)
                return false;
        }
        return true;
    }
};
```



**   解法二：窗口移动法。**

**    首先，初始化一个长度为0的窗口，定义头部是begin，尾部是tail。判断tail后面的一个单词，如果在words里面，则tail往后移动，即窗口伸长一个单词的量；如果tail后面的单词压根儿不在words里面，那么把begin后移到最后位置，重新初始化窗口，继续判断；如果tail后面的单词是之前出现过，不过words中没有这个单词的容量，那么begin后移到该单词第一次出现的位置，继续判断。这个方法的时间复杂度为O(Lw)。**

    代码：



```cpp
class Solution {
public:
    vector<int> findSubstring(string S, vector<string> &L) {
        unordered_map<string, int>wordTimes;//L中单词出现的次数
        for(int i = 0; i < L.size(); i++)
            if(wordTimes.count(L[i]) == 0)
                wordTimes.insert(make_pair(L[i], 1));
            else wordTimes[L[i]]++;
        int wordLen = L[0].size();
         
        vector<int> res;
        for(int i = 0; i < wordLen; i++)
        {//为了不遗漏从s的每一个位置开始的子串，第一层循环为单词的长度
            unordered_map<string, int>wordTimes2;//当前窗口中单词出现的次数
            int winStart = i, cnt = 0;//winStart为窗口起始位置,cnt为当前窗口中的单词数目
            for(int winEnd = i; winEnd <= (int)S.size()-wordLen; winEnd+=wordLen)
            {//窗口为[winStart,winEnd)
                string word = S.substr(winEnd, wordLen);
                if(wordTimes.find(word) != wordTimes.end())
                {
                    if(wordTimes2.find(word) == wordTimes2.end())
                        wordTimes2[word] = 1;
                    else wordTimes2[word]++;
                     
                    if(wordTimes2[word] <= wordTimes[word])
                        cnt++;
                    else
                    {//当前的单词在L中，但是它已经在窗口中出现了相应的次数，不应该加入窗口
                     //此时，应该把窗口起始位置想左移动到，该单词第一次出现的位置的下一个单词位置
                        for(int k = winStart; ; k += wordLen)
                        {
                            string tmpstr = S.substr(k, wordLen);
                            wordTimes2[tmpstr]--;
                            if(tmpstr == word)
                            {
                                winStart = k + wordLen;
                                break;
                            }
                            cnt--;
                        }
                    }
                     
                    if(cnt == L.size())
                        res.push_back(winStart);
                }
                else
                {//发现不在L中的单词
                    winStart = winEnd + wordLen;
                    wordTimes2.clear();
                    cnt = 0;
                }
            }
        }
        return res;
    }
};
```


另外还有一个代码，没有调试成功，后续修改ing。



```cpp
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        vector <int> res;
        int N = words.size(); // number of words
        if(N == 0)
            return res;
        int len = words[0].length();  // length of each word
        int strLen = s.length();  // length of string
        
        map<string, int> countWords;  // get the wordCount map
        for(int i = 0; i < N; i ++)
        {
            if(countWords.count(words[i]))
                countWords[words[i]]++;
            else
                countWords[words[i]] = 1;
        }
        
        map<string, int> counting = countWords;
        int begin = 0, tail = begin;
        while(tail < strLen)
        {
            if(tail - begin + 1 == N * len)  // get results
            {
                res.push_back(begin);
                begin = tail + 1;
                tail = begin;
                counting = countWords;
                continue;
            }
            
            string w = s.substr(tail, len);
            
            int kind = moveClass(w, counting);  // get the manner how to move the tail
            
            if(kind == 1)  // if w not in countWords
            {
                begin = tail + 2;
                tail = begin;
                counting = countWords;
                continue;
            }
            if(kind == 2) // if w in countWords now
            {
                tail += len;
                continue;
            }
            if(kind == 3)  // if w in countWords in past
            {
                string tmp = s.substr(begin, tail - begin + 1);
                begin = tmp.find(w) + 1;
                tail = begin;
                counting = countWords;
                continue;
            }
        }
        return res; 
    }
    
    int moveClass(string w, map<string, int>& counting)
    {
        if(counting.count(w) == 0)
            return 1;
        else if(counting[w] >= 1)
        {
            counting[w]--;
            return 2;
        }
        else
            return 3;
    }
};
```


























