# C语言二维数组Array[][] - Koma Hub - CSDN博客
2017年11月30日 20:11:33[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：157
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
int main(int argc, char*argv[]){
    int a[5][3]={ {1,2,3},{4,5,6},{7,8,9},{10,11,12},{13,14,15} };
    int b[5][3]={ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int i,j;
    for(i=0;i<5;i++)
    {
        for(j=0;j<3;j++)
        {
            printf("%2d  ",a[i][j]);
        }
        printf("             ");
        for(j=0;j<3;j++)
        {
            printf("%2d  ",b[i][j]);
        }
        printf("\n");
    }
    return 0;
}
```
结果：
```
-bash-4.1$ gcc -o a Demo_23_Array[][].c
-bash-4.1$ ./a
 1   2   3                1   2   3  
 4   5   6                4   5   6  
 7   8   9                7   8   9  
10  11  12               10  11  12  
13  14  15               13  14  15
```
