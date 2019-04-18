# log的底数是？ - wydbyxr的博客 - CSDN博客
2018年12月26日 14:54:56[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：828
  参考：[https://www.zhihu.com/question/28207179?sort=created](https://www.zhihu.com/question/28207179?sort=created)
  普通应用都是10，计算机学科是2，编程语言里面是e。
  常用编程语言中Math.log一般都是以E为底（MATLAB里面是以e为底的）。
  ln是以e为底数，lg是以10为底数。
# 在C语言中
```
printf(""%f\n"",log(10)); //log是以e为底的对数函数 
printf(""%f\n"",log10(100)); //log10是以10为底的对数函数
```
