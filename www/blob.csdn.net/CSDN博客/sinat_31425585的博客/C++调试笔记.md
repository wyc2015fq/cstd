# C++调试笔记 - sinat_31425585的博客 - CSDN博客
2016年08月18日 19:41:56[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：375

1、在VS2010+opencv2.4.4平台上运行程序时，release模式可以通过，而debug模型出现下面错误：
KernelBase.dll!7562c52f()
 [下面的框架可能不正确和/或缺失，没有为KernelBase.dll加载符号]
  KernelBase.dll!7562c52f()
  opencv_core244.dll!644240fe()
  msvcr100.dll!724b872d()
  opencv_core244.dll!645846c0()
解决方案：新建一个工程项目，将源码拿过来重现跑一下就行了~
2、error C2664: 'void ATL::CStringT<BaseType,StringTraits>::Format (...)
解决方法：项目->属性-配置属性-常规-字符集-多字节字符集
3、错误 1 error LNK2019:无法解析的外部符号_main，该符号在函数___tmainCRTStartup中被引用 E:\MSVCRTD.lib(crtexe.obj)
解决方法：创建一个main函数接口
4、错误 8 error C2447: “{”: 缺少函数标题(是否是老式的形式表?)
原因：文本中有错乱字符！
解决办法：新建一个txt文档，将代码复制过去，然后贴回来。
5、错误：不允许指针指向不完整的类类型  std::ofstream*  database
原因：忘记添加头文件
解决方法：添加头文件#include <fstream>
参考博客：[https://blog.csdn.net/qq10593994/article/details/47110897](https://blog.csdn.net/qq10593994/article/details/47110897)
6、错误：'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
解决方法：在stdafx.h中添加：
#define _CRT_SECURE_NO_DEPRECATE
7、错误： undefined reference to `xxxx'
原因：编译时，未链接对应库文件
解决方法：
```cpp
target_link_libraries(test_face ${TEST_LIBS})
```
