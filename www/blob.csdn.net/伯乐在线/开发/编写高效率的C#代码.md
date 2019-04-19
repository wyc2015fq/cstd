# 编写高效率的C#代码 - 文章 - 伯乐在线
原文出处： [熊二哥](http://www.cnblogs.com/wanliwang01/p/EffectiveCSharp.html)
周末空闲，选读了一下一本很不错的C#语言使用的书，特此记载下便于对项目代码进行重构和优化时查看。
Standing On Shoulders of Giants，附上思维导图，其中标记的颜色越深表示在实际中的实际意义越大。
![636325-20160125005935109-815743093](http://jbcdn2.b0.upaiyun.com/2016/06/90d4d949a7d71f0c728fc09679feebe71.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/52ae37e3d0c8422b35627c18ffa85feb.png)
|**名称**|**内容和示例**|
|----|----|
|提供API时尽量提供泛型接口|Public interface IComparable<T>{ int CompareTo(T other) }|
|泛型约束尽可能的严格并有效|Public delegate T FactoryFunc<T>(); Public static T Factory<T>( FactoryFunc<T> newT) where T:new() { T t = newt();}|
|通过运行时类型检查具体化泛型算法|比如根据不同的集合类型优化相应算法|
|使用泛型强制执行编译时类型推测|Public static T ReadFromStream(XmlReader inputStream) { return (T)factory.Deserialize(inputStream) }|
|保证自定义泛型类支持可析构的类型参数|Public sealed class EngineDriver<T>:IDisposable where T:Engine, new() { public void Dispose(){ var resource = driver as IDisposable; if(resource != null) resource.Dispose(); } }|
|通过委托在类型参数上定义方法约束|Public static T Add<T>(T left, T right, Func<T,T,T> addFunc){ return addFunc(right, left); }|
|不要在基类和接口上创建具体化的泛型类型|尽可能使的基类和接口的适用范围更加的广阔|
|推荐使用泛型方法，除非类型参数是实例字段|Public static T Max<T>(T left, T right) { return Comparer<T>.Default.Compare(left, right) < 0 ? right : left }|
|推荐使用泛型的Tuple作为输出和引用参数|当设置方法的返回值，或者在需要使用ref参数的情形时，使用Tuple<>元组可以使代码更清晰，当然如果参数比较复杂，还是选择建立对应的DTO类型为宜|
|在泛型接口上增加对应的传统接口|这个在大家基础架构时非常重要，每个方法均提供泛型版本和object版本，使得代码有很强的兼容性。 Public static bool CheckEquality(object left, object right) { return left.Equals(right); } Public static bool CheckEquality<T>(T left, T right) where T:IEquatable<T> { return left.Equals(right); }|
![](http://jbcdn2.b0.upaiyun.com/2016/06/f917d546f2b297715337c9eae3ce7ab8.png)
|**名称**|**内容和示例**|
|----|----|
|使用线程池代替创建线程|经过微软的官方测试，由自己调度线程和使用线程池，在每10万个计算消耗的平均时长比较中，前者所消耗时长为后者三倍，因而选用线程池作为默认多线程处理机制是合理的选择 Private static double ThreadPoolThreads(int numThreads) { var start = new Stopwatch(); Using(var e = new AutoResetEvent(false)){ int workerThreads = numThreads; start.Start();//**watch.ElapsedMilliseconds, watch.Restart(), watch.Stop();** for(var I = 0; I < numThreads; thread++) ThreadPool.QueueUserWorkItem( (x)=>{ // to do If(Interlocked.Decrement(ref workThreads) == 0) { e.Set(); } }); }}|
|使用后台工作者组件对象用于处理多线程通信|现在已经不再使用后台Worker，而推荐使用Task任务模型替代它，其逻辑为 ![](http://jbcdn2.b0.upaiyun.com/2016/06/5f986985e452eb4252ece4e6efd02102.png)|
|将lock作为优先级最高的同步原语|使用lock相当于使用了Monitor.Enter和Exit，不过要方便很多，使用的是临界区的概念。Public int TotalNum { get{ lock(syncObj) return total; } set{ lock(syncObj) total++;} }|
|Lock中方法体尽可能精简|在使用lock时，一定不要使用lock(this)和lock(typeof(MyType))的形式，这会造成很多的问题，必须保证锁的对象不是公开无法被外部使用的，常见的对方法加锁的形式有： 1．使用特性，[MethodImpl(MethodImplOptions.Synchronized)] 2.使用私有变量作为锁变量 private object syncHandler = new object(); 此外还有一种复杂点的形式如下。 Private object syncHandle; Private object GetSyncHandle(){ InterLocked.CompareExchange(ref syncHandle, new object(), null); }|
|避免在临界区中调用未知代码|比如不要在临界区中使用事件，因为事件的处理方法由调用方注册，是未知的，会造成相关的问题，一定要保证临界区中方法的确定性|
|理解在WinForm和WPF中的跨线程调用|做过WinForm编程的亲，一定遇到过一个InvalidOperationException，内容为跨线程操作非法，访问Control的线程不是创建线程，这其实是Winform、WPF等框架对UI的保护，避免多个不同线程修改UI值的情况。这种情况主要有一下三种方式来处理，最推荐的解决方案为第二种。- 在Form的构造方法中加入 Control.CheckForIllegalCrossThreadCalls =false; (不推荐)- Private void UpdateControl(string msg){ Action act = (x)=>{this.controlA.Text = x; } if(controlA.InvokeRequired) { this.controlA.Invoke(act, msg); } else{ act(); } }- 使用BackgroundWorker组件(elide)|
![](http://jbcdn2.b0.upaiyun.com/2016/06/931e479ea61212b85933f234dc0f38aa.png)
|**名称**|**内容和示例**|
|----|----|
|为序列创建可组合的API， yield return xxx|Public static IEnumerable Square(IEnumerable nums) { foreach(var num in nums) yield return num * num; }|
|通过Action，Predicate，Functions解耦迭代器|Public static IEnumerable Filter(IEnumerable sequence, Predicate filterFunc) { if(filterFunc(int)) yield return item; }|
|根据请求生成序列|[IEnumerable].TakeWhile(num => num < 5);|
|通过Function参数解耦|Public static T Sum(IEnumerable sequence, T total, Func<T,T,T> accumulator) { foreach(T item in sequence){ total = accumulator(total, item); return total; } }|
|创建清晰，最小化，完整的方法组|即在提供方法时，尽可能的保证完备性（支持主要的类型）|
|推荐定义方法重载操作符|还记得在学习C++时，很推荐重载操作符，不过在面向对象语言的今天，使用可读性更强的方法更合理|
|理解事件是如何增加对象运行时的耦合性|public event EventHandler OnProgress; public void DoLotsOfStuff() { for (var i = 0; i < 100; i++) { SomeWork(); var args = new WorkerEventArgs(); args.Percent = i; //关于这个=，我总是不算特别明白，不过记得是线程安全的代码 //可以理解为，使用这个，其他调用这个事件的对象就不会被锁定 var progHandler = OnProgress;   if (progHandler != null) { //注意这里的this progHandler(this, args); } if (args.Cancel) return; } } 这里想补充的是，event属于编译时解耦，你可以看到，该事件的订阅者都没有入侵事件所属的发布者（发布者-订阅者默认），但实际上，在运行时，所有的订阅者其实是和事件紧密关联在一起的，订阅者们修改共享数据的操作存在很大的不确定性。简而言之，事件是编译时解耦，运行时耦合的。|
|只声明非虚事件对象|在.NET中，事件提供了类似属性的简易语法，通过add,remove方法添加相关事件处理程序，其实event就是delegate的包装器，这个特殊的委托便于**应用事件处理模型**，同时提供线程安全性。由于事件的运行时耦合性，如果使用虚事件容易造成未知的错误， private EventHandler progressEvent; public event EventHandler OnProgress { [MethodImpl(MethodImplOptions.Synchronized)] add { progressEvent += value; } [MethodImpl(MethodImplOptions.Synchronized)] remove { progressEvent -= value; } }|
|通过异常报告方法契约错误|当出现业务异常流程时，推荐抛出异常而不是使用TryXXX组合的方式，因为这样代码更加简单易懂。当然在与业务无关的，如简单数据转换的场景下，使用TryXXX是很好的选择|
|确定属性的行为和数据一样|让属性尽可能的简单，不要将复杂逻辑放在属性，如果需要可以通过提供相应方法的方式，使得代码更加通俗易懂，且使得调用人坚信属性的调用不会造成任何的性能影响|
|区分继承和组合|在适当的场景下，用组合代替继承是常见的代码设计模式，这样可以减少类的污染，在选用策略模式的场景下，组合使用的非常的多，常见的形式如下： public interface IContract{ void SampleImplMethod(); } public class MyInnerClass:IContract{ public void SampleImplMethod (){ //elided }} public class MyOuterClass:IContract{ private IContract impl = new MyInnerClass(); public void SampleImplMethod (){ impl.SampleImplMethod(); }}|
![](http://jbcdn2.b0.upaiyun.com/2016/06/303434c3cc64121a3f2d10019d85b3be.png)
|**名称**|**内容和示例**|
|----|----|
|通过扩展方法扩展接口|Public static bool LessThan(this T left, T right) where T : IComparable { return left.CompareTo(right) < 0; }|
|通过扩展方法增强已经构建的类型|这部分很容易理解，比如你使用系统提供的相关类，无法修改源码（虽然已开源），这时为了代码的便捷性和可读性，使用扩展方法增强该类变得非常有效|
|推荐隐式类型的本地变量|简单方便|
|通过匿名类限制类的可见范围|使得代码的封装性更好，更加健壮|
|为外部的组件创建可组合的API|要求提供的API具有更好的健壮性，功能相对完整并独立，复用性更强，例如尽量不要使用可空类型作为接口参数等|
|避免修改绑定的变量|这部分内容涉及闭包，通过以下的例子可以很容易的理解 public void Test() { int index = 0; Func<IEnumerable> sequence = () => Generate(30, () => index ++); index = 20; foreach (var item in sequence()) { Console.WriteLine(item); } } private IEnumerable Generate(int num, Func act) { for (; num > 0; num–) { yield return act(); } }|
|在匿名类型上定义本地函数|public void Test01() { var randomNumbers = new Random(); var sequence = (from x in Generate(100, () => randomNumbers.NextDouble() * 100) let y = randomNumbers.NextDouble() * 100 select new { x, y }).TakeWhile(point => point.x < 75); foreach (var item in sequence) { Console.WriteLine(item); }}|
|不要重载扩展方法|由于个人创建扩展方法的普遍性和完备性不强，重载此类方法容易降低程序的健壮性|
![](http://jbcdn2.b0.upaiyun.com/2016/06/059fa381936cae4b236de1c41146898e.png)
|**名称**|**内容和示例**|
|----|----|
|理解查询表达式如何映射到方法调用|简单来说，我们所写的LINQ语句都会先转化为对应的扩展方法，然后再解析相关的表达式树最后生成对应语句。 var people = from e in employees where e.Age > 30 orderby e.LastName, e.FirstName, e.Age select e; var people = employees.Where(e=>e.Age > 30).OrderBy(e=>e.LastName).ThenBy(e=>e.FirstName).ThenBy(e=>e.Age);|
|推荐Lazy延迟加载查询|延迟加载表示数据到真正使用时再去获取，这个概念不太容易理解，简单来说，我们的获得集合函数调用实际上只是生成相应的查询语句，但并未实际执行，获得任何对象，只有在我们对其经行迭代等操作时，才真正的加载数据。这些概念其实都和委托紧密相关，从逻辑上讲就是加了一个新的层次，函数本身（可以说是其指针、地址）是一个层次，函数的实际调用又是一个层次，在javascript也有相似的概念，就比如FunctionA和FunctionA()的区别。 Private static IEnumerable<TResult> Generate<TResult>(int number, Func<TResult> generator) { for(var i = 0; i < number; i++) yield return generator(); }注意到Func<TResult>这个格式没有，和Task<TResult>何其相似，一个是异步返回值，一个是延迟的返回值，仅仅是一个方便理解的小思路哈。|
|推荐使用lambda表达式代替方法|这儿的实际意思是指在使用LINQ时，由于每个查询的局限性，不推荐在查询中调用外部方法，而因尽可能通过LINQ自身来完成相应工作，减少各个查询间的干扰|
|避免在Func和Action中抛出异常|这个也很好理解，由于Action等委托常用于集合操作中，而任何一个一场都会中断整个集合的操作，给集合操作带来了很大的不确定性，并且在并行运算时更加难以控制，因而在Action中把异常捕获并处理掉更加的合理。相信大家在job中常常会遇到循环调用的场景，这是通过返回值将相关的异常信息带回是更合理的处理方式，之后无论是记log还是给相关人发邮件都显得非常的合理|
|区分预先执行和延迟执行|在实际应用时，将正常加载和延迟加载组合使用非常的常见 var method1 = MethodA(); var answer = DoSomething(**()=>method1**, ()=>MethodB(), ()=>MethodC()); 此外，想说的是，在项目中，比如大部分数据是正常加载，少部分数据使用延迟加载，而一些特殊的场景通过（比如缓存服务器）则使用预热（预先加载）的方式，弄清这里面的逻辑会让这部分的应用更加得心应手|
|避免捕获昂贵的资源|之前介绍了C#编译器如何生成委托和变量是如何在一个闭包的内部被捕获的，下面是一个简单的构建闭包的例子 int counter = 0; IEnumerable<int> numbers = Generate(30, ()=>counter++); 其实际生成的代码如下： private class Closure { public int generatedCounter; public int generatorFunc(){ return generatedCounter ++; } } var c = new Closure(); c.generatedCounter = 0; IEnumerable<int> sequence = Generate(30, new Func<int>(c.generatorFunc)); 通过闭包的形式，我们可以发现其扩展了捕获对象的生命周期，如果这个捕获对象是一个昂贵的资源，比如说是个很大的文件流，那么就可能发生内存泄露的情况。因而在委托中使用本地的资源，一定要非常的当心，比较合理的方式是，将你所需要的内容缓存后释放原始对象。|
|区别IEnumerable和IQueryable的数据源|由于IQueryable数据源其实是对IEnumerable数据源的封装和增强，简答来说，IQueryable对象的相关数据处理操作的性能要远高于IEnumerable对象，因而如果实际的返回值为IQueryable对象，那么不要经行相关的转化，当然也可以通过typeA as IQueryable来尝试转化，如果本来就是IQueryable对象则直接返回，反之对其进行封装后返回|
|通过Single()和First()方法强行控制查询的语义|这个就是让我们的查询语句通过语义来指导查询，尽早的抛出异常 var stus = (from p in Students where p.Score > 60 orderby p.ID select p).Skip(2).First();|
|推荐存储Expression<.>替代Func<>|这部分很有意思，当然理解难度也不小，毕竟Expression完全可以实现一个简单的编译器了，真心强大。我们所使用的LINQ完全是建立在其上的，这儿只做个很粗略的学习，作为未来加强学习的引子，可以看到，Expression表达式树是Func的抽象 **Expression**<Func<int, bool>> IsOdd = val % 2 == 1;Expression<Func<int, bool>> IsLargeNumber = val => val > 99; InvocationExpression callLeft = Expression.Invoke(IsOdd, Expression.Constant(5)); **InvocationExpression** callRight = Expression.**Invoke**(IsLargeNumber, Expression.Constant(5)); **BinaryExpression** Combined = Expression.**MakeBinary**(ExpressionType.**Add**, callLeft, callRight); Expression<Func<bool>> typeCombined = Expression.Lamda<Func<bool>>( Combined); Func<bool> compiled = typeCombined.Compile(); Bool result = compiled();|
![](http://jbcdn2.b0.upaiyun.com/2016/06/093c7c0c5ddf092fb2e379c9fe33725a.png)
|**名称**|**内容和示例**|
|----|----|
|最小化可空类型的可见性|简单来说，就是减少在公共方法API的输入参数和输出返回值中使用可空类型，因而这样会加大方法的调用难度。当然在内部方法和实体类(包括代码生成的实体类)中使用还是非常方便有效的|
|给部分类和部分方法建立构造器，设值器和事件处理器|这个主题常出现在有代码生成器出现的场景，比如说使用代码生成工具生成DAO层，其中只包含最基础的CRUD操作，当扩展时，我们如果直接修改类文件，那么当下一次数据库修改，再次生成代码时就可能出现代码覆盖等错误，因而在这种情况下我们会考虑使用分布类（说实话分布方法，我自己也没怎么用过，记得在以前做C++时用过类似external关键字引用外部方法的情形，形式上有点像）。这是需要注意的是，工具生成类和扩展类（一般来说类名相同，但文件名加上Ext并放入对应层次文件夹中）的设计，需要仔细考虑默认构造方法、属性值设置器、事件处理器等类成员的构建。|
|将数组参数限制为参数数组|由于数组的不确定性，因而不推荐将数组作为参数(指的是不同类型的数据放入一个object[]中，使得方法的使用非常容易出错，当然泛型的数据集合等除外)，而推荐params的形式来传递相应数据，这样API参数在不存在或者提供null值时也不会报错。 Private static void Write(params object[] params) { foreach(object o in params) Console.WriteLine(o); }|
|避免在构造器中调用虚方法|这其实是个很有用的建议，尤其是在构建集成关系复杂的基类及其派生类时，由于子类、父类构造方法调用顺序原因，很容造成初始化和赋值的错误，用一个简单的例子来说明这个问题，借用书中的一句原话，”一个对象在其所有构造器执行完成前并没有完整的被构建” class A { protected A() { MethodA(); } protected virtual void MethodA(){ Console.WriteLine(“MethodA in A”); } } class B : A{ private readonly string msg = “set by initializer”; public B(string msg){ this.msg = msg; } protected override void MethodA(){ Console.WriteLine(msg); } } class Program{ static void Main(string[] args){ B b = new B(“Constructed in main”); } } 这儿的结果是”set by initializer”，首先调用B的构造方法，由于msg是readonly赋值木有成功，然后调用父类无参构造方法，实际调用子类MethodA有以上结果。这部分在实际中我也曾犯过相似的错误，需要非常小心。|
|对大对象考虑使用弱引用|弱引用的概念接触的相对较少，实际就是将直接引用转化为间接引用 Var weakR = new WeakReference(largeObj); largeObj = null; 咋一看，感觉确实不太好明白，这儿的意图是首先将大对象的引用(指针)放入一个包装类型，成为弱引用，之后将直接引用对象释放，这样就形成弱引用，利于垃圾回收，其使用场景主要针对没有提供IDispose接口的大对象。说实话，在实际中，我也没有这样使用过，之后尝试后再给大家分享。|
|推荐对易变量和不可序列化的数据使用隐式属性|简单来说，就是在非Serializable对象中推荐使用priavte set，可以保护数据安全并便于提供验证等方法。当然在支持序列化时，public的set方法和默认无参的构造函数都是必须的|
谢谢大家的阅读，希望自己早日成为一名合格的程序员！
少年辛苦终身事，莫向光阴惰寸功
参考文献：
- 
[美]Bill, Wagner. More Effective C#[M]. 北京:人民邮电出版社, 2009.
