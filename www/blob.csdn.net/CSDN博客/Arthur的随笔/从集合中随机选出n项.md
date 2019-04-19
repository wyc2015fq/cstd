# 从集合中随机选出n项 - Arthur的随笔 - CSDN博客
2011年06月25日 09:39:00[largetalk](https://me.csdn.net/largetalk)阅读数：834标签：[python																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
import random
#如果剩余选择空间个数乘以随机数大于等于剩余选择数，则选择该数，否则跳过
def sample(sets, n):
    res = []
    m = t = 0
    N = len(sets)
    for item in sets:
        u = random.random()
        if (N-t)*u >= (n-m):
            t += 1
        else:
            res.append(item)
            m += 1
            t += 1
            if m >= n:
                break
    return res
#this come from python cookbook
def sample2(sets, n):
    N = len(sets)
    res = []
    for samp in xrange(n, 0, -1):
        cumprob = 1.0
        u = random.random()
        while u < cumprob:
            cumprob -= cumprob * samp / N
            N -= 1
        res.append(sets[len(sets)-N-1])
    return res
print sample(xrange(100), 5)
print sample2(xrange(100), 5)
