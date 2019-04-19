# python assert_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
>>> assert 1 == 0,'one does not equal zero'
抛出去的是AssertionError异常，但是可以给这个异常传递参数，也就是后面的字符串
打印异常的时候，会把传进去的参数给打印出来
Traceback (most recent call last):
File "", line 1, in
AssertionError: one does not equal zero
用法： assert 表达式【，argument】（异常参数可有可无）
