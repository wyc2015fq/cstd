# 如何在代码中区分OpenCV版本 如何区分是OpenCV2还是OpenCV3 - DumpDoctorWang的博客 - CSDN博客





2018年07月17日 10:42:01[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1293








# 1、C++

OpenCV在C++中的版本号在<opencv2/core/version.hpp>里面，贴出部分和版本相关的定义

```
// opencv2
#define CV_VERSION_EPOCH    2
#define CV_VERSION_MAJOR    4
#define CV_VERSION_MINOR    13
#define CV_VERSION_REVISION 4
```

```
//opencv3
#define CV_VERSION_MAJOR    3
#define CV_VERSION_MINOR    4
#define CV_VERSION_REVISION 1
#define CV_VERSION_STATUS   ""
```

CV_VERSION_EPOCH是OpenCV2指定主版本号“2”的宏定义，只在OpenCV2中有，因此可使用如下代码来区分是OpenCV2还是OpenCV3

```cpp
#include <iostream>
#include <opencv2/core/version.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

#if CV_VERSION_EPOCH == 2
#define OPENCV2
#elif CV_VERSION_MAJOR == 3
#define  OPENCV3
#else
#error Not support this OpenCV version
#endif

int main() {
#ifdef OPENCV2
    std::cout << "Hello, OpenCV2!" << std::endl;
#elif defined(OPENCV3)
    std::cout << "Hello, OpenCV3!" << std::endl;
#endif
    return 0;
}
```

# 2、Python

```python
import cv2

if __name__ == "__main__":
    version = cv2.__version__
    if version.split('.')[0]=="3":
        print("this is opencv3")
    else:
        print("this is opencv2")
```





