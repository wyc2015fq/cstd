
# Java实用方法整理（十六）——异常处理方法之 try...catch...finally - manduner的博客 - CSDN博客


2018年08月24日 17:02:19[Manduner_TJU](https://me.csdn.net/manduner)阅读数：47


# 1，描述
finally关键字的特点是：
被finally控制的语句体一定会执行
特殊情况：在执行到finally之前，jvm被强制退出（例如，System.exit(0)）,此时finally控制的语句体不再执行
# 2，实例
![](https://img-blog.csdn.net/20180824170143396?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21hbmR1bmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


