# C# System.BadImageFormatException 解决方法 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月08日 20:50:55[boonya](https://me.csdn.net/boonya)阅读数：5261标签：[C#																[dll																[目标平台																[错误](https://so.csdn.net/so/search/s.do?q=错误&t=blog)
个人分类：[C#																[WorkPractice](https://blog.csdn.net/boonya/article/category/1796195)](https://blog.csdn.net/boonya/article/category/1776321)





出现System.BadImageFormatException 异常有两种情况：程序目标平台不一致&引用dll文件的系统平台不一致。

**异常参考：**

[BadImageFormatException](http://msdn.microsoft.com/zh-cn/library/system.badimageformatexception%28v=vs.100%29.aspx)


**程序目标平台不一致：**


项目>属性>生成：x86 和x64平台

![](https://img-blog.csdn.net/20180708204723873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


注：此种问题一般是编译错误。

**引用dll文件的系统平台不一致**


程序中引用外部dll文件的目标平台的版本x86和x64不一致。

注：此种问题是运行时错误。](https://so.csdn.net/so/search/s.do?q=目标平台&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)




