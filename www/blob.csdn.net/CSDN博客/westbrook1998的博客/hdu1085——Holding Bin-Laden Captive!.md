# hdu1085——Holding Bin-Laden Captive! - westbrook1998的博客 - CSDN博客





2018年07月29日 16:31:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30标签：[母函数](https://so.csdn.net/so/search/s.do?q=母函数&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
We all know that Bin-Laden is a notorious terrorist, and he has disappeared for a long time. But recently, it is reported that he hides in Hang Zhou of China!  

  “Oh, God! How terrible! ”  
![这里写图片描述](https://odzkskevi.qnssl.com/b858f96d99fe58f794dd2613cb15429b?v=1532591800)

  Don’t be so afraid, guys. Although he hides in a cave of Hang Zhou, he dares not to go out. Laden is so bored recent years that he fling himself into some math problems, and he said that if anyone can solve his problem, he will give himself up!  

  Ha-ha! Obviously, Laden is too proud of his intelligence! But, what is his problem?  

  “Given some Chinese Coins (硬币) (three kinds– 1, 2, 5), and their number is num_1, num_2 and num_5 respectively, please output the minimum value that you cannot pay with given coins.”  

  You, super ACMer, should solve the problem easily, and don’t forget to take $25000000 from Bush!  

  Input 

  Input contains multiple test cases. Each test case contains 3 positive integers num_1, num_2 and num_5 (0<=num_i<=1000). A test case containing 0 0 0 terminates the input and this test case is not to be processed.  

  Output 

  Output the minimum positive value that one cannot pay with given coins, one line for one case.  

  Sample Input 

  1 1 3 

  0 0 0 

  Sample Output 

  4
母函数的问题 经典的这种凑硬币的问题 

注意最后一定要+1 因为有可能所有指数次幂都存在 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=10050;
int tmp[MAXN];
int ans[MAXN];
int main(void){
    int a,b,c;
    while(~scanf("%d%d%d",&a,&b,&c)){
        if(a==0 && b==0 && c==0){
            break;
        }
        memset(tmp,0,sizeof(tmp));
        memset(ans,0,sizeof(ans));
        //初始化第一个多项式
        for(int i=0;i<=a;i++){
            ans[i]=1;
        }
        //第一和第二个多项式相乘
        for(int i=0;i<=a;i++){
            for(int j=0;j<=b;j++){
                tmp[i+2*j]+=ans[i];
            }
        }
        for(int i=0;i<=(a+2*b);i++){
            ans[i]=tmp[i];
            tmp[i]=0;
        }
        //第二和第三个多项式相乘
        for(int i=0;i<=(a+2*b);i++){
            for(int j=0;j<=c;j++){
                tmp[i+5*j]+=ans[i];
            }
        }
        //注意这里要+1
        for(int i=0;i<=(a+2*b+5*c);i++){
            ans[i]=tmp[i];
            tmp[i]=0;
        }
        for(int i=0;i<=(a+2*b+5*c+1);i++){
            if(ans[i]==0){
                printf("%d\n",i);
                break;
            }
        }
    }
    return 0;
}
```





