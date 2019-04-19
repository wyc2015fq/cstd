# leetcode题库：6. Z字形变换 - Koma Hub - CSDN博客
2018年04月24日 11:00:04[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：135
***题目：***
/**题目：6. Z字形变换（题目地址：[https://leetcode-cn.com/problems/zigzag-conversion/description/](https://leetcode-cn.com/problems/zigzag-conversion/description/)）
 * 将字符串 "PAYPALISHIRING" 以Z字形排列成给定的行数：
 * P   A   H   N
 * A P L S I I G
 * Y   I   R
 * 之后从左往右，逐行读取字符："PAHNAPLSIIGYIR"
 * 示例 1:
 * 输入: s = "PAYPALISHIRING", numRows = 3
 * 输出: "PAHNAPLSIIGYIR"
 * 示例 2:
 * 输入: s = "PAYPALISHIRING", numRows = 4
 * 输出: "PINALSIGYAHRPI"
 * 解释:
 * P     I    N
 * A   L S  I G
 * Y A   H R
 * P     I
 */
用了一个简单的方法，实现起来最简单，比较容易理解吧。我的***思路是：***
![](https://img-blog.csdn.net/20180424105321107)
***我的代码：***
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/**题目：6. Z字形变换
 * 将字符串 "PAYPALISHIRING" 以Z字形排列成给定的行数：
 * P   A   H   N
 * A P L S I I G
 * Y   I   R
 * 之后从左往右，逐行读取字符："PAHNAPLSIIGYIR"
 * 示例 1:
 * 输入: s = "PAYPALISHIRING", numRows = 3
 * 输出: "PAHNAPLSIIGYIR"
 * 示例 2:
 * 输入: s = "PAYPALISHIRING", numRows = 4
 * 输出: "PINALSIGYAHRPI"
 * 解释:
 * P     I    N
 * A   L S  I G
 * Y A   H R
 * P     I
 */
 
char* convert(char* s, int numRows) 
{
    char flag = '#';
    
    int length = strlen(s);
    int nz = 2*numRows - 2;
    int nx = length/nz + ((length%nz>0)?1:0);
    
    //printf("%d, %d, %d, %d\n",length,numRows,nx,nz);
    printf("Length of \"%s\" is %d.\n",s,length);
    int nz2 = nz + 1;
    int L = nz2 * nx;
    char *str = malloc(sizeof(char)*(L+1));
    int i, ix, iz;
    for(i=0;i<length;i++)
    {
        ix = i/nz;
        iz = i%nz;
        str[ix*nz2 + iz] = s[i];
    }
    for(i=0;i<nx-((length%nz>0)?1:0);i++)
    {
        str[i*nz2+nz] = flag;
    }
    for(i=nz2*(nx-((length%nz>0)?1:0))+length%nz;i<L;i++)
    {
        str[i] = flag;
    }
    str[L] = '\0';
    printf("Length of \"%s\" is %d.\n",str,L);
    
    char *out = malloc(sizeof(char)*length);
    int offset = numRows-1, j, iout=0, id;
    while(offset>=0){
        for(i=0;i<nx;i++)
        {
            id = i * nz2 + numRows -1;
            
            if(offset!= 0 && str[id - offset] != flag)
                out[iout++] = str[id - offset];
            if(offset!= 0 && str[id + offset] != flag)
                out[iout++] = str[id + offset];
            if(offset== 0 && str[id] != flag)
                out[iout++] = str[id];
            
        }
        offset --;
    }
    out[length] = '\0';
    printf("Length of \"%s\" is %d.\n",out,strlen(out));
    return out;
}
int main(int argc, char **argv)
{
    char *str = "123456789abcdefg";
    int numRows = 2;
    printf(" Input: %s\nOutput: %s\n",str,convert(str, numRows));
    
    return 1;
}
```
***测试结果：***
```
D:\test>gcc leetcode6.c
D:\test>a.exe
Length of "abcdefghijklmnopqr" is 18.
Length of "abcdefghij#klmnopqr###" is 22.
Length of "akbjlcimdhnregoqfp" is 18.
 Input: abcdefghijklmnopqr
Output: akbjlcimdhnregoqfp
```
***再来一个：***
```
char *str = "123456789abcdefg";
int numRows = 2;
Length of "123456789abcdefg" is 16.
Length of "12#34#56#78#9a#bc#de#fg#? is 24.
Length of "13579bdf2468aceg" is 16.
 Input: 123456789abcdefg
Output: 13579bdf2468aceg
```
