# Run-Time Check Failure #2 - Stack around the variable  was corrupte问题处理 - fanyun的博客 - CSDN博客
2018年01月21日 19:17:16[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2955
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
        VS编译的过程中，报上面的错误，一般来说都是内存越界。
        把 project->配置属性->c/c++->代码生成->基本运行时检查 为 默认值 就不会报本异常。如果改为其他就有exception。
        解决方案：
       (1).增加所选内存的分配大小。
          假如你的这块程序中，有数组操作，先屏蔽掉，看是否还会出现此类问题，如果有，说明不是所判断的，如果没有，说明此处内存越界。
       (2).使用自动分配内存。
