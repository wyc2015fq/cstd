
# 383. Ransom Note - OraYang的博客 - CSDN博客

2017年08月08日 10:05:32[OraYang](https://me.csdn.net/u010665216)阅读数：103标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given an arbitrary ransom note string and another string containing letters from all the magazines, write a function that will return true if the ransom note can be constructed from the magazines ; otherwise, it will return false.
Each letter in the magazine string can only be used once in your ransom note.
Note:
You may assume that both strings contain only lowercase letters.
canConstruct("a", "b") -> false
canConstruct("aa", "ab") -> false
canConstruct("aa", "aab") -> true思路：本题题目描述很长，但是总结起来就是一句话：寻找两个字符串中的交叉项（保留重复值），代码思路与前面一篇博客：Intersection
 of Two ArraysII惊人相似
代码：

```python
class Solution {
public:
    bool canConstruct(string ransomNote, string magazine) {  
        for(int j = 0;j<ransomNote.size();j++)  
        {  
            string::iterator itr = find(magazine.begin(),magazine.end(),ransomNote[j]);  
            if(itr!=magazine.end())  
            {  
                magazine.erase(itr);           
            }
            else
                return false;
                  
        }  
        return true;  
        
    }
};
```


