# codeforces17A——Noldbach problem - westbrook1998的博客 - CSDN博客





2018年05月17日 13:29:04[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46








> 
Nick is interested in prime numbers. Once he read about Goldbach problem. It states that every even integer greater than 2 can be expressed as the sum of two primes. That got Nick’s attention and he decided to invent a problem of his own and call it Noldbach problem. Since Nick is interested only in prime numbers, Noldbach problem states that at least k prime numbers from 2 to n inclusively can be expressed as the sum of three integer numbers: two neighboring prime numbers and 1. For example, 19 = 7 + 11 + 1, or 13 = 5 + 7 + 1. 

  Two prime numbers are called neighboring if there are no other prime numbers between them. 

  You are to help Nick, and find out if he is right or wrong. 

  Input 

  The first line of the input contains two integers n (2 ≤ n ≤ 1000) and k (0 ≤ k ≤ 1000). 

  Output 

  Output YES if at least k prime numbers from 2 to n inclusively can be expressed as it was described above. Otherwise output NO. 

  Examples 

  Input 

  27 2 

  Output 

  YES 

  Input 

  45 7 

  Output 

  NO
会错题意的结果…看漏了一个at least,导致一道简单的题目卡了四五次WA

代码:

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1050;
bool isPrime[MAXN];
void iP(void){
    memset(isPrime,true,sizeof(isPrime));
    isPrime[0]=false;
    isPrime[1]=false;
    for(int i=2;i<=1000;i++){
        if(isPrime[i]){
            for(int j=i*i;j<=MAXN;j+=i){
                isPrime[j]=false;
            }
        }
    }
}
int primes[168]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997};
int main(void){
    iP();
    int n,k;
    scanf("%d%d",&n,&k);
    int res=0;
    for(int i=2;i<=n;i++){
        if(isPrime[i]){
            for(int j=0;primes[j+1]<=i;j++){
                if(primes[j]+primes[j+1]+1==i){
                    res++;
                    break;
                }
            }
        }
    }
    if(k<=res){
        printf("YES\n");
    }
    else{
        printf("NO\n");
    }
    return 0;
}
```





