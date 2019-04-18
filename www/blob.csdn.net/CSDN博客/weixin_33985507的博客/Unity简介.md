# Unity简介 - weixin_33985507的博客 - CSDN博客
2008年11月23日 15:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Unity 是由微软开发的一个轻量级、可扩展的依赖注入(Dependency Injection, DI)容器，支持构造函数、属性和方法的依赖注入。这里不对IOC和DI做太多的介绍，详见：《[IoC 容器和Dependency Injection 模式](http://wiki.entlib.net.cn/martin_fowler_InversionOfControlContainersAndTheDependencyInjectionPattern.ashx)》。本文这里我们直接来看用Unity写的代码。
**器材准备：**
假设我们要做个遥控车，所以我们有以下的类：
首先，我们弄个接收陀机：
public interface 陀机接口
{
    void 前进(int 速度);
    void 后退(int 速度);
    void 左转(int 角度);
    void 右转(int 角度);
}
[](http://11011.net/software/vspaste)
然后，我们把这陀机装到遥控车上：
public class 蚊车基类 : 陀机接口
{
    #region 陀机接口 成员
    public virtual void 前进(int 速度) { }
    public virtual void 后退(int 速度) { }
    public virtual void 左转(int 角度) { }
    public virtual void 右转(int 角度) { }
    #endregion
}
[](http://11011.net/software/vspaste)
public class AWD蚊车 : 蚊车基类
{
    public override void 前进(int 速度)
    {
        Console.WriteLine("AWD蚊车前进");
    }
}
然后，我们再为这个陀机选一个合适的遥控： 
public interface 遥控接口
{
    void 前进(int 速度);
    void 后退(int 速度);
    void 左转(int 角度);
    void 右转(int 角度);
}
[](http://11011.net/software/vspaste)
public class 四通道比例遥控 : 遥控接口
{
    private 陀机接口 _行驶;
    public 四通道比例遥控(陀机接口 行驶)
    {
        this._行驶 = 行驶;
    }
    #region 遥控接口 成员
    public void 前进(int 速度)
    {
        _行驶.前进(速度);
    }
    public void 后退(int 速度) { }
    public void 左转(int 角度) { }
    public void 右转(int 角度) { }
    #endregion
}
器材准备好后，我们就可以开始我们的遥控车之旅了。
**1、构造器注入：**
我们看到上面的四通道比例遥控构造函数需要一个陀机接口来构造一个遥控。我们用Unity来实例化一个四通道比例遥控看看：
IUnityContainer container = new UnityContainer();
container.RegisterType<陀机接口, AWD蚊车>();//注册(映射)需要的注入类型四通道比例遥控 遥控 = container.Resolve<四通道比例遥控>();
遥控.前进(10);
Console.Read();
运行看一下：
![image](https://images.cnblogs.com/cnblogs_com/QLeelulu/WindowsLiveWriter/Unity_17C8/image_thumb.png)
没错，很神奇把。Unity根据我们注册的映射(container.RegisterType<陀机接口, AWD蚊车>())，自动创建了一个构造函数需要的对象的实例。 
**2、属性注入**
我们为AWD蚊车添加一个电池的属性：
public class AWD蚊车 : 蚊车基类
{
    [Dependency]
    public 电池 电池 { get; set; }
    public override void 前进(int 速度)
    {
        Console.WriteLine("AWD蚊车当前使用的电池：" + 电池.品名);
        Console.WriteLine("AWD蚊车前进.当前速度：" + 速度);
    }
}
[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)
 注意上面我为需要注入的电池属性加上了[Dependency]的特性标记。然后看看测试代码：IUnityContainer container = new UnityContainer();
container.RegisterType<陀机接口, AWD蚊车>();
container.RegisterType<电池, 大容量电池>();
四通道比例遥控 遥控 = container.Resolve<四通道比例遥控>();
遥控.前进(10);
Console.Read();
[](http://11011.net/software/vspaste)然后运行看一下：
![image](https://images.cnblogs.com/cnblogs_com/QLeelulu/WindowsLiveWriter/Unity_17C8/image_thumb_1.png)
不少朋友对于要给属性加上一个[Dependency] 特性标记很不爽，认为这样使类看起来很“脏”，而且还入侵到实体的内部了，是很不好的。
那不要紧，我们也可以把[Dependency] 标记去掉，然后在容器中配置要注入的属性，如下这样写：
container.Configure<InjectedMembers>()
    .ConfigureInjectionFor<AWD蚊车>(new InjectionProperty("电池"));
**3、方法注入**
我们先为AWD蚊车添加一个“安装电池”的方法：
//[Dependency]
public 电池 电池 { get; set; }
[InjectionMethod]
public void 安装电池(电池 电池)
{
    this.电池 = 电池;
}
上面我们为“安装电池”方法添加了一个[InjectionMethod]的attribute标记，指明这个方法是个注入方法。
然后我们看一下测试代码：
IUnityContainer container = new UnityContainer();
container.RegisterType<陀机接口, AWD蚊车>();//注册(映射)需要的注入类型
container.RegisterType<电池, 大容量电池>();
四通道比例遥控 遥控 = container.Resolve<四通道比例遥控>();
遥控.前进(10);
Console.Read();
这里的测试代码和前面的属性注入的测试代码是一样的。注意在测试代码中我们并没有去调用“安装电池”方法，不过Unity会在获取该对象的时候(container.Resolve<四通道比例遥控>();)，自动去调用注入的“安装电池”方法，并为这个方法传递他需要的参数的实例： 
![image](https://images.cnblogs.com/cnblogs_com/QLeelulu/WindowsLiveWriter/Unity_17C8/image_thumb_2.png)
运行结果：
![image](https://images.cnblogs.com/cnblogs_com/QLeelulu/WindowsLiveWriter/Unity_17C8/image_thumb_3.png)
如果你确实很讨厌类污染，不能忍受在方法上面添加一个[InjectionMethod] 特性标记的话，你也可以这样写：
container.Configure<InjectedMembers>()
    .ConfigureInjectionFor<AWD蚊车>(new InjectionMethod("安装电池", new ResolvedParameter<电池>()));
[](http://11011.net/software/vspaste)
[](http://11011.net/software/vspaste)
更多信息请见：[http://www.codeplex.com/unity/](http://www.codeplex.com/unity/)
