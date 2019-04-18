# hihoCoder #1049 : 后序遍历 - xcw0754 - 博客园
# [hihoCoder #1049 : 后序遍历](https://www.cnblogs.com/xcw0754/p/4458314.html)
题意：
　　提供前序遍历结果，和中序遍历结果。输出后序遍历结果。最多26个节点，即从‘A’～‘Z’。
思路：
　　递归建立树，再递归后续遍历。前序遍历的结果就是做了一次DFS的结果，所以可以从左到右顺序来遍历前序序列，每遇到1个字母就以其为一个节点，建立以它为根的子树，直到该子树建立完成为止。怎么判断所建立的这个节点是叶子节点来断定结束了呢？这要依靠中序序列了，中序中每个字母都能把一段子序列分开两半，左边是左子树，右边是右子树。那么操作在中序序列上，再以前序序列作为辅助。
注：一开始把前序起个名字为prev，结果编译错误，改为pre就过了。
```
1 #include <iostream>
 2 #include <string>
 3 #include <cstdio>
 4 using namespace std;
 5 string pre, medi;
 6 int point;
 7 struct node
 8 {
 9     char c;
10     node *left;
11     node *right;
12 };
13 
14 node *createnode(char c)
15 {
16     node *p=(node*)new(node);
17     p->left=0;
18     p->right=0;
19     p->c=c;
20     return p;
21 }
22 node* DFS(int a,int cur,int b)    // cur是根。在区间[a,b]中，cur将其分为两棵子树进行递归。
23 {
24     if(a==b)
25         return createnode(medi[cur]);
26     node *p=createnode(medi[cur]);    //先创建当前节点
27     if(cur-a!=0)    //找左孩子
28     {
29         point++;
30         p->left = DFS(a, medi.find(pre[point]), cur-1);
31     }
32     if(b-cur!=0)    //找右孩子
33     {
34         point++;
35         p->right = DFS(cur+1, medi.find(pre[point]), b);
36     }
37     return p;   //返回整个子树
38 }
39 void sao(node *p)   //后续遍历树
40 {
41     if(p->left)
42         sao(p->left);
43     if(p->right)
44         sao(p->right);
45     cout<< p->c ;
46 }
47 int main()
48 {
49     //freopen("input.txt", "r", stdin);
50     cin>> pre>> medi;
51     sao( DFS(0, medi.find(pre[0]), pre.size()-1) );
52     cout<<endl;
53     return 0;
54 }
AC代码
```
看了大神的代码是没有建立树，直接递归来输出的。于是自己实现了一下，很OK！
```
1 #include <iostream>
 2 #include <string>
 3 using namespace std;
 4 
 5 string pre, medi;
 6 int point;    //重点在这个变量，用于从左到右遍历前序序列的。
 7 void DFS(int a,int cur,int b)    // cur是根。在区间[a,b]中，cur将其分为两棵子树进行递归。
 8 {
 9     if(a==b) 
10     {
11         cout<<medi[cur];
12         return;
13     }
14     if(cur-a!=0)
15         DFS(a, medi.find(pre[++point]), cur-1);
16     if(b-cur!=0)
17         DFS(cur+1, medi.find(pre[++point]), b);
18     cout<<medi[cur];
19 }
20 int main()
21 {
22     cin>> pre>> medi;
23     DFS(0, medi.find( pre[0] ), pre.size()-1);
24     return 0;
25 }
更短更易懂的AC代码
```

