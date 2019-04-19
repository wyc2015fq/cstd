# MEF实现设计上的“松耦合”（4）：构造函数注入 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4848885.html)
前言：今天十一长假的第一天，本因出去走走，奈何博主最大的乐趣是假期坐在电脑前看各处堵车，顺便写写博客，有点收获也是好的。关于MEF的知识，之前已经分享过三篇，为什么有今天这篇？是因为昨天分享领域服务的时候，用到MEF的注入有参构造函数的方法，博主好奇心重，打算稍微深挖一下，这篇来对此知识点做个总结。
还是将前面三篇的目录列出来，对MEF没有了解的朋友，可以先看看：
- [C#进阶系列——MEF实现设计上的“松耦合”（一）](http://blog.jobbole.com/99221/)
- [C#进阶系列——MEF实现设计上的“松耦合”（二）](http://blog.jobbole.com/99229/)
- [C#进阶系列——MEF实现设计上的“松耦合”（终结篇：面向接口编程）](http://blog.jobbole.com/99235/)
## 一、知识点回顾
我们知道MEF作为IOC的方式之一，它的主要作用是解耦，MEF加上面向接口编程，可以使得你的设计更加灵活。我们知道类的构造函数是可以重载的，我们通过构造函数可以向对象传递参数。那么如果我们的MEF也需要通过构造函数传参怎么办呢？别担心，有我们神奇的ImportingConstructor为您解决。
## 二、代码示例
作为分享代码，博主还是打算用前面DDD里面领域服务用到的那个Demo，现学现卖嘛，O(∩_∩)O~
### 1、准备代码：
作为MEF的导入导出的对象，我们先来看三个仓储接口和实现

C#
```
public interface IUserRepository:IRepository
    {
        IEnumerable GetUsersByRole(TB_ROLE oRole);
    }
　　[Export(typeof(IUserRepository))]
    public class UserRepository:EFBaseRepository,IUserRepository
    {
        
        public IEnumerable GetUsersByRole(TB_ROLE oRole)
        {
            throw new NotImplementedException();
        }
    }
 
    public interface IRoleRepository:IRepository
    {
        
    }
    [Export(typeof(IRoleRepository))]
    public class RoleRepository:EFBaseRepository,IRoleRepository
    {
    }
 
    public interface IUserRoleRepository : IRepository
    {
    }
    [Export(typeof(IUserRoleRepository))]
    public class UserRoleRepository : EFBaseRepository, IUserRoleRepository
    {
    }
```
### 2、构造函数传入单个参数
直接来看代码吧：

C#
```
[Export(typeof(IPowerManagerDomainService))]
    public class PowerManagerDomainService:IPowerManagerDomainService
    {
        private IUserRepository _userRepository = null;
        private IRoleRepository _roleRepository = null;
        private IUserRoleRepository _userroleRepository = null;
        [ImportingConstructor]
        public PowerManagerDomainService(IUserRoleRepository oUserRoleRepository)
        {
            _userroleRepository = oUserRoleRepository;
        }
}
```
为什么通过这里的ImportingConstructor特性就能将参数IUserRoleRepository oUserRoleRepository顺利传进来？还记得前面的准备代码吗，IUserRoleRepository的实现类UserRoleRepository上面标记过导出[Export(typeof(IUserRepository))]，所以这里能将参数顺利导入进来。还是来看看调用代码：

C#
```
[Import]
        public IPowerManagerDomainService powerDomainService { get; set; }
        static void Main(string[] args)
        {
            var oProgram = new Program();
            Regisgter.regisgter().ComposeParts(oProgram);
            Console.ReadKey();
        }
```
来调试代码看看：
![](http://jbcdn2.b0.upaiyun.com/2016/04/b8bc43bc59a261aa696ebdbf4a9e2cb8.png)
### 3、构造函数传入多个参数
其实多个参数和上面单个参数的的也没啥太大区别

C#
```
[ImportingConstructor]
        public PowerManagerDomainService(IUserRepository oUserRepository, IRoleRepository oRoleRepository)
        {
            _userRepository = oUserRepository;
            _roleRepository = oRoleRepository;
        }
```
同样要求IUserRepository 和 IRoleRepository 类型要有对应的Export。
### 4、构造函数参数有多个导出
上面的例子都是默认仓储的接口类型都只有一个导出的情况，当实际项目中，业务逻辑较复杂的时候，某一个接口往往存在多个实现类的导出，这种情况下我们要怎么办呢？比如IUserRepository仓储接口有两个实现类：

C#
```
[Export("userRepository_A", typeof(IUserRepository))]
    public class UserRepository_A:EFBaseRepository,IUserRepository
    {
        
        public IEnumerable GetUsersByRole(TB_ROLE oRole)
        {
            throw new NotImplementedException();
        }
    }
　　[Export("userRepository_B", typeof(IUserRepository))]
    public class UserRepository_B:EFBaseRepository,IUserRepository
    {
        
        public IEnumerable GetUsersByRole(TB_ROLE oRole)
        {
            throw new NotImplementedException();
        }
    }
```
这种情况下，如果我们直接在构造函数里面这样写

C#
```
[ImportingConstructor]
        public PowerManagerDomainService(IUserRepository oUserRepository, IRoleRepository oRoleRepository)
        {
            _userRepository = oUserRepository;
            _roleRepository = oRoleRepository;
        }
```
肯定是会报错的。那么我们的解决方案是：

C#
```
[ImportingConstructor]
        public PowerManagerDomainService([Import("userRepository_A", typeof(IUserRepository))]IUserRepository oUserRepository, IRoleRepository oRoleRepository)
        {
            _userRepository = oUserRepository;
            _roleRepository = oRoleRepository;
        }
```
### 5、多个构造函数的导入
了解了上面那么多，我们还想扩展一下，我们知道构造函数是可以重载的，一个类可以有多个构造函数。那么如果我们想在多个构造函数上面同时标记ImportingConstructor特性，然后根据需要调用不同的构造函数，这样真的行吗？比如我们想这样写：

C#
```
[Export(typeof(IPowerManagerDomainService))]
    public class PowerManagerDomainService:IPowerManagerDomainService
    {
        private IUserRepository _userRepository = null;
        private IRoleRepository _roleRepository = null;
        private IUserRoleRepository _userroleRepository = null;
        [ImportingConstructor]
        public PowerManagerDomainService(IUserRoleRepository oUserRoleRepository)
        {
            _userroleRepository = oUserRoleRepository;
        }
        [ImportingConstructor]
        public PowerManagerDomainService([Import(typeof(IUserRepository))]IUserRepository oUserRepository, IRoleRepository oRoleRepository)
        {
            _userRepository = oUserRepository;
            _roleRepository = oRoleRepository;
        }
　　}
```
到底行不行呢？我们来测一把：
![](http://jbcdn2.b0.upaiyun.com/2016/04/f922162c50ca5be8c5827de57bb151da.png)
愿望是美好的，但异常是残酷的！看异常的具体信息：因为未能选择构造函数进行构造。请确保该类型具有默认构造函数或有一个标记有“System.ComponentModel.Composition.ImportingConstructorAttribute”的构造函数。很显然MEF的ImportingConstructorAttribute特性不支持这种多个构造函数同时标注的情况。将ImportingConstructorAttribute转到定义，发现它也没有其他可用属性
![](http://jbcdn2.b0.upaiyun.com/2016/04/c1a2efd80664c1d828aad85b386ee1eb.png)
难道是博主的需求太奇葩啦？苦思良久，仍未找到解决方案。后来博主仔细想了想，**可能是侧重点的问题，MEF一般情况是和面向接口编程联系起来用的，也就是说正常情况下我们定义的是一个接口类型的变量，例如：**

C#
```
[Import]
        public IPowerManagerDomainService powerDomainService { get; set; }
```
**它允许你有多个接口的实现类，如果你有多个构造函数的需求，完全可以一个接口写多个实现类去做，通过导入不同的实现类去代替不同的构造函数的用法**。也不知道园友有没有更好的解决方案？不吝赐教~~
