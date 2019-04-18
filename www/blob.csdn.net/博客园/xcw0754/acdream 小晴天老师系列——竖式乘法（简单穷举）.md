# acdream  小晴天老师系列——竖式乘法（简单穷举） - xcw0754 - 博客园
# [acdream  小晴天老师系列——竖式乘法（简单穷举）](https://www.cnblogs.com/xcw0754/p/4650791.html)

### 　　　　小晴天老师系列——竖式乘法
Time Limit: 4000/2000MS (Java/Others)    Memory Limit: 128000/64000KB (Java/Others)
#### Problem Description
小晴天是ACdream团队中最牛的老师之一，他最擅长数学运算~这天他翻开一本《AC is not a dream》杂志，发现最后一页有一道很经典的思维题，题目很简单，每个框填写一个数字，构成一个竖式，每个数的最高位不能为0，但是有一些数字被隐藏掉了，然后让你根据没有隐藏的数字填出隐藏的数字。
如下图：
![](http://acdream.info/img/prob/1704/4.png)
然后小晴天二话不说，三下五除二就写出了答案：
![](http://acdream.info/img/prob/1704/3.png)
然后小晴天就觉得这样的题目太简单了，于是问你是否有办法来求出一道题目有多少种不同的答案呢？（只要有一个方框有不同的数字即为不同的答案）
#### Input
多组数据，首先是一个整数t(t<=20)，表示数据组数。
对于每组数据，用5行表示一个竖式，每行均为一个字符串，仅含有星号(*)与数字('0'~'9')组成，其中星号表示空白
其中第一行为长度为3的字符串。
第二行为长度为2的字符串。
第三行为长度为4的字符串。
第四行为长度为3的字符串。
第五行为长度为5的字符串。
#### Output
对于每组数据，输出一个整数x，表示符合乘法竖式法则的填法的种类。
#### Sample Input
2
***
**
3384
846
*****
4**
**
3384
846
*****
#### Sample Output
2
1
#### Hint
样例1，除了题目中的那种情况，还有这种
![](http://acdream.info/img/prob/1704/5.png)
而样例2，因为第一个数的百位被固定为4，故只有一种填法。
思路：第一行和第二行两个数都是比较小的，直接穷举这2个数，再将17格子的所有可能行的结果保存。根据输入17个就直接搜可行的结果匹配。注意5个数字的开头都不能为0。
```
1 /*
  2 * this code is made by xcw0754
  3 * Problem: 1704
  4 * Verdict: Accepted
  5 * Submission Date: 2015-07-16 10:18:37
  6 * Time: 4MS
  7 * Memory: 36836KB
  8 */
  9 //#pragma comment(linker,"/STACK:102400000,102400000")
 10 #include <iostream>
 11 #include <stdio.h>
 12 #include <string.h>
 13 #include <vector>
 14 #include <stack>
 15 #include <algorithm>
 16 #include <map>
 17 #include <bits/stdc++.h>
 18 #define LL long long
 19 #define pii pair<int,int>
 20 #define INF 0x7f7f7f7f
 21 using namespace std;
 22 const int N=500005;
 23 int up;
 24 int s[N][18];
 25  
 26  
 27 void pre_cal()
 28 {
 29     up=0;
 30     for(int i=100; i<1000; i++)
 31     {
 32         for(int j=10; j<100; j++)
 33         {
 34             s[up][0]=i/100; //不能为0
 35             s[up][1]=i/10%10;
 36             s[up][2]=i%10;
 37  
 38             s[up][3]=j/10;  //不能为0
 39             s[up][4]=j%10;
 40  
 41             int tmp=i*(j%10);   //第3行
 42             s[up][5]=tmp/1000;  //不能为0
 43             if(!s[up][5] || s[up][5]>9)  continue;
 44             s[up][6]=tmp/100%10;
 45             s[up][7]=tmp/10%10;
 46             s[up][8]=tmp%10;
 47  
 48             tmp=i*(j/10);       //第4行
 49             s[up][9]=tmp/100;   //不能为0
 50             if(!s[up][9]|| s[up][9]>9)  continue;
 51             s[up][10]=tmp/10%10;
 52             s[up][11]=tmp%10;
 53  
 54             tmp=i*j;            //第5行
 55             s[up][12]=tmp/10000;//不能为0
 56             if(!s[up][12])  continue;
 57             s[up][13]=tmp/1000%10;
 58             s[up][14]=tmp/100%10;
 59             s[up][15]=tmp/10%10;
 60             s[up][16]=tmp%10;
 61             up++;
 62         }
 63     }
 64  
 65 }
 66  
 67 int now[18];
 68 vector<int> alk;
 69 char c[18];
 70  
 71 void get_input()
 72 {
 73     memset(now,0,sizeof(now));
 74     alk.clear();
 75  
 76     scanf("%s",c);
 77     scanf("%s",c+3);
 78     scanf("%s",c+5);
 79     scanf("%s",c+9);
 80     scanf("%s",c+12);
 81  
 82     for(int i=0; i<17; i++)
 83         if(isdigit(c[i]))
 84         {
 85             now[i]=c[i]-'0';
 86             alk.push_back(i);
 87         }
 88 }
 89  
 90  
 91 int cal()
 92 {
 93     int ans=0;
 94     for(int i=0; i<up; i++)
 95     {
 96         int j;
 97         for(j=0; j<alk.size(); j++) //这几个都要匹配
 98         {
 99             int k=alk[j];
100             if(now[k]!=s[i][k]) break;
101         }
102         if(j==alk.size())
103             ans++;
104     }
105     return ans;
106 }
107  
108 int main()
109 {
110     //freopen("input.txt", "r", stdin);
111     pre_cal();
112     int t;
113     scanf("%d",&t);
114     while(t--)
115     {
116         get_input();
117         printf("%d\n",cal());
118     }
119     return 0;
120 }
AC代码
```

