
# 125. Valid Palindrome - OraYang的博客 - CSDN博客

2017年07月25日 15:50:17[OraYang](https://me.csdn.net/u010665216)阅读数：112标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.
For example,
`"A man, a plan, a canal: Panama"`is a palindrome.
`"race a car"`isnota palindrome.
Note:
Have you consider that the string might be empty? This is a good question to ask during an interview.
For the purpose of this problem, we define empty string as valid palindrome.
思路：
本题思路简单，判断字符串是否为回文，属于面试经典题目，步骤如下：
1. 将字符串标点去除；
2. 将字符串中空格去除；
3. 将字符串小写字母转换为大写字母，统一表示
4. 建立两个指针指向字符串首尾，依次比较
代码：

```python
class Solution {
public:
    bool isPalindrome(string s) {
        if(s.size()==0)
            return true;
            
        string::iterator itr = s.begin();
        while(itr!=s.end())
        {
            if(ispunct(*itr))
            {
                s.erase(itr);
                continue;
            }
            else if(*itr==' ')
            {
                s.erase(itr);
                continue;
            }
            itr++;
        }
        for(int i =0;i<s.size();i++)
        {
            if(s[i]>='a')
                s[i] -= 32;
        }
            
        string::iterator begin = s.begin();
        
        string::iterator end = s.end();
        end--;
        for(;begin<end;begin++,end--)
        {
            if(*begin!=*end)
            return false;
        }
        
        return true;
     
    }
};
```




