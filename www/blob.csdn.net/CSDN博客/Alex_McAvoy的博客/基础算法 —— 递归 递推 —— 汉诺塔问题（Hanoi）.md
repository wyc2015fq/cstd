# 基础算法 —— 递归/递推 —— 汉诺塔问题（Hanoi） - Alex_McAvoy的博客 - CSDN博客





2018年04月23日 22:00:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：171








# 【问题提出】

Hanoi塔由n个大小不同的圆盘和三根木柱a,b,c组成。开始时，这n个圆盘由大到小依次套在a柱上，如图所示。 

![这里写图片描述](https://img-blog.csdn.net/20160518104857102)

要求把a柱上n个圆盘按下述规则移到c柱上： 

　　(1)一次只能移一个圆盘； 

　　(2)圆盘只能在三个柱上存放； 

　　(3)在移动过程中，不允许大盘压小盘。 

问将这n个盘子从a柱移动到c柱上，总计需要移动多少个盘次？ 

# 【问题解答】

**解：**设Hn为n个盘子从a柱移到c柱所需移动的盘次。

    显然，当n=1时，只需把a 柱上的盘子直接移动到c柱就可以了，故：H1=1。

    当n=2时，先将a柱上面的小盘子移动到b柱上去，然后将大盘子从a柱移到c柱,最后，将b柱上的小盘子移到c柱上，共记3个盘次，故：H2=3。

    以此类推，当a柱上有n(n>=2)个盘子时，总是先借助c柱把上面的n-1个盘子移动到b柱上，然后把a柱最下面的盘子移动到c柱上，再借助a柱把b柱上的n-1个盘子移动到c柱上，总共移动H(n-1)+1+H(n-1)个盘次。 

    ∴Hn=2H(n-1)+1，边界条件：H1=1

# 【递推实现】

```cpp
#include<stdio.h>
int ct=1;//记录步数,在步骤中输出
void move(int n,char from,char to)
{
    printf("第 %2d 步：把第 %d 个盘子：  %c >>>>>>> %c\n",ct++,n,from,to);
}
int hanoi(int n)//输出步数：
{
    int cnt = 2,ans = 1;
    if(n == 1)
        return 1;
    else
        return 2* hanoi(n-1) +1;
}
void hanoi_tower(int n,char x,char y, char z) //输出步骤
{
    if(n==1)
        move(1,x,z);
    else{
        hanoi_tower(n-1,x,z,y);
        move(n,x,z);
        hanoi_tower(n-1,y,x,z);
    }
}
int main()
{
    int n;//盘子个数
    printf("输入盘子个数:\n");
    scanf("%d",&n);
    char x = 'A',y = 'B',z = 'C';
    int t = hanoi(n);
    printf("一共需要%2d步。\n",t);
    hanoi_tower(n,x,y,z);
    return 0;
}
```

# 【递归实现】

```cpp
#include<stdio.h>
void move(int n, char x, char y, char z)//将n个圆盘从x柱子上借助y柱子移动到z柱子上
{
     if(n == 1)
     	printf("圆盘编号 %d :从 %c 移动到 %c\n",n,x,z);
     else
     {
         move(n-1,x,y,z);
         printf("圆盘编号 %d:从 %c 移动到 %c\n",n,x,z);
         move(n-1,y,x,z);
      }
 }
int main()
{
    int n;//n代表圆盘的个数
    /*A,B,C分别代表三个柱子*/
    char ch1 = 'A';
    char ch2 = 'B';
    char ch3 = 'C';

     printf("请输入圆盘的个数：");
     scanf("%d",&n);
     move(n,ch1,ch2,ch3);
     return 0;
}
```





