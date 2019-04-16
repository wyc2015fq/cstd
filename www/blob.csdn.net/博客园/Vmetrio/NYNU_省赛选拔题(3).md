# NYNU_省赛选拔题(3) - Vmetrio - 博客园







# [NYNU_省赛选拔题(3)](https://www.cnblogs.com/wangmengmeng/p/5271845.html)





## 题目描述


二叉树，若其与自己的镜像完全相同，就称其为镜像树。

![](http://xcacm.hfut.edu.cn/upload/201505/tree1.png)

是一棵镜像树；
而

![](http://xcacm.hfut.edu.cn/upload/201505/tree2.png)

不是镜像树。
现给你一棵二叉树，请你判断其是不是镜像树。


## 输入

第一行是一个整数数T，表示测试数据有多少组
每组数据第一行是一个正整数n(1<=n<=100)，表示二叉树中节点的数量
下面n行，每行有三个正整数a b c(1<=a<=100,0<=b,c<=100)，表示以编号a的节点为父节点，它的左孩子节点编号为b，右孩子节点编号为c，若b=0表示没有左孩子节点，c=0表示没有右孩子节点，树的根节点是编号为1的节点，节点的编号都>=1（保证数据中给出的二叉树拓扑结构是合法的）
下面一行是n个正整数vi(1<=vi<=100)，表示编号为i的节点的值。

## 输出

若数据中表示的二叉树是镜像树，输出“Yes”，否则输出“No”，每个输出单独占一行




## 样例输入
2
7
1 2 3
2 4 5
3 6 7
4 0 0
5 0 0
6 0 0
7 0 0
1 2 2 3 4 4 3
5
1 2 3
2 0 4
3 0 5
4 0 0
5 0 0
1 2 2 3 3

## 样例输出
Yes
No







```
1 #include <iostream>
 2 using namespace std;
 3 bool flag=1;
 4 struct node
 5 {
 6     int data;
 7     int lchild,rchild;
 8 
 9 }t[150];
10 void dfs(int r1,int r2)
11 {
12     if(r1==0&&r2==0)
13         return ;
14     else if(r1==0&&r2!=0||r1!=0&&r2==0||t[r1].data!=t[r2].data)
15     {
16         flag=0;
17         return ;
18     }
19     dfs(t[r1].lchild,t[r2].rchild);
20     dfs(t[r1].rchild,t[r2].lchild);
21 }
22 int main()
23 {
24     int i,j,T,n;
25     cin>>T;
26     while(T--)
27     {
28         cin>>n;
29         for(i=1;i<=n;i++)
30         {
31             int k;
32             cin>>k;
33             cin>>t[i].lchild>>t[i].rchild;
34         }
35         for(i=1;i<=n;i++)
36             cin>>t[i].data;    
37         flag=1;
38         dfs(t[1].lchild,t[1].rchild);
39         if(flag)
40             cout<<"Yes"<<endl;
41         else
42             cout<<"No"<<endl;
43     }
44 }
```














