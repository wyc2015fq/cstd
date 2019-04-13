
# javascript的V8引擎的初步使用 - 大坡3D软件开发 - CSDN博客

2018年04月17日 15:33:07[caimouse](https://me.csdn.net/caimouse)阅读数：579


这段时间在使用javascript，常常地想javascript是怎么样运行呢？又是怎么样从JS代码转换为机器码呢？在深入了解JS运行机制之前，先来运行一下V8引擎也是入门的前奏。由于不能访问chrome浏览器开源网站，因此使用nodejs的代码，Node.js可以看作是javascritp的运行时环境(JsRE)，一方面，它提供了多种可调用的例如读写文件，网络请求，系统信息等API。另一方面，因为CPU执行的是一系列的机器码，它还负责将js代码解释成机器码执行，这部分工作是由V8引擎完成。V8是node的心脏，其诞生之初的目标，就是为了提高js的执行效率，它甚至直接将js编译为本地机器码，以节省一般脚本程序解释执行的时间。所以使用nodejs代码，也可以使用它里面的V8引擎的。
至于怎么样编译nodejs，请参考nodejs的网站，经过编译之后，就会生成下面的lib文件：
v8_base_0.lib;v8_base_1.lib;v8_base_2.lib;v8_base_3.lib;v8_libbase.lib;v8_libplatform.lib;v8_nosnapshot.lib;icui18n.lib;icuucx.lib;icustubdata.lib;
然后再在项目里包含头文件目录：
D:\AI\sample\Nodejs_src\node-v6.11.3\deps\v8\include
接着使用VS2015社区版本，记得一定使用它，目前只支持这个编译器，并且只支持WIN7和之后的系统。
创建一个控制台应用程序，代码如下：
```python
// TestV8.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <malloc.h>  
#include <stdlib.h>
#include <memory>
#include <v8.h>
#include <libplatform/libplatform.h>
#pragma comment(lib,"winmm.lib")

using namespace v8;
class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
	virtual void Free(void* data, size_t) { free(data); }
};
int main(int argc, char* argv[])
{
	V8::InitializeICU();
	Platform* platform = platform::CreateDefaultPlatform();
	V8::InitializePlatform(platform);
	V8::Initialize();
	// 创建一个新的Isolate和并设置为当前使用
	v8::Isolate::CreateParams create_params;
	ArrayBufferAllocator buf;
	create_params.array_buffer_allocator = &buf;
	Isolate* isolate = Isolate::New(create_params);
	
	{
		Isolate::Scope isolate_scope(isolate);
		// 创建栈范围句柄
		HandleScope handle_scope(isolate);
		// 创建上下环境变量
		Local<Context> context = Context::New(isolate);
		// 编译和运行“hello world”脚本
		Context::Scope context_scope(context);
		// 创建一个字符串包含JavaScript的脚本源码
		Local<String> source = String::NewFromUtf8(isolate, "function sum(a, b){return a+b;}; 'Hello' + ', World! ' + sum(1,2);");
		// 编译脚本
		Local<Script> script = Script::Compile(source);
		// 运行脚本代码
		Local<Value> result = script->Run();
		// 输出脚本运行结果，从UTF8字符串转换为普通字符串并打印输出
		String::Utf8Value utf8(result);
		printf("%s\n", *utf8);
	}
	// 删除isolate，释放V8资源，删除平台资源。
	isolate->Dispose();
	V8::Dispose();
	V8::ShutdownPlatform();
	delete platform;
	platform = nullptr;
    return 0;
}
```

运行结果如下：
![](https://img-blog.csdn.net/20180417153121165)
可以看js的脚本运行结果是 Hello, World! 3，后面的3是函数sum(a,b)的计算结果。

### 比特币源码入门教程

### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)

### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)

### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)

### Python游戏开发入门
[http://edu.csdn.net/course/detail/5690](http://edu.csdn.net/course/detail/5690)

### 你也能动手修改C编译器
[http://edu.csdn.net/course/detail/5582](http://edu.csdn.net/course/detail/5582)


