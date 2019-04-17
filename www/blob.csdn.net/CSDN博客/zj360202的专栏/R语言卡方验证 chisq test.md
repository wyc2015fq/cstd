# R语言卡方验证 chisq.test - zj360202的专栏 - CSDN博客





2013年08月20日 10:59:12[zj360202](https://me.csdn.net/zj360202)阅读数：5002
个人分类：[R语言](https://blog.csdn.net/zj360202/article/category/1573723)









```java
> a<-c(1,6)
> b<-c(3,6)
> chisq.test(rbind(a,b),correct=FALSE)

        Pearson's Chi-squared test

data:  rbind(a, b)
X-squared = 0.7619, df = 1, p-value = 0.3827

警告信息：
In chisq.test(rbind(a, b), correct = FALSE) : Chi-squared近似算法有可能不准
```






