# LeetCode  Length of Last Word 最后一个字的长度 - xcw0754 - 博客园
# [LeetCode  Length of Last Word 最后一个字的长度](https://www.cnblogs.com/xcw0754/p/4126584.html)
```
1 class Solution {
 2 public:
 3     int lengthOfLastWord(const char *s) {
 4         if(s=="")    return 0;
 5         string snew=s;
 6         int n=0,len=strlen(s);
 7         char *p=&snew[len-1];
 8         while(*p==' '&&len!=0){
 9             *p--;
10             len--;
11         }
12         while(*p!=' '&&len!=0){
13             n++;
14             p--;
15             len--;
16         }
17         return n;
18     }
19 };
```
题意：给一个数组，以空格来判断是否字的结束与开始。一个词的前后都是空格，中间无空格。返回最后一个字的长度。
思路：给的是一个不可修改的字符串，为了方便，创建另外的指针。若最后是空格，先将后面的空格过滤，从后开始寻找第一个不是空格的字符，开始计数，继续往前数，直到有空格或者已经扫完字符串为止。
吐槽：感觉snew可以省略的，直接利用s就行。但是语法忘了~

