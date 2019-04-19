# C语言控制台窗口界面编程：用printf在终端打印一个GUI窗口 - Koma Hub - CSDN博客
2018年05月03日 16:29:32[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：366
环境：cygwin（linux）+gcc
先给出效果图：
![](https://img-blog.csdn.net/20180503162815906)
源代码：
```cpp
#include<stdio.h>  
#include<stdlib.h>  
/* \033[字符的显示方式;字符的颜色;字符的背景颜色m 需要显示的字符 \033[m */  
    /* 字符的显示方式: 
    0（默认值）、1（加粗）、2（灰显）、3（斜体）、4（下划线）、7（反显）、9（删除线） 
       字符的颜色: 
    30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（深蓝色）、 
    35（紫色）、36（浅蓝色）、37（白色） 
       字符的背景颜色: 
    40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、 
    45（紫色）、46（浅蓝色）、47（白色） */ 
    
#define loop(n) for(int i=0;i<n;i++)
int main(int argc, char **argv)  
{  
    int ix, iz, nx, nz, bndr=5;
    nx = 50;
    nz = 40;
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;43m \033[m");
    loop(5)printf("\b");
    printf("\033[;35;43m_ O X\033[m");
    loop(nx/3*2)printf("\b");
    printf("\033[;31;43m Main Window \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    printf("\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    loop(nx-3)printf("\b");
    printf("\033[;30;46m ifo:This is a test. \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    loop(nx-3)printf("\b");
    printf("\033[;30;46m     This is second test. \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    loop(nx-3)printf("\b");
    printf("\033[;30;46m     This is third test. \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    loop(nx-3)printf("\b");
    printf("\033[;30;46m     Code BY RongTao. 2018.05 \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    printf("\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    loop(nx-3)printf("\b");
    printf("\033[;30;46m Are you sure to continue?   \033[m");
    printf("\033[;;41m YES \033[m");
    printf("\033[;;46m  \033[m");
    printf("\033[;;41m NO \033[m\n");
    
    loop(bndr)printf(" ");
    loop(nx)printf("\033[;;46m \033[m");
    printf("\n");
}
```
更多信息请参考：[https://blog.csdn.net/rong_toa/article/details/79794014](https://blog.csdn.net/rong_toa/article/details/79794014)
