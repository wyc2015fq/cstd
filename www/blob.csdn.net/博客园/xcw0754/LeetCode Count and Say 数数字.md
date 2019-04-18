# LeetCode  Count and Say 数数字 - xcw0754 - 博客园
# [LeetCode  Count and Say 数数字](https://www.cnblogs.com/xcw0754/p/4084088.html)
```
1 class Solution {
 2 public:
 3     string countAndSay(int n) {
 4     if(n==1)    return "1";
 5     string str0="",str1="1";
 6     int i,t,count;
 7     char c='*';
 8     for(i=0;i<n-1;i++){    //一共要数n-1次,假如n=2，那么只要数str1这一次就行了
 9         count=1;
10         if(i%2!=0){    //i为奇数，数str0
11             c=str0[0];
12             str1="";
13             while(c!='\0'){    //将str0 转到str1
14                 t=1;
15                 if(str0[count]!='\0'){
16                     while(str0[count]==c){    //判断前一个字符串里有多少个同样的，记录为count+1次
17                         count++;    //记录读出串的位置
18                         t++;        //记录相同字的个数
19                     }
20                     str1=str1+char(t+'0');    //记录到str1中
21                     str1=str1+c;
22                     c=str0[count++];
23                 }
24                 else{
25                     str1=str1+char(t+'0');    //记录到str1中
26                     str1=str1+c;
27                     c='\0';
28                 }
29             }
30         }
31         else{    //i为偶数，数str1
32             c=str1[0];
33             str0="";
34             while(c!='\0'){    //将str0 转到str1
35                 t=1;
36                 if(str1[count]!='\0'){
37                     while(str1[count]==c){    //判断前一个字符串里有多少个同样的，记录为count+1次
38                         count++;    //记录读出串的位置
39                         t++;        //记录相同字的个数
40                     }
41                     str0=str0+char(t+'0');    //记录到str1中
42                     str0=str0+c;
43                     c=str1[count++];
44                 }
45                 else{
46                     str0=str0+char(t+'0');    //记录到str1中
47                     str0=str0+c;
48                     c='\0';
49                 }
50             }
51         }
52     }
53     if(n%2==0)
54         return str0;
55     else
56         return str1;
57  }
58 };
```
题意：
`1, 11, 21, 1211, 111221, ...`
`1` is read off as `"one 1"` or `11`.
`11` is read off as `"two 1s"` or `21`.
`21` is read off as `"one 2`, then `one 1"` or `1211`.
Given an integer *n*, generate the *n*th sequence.
当n=1时，返回1；
当n=2时，返回11；　　//数n-1时所返回的数，就是1个1的意思
当n=3时，返回21；　　//也就是数n=2时所要返回的数，就是两个1；
当n=4时，返回1211；  //可以看到n=3时返回的是21，有一个2和一个1，即1211   
。。。。。
类推下去，格式是这样子的     [多少个][什么样的字符][多少个][什么样的字符]。。。。
思路：
用两个字符串，从n=1开始数，完了存到一个字符串，这个字符串的内容就是n=2所需要返回的。再数这个字符串里的东西，存到另一个字符串。
控制好数的次数就行了，问题不大。

