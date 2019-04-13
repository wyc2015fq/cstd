
# windows 64位  未能加载文件或程序集“XXX.XXX”或它的某一个依赖项解决方法 - 追求卓越,做到专业 - CSDN博客


2013年06月19日 16:39:52[Waldenz](https://me.csdn.net/enter89)阅读数：1725


操作系统为64位，未能加载文件或程序集“XXX.XXX”或它的某一个依赖项。试图加载格式不正确的程序。
说明:执行当前 Web 请求期间，出现未处理的异常。请检查堆栈跟踪信息，以了解有关该错误以及代码中导致错误的出处的详细信息。
异常详细信息:System.BadImageFormatException: 未能加载文件或程序集“XXX”或它的某一个依赖项。试图加载格式不正确的程序。
![](https://img-blog.csdnimg.cn/20190103200315933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
解决方法：
将应用程序 生成为  目标平台：Any CPU;
测试环境是  win7 64位，把iis 应用程序池 --高级设置--启用32位应用程序 ：true

