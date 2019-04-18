# UVA 489 Hangman Judge （字符匹配） - xcw0754 - 博客园
# [UVA 489 Hangman Judge （字符匹配）](https://www.cnblogs.com/xcw0754/p/4588354.html)
题意：给一个字符串A，只含小写字符数个。再给一个字符串B，含小写字符数个。规则如下：
1、字符串B从左至右逐个字符遍历，对于每个字符，如果该字符在A中存在，将A中所有该字符删掉，若不存在，则错误次数+1。
2、当错误次数达到7时，游戏结束，输了lose。
3、当串A中已经没有字符了，游戏结束，赢了win。（串B后面还没遍历到的也不用遍历了）
4、当错误次数没到达7，但是字符串A还有剩下的字符没消去，则chickened out。
错满7个就输，在满7个之前匹配完了就赢，在满7个之前没匹配完就chickened out。
思路：将串A统计26个字符是否出现（用hash），遍历串B，每次匹配成功就删掉26个字母中对应那个，匹配失败就统计是否到达7次。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 using namespace std;
 4 const int N=129;
 5 int has1[N], num[N];
 6 string s1, s2;
 7 
 8 int cal()   //匹配是-1，大于0小于7是悬着，大于7是挂了
 9 {
10     int cnt1=0, cnt3=0;
11     for(int i=0; i<s1.size(); i++)  has1[s1[i]]++;
12     for(int i='a'; i<='z'; i++)     if(has1[i]>0) cnt1++;  //需要多少个才匹配
13 
14     for(int i=0; i<s2.size(); i++)
15     {
16         if(has1[s2[i]]) //匹配的
17         {
18             has1[s2[i]]=0;
19             cnt1--;     //又匹配了一个
20             if(!cnt1) return -1;    //匹配完了
21         }
22         else    cnt3++; //错误的
23         if(cnt3>=7)     return 10;       //错了7个
24     }
25     return cnt3;
26 }
27 
28 
29 int main()
30 {
31     freopen("e://input.txt","r",stdin);
32     int n;
33     while(scanf("%d",&n),n>0)
34     {
35         printf("Round %d\n",n);
36         memset(has1,0,sizeof(has1));
37         memset(num,0,sizeof(num));
38         cin>>s1>>s2;
39         int tmp=cal();
40         if(tmp==-1)     printf("You win.\n");
41         else if(tmp<7)  printf("You chickened out.\n");
42         else            printf("You lose.\n");
43     }
44     return 0;
45 }
AC代码
```

