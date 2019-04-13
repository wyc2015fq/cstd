
# 290. Word Pattern - OraYang的博客 - CSDN博客

2017年08月05日 11:20:27[OraYang](https://me.csdn.net/u010665216)阅读数：148标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a`pattern`and a string`str`,
 find if`str`follows the same pattern.
Herefollowmeans a full match, such that there is a bijection between a letter in`pattern`and
 anon-emptyword in`str`.
Examples:
pattern ="abba", str ="dog
 cat cat dog"should return true.
pattern ="abba", str ="dog
 cat cat fish"should return false.
pattern ="aaaa", str ="dog
 cat cat dog"should return false.
pattern ="abba", str ="dog
 dog dog dog"should return false.
Notes:
You may assume`pattern`contains only lowercase letters, and`str`contains
 lowercase letters separated by a single space.
思路：本题考察了两个知识点：1.string类分割单词，自己实现了一个函数，很好用；2.哈希表映射，与以前的一片博客很类似：[Isomorphic Strings](http://blog.csdn.net/u010665216/article/details/76397676)
代码：

```python
class Solution {
public:
    bool wordPattern(string pattern, string str) {
        vector<string> s = split(str,' ');
        unordered_map<char,string> temp;
        if(s.size()!=pattern.size())
            return false;
        for(int i =0;i<s.size();i++)
        {
            if(temp.find(pattern[i])==temp.end())
                temp[pattern[i]] = s[i];
            else if(temp[pattern[i]]!=s[i])
                return false;
        }
        unordered_map<string,char> temp_;
        for(int j = 0;j<pattern.size();j++)
        {
           if(temp_.find(s[j]) == temp_.end())
                temp_[s[j]] = pattern[j];
            else if(temp_[s[j]]!=pattern[j])
                return false; 
        }
        return true;
        
        
    }
private:
    vector<string> split(string srcStr, char sp)  
{  
    vector<string> ret;  
    if (srcStr.empty()) return ret;  
    int start = 0;  
    int end = 0;  
  
    while( string::npos != (start = srcStr.find_first_not_of(sp, end)) )  
    {  
        if (string::npos == (end = srcStr.find_first_of(sp, start))) end = srcStr.length();  
        ret.push_back(srcStr.substr(start, (end - start))) ;  
    }  
  
    return ret ;  
}  
};
```


