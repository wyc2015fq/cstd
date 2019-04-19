# 2016"百度之星" - 资格赛（Astar Round1）  Problem C（字典树） - HJ - CSDN博客
2017年08月03日 08:24:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：254
Problem Description 
度熊手上有一本神奇的字典，你可以在它里面做如下三个操作：
1、insert : 往神奇字典中插入一个单词
2、delete: 在神奇字典中删除所有前缀等于给定字符串的单词
3、search: 查询是否在神奇字典中有一个字符串的前缀等于给定的字符串
Input 
这里仅有一组测试数据。第一行输入一个正整数N(1≤N≤100000)，代表度熊对于字典的操作次数，接下来N行，每行包含两个字符串，中间中用空格隔开。第一个字符串代表了相关的操作（包括： insert, delete 或者 search）。第二个字符串代表了相关操作后指定的那个字符串，第二个字符串的长度不会超过30。第二个字符串仅由小写字母组成。
Output 
对于每一个search 操作，如果在度熊的字典中存在给定的字符串为前缀的单词，则输出Yes 否则输出 No。
Sample Input
5 
insert hello 
insert hehe 
search h 
delete he 
search hello
Sample Output
Yes 
No
用字典树进行增、查、删改三个操作：
```
1）增：开辟新的子节点
2）查：查找道给定字符串末尾为止的节点num值
3）删改：先改变到给定字符串末尾为止的节点之前的所有num值，然后顺着给定字符串查找第一个num为0的节点，删除所有这个节点的叶子节点。
```
```
#include<iostream>
#include<cstring>
using namespace std;
#define maxn 3000005
char a[33],b[33];
int root,cnt;
struct node{
    int num,next[26],fa;
}trie[maxn];
void build(int c,int fa)         //以c为根，fa为父节点，建一棵trie树 
{
    trie[c].num==0;
    trie[c].fa=fa;
    memset(trie[c].next,-1,sizeof(trie[c].next));          //trie树的叶子节点初始值都都赋为-1 
}
void Insert()                      //插入操作 
{
    int l=strlen(a),p=root;
    for(int i=0;i<l;i++)
    {
        int id=a[i]-'a';
        if(trie[p].next[id]==-1)
        {
            trie[p].next[id]=++cnt;
            build(cnt,p);
        } 
        p=trie[p].next[id];
        trie[p].num++;              //每一个节点的num表示在字典树里到这个位置为止的前缀字符相同的字符串的个数 
    }
} 
int Search()                     //查询操作 
{
    int l=strlen(a),p=root;
    for(int i=0;i<l;i++)
    {
        int id=a[i]-'a';
        if(trie[p].next[id]==-1)
          return 0;
        p=trie[p].next[id];
    }
    return trie[p].num;            //返回字典树里字符串的前缀等于a[]的个数 
}
void Delete()                   //删除操作 
{
    int l=strlen(a),p=root;
    for(int i=0;i<l;i++)
    {
        int id=a[i]-'a';
        if(trie[p].next[id]==-1)
          return ;
        p=trie[p].next[id];
    }
    int ans=trie[p].num;               //ans的值表示字典树里字符串的前缀等于a[]的个数 
    while(trie[p].fa!=-1)
    {
        trie[p].num-=ans;              //每次到一个节点就把这个节点保存下来的num值减去ans的值 
        p=trie[p].fa;           //查找父节点，从叶子节点往根节点遍历 
    }
    p=root;                //p重新赋根节点的值 
    for(int i=0;i<l;i++)
    {
        int id=a[i]-'a';
        if(trie[trie[p].next[id]].num==0)       //如果从根开始往下查找找到第一个num值为0的节点 
        {
            trie[p].next[id]=-1;                 //把这个节点和这个节点所有的子节点都删除 
            return ;                         //最后跳出 
        }
        p=trie[p].next[id];
    }
}
int main()
{
    int n;
    cin>>n;
    root=cnt=0;              
    build(root,-1);
    while(n--)
    {
        cin>>b>>a;
        if(b[0]=='i')
          Insert();
        else if(b[0]=='d')
          Delete();
        else if(b[0]=='s') 
        {
            int ans=Search();
            if(ans==0)
              cout<<"No"<<endl;
            else
              cout<<"Yes"<<endl; 
        }
    }
    return 0;
}
```
