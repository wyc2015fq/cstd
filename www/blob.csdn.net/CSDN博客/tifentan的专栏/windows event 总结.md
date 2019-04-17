# windows event 总结 - tifentan的专栏 - CSDN博客





2017年12月29日 14:34:17[露蛇](https://me.csdn.net/tifentan)阅读数：338








1.官方demo：[https://msdn.microsoft.com/en-us/library/windows/desktop/ms686915(v=vs.85).aspx](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686915(v=vs.85).aspx)

2.关于pulseEvent,官方建议是不要用了，因为不能确保到达。这里有篇文章说的很清楚了。 
[关于线程同步 PulseEvent()](http://blog.csdn.net/crst_zh/article/details/4243916)

3.关于setEvent，手动event可以激活所有等待的线程，并保持set状态，直到手动reset；自动event的话激活一个等待的线程，如果没线程在等待，也会保持set状态，直到激活一个，然后自动立刻reset。



