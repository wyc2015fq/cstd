# Codeforces1003B——Binary String Constructing - westbrook1998的博客 - CSDN博客





2018年08月09日 18:53:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：43








> 
You are given three integers a, b and x. Your task is to construct a binary string s of length n=a+b such that there are exactly a zeroes, exactly b ones and exactly x indices i`(where 1≤i<n)`such that si≠si+1. It is guaranteed that the answer always exists. 

  For example, for the string “01010” there are four indices i such that`1≤i<n and si≠si+1 ``(i=1,2,3,4).` For the string “111001” there are two such indices i`(i=3,5).`

  Recall that binary string is a non-empty sequence of characters where each character is either 0 or 1. 

  Input 

  The first line of the input contains three integers a, b and x`(1≤a,b≤100,1≤x<a+b).`

  Output 

  Print only one string s, where s is any binary string satisfying conditions described above. It is guaranteed that the answer always exists. 

  Examples 

  Input 

  2 2 1 

  Output 

  1100 

  Input 

  3 3 3 

  Output 

  101100 

  Input 

  5 3 6 

  Output 

  01010100 

  Note 

  All possible answers for the first example: 

  1100; 

  0011. 

  All possible answers for the second example: 

  110100; 

  101100; 

  110010; 

  100110; 

  011001; 

  001101; 

  010011; 

  001011.
这题借鉴了一下同学的思路 结果我先a了 他还没a 

就是给一定数量的0或1 然后要求构造出01串满足相邻不同的数量等于所给的x 

首先可以看出  

01—1个不同 

0101—3个不同 

010101—5个不同 

所以我们先判断x是奇是偶，如果是偶数，先减1变成奇数 

所以有x个不同 就有x/2 个01串 

然后现在串左边就是0，右边就是1，再判断剩下的情况， 

如果x本来就是奇数的，这时候已经构造完成了，剩下的就是把剩下的0放左边 1放右边 

（这里今天又get到了c++ string的用法 原来也可以用+来连接字符串） 

然后就是x本来是偶数的情况了，又分为三种情况 

0 1 都剩有： 

因为还缺一个不同，所以先把所有0放左边，然后再把所有1也放左边，那这样又产生了一个01串 

只剩下0： 

0全部放左边 

只剩下1： 

1全部放右边
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
const int MAXN=205;
int main(void){
    int a,b,x;
    string res="";
    scanf("%d%d%d",&a,&b,&x);
    int f=0;
    if(x%2==0){
        x--;
        f=1;
    }
    int t=(x+1)/2;
    while(t--){
        res+="01";
        a--;
        b--;
    }
    if(f){
        if(a && b){
            while(a--){
                res="0"+res;
            }
            while(b--){
                res="1"+res;
            }
        }
        else if(a){
            while(a--){
                res+="0";
            }
        }
        else if(b){
            while(b--){
                res="1"+res;
            }
        }
    }
    else{
        while(a--){
            res="0"+res;
        }
        while(b--){
            res+="1";
        }
    }
    cout << res << endl;
    return 0;
}
```






