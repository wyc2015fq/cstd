# 《UNIX环境高级编程》——UNIX标准及实现 - 一世豁然的专栏 - CSDN博客





2014年10月30日 17:00:08[一世豁然](https://me.csdn.net/Explorer_day)阅读数：836标签：[Linux应用编程](https://so.csdn.net/so/search/s.do?q=Linux应用编程&t=blog)
个人分类：[liunx应用编程](https://blog.csdn.net/Explorer_day/article/category/2276817)









一、UNIX标准化

 1、ISO  C

   1）、ISO  C 标准定义的头文件（P21  图2-1）




 2、IEEE  POSIX

   1）、POSIX：可移植操作系统。

   2）、POSIX标准定义的必需的头文件。（P23  图2-2）

   3）、POSIX标准定义的XSI可选头文件（P24  图2-3）

   4）、POSIX标准定义的可选头文件（P24  图2-4）

   5）、POSIX可选接口组和选项码（P24  图2-5）




 3、Single  UNIX  Specification




 4、FIPS







二、UNIX系统实现（略）







三、标准和实现的关系（略）







四、限制

 1、三种限制

   1）、编译时限制（头文件）。

   2）、与文件或目录无关额运行时限制（sysconf）。

   3）、与文件或目录有关的运行时限制（pathconf和fpathconf）。




 2、ISO  C限制

   1）、ISO  C定义的所有的编译时限制都在头文件<limits.h>中。

   2）、<limits.h>中定义的整型值大小。（P30  图2-6）

   3）、在各种平台上ISO的限制。（P31  图2-7）




 3、POSIX限制

   1）、七类限制和常量

          I、数值限制：LONG  BIT、SSIZE_MAX、和WOED_BIT。

          II、最小值：（P32  图2-8）


          III、最大值：_POSIX_CLOCKRES_MIN。

          IV、运行时可以增加的值：CHARCLASS_NAME_MAX、COLL_WEIGHTS、LINE_MAX、NGROUPS_MAX和RE_DUP_MAX。

          V、运行时不变值（可能不确定）：（P31  图2-9）

          VI、其他不变值：NL_ARGMAX、NL_MSCMAX、NL_SETMAX和NL_TEXTMAX。

          VII、路径名可变值：FILESIZEBITS、LINK_MAX、MAX_CANON、MAX_INPUT、NAME_MAX、PATH_MAX、PIPE_BUF和SYMLINK_MAK。

   2）、XSI限制

          I、最小值：（P33  图2-10）

          II、运行时不变值（可能不确定）：IOV_MAX和PATH_SIZE。

   3）、sysconf、pathconf和fpathconf

          #include<unistd.h>

          long  sysconf(int  name);

          long  pathconf(const  char  *pathname,  int  name);

          long  fpathconf(int  fd,  int  name);

          I、对sysconf的限制及name参数。（P34  图2-11）

          II、对pathconf和fpathconf的限制及name参数。（P35  图2-12）

   4）、不确定的运行时限制

          I、路径名。

          II、最大打开文件数。







五、选项

 1、三种处理选项的方法

    I、编译时选项定义在<unistd.h>中。

    II、与文件或目录无关的运行时选项用sysconf函数来判断。

    III、与文件或目录有关的运行时选项用pathconf和fpathconf函数来判断。




 2、pathconf和fpathconf选项及name参数（P44  图2-18）




 3、sysconf选项及name参数（P45  图2-19）







六、功能宏测试（略）







七、基本系统数据类型（P47  图2-21）







八、标准之间的冲突




