# windows上用libnfc的库函数编程 - xqhrs232的专栏 - CSDN博客
2013年11月28日 10:03:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：996
原文地址::[http://blog.sina.com.cn/s/blog_a2ae2da90101m1kp.html](http://blog.sina.com.cn/s/blog_a2ae2da90101m1kp.html)
相关网帖
1、Libnfc-1.6.0在Windows下的编译过程----[http://wenku.baidu.com/link?url=vzbC5JrQ-sj6v7BlZF9d-693N_rc7KLcz8TyG171Ot9w35cOmQIfvjdXYD7bMxbvQwLO8g60Yf4dVu0TfLfPKcQ36ZJkJ0F77jiUChpptmy](http://wenku.baidu.com/link?url=vzbC5JrQ-sj6v7BlZF9d-693N_rc7KLcz8TyG171Ot9w35cOmQIfvjdXYD7bMxbvQwLO8g60Yf4dVu0TfLfPKcQ36ZJkJ0F77jiUChpptmy)
2、为神马android NFC不能模拟RFID Tag----[http://blog.sina.com.cn/s/blog_a2ae2da90101m1q6.html](http://blog.sina.com.cn/s/blog_a2ae2da90101m1q6.html)
## [windows上怎么用libnfc的库函数编程](http://www.cnblogs.com/duanguyuan/archive/2013/03/11/2953923.html)
根据libnfc的说明文档，我在win7上装了
libnfc-1.7.0-rc6.Zip
MinGW64
libusb-win32-bin-1.2.6.0
CMake -2.8.10-win32-x86
然后用mingw32-make命令编译成功，并产生了libnfc.dll文件，并且能够顺利运行nfc-list.exe等例子。
但是我想问一下，如果我想自己写一个.c文件，我该如何调用libnfc的头文件库和函数库编译它呢？
//test.cpp
#include
#include "libnfc_read_only\include\nfc\nfc.h"
using namespace std;
int main() {
cout << nfc_version() << "\n";
return 0;
}
我用 gcc -o test test.cpp -lnfc 命令编译，会报错说找不到头文件。我将之前编译成功的libnfc.dll文件放在同一目录下，仍然报错。
求指导:-)
解决方法：
（因为之前是在libnfc社区发的贴，回帖也是那里回的，所以直接粘贴过来了。我的英文有点烂，应该大意是能看懂的~）
I got it!
Thanks a lot for yobibe's help. I finally compiled it successfully, the steps are as follow:
Before that, I'll describe my files and directories:
The operating system is windows 7-32bit.
E:.
├─libnfc-1.7.0-rc6
│ ├─cmake
│ ├─contrib
│ ├─examples
│ ├─include
│ ├─libnfc
│ ├─libusb
│ ├─m4
│ ├─test
│ └─utils
│ ├─nfc-list.c
│ ├─CmakeList.txt
│ └─test.c
│
└─nfc_built
├─utils
│ ├─nfc-list.exe
│ └─libnfc.dll
├─libnfc
│ └─libnfc.dll
│
└─(and so on)
when the operation "Cmake-->mingw32-make" achieved, copy the E:\nfc_built\libnfc\libnfc.dll to E:\nfc_built\utils, and then open the cmd.exe window:
C:\Users\WangYong> cd :e\libnfc\nfc_built\utils
e\libnfc\nfc_built\utils> nfc-list.exe
nfc-list.exe will run and then print the result.
If you write a program by yourself, for example:
//test.c
#include
#include
int main()
{
printf("%s",nfc_version());
return 0;
}
Next you should put the test.c in the E:\libnfc\libnfc-1.7.0-rc5\utils, and modify the CmakeList.txt which is under the same directory(E:\libnfc\libnfc-1.7.0-rc5\utils) by adding the "test" to CmakeList.txt:
SET(UTILS-SOURCES
nfc-emulate-forum-tag4
nfc-list
nfc-mfclassic
nfc-mfultralight
nfc-read-forum-tag3
nfc-relay-picc
nfc-scan-device
test
)
Finally, do the mingw32make again, and then you will see the test.exe under E:\libnfc\nfc_built\utils. Run it:
E:\libnfc\nfc_built\utils> test.exe
ok! The version of libnfc will be printed on the cmd screen. You can also compile and run the quick_start_example1.c by this way. Good luck!
