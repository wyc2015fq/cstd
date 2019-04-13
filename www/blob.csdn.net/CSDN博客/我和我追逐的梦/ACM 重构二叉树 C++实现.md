
# ACM 重构二叉树 C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月02日 23:53:16[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1907


重构二叉树
Description
根据输入的二叉树前序和中序遍历序列重构二叉树，输出对应节点的左右子节点。
输入：
第一行是一个整数N（1<=N<=20），表示有多少个测试例子。以下每个测试例子的第一行是本测试例子的二叉树的前序遍历，第二行是中序遍历，第三行首先是一个整数M，表示要求输出结果的数目，以后有M个节点，每个中间由一个空格隔开。
输出：
每行输出一个例子的所有结果，如果其子节点为空则输出字符\#，同一例子的不同节点的输出结果之间用一个空格隔开
Sample Input
1
ABCDEF
CBEDFA
3 A B C
Sample Output
B\# CD \#\#
//C++实现
\#include<iostream>
using namespace std;
char str1[100],str2[100],s[100];
int c[100],d[100];
typedef struct trees
{
int value;
int k;
};
trees tree[100];
void insert(int cc,int i)
{
if(c[cc]<tree[i].k)
{
if(tree[2*i].value=='\#')
{
tree[2*i].value=cc;
tree[2*i].k=c[cc];
d[cc]=2*i;
}
else insert(cc,2*i);
}
else
{
if(tree[2*i+1].value=='\#')
{
tree[2*i+1].value=cc;
tree[2*i+1].k=c[cc];
d[cc]=2*i+1;
}
else insert(cc,2*i+1);
}
}
int main()
{
int cases;
cin>>cases;
int lena,lenb;
while(cases--)
{
int n;
scanf("%s %s",str1,str2);
scanf("%d",&n);
lena=strlen(str1);
lenb=strlen(str2);
int i;
for(i=0;i<100;i++)
tree[i].value='\#';
for(i=0;i<n;i++)
{
scanf(" %c",&s[i]);
}
for(i=0;i<lenb;i++)
{
c[str2[i]]=i+1;
}
tree[1].value=str1[0];
tree[1].k=c[str1[0]];         //用来比较的,str2中的位置
d[str1[0]]=1;                 //每个字符的存储位置
for(i=1;i<lena;i++)
insert(str1[i],1);
int k;
for(i=0;i<n;i++)
{
k=d[s[i]];
printf("%c%c ",tree[2*k].value,tree[2*k+1].value);
}
cout<<endl;
}
return 0;
}


