# C语言printf控制光标位置和清空屏幕 - Koma Hub - CSDN博客
2018年05月21日 14:38:15[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1470
**控制光标位置：**
```cpp
void locateCursor(const int row, const int col){
    printf("%c[%d;%dH",27,row,col);
}
```
**清空屏幕：**
```cpp
void clearScreen(){
    printf("\e[1;1H\e[2J");
}
```
**测试：**
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void locateCursor(const int row, const int col){
    printf("%c[%d;%dH",27,row,col);
}
void delay(unsigned int xms)  
{
    unsigned int x,y;
    for(x=xms;x>0;x--)
        for(y=1000000;y>0;y--);
}
int main()
{
    int i=0;
    system("clear");
    do{
        locateCursor(1,10);
        printf("%3d%%\n",i);
        delay(10);
        
    }while(i++<100);
}
```
**结果：**
![](https://img-blog.csdn.net/20180521143755181)
