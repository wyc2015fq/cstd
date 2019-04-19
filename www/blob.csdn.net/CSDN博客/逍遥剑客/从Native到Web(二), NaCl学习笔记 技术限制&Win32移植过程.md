# 从Native到Web(二), NaCl学习笔记: 技术限制&Win32移植过程 - 逍遥剑客 - CSDN博客
2012年11月05日 00:12:36[xoyojank](https://me.csdn.net/xoyojank)阅读数：9254

## 一些限制
相对于Win32来说, NaCl相当于另一个平台, 一些操作系统相关的API需要移植. 除此之外, 参考[Technical Overview](https://developers.google.com/native-client/overview), 还有一些其它的限制:
- 不支持硬件异常
- 不支持创建进程
- 不支持传统的TCP/UDP sockets (有其它方式去实现, RakNet已经有个预览版本)
- 不支持查询可用内存
- 内联汇编必须兼容 Native Client 验证器(使用SDK中的 ncval 工具检查) (一些使用汇编优化的代码(如数学库)可能不能使用)
- Pepper API 必须从主线程调用 (对于引擎的多线程架构有所冲击: IO, OpenGL, Input...)
- 必须同时支持32位和64位 (很多游戏引擎没有考虑64位, 需要解决一些兼容问题)
- C runtime只支持[GLIBC](http://www.gnu.org/software/libc/)和[newlib](http://sourceware.org/newlib/)(一些操作系统相关的API必须改成标准库实现)
- 渲染使用OpenGL ES (需要做一个渲染器)
- Pepper Thread 中不能进行阻塞性的操作 (好多操作都要考虑重新实现, 或者转移到线程)
- 文件只能访问HTTP服务器或者本地Cache中的 (限制了资源加载来源)
这些限制都是为了保证安全性(想想ActiveX为什么失败了)和跨平台(Win/Linux/OSX使用同一个版本)
## 从Win32到NaCl
参考nacl_sdk\vs_addin\examples\hello_nacl_cpp\hello_nacl_cpp.sln
- 使用PPAPI平台编译运行(编译成Chrome DLL插件, 可以调用现有的Win32API, 保证了移植过程的平滑)
- 实现Native Client Framework的抽象类, pp::Instance, pp::Module, 模块创建函数pp::CreateModule()
```cpp
// **** Native Client Framework ****
// The Instance class.
class NaClProjectInstance : public pp::Instance
{
...
};
// The Module class.
class NaClProjectModule : public pp::Module
{
public:
	NaClProjectModule() : pp::Module() {}
	virtual ~NaClProjectModule() {}
	virtual pp::Instance* CreateInstance(PP_Instance instance)
	{
		return new NaClProjectInstance(instance);
	}
};
namespace pp
{
Module* CreateModule()
{
	return new NaClProjectModule();
}
}
```
- 替换WinMain到Instance派生类的Init. 这时由于主循环在Init里, 所以插件会卡在Init函数里
```cpp
// Main message loop, Windows style.
	while (uMsg.message != WM_QUIT)
	{
		if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);
		}
	}
	return uMsg.wParam;
```
- 替换WndProc为回调函数, 这样窗口和NaCl模块就可以同时运行了. 这里的实现比较有意思, 因为没有主线程的完全控制权, 使用了事件机制模拟主循环
```cpp
// Implements Windows window message loop with a callback function.
	void NaClProjectInstance::SendCallback(int result)
	{
		pp::Core* core = pp::Module::Get()->core();
		core->CallOnMainThread(
		    100,
		    factory_.NewCallback(&NaClProjectInstance::HandleWindowMsg),
		    result);
	}
	void HandleWindowMsg(int32_t result)
	{
		MSG uMsg;
		if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);
		}
		SendCallback(0);
	}
```
- 删除创建窗口的代码, 转移画板到浏览器里
- 删除所有WindowsAPI调用
- 编译成NaCl64平台的nexe(此时不能使用VS2010调试, 只能以gdb方式)
![](https://img-my.csdn.net/uploads/201211/05/1352045402_9551.png)
- 可以发布了, hello_nacl_plus目录(包含nexe)拷贝到example目录, 启动HTTP服务器, 像运行示例程序那样运行
可以说, 有了PPAPI插件的帮助, 整个移植过程相当的平滑, 可以一步一步进行, 比起什么android/linux/osx平台的移植工作方便太多了.
接下来就是考虑3D游戏相关的移植工作了, 上面提到的很多技术限制也会引起一些传统结构的调整
