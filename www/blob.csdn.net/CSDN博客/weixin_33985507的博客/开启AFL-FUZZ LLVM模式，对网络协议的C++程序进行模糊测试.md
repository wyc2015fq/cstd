# 开启AFL-FUZZ LLVM模式，对网络协议的C++程序进行模糊测试 - weixin_33985507的博客 - CSDN博客
2017年12月18日 09:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：50
需要下载cfe-3.5.2 llvm-3.5.2  clang-tools-extra-3.5.2 compiler-rt-3.5.2
1、xz -d 解压 这些文件
2、tar xvf 解压上一步解压出来的4个tar文件
3、整合
mv cfe-3.5.2.src clang
mv clang llvm-3.5.2.src/tools
mv clang-tools-extra-3.5.2.src extra
mv extra/ llvm-3.5.2.src/tools/clang/
mv compiler-rt-3.5.2.src compiler-rt
mv compiler-rt llvm-3.5.2.src/projects/
4、编译安装
mkdir build-3.5
cd build-3.5/
../llvm-3.5.2.src/configure --enable-optimized --enable-targets=host-only
make -j 4
make install
5、编译安装afl-fuzz的llvm模块
cd afl-2.50b/
cd llvm_mode/
make
make install
然后进入 afl-2.50b目录。重新make install 激活安装成功的afl-fast模块
6、修改head.cmake
SET(CMAKE_CXX_COMPILER "afl-clang-fast++")
SET(CMAKE_C_COMPILER "afl-clang-fast")
//修改编译器为 afl-clang-fast++ 和 afl-clang-fast++
7、修改main.cpp
while (__AFL_LOOP(1000))
{
string data;
ifstream file;
file.open(argv[3]);
file>>data;
int dst_port = x000;
char *dst_ip = "127.0.0.1";
bzero(&serveraddr,sizeof(serveraddr));
serveraddr.sin_family = AF_INET;
serveraddr.sin_addr.s_addr = inet_addr(dst_ip);
serveraddr.sin_port = htons(dst_port);
//socket send
int sockfd;
sockfd = socket(AF_INET, SOCK_STREAM, 0);
connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
send(sockfd, data.c_str(), data.size(), 0);
printf("Send OK!\n");
file.close();
}
8、进行模糊测试
进入模块的bin目录
创建 in out目录，放置好初始用例
afl-fuzz -i in -o out -m none ./XXX ../conf/conf.xml @@
