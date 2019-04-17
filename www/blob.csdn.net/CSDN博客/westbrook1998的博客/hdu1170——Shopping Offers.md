# hdu1170——Shopping Offers - westbrook1998的博客 - CSDN博客





2018年08月25日 18:32:31[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：25标签：[状压dp																[dp](https://so.csdn.net/so/search/s.do?q=dp&t=blog)](https://so.csdn.net/so/search/s.do?q=状压dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
![这里写图片描述](https://odzkskevi.qnssl.com/4449b86367740ab4059dd531e03d3775?v=1535165921) n a shop each kind of product has a price. For example, the price of a flower is 2 ICU (Informatics Currency Units) and the price of a vase is 5 ICU. In order to attract more customers, the shop introduces some special offers.  

  A special offer consists of one or more product items for a reduced price. Examples: three flowers for 5 ICU instead of 6, or two vases together with one flower for 10 ICU instead of 12.  

  Write a program that calculates the price a customer has to pay for certain items, making optimal use of the special offers. That is, the price should be as low as possible. You are not allowed to add items, even if that would lower the price.  

  For the prices and offers given above, the (lowest) price for three flowers and two vases is 14 ICU: two vases and one flower for the reduced price of 10 ICU and two flowers for the regular price of 4 ICU.  

  Input 

  Your program is to read from standard input. The first line contains the number b of different kinds of products in the basket (0 <= b <= 5). Each of the next b lines contains three values c, k, and p. The value c is the (unique) product code (1 <= c <= 999). The value k indicates how many items of this product are in the basket (1 <= k <= 5). The value p is the regular price per item (1 <= p <= 999). Notice that all together at most 5*5=25 items can be in the basket. The b+2nd line contains the number s of special offers (0 <= s <= 99). Each of the next s lines describes one offer by giving its structure and its reduced price. The first number n on such a line is the number of different kinds of products that are part of the offer (1 <= n <= 5). The next n pairs of numbers (c,k) indicate that k items (1 <= k <= 5) with product code c (1 <= c <= 999) are involved in the offer. The last number p on the line stands for the reduced price (1 <= p <= 9999). The reduced price of an offer is less than the sum of the regular prices. 

  Output 

  Your program is to write to standard output. Output one line with the lowest possible price to be paid. 

  Sample Input 

  2 

  7 3 2 

  8 2 5 

  2 

  1 7 3 5 

  2 7 1 8 2 10 

  Sample Output 

  14
第一次学状压dp 光写注释就写到吐血 太难理解了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int INF=0x3f3f3f3f;
int Six[10]={1,6,36,216,1296,7776,46656,279936};
int dp[500005];
//每个套餐最多有5种物品，每种物品最多买5个
struct node{
    //比如00032(6进制)表示第0种物品买2个，第1种买3个
    //六进制表示
    int has;
    //状态作为容量
    int n;
    //价格作为价值
    int v;
}s[205];
//记录某个编号的物品在六进制的第几位
int Hash[1005];
int main(void){
    //freopen("data.txt","r",stdin);
    //物品种类数
    int b;
    while(~scanf("%d",&b)){
        memset(Hash,-1,sizeof(Hash));
        //不用套餐直接买的价格
        int ans=0;
        //物品标号
        int tmp;
        //总的状态，要购买的物品和数量（作为背包容量）
        int Num=0;
        for(int i=0;i<b;i++){
            scanf("%d%d%d",&tmp,&s[i].n,&s[i].v);
            ans+=s[i].n*s[i].v;
            //六进制的每一位表示每一种物品
            Num+=s[i].n*Six[i];
            Hash[tmp]=Six[i];
            s[i].has=s[i].n=Six[i];
        }
        //优惠套餐数
        int m;
        scanf("%d",&m);
        //套餐中的物品种类数
        int k;
        for(int i=0;i<m;i++){
            scanf("%d",&k);
            int x,y;
            while(k--){
                //优惠套餐中物品的种类和数量
                scanf("%d%d",&x,&y);
                if(Hash[x]==-1){
                    //前面没出现过这种物品，即这种物品无需购买
                    continue;
                }
                s[i+b].n+=Hash[x]*y;
            }
            scanf("%d",&s[i+b].v);
        }
        // printf("%d\n",Num);
        // int t=Num;
        // while(t){
        //     printf("%d",t%6);
        //     t/=6;
        // }
        // printf("\n");
        // for(int i=0;i<b+m;i++){
        //     printf("%d %d ",s[i].has,s[i].n);
        //     //1 1 2
        //     //6 6 5
        //     //0 3 5
        //     //这里13化为六进制就是21 也就表示0种类的1个 1种类的2个
        //     //0 13 10
        //     t=s[i].n;
        //     while(t){
        //         printf("%d",t%6);
        //         t/=6;
        //     }
        //     printf(" %d\n",s[i].v);
        // }
        //所以就是将每一个优惠套餐的状态用一个六进制数来保存，转换成完全背包问题
        memset(dp,INF,sizeof(dp));
        dp[0]=0;
        for(int i=0;i<b+m;i++){
            //j是状态，dp[j]表示达到j状态的最小价格
            for(int j=s[i].n;j<=Num;j++){
                dp[j]=min(dp[j],dp[j-s[i].n]+s[i].v);
                //printf("%d\n",dp[j]);
            }
        }
        printf("%d\n",min(ans,dp[Num]));
    }
    return 0;
}
```






