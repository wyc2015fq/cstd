# Matrix  (二维树状数组) - Vmetrio - 博客园







# [Matrix  (二维树状数组)](https://www.cnblogs.com/wangmengmeng/p/5515366.html)






Description


Given an N*N matrix A, whose elements are either 0 or 1. A[i, j] means the number in the i-th row and j-th column. Initially we have A[i, j] = 0 (1 <= i, j <= N). 

We can change the matrix in the following way. Given a rectangle whose upper-left corner is (x1, y1) and lower-right corner is (x2, y2), we change all the elements in the rectangle by using "not" operation (if it is a '0' then change it into '1' otherwise change it into '0'). To maintain the information of the matrix, you are asked to write a program to receive and execute two kinds of instructions. 

1. C x1 y1 x2 y2 (1 <= x1 <= x2 <= n, 1 <= y1 <= y2 <= n) changes the matrix by using the rectangle whose upper-left corner is (x1, y1) and lower-right corner is (x2, y2). 
2. Q x y (1 <= x, y <= n) querys A[x, y]. 




Input


The first line of the input is an integer X (X <= 10) representing the number of test cases. The following X blocks each represents a test case. 

The first line of each block contains two numbers N and T (2 <= N <= 1000, 1 <= T <= 50000) representing the size of the matrix and the number of the instructions. The following T lines each represents an instruction having the format "Q x y" or "C x1 y1 x2 y2", which has been described above. 




Output


For each querying output one line, which has an integer representing A[x, y]. 

There is a blank line between every two continuous test cases. 




Sample Input

1
2 10
C 2 1 2 2
Q 2 2
C 2 1 2 1
Q 1 1
C 1 1 2 1
C 1 2 1 2
C 1 1 2 2
Q 1 1
C 1 1 2 1
Q 2 1




Sample Output

1
0
0
1




题解:这个PDF讲的挺好->   浅谈信息学竞赛中的“0 ” 和 “1” ——二进制思想在信息学竞赛中的应用 



代码:

```
1 #include <stdio.h>
 2 #include <string.h>
 3 #include <math.h>
 4 #include <algorithm>
 5 #include <iostream>
 6 #include <ctype.h>
 7 #include <iomanip>
 8 #include <queue>
 9 #include <stdlib.h>
10 using namespace std;
11 
12 int a[1111][1111];
13 int n,m;
14 char s[10];
15 
16 int lowbit(int x)
17 {
18     return x & (-x);
19 }
20 
21 int get(int x,int y)   
22 {
23     int sum=0;
24     for(int i=x; i>0; i-=lowbit(i)){
25         for(int j=y; j>0; j-=lowbit(j)){
26             sum+=a[i][j];
27         }
28     }
29     return sum;
30 } 
31 
32 void add(int x,int y)
33 {
34     for(int i=x; i<=n; i+=lowbit(i)){
35         for(int j=y; j<=n; j+=lowbit(j)){
36             a[i][j]++;
37         }
38     }
39 }
40 
41 int main()
42 {
43     int t;
44     scanf("%d",&t);
45     while(t--){
46         scanf("%d%d",&n,&m);
47         memset(a,0,sizeof(a));
48         while(m--){
49             scanf("%s",s);
50             if(s[0]=='C'){  
51                 int x1,y1,x2,y2;  
52                 scanf("%d%d%d%d",&x1,&y1,&x2,&y2);  
53                 add(x2+1,y2+1);  
54                 add(x1,y1);  
55                 add(x1,y2+1);  
56                 add(x2+1,y1);  
57             }  
58             else{
59                 int x,y;
60                 scanf("%d%d",&x,&y);
61                 printf("%d\n",get(x,y)%2);
62             }
63         }
64         if(t)
65             printf("\n");
66     }
67     return 0;
68 }
```














