# hdu  1754  I Hate It（线段树） - HJ - CSDN博客
2016年08月18日 16:14:30[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：442
Problem Description 
很多学校流行一种比较的习惯。老师们很喜欢询问，从某某到某某当中，分数最高的是多少。 
这让很多学生很反感。
不管你喜不喜欢，现在需要你做的是，就是按照老师的要求，写一个程序，模拟老师的询问。当然，老师有时候需要更新某位同学的成绩。
Input 
本题目包含多组测试，请处理到文件结束。 
在每个测试的第一行，有两个正整数 N 和 M ( 0大于N小于等于200000,0大于M小于5000 )，分别代表学生的数目和操作的数目。 
学生ID编号分别从1编到N。 
第二行包含N个整数，代表这N个学生的初始成绩，其中第i个数代表ID为i的学生的成绩。 
接下来有M行。每一行有一个字符 C (只取’Q’或’U’) ，和两个正整数A，B。 
当C为’Q’的时候，表示这是一条询问操作，它询问ID从A到B(包括A,B)的学生当中，成绩最高的是多少。 
当C为’U’的时候，表示这是一条更新操作，要求把ID为A的学生的成绩更改为B。
Output 
对于每一次询问操作，在一行里面输出最高成绩。
Sample Input
5 6 
1 2 3 4 5 
Q 1 5 
U 3 6 
Q 3 4 
Q 4 5 
U 2 9 
Q 1 5
Sample Output
5 
6 
5 
9
Hint 
Huge input,the C function scanf() will work better than cin
线段树的基础题，关于线段树的相关知识，可以查看下面这个链接：
[http://blog.csdn.net/metalseed/article/details/8039326](http://blog.csdn.net/metalseed/article/details/8039326)
这个题目是单点更新，可以算的上线段树入门题。
线段树支持动态范围最值问题，主要作用是对一串数据通过建树来进行对其中一个或者一串数字查询、更新，求和等等。线段树类似于平衡二叉树，与平衡二叉树不同的地方的是线段树需要维护信息，最好的习惯是开一个结构体数组，把需要维护的信息放进去，如下：
```
struct node
{
    int left,right;
    int min(max/val/sum);
}tree[N*3]
```
注意，线段树是用空间换时间，在构造结构体数组的时候，大小需要达到平常给出数据的3~4倍。
一般的，线段树支持下面三种数据操作：
```
Update(x,v):把A(x)修改为v
Query(L,R):计算子序列A(L),A(L+1),...,A(R)的元素和、最小值和最大值
Add(L,R,v):把A(L),A(L+1),...,A(R)的值全部增加v
```
时间复杂度均为O（logn）
首先，最基础的是要学会建树。
线段树的思想是把一串数据以二分的思想层层分级，让每个数据都找到叶子节点，然后根据平衡二叉树的特性，每个节点的左右两个孩子节点的下标跟节点的关系是二倍和二倍加一的关系，迅速查询每一个数据。
建树代码如下：
```
void build(int l,int r,int root)    //l,r表示当前一串数据的左右两个端点下标值，root表示根节点
{
    ss[root].ll=l;    
    ss[root].rr=r;
    ss[root].max=0;          //在数据没有更新或者修改之前赋初值为0
    if(l==r) return ; //如果只有一个数据，代表当前递归已经到了叶子节点，则直接返回，不需要建树了
    int mid=(l+r)/2;         //否则二分数据长度，分别对左右数据继续建子树
    build(l,mid,root*2);          //建立左子树
    build(mid+1,r,root*2+1);          //建立右子树
}
```
这样，一串数据就以平衡二叉树的形式建立起来了，以后查询的时候只需要根据左右孩子节点与父亲节点的下标的特殊关系减少到O(logn)的复杂度，以下是查询的代码：
```
int find(int l,int r,int root)     
{
    if(ss[root].ll==l&&ss[root].rr==r)   //如果递归到叶子节点，返回叶子节点信息值并且回溯到上一层
      return ss[root].max;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)            //往左走
      find(l,r,root*2);     
    else if(l>=mid+1)     //往右走
      find(l,r,root*2+1);
    else
      return max(find(l,mid,root*2),find(mid+1,r,root*2+1));   //当前节点完全包含在查询区间内
}
```
有查询就有修改，对于单点修改就是先查询到该数据再将该数据信息进行修改，不影响其他数据，代码如下：
```
void insert(int l,int r,int a,int root)   
{
    if(ss[root].ll==l&&ss[root].rr==r)  //已经到了叶子节点，数据修改了之后直接回溯上一层
    {
        ss[root].max=a;
        return ;
    }
    else if(a>ss[root].max)   //否则如果当前子树存储的最大值小于需要更新的最大值，则直接更新最大值，并且继续往下面的左右子树走
      ss[root].max=a;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)          //往左走
      insert(l,r,a,2*root);
    else if(l>=mid+1)        //往右走
      insert(l,r,a,2*root+1);
}
```
对于稍微复杂一点的就是区间更新，有一种lazy操作（也叫延迟操作）。通俗的举一个例子，比如说你需要多次对[1,n]这个区间内的数都加上一个值v，你可以先在[1,n]这个区间做一个标记，表示这整个区间的数都被加上了一个值，那么前面n次操作都只需要对lazy修改即可。
等询问时再把这个lazy标记传到下面的区间去。
具体代码如下：
```
void Add(int t,int l,int r,int c)  
{
    if(f[t].l==l&&f[t].r==r)   //如果是叶子节点，就不需要lazy操作了
    {
        f[t].add+=c;
        return ;
    }
    f[t].sum+=c*(r-l+1);     //记录好这个区间的下标距离差
    int mid=(f[t].l+f[t].r)/2;
    if(r<=mid)    //往左走
      Add(t*2,l,r,c);
    else             
    {
        if(l>mid)         //往右走
          Add(t*2+1,l,r,c);
        else                //左右并进
        {
            Add(t*2,l,mid,c);
            Add(t*2+1,mid+1,r,c); 
        }
    }
}
```
关于线段树的基础知识就这么多，这一块的深度很大，前几天看一个巨巨写二维数据更新查询，用到的是二维线段树操作，对于目前我的水平来看，还是菜菜，看着一百八十+的代码懵逼。
本题代码如下：
```cpp
#include<iostream>
#include<algorithm> 
#include<cstdio>
#include<cstring>
using namespace std;
int a,b;
struct node
{
    int ll,rr,max;          //ll表示左子节点，rr表示右子节点，max存储当前下标的最大值 
}ss[1200005];
int find(int l,int r,int root)      //查找 
{
    if(ss[root].ll==l&&ss[root].rr==r)
      return ss[root].max;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)
      find(l,r,root*2);
    else if(l>=mid+1)
      find(l,r,root*2+1);
    else
      return max(find(l,mid,root*2),find(mid+1,r,root*2+1));
}
void build(int l,int r,int root)    //建树 
{
    ss[root].ll=l;
    ss[root].rr=r;
    ss[root].max=0;
    if(l==r) return ;
    int mid=(l+r)/2;
    build(l,mid,root*2);
    build(mid+1,r,root*2+1);
}
void insert(int l,int r,int a,int root)     //插入数值（更新） 
{
    if(ss[root].ll==l&&ss[root].rr==r)
    {
        ss[root].max=a;
        return ;
    }
    else if(a>ss[root].max)
      ss[root].max=a;
    int mid=(ss[root].ll+ss[root].rr)/2;
    if(r<=mid)
      insert(l,r,a,2*root);
    else if(l>=mid+1)
      insert(l,r,a,2*root+1);
}
int main()
{
    int n,m;
    while(~scanf("%d %d",&n,&m))
    {
        char c; 
        build(1,n,1);         //先建一个空树 
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a); 
            insert(i,i,a,1);     //每输入一个值，插入更新线段树一次 
        }
        getchar();
        for(int i=0;i<m;i++)
        {
            scanf("%c %d %d",&c,&a,&b);
            getchar();
            if(c=='Q')
              printf("%d\n",find(a,b,1));  
            else
              insert(a,a,b,1); 
        }
    } 
    return 0;
}
```
