# hdu1051——Wooden Sticks - westbrook1998的博客 - CSDN博客





2018年05月16日 21:41:51[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：24标签：[贪心																[重点题](https://so.csdn.net/so/search/s.do?q=重点题&t=blog)](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
There is a pile of n wooden sticks. The length and weight of each stick are known in advance. The sticks are to be processed by a woodworking machine in one by one fashion. It needs some time, called setup time, for the machine to prepare processing a stick. The setup times are associated with cleaning operations and changing tools and shapes in the machine. The setup times of the woodworking machine are given as follows:  

  (a) The setup time for the first wooden stick is 1 minute.  

  (b) Right after processing a stick of length l and weight w , the machine will need no setup time for a stick of length l’ and weight w’ if l<=l’ and w<=w’. Otherwise, it will need 1 minute for setup.  

  You are to find the minimum setup time to process a given pile of n wooden sticks. For example, if you have five sticks whose pairs of length and weight are (4,9), (5,2), (2,1), (3,5), and (1,4), then the minimum setup time should be 2 minutes since there is a sequence of pairs (1,4), (3,5), (4,9), (2,1), (5,2). 

  Input 

  The input consists of T test cases. The number of test cases (T) is given in the first line of the input file. Each test case consists of two lines: The first line has an integer n , 1<=n<=5000, that represents the number of wooden sticks in the test case, and the second line contains n 2 positive integers l1, w1, l2, w2, …, ln, wn, each of magnitude at most 10000 , where li and wi are the length and weight of the i th wooden stick, respectively. The 2n integers are delimited by one or more spaces.  

  Output 

  The output should contain the minimum setup time in minutes, one per line.  

  Sample Input 

  3  

  5  

  4 9 5 2 2 1 3 5 1 4  

  3  

  2 2 1 1 2 2  

  3  

  1 3 2 2 3 1 

  Sample Output 

  2 

  1 

  3
题目大体意思就是说一堆木材有长度和重量，然后依次放入机器加工，如果当前放入的木材长度和重量都大于等于上一个，就不用花1分钟去安装机器，求最短时间的方案 

思路就是先将木材按长度重量从小到大排序，然后两重循环遍历，第一重找到递增序列的起点（未访问），然后内循环就遍历以此元素为起点的整个递增序列，并标记访问

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
struct wood{
    int l;
    int w;
    int v;
};
bool cmp(wood a,wood b){
    if(a.l<b.l){
        return true;
    }
    else if(a.l>b.l){
        return false;
    }
    else{
        return a.w<b.w;
    }
}
const int MAXN=5050;
wood w[MAXN];
int t,n;
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%d%d",&w[i].l,&w[i].w);
            w[i].v=0;
        }
        int ans=0;
        int vis=0;
        sort(w,w+n,cmp);
        int s=0;
        for(int i=0;i<n;i++){
            if(w[i].v==0){
                w[i].v=1;
                ans++;
                int wg=w[i].w;
                for(int j=i+1;j<n;j++){
                    if(w[j].v==0 && w[j].w>=wg){
                        wg=w[j].w;
                        w[j].v=1;
                    }
                }
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```






