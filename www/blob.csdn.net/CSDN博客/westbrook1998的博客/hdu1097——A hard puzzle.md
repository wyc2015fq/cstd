# hdu1097——A hard puzzle - westbrook1998的博客 - CSDN博客





2018年02月19日 21:02:41[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：75








```
#include <stdio.h>
int main(void)
{
    int a,b;
    while(scanf("%d %d",&a,&b)!=EOF)
    {
        int t=1;
        a=a%10;
        while(b>0)
        {
            if(b%2==1)
                t=(t*a)%10;
            b=b/2;
            a=(a*a)%10;
        }
        printf("%d\n",t%10);
    }
    return 0;
}
```



