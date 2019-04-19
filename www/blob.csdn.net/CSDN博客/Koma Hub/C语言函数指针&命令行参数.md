# C语言函数指针&命令行参数 - Koma Hub - CSDN博客
2017年11月29日 20:16:06[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：126
个人分类：[C/C++																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
我们可以用函数指针实现多态等高端函数，再加上命令行参数让你的程序提高一个档次。
程序如下：
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
int f1(int a){
    printf("f1:%d\n",a);
    return a;
}
int f2(int a){
    printf("f2:%d\n",a);
    return a*a;
}
int f3(int a){
    printf("f3:%d\n",a);
    return a*a*a;
}
int main(int argc, char*argv[]){
    int (*m)(int);
    int iflag = 1;
    int a = 2;
    if (argc > 1) iflag = atoi(argv[1]);
    if (argc > 2)     a = atoi(argv[2]);
    switch(iflag){
        case 1:
            m = &f1;
            break;
        case 2:
            m = &f2;
            break;
        case 3:
            m = &f3;
            break;
        default:break;
    }
    printf("%d\n",m(a));
    
    return 0;
}
```
编译与运行：
```
-bash-4.1$ gcc -o a Demo.c
-bash-4.1$ ./a
f1:2
2
-bash-4.1$ ./a 2
f2:2
4
-bash-4.1$ ./a 3
f3:2
8
-bash-4.1$ ./a 3 4
f3:4
64
-bash-4.1$ ./a 3 5
f3:5
125
-bash-4.1$ ./a 3 6
f3:6
216
-bash-4.1$ ./a 2 6
f2:6
36
-bash-4.1$ ./a 1 6
f1:6
6
-bash-4.1$
```
