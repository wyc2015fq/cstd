# cmake常用命令 - KinboSong的博客 - CSDN博客
2018年03月31日 10:45:43[songjinbo3](https://me.csdn.net/KinboSong)阅读数：114
基本的cmake命令：
1、project<工程名>
2、add_executable(<程序名> <源文件>)
3、add_library(<程序名> [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)
4、set_target_properties(<target> PROPERTIES pro1 value1 pro2 value2 ...)
5、add_subdirectory(<source_dir> <binary_dir>)
6、INCLUDE_DIRECTORIES(dir1 dir2 ...)
7、LINK_DIRECTORIES(dir1 dir2 ...)
8、TARGET_LINK_LIBRARIES
