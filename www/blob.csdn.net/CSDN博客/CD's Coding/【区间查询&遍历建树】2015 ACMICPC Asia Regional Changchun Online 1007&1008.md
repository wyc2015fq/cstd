# 【区间查询&遍历建树】2015 ACMICPC Asia Regional Changchun Online 1007&1008 - CD's Coding - CSDN博客





2015年09月19日 11:09:41[糖果天王](https://me.csdn.net/okcd00)阅读数：648标签：[线段树																[前序遍历中序遍历求二叉树																[BST																[区间查询](https://so.csdn.net/so/search/s.do?q=区间查询&t=blog)
个人分类：[Coding Competition																[模板记忆](https://blog.csdn.net/okcd00/article/category/1650751)](https://blog.csdn.net/okcd00/article/category/2133459)





哇……标题好长

【静态线段树区间查询 & 通过前序中序遍历构造树】2015 ACMICPC Asia Regional Changchun Online 1007&1008


本来打算这么干的……想想还是不了

这场比赛中有六道通过率较高的题目，感觉都可以学到不少东西，[厉害的后辈](http://my.csdn.net/lwt36)整理了出来，推荐可以[去看看](http://blog.csdn.net/lwt36/article/details/48415647)（这些都是他们在赛场上就写出来了的哦，特别厉害！）。

这场比赛中，我们队伍的大家各负责两题，我就把我的1007和1008具体记录一下吧~






# The Water Problem
**Time Limit: 1500/1000 MS (Java/Others)    Memory Limit: 131072/131072 K (Java/Others)Total Submission(s): 1634    Accepted Submission(s): 1245**



Problem Description


In Land waterless, water is a very limited resource. People always fight for the biggest source of water. Given a sequence of water sources with representing
 the size of the water source. Given a set of queries each containing  integers  and ,
 please find out the biggest water source between  and .





Input


First you are given an integer  indicating
 the number of test cases. For each test case, there is a number  on
 a line representing the number of water sources.  integers
 follow, respectively ,
 and each integer is in .
 On the next line, there is a number  representing
 the number of queries. After that, there will be  lines
 with two integers  and  indicating
 the range of which you should find out the biggest water source.





Output


For each query, output an integer representing the size of the biggest water source.





Sample Input


3
1
100
1
1 1
5
1 2 3 4 5
5
1 2
1 3
2 4
3 4
3 5
3
1 999999 1
4
1 1
1 2
2 3
3 3






Sample Output


100
2
3
4
4
5
1
999999
999999
1







题意说明：在一维的世界（线上）有若干水源，他们的容量有大有小，可以想象数轴上有若干个点分布在不同坐标点上，各带有一个权值。给定一个坐标范围，如[-5,12]，问其中包含的水源中最大的水源容量是多少。



题目中其实已经在暗示线段树区间查询了，然后又看出没有单点更新或者区间更新，那这就是一道很明显的线段树区间查询的入门级题目，线段树基础概念在此就不多赘述了。




### 1007-Code：



```cpp
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
#define maxn 222222

#define lson l,m,rt<<1
#define rson m+1,r,rt<<1|1

int MAX[maxn<<2];
inline int max(int a,int b){return a>b? a:b;}

void PushUP(int rt)
{
    MAX[rt]=max(MAX[rt<<1],MAX[rt<<1|1]);
}

void Build(int l,int r,int rt)
{
    if(l==r)
    {
        scanf("%d",&MAX[rt]);
        return;
    }
    int m=(l+r)>>1;
    Build(lson);
    Build(rson);
    PushUP(rt);
}

void Update(int p,int add,int l,int r,int rt)
{
    if(l==r)
    {
        MAX[rt]=add;
        return;
    }
    int m=(l+r)>>1;
    if(p<=m)
        Update(p,add,lson);
    else
        Update(p,add,rson);
    PushUP(rt);
}

int Query(int L,int R,int l,int r,int rt)
{
    if(L<=l&&R>=r)
        return MAX[rt];
    int m=(l+r)>>1;
    int ret=0;
    if(L<=m)   ret=max(ret,Query(L,R,lson));
    if(R>m)    ret=max(ret,Query(L,R,rson));
    return ret;
}

int arr[1024]={0};

int main()
{
	int cases=0;
	//freopen("in.txt","r",stdin);
	scanf("%d",&cases);
	for(int _case=1;_case<=cases;_case++)
	{
		int n; scanf("%d",&n);
		memset(arr,0,sizeof arr);
		Build(1,n,1);
		int m; scanf("%d",&m);
		for(int i=0;i<m;i++)
		{
			int l,r;
			scanf("%d%d",&l,&r);
			printf("%d\n",Query(l,r,1,n,1));
		} 
	}
	//fclose(stdin);
	return 0;
}
```








# Elven Postman
**Time Limit: 1500/1000 MS (Java/Others)    Memory Limit: 131072/131072 K (Java/Others)Total Submission(s): 1108    Accepted Submission(s): 587**



Problem Description


Elves are very peculiar creatures. As we all know, they can live for a very long time and their magical prowess are not something to be taken lightly. Also, they live on trees. However, there is something about them you may not know. Although delivering stuffs
 through magical teleportation is extremely convenient (much like emails). They still sometimes prefer other more “traditional” methods.


So, as a elven postman, it is crucial to understand how to deliver the mail to the correct room of the tree. The elven tree always branches into no more than two paths upon intersection, either in the east direction or the west. It coincidentally looks awfully
 like a binary tree we human computer scientist know. Not only that, when numbering the rooms, they always number the room number from the east-most position to the west. For rooms in the east are usually more preferable and more expensive due to they having
 the privilege to see the sunrise, which matters a lot in elven culture.


Anyways, the elves usually wrote down all the rooms in a sequence at the root of the tree so that the postman may know how to deliver the mail. The sequence is written as follows, it will go straight to visit the east-most room and write down every room it
 encountered along the way. After the first room is reached, it will then go to the next unvisited east-most room, writing down every unvisited room on the way as well until all rooms are visited.


Your task is to determine how to reach a certain room given the sequence written on the root.


For instance, the sequence 2, 1, 4, 3 would be written on the root of the following tree.

![](http://acm.hdu.edu.cn/data/images/C621-1008-1.jpg)





Input


First you are given an integer 10 indicating
 the number of test cases.


For each test case, there is a number 1000 on
 a line representing the number of rooms in this tree.  integers
 representing the sequence written at the root follow, respectively 1 where 11.


On the next line, there is a number  representing
 the number of mails to be sent. After that, there will be  integers 1 indicating
 the destination room number of each mail.





Output


For each query, output a sequence of move ( or )
 the postman needs to make to deliver the mail. For that  means
 that the postman should move up the eastern branch and  the
 western one. If the destination is on the root, just output a blank line would suffice.


Note that for simplicity, we assume the postman always starts from the root regardless of the room he had just visited.





Sample Input


2
4
2 1 4 3
3
1 2 3
6
6 5 4 3 2 1
1
1






Sample Output


E

WE
EEEEE







题意说明：<我是把纸转180°来看的，不是我左右不分哦>精灵邮递员要给住在树上的精灵送邮件。每棵树上都以前序遍历（根-左-右）写着这棵树上的居民所在，同时，我们又知道居民的数字是按照从东向西排序的，即根据数字升序中序遍历（左-根-右）。邮递员想知道每个居民住在哪里，即我们需要通过这两种遍历获得这棵树，然后就可以用二叉搜索树（BST）来找居民位置了。于是难点就在如何使用遍历获得树（数据结构课project）。



我们首先得牢记三种递归规则：

1、前序遍历：根—左—右

2、中序遍历：左—根—右

2、后序遍历：左—右—根

从规则可以看出：前根序列的第一个值肯定是整个二叉树的根。后根序列的最后一个值肯定是整个二叉树的根。而知道中根序列和前、后根的一个序列后，必然中根序列将分以根分为两个部分：左子树和右子树。这样，在子树里面找到做子树根的那个值，继续分左右子树，这样下去即可确定二叉树的形态。同时，我们可以看到，如果只知道前、后根遍历。不知道中根，则二叉树形态无法唯一确定。


你们知道么……这段话我是在“[小学生自学网](http://www.xxszxw.net/cx/ShowArticle.asp?ArticleID=14954)”上看到的……

### 1008-Code：



```cpp
#include <map>
#include <cmath> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

//数据结构定义
struct NODE
{
    NODE* pLeft;         //左子树
    NODE* pRight;        //右子树
    int chValue;        //该节点的值
};
 
void ReBuild(int* pPreOrder,int* pInOrder,int nTreeLen,NODE** pRoot)
{
    //检查边界条件
    if(pPreOrder==NULL || pInOrder==NULL){return;}
 
    //获得前序遍历的第一个节点
    NODE* pTemp = new NODE;
    pTemp->chValue = *pPreOrder;
    pTemp->pLeft   = NULL;
    pTemp->pRight  = NULL;
 
    //如果节点为空，把当前节点复制到根节点
    if(*pRoot == NULL){*pRoot = pTemp;}
 
    //如果当前树长度为1，那么已经是最后一个节点
    if(nTreeLen == 1){return;}
 
    //寻找子树长度
    int* pOrgInOrder = pInOrder;
    int* pLeftEnd = pInOrder;
    int nTempLen = 0;
 
    //找到左子树的结尾
    while(*pPreOrder != *pLeftEnd)
    {
        if(pPreOrder==NULL || pLeftEnd==NULL) {return;}
        nTempLen++;
        //记录临时长度，以免溢出
        if(nTempLen > nTreeLen) {break;}
        pLeftEnd++;
    }
 
    //寻找子树长度
    int nLeftLen = 0;
    nLeftLen = (int)(pLeftEnd-pOrgInOrder);
    int nRightLen = 0;
    nRightLen = nTreeLen - nLeftLen - 1;
 
    //重建子树
    if(nLeftLen > 0) {ReBuild(pPreOrder+1,pInOrder,nLeftLen,&((*pRoot)->pLeft));}
    if(nRightLen > 0){ReBuild(pPreOrder+nLeftLen+1,pInOrder+nLeftLen+1,nRightLen,&((*pRoot)->pRight));}
 
}
 
//前序遍历结果
void PrePrint(NODE* pRoot)
{
    if(pRoot == NULL) return;
    cout<<pRoot->chValue<<" ";
    PrePrint(pRoot->pLeft);
    PrePrint(pRoot->pRight);
}
 
//中序遍历结果
void InPrint(NODE* pRoot)
{
    if(pRoot == NULL) return;
    InPrint(pRoot->pLeft);
    cout<<pRoot->chValue<<" ";
    InPrint(pRoot->pRight);
}
 
int szPreOrder[1024];
int szInOrder[1024];
map<int,string> mis;

string qsearch(int qry, string dir, NODE* pRoot,string route)
{
	if(pRoot == NULL) return "";
	else if(pRoot->chValue == qry)
		return route + dir;
	else if(pRoot->chValue < qry) 
		return qsearch(qry,"W",pRoot->pRight, route+dir);
	else
		return qsearch(qry,"E",pRoot->pLeft, route+dir);	
}

int main()
{
	//freopen("in.txt","r",stdin);
	int t;	scanf("%d",&t);
	for(int ti=0;ti<t;ti++)
	{
		int tlen;	scanf("%d",&tlen);
    	for(int i=0;i<tlen;i++) { szInOrder[i] = i+1; }
    	for(int i=0;i<tlen;i++)	{ scanf("%d",&szPreOrder[i]); }
    	NODE* pRoot = NULL;
    	ReBuild(szPreOrder,szInOrder,tlen,&pRoot);
    	//PrePrint(pRoot);
    	mis.clear();
    	string ss = "";
    	int q,qry;	scanf("%d",&q);
    	for(int qi=0;qi<q;qi++)
    	{
    		scanf("%d",&qry);
    		cout<<qsearch(qry,ss,pRoot,ss)<<endl;
    	}
	}
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=BST&t=blog)](https://so.csdn.net/so/search/s.do?q=前序遍历中序遍历求二叉树&t=blog)](https://so.csdn.net/so/search/s.do?q=线段树&t=blog)




