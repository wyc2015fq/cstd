# n阶等和幻方 - ljx0305的专栏 - CSDN博客
2010年04月05日 22:19:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：798
输入一个奇数n，构造并输出一个n阶等和幻方，
  即每一行每一列和两对角线上的n个数的和相等
  如当n=5时，有（构造方法请自行搜索或者观察下表）：
  03 16 09 22 15
  20 08 21 14 02
  07 25 13 01 19
  24 12 05 18 06
  11 04 17 10 23
 记得这种幻方的构造方法小时候听同学说过，嘿嘿（貌似是小学的时候.......），方法很简单的
如图
![](https://p-blog.csdn.net/images/p_blog_csdn_net/bysdy/%E6%9C%AA%E5%91%BD%E5%90%8D.jpg)
然后在把5*5格外的数字写入对应的空格内就OK了~~~
按照这种方法用C实现，代码如下：
#include<stdio.h>
#include<stdlib.h>
int **a,n;
void Initarray()
{
    int i,j;
    a=(int **)malloc((2*n-1)*sizeof(int*));
    for(i=0;i<=2*n-1;i++)
        a[i]=(int *)malloc((2*n-1)*sizeof(int));
    for(i=0;i<2*n-1;i++)
        for(j=0;j<=2*n-1;j++)
            a[i][j]=0;
}//创建初始化数组
void first()
{
    int x,y,i,j,num=1,t;
    for(y=n-1,x=0,t=1;y<=2*n-2;x++,y++,t++)//从左边中间开始向下1，n+1，2n+1......
        for(i=y,j=x;num<=t*n;i--,j++,num++)//依次递增，1，2，3，每次到n,2n,3n....结束
            a[i][j]=num;
}//将数组构造成如图的形式
void change()
{
    int x,y,i,j,kx,ky;
    for(i=0;i<=2*n-2;i++)
        for(j=0;j<=2*n-2;j++)
            if(a[i][j]!=0){
                if(i>=n-1-n/2&&i<=n-1+n/2&&j>=n-1-n/2&&j<=n-1+n/2)
                    continue;//是n*n中的数，跳出
                if(i<n-1-n/2)//是最上面的那些数
                    kx=1,ky=0;
                if(j<n-1-n/2)//是最左面的那些数
                    kx=0,ky=1;
                if(j>n-1+n/2)//是最右面的那些数
                    kx=0,ky=-1;
                if(i>n-1+n/2)//是最下面的那些数
                    kx=-1,ky=0;
                x=i+n*kx,y=j+n*ky;
                a[x][y]=a[i][j];//写入幻方内的对应位置
                a[i][j]=0;//已经写入幻方内，置0
            }
}
void print()
{
    int i,j;
    for(i=n-1-n/2;i<=n-1+n/2;i++){
        for(j=n-1-n/2;j<=n-1+n/2;j++)
            if(a[i][j]!=0)
                printf("%02d ",a[i][j]);
        printf(" ");
    }
}//打印该幻方
int main()
{
    int m,x,y,i,j,num,t;
    scanf("%d",&n);
    Initarray();
    first();
    change();
    print();
    return 0;
}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/bysdy/archive/2008/02/01/2077285.aspx](http://blog.csdn.net/bysdy/archive/2008/02/01/2077285.aspx)
