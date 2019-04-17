# python参数重载 - 别说话写代码的博客 - CSDN博客





2018年08月16日 09:58:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：105








python不支持参数重载

注意底下示例：

![](https://img-blog.csdn.net/20180816094705304?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

说明上例中调用test()并没有调用没有参数的test()，只调用了最后一个

![](https://img-blog.csdn.net/20180816095228530?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上例中说明了只调用了最后一个函数

![](https://img-blog.csdn.net/20180816095632332?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面这个证明。python不能参数重载，只要函数 名相同，无论参数是否变化，只 调用最后一个函数



但应该有其他方法可以让参数 重载的吧



