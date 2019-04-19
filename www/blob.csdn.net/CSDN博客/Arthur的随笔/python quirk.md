# python quirk - Arthur的随笔 - CSDN博客
2011年05月05日 08:56:00[largetalk](https://me.csdn.net/largetalk)阅读数：724标签：[python																[fun																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=fun&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
import time
def fun(x=time.time()):
    print 'in fun', x
print time.time()
time.sleep(3)
print time.time()
fun()
fun(time.time())
print result is:
1304556761.25
1304556764.25
**in fun 1304556761.25**
in fun 1304556764.25
