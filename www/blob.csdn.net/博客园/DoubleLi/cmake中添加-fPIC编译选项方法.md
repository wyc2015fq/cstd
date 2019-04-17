# cmake中添加-fPIC编译选项方法 - DoubleLi - 博客园






合并openjpeg/soxr/vidstab/snappy等多个cmake库时，为了解决下述问题：

relocation R_X86_64_32 against `.text' can not be used when making a shared object; recompile with -fPIC
分别验证过以下二种方法

第一种 （笔者所采用的，因需修改原始CMakeLists.txt文件，感觉不太方便，非常希望能在cmake命令参数中处理）



add_compile_options(-fPIC)

第二种



set（CMAKE_C_FLAGS“$ {CMAKE_C_FLAGS} -fPIC”）
set（CMAKE_CXX_FLAGS“$ {CMAKE_CXX_FLAGS} -fPIC”）











