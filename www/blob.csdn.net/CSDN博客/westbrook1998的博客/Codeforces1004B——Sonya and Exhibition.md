# Codeforces1004B——Sonya and Exhibition - westbrook1998的博客 - CSDN博客





2018年08月09日 19:11:50[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35标签：[思维](https://so.csdn.net/so/search/s.do?q=思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Sonya decided to organize an exhibition of flowers. Since the girl likes only roses and lilies, she decided that only these two kinds of flowers should be in this exhibition. 

  There are n flowers in a row in the exhibition. Sonya can put either a rose or a lily in the i-th position. Thus each of n positions should contain exactly one flower: a rose or a lily. 

  She knows that exactly m people will visit this exhibition. The i-th visitor will visit all flowers from li to ri inclusive. The girl knows that each segment has its own beauty that is equal to the product of the number of roses and the number of lilies. 

  Sonya wants her exhibition to be liked by a lot of people. That is why she wants to put the flowers in such way that the sum of beauties of all segments would be maximum possible. 

  Input 

  The first line contains two integers n and m (1≤n,m≤103) — the number of flowers and visitors respectively. 

  Each of the next m lines contains two integers li and ri (1≤li≤ri≤n), meaning that i-th visitor will visit all flowers from li to ri inclusive. 

  Output 

  Print the string of n characters. The i-th symbol should be «0» if you want to put a rose in the i-th position, otherwise «1» if you want to put a lily. 

  If there are multiple answers, print any. 

  Examples 

  Input 

  5 3 

  1 3 

  2 4 

  2 5 

  Output 

  01100 

  Input 

  6 3 

  5 6 

  1 4 

  4 6 

  Output 

  110010 

  Note 

  In the first example, Sonya can put roses in the first, fourth, and fifth positions, and lilies in the second and third positions; 

  in the segment [1…3], there are one rose and two lilies, so the beauty is equal to 1⋅2=2; 

  in the segment [2…4], there are one rose and two lilies, so the beauty is equal to 1⋅2=2; 

  in the segment [2…5], there are two roses and two lilies, so the beauty is equal to 2⋅2=4. 

  The total beauty is equal to 2+2+4=8. 

  In the second example, Sonya can put roses in the third, fourth, and sixth positions, and lilies in the first, second, and fifth positions; 

  in the segment [5…6], there are one rose and one lily, so the beauty is equal to 1⋅1=1; 

  in the segment [1…4], there are two roses and two lilies, so the beauty is equal to 2⋅2=4; 

  in the segment [4…6], there are two roses and one lily, so the beauty is equal to 2⋅1=2. 

  The total beauty is equal to 1+4+2=7.
感觉cf很喜欢搞01串… 

这题读懂题意就不难了，要构造一个01串，使得询问的q个区间中，1的个数和0的个数相乘的和最大，很明显，要乘积最大，那么01串中0和1的个数要尽量均分，那就是0101交替了，这样无论如何询问区间，0和1的个数最多差1

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int n,m,a,b;
    scanf("%d%d",&n,&m);
    while(m--){
        scanf("%d%d",&a,&b);
    }
    bool flag=true;
    for(int i=0;i<n;i++){
        if(flag){
            printf("0");
            flag=false;
        }
        else{
            printf("1");
            flag=true;
        }
    }
    printf("\n");
    return 0;
}
```





