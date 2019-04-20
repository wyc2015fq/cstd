# 使用google breakpad生成dump文件 - sandro_zhang的专栏 - CSDN博客
2011年09月16日 15:36:27[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：813标签：[google																[http服务器																[exception																[callback																[pointers																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=pointers&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)
个人分类：[google breakpad](https://blog.csdn.net/sandro_zhang/article/category/888645)
使用google breakpad生成dump文件 
breakpad的主要功能就是生成dump文件，还可以将dump文件发送的远程http服务器。 
breakpad的SVN在这里http://google-breakpad.googlecode.com/svn/trunk 
linux,mac,solaris,windows下面产生dump文件的例子文件在这几个文件里面，他们的流程几乎是一样的。 
src/client/linux/handler/exception_handler_test.cc 
src/client/mac/handler/exception_handler_test.cc 
src/client/solaris/handler/exception_handler_test.cc 
src/processor/testdata/test_app.cc 
首先编译breakpad中的exception_handler库，然后链接过去就可以了。 
另外src\client\windows\tests\crash_generation_app这个测试工程总是dump失败，谁知道告诉我。 
mozilla有一个完全跨平台的dump toolkit，在这里可以找到源代码 
http://mxr.mozilla.org/mozilla/source/toolkit/crashreporter/nsExceptionHandler.h 
http://mxr.mozilla.org/mozilla/source/toolkit/crashreporter/nsExceptionHandler.cpp 
就拿上面windows的例子来说，代码如下。
```cpp
#include <cstdio>
#include "client/windows/handler/exception_handler.h"
namespace {
	static bool callback(const wchar_t *dump_path, const wchar_t *id,
		void *context, EXCEPTION_POINTERS *exinfo,
		MDRawAssertionInfo *assertion,
		bool succeeded) {
			if (succeeded) {
				printf("dump guid is %ws\n", id);
			} else {
				printf("dump failed\n");
			}
			fflush(stdout);
			return succeeded;
	}
	static void CrashFunction() {
		int *i = reinterpret_cast<int*>(0x45);
		*i = 5;  // crash!
	}
}  // namespace
int main(int argc, char **argv) {
	google_breakpad::ExceptionHandler eh(
		L".", NULL, callback, NULL,
		google_breakpad::ExceptionHandler::HANDLER_ALL);
	CrashFunction();
	printf("did not crash?\n");
	return 0;
}
```
运行完之后会在当前目录下生成一个dmp文件，用windbg打开，在命令行中输入.ecxr，然后查看调用崩溃现场，图片如下。 
其他平台可以用gdb查看崩溃现场。 
![](http://dl.iteye.com/upload/attachment/102736/33b1fc09-6d4f-3f88-a5f3-a946bff1ec64.png)
感谢:[http://zhangyafeikimi.iteye.com/blog/384351](http://zhangyafeikimi.iteye.com/blog/384351)
