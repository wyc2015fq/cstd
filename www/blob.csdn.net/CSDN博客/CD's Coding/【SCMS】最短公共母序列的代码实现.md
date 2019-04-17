# 【SCMS】最短公共母序列的代码实现 - CD's Coding - CSDN博客





2015年07月21日 16:03:27[糖果天王](https://me.csdn.net/okcd00)阅读数：1516








## 前言

其实说起初衷，是之前吾辈的学校开展了个数模比赛，我去帮别人当帮手，当时的B题是基因拼接，D题是污染程度评定

作为ACMer，我当然喜欢B题这种像ACM竞赛题一样感觉的东西，但无奈人家才是参(da)赛(ye)者(a)，他们选了D，嘛，其实据说那次我做的也还是不错。

回到正题，于是我就开始考虑这种算法改如何实现呢，反正现在是学生，在CNKI上看论文不要钱，到处看看咯。

看到个论文，用dp实现生物序列最短公共超序列，感觉靠谱，要不然就试试呗，拿来当模板用，说不定哪天就出道题呢~

给他起了个名儿![偷笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/titter.gif)

SCMS = Shortest Common Mother-Sequence (最短公共母序列)

## 建模

可以将题意理解为ACM格式，则为如下情况：

数据组数为T，【给出T】

每组数据中有n条基因序列，【给出N】

以下则为n行，每行以一个字符串表示一条基因序列的组成【给出str_i】

【每条基因序列均为母序列中的一个子片段，此处的子片段指的是子串而非子序列】（给自己降低一点难度嘛不要在意）

要求输出满足所有这些基因序列的母序列。




## 代码实现(C/C++)



```cpp
#include<stdio.h>
#include<string.h>

struct str
{
    char letter[1600];
    int len;
}ans,a[20];

struct
{
    struct str temp;
    int x,y;
}add[500];

int f[1<<12][15],father[1<<12][15],reduce[20][20],cost[20][20],n,m,max,Test,s;
struct str ADD(struct str ans,int k,int reduce)
{
    int i;
    for(i=ans.len;i<ans.len+a[k].len-reduce;i++)
        ans.letter[i]=a[k].letter[reduce+i-ans.len];
    ans.letter[i]=0;
    ans.len=ans.len+a[k].len-reduce;
    return ans;
}
void quick(int l,int r)
{
    int i=l,j=r;
    struct str x=add[(i+j)>>1].temp;
    while(i<=j)
    {
        while(strcmp(add[i].temp.letter,x.letter)<0)  i++;
        while(strcmp(add[j].temp.letter,x.letter)>0)  j--;
        if(i<=j)
        {
            add[0]=add[i],add[i]=add[j],add[j]=add[0];
            i++,j--;
        }
    }
    if(i<r)  quick(i,r);
    if(j>l)  quick(l,j);
}
void READY()
{
    int i,j,k,r;
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
            for(k=0;k<a[i].len;k++)
            {
                for(r=0;k+r<a[i].len&&a[i].letter[k+r]==a[j].letter[r];r++);
                if(k+r==a[i].len)
                {
                    reduce[j][i]=r;
                    break;
                }
            }
    s=0;
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
        {
            s++;
            add[s].temp=ADD(a[i],j,reduce[j][i]),add[s].x=i,add[s].y=j;
        }
    quick(1,s);
    for(i=1;i<=s;i++)
        cost[add[i].x][add[i].y]=i;
}
void read()
{
    int i,j,k,r;
    scanf("%d",&n);
    memset(reduce,0,sizeof(reduce));
    for(i=1;i<=n;i++)
    {
        scanf("%s",a[i].letter);
        a[i].len=strlen(a[i].letter);
    }
    for(i=1;i<=n;i++)
        for(j=1;j<=n;j++)
        {
            if(i==j)  continue;                 
            for(k=0;k<a[j].len;k++)
            {
                for(r=0;r+k<a[j].len&&r<a[i].len&&a[j].letter[k+r]==a[i].letter[r];r++);
                if(r==a[i].len)  break;
            }
            if(k<a[j].len)  a[i].letter[0]=0;
        }
    for(i=n;i>=1;i--)
        if(a[i].letter[0]==0)
        {
            for(j=i;j<n;j++)
                a[j]=a[j+1];
            n--;
        }
    m=(1<<n)-1;
}
void DP()
{
    int i,j,k;
    for(i=0;i<=m;i++)
        for(j=1;j<=n;j++)
            f[i][j]=father[i][j]=0;
    for(i=1;i<=m;i++)
        for(j=1;j<=n;j++)
        {
            int add=1<<(j-1);
            if((i&add)==0)
                for(k=1;k<=n;k++)
                    if((i&(1<<(k-1)))!=0)
                    {
                        if(father[i+add][j]==0)
                        {
                            f[i+add][j]=f[i][k]+reduce[k][j],father[i+add][j]=k;
                            continue;
                        }
                        if(f[i+add][j]<f[i][k]+reduce[k][j])
                            f[i+add][j]=f[i][k]+reduce[k][j],father[i+add][j]=k;
                        else if(f[i+add][j]==f[i][k]+reduce[k][j]&&cost[j][father[i+add][j]]>cost[j][k])
                            father[i+add][j]=k;
                    }
        }
}
void solve()
{
    max=-1;
    int i,j,now;
    for(i=1;i<=n;i++)
    {
        if(max<f[m][i])
        {
            max=f[m][i],now=m;
            ans=a[i];
            for(j=i;father[now][j]!=0;)
            {
                ans=ADD(ans,father[now][j],reduce[father[now][j]][j]);
                int t=j;
                j=father[now][j],now-=(1<<(t-1));
            }
        }
        else if(max==f[m][i])
        {
            struct str temp=a[i];
            now=m;
            for(j=i;father[now][j]!=0;)
            {
                temp=ADD(temp,father[now][j],reduce[father[now][j]][j]);
                int t=j;
                j=father[now][j],now-=(1<<(t-1));
            }
            if(strcmp(ans.letter,temp.letter)>0)  ans=temp;
        }
    }
}
int main()
{
    int T=0;
    scanf("%d",&T);
    for(int i=1;i<=T;i++)
    {
		read();
		READY();
		DP();
		solve();
		printf("%s\n",ans.letter);
    }
    return 0;
}
```



## Reference

参阅的论文如下，关于证明过程和各种条件约束详见论文：

[《两个生物序列最短公共超序列的动态规划算法》](http://wenku.baidu.com/link?url=l37rIBvD8Ax4qq8iblILeNk4PPRof4eLQS7MMK0GDeA455WlJyRVxJwK7tgpmGN5BdemYJFtEmKq_FbXNAeeRtNAWp4gU2i6U0gkV2dS62O)

黄永莲1, 孙世军2

 (1.湛江师范学院生物科学与化学学院,广东湛江524048; 

  2.湛江师范学院信息科技学院,广东湛江524048)


摘要:将动态规划算法用于两个生物序列的最短公共超序列的计算。

计算过程分为两个算法,第一个算法计算两个序列的所有前缀的最短公共超序列的长度,

并存放在一个矩阵中,第二个算法利用前面所得的矩阵,找到两个序列的最短公共超序列。

关键词:生物序列;动态规划算法;超序列;最短公共超序列

中图分类号:Q-31   文献标识码:A   文章编号:1671-0231(2005)03-0063-03




