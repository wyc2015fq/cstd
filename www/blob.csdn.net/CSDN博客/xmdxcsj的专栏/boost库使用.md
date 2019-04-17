# boost库使用 - xmdxcsj的专栏 - CSDN博客





2017年04月22日 08:57:04[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1082








[boost C++库简介](https://zh.wikipedia.org/zh-cn/Boost_C%2B%2B_Libraries)
[linux下载安装](http://www.boost.org/doc/libs/1_64_0/more/getting_started/unix-variants.html)

大部分库不需要编译，使用的时候只需要include相应的hpp文件即可，少数库需要提前编译。 

假设下载的库文件解压到目录/path/to/boost 

使用boost数学库中的expint函数（matlab）
```cpp
#include "boost/math/special_functions/expint.hpp"
#include <iostream>

int main()
{
    using namespace boost::math;
    //matlab: x->expint(x)
    //boost:  x->-expint(-x)
    float x = -1.2;
    float y = (-1) * expint(x);
    std::cout << y << std::endl;
}
```

在Makefile增加boost的安装路径即可

```bash
BOOST_DIR = /path/to/boost
INCLUDE = -I$(BOOST_DIR)
```




