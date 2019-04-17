# hdu1301——Jungle Roads - westbrook1998的博客 - CSDN博客





2018年06月25日 20:15:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








> 
![这里写图片描述](https://odzkskevi.qnssl.com/4e1a455cd8286ae049ef90e7be32bbfb?v=1529890646)

   The Head Elder of the tropical island of Lagrishan has a problem. A burst of foreign aid money was spent on extra roads between villages some years ago. But the jungle overtakes roads relentlessly, so the large road network is too expensive to maintain. The Council of Elders must choose to stop maintaining some roads. The map above on the left shows all the roads in use now and the cost in aacms per month to maintain them. Of course there needs to be some way to get between all the villages on maintained roads, even if the route is not as short as before. The Chief Elder would like to tell the Council of Elders what would be the smallest amount they could spend in aacms per month to maintain roads that would connect all the villages. The villages are labeled A through I in the maps above. The map on the right shows the roads that could be maintained most cheaply, for 216 aacms per month. Your task is to write a program that will solve such problems.  

  The input consists of one to 100 data sets, followed by a final line containing only 0. Each data set starts with a line containing only a number n, which is the number of villages, 1 < n < 27, and the villages are labeled with the first n letters of the alphabet, capitalized. Each data set is completed with n-1 lines that start with village labels in alphabetical order. There is no line for the last village. Each line for a village starts with the village label followed by a number, k, of roads from this village to villages with labels later in the alphabet. If k is greater than 0, the line continues with data for each of the k roads. The data for each road is the village label for the other end of the road followed by the monthly maintenance cost in aacms for the road. Maintenance costs will be positive integers less than 100. All data fields in the row are separated by single blanks. The road network will always allow travel between all the villages. The network will never have more than 75 roads. No village will have more than 15 roads going to other villages (before or after in the alphabet). In the sample input below, the first data set goes with the map above.  

  The output is one integer per line for each data set: the minimum cost in aacms per month to maintain a road system that connect all the villages. Caution: A brute force solution that examines every possible set of roads will not finish within the one minute time limit.  

  Input 

  9 

  A 2 B 12 I 25 

  B 3 C 10 H 40 I 8 

  C 2 D 18 G 55 

  D 1 E 44 

  E 2 F 60 G 38 

  F 0 

  G 1 H 35 

  H 1 I 35 

  3 

  A 2 B 10 C 40 

  B 1 C 20 

  0 

  Output 

  216 

  30 

  Sample Input 

  9 

  A 2 B 12 I 25 

  B 3 C 10 H 40 I 8 

  C 2 D 18 G 55 

  D 1 E 44 

  E 2 F 60 G 38 

  F 0 

  G 1 H 35 

  H 1 I 35 

  3 

  A 2 B 10 C 40 

  B 1 C 20 

  0 

  Sample Output 

  216 

  30
emmm最小生成树的一道模板题，抄了一下Kruskal模板 

这题首先要读懂题意，看了老半天 才看明白输入的格式 然后就是字符和数字的读取用`scanf` 然后回车注意要用`getchar()` 去掉

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
const int MAXN=10050;
int n,m;
int u[MAXN],v[MAXN],w[MAXN],p[MAXN],r[MAXN];
int cmp(const int i,const int j){
    return w[i]<w[j];
}
int find(int x){
    return p[x]==x ? x : p[x]=find(p[x]);
}
int Kruskal(){
    int ans=0;
    for(int i=0;i<n;i++){
        p[i]=i;
    }
    for(int i=0;i<m;i++){
        r[i]=i;
    }
    sort(r,r+m,cmp);
    for(int i=0;i<m;i++){
        int e=r[i];
        int x=find(u[e]);
        int y=find(v[e]);
        if(x!=y){
            ans+=w[e];
            //printf("ans=%d\n",ans);
            p[x]=y;
        }
    }
    return ans;
}
int main(void){
    while(~scanf("%d",&n)){
        if(n==0){
            break;
        }
        m=0;
        int k=0;
        char s;
        int num;
        for(int i=0;i<n-1;i++){
            getchar();
            scanf("%c%d",&s,&num);
            m+=num;
            char e;
            int we;
            for(int j=0;j<num;j++){
                getchar();
                scanf("%c%d",&e,&we);
                u[k]=s-'A';
                v[k]=e-'A';
                w[k++]=we;
            }
        }
        int res=Kruskal();
        printf("%d\n",res);
    }
    return 0;
}
```





