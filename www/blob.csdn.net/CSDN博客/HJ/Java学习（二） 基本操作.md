# Java学习（二） 基本操作 - HJ - CSDN博客
2018年02月07日 14:30:41[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：175
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、数组基本操作
1.直接选择排序
描述：以一个角标的元素和其他元素进行比较，在内循环第一次结束，最值出现在头角标位置上。
**public static void** selectSort**(int[]** arr**){    for(int** x**=**0**;** x**<**arr**.**length**-**1**;** x**++)    {        for(int** y**=**x**+**1**;** y**<**arr**.**length**;** y**++)**
        //为什么y的初始化值是 x+1？
        //因为每一次比较，都用x角标上的元素和下一个元素进行比较。
**        {            if(**arr**[**x**]>**arr**[**y**])            {                int** temp** =** arr**[**x**];**
                arr**[**x**] =** arr**[**y**];**
                arr**[**y**] =** temp**;            }        }    }}**
2.冒泡排序
描述：相邻两个元素进行比较，如果满足条件就进行位置置换。内循环结束一次，最值出现在尾角标位置。
**public static void** bubbleSort**(int[]** arr**){    for(int** x**=**0**;** x**<**arr**.**length**-**1**;** x**++)    {        for(int** y**=**0**;** y**<**arr**.**length**-**x**-**1**;** y**++)**
        //-x:让每次参与比较的元减。
        //-1:避免角标越界。
**        {            if(**arr**[**y**]>**arr**[**y**+**1**])            {                int** temp** =** arr**[**y**];**
                arr**[**y**] =** arr**[**y**+**1**];**
                arr**[**y**+**1**] =** temp**;            }        }    }}**
3.二分查找
注意：这种查找只对有序数组有效。
**public static int** halfSeach**(int[]** arr**,int** key**){    int** min**,**mid**,**max**;**
    min** =** 0**;**
    max** =** arr**.**length**-**1**;**
    mid** = (**max**+**min**)/**2**;    while(**arr**[**mid**]!=**key**)    {        if(**key**>**arr**[**mid**])**
            min** =** mid** +** 1**;        else if(**key**<**arr**[**mid**])**
            max** =** mid** -** 1**;        if(**min**>**max**)            return -**1**;**
        mid** = (**max**+**min**)/**2**;    }    return** mid**;}**
4.数组反转 
**public static void** reverseArray**(int[]** arr**){    for(int** start**=**0**,**end**=**arr**.**length**-**1**;** start**<**end**;** start**++,**end**--)    {**
        swap**(**arr**,**start**,**end**);    }}**
//对数组的元素进行位置的置换。
**public static void** swap**(int[]** arr**,int** a**,int** b**){    int** temp** =** arr**[**a**];**
    arr**[**a**] =** arr**[**b**];**
    arr**[**b**] =** temp**;}**
二、Arrays的使用
**遍历**： toString()     
将数组的元素以字符串的形式返回
**排序：** sort()        
将数组按照升序排列
**查找：** binarySearch()     
在指定数组中查找指定元素，返回元素的索引，如果没有找到返回（-插入点-1） 注意：使用查找的功能的时候，数组一定要先排序。
