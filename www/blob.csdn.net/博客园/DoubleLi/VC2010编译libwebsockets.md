# VC2010编译libwebsockets - DoubleLi - 博客园








1. 安装cmake： https://cmake.org/files/v3.6/cmake-3.6.0-win64-x64.msi

2. 下载libwebsocket源码:

git clone https://github.com/warmcat/libwebsockets.git

tag到最后一个稳定版版本: git checkout  v2.0.2

3. 下载openssl 的预编译版本

4. 执行cmake指令

参考:

https://github.com/warmcat/libwebsockets/blob/master/README.build.md

*"C:\Program Files\CMake\bin\cmake" -G "Visual Studio 10" ..  -DOPENSSL_ROOT_DIR="C:\data\TestProjects\libwebsockets-win32(old)\3rdlib\openssl"*

5. 用VC2010打开生成的工程文件: libwebsockets.sln, 执行编译，成功! 

![](https://upload-images.jianshu.io/upload_images/1617677-de1cd1e8da58f7c6.png?imageMogr2/auto-orient/strip)



作者：晕哥哥
链接：http://www.jianshu.com/p/f275e2fa53d9
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。









