# vs2015编译使用GRPC - DoubleLi - 博客园






1.获取源码：位于github上

电脑装有git的直接克隆，未装git下载压缩包也可以

git clone  https://github.com/grpc/grpc.git

cd grpc

git submodule update --init  这条命令在我电脑上不起作用，我采用的办法是，对着grpc文件夹点击鼠标右键，选择

【tortoiseGit】->选择【更新子模块】，如果没有安装Git可以点开github上grpc\third_party目录，可以看到grpc外部链接的子模块的地址，一个个下载即可



2.下载依赖包：opensll 和zlib


个人习惯用vs，那我们开始编译，编译之前需要下载nuget，要下载windows的.exe后缀，网上大多说的都是结合vs的，是错误的，地址：https://dist.nuget.org/index.html，不需要安装，将该文件至于环境变量目录下，即可执行nuget命令，用于依赖包的网络下载。主要是依赖于openssl和zlib库。


1:cd grpc   cd vsprojects

2:nuget restore grpc.sln，在vs命令行窗口执行这句，dos窗口我没试过，不知道是否也可以

3.下载gmock

cd grpc\third_party\protobuf

下载gmock: git clone -b release-1.7.0 https://github.com/google/googlemock.git gmock

cd gmock :git clone -b release-1.7.0 https://github.com/google/googletest.git gtest

上述两个包下载如果没有安装git,直接去下载压缩包

4.下载界面版的cmake

由于probuf没有.sln文件，我下载了界面版cmake,个人认为界面版的cmake不要安装，还比命令行好用，cmake中选择vs std 14 2015即可，后面讲下使用cmake时指定的两个目录：资源目录为有CMakeList的目录,即：third_party\protobuf\cmake，生成sln的目录：grpc\third_party\protobuf\cmake\build\solution，为什么是这个目录呢？

因为后面grpc会用你编译的库，你指定这个目录就不用修改【附加库目录】了，如果你生成库为debug版本，后面多个d，则去掉

5.开始编译protobuf,

打开sln，用vs2015编译即可，这个应该是非常顺利的，不会有什么错误，生成的debug库最好去掉‘d’,比如：libprotobufd.lib改为libprotobuf.lib

      6.开始编译grpc和grpc插件

cd grpc\vsprojects 打开工程既可，编译即可

我只遇到了个错误：error1189,在【预处理器】定义中添加_WIN32_WINNT=0x600，如果报什么链接错误就简单了，大家可以自行查出缺少了哪些库，如果你搞不清库应该放在哪个路径，你可以直接指定库的目录，最后有个关于grpc++dll的子项目没有通过，不用管，因为我们要用的是grpc和grpc++

7.集成


建议大家先采用官方的实例集成，目录：grpc\examples\protos，生成c++文件

官方说明：http://doc.oschina.net/grpc?t=57966

生成命令：在dos里执行即可

$protoc -I ../../protos --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../../protos/route_guide.proto

$protoc -I ../../protos --cpp_out=. ../../protos/route_guide.proto


为什么要采用官方的例子，请看下面说明：

这里我遇到个大坑，java同事把的proto文件发给我，我直接用，怎么集成都有编译错误，注意这里是编译错误，我开始怀疑上面我做错了哪一步，后来把他的proto里的代码拷贝到官方的route_guide.proto,调成和官方一样的格式，比如空格等，后来集成成功，因为这个问题，我反复编译了好几次

grpc等等，浪费了大量时间

如果集成遇到了链接错误：则将对应的库强制包含里面，

include：




libs:







zlib和ssl的库我是通过nuget下的教程，https://www.nuget.org/packages/openssl/


方法一：在VS中用nuget控制台执行命令即可，这个网上有大量教程，简单易用，方法二：你可以自己cmake或者下载

集成demo如下：我的demo有几层命令空间，如果不好理解，请大家参照上面官方的示例











