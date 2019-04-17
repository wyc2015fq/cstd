# 在Windows上一键编译各种版本的Protobuf - DoubleLi - 博客园






所需工具 : cmake  for  windows 和  git for windows

原理:protobuf 是google的一个开源项目,其源代码在github上可以下载到,并且源码都采用cmake来构建,所以我们可以把源码下载到本地,然后了利用cmake构建本地工程,然后编译.

步骤一:下载源码

　　复制以下代码,保存到 download_protobuf_source.bat 文件中,运行即可

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake,并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
set PROTOBUF_VESION="3.0.0-beta-4"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf_%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%

::从githug上拉取protobuf源代码
git clone -b %PROTOBUF_VESION% https://github.com/google/protobuf.git %PROTOBUF_PATH%

::从github上拉取gmock
cd %PROTOBUF_PATH%
git clone -b release-1.7.0 https://github.com/google/googlemock.git gmock

::从github上拉取gtest
cd gmock
git clone -b release-1.7.0 https://github.com/google/googletest.git gtest

pause

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

步骤二:编译

　　你可以利用cmake构建你所需要的版本,下面的的例子是构建并编译一个VS2013版本的protobuf



　　例:构建VS2013版本

　　复制以下代码,保存到 build_VS.bat 文件,放到 download_protobuf_source.bat 同级目录,然后执行

　　例如

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723113034076-676387342.png)



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake,并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
::必须与下载的版本一致
set PROTOBUF_VESION="3.0.0-beta-4"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf_%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%
cd %PROTOBUF_PATH%

::设置VS工具集,相当于指定VS版本,取决于VS的安装路径
set VS_DEV_CMD="D:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"
::设置工程文件夹名字,用来区分不同的VS版本
set BUILD_PATH="build_VS2013"
::设置编译版本 Debug Or Release
set MODE="Release"


cd cmake
if not exist %BUILD_PATH% md %BUILD_PATH%

cd %BUILD_PATH%
if not exist %MODE% md %MODE%
cd %MODE%

::开始构建和编译
call %VS_DEV_CMD%
cmake ../../ -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%MODE%
call extract_includes.bat
nmake /f Makefile

echo %cd%
pause

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

当然,你也可以的通过修改上面的的脚本来编译你所需要的VS版本,具体的参数注释的很详细

当进度达到100%的时候,说明编译完成

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723112422451-1308573484.png)

此时,所有的东西都已经生成,包括头文件 和 lib文件

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723112646560-372148427.png)

测试

　　新建VS2013工程,设置好google/protobuf 头文件目录 和 lib库目录,链接到 libprotobuf.lib 或者 libprotobuf-lite.lib ,此处不赘述.

　　新建 protocol.proto 文件,输入以下协议,**需要注意的是,一定要加入 syntax = "proto2" 指定语法规则版本**,否则在执行 protoc.exe 的过程中会报警告.如果不加也没有影响,默认为 proto2 的语法规则

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

// 指定语法规则 proto2 or proto3
syntax = "proto2";

message Book
{
    optional string name = 1;
    optional int32 pages = 2;
    optional float price = 3;
}

message Student
{
    optional int32 age = 1;
    optional string name = 2;
    optional float score = 3;
    repeated Book arrBook = 4;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　新建生成协议脚本 gen.bat ,输入以下内容

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

@echo off & color 0A

:: protoc程序名
set "PROTOC_EXE=protoc.exe"
:: .proto文件名
set "PROTOC_FILE_NAME=protocol.proto"

set "PROTOC_PATH=%cd%"
set "CPP_OUT_PATH=%cd%"

::生成.h和.cc
"%PROTOC_PATH%\%PROTOC_EXE%" --proto_path="%PROTOC_PATH%" --cpp_out="%CPP_OUT_PATH%" "%PROTOC_PATH%\%PROTOC_FILE_NAME%"
pause

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

　　把生成的 protocol.pb.h 和 protocol.pb.cc 加入到刚才的工程

　　例如

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723130238826-1655944032.png)



输入代码:



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

#include <stdio.h>
#include <stdint.h>
#include "protocol.pb.h"
int32_t main()
{
    Student *student1 = new Student();
    student1->set_age(1);
    student1->set_name("tom");
    student1->set_score(98.5);
    for (uint32_t i = 0; i < 5; ++i)
    {
        char name[32] = { 0 };
        sprintf_s(name, 32, "book_%d", i);
        Book *pBook = student1->add_arrbook();
        pBook->set_name(name);
        pBook->set_price(1.2f * (i + 1));
        pBook->set_pages((i + 1) * 15);
    }
    //printf("%s\n", student1->DebugString().c_str());
    char buf[1024] = {0};
    int32_t len = student1->ByteSize();
    student1->SerializeToArray(buf, len);
    printf("btye size = %d\n", len);

    Student student2;
    student2.ParseFromArray(buf, len);
    printf("%s\n", student2.DebugString().c_str());
    getchar();
    return 0;
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



**注意事项:在属性面板中把运行库设置为 MT**

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723122854263-830280651.png)

编译运行,成功,结果如下

![](https://images2015.cnblogs.com/blog/627533/201607/627533-20160723130334341-1005456208.png)

 附:编译MinGW版本protobuf的脚本,与build_VS.bat大同小异

文件:build_MinGW.bat

内容:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

::参考文章 https://github.com/google/protobuf/blob/master/cmake/README.md
::默认当前操作系统已安装 git 和 cmake 和 MinGW,并配置好了环境变量
echo off & color 0A

::设置所需要的Protobuf版本,最新版本可以在github上查到 https://github.com/google/protobuf
::必须与下载的版本一致
set PROTOBUF_VESION="3.0.0-beta-4"
echo %PROTOBUF_VESION%
set PROTOBUF_PATH="protobuf_%PROTOBUF_VESION%"
echo %PROTOBUF_PATH%
cd %PROTOBUF_PATH%


::设置工程文件夹名字
set BUILD_PATH="build_MinGW"
::设置编译版本 Debug Or Release
set MODE="Release"


cd cmake
if not exist %BUILD_PATH% md %BUILD_PATH%

cd %BUILD_PATH%
if not exist %MODE% md %MODE%
cd %MODE%

::开始构建编译
cmake ../../ -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%MODE%
mingw32-make.exe
echo %cd%
pause

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```









