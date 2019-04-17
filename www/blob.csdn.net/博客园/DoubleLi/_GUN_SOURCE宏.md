# _GUN_SOURCE宏 - DoubleLi - 博客园






**问题描述：在编译程序时，提示一个错误和一个警告。**

error：storage size of tz isn’t know；其中tz是struct timezone类型的变量。

warning：implicit declaration of function lstat；



使用man手册查了一下，发现tz需要加上sys/time.h头文件，lstat需要加上sys/lstat.h头文件，但这两个头文件都已经包含了，百思不得其解。



无意间搜到一种解决方案，在编译时加上-D_GNU_SOURCE(定义_GNU_SOURCE宏)，于是照做，程序编译通过。继续了解了_GNU_SOURCE宏，发现它是在features.h中用于特性控制的一个功能测试宏，而上面提到的两个头文件sys/time.h, sys/stat.h都包含了features.h文件。





**/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */**

#ifdef _GNU_SOURCE

# undef  _ISOC99_SOURCE

# define _ISOC99_SOURCE 1

# undef  _POSIX_SOURCE

# define _POSIX_SOURCE  1

# undef  _POSIX_C_SOURCE

# define _POSIX_C_SOURCE    200809L

# undef  _XOPEN_SOURCE

# define _XOPEN_SOURCE  700

# undef  _XOPEN_SOURCE_EXTENDED

# define _XOPEN_SOURCE_EXTENDED 1

# undef  _LARGEFILE64_SOURCE

# define _LARGEFILE64_SOURCE    1

# undef  _BSD_SOURCE

# define _BSD_SOURCE    1

# undef  _SVID_SOURCE

# define _SVID_SOURCE   1

# undef  _ATFILE_SOURCE

# define _ATFILE_SOURCE 1

#endif



其中_ISOC99_SOURCE, _POSIX_SOURCE, _XOPEN_SOURCE都是功能测试宏，用于指示是否包含对应标准的特性，而这些不同的特性源于过去20多年来进行的各种标准化工作（ANSI, ISO, POSIX, FIPS等），不同的标准支持实现了不同的特性，如系统时间的获取，stat结构是在ANSI标准中是不支持的，而定义了_GNU_SOURCE相当于开启了对所有特性的支持。









