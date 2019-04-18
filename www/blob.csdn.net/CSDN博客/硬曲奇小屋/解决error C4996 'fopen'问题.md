# 解决error C4996: 'fopen'问题 - 硬曲奇小屋 - CSDN博客





2017年10月17日 15:56:17[HardCookies](https://me.csdn.net/james_616)阅读数：236








今天将读高光谱的程序从Mac迁移到Win的VS上，编译、运行结果报错。
`error C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.`
同时它给出了解决方法，那就是在预处理器中使用_CRT_SECURE_NO_WARNINGS.

CRT函数（C Runtime Library = C运行时间库）函数，是微软公司对C/C++语言的扩展。CRT函数就是标准的C语言函数。例如，printf、scanf、strlen、fopen等函数就属于CRT函数。具体的实现方法如下： 


![这里写图片描述](https://img-blog.csdn.net/20171017153910695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


在属性 - C/C++ - 预处理器 - 预处理器定义 中添加_CRT_SECURE_NO_WARNINGS。 

就可以编译成功了！



