# poj2443——Set Operation - westbrook1998的博客 - CSDN博客





2018年08月06日 11:06:48[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[BitSet](https://so.csdn.net/so/search/s.do?q=BitSet&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given N sets, the i-th set (represent by S(i)) have C(i) element (Here “set” isn’t entirely the same as the “set” defined in mathematics, and a set may contain two same element). Every element in a set is represented by a positive number from 1 to 10000. Now there are some queries need to answer. A query is to determine whether two given elements i and j belong to at least one set at the same time. In another word, you should determine if there exist a number k (1 <= k <= N) such that element i belongs to S(k) and element j also belong to S(k). 

  Input 

  First line of input contains an integer N (1 <= N <= 1000), which represents the amount of sets. Then follow N lines. Each starts with a number C(i) (1 <= C(i) <= 10000), and then C(i) numbers, which are separated with a space, follow to give the element in the set (these C(i) numbers needn’t be different from each other). The N + 2 line contains a number Q (1 <= Q <= 200000), representing the number of queries. Then follow Q lines. Each contains a pair of number i and j (1 <= i, j <= 10000, and i may equal to j), which describe the elements need to be answer. 

  Output 

  For each query, in a single line, if there exist such a number k, print “Yes”; otherwise print “No”. 

  Sample Input 

  3 

  3 1 2 3 

  3 1 2 5 

  1 10 

  4 

  1 3 

  1 5 

  3 5 

  1 10 

  Sample Output 

  Yes 

  Yes 

  No 

  No 

  Hint 

  The input may be large, and the I/O functions (cin/cout) of C++ language may be a little too slow for this problem.
学了Bitset这种东西 

//定义了一个bitset数组 一共有10005个bitset 每个bitset有1005个二进制位 

//在这个题里 b[i]的第j位是1的话说明 i这个整数 在j这个集合里面 

//所以 判断x y是否同在至少一个集合中 也就是判断b[x] & b[y] (还是一个bitset) .any() 

代码：
```cpp
#include <cstdio>
#include <algorithm>
#include <bitset>
using namespace std;
//定义了一个bitset数组 一共有10005个bitset 每个bitset有1005个二进制位
//在这个题里 b[i]的第j位是1的话说明 i这个整数 在j这个集合里面
//所以 判断x y是否同在至少一个集合中 也就是判断b[x] & b[y] (还是一个bitset) .any()
bitset<1005> b[10005];
int main(void){
    int n,c,t;
    int q,x,y;
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&c);
        for(int j=0;j<c;j++){
            scanf("%d",&t);
            b[t].set(i);
        }
    }
    scanf("%d",&q);
    while(q--){
        scanf("%d%d",&x,&y);
        if((b[x]&b[y]).any()){
            printf("Yes\n");
        }
        else{
            printf("No\n");
        }
    }
    return 0;
}
```






