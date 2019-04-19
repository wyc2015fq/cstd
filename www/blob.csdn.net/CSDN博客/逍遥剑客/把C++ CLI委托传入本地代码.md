# 把C++/CLI委托传入本地代码 - 逍遥剑客 - CSDN博客
2010年10月13日 11:26:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4360
参考: [http://resnikb.wordpress.com/2009/05/18/passing-ccli-delegate-to-native-code/](http://resnikb.wordpress.com/2009/05/18/passing-ccli-delegate-to-native-code/)
C++/CLI可以直接执行C++, 这个没有问题, 那么反过来呢?
比如底层C++进行一项任务, 完成了需要通知上层的C++/CLI, 总不能在上面不停地查询吧?
通常这是通过回调来实现的, 说漂亮点就是观察者模式, 说成.net的就是委托.
而委托, 本质是就是函数指针. 以前也提到过[C++委托](http://blog.csdn.net/xoyojank/archive/2009/06/15/4271600.aspx)的实现.
.net提供了一个方法把委托转换成函数指针: [Marshal::GetFunctionPointerForDelegate](http://msdn.microsoft.com/en-us/library/system.runtime.interopservices.marshal.getfunctionpointerfordelegate.aspx)
 跟String的转换一样, 需要注意保证内存指针不会被托管机制移动/回收.
// DelegateCallback.cpp : main project file.
#include "stdafx.h"
#include <assert.h>
#pragma unmanaged
typedef void (__stdcall* EventCallback)(int);
class Native
{
public:
	/// 注册回调函数
	static void RegisterCallback(EventCallback callback)
	{
		assert(0 != callback);
		ms_callback = callback;	
	}
	/// 调用注册的回调函数
	static void Invoke(int i)
	{
		assert(0 != ms_callback);
		ms_callback(i);
	};
private:
	static EventCallback ms_callback;
};
EventCallback Native::ms_callback = 0;
#pragma managed
using namespace System;
using namespace System::Runtime::InteropServices;
public delegate void EventDelegate(int i);
ref class NativeInterface
{
public:
	NativeInterface()
	{
		// 从成员函数创建一个委托
		this->nativeCallback = gcnew EventDelegate(this, &NativeInterface::Callback);
		// 保证委托不会被内存移动和垃圾回收掉
		this->delegateHandle = GCHandle::Alloc(this->nativeCallback);
		// 转换为函数指针注册
		IntPtr ptr = Marshal::GetFunctionPointerForDelegate(this->nativeCallback);
		Native::RegisterCallback( static_cast<EventCallback>(ptr.ToPointer()) );
	}
	~NativeInterface()
	{
		// 释放委托句柄
		if (this->delegateHandle.IsAllocated)
			this->delegateHandle.Free();
	}
private:
	void Callback(int i)
	{
		Console::WriteLine("托管命令行输出: {0}", i);
	}
private:
	GCHandle delegateHandle;
	EventDelegate^ nativeCallback;
};
//------------------------------------------------------------------------------
int main(array<System::String^>^ args)
{
	NativeInterface^ ni = gcnew NativeInterface();
	// 这个可以在native c++中调用
	Native::Invoke(12345);
	delete ni;
	ni = nullptr;
    return 0;
}
![](http://hi.csdn.net/attachment/201010/13/0_1286939869HCf8.gif)
