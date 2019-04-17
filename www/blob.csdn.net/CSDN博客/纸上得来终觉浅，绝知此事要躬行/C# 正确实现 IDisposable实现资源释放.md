# C# 正确实现 IDisposable实现资源释放 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月04日 13:04:13[boonya](https://me.csdn.net/boonya)阅读数：1140








文章来自：[https://msdn.microsoft.com/zh-cn/library/ms244737(v=vs.80).aspx](https://msdn.microsoft.com/zh-cn/library/ms244737(v=vs.80).aspx)


## 正确实现IDisposable原因

未正确实现 **IDisposable**。下面列出了产生此问题的一些原因：
- 
IDisposable 在类中重新实现。

- 
Finalize 被再次重写。

- 
Dispose 被重写。

- 
Dispose() 不是公共、密封或命名的 Dispose。

- 
Dispose(bool) 不是受保护的、虚拟的或未密封的方法。

- 
在未密封类型中，Dispose() 必须调用 Dispose(true)。

- 
对于未密封类型，Finalize 实现不调用 Dispose(bool) 和事例类终结器中的一个或两个。

- 
与上述任何模式冲突都将触发此警告。

- 
每个未密封的根 IDisposable 类型都必须提供其各自的受保护虚拟 void Dispose(bool) 方法。Dispose() 应当调用 Dipose(true)，Finalize 应当调用 Dispose(false)。如果创建未密封的根 IDisposable 类型，必须定义 Dispose(bool) 并调用它。

- 
下面的伪代码提供一个说明如何在使用托管和本机资源的类中实现 Dispose(bool) 的常规示例。




```
public class Resource : IDisposable 
{
    private IntPtr nativeResource = Marhsal.AllocHGlobal(100);
    private AnotherResource managedResource = new AnotherResource();

// Dispose() calls Dispose(true)
    public void Dispose()
    {
        Dispose(true);
        GC.SuppressFinalize(this);
    }
    // NOTE: Leave out the finalizer altogether if this class doesn't 
    // own unmanaged resources itself, but leave the other methods
    // exactly as they are. 
    ~Resource() 
    {
        // Finalizer calls Dispose(false)
        Dispose(false);
    }
    // The bulk of the clean-up code is implemented in Dispose(bool)
    protected virtual void Dispose(bool disposing)
    {
        if (disposing) 
        {
            // free managed resources
            if (managedResource != null)
            {
                managedResource.Dispose();
                managedResource = null;
            }
        }
        // free native resources if there are any.
        if (nativeResource != IntPtr.Zero) 
        {
            Marshal.FreeHGlobal(nativeResource);
            nativeResource = IntPtr.Zero;
        }
    }
}
```


#### 规则说明

所有的 IDisposable 类型都应当正确实现 Dispose 模式。



如何修复冲突


检查代码并确定下面的哪些解决方案能够修复此冲突。
- 
从 {0} 实现的接口列表中移除 IDisposable，而改为重写基类 Dispose 实现。 

- 
从类型 {0} 中移除终结器，重写 Dispose(bool disposing)，并在“disposing”为 false 的代码路径中放入终结逻辑。 

- 
移除 {0}，重写 Dispose(bool disposing)，并在“disposing”为 true 的代码路径中放入释放逻辑。 

- 
确保将 {0} 声明为 public 和 sealed。 

- 
将 {0} 重命名为“Dispose”，并确保将它声明为 public 和 sealed。 

- 
确保将 {0} 声明为 protected、virtual 和 unsealed。 

- 
修改 {0} 以便它调用 Dispose(true)，然后对当前对象实例（在 Visual Basic 中为“this”或“Me”）调用 GC.SuppressFinalize，然后返回。 

- 
修改 {0} 以便它调用 Dispose(false)，然后返回。 

- 
如果编写未密封的根 IDisposable 类，请确保 IDisposable 的实现遵循上面描述的模式。





何时排除警告


不要排除与该规则有关的警告。









