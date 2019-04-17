# Hexadecimal View（HDU-4054） - Alex_McAvoy的博客 - CSDN博客





2019年03月16日 21:29:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[HDU																[字符串处理——模拟与暴力](https://blog.csdn.net/u011815404/article/category/8650811)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Hexadecimal is very important and useful for computer programmers. You are requested to provide a hexadecimal view for given data. The hexadecimal view is made up of one or more rows. Every row except the last one represents 16 characters. Each row consists of three columns separated by a space: 

* addr: the 4-digit hexadecimal beginning address of this row. 

* dump: the hexadecimal representation of this row, separating every two characters by a whitespace. If there are less than 16 characters in the last row, pad it with spaces. 

* text: the ASCII translation of this row, with uppercase characters converted to lowercase and lowercase characters converted to uppercase. 

Use lowercase for the letter digits. See sample for more details. 

# Input

There are multiple test cases. Each line is a test case. The line is made up of no less than 1 and no more than 4096 printable characters including spaces. 

# Output

 For each test case, output its hexadecimal view. Do not output any extra spaces after the last character of text. 

# Sample Input

**Hex Dump#include <cstdio>printf("Hello, World!\n");main = do getLine >>= print . sum . map read . words**

# **Sample Output**

**0000: 4865 7820 4475 6d70                     hEX dUMP0000: 2369 6e63 6c75 6465 203c 6373 7464 696f #INCLUDE <CSTDIO0010: 3e                                      >0000: 7072 696e 7466 2822 4865 6c6c 6f2c 2057 PRINTF("hELLO, w0010: 6f72 6c64 215c 6e22 293b                ORLD!\N");0000: 6d61 696e 203d 2064 6f20 6765 744c 696e MAIN = DO GETlIN0010: 6520 3e3e 3d20 7072 696e 7420 2e20 7375 E >>= PRINT . SU0020: 6d20 2e20 6d61 7020 7265 6164 202e 2077 M . MAP READ . W0030: 6f72 6473                               ORDS**


题意：每行给出一个字符串，要求先输出字符串的位置，然后再输出每个字符 ASCII 码的十六进制数，最后将字符串的大写转为小写，小写转为大写输出

思路：模拟，具体看代码

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
char str[N];
int main(){

    while(gets(str)){
        int len=strlen(str);
        int col;//行数

        if(len%16==0)//能整除时
            col=len/16;
        else//不能整除时
            col=len/16+1;//行数需要+1


        int j=0,k=0;
        for(int i=0;i<col;i++){//枚举行数
            printf("%03x0: ",i);//输出地址

            //输出16进制表示
            for(;j<col*16;j++){//对于每行的每个字母
                if(j<len)//小于长度时
                    printf("%x",str[j]);//转为16进制输出
                else//补齐空格
                    printf("  ");

                if(j%2)
                    printf(" ");
                if( (j+1)%16==0 ){
                    j++;
                    break;
                }
            }

            //输出大小写转化
            for(;k<len;k++){//对于行的每个字母
                if(str[k]>='A' && str[k]<='Z' )//大写转小写
                    printf("%c",str[k]+32);
                else if(str[k]>='a' && str[k]<='z')//小写转大写
                    printf("%c",str[k]-32);
                else//特殊符号
                    printf("%c",str[k]);

                if(!str[k])
                    break;
                if( (k+1)%16==0 ){
                    k++;
                    break;
                }
            }
            printf("\n");
        }
    }
    return 0;
}
```






