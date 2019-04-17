# hdu1032——The 3n + 1 problem - westbrook1998的博客 - CSDN博客





2018年07月07日 09:44:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








> 
Problems in Computer Science are often classified as belonging to a certain class of problems (e.g., NP, Unsolvable, Recursive). In this problem you will be analyzing a property of an algorithm whose classification is not known for all possible inputs.  

  Consider the following algorithm:  

      1.      input n  

      2.      print n  

      3.      if n = 1 then STOP  

      4.           if n is odd then n <- 3n + 1  

      5.           else n <- n / 2  

      6.      GOTO 2  

  Given the input 22, the following sequence of numbers will be printed 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1  

  It is conjectured that the algorithm above will terminate (when a 1 is printed) for any integral input value. Despite the simplicity of the algorithm, it is unknown whether this conjecture is true. It has been verified, however, for all integers n such that 0 < n < 1,000,000 (and, in fact, for many more numbers than this.)  

  Given an input n, it is possible to determine the number of numbers printed (including the 1). For a given n this is called the cycle-length of n. In the example above, the cycle length of 22 is 16.  

  For any two numbers i and j you are to determine the maximum cycle length over all numbers between i and j.  

  Input 

  The input will consist of a series of pairs of integers i and j, one pair of integers per line. All integers will be less than 1,000,000 and greater than 0.  

  You should process all pairs of integers and for each pair determine the maximum cycle length over all integers between and including i and j.  

  You can assume that no opperation overflows a 32-bit integer.  

  Output 

  For each pair of input integers i and j you should output i, j, and the maximum cycle length for integers between and including i and j. These three numbers should be separated by at least one space with all three numbers on one line and with one line of output for each line of input. The integers i and j must appear in the output in the same order in which they appeared in the input and should be followed by the maximum cycle length (on the same line).  

  Sample Input 

  1 10 

  100 200 

  201 210 

  900 1000 

  Sample Output 

  1 10 20 

  100 200 125 

  201 210 89 

  900 1000 174
原来是一道暴力题..我还是wa了近十次… 

第一次想用dp打表..打表 得出的结果不太对 

第二次暴力打表 100万跑步出来 

第三次直接暴力 结果忘记i可能大于j这种情况 

然后接下来就连续wa了四五次 原来我是判断m和n的大小情况然后直接交换 这样子输出的时候m和n的顺序就乱了…坑爹啊 
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int solve(int x){
    int ans=1;
    while(x!=1){
        if(x%2){
            x=x*3+1;
        }
        else{
            x/=2;
        }
        ans++;
    }
    return ans;
}
int main(void){
    int m,n;
    while(~scanf("%d%d",&m,&n)){
        int s=min(m,n);
        int t=max(m,n);
        int ans=0;
        for(int i=s;i<=t;i++){
            ans=max(ans,solve(i));
        }
        printf("%d %d %d\n",m,n,ans);
    }
    return 0;
}
```






