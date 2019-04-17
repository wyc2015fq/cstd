# poj1733——Parity game - westbrook1998的博客 - CSDN博客





2018年08月17日 12:47:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：40








> 
Now and then you play the following game with your friend. Your friend writes down a sequence consisting of zeroes and ones. You choose a continuous subsequence (for example the subsequence from the third to the fifth digit inclusively) and ask him, whether this subsequence contains even or odd number of ones. Your friend answers your question and you can ask him about another subsequence and so on. Your task is to guess the entire sequence of numbers.  

  You suspect some of your friend’s answers may not be correct and you want to convict him of falsehood. Thus you have decided to write a program to help you in this matter. The program will receive a series of your questions together with the answers you have received from your friend. The aim of this program is to find the first answer which is provably wrong, i.e. that there exists a sequence satisfying answers to all the previous questions, but no such sequence satisfies this answer. 

  Input 

  The first line of input contains one number, which is the length of the sequence of zeroes and ones. This length is less or equal to 1000000000. In the second line, there is one positive integer which is the number of questions asked and answers to them. The number of questions and answers is less or equal to 5000. The remaining lines specify questions and answers. Each line contains one question and the answer to this question: two integers (the position of the first and last digit in the chosen subsequence) and one word which is either `even' or`odd’ (the answer, i.e. the parity of the number of ones in the chosen subsequence, where `even' means an even number of ones and`odd’ means an odd number). 

  Output 

  There is only one line in output containing one integer X. Number X says that there exists a sequence of zeroes and ones satisfying first X parity conditions, but there exists none satisfying X+1 conditions. If there exists a sequence of zeroes and ones satisfying all the given conditions, then number X should be the number of all the questions asked. 

  Sample Input 

  10 

  5 

  1 2 even 

  3 4 odd 

  5 6 even 

  1 6 even 

  7 10 odd 

  Sample Output 

  3
带权并查集 用r数组来记录该点到根节点这段距离的权值，也就是1的个数，因为只有可能0或1，所以就可以%2来更新

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
using namespace std;
const int N=20050;
map<int,int> mp;
int n,m;
int p[N];
//0代表偶数次　1代表奇数次
int r[N];
int x,y;
char q[10];
bool flag=false;
int find(int x){
    if(x!=p[x]){
        int fa=find(p[x]);
        //更新ｒ
        // x-----p[x]--------new_p[x]
        // x-----------------new_p[x]
        //或者r[x]=(r[x]+r[p[x]])%2
        r[x]=r[x]^r[p[x]];
        //压缩路径
        p[x]=fa;
    }
    return p[x];
}
bool join(int a,int b,int d){
    int fa=find(a);
    int fb=find(b);
    if(fa==fb){
        if((r[a]^r[b])!=d){
            //true代表出现矛盾
            return true;
        }
        else{
            return false;
        }
    }
    else{
        p[fb]=fa;
        //比如r[a]: 1-5为奇数个 1  r[b]: 3-8为奇数个 1
        //而d就是1-3 假设为奇数个1 那么r[fb]就是5-8 应该为偶数个0
        r[fb]=(r[a]+d-r[b]+2)%2;
        return false;
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    //init
    int idx=1;
    mp.clear();
    for(int i=0;i<N;i++){
        p[i]=i;
        r[i]=0;
    }
    //input
    scanf("%d",&n);
    scanf("%d",&m);
    int ans=m;
    for(int i=1;i<=m;i++){
        scanf("%d%d %s",&x,&y,q);
        //离散化+闭区间处理
        if(!mp[x-1]){
            mp[x-1]=idx++;
        }
        if(!mp[y]){
            mp[y]=idx++;
        }
        if(flag){
            continue;
        }
        //printf("%s %d\n",q,strcmp(q,"even")==0);
        if(strstr(q,"even")){
            //printf("%d\n",i);
            if(join(mp[x-1],mp[y],0) && !flag){
                //printf("zheli\n");
                flag=true;
                ans=i-1;
            }
        }
        else{
            if(join(mp[x-1],mp[y],1) && !flag){
                //printf("nali\n");
                flag=true;
                ans=i-1;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





