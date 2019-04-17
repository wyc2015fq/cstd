# C语言基础：用快速排序实现输出最大数 - Machine Learning with Peppa - CSDN博客





2017年11月17日 23:09:26[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：222
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)









```cpp
//此函数用来寻找从右往左第1位~第K位数字中最大的数
//因为数组长度较小，采用递归排序的非链表方法

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define div 10
#define N 10

typedef int cat;

cat digit(cat n, cat k);   //拆分函数
void quick_sort(cat a[], cat low, cat high);
cat split(cat a[], cat low, cat high);

cat main(void)
{
    cat i, e, num_re;

    printf("please enter the number: \n");
    scanf("%d", &i);   //注意这里输入的是一串连续数字
    printf("please enter the digit you want: \n");
    scanf("%d", &e);  //输入位数k

    num_re = digit(i,e);

    if (num_re != 0){  //判断返回值是否为真
    printf("the number you want is: %d ", num_re);
    }
    else{
        printf("Please enter the digit within the range\a");
    }

    return 0;
}

cat digit(cat n, cat k)  //定义两个形式参数n,k
{
    cat num = 0;
    cat arr[div], g, temp, status;

    if(n)
    {
        status = n;

        for(g = 0; g <= k; g++)   //先将数字拆分成单个字符存入数组
        {
            temp  = status % 10;  //寻找个位数
            status /= 10;   //从右向左减少一位

            arr[g] = temp;
            printf("%d\n", arr[g]);
            //g++;
        }

        quick_sort(arr, 0, k);//第三个参数的长度为变量K可以减少排序位数
        num = arr[k]; //注意这里不用-1

        return num;
    }

    else{
        printf("Please enter the right digit");
        return 0;
    }
}


//使用快速排序返回最大值

void quick_sort(cat a[], cat low, cat high)
{
    cat temp;

    if (low >= high){
        return;
    }
    else{
        temp = split(a, low, high);

        quick_sort(a, 0, temp - 1);
        quick_sort(a, temp + 1, high);
    }
}


cat split(cat a[], cat low, cat high)
{
    cat value = a[low];

    for(;;)
    {
        while (low < high && value <= a[high])
            high--;
        if (low >= high) {//为什么去掉大于号，函数无解
                break;}
        a[low++] = a[high];
        //low++;

        while (low < high && value >= a[low])
            low++;
        if (low >= high) break;
        a[high--] = a[low];
        //high--;
    }

    a[low] = value;
    return low;
}


/* 此为原快速排序代码main函数体
cat main(void)
{
    cat a[N], i;

    printf("Please enter %d number of sorting: ", N);

    for(i = 0; i < N; i++)
    {
        scanf("%d", &a[i]);
    }

    quick_sort(a, 0, N - 1);

    printf("In sorted order: ");
    for(i = 0; i < N; i++)
    {
        printf("%d \n", a[i]);
    }

    return 0;
}*/
```



