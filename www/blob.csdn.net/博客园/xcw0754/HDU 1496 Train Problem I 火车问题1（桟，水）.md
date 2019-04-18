# HDU 1496 Train Problem I 火车问题1（桟，水） - xcw0754 - 博客园
# [HDU 1496 Train Problem I 火车问题1（桟，水）](https://www.cnblogs.com/xcw0754/p/4550508.html)
题意：
　　给出两个串，串中的数字i 代表编号为i的火车进入车站的顺序，车站如桟一样，先进后出。第二个串是火车出站的顺序，问若按照第一个串那样进站，是否有可能如第二个串一样的出站顺序？火车顶多9辆，即1～9。
思路：
　　用桟模拟，每进入一辆火车就加到桟顶，判断第二个串中开头是否是此车，若是，两者都删掉，若不是，继续添加火车进桟，继续判断。直到判断匹配了，将桟顶出桟，串头删除，继续匹配桟顶和串头，直到不匹配了，继续进桟。这样一直反复，直到最后，桟中一定为空，第二个串也一定为空。以此判断是否符合条件。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int a, b, c, d;
 4 int has1[1000006];
 5 int has2[1000006];
 6 int qq[105][105][105][105];
 7 
 8 
 9 int cal()
10 {
11     memset(has1,0,sizeof(has1));
12     memset(has2,0,sizeof(has2));
13     int ans=0;
14     for(int x1=1; x1<=100; x1++)
15         for(int x2=1; x2<=100; x2++)
16         {
17             int tmp=a*x1*x1+ b*x2*x2;
18             if( tmp>=0 )
19                 has1[tmp]++;
20             else
21                 has2[-tmp]++;
22         }
23 
24 
25     for(int x3=1; x3<=100; x3++)
26     {
27         for(int x4=1; x4<=100; x4++)
28         {
29             int tmp=c*x3*x3+d*x4*x4;
30             if(tmp>0)
31                 ans+=has2[tmp];    
32             else
33                 ans+=has1[-tmp];    
34         }
35     }
36     return (ans<<4);
37 }
38 
39 
40 int main()
41 {
42     //freopen("input.txt", "r", stdin);
43     while(~scanf("%d%d%d%d",&a,&b,&c,&d))
44     {
45     
46         if(a>0 && b>0 && c>0 && d>0 || a<0 && b<0 && c<0 && d<0)  
47         {  
48                 printf("0\n");  
49                 continue;  
50         }  
51     
52 
53         if(!qq[a+50][b+50][c+50][d+50])
54         {
55             qq[a+50][b+50][c+50][d+50]=cal()+1;
56         }
57         printf("%d\n",qq[a+50][b+50][c+50][d+50]-1);
58     }
59 
60     return 0;
61 }
AC代码
```

