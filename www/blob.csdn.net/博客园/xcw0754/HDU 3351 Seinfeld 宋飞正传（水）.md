# HDU 3351 Seinfeld 宋飞正传（水） - xcw0754 - 博客园
# [HDU 3351 Seinfeld 宋飞正传（水）](https://www.cnblogs.com/xcw0754/p/4262653.html)
题意：
　　给出一个串，串内只有大括号，问经过几次改变可使全部括号合法？改变指的是可以将某一方向的括号变成另一方向。
思路：
　　利用栈的特点，若出现成对的合法括号，直接删掉，留下那些不合法的成为一串。既然不合法，那么必须改变，既然要改变，至少得1次吧？那就是使两个非法括号（这里指的是两个一半的括号，即左左，右右，右左）成为合法至少需要一次改变，那好！总共需要几次改变？就看剩下来的那些不合法的括号有多少半了，非法的也肯定是偶数个的。
留下的可能存在3种情况：
　　1、右括号在前，左括号在后。例如：}}{{{{。设左括号个数为left，右为right。这两个数为偶数时，结果为(left+right)/2。还可能存在奇数的情况，此时结果为(left+right)/2+1，比如}}}{{{{{，有8个括号，右3个，左5个，只需要将第1个括号反转，就能消除第1和第2个，对于左括号也一样，最后只剩下 }{ 这两个，需要转2次才行。
　　2、只有左括号。那么结果为left/2。
　　3、只有右括号。那么结果为right/2。
```
1 #include <iostream>
 2 #include <stack>
 3 #include <cstring>
 4 #define N 2010
 5 using namespace std;
 6 char str[N];
 7 int process()
 8 {
 9     int i=0,left=0,right=0;
10     int len=strlen(str);
11     stack<bool> bo;
12     while(i<len)
13     {
14         if(str[i++]=='{')
15         {
16             bo.push(0);
17             left++;
18         }
19         else
20         {
21             if(bo.empty()==false&&bo.top()==0)
22             {
23                 bo.pop();    //删除必须保证栈非空且栈顶是左括号
24                 left--;
25             }
26             else
27             {
28                 bo.push(1);    //1代表右括号
29                 right++;
30             }
31         }
32     }
33     if(left%2==0)    return ((left>>1) + (right>>1));
34     else    return ((left>>1) + (right>>1) +2);
35 }
36 
37 int main()
38 {
39     int num=0;
40     while(scanf("%s",str),str[0]!='-')
41     {
42         printf("%d",++num);        //格式
43         printf(". %d\n",process());
44     }
45     return 0;
46 }
3351
```

