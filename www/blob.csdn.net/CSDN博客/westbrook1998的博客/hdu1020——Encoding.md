# hdu1020——Encoding - westbrook1998的博客 - CSDN博客





2018年05月09日 20:49:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








> 
Given a string containing only ‘A’ - ‘Z’, we could encode it using the following method:  

  1. Each sub-string containing k same characters should be encoded to “kX” where “X” is the only character in this sub-string.  

  2. If the length of the sub-string is 1, ‘1’ should be ignored.  

  Input 

  The first line contains an integer N (1 <= N <= 100) which indicates the number of test cases. The next N lines contain N strings. Each string consists of only ‘A’ - ‘Z’ and the length is less than 10000.  

  Output 

  For each test case, output the encoded string in a line.  

  Sample Input 

  2 

  ABC 

  ABBCCC 

  Sample Output 

  ABC 

  A2B3C
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int maxn=10010;
char str[maxn];
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(str,'\0',sizeof(str));
        scanf("%s",str);
        int len=strlen(str);
        for(int i=0;i<len;i++){
            if(str[i]!=str[i+1]){
                printf("%c",str[i]);

            }
            else{
                int num=2;
                i++;
                while(i<len-1 && str[i]==str[i+1]){
                    num++;
                    i++;
                }
                printf("%d%c",num,str[i]);
            }
        }
        printf("\n");
    }
    return 0;
}
```

没有用到什么高深的算法，主要就是尽量减少遍历次数（一次遍历即可，边判断边输出），如果相邻两个字符不同，那么肯定直接输出前一个字符，如果相同，那么至少是2个，所以定义`num`为2，然后再一个循环找到`num`的最终值，输出即可，这个过程中要注意数组越界





