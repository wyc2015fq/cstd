# 单件模式的基类，C++ - 长歌行 - CSDN博客





2013年01月25日 23:54:36[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：2364
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)

所属专栏：[初探设计模式（C++实现）](https://blog.csdn.net/column/details/baadf00df00dabadfds0.html)









看《游戏编程精粹1》中间有一段代码



```cpp
#include <cassert>
 
 
 // 一个单件模式的模板类
 template<typename T>
 class SingleTon
 {
 public:
 	static T* ms_SingleTon;
 
 	SingleTon()
 	{
 		assert(!ms_SingleTon);
 		int offset = (int)(T*)1 - 
 			(int)(SingleTon<T>*)(T*)1;
 		ms_SingleTon = (T*)((int)this + offset);
 	}
 	~SingleTon()
 	{
 		assert(ms_SingleTon);
 		ms_SingleTon = 0;
 	}
 
 	static T& GetSingleTon()
 	{
 		assert(ms_SingleTon);
 		return *ms_SingleTon;
 	}
 
 	static T* GetSingleTonPtr()
 	{
 		return ms_SingleTon;
 	}
 };
 
 // 初始化
 template<typename T> T* SingleTon<T>::ms_SingleTon = 0;
 
 
 // 派生出来的单件类
 class Texture;
 class TextureMgr : public SingleTon<TextureMgr>
 {
 public:
 	Texture *GetTexture(const char *pName)
 	{
 		return NULL;
 	}
 protected:
 private:
 };
 
 #define g_TextureMgr TextureMgr::GetSingleTon()
```


用于实现一个单件模式的基类，其他只有一个实例的类可以由此类派生出来，例如纹理管理类等

1.使用这种方式可以在一定的作用域类进行指针的自创建和销毁，不用担心会创建出多余的对象和指针。

2.而且提供一种派生的手段。

其中    int offset = (int)(T*)1 - (int)(SingleTon<T>*)(T*)1;

这句我理解的不是很清楚。大概是计算一下特化的模板类的指针和特化的类的指针在内存中的偏移值（实际占用的位数的偏移。。。？？）。

然后根据这个偏移对特化的模板类进行偏移。。

但是我调试的offset一直是0啊

int a = (int)(int*)1;
int b = (int)(double*)1;

他们的值都是1

应该就是数字1在内存中的表示(0x00000001)，然后转换为整形/双精度浮点型/或者其他类型，然后在显示其内存地址。。

同一个编译环境应该都是一样的吧。。不太清楚。。




回顾一下一般的方法吧，貌似是不能派生，而且是要用指针来实现，不能够自动销毁内存的。



```cpp
// 一般使用的方法
 class Ston
 {
 public:
 	static Ston *ms_pSton;
 
 	static Ston* CreatePtr()
 	{
 		if (ms_pSton)
 		{
 			return ms_pSton;
 		}
 		else
 		{
 			ms_pSton = new Ston;
 			return ms_pSton;
 		}
 	}
 };
 
 Ston* Ston::ms_pSton = NULL;
```


再粘贴一段两者的使用



```cpp
void TestFunc1()
 {
 	TextureMgr	tMgr;
 	Texture *pTexStone = g_TextureMgr.GetTexture("stone");
 }
 void TestFunc2()
 {
 	Ston *pSingleton = Ston::CreatePtr();
 	if (pSingleton)
 	{
 		delete pSingleton;
 		pSingleton = NULL;
 	}
 }
 
 
 
 int _tmain(int argc, _TCHAR* argv[])
 {
 
 	int a = (int)(int*)1;
 	int b = (int)(double*)1;
 
 	TestFunc1();
 
 	TestFunc2();
 
 
 	Texture *pTexStone = g_TextureMgr.GetTexture("stone");
 	return 0;
 }
```


当然在Texture *pTex.....一句会有一个断言提示。因为对象已经被销毁了嘛
            


