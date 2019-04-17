# Codeforces987E——Petr and Permutations - westbrook1998的博客 - CSDN博客





2018年11月01日 13:28:13[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[奇偶性](https://so.csdn.net/so/search/s.do?q=奇偶性&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
Petr likes to come up with problems about randomly generated data. This time problem is about random permutation. He decided to generate a random permutation this way: he takes identity permutation of numbers from 1 to n and then 3n times takes a random pair of different elements and swaps them. Alex envies Petr and tries to imitate him in all kind of things. Alex has also come up with a problem about random permutation. He generates a random permutation just like Petr but swaps elements 7n+1 times instead of 3n times. Because it is more random, OK?!

You somehow get a test from one of these problems and now you want to know from which one.

Input

In the first line of input there is one integer n (103≤n≤106).

In the second line there are n distinct integers between 1 and n — the permutation of size n from the test.

It is guaranteed that all tests except for sample are generated this way: First we choose n — the size of the permutation. Then we randomly choose a method to generate a permutation — the one of Petr or the one of Alex. Then we generate a permutation using chosen method.

Output

If the test is generated via Petr’s method print “Petr” (without quotes). If the test is generated via Alex’s method print “Um_nik” (without quotes).

Example

Input

5

2 4 5 1 3

Output

Petr

Note

Please note that the sample is not a valid test (because of limitations for n) and is given only to illustrate input/output format. Your program still has to print correct answer to this test to get AC.

Due to randomness of input hacks in this problem are forbidden.
给一个序列，是由1-n这个序列经过几次任意两个数交换得到的，如果交换次数是3n就是第一个人，如果是7n+1，就是第二个人

可以看出3n和7n+1一个是奇数一个是偶数，一开始想的是逆序数，猜逆序数个数奇偶性和n奇偶性若相同则第一个人，否则是第二个人

后来想到逆序数不行，考虑当前数列能交换几次到原本的数列，然后判断这个数的奇偶性再猜个结论
代码：

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e6+50;
int a[N];
int n;
int main(void){
    scanf("%d",&n);
    int cnt=0;
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<=n;i++){
        if(a[i]==i){
            continue;
        }
        else{
            while(a[i]!=i){
                cnt++;
                int t=a[i];
                swap(a[i],a[t]);
            }
        }
    }
    //printf("%d\n",ans);
    if(n%2==0 && cnt%2==0 || n%2==1 && cnt%2==1){
        printf("Petr\n");
    }
    else{
        printf("Um_nik\n");
    }
    return 0;
}
```






