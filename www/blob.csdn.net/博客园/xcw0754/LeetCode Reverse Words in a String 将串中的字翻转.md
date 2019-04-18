# LeetCode Reverse Words in a String 将串中的字翻转 - xcw0754 - 博客园
# [LeetCode Reverse Words in a String 将串中的字翻转](https://www.cnblogs.com/xcw0754/p/4127275.html)
```
1 class Solution {
 2 public:
 3     void reverseWords(string &s) {
 4             string end="",tem="";
 5             char *p=&s[0];
 6             while(*p!='\0'){
 7                 while(*p==' ')                //过滤多余的空格,针对串头
 8                     p++;
 9                 while(*p!=' '&&*p!='\0'){    //积累一个单词，存于临时串
10                     tem=tem+*p;
11                     p++;
12                 }
13                 while(*p==' ')                //过滤多余的空格，针对串尾
14                     p++;
15                 if(*p!='\0')        //最后一个字不用加空格
16                     tem=' '+tem;
17                 end=tem+end;
18                 tem="";            //临时字符串清空
19             }
20             s=end;
21     }
22 };
```
题意：将字符串中的字按反序排列，每个字中间有一个空格，串前和串尾无空格。字的顺序不用改变，改变的是字在串中的顺序。
思路：过滤串的前面和后面的空格，用指针从前往后扫， 再用一个临时串保存字，满一个字的时候就添加在将最终的串的前面。扫完该串就将最终的串赋给s。

