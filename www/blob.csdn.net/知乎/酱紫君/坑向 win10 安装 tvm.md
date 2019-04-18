# 坑向: win10 安装 tvm - 知乎
# 

我怀疑tvm团队中没有人用win的...所有的demo都默认你在用*unix, 所有的脚本在 win 下都是无法运行的...

我们现在有一台崭新的 win10, 假设你已经知道怎么用 git, conda 和 VS全家桶了, 跳过.

先装 cmake: [Download | CMake](https://link.zhihu.com/?target=https%3A//cmake.org/download/), PATH 自动设置一下
![](https://pic4.zhimg.com/v2-521a08c2a0d8d2b327646d2c4437227b_b.jpg)
然后 llvm 和 clang 装一下 [http://releases.llvm.org/download.html](https://link.zhihu.com/?target=http%3A//releases.llvm.org/download.html)

拉到下面有个win的预编译版本, 别傻乎乎的去自己编译...

当然这有个坑, 你选什么他都不会帮你加PATH.

```bash
setx /M Path "%PATH%;C:\Program Files\LLVM\bin";
clang -v
pause
```

Admin 权限下注册一下 Path, 显示 clang 版本号就对了
![](https://pic1.zhimg.com/v2-c50b7f77d6426fcddb82b342d5f9a8f8_b.jpg)
接下来就 把项目下下来自己编译

```
git clone https://github.com/dmlc/tvm.git
cd tvm mkdir build && cd build
cmake -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES="Release" ..
```

然后生成了一堆乱七八糟的东西
![](https://pic3.zhimg.com/v2-10c0eaa1772ef875252cd5f6b2c8c48e_b.jpg)
双击 tvm.sln 打开项目
![](https://pic4.zhimg.com/v2-b461906867ea5b244f045b04d30cbbe7_b.jpg)
有什么需要可以在 CMakeList 里调

USE_LLVM 指向 "C/Program_Files/LLVM/bin", 必须是 unix 风格的路径

右键点生成, 然后你会看见成千上万行的模板错误, dll 错误, 最后反正build成功就行...

然后 Release 文件夹里有一堆的东西
![](https://pic1.zhimg.com/v2-6b96234087c3f9472d7347112b8e97a0_b.jpg)
然后加入 PythonPath 就能用了, 比如我clone到了`D:\Hybrid\tvm` 就这么写:

```bash
setx PYTHONPATH "%PYTHONPATH%;D:\Hybrid\tvm\python";
setx PYTHONPATH "%PYTHONPATH%;D:\Hybrid\tvm\build\Release";
```

但是如果你用 conda 那就还要多一步, 你需要把 tvm 文件夹下的内容和 Release Build 里的 dll 全都丢进你创建的那个 env/Lib 里.

然后在你的 conda 环境中使用pip装上依赖:

```bash
pip install numpy decorator attrs tornado psutil xgboost
```

这么多事本来都可以一句 `conda install tvm -y` 搞定的...

题图: [https://www.pixiv.net/member_illust.php?mode=medium&illust_id=71386007](https://link.zhihu.com/?target=https%3A//www.pixiv.net/member_illust.php%3Fmode%3Dmedium%26illust_id%3D71386007)

