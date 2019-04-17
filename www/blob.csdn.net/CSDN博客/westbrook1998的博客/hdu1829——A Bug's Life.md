# hdu1829——A Bug's Life - westbrook1998的博客 - CSDN博客





2018年08月16日 23:04:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[带权并查集																[种类并查集](https://so.csdn.net/so/search/s.do?q=种类并查集&t=blog)](https://so.csdn.net/so/search/s.do?q=带权并查集&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Background  

  Professor Hopper is researching the sexual behavior of a rare species of bugs. He assumes that they feature two different genders and that they only interact with bugs of the opposite gender. In his experiment, individual bugs and their interactions were easy to identify, because numbers were printed on their backs.  

  Problem  

  Given a list of bug interactions, decide whether the experiment supports his assumption of two genders with no homosexual bugs or if it contains some bug interactions that falsify it.  

  Input 

  The first line of the input contains the number of scenarios. Each scenario starts with one line giving the number of bugs (at least one, and up to 2000) and the number of interactions (up to 1000000) separated by a single space. In the following lines, each interaction is given in the form of two distinct bug numbers separated by a single space. Bugs are numbered consecutively starting from one. 

  Output 

  The output for every scenario is a line containing “Scenario #i:”, where i is the number of the scenario starting at 1, followed by one line saying either “No suspicious bugs found!” if the experiment is consistent with his assumption about the bugs’ sexual behavior, or “Suspicious bugs found!” if Professor Hopper’s assumption is definitely wrong. 

  Sample Input 

  2 

  3 3 

  1 2 

  2 3 

  1 3 

  4 2 

  1 2 

  3 4 

  Sample Output 

  Scenario #1: 

  Suspicious bugs found! 

  Scenario #2: 

  No suspicious bugs found! 

  Hint 

  Huge input,scanf is recommended.
带权并查集特别是这种种类并查集还是不怎么看的懂….        

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=2005;
int p[N];
//与根节点的关系 0异性 1同性
int re[N];
int n,k;
bool flag;
void init(){
    flag=false;
    for(int i=0;i<=n;i++){
        p[i]=i;
        re[i]=0;
    }
}
int find(int x){
    if(x==p[x]){
        return x;
    }
    //递归找到根节点，（上面的re关系数组也已经更新）
    int t=find(p[x]);
    printf("递归到%d %d\n",x,t);
    //这里这个p[x]还没更新的的
    //比如          -4   递归更新的时候，1的父节点2已经接上了4,
    //         - 3      但是返回到最上面一层的时候，p[1]仍然是2
    //      -2
    //     1
    printf("更新re%d %d\n",re[x],re[p[x]]);
    re[x]=(re[x]+re[p[x]])%2;
    printf("更新完毕%d\n",re[x]);
    printf("%d\n",re[x]);
    //压缩路径
    return p[x]=t;
}
void join(int a,int b){
    int fa=find(a);
    int fb=find(b);
    printf("%d--%d\n",fa,fb);
    printf("%d %d %d\n",re[1],re[2],re[3]);
    if(fa==fb){
        //出现同性矛盾
        if(re[a]==re[b]){
            flag=1;
        }
    }
    else{
        printf("unit\n");
        p[fa]=fb;
        printf("%d %d\n",re[a],re[b]);
        //第一次合并时，本来两个都是0,表示到根节点（本身）为同性，以fb为根合并后，就应该将re[fa]更新为1
        //因为这两个是异性了
        //re[fa]=(re[a]+re[b]+1)%2;
        //异或 再0取1 1取0
        re[fa]=!(re[a]^re[b]);
        printf("%d %d %d\n",re[fa],re[a],re[b]);
    }
}
int main(void){
    freopen("data.txt","r",stdin);
    int t,a,b;
    int c=1;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&k);
        init();
        while(k--){
            scanf("%d%d",&a,&b);
            if(flag){
                continue;
            }
            join(a,b);
        }
        printf("Scenario #%d:\n",c++);
        if(flag){
             printf("Suspicious bugs found!\n");
         }
        else{
             printf("No suspicious bugs found!\n");
         }
        printf("\n");
    }
    return 0;
}
```






