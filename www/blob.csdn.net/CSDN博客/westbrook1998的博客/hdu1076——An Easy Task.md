# hdu1076——An Easy Task - westbrook1998的博客 - CSDN博客





2018年02月19日 20:22:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59标签：[hdu																[算法](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=hdu&t=blog)
个人分类：[hdu](https://blog.csdn.net/westbrook1998/article/category/7459916)








```
#include <stdio.h>
int isLeap(int y)
{
    if(y%4==0 && y%100!=0 || y%400==0)
        return 1;
    else
        return 0;
}
int main(void)
{
    int n;
    scanf("%d",&n);
    while(n--)
    {
        int y,n;
        int count=0;
        scanf("%d %d",&y,&n);
        while(1)
        {
            if(isLeap(y))
            {
                count++;
                if(count==n)
                    break;    
            }
            y++;
        }
        printf("%d\n",y);
    }
    return 0;
}
```




