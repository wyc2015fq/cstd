# opencv 1.0下运行正常的代码，后续版本不能正常运行 - 三少GG - CSDN博客
2010年12月09日 17:41:00[三少GG](https://me.csdn.net/scut1135)阅读数：1709标签：[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
在opencv 1.0下运行正常的代码，后续版本不能正常运行:
1、vc的max（）不能用：error C3861: “max”: 找不到标识符；后来发现cxcore.hpp里把它取消了：
namespace cv {
#undef min
#undef max
..........
