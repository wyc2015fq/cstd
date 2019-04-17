# grpc vs2015编译 - DoubleLi - 博客园






# 获取gRPC源码

gRPC是开源框架，项目代码在github上，所以首先要安装github。
github安装后，在指定文件夹中，执行git命令就可以获取gRPC的所有源码。

git clone  https://github.com/grpc/grpc.git

cd grpc

git submodule update --init



step1. 编译protobuf


参考readme用CMAKE生成工程文件，编译即可。首先打开vs2015开发人员命令提示符窗口，切换到对应的protobuf目录

具体步骤：

1:cd protobuf

2: [Git](http://lib.csdn.net/base/git) clone -b release-1.7.0 https://github.com/google/googlemock.[git](http://lib.csdn.net/base/git) gmock

3:cd gmock

4:git clone -b release-1.7.0 https://github.com/google/googletest.git gtest

5:cd ..\cmake

6:mkdir build & cd build & mkdir install

7:mkdir debug & cd debug

8:cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../install ../..

9:nmake && nmake install

10:生成完成，在install目录下面有对应的Lib文件。在cmake目录下面mkdir debug，然后把install/lib目录下的所有库文件拷贝的debug路径，并把后缀d去掉。例如protobuf生成的库名称为libprotocd.lib，应该改名成libprotoc.lib。其他的依次类推。后面编译grpc会用到这些库。



**step2. 编译grpc，grpc_protoc_plugin：**在vsprojects里有建好的工程文件，下载nuget.exe，用于依赖包的网络下载。主要是依赖于openssl和zlib库。在编译grpc时，出现编译boringssl，出现很多错误，可以把工程移除

具体步骤：

1:cd vsprojects

2:nuget restore grpc.sln

3:msbuild grpc.sln /p:Configuration=Debug

grpc库生成成功。

4：编译grpc_cpp_plugin，执行命令：msbuild grpc_protoc_plugins.sln /p:Configuration=Debug

grpc_cpp_plugin.exe插件编译成功




用vs2015编译基本不会遇到什么问题，除了：grpc_cpp_plugin依赖libprotoc.lib，而protobuf生成的库名称为libprotocd.lib，这块需要手动改一下

**step3. zlib** 参考readme

**step4编译helloworld**

1: cd grpc/example/protos

2: 把proto.exe(protobuf工程生成，在install/bin目录下)、helloworld.proto拷贝到vsprojects\debug路径下面。此时在vsprojects\debug路径下面执行命令





**[cpp]**[view plain](http://blog.csdn.net/cupidove/article/details/53334920#)[copy](http://blog.csdn.net/cupidove/article/details/53334920#)



- protoc.exe -I=. --grpc_out=. --plugin=protoc-gen-grpc=.\grpc_cpp_plugin.exe helloworld.proto  
- protoc.exe -I=. --cpp_out=. helloworld.proto  



生成了两套文件







**[cpp]**[view plain](http://blog.csdn.net/cupidove/article/details/53334920#)[copy](http://blog.csdn.net/cupidove/article/details/53334920#)



- hellowworld.pb.h  
- hellowworld.pb.cc  
- hellowworld.grpc.pb.h  
- hellowworld.grpc.pb.cc  



其中前两个是protoc生成的，后两个是插件生成的



3：在vsprojects目录创建空的WIN32 C++工程grpc_helloworld.sln，在目录grpc_helloworld下面有两个文件夹。目录结构如下图

![](http://img.blog.csdn.net/20161125155022895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://img.blog.csdn.net/20161125155040278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](http://img.blog.csdn.net/20161125155058224?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](http://img.blog.csdn.net/20161125155112684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

工程配置：

头文件包含路径：




**[cpp]**[view plain](http://blog.csdn.net/cupidove/article/details/53334920#)[copy](http://blog.csdn.net/cupidove/article/details/53334920#)



- $(SolutionDir)..\..\grpc\third_party\protobuf\src;$(SolutionDir)..\..\grpc\include;$(ProjectDir)..\protobuf;%(AdditionalIncludeDirectories)  



预处理器设定




**[cpp]**[view plain](http://blog.csdn.net/cupidove/article/details/53334920#)[copy](http://blog.csdn.net/cupidove/article/details/53334920#)



- _DEBUG;_WIN32_WINNT=0x600;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;%(PreprocessorDefinitions)  


代码生成-->运行库设置为 "多线程调试 (/MTd)"



附件库目录设定




**[cpp]**[view plain](http://blog.csdn.net/cupidove/article/details/53334920#)[copy](http://blog.csdn.net/cupidove/article/details/53334920#)



- $(OutDir);$(SolutionDir)packages\grpc.dependencies.openssl.1.0.204.1\build\native\lib\v140\Win32\Debug\dynamic;$(SolutionDir)packages\grpc.dependencies.zlib.1.2.8.10\build\native\lib\v140\Win32\Debug\dynamic\cdecl;%(AdditionalLibraryDirectories)  





附加依赖项设定

libprotobuf.lib;grpc.lib;gpr.lib;grpc++.lib;Ws2_32.lib;libeay32.lib;ssleay32.lib;zlib.lib;%(AdditionalDependencies)



编译工程文件，顺利的话就可以生成对应的exe了。

将gRPC的C++ example的代码拷贝到我们刚创建的项目中，编译，出现一些error：


### 错误A：


Error2
 error C1189:#error :"Please
 compile grpc with _WIN32_WINNT of at least 0x600 (aka Windows Vista)" d:\projects_framework\grpc\include\grpc\impl\codegen\port_platform.h 59 1 Server_Cpp


解决：在项目属性中的`Preprocessor Definitions`中添加`_WIN32_WINNT=0x600`

### 错误B：
`Error    1    error C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'    c:\program files (x86)\microsoft visual studio 12.0\vc\include\xutility    2132    1    Server_Cpp`
解决：在项目属性中的Preprocessor Definitions中添加

```
_SCL_SECURE_NO_WARNINGS
_CRT_SECURE_NO_WARNINGS
```

### 错误C：
`error LNK2038: mismatch detected for 'RuntimeLibrary': value`
解决：只需要主程序和静态库都采用同一种Runtime Libray编译即可。
在项目属性C/C++中的Code Generation的Runtime Library选择Multi-threaded(/MT)

### 错误D：
`Error    2    error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MT_StaticRelease' doesn't match value 'MTd_StaticDebug' in greeter_server.obj    D:\Projects_My\gRPC\gRPC_cpp\Server_Cpp\libprotobuf.lib(generated_message_util.obj)    Server_Cpp`
解决：原因是将debug的配置拷贝到release时，将Preprocessor Definitions的_DEBUG也考到release中了，去掉_DEBUG即可

### 错误E：
`Error    113    error LNK2001: unresolved external symbol _SSL_state    D:\Projects_My\gRPC\gRPC_cpp\Server_Cpp\grpc.lib(ssl_transport_security.obj)    Server_Cpp`
解决：添加openssl的lib（用v120，可能是vs的版本号），具体用v1xx可以根据自己的vs版本来对应


`D:\XXXX\grpc\vsprojects\packages\grpc.dependencies.openssl.1.0.204.1\build\native\lib\v120\Win32\Release\static`


库文件：

```
libeay32.lib
ssleay32.lib
```

### 错误F：
`Error    2    error LNK2005: "private: __thiscall type_info::type_info(class type_info const &)" (??0type_info@@AAE@ABV0@@Z) already defined in LIBCMT.lib(typinfo.obj)    D:\Projects_My\gRPC\gRPC_cpp\Server_Cpp\msvcrt.lib(ti_inst.obj)    Server_Cpp`
解决：
Project Settings:
-> Configration Properties -> Linker -> Input -> Ignore Specific Library: LIBCMT.lib

在编译的时候，有可以还会遇到找不到Grpc内的某个符号，比如Channel、Complete_quque等, 以下以channel为例说明解决方法：

GRPC编译grpc++.lib时，生成的Channel.obj会被覆盖（原因没查出来，大概是因为grpc.lib编译时同样会生成channel.obj）

 删掉obj目录中C:\path\to\grpc\vsprojects\IntDir\grpc++中的channel.obj，然后在工程里“生成”（不要重新生成）










