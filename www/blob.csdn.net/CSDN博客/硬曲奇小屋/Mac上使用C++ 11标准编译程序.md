# Mac上使用C++ 11标准编译程序 - 硬曲奇小屋 - CSDN博客





2017年10月25日 16:22:12[HardCookies](https://me.csdn.net/james_616)阅读数：2338








《C++ Primer 5e》是基于C++ 11的标准写的，相比于之前的标准添加了许多新的特性，比如范围for循环。Mac上默认的C++编译器是Clang，我们可以在终端查看。

```
$ g++ --version
Configured with: --prefix=/Applications/Xcode.app/Contents/Developer/usr --with-gxx-include-dir=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/usr/include/c++/4.2.1
Apple LLVM version 8.1.0 (clang-802.0.42)
Target: x86_64-apple-darwin16.7.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

下面我们写一段需要使用C++ 11特性的程序，这里就用范围for循环：

```cpp
// range_for.cpp
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
    vector<int> v{1,2,3,4,5,6,7,8,9};
    for (auto &i : v)
        i *= i;
    for (auto &i : v)
        cout << i << " ";
    cout << endl;
    return 0;
}
```

直接编译`g++ range_for.cpp`会出现错误和警告

![这里写图片描述](https://img-blog.csdn.net/20171025161628739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们需要添加`-std=c++11`才能正常编译， 
`$ g++ -std=c++11 range_for.cpp`

运行编译后的结果`$ ./a.out`可以得到正确的输出： 
`1 4 9 16 25 36 49 64 81`



