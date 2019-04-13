
# 520. Detect Capital - OraYang的博客 - CSDN博客

2017年08月23日 10:11:22[OraYang](https://me.csdn.net/u010665216)阅读数：563所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Given a word, you need to judge whether the usage of capitals in it is right or not.
We define the usage of capitals in a word to be right when one of the following cases holds:
All letters in this word are capitals, like "USA".
All letters in this word are not capitals, like "leetcode".
Only the first letter in this word is capital if it has more than one letter, like "Google".
Otherwise, we define that this word doesn't use capitals in a right way.
Example 1:
Input:"USA"Output:True
Example 2:
Input:"FlaG"Output:False
Note:The input will be a non-empty word consisting of uppercase and lowercase latin letters.
思路：本题思路很简单，题目中的限制已经说的很清楚，只要用简单的逻辑判断结构即可
代码：

```python
class Solution {
public:
    bool detectCapitalUse(string word) {
        string::iterator itr = word.begin();
        if(*itr>='a'&&*itr<='z')
        {
            while(itr!=word.end()){
                if(*itr>='A'&&*itr<='Z')
                    return false;
                itr++;
            }
        }
        else if(*itr>='A'&&*itr<='Z')
        {
            itr++;
            if(*itr>='a'&&*itr<='z')
                while(itr!=word.end()){
                    if(*itr>='A'&&*itr<='Z')
                        return false;
                    itr++;
            }
            else 
                while(itr!=word.end()){
                    if(*itr>='a'&&*itr<='z')
                        return false;
                    itr++;
        }
        }
        return true;  
    }
};
```


