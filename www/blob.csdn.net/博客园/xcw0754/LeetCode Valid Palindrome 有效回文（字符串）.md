# LeetCode  Valid Palindrome 有效回文（字符串） - xcw0754 - 博客园
# [LeetCode  Valid Palindrome 有效回文（字符串）](https://www.cnblogs.com/xcw0754/p/4084072.html)
```
1 class Solution {
 2 public:
 3     bool isPalindrome(string s) {
 4     if(s=="")    return true;
 5     if(s.length()==1)    return true;    //单个字符，对称
 6     char *p,*q;
 7     p=&s[0];                //p指向开头
 8     q=&s[s.length()-1];        //q指向末尾
 9     while(p!=q){
10                 //测试字符串里是否有字母或数字，若没有，则立刻返回
11         while(    (*p<'0'    ||    (*p>'9'&&*p<'A')    ||    (*p>'Z'&&*p<'a')    ||    *p>'z')&&p!=q){        
12             p++;
13         }
14         while(    (*q<'0'    ||    (*q>'9'&&*q<'A')    ||    (*q>'Z'&&*q<'a')    ||    *q>'z')&&p!=q)    //非字母和数字，跳过
15             q--;
16         if(*q>='A'&&*q<='Z')    //若大写，转为小写
17             *q=*q+32;
18         if(*p>='A'&&*p<='Z')    //若大写，转为小写
19             *p=*p+32;
20         if(p==q)
21             break;
22         if(*p==*q){
23             p++;
24             if(p==q)
25                 break;
26             q--;
27         }
28         else
29             return false;
30     }
31     return true;
32     }
33 };
```
题意：
`"A man, a plan, a canal: Panama"` is a palindrome.是回文
`"race a car"` is *not* a palindrome.非回文
回文:即将字符串倒过来之后和原来仍一样。如：did=did
但是，此题要求过滤掉非数字和字母的其他字符，而且不区分大小写，A和a是一样的。
思路：用两个指针，分别指向字符串的头和尾，每次判断要过滤掉无效的字符。
注意：要考虑空串（即没有字母和数字，可能只有空格，标点什么的），只有1个字符的字符串。还得考虑两个指针指向同一个地址时即已经是回文了。

