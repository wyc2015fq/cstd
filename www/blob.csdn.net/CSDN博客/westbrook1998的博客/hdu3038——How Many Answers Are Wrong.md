# hdu3038——How Many Answers Are Wrong - westbrook1998的博客 - CSDN博客





2018年08月16日 15:04:41[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：24标签：[并查集																[带权并查集](https://so.csdn.net/so/search/s.do?q=带权并查集&t=blog)](https://so.csdn.net/so/search/s.do?q=并查集&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
TT and FF are … friends. Uh… very very good friends -***__***-b  

  FF is a bad boy, he is always wooing TT to play the following game with him. This is a very humdrum game. To begin with, TT should write down a sequence of integers-_-!!(bored).  
![这里写图片描述](https://odzkskevi.qnssl.com/956a59c85e195b555e72de064fd5a563?v=1534044250)

  Then, FF can choose a continuous subsequence from it(for example the subsequence from the third to the fifth integer inclusively). After that, FF will ask TT what the sum of the subsequence he chose is. The next, TT will answer FF’s question. Then, FF can redo this process. In the end, FF must work out the entire sequence of integers.  

  Boring~~Boring~~a very very boring game!!! TT doesn’t want to play with FF at all. To punish FF, she often tells FF the wrong answers on purpose.  

  The bad boy is not a fool man. FF detects some answers are incompatible. Of course, these contradictions make it difficult to calculate the sequence.  

  However, TT is a nice and lovely girl. She doesn’t have the heart to be hard on FF. To save time, she guarantees that the answers are all right if there is no logical mistakes indeed.  

  What’s more, if FF finds an answer to be wrong, he will ignore it when judging next answers.  

  But there will be so many questions that poor FF can’t make sure whether the current answer is right or wrong in a moment. So he decides to write a program to help him with this matter. The program will receive a series of questions from FF together with the answers FF has received from TT. The aim of this program is to find how many answers are wrong. Only by ignoring the wrong answers can FF work out the entire sequence of integers. Poor FF has no time to do this job. And now he is asking for your help~(Why asking trouble for himself~~Bad boy)  

  Input 

  Line 1: Two integers, N and M (1 <= N <= 200000, 1 <= M <= 40000). Means TT wrote N integers and FF asked her M questions.  

  Line 2..M+1: Line i+1 contains three integer: Ai, Bi and Si. Means TT answered FF that the sum from Ai to Bi is Si. It’s guaranteed that 0 < Ai <= Bi <= N.  

  You can assume that any sum of subsequence is fit in 32-bit integer.  

  Output 

  A single line with a integer denotes how many answers are wrong. 

  Sample Input 

  10 5 

  1 10 100 

  7 10 28 

  1 3 32 

  4 6 41 

  6 6 1 

  Sample Output 

  1 
琢磨了很久的大佬的博客，还是不太明白，太牛比了这个带权并查集

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int INF=0x3f3f3f3f;
const int N=200020;
int p[N];
//sum数组就表示该点到其前驱节点（非根节点）的距离，有正负 区间从左到右值为负 区间从右到左值为正
int sum[N];
int find(int x){
    if(p[x]==x){
        return x;
    }
    int t=p[x];
    p[x]=find(p[x]);
    //压缩路径，所以这个节点到前驱元的距离要加上这个节点的前驱元到前前驱元的距离
    sum[x]+=sum[t];
    return p[x];
}
bool join(int a,int b,int Sum){
    int fa=find(a);
    int fb=find(b);
    printf("--a:%d--b:%d--\n",a,b);
    printf("--fa:%d--fb:%d--\n",fa,fb);
    if(fa==fb){
        //有相同的前驱元（根），减号是因为区间有正负，不等即矛盾
        if(sum[b]-sum[a]==Sum){
            return false;
        }
        else{
            return true;
        }
    }
    //合并
    else{
        //将b连在a上
        p[fb]=fa;
        //线段图表示    b-------fb
        //                 a------------fa     
        //b到fb就是sum[b]  a到fa就是sum[a] 而当前根节点为fa,所以所求的sum[fb]就是sum[a]+Sum(a,b)-sum[b]
        //因为sum有正负号，所以方向没问题 
        sum[fb]=sum[a]+Sum-sum[b];
        printf("%d %d\n",sum[fa],sum[fb]);
        return false;
    }

}
int main(void){
    freopen("data.txt","r",stdin);
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        int cnt=0;
        int x;
        int y;
        int Sum;
        for(int i=0;i<=n;i++){
            p[i]=i;
            sum[i]=0;
        }
        while(m--){
            scanf("%d%d%d",&x,&y,&Sum);
            if(join(x-1,y,Sum)){
                cnt++;
                printf("x:%d  y:%d\n",x,y);
            }
        }
        printf("sum:\n");
        for(int i=0;i<=n;i++){
            printf("%d ",sum[i]);
        }
        printf("\n");
        for(int i=0;i<=n;i++){
            printf("%d ",p[i]);
        }
        printf("\n");
        printf("%d\n",cnt);
    }
    return 0;
}
```






