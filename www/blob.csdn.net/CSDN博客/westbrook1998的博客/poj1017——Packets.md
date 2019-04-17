# poj1017——Packets - westbrook1998的博客 - CSDN博客





2018年07月05日 12:28:00[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
A factory produces products packed in square packets of the same height h and of the sizes 1*1, 2*2, 3*3, 4*4, 5*5, 6*6. These products are always delivered to customers in the square parcels of the same height h as the products have and of the size 6*6. Because of the expenses it is the interest of the factory as well as of the customer to minimize the number of parcels necessary to deliver the ordered products from the factory to the customer. A good program solving the problem of finding the minimal number of parcels necessary to deliver the given products according to an order would save a lot of money. You are asked to make such a program. 

  Input 

  The input file consists of several lines specifying orders. Each line specifies one order. Orders are described by six integers separated by one space representing successively the number of packets of individual size from the smallest size 1*1 to the biggest size 6*6. The end of the input file is indicated by the line containing six zeros. 

  Output 

  The output file contains one line for each line in the input file. This line contains the minimal number of parcels into which the order from the corresponding line of the input file can be packed. There is no line in the output file corresponding to the last “null” line of the input file. 

  Sample Input 

  0 0 4 0 0 1  

  7 5 1 0 0 0  

  0 0 0 0 0 0  

  Sample Output 

  2  

  1 
简单的问题给搞复杂了 

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int p[6];
int main(void){
    while(~scanf("%d%d%d%d%d%d",&p[0],&p[1],&p[2],&p[3],&p[4],&p[5])){
        if(p[0]==0 && p[1]==0 && p[2]==0 && p[3]==0 && p[4]==0 && p[5]==0){
            break;
        }
        int ans=p[5]+p[4]+p[3]+(p[2]+3)/4;
        // 计算2*2的个数
        int y=p[3]*5;
        if(p[2]%4==3){
            y+=1;
        }
        else if(p[2]%4==2){
            y+=3;
        }
        else if(p[2]%4==1){
            y+=5;
        }
        if(y<p[1]){
            ans+=((p[1]-y)+8)/9;
        }
        // 计算1*1的个数
        int x=36*ans-4*p[1]-9*p[2]-16*p[3]-25*p[4]-36*p[5];
        if(x<p[0]){
            ans+=((p[0]-x)+35)/36;
        }
        printf("%d\n",ans);
    }
    return 0;
}
```





