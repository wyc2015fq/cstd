# 如何将Emacs打造成现代的C/C++ IDE(三 ccls） - alw2009的博客 - CSDN博客
2018年12月31日 14:10:58[traveler_zero](https://me.csdn.net/alw2009)阅读数：512
今天的目标是让emacs可以进行头文件补全， 关键字补全， 基于语法和语义的补全。包括以下步骤：
0. 准备
`sudo yum install -y libxml2 libxml2-devel`
一、下载和编译clang （我的是centos操作系统， 如果是ubuntu可以省略这步）
参考[https://llvm.org/docs/GettingStarted.html#for-developers-to-work-with-a-git-monorepo](https://llvm.org/docs/GettingStarted.html#for-developers-to-work-with-a-git-monorepo)
需要先编译好gcc7或更高版本的编译器， 参考：[如何在Linux上安装最新版本的gcc](https://mp.csdn.net/postedit/85393497)。
```bash
git clone https://git.llvm.org/git/llvm.git
git clone https://git.llvm.org/git/clang.git llvm/tools/clang
cd llvm
cmake -H. -BRelease -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DLLVM_TARGETS_TO_BUILD=X86 -DCMAKE_CXX_COMPILER=$HOME/newlocal/gcc/bin/g++ -DCMAKE_C_COMPILER=$HOME/newlocal/gcc/bin/gcc -DCMAKE_INSTALL_PREFIX=$HOME/newlocal/llvm -DLLVM_ENABLE_RTTI=ON
cd Release
make clangFormat clangFrontendTool clangIndex clangTooling clang
```
二、下载编译ccls
参考：[https://github.com/MaskRay/ccls/wiki/Getting-started](https://github.com/MaskRay/ccls/wiki/Getting-started)
注意：编译ccls需要支持c++17的编译器， 也可以用第一步编译好的clang++，[但是还需要编译llvm的libc++](https://libcxx.llvm.org/)。 这里使用g++。
```
git clone --depth=1 --recursive https://github.com/MaskRay/ccls
cd ccls
cmake -H. -BRelease -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_RTTI=ON -DCMAKE_CXX_COMPILER=$HOME/newlocal/gcc/bin/g++ -DSYSTEM_CLANG=ON -DCMAKE_PREFIX_PATH="$HOME/github/llvm/Release;$HOME/github/llvm/Release/tools/clang;$HOME/github/llvm;$HOME/github/llvm/tools/clang"
cmake --build Release
export PATH=$(pwd)/Release:$PATH
```
三、配置.spacemacs
在dotspacemacs-configuration-layers将c-c++层修改为如下
```
(c-c++ :variables
       c-c++-backend 'lsp-ccls)
```
四、效果演示
![](https://img-blog.csdnimg.cn/20181231141035306.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2FsdzIwMDk=,size_16,color_FFFFFF,t_70)
