# poj2196——Specialized Four-Digit Numbers - westbrook1998的博客 - CSDN博客





2018年07月12日 22:18:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：75








> 
Find and list all four-digit numbers in decimal notation that have the property that the sum of its four digits equals the sum of its digits when represented in hexadecimal (base 16) notation and also equals the sum of its digits when represented in duodecimal (base 12) notation.  

  For example, the number 2991 has the sum of (decimal) digits 2+9+9+1 = 21. Since 2991 = 1*1728 + 8*144 + 9*12 + 3, its duodecimal representation is 1893 12, and these digits also sum up to 21. But in hexadecimal 2991 is BAF 16, and 11+10+15 = 36, so 2991 should be rejected by your program.  

  The next number (2992), however, has digits that sum to 22 in all three representations (including BB0 16), so 2992 should be on the listed output. (We don’t want decimal numbers with fewer than four digits – excluding leading zeroes – so that 2992 is the first correct answer.)  

  Input 

  There is no input for this problem 

  Output 

  Your output is to be 2992 and all larger four-digit numbers that satisfy the requirements (in strictly increasing order), each on a separate line with no leading or trailing blanks, ending with a new-line character. There are to be no blank lines in the output. The first few lines of the output are shown below. 

  Sample Input 

  There is no input for this problem 

  Sample Output 

  2992 

  2993 

  2994 

  2995 

  2996 

  2997 

  2998 

  2999
找出所有四位数满足10 12 16进制表示各位相加和相等的数

… 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int sum(int n,int f){
    int a=3;
    int ans=0;
    while(n>0){
        int t=(int)pow(f,a);
        ans+=(n/t);
        n%=t;
        a--;
    }
    return ans;
}
bool judge(int n){
    if(sum(n,10)==sum(n,12) && sum(n,10)==sum(n,16) && sum(n,12)==sum(n,16)){
        return true;
    }
    return false;
}
int main(void){
    for(int i=1000;i<=9999;i++){
        if(judge(i)){
            printf("%d\n",i);
        }
    }
    return 0;
}
```





