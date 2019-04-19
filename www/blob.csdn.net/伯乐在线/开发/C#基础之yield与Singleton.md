# C#基础之yield与Singleton - 文章 - 伯乐在线
原文出处： [方小白](http://www.cnblogs.com/fangyz/p/5597195.html )
## **1.实例解析yiled的作用**
最近参加java笔试题第一次见到yield这个关键字，既然遇见了那肯定要掌握，下面是C#中关于yield关键字的总结。yield这个关键字作用于迭代器块中，其最本质的功能有2个：一是“依次”向枚举对象提供值，二是发出迭代结束信号。这两个功能对应的语句分别是yield return和yield break。
下面有2个小例子，分别没有使用yield和有使用yield。先来看第一个，当我调试时显然执行到GetResult()方法时将会跳转到方法内部并且执行完，接着再去执行输出当前值语句。从结果可以看出第一个是0，说明返回的枚举数所在的位置在集合中是0，接着才是我想要的遍历数据，也就是说只有调用MoveNext()后枚举数才会继续向前移动得到下一个值，但是此时数据已全部加载到内存。
再来看第二个例子，当我调试到GetResultByYield()方法时我想进入到这个方法内部结果发现直接执行下一句，根本就没有进入到GetResultByYield()方法内部。此时发现result.Current是null，再加上前面根本都没执行方法内部的代码，因此我猜测此时集合都是空的。继续调试，当执行MoveNext()方法时才去执行GetResultByYield()，接着执行到yield return随即返回main()方法输出枚举数所代表的集合中的值。
从上面可以看到只有调用MoveNext()需要用的时候才去执行方法来获得结果，不用的时候并不会有任何结果。这个地方编译器会有一个状态机用来保存迭代器的状态，以保证for循环时是从上一次yield return停止的状态继续执行。这个过程就好比小方要喝一升的水，如果它用一个一升的杯子喝那么他要准备一个一升的容器去饮水机装满一升的水。
如果小方喝到一半喝不完了，那接下来剩下的水则将被回收，这样无论能不能喝完都必须准备好一升的水，就像第一个例子。现在让杯子的容积缩小为0.2升，小方喝完一杯后再去饮水机去打水，每次只喝0.2升。这样只有他要去喝的时候才去打水，如果他喝到一半不想喝了显然浪费的水比第一种方式多，这就像第二个例子。最后根据条件不再需要数据便可调用yield return来跳出while循环，如果不写yield break仍然可以正常结束迭代。

C#
```
/// 
    /// 不使用yield的时候
    /// 
    class Program
    {
        static void Main(string[] args)
        {
            //得到一个迭代结果
            var result = GetResult();
            //输出当前的值
            Console.WriteLine(result.Current);
            Console.WriteLine("开始遍历");
            while (result.MoveNext())
            {
                Console.WriteLine(result.Current);
            }
            Console.WriteLine("遍历结束");
            Console.ReadLine();
        }
        //不使用yield来进行迭代
        static IEnumeratorint> GetResult()
        {
            var arr = new int[] { 1, 6, 8, 12,15};
            Listint> list = new Listint>();
            foreach (int item in arr)
            {
                if (item 12)
                    list.Add(item);
            }
            return list.GetEnumerator();
        }
    }
```


C#
```
/// 
    /// 使用yield关键字
    /// 
    class Program
    {
        static void Main(string[] args)
        {
            //得到一个迭代结果
            var result = GetResultByYield();
            //输出当前的值
            Console.WriteLine(result.Current);
            Console.WriteLine("开始遍历");
            while (result.MoveNext())
            {
                Console.WriteLine(result.Current);
            }
            Console.WriteLine("遍历结束");
            Console.ReadLine();  
        }
        //使用yield来进行迭代
        static IEnumerator GetResultByYield()
        {
            var arr = new int[] { 1,6,8,12,15};
            foreach (var item in arr)
            {
                yield return item;
                if (item == 12)
                    yield break;
            }
        }
    }
```
输出结果如下：
![](http://jbcdn2.b0.upaiyun.com/2016/06/caf194851fc3ad05400b613f1a4c3bf92.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/2a0a600553e9a6652652f11b5f7c23a82.png)
## **2.深入yield**
将上面第二个例子放入Reflector工具中，便得到了下面三段代码。第一段是完整的Pragrom类的C#代码，第二段是d__0密封类的C#展开代码，第三段是GetResultByYield()方法的IL代码。在第一段代码中可以看到系统自动生成了一个d__0密封类，它里面声明了一些名字很奇怪的字段，不过我们可以很清楚的看到这个类里面有最重要的MoveNext()方法和Current属性。
第二段代码则是这个密封类的C#展开代码，到这里不知道读者有没有和我当初一样的疑问：为什么要自动生成一个密封类呢？答案就在第三段代码中，可以看到在GetResultByYield()方法中并没有遍历数组，甚至都没有看到创建数组的newarr指令，而是newobj创建了d__0密封类的实例对象。这也正是前面调试的时候为什么根本就没进去GetResultByYield()方法的原因，因为真真的实现代码是在密封类里面的MoveNext()方法中。前面还提到yield是按需所取，因此需要一个状态机来记录每次yield return的状态。
在MoveNext()方法中由于密封类构造函数传进去的是一个0(在第三段代码中可以看到)，因此第一次进入到MoveNext方法时this.__state=0。此时current字段由于没赋值因此就是null了。接着创建数组并开始一个while循环(原来foreach就是while循环)，在循环中给current字段赋值并让state字段值为2，最后返回true。拿Current属性时就是拿while循环中给current赋的值，再次进入这个方法内此时state等于2于是跳转到Label_0090，也就是进入while语句块中继续循环，这就是按需所取的原理。当遇到yield break后会先执行Dispose释放资源，再执行break语句跳出循环。可以看到上述这个过程就是一个状态机，而这个密封类是为建立一个状态机来生成的，现在我们自己也可以写出一个状态机了。

C#
```
internal class Program
{
    // Methods
    public Program();
    private static IEnumerator GetResultByYield();
    private static void Main(string[] args);
    // Nested Types
    [CompilerGenerated]
    private sealed class d__0 : IEnumeratorobject>, IEnumerator, IDisposable
    {
        // Fields
        private int 1__state;
        private object 2__current;
        public int[] 7__wrap4;
        public int 7__wrap5;
        public int[] 5__1;
        public int 5__2;
        // Methods
        [DebuggerHidden]
        public d__0(int 1__state);
        private void m__Finally3();
        private bool MoveNext();
        [DebuggerHidden]
        void IEnumerator.Reset();
        void IDisposable.Dispose();
        // Properties
        object IEnumeratorobject>.Current { [DebuggerHidden] get; }
        object IEnumerator.Current { [DebuggerHidden] get; }
    }
}
```


C#
```
private sealed class d__0 : IEnumeratorobject>, IEnumerator, IDisposable
{
    // Fields
    private int 1__state;
    private object 2__current;
    public int[] 7__wrap4;
    public int 7__wrap5;
    public int[] 5__1;
    public int 5__2;
    // Methods
    [DebuggerHidden]
    public d__0(int 1__state)
    {
        this.1__state = 1__state;
    }
    private void m__Finally3()
    {
        this.1__state = -1;
    }
    private bool MoveNext()
    {
        try
        {
            switch (this.1__state)
            {
                case 0:
                    this.1__state = -1;
                    this.5__1 = new int[] { 1, 6, 8, 12, 15 };
                    this.1__state = 1;
                    this.7__wrap4 = this.5__1;
                    this.7__wrap5 = 0;
                    while (this.7__wrap5 this.7__wrap4.Length)
                    {
                        this.5__2 = this.7__wrap4[this.7__wrap5];
                        this.2__current = this.5__2;
                        this.1__state = 2;
                        return true;
                    Label_0090:
                        this.1__state = 1;
                        if (this.5__2 == 12)
                        {
                            this.System.IDisposable.Dispose();
                            break;
                        }
                        this.7__wrap5++;
                    }
                    this.m__Finally3();
                    break;
                case 2:
                    goto Label_0090;
            }
            return false;
        }
        fault
        {
            this.System.IDisposable.Dispose();
        }
    }
    [DebuggerHidden]
    void IEnumerator.Reset()
    {
        throw new NotSupportedException();
    }
    void IDisposable.Dispose()
    {
        switch (this.1__state)
        {
            case 1:
            case 2:
                this.m__Finally3();
                break;
        }
    }
    // Properties
    object IEnumeratorobject>.Current
    {
        [DebuggerHidden]
        get
        {
            return this.2__current;
        }
    }
    object IEnumerator.Current
    {
        [DebuggerHidden]
        get
        {
            return this.2__current;
        }
    }
}
```


C#
```
.method private hidebysig static class [mscorlib]System.Collections.IEnumerator GetResultByYield() cil managed
    {
        .maxstack 1
        .locals init (
            [0] class ConsoleApplication1.Program/d__0 d__,
            [1] class [mscorlib]System.Collections.IEnumerator enumerator)
        L_0000: ldc.i4.0 
        L_0001: newobj instance void ConsoleApplication1.Program/d__0::.ctor(int32)
        L_0006: stloc.0 
        L_0007: ldloc.0 
        L_0008: stloc.1 
        L_0009: br.s L_000b
        L_000b: ldloc.1 
        L_000c: ret 
    }
```
## **3.单例模式**
单例模式没什么好说的，当然如果深挖应该也是大有学问，其中我觉得比较好的一种写法如下。单例模式的代码我看过多次不过却没怎么写，结果真真写的时候再加上时间又有点紧最后写的一塌糊涂。以后写代码要兴平气和地去写，急躁的状态写不出什么好代码。当然总会有烦躁的时候，所以只能多写代码来让自己写出高质量的代码成为一种习惯！

C#
```
class A
    {
        private static A instance = new A();
        public static A Instance
        {
            get { return A.instance; }
        }
    }
```
