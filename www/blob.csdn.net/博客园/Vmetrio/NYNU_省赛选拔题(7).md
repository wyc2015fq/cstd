# NYNU_省赛选拔题(7) - Vmetrio - 博客园







# [NYNU_省赛选拔题(7)](https://www.cnblogs.com/wangmengmeng/p/5271878.html)





## 题目描述


In computer science, a binary tree is a tree data structure in which each node has at most two children. Consider an infinite full binary tree (each node has two children except the leaf nodes) defined as follows. For a node labelled v its left child will be labelled 2 * v and its right child will be labelled 2 * v + 1. The root is labelled as 1.



You are given n queries of the form i, j. For each query, you have to print the length of the shortest path between node labelled i and node labelled j.




## 输入


First line contains n(1 ≤ n ≤ 10^5), the number of queries. Each query consists of two space separated integers i and j(1 ≤ i, j ≤ 10^9) in one line.


## 输出


For each query, print the required answer in one line


## 样例输入
2
1024 2048
3214567 9998877
## 样例输出
1
44




```
1 #include <iostream>  
 2 using namespace std;  
 3 int f[10000];  
 4 int len;    
 5 void pre()  
 6 {
 7     f[0]=1;
 8     for(int i=1;;i++){
 9         f[i]=f[i-1]*2;          
10         if(f[i]>1e9){
11             len=i-1; 
12             break; 
13         }  
14     } 
15 }  
16 int a,b;
17 int t;
18 int main()
19 {
20     pre();
21     while(cin>>t)
22     while(t--){
23         cin>>a>>b;
24         if(a>b)swap(a,b);
25         int cnt=0;
26         int l;
27         for(int i=0;i<len;i++){
28             if(a>=f[i]&&a<f[i+1]){
29                 l=i;break;}
30         }          
31         while(1)          {
32             if(b>=f[l]&&b<f[l+1])
33             break;              
34             b/=2;              
35             cnt++;          
36         }          
37         while(a!=b){
38             a/=2;        
39             b/=2;              
40             cnt+=2;          
41         }          
42         cout<<cnt<<endl;      
43     }      
44     return 0;  
45 }
```














