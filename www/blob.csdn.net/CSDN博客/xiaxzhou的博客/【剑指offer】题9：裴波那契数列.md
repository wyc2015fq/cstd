# 【剑指offer】题9：裴波那契数列 - xiaxzhou的博客 - CSDN博客





2017年06月16日 11:12:37[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：138
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









> 
测试：


-1

0

999

```
long long  Fibonacci(int n) 
 {
        if(n==0)
            return 0;
        if(n==1)
            return 1;
        long long f_1 = 1,f_2 = 0;
        long long f(-1);
        for(auto i = 2;i<=n;++i)
        {
            f = f_1+f_2;
            f_2 = f_1;
            f_1 = f;
        }
        return f;
  }
```



