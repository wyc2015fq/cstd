# CodeForces1041B——Buying a TV Set - westbrook1998的博客 - CSDN博客





2018年09月17日 12:07:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：129标签：[数学思维](https://so.csdn.net/so/search/s.do?q=数学思维&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Monocarp has decided to buy a new TV set and hang it on the wall in his flat. The wall has enough free space so Monocarp can buy a TV set with screen width not greater than a and screen height not greater than b. Monocarp is also used to TV sets with a certain aspect ratio: formally, if the width of the screen is w, and the height of the screen is h, then the following condition should be met: wh=xy. 

  There are many different TV sets in the shop. Monocarp is sure that for any pair of positive integers w and h there is a TV set with screen width w and height h in the shop. 

  Monocarp isn’t ready to choose the exact TV set he is going to buy. Firstly he wants to determine the optimal screen resolution. He has decided to try all possible variants of screen size. But he must count the number of pairs of positive integers w and h, beforehand, such that (w≤a), (h≤b) and (wh=xy). 

  In other words, Monocarp wants to determine the number of TV sets having aspect ratio xy, screen width not exceeding a, and screen height not exceeding b. Two TV sets are considered different if they have different screen width or different screen height. 

  Input 

  The first line contains four integers a, b, x, y (1≤a,b,x,y≤1018) — the constraints on the screen width and height, and on the aspect ratio. 

  Output 

  Print one integer — the number of different variants to choose TV screen width and screen height so that they meet the aforementioned constraints. 

  Examples 

  Input 

  17 15 5 3 

  Output 

  3 

  Input 

  14 16 7 22 

  Output 

  0 

  Input 

  4 2 6 4 

  Output 

  1 

  Input 

  1000000000000000000 1000000000000000000 999999866000004473 999999822000007597 

  Output 

  1000000063 

  Note 

  In the first example, there are 3 possible variants: (5,3), (10,6), (15,9). 

  In the second example, there is no TV set meeting the constraints. 

  In the third example, there is only one variant: (3,2).
找出有多少组数c d满足a<=a b<=b 而且a b 的比例等于所给比例x y 

理解完题意之后一开始居然是想暴力 队友及时阻止然后告诉我只要用上限a b除以x和y的比例就行，然后取小的一个值 

然后没过样例就交了 发现第三个样例错了 比例x/y要约分一下
代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long a,b,x,y;
int main(void){
    scanf("%I64d%I64d%I64d%I64d",&a,&b,&x,&y);
    long long ans=__gcd(x,y);
    printf("%I64d\n",min(a/(x/ans),b/(y/ans)));
    return 0;
}
```






