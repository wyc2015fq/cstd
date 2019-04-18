# 编程小知识之 Dispose 模式 - tkokof1的专栏 - CSDN博客

2019年01月14日 18:08:37[tkokof1](https://me.csdn.net/tkokof1)阅读数：34


> 
本文简述了 C# 中 Dispose 模式的一些知识

之前对 C# 中的 Dispose 模式只有些模糊印象,近来又了解了一些相关知识,在此简单做些记录~

C# 程序中每种类型都可以看做是一种资源,这些资源可以分成两类:
- 托管资源 : 受 [CLR](https://en.wikipedia.org/wiki/Common_Language_Runtime) 管理(分配和释放)的资源,譬如 new 出的某个类型对象
- 非托管资源 : 不受 CLR 管理(分配和释放)的资源,譬如某个 native 的文件句柄

对于托管资源,由于受 CLR 的管理,大部分情况下我们都不用操心资源的释放问题,但是对于非托管资源,由于不受 CLR 的管理,释放的问题便必须我们自己来做了.

那么我们通过什么方法来释放这些非托管资源呢, C# 提供了一个标准接口 **IDisposable** :

```
public interface IDisposable
{
	void Dispose();
}
```

如果你程序中的某个类型需要释放非托管资源,就让他实现 **IDisposable** 接口,也就是通过 **void Dispose()** 方法来实现非托管资源的释放, 示例代码如下:

```
// dispose pattern v1
public class DisposePattern : IDisposable
{
	// external unmanaged resource
	IntPtr m_handle;
	
	public DisposePattern(IntPtr handle)
	{
		m_handle = handle;
	}
	
	public void Dispose()
    {
		// release external unmanaged resource
		if (m_handle != IntPtr.Zero)
		{
		    CloseHandle(m_handle);
		    m_handle = IntPtr.Zero;
		}	
	}
	
	// close handle method
	[System.Runtime.InteropServices.DllImport("Kernel32")]
    extern static bool CloseHandle(IntPtr handle);
}
```

上面的示例代码有个很大的问题,如果外部代码没有调用 DisposePattern 的 Dispose 方法,那么 DisposePattern 持有的非托管资源(m_handle)便泄露了.

就编程规范来讲,其实是应该规避外部代码不调用 Dispose 方法的行为,如果这可以做到,那么示例代码中的 Dispose 实现便已经足够了,但是这在实际中往往难以保证(或者说做到保证的成本太高),另外从实现的角度来看, DisposePattern 如果能在外部代码不调用 Dispose 方法的前提下仍然保证非托管资源不泄露,那么程序也会更加健壮.

如何实现呢?我们需要借助 C# 中的析构函数(或者叫终结器)

这里我们暂时不去关注 C# 中析构函数的各个细节,**只要知道析构函数可以在类型被回收之前执行就行了**,新的示例代码如下:

```
// dispose pattern v2
public class DisposePattern : IDisposable
{
	// external unmanaged resource
	IntPtr m_handle;
	
	public DisposePattern(IntPtr handle)
	{
		m_handle = handle;
	}
	
	// destructor
	~DisposePattern()
	{
		// release external unmanaged resource
		if (m_handle != IntPtr.Zero)
		{
		    CloseHandle(m_handle);
		    m_handle = IntPtr.Zero;
		}
	}
	
	public void Dispose()
    {
		// release external unmanaged resource
		if (m_handle != IntPtr.Zero)
		{
		    CloseHandle(m_handle);
		    m_handle = IntPtr.Zero;
		}
		
		// get rid of ~DisposePattern() call
		GC.SuppressFinalize(this);
	}
	
	// close handle method
	[System.Runtime.InteropServices.DllImport("Kernel32")]
    extern static bool CloseHandle(IntPtr handle);
}
```

可以看到我们额外定义了 **~DisposePattern()**,并在其中实现了非托管资源的释放,这就保证了即使外部代码不调用 Dispose 方法,非托管资源也能正确释放(在 DisposePattern 回收之前),相对的,如果外部代码调用了 Dispose 方法,我们便不需要再调用 ~DisposePattern() 了(当然,这里只是说不需要,不是说不可以,这里在 Dispose 之后继续调用 ~DisposePattern() 也是可以的,这也是出于健壮性的考虑), Dispose() 方法中的 **GC.SuppressFinalize(this);** 便是用来"屏蔽"析构函数的执行的(定义了析构函数的类型可以通过调用 GC.SuppressFinalize 来抑制析构函数的执行).

实际的代码中,一个类型除了持有非托管资源,自然也会持有托管资源,如果这些托管资源(类型)也实现了 **IDisposable** 接口(*或者更广义的来说,实现了 Dispose 之类的释放资源方法.这里我们将问题标准化(简化),规定实现释放资源方法就需要实现 **IDisposable** 接口*)

最终的实现代码如下:

```
// dispose pattern v3
public class DisposePattern : IDisposable
{
	// external unmanaged resource
	IntPtr m_handle;
	// managed resource
	Component m_component = new Component();
	// disposed flag
    bool m_disposed = false;
	
	// internal dispose method
	void Dispose(bool disposing)
	{
		if (!m_disposed)
		{
			if (disposing)
			{
				// Dispose managed resources
				m_component.Dispose();
			}
			
			// release external unmanaged resource
			if (m_handle != IntPtr.Zero)
			{
			    CloseHandle(m_handle);
			    m_handle = IntPtr.Zero;
			}
			
			m_disposed = true;
		}
	}
	
	public DisposePattern(IntPtr handle)
	{
		m_handle = handle;
	}
	
	// destructor
	~DisposePattern()
	{
		Dispose(false);
	}
	
	public void Dispose()
    {
		Dispose(true);
		
		// get rid of ~DisposePattern() call
		GC.SuppressFinalize(this);
	}
	
	// close handle method
	[System.Runtime.InteropServices.DllImport("Kernel32")]
    extern static bool CloseHandle(IntPtr handle);
}
```

上面示例中的 DisposePattern 实现便是所谓的 **Dispose 模式**,代码中的几个要点还需要细细说明一下:
- 我们抽象了一个内部的 **void Dispose(bool disposing)** 方法来辅助我们的 Dispose 模式实现,这主要是出于代码可读性和可维护性的考虑.
- 新增加的 **bool m_disposed** 成员主要是为了解决外部代码重复调用(之前说明的是不调用和仅调用一次) Dispose() 方法的问题(之前其实也存在重复调用的问题,只是我们通过 **if (m_handle != IntPtr.Zero)** 这种编码方式规避了)
- void Dispose(bool disposing) 方法的参数 **bool disposing** 的意思,是用来区分 Dispose 调用路径的(是外部代码调用还是析构函数调用),如果是外部代码调用,我们一并释放托管资源和非托管资源,如果是析构函数调用,我们仅释放非托管资源(托管资源在他们各自的析构函数中进行 Dispose),至于为何需要做这种区分,可以简单理解为这是 Dispose 模式的实现规范(**想继续了解的同学可以进一步看看后面的说明**).

#### 参考资料

[改善C#程序的建议4：C#中标准Dispose模式的实现](https://www.cnblogs.com/luminji/archive/2011/03/29/1997812.html)

[IDisposable Interface](https://docs.microsoft.com/en-us/dotnet/api/system.idisposable?view=netframework-4.7.2)

[Why using finalizers is a bad idea](https://www.viva64.com/en/b/0437/)

[Finalize and Dispose](http://www.monitis.com/blog/improving-net-application-performance-part-5-finalize-and-dispose/)

#### 更多说明

之前我们提到, Dispose 模式中区分了 Dispose 的调用路径(如果是外部代码调用,我们一并释放托管资源和非托管资源,如果是析构函数调用,我们仅释放非托管资源),这里可以引出几个问题:
- 如果是外部代码调用,我们可以**不释放托管资源**吗(标准实现是一并释放托管资源和非托管资源)?
- 答案是**可以**的(这些托管资源会在他们各自的析构函数中被 Dispose),只是不符合 Dispose 方法的语义(Dispose 的语义即释放所用资源,包括托管资源和非托管资源).
- 如果是析构函数调用,我们可以**释放托管资源**吗(标准实现是仅释放非托管资源)?
- 答案是**不可以**的,你可以简单理解为在析构函数中不可以引用其他托管资源(其实,实现上来讲,你是可以在析构函数中引用其他托管资源的,只是这些托管资源如果也实现了析构函数(譬如这些托管资源自身实现了 Dispose 模式),那么 CLR 调用这些析构函数的顺序是不定的,这会造成重复释放等问题,再者,如果你引用静态变量来进行托管资源的释放,在 Environment.HasShutdownStarted == true 的情况下更不能安全进行,综上,**你不应该在析构函数中释放托管资源**)

