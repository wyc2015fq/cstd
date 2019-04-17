# C语言求素数 - 别说话写代码的博客 - CSDN博客





2017年06月23日 23:30:52[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：330








                
#include#includeint fun(int lim,int aa[]){ int n, i, j,p=0; for (i = 2; i <= lim ; i++) { n = 0; for (j = 2; j <=i/2 ; j++)//注意此处为j<=i/2，不然若为2的倍数的数无法得出结果 { if (i%j == 0) n = 1; } if (n == 0)//说明是素数 { aa[p++] = i; } } printf("\n小于等于他的全部素数为:\n"); for (i = 0;
 i < p;i++) printf("%d ", aa[i]); return p;}/*20．编写函数int fun(int lim，int aa[MAX])，该函数的功能是求出小于或等于lim的所有素数并放在aa数组中，该函数返回所求出的素数的个数。*/void main(){ int lim, aa[50],p; printf("请输入一个数字："); scanf_s("%d",&lim); if (lim < 2) { printf("这个范围没有素数。"); exit(-1); } p = fun(lim,aa);
 printf("共有%d个\n", p); system("pause");}
            


