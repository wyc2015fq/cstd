# 小话设计模式原则之（3）：接口隔离原则ISP - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/5266209.html)
前言：有朋友问我，设计模式原则这些东西在园子里都讨论烂了，一搜一大把的资料，还花这么大力气去整这个干嘛。博主不得不承认，园子里确实很多这方面的文章，并且不乏出色的博文。博主的想法是，既然要完善知识体系，就不能半途而废。今天就来看看设计模式原则的另一个：接口隔离原则。
# 一、原理介绍
## 1、官方定义
接口隔离原则，英文缩写**ISP**，全称Interface Segregation Principle。
原始定义：Clients should not be forced to depend upon interfaces that they don’t use，还有一种定义是The dependency of one class to another one should depend on the smallest possible interface。
官方翻译：其一是不应该强行要求客户端依赖于它们不用的接口；其二是类之间的依赖应该建立在最小的接口上面。简单点说，客户端需要什么功能，就提供什么接口，对于客户端不需要的接口不应该强行要求其依赖；类之间的依赖应该建立在最小的接口上面，这里最小的粒度取决于单一职责原则的划分。
## 2、自己理解
### 2.1、原理解释
- 不应该强行要求客户端依赖于它们不用的接口。语句很好理解，即客户端需要什么接口，就依赖什么接口，不需要的就不依赖。那么我们反过来说，如果客户端依赖了它们不需要的接口，那么这些客户端程序就面临不需要的接口变更引起的客户端变更的风险，这样就会增加客户端和接口之间的耦合程度，显然与“高内聚、低耦合”的思想相矛盾。
- 类之间的依赖应该建立在最小的接口上面。何为最小的接口，即能够满足项目需求的相似功能作为一个接口，这样设计主要就是为了“高内聚”。那么我们如何设计最小的接口呢？那就要说说粒度的划分了，粒度细化的程度取决于我们上一章讲的的[单一职责原则](http://blog.jobbole.com/99587/)里面接口划分的粒度。从这一点来说，接口隔离和单一职责两个原则有一定的相似性。
### **2.2、接口隔离原则和单一职责原则**
从功能上来看，接口隔离和单一职责两个原则具有一定的相似性。其实如果我们仔细想想还是有区别的。
（1）从原则约束的侧重点来说，接口隔离原则更关注的是接口依赖程度的隔离，更加关注接口的“高内聚”；而单一职责原则更加注重的是接口职责的划分。
（2）从接口的细化程度来说，单一职责原则对接口的划分更加精细，而接口隔离原则注重的是相同功能的接口的隔离。接口隔离里面的最小接口有时可以是多个单一职责的公共接口。
（3）单一职责原则更加偏向对业务的约束，接口隔离原则更加偏向设计架构的约束。这个应该好理解，职责是根据业务功能来划分的，所以单一原则更加偏向业务；而接口隔离更多是为了“高内聚”，偏向架构的设计。
# 二、场景示例
下面就以订单的操作为例来说明下接口隔离的必要性。
## 1、胖接口
软件设计最初，我们的想法是相同功能的方法放在同一个接口里面，如下，所有订单的操作都放在订单接口IOrder里面。理论上来说，这貌似没错。我们来看看如何设计。

C#
```
public interface IOrder
    {
        //订单申请操作
        void Apply(object order);
        //订单审核操作
        void Approve(object order);
        //订单结束操作
        void End(object order);
    }
```
刚开始只有销售订单，我们只需要实现这个接口就好了。

C#
```
public class SaleOrder:IOrder
    {
        public void Apply(object order)
        {
            throw new NotImplementedException();
        }
        public void Approve(object order)
        {
            throw new NotImplementedException();
        }
        public void End(object order)
        {
            throw new NotImplementedException();
        }
    }
```
后来，随着系统的不断扩展，我们需要加入生产订单，生产订单也有一些单独的接口方法，比如：排产、冻结、导入、导出等操作。于是我们向订单的接口里面继续加入这些方法。于是订单的接口变成这样：

C#
```
public interface IOrder
    {
        //订单申请操作
        void Apply(object order);
        //订单审核操作
        void Approve(object order);
        //订单结束操作
        void End(object order);
        //订单下发操作
        void PlantProduct(object order);　　　　 //订单冻结操作
        void Hold(object order);
        //订单删除操作
        void Delete(object order);
        //订单导入操作
        void Import();
        //订单导出操作
        void Export();
    }
```
我们生产订单的实现类如下

C#
```
//生产订单实现类
    public class ProduceOrder : IOrder
    {
        /// 
        /// 对于生产订单来说无用的接口
        /// 
        /// 
        public void Apply(object order)
        {
            throw new NotImplementedException();
        }
        /// 
        /// 对于生产订单来说无用的接口
        /// 
        /// 
        public void Approve(object order)
        {
            throw new NotImplementedException();
        }
        /// 
        /// 对于生产订单来说无用的接口
        /// 
        /// 
        public void End(object order)
        {
            throw new NotImplementedException();
        }
        public void PlantProduct(object order)
        {
            Console.WriteLine("订单下发排产");
        }　　　　 public void Hold(object order)
        {
            Console.WriteLine("订单冻结");
        }
        public void Delete(object order)
        {
            Console.WriteLine("订单删除");
        }
        public void Import()
        {
            Console.WriteLine("订单导入");
        }
        public void Export()
        {
            Console.WriteLine("订单导出");
        }
    }
```
销售订单的实现类也要相应做修改

C#
```
//销售订单实现类
    public class SaleOrder:IOrder
    {
        public void Apply(object order)
        {
            Console.WriteLine("订单申请");
        }
        public void Approve(object order)
        {
            Console.WriteLine("订单审核处理");
        }
        public void End(object order)
        {
            Console.WriteLine("订单结束");
        }
        #region 对于销售订单无用的接口方法
        public void PlantProduct(object order)
        {
            throw new NotImplementedException();
        }　　　　 public void Hold(object order)
        {
            throw new NotImplementedException();
        }
        public void Delete(object order)
        {
            throw new NotImplementedException();
        }
        public void Import()
        {
            throw new NotImplementedException();
        }
        public void Export()
        {
            throw new NotImplementedException();
        } 
        #endregion
    }
```
需求做完了，上线正常运行。貌似问题也不大。系统运行一段时间之后，新的需求变更来了，要求生成订单需要一个订单撤销排产的功能，那么我们的接口是不是就得增加一个订单撤排的接口方法CancelProduct。于是乎接口变成这样：

C#
```
public interface IOrder
    {
        //订单申请操作
        void Apply(object order);
        //订单审核操作
        void Approve(object order);
        //订单结束操作
        void End(object order);
        //订单下发操作
        void PlantProduct(object order);
        //订单撤排操作
        void CancelProduct(object order);
        //订单冻结操作
        void Hold(object order);
        //订单删除操作
        void Delete(object order);
        //订单导入操作
        void Import();
        //订单导出操作
        void Export();
    }
```
**这个时候问题就来了，我们的生产订单只要实现这个撤销的接口貌似就OK了，但是我们的销售订单呢，本来销售订单这一块我们不想做任何的变更，可是由于我们IOrder接口里面增加了一个方法，销售订单的实现类是不是也必须要实现一个无效的接口方法？这就是我们常说的“胖接口”导致的问题。由于接口过“胖”，每一个实现类依赖了它们不需要的接口，使得层与层之间的耦合度增加，结果导致了不需要的接口发生变化时，实现类也不得不相应的发生改变。这里就凸显了我们接口隔离原则的必要性，下面我们就来看看如何通过接口隔离来解决上述问题。**
### **2、接口隔离**
我们将IOrder接口分成两个接口来设计

C#
```
//删除订单接口
    public interface IProductOrder
    {
        //订单下发操作
        void PlantProduct(object order);
        //订单撤排操作
        void CancelProduct(object order);
        //订单冻结操作
        void Hold(object order);
        //订单删除操作
        void Delete(object order);
        //订单导入操作
        void Import();
        //订单导出操作
        void Export();
    }
    //销售订单接口
    public interface ISaleOrder
    {
        //订单申请操作
        void Apply(object order);
        //订单审核操作
        void Approve(object order);
        //订单结束操作
        void End(object order);
    }
```
对应的实现类只需要实现自己需要的接口即可

C#
```
//生产订单实现类
    public class ProduceOrder : IProductOrder
    {
        public void PlantProduct(object order)
        {
            Console.WriteLine("订单下发排产");
        }
        public void CancelProduct(object order)
        {
            Console.WriteLine("订单撤排");
        }
        public void Hold(object order)
        {
            Console.WriteLine("订单冻结");
        }
        public void Delete(object order)
        {
            Console.WriteLine("订单删除");
        }
        public void Import()
        {
            Console.WriteLine("订单导入");
        }
        public void Export()
        {
            Console.WriteLine("订单导出");
        }
    }
    //销售订单实现类
    public class SaleOrder : ISaleOrder
    {
        public void Apply(object order)
        {
            Console.WriteLine("订单申请");
        }
        public void Approve(object order)
        {
            Console.WriteLine("订单审核处理");
        }
        public void End(object order)
        {
            Console.WriteLine("订单结束");
        }
    }
```
**这样设计就能完美解决上述“胖接口”导致的问题，如果需要增加订单操作，只需要在对应的接口和实现类上面修改即可，这样就不存在依赖不需要接口的情况。通过这种设计，降低了单个接口的复杂度，使得接口的“内聚性”更高，“耦合性”更低。由此可以看出接口隔离原则的必要性。**
# 三、总结
通过以上订单功能的优化，我们看到了接口隔离原则的必要性，当然，关于接口隔离原则和单一职责原则的细节我们也不必过多追究，不管何种原则，能解决我们的设计问题就是好的原则、我们必须遵守的原则。
软件设计原则系列文章
- [《小话设计模式原则之（1）：依赖倒置原则DIP》](http://blog.jobbole.com/99563/)
- [《小话设计模式原则之（2）：单一职责原则SRP》](http://blog.jobbole.com/99587/)
- 《[小话设计模式原则之（4）：开闭原则OCP](http://blog.jobbole.com/99617/)》
