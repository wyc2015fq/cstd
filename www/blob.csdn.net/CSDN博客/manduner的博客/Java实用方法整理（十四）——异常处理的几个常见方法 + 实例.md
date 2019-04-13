
# Java实用方法整理（十四）——异常处理的几个常见方法 + 实例 - manduner的博客 - CSDN博客


2018年08月24日 16:15:45[Manduner_TJU](https://me.csdn.net/manduner)阅读数：55


# 1，方法描述
遇到异常，尤其是编译期间的异常，应该怎么处理呢？通常使用的方法有如下几种：
（1）public String getMessage() ：返回此throwable即该异常的详细消息字符串
（2）public String toString() ：返回抛出异常的简短描述，结果是：
这个对象的类的name
“：”（一个冒号和一个空格）
调用这个对象的getLocalizedMessage()方法的结果
（3）public void printStackTrace() ：获取异常类名和异常信息，以及异常出现在程序中的位置。把这些异常信息输出在控制台
# 2，实例
### （1）getMessage()
![](https://img-blog.csdn.net/20180824160859723?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### （2） public String toString()
![](https://img-blog.csdn.net/20180824161039303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### （3）public void printStackTrace()
![](https://img-blog.csdn.net/2018082416131459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


