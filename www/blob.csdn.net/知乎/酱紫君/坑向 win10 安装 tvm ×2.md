# 坑向: win10 安装 tvm ×2 - 知乎
# 

深度学习方面 Win 上的支持吧, 先天原因总是比 Unix 晚一步, 可总得有人踩坑不是, 不然只会越来越坑...

我后来发现我还是得自己编译一遍 LLVM, 他那个预编译版 CMake 调用关系没了...

有时候要库有时候要源码, 我还是得用 CMake 编译一遍....

虽说 LLVM 只是 TVM 的一个可选后端而已, 但我还能不选吗...

我当然是为了把局限在一个框架里跑的模型转化为万能的 LLVM IR 啦, 这功能必选啊. (再说 StackVM 现在就是个坑啊).

去 [LLVM Download Page](https://link.zhihu.com/?target=http%3A//releases.llvm.org/download.html) 把 release 源码下下来, 命名为 LLVM

从git下nightly的话又出什么奇妙的坑就是双倍踩坑了, 找事....

你也别太傻学人家命令行一个 make 就开干, 那玩意儿初编译一遍要一个小时, 如果带 clang 还得翻倍...

我们可是吃饱了撑的VS全家桶玩家啊, 新建一个 build文件夹

cmake 一键生成VS项目了解一下

`cmake -G "Visual Studio 15 2017 Win64" .. -Thost=x64`

注意此时的目录结构, 找到 LLVM\build\lib\cmake\llvm 文件夹, 这里就是我们要找的 CMake 表了.
![](https://pic1.zhimg.com/v2-6c4fb6979624ff65b3c176affd13f3d4_b.jpg)
这不就有了吗, 但很大一部分功能我们并不需要, 我们只要 lib 里的部分, 具体有 LLVMCore, LLVMCodeGen 等七八个, 懒得一个个找出来编译的话整个 Libraries 都编译算了, 加起来时间顶多也就半个小时.
![](https://pic4.zhimg.com/v2-1ef1d167c85679208692e37802f7a1e7_b.png)
注意是 X64 Release 模式, 不然就和论坛这老哥一样了, LLVMDemangle.lib 不存在.

[Unable to install TVM along with LLVM on Windows](https://link.zhihu.com/?target=https%3A//discuss.tvm.ai/t/unable-to-install-tvm-along-with-llvm-on-windows/414/5)

还有这位老哥, LLVMSupport.lib 冲突

[How to install tvm on Windows?](https://link.zhihu.com/?target=https%3A//discuss.tvm.ai/t/how-to-install-tvm-on-windows/1296/3)

至于我是怎么发现这俩老哥的...别说了, 我今天编译了6遍 llvm 了...

CUDA篇太过简单, 下载安装就完事了, PATH都不用调, 略...

然后回到 TVM, 打开 LLVM 和 CUDA(+CUDNN) 支持, 重新生成项目

```bash
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CONFIGURATION_TYPES="Release" .. \
  -DLLVM_DIR=/d/Hybrid/LLVM/build/lib/cmake/llvm
```

 重新编译, conda 的话再重新安装就能使用完全版的 TVM 了.

我们可以写个批处理刷新状态, 不然这么长串命令太蠢了, 比如新建一个 bash, 去他丫的cmd...

```bash
#!/usr/bin/env bash
rm CMakeCache.txt
cmake .. -G "Visual Studio 15 2017 Win64" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CONFIGURATION_TYPES="Release"\
  -DLLVM_DIR=/d/Hybrid/LLVM/build/lib/cmake/llvm
sleep 60
```

这是我的 build 日志, 可以看到正确链接到了 LLVM 和 CUDA

```python
-- Selecting Windows SDK version 10.0.17763.0 to target Windows 10.0.17134.
-- The C compiler identification is MSVC 19.16.27027.1
-- The CXX compiler identification is MSVC 19.16.27027.1
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x64/cl.exe -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Build with RPC support...
-- Build with Graph runtime support...
-- VTA build is skipped in Windows..
-- Found CUDA_TOOLKIT_ROOT_DIR=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2
-- Found CUDA_CUDA_LIBRARY=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/lib/x64/cuda.lib
-- Found CUDA_CUDART_LIBRARY=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/lib/x64/cudart.lib
-- Found CUDA_NVRTC_LIBRARY=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/lib/x64/nvrtc.lib
-- Found CUDA_CUDNN_LIBRARY=CUDA_CUDNN_LIBRARY-NOTFOUND
-- Found CUDA_CUBLAS_LIBRARY=C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/lib/x64/cublas.lib
-- Build with CUDA support
-- Link with static LLVM libraries
-- Found LLVM_INCLUDE_DIRS=D:/Hybrid/LLVM/includeD:/Hybrid/LLVM/build/include
-- Found LLVM_DEFINITIONS=-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS
-- Found TVM_LLVM_VERSION=70
-- Build with LLVM 7.0.1
-- Set TVM_LLVM_VERSION=70
-- Build with contrib.hybriddump
-- Configuring done
-- Generating done
-- Build files have been written to: D:/Hybrid/tvm/build
```

扯上 CUDA 就是不一样, 突然就变大了五倍, 突破了两位数.
![](https://pic3.zhimg.com/v2-ec433f622a4ea41b7b60f5f5187140c6_b.jpg)
然后尝试几个基本功能:

```python
import tvm

A = tvm.placeholder((,), name='A')
B = tvm.placeholder((,), name='B')
C = tvm.compute(A.shape, lambda *i: A(*i) + B(*i), name='C')
s = tvm.create_schedule(C.op)
f = tvm.lower(s, [A, B, C], name="test_add")
m = tvm.build(f, target="llvm")
```

都能跑那就没事了.

我还得琢磨demo里那堆 unix 风代码, 人肉把 makefile 翻译成 cmake

题图: [https://www.pixiv.net/member_illust.php?mode=medium&illust_id=47997678](https://link.zhihu.com/?target=https%3A//www.pixiv.net/member_illust.php%3Fmode%3Dmedium%26illust_id%3D47997678)


