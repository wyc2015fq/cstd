# 使用VC库函数中的快速排序函数 - 数据之美的博客 - CSDN博客
2017年09月06日 20:22:18[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：154
上一篇讲了快速排序的实现。但在很多场合，直接使用快速排序的库函数是很方便的。下面讲下VC中库函数qsort()的用法：
函数原型：
void qsort(void *base,size_t num,size_t width, int (__cdecl *compare )(const void *, const void *) );
第一个是数组地址，第二是数组大小，第三个是数组中每个元素的字节数，最后一个是个函数指针，表示如何比较数组中的元素。
头文件 #include <stdlib.h>
下面分别就int等整数数据，double等浮点数据，结构体和类，按指定方式这四种情况进行讲解。
实例1   对int等整数数据进行排序
**[cpp]**[view
 plain](http://blog.csdn.net/morewindows/article/details/6684561#)[copy](http://blog.csdn.net/morewindows/article/details/6684561#)
- int cmp(constvoid *x, constvoid *y)  
- {  
- return *(int*)x - *(int*)y;  
- }  
- qsort(a, MAXN, sizeof(a[0]), cmp);   
MAXN为数组大小，下同
实例2   对double等浮点数进行排序
**[cpp]**[view
 plain](http://blog.csdn.net/morewindows/article/details/6684561#)[copy](http://blog.csdn.net/morewindows/article/details/6684561#)
- int cmpDouble(constvoid *x, constvoid *y)  
- {  
- return (*(double*)x > *(double*)y ? 1 : -1);  
- }  
- qsort(a, n, sizeof(a[0]), cmpDouble);  
实例3  对结构体，类等复杂数据进行排序
如
**[cpp]**[view
 plain](http://blog.csdn.net/morewindows/article/details/6684561#)[copy](http://blog.csdn.net/morewindows/article/details/6684561#)
- struct Student  
- {  
- char szName[30];  
- int  nAge;  
- };  
先对年龄排序，年龄相同再按姓名排序。
**[cpp]**[view
 plain](http://blog.csdn.net/morewindows/article/details/6684561#)[copy](http://blog.csdn.net/morewindows/article/details/6684561#)
- int cmpStudent (constvoid *x, constvoid *y)  
- {   //先作下指针转换，再按要求比较
-     Student *pNodex = (Student*)x, *pNodey = (Student*)y;  
- if (pNodex->nAge != pNodey->nAge)  
- return pNodex->nAge - pNodey->nAge;  
- else
- return strcmp(pNodex->szName, pNodey->szName);  
- }  
- qsort(a, n, sizeof(a[0]), cmpStudent);  
实例4     按指定方式进行排序。
如对只有大小写字母的字符串"AajkuKdYUBCDwyz"进行排序，要求大写字母在前，小写字母在后。
**[cpp]**[view
 plain](http://blog.csdn.net/morewindows/article/details/6684561#)[copy](http://blog.csdn.net/morewindows/article/details/6684561#)
- int cmp1(constvoid *x, constvoid *y)  
- {  
- char *pcx = (char*)x, *pcy = (char*)y;  
- 
- bool flag1 = *pcx >= 'A' && *pcx <= 'Z';  
- bool flag2 = *pcy >= 'A' && *pcy <= 'Z';  
- 
- if(flag1 == flag2)    //如果都为大写字母或都为小写字母
- return *pcx - *pcy;  
- else//否则，谁为大写字母，谁的权值小。
- return flag1 ? -1 : 1;  
- }  
- int main()  
- {  
- char szText[] = "AajkuKdYUBCDwyz";  
-     qsort(szText, strlen(szText), sizeof(szText[0]), cmp1);  
-     printf("%s\n", szText);  
- return 0;  
- }  
