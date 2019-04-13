
# 345. Reverse Vowels of a String - OraYang的博客 - CSDN博客

2017年08月07日 08:58:51[OraYang](https://me.csdn.net/u010665216)阅读数：136标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Write a function that takes a string as input and reverse only the vowels of a string.
Example 1:
Given s = "hello", return "holle".
Example 2:
Given s = "leetcode", return "leotcede".
Note:
The vowels does not include the letter "y".
思路：首先要明确元音字母：‘a','e','i','o','u';'A','E','I','O','U'，很多人容易忽视大写的元音字母
1.维护两个指针，i，j
2.首尾遍历，但i,j均指向元音字母时，进行互换
3.i>j,返回
代码：

```python
class Solution {
public:
    string reverseVowels(string s) {
        int i,j;
        i =0;
        j = s.size()-1;
        while(i<j){
            while(i<j&&s[i]!='a'&&s[i]!='e'&&s[i]!='i'&&s[i]!='o'&&s[i]!='u'&&s[i]!='A'&&s[i]!='E'&&s[i]!='I'&&s[i]!='O'&&s[i]!='U')
                i++;
            while(i<j&&s[j]!='a'&&s[j]!='e'&&s[j]!='i'&&s[j]!='o'&&s[j]!='u'&&s[j]!='A'&&s[j]!='E'&&s[j]!='I'&&s[j]!='O'&&s[j]!='U')
                j--;
            swap(&s[i],&s[j]);
            i++;
            j--;
                
        }
        return s;
    }
private:
    void swap(char* a,char* b)
    {
        char temp = *a;
        *a = *b;
        *b = temp ;    
    }
};
```


