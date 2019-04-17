# 如何在vs2010中使用SSE指令集 - 战斗蜗牛的专栏 - CSDN博客





2014年04月05日 01:22:40[vbskj](https://me.csdn.net/vbskj)阅读数：9100








1、主要就是在工程中包含SSE头文件即可，具体如下：




    mmintrin.h               MMX

      xmmintrin.h           SSE

      emmintrin.h          SSE2

      pmmintrin.h          SSE3

      tmmintrin.h           SSSE3

      intrin.h                    SSE4A

      smmintrin.h          SSE4.1

      nmmintrin.h          SSE4.2




2、然后vs2010工程的编译方式最好修改为x64的64位编译。




3、项目属性的“C/C++”->“语言”中，添加openmp支持。




4、使用鲁大师查看你的CPU特征，以本人i5 2410为例，特征为“MMX SSE SSE2 SSE3 SSE4.1 SSE4.2”，就说明该CPU支持SSE 4.2指令集。

![](https://img-blog.csdn.net/20140405012309625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmJza2o=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





5、在vs2010工程中添加相应指令集的头文件，既可以开始SSE编程之旅。



