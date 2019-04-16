# HDU1698 Just a Hook 区间更新，查询区间【线段树】 - 紫芝的博客 - CSDN博客





2018年11月03日 15:48:48[紫芝](https://me.csdn.net/qq_40507857)阅读数：31








# Just a Hook

**Time Limit: 4000/2000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 15129    Accepted Submission(s): 7506**

Problem Description

In the game of DotA, Pudge’s meat hook is actually the most horrible thing for most of the heroes. The hook is made up of several consecutive metallic sticks which are of the same length.


![](http://acm.hdu.edu.cn/data/images/C116-1010-1.JPG)



Now Pudge wants to do some operations on the hook.


Let us number the consecutive metallic sticks of the hook from 1 to N. For each operation, Pudge can change the consecutive metallic sticks, numbered from X to Y, into cupreous sticks, silver sticks or golden sticks.

The total value of the hook is calculated as the sum of values of N metallic sticks. More precisely, the value for each kind of stick is calculated as follows:


For each cupreous stick, the value is 1.

For each silver stick, the value is 2.

For each golden stick, the value is 3.


Pudge wants to know the total value of the hook after performing the operations.

You may consider the original hook is made up of cupreous sticks.

Input

The input consists of several test cases. The first line of the input is the number of the cases. There are no more than 10 cases.

For each case, the first line contains an integer N, 1<=N<=100,000, which is the number of the sticks of Pudge’s meat hook and the second line contains an integer Q, 0<=Q<=100,000, which is the number of the operations.

Next Q lines, each line contains three integers X, Y, 1<=X<=Y<=N, Z, 1<=Z<=3, which defines an operation: change the sticks numbered from X to Y into the metal kind Z, where Z=1 represents the cupreous kind, Z=2 represents the silver kind and Z=3 represents the golden kind.

Output

For each case, print a number in a line representing the total value of the hook after the operations. Use the format in the example.

Sample Input

1

10

2

1 5 2

5 9 3

Sample Output

Case 1: The total value of the hook is 24.

Source

[2008 “Sunline Cup” National Invitational Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=2008%20%A1%B0Sunline%20Cup%A1%B1%20National%20Invitational%20Contest&source=1&searchmode=source)

Recommend

wangye   |   We have carefully selected several similar problems for you:  [1542](http://acm.hdu.edu.cn/showproblem.php?pid=1542)[1394](http://acm.hdu.edu.cn/showproblem.php?pid=1394)[2795](http://acm.hdu.edu.cn/showproblem.php?pid=2795)[1255](http://acm.hdu.edu.cn/showproblem.php?pid=1255)[1828](http://acm.hdu.edu.cn/showproblem.php?pid=1828)

### 题意：

有一条长度为N的金属链，初始每一块的值为1，然后是Q个操作，每个操作把【L，R】区间内的值变为X，求这条金属链的权值和？

### 分析：

与点更新不同的是，这是对某一个区间的值进行更新；线段树每一个节点代表一个区间，节点中存储的值代表金属的成色，1-铜，2-银，3-金；如果这个区间有杂色金属，则值为-1 

```cpp
#include <iostream>
#include <cstdio>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn = (1e5+10);

struct node{
	int left,right,n;
};
node tree[maxn<<2];

//初始化线段树
void build(int rt,int l,int r){
    tree[rt].left=l;
    tree[rt].right=r;
    tree[rt].n=1;
    if(l==r){//递归出口，即当前节点为叶子节点
        return;
    }
    int mid=(tree[rt].left+tree[rt].right)>>1;
    build(rt<<1,l,mid);//递归初始化当前节点的左儿子
    build(rt<<1|1,mid+1,r);//递归初始化当前节点的右儿子
}

//从根节点开始更新，在区间[l,r]插入X
void update(int rt,int l,int r,int x){
    if(tree[rt].n==x)
        return;//颜色相同，直接返回
    if(tree[rt].left==l&&tree[rt].right==r){//插入的区间匹配，直接修改该区间的值
        tree[rt].n=x;
        return;
    }
    if(tree[rt].n!=-1){//是纯色
        tree[rt<<1].n=tree[rt<<1|1].n=tree[rt].n;
        tree[rt].n=-1;
    }
    int mid=(tree[rt].left+tree[rt].right)>>1;
    if(r<=mid)//更新区间在中点的左边，更新左子树
        update(rt<<1,l,r,x);
    else if(l>mid)//更新区间在中点的右边，更新右子树
        update(rt<<1|1,l,r,x);
    else{//更新区间包含中点，同时更新左右子树
        update(rt<<1,l,mid,x);
        update(rt<<1|1,mid+1,r,x);
    }
}
//查询区间[L,R]的和
int query(int rt,int l,int r){
    if(tree[rt].n==-1)//杂色
       return query(rt<<1,l,r)+query(rt<<1|1,l,r);
    else
        return (tree[rt].right-tree[rt].left+1)*tree[rt].n;
}
int main()
{
    int T;
    scanf("%d",&T);
    int ca=1;
    while(T--){
        int n,q;
        scanf("%d%d",&n,&q);
        build(1,1,n);
        while(q--){
            int x,y,z;
            scanf("%d%d%d",&x,&y,&z);
            update(1,x,y,z);
        }
        int ans=query(1,1,n);
        printf("Case %d: The total value of the hook is %d.\n",ca++,ans);
    }
}
```





