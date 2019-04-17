# make的link_directories命令不起作用 - DoubleLi - 博客园






按照《CMake Practice》中第六章的设置，采用include_directories命令去寻找共享库的路径，src/CMakeLists.txt如下：

```
ADD_EXECUTABLE(main main.c)
INCLUDE_DIRECTORIES(/tmp/include/hello)
LINK_DIRECTORIES(/tmp/lib/)
TARGET_LINK_LIBRARIES(main libhello.a)
```

执行cmake及make后，仍然有link error。

cmake的官网对include_directories的说明如下：

*Specify directories in which the linker will look for libraries.

link_directories(directory1 directory2 …)

Note that this command is rarely necessary. Library locations returned by find_package() and find_library() are absolute paths. Pass these absolute library file paths directly to the target_link_libraries() command. CMake will ensure the linker finds them.*

官网不推荐使用link_directoris，而是推荐使用find_package和find_library寻找共享库的绝对路径，再传给target_link_libraries使用。

按照[这里](http://stackoverflow.com/questions/31438916/cmake-cannot-find-library-using-link-directories)的例子，改写了src/CMakeLists.txt如下：

```
ADD_EXECUTABLE(main main.c)
INCLUDE_DIRECTORIES(/tmp/include/hello)

find_library(LIBHELLO_PATH hello /tmp/lib)
IF(NOT LIBHELLO_PATH)
MESSAGE(FATAL_ERROR "libhello not found")
ENDIF(NOT LIBHELLO_PATH) 

MESSAGE(STATUS ${LIBHELLO_PATH} " found")
TARGET_LINK_LIBRARIES(main ${LIBHELLO_PATH})
```

这下可以编译通过了。









