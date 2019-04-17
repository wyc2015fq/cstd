# hdu1069——Monkey and Banana - westbrook1998的博客 - CSDN博客





2018年06月25日 14:08:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30标签：[dp																[动态规划](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
A group of researchers are designing an experiment to test the IQ of a monkey. They will hang a banana at the roof of a building, and at the mean time, provide the monkey with some blocks. If the monkey is clever enough, it shall be able to reach the banana by placing one block on the top another to build a tower and climb up to get its favorite food.  

  The researchers have n types of blocks, and an unlimited supply of blocks of each type. Each type-i block was a rectangular solid with linear dimensions (xi, yi, zi). A block could be reoriented so that any two of its three dimensions determined the dimensions of the base and the other dimension was the height.  

  They want to make sure that the tallest tower possible by stacking blocks can reach the roof. The problem is that, in building a tower, one block could only be placed on top of another block as long as the two base dimensions of the upper block were both strictly smaller than the corresponding base dimensions of the lower block because there has to be some space for the monkey to step on. This meant, for example, that blocks oriented to have equal-sized bases couldn’t be stacked.  

  Your job is to write a program that determines the height of the tallest tower the monkey can build with a given set of blocks.  

  Input 

  The input file will contain one or more test cases. The first line of each test case contains an integer n,  

  representing the number of different blocks in the following data set. The maximum value for n is 30.  

  Each of the next n lines contains three integers representing the values xi, yi and zi.  

  Input is terminated by a value of zero (0) for n.  

  Output 

  For each test case, print one line containing the case number (they are numbered sequentially starting from 1) and the height of the tallest possible tower in the format “Case case: maximum height = height”.  

  Sample Input 

  1 

  10 20 30 

  2 

  6 8 10 

  5 5 5 

  7 

  1 1 1 

  2 2 2 

  3 3 3 

  4 4 4 

  5 5 5 

  6 6 6 

  7 7 7 

  5 

  31 41 59 

  26 53 58 

  97 93 23 

  84 62 64 

  33 83 27 

  0 

  Sample Output 

  Case 1: maximum height = 40 

  Case 2: maximum height = 21 

  Case 3: maximum height = 28 

  Case 4: maximum height = 342
经典的动态规划问题，题目和样例看了好久才看懂，有n个长方体然后各有无限个，然后叠在一起 上面的长方体底的长宽要严格小于下面的（一边相等 一边小于也可以） 然后问最高能叠多高

首先一个长方体可以有六种摆法 其实也可以是三种 然后判断的时候多加几行 然后将n个长方体的6n个摆放状态存在vector里 按x(x相同就按y)从大到小排序  然后用一个O(n^2)的两重循环来算这个dp数组 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
using namespace std;
const int MAXN=50;
const int INF=1e8;
int dp[MAXN*6];
struct B{
    int x,y,z;
    B(int _x=0,int _y=0,int _z=0):x(_x),y(_y),z(_z){}
};
vector<B> bs;
bool cmp(B a,B b){
    if(a.x==b.x){
        return a.y>b.y;
    }
    else{
        return a.x>b.x;
    }
}
bool fun(int i,int j){
    return (bs[i].x<bs[j].x && bs[i].y<bs[j].y);
}
int main(void){
    int n;
    int T=0;
    while(~scanf("%d",&n)){
        if(n==0){
            break;
        }
        int x,y,z;
        bs.clear();
        for(int i=0;i<n;i++){
            scanf("%d%d%d",&x,&y,&z);
            bs.push_back(B(x,y,z));
            bs.push_back(B(y,x,z));
            bs.push_back(B(x,z,y));
            bs.push_back(B(z,x,y));
            bs.push_back(B(z,y,x));
            bs.push_back(B(y,z,x));
        }
        n=bs.size();
        sort(bs.begin(),bs.end(),cmp);
        /*
        for(int i=0;i<n;i++){
            printf("%d %d %d\n",bs[i].x,bs[i].y,bs[i].z);
        }
        */
        memset(dp,0,sizeof(dp));
        int ans=-INF;
        for(int i=0;i<n;i++){
            dp[i]=bs[i].z;
            for(int j=0;j<i;j++){
                if(fun(i,j)){
                    dp[i]=max(dp[i],dp[j]+bs[i].z);
                }
            }
            ans=max(ans,dp[i]);
        }
        ans=max(0,ans);
        printf("Case %d: maximum height = %d\n",++T,ans);
    }
    return 0;
}
```






