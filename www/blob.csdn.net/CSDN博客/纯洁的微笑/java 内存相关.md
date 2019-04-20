# java 内存相关 - 纯洁的微笑 - CSDN博客
2011年12月20日 14:15:37[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：58
个人分类：[杂项](https://blog.csdn.net/ityouknow/article/category/8364190)
                空闲内存：
Runtime.getRuntime().freeMemory()
总内存：
Runtime.getRuntime().totalMemory()
最大内存：
Runtime.getRuntime().maxMemory()
已占用的内存：
Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()            
