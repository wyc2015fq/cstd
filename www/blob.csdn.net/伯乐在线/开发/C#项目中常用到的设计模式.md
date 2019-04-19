# C#项目中常用到的设计模式 - 文章 - 伯乐在线
原文出处： [禅宗花园...迷失的佛](http://www.cnblogs.com/VectorZhang/p/5554388.html)
## 1. 引言
一个项目的通常都是从Demo开始，不断为项目添加新的功能以及重构，也许刚开始的时候代码显得非常凌乱，毫无设计可言。但是随着项目的迭代，往往需要将很多相同功能的代码抽取出来，这也是设计模式的开始。熟练运用设计模式应该是每一个软件开发人员的必备技能。今天给大家介绍几个常用的设计模式。
## 2. 单例模式
单例模式恐怕是很多开发人员最先接触到的模式之一，可以认为就是一个全局变量。它的初始化过程无非就是一开始就new 一个instance，或者惰性初始化等需要用到的时候new 一个instance。这里需要注意的是在多线程情况下new一个instance。通常加上lock 可以解决问题。这里我们利用C# 的系统函数 Interlocked.CompareExchange


C#
```
internal class SingletonOne
    {
        private static SingletonOne _singleton;
        private SingletonOne()
        {
        }
        public static SingletonOne Instance
        {
            get
            {
                if (_singleton == null)
                {
                    Interlocked.CompareExchange(ref _singleton, new SingletonOne(), null);
                }
                return _singleton;
            }
        }
    }
```
## 3. 迭代器模式
迭代器模式也是用的比较多的一种，通常见于C#的内置容器数据结构 List，Stack等等，为了便于遍历容器内元素。这里给出一个简单版的Stack实现


C#
```
internal class Stack : IEnumerable, IEnumerable
    {
        private T[] _array;
        private int _index;
        private const int DefaultSize = 4;
        public Stack(int size)
        {
            var sized = size > 0 ? size : DefaultSize;
            this._array = new T[sized];
            this._index = 0;
        }
        public int Count
        {
            get { return this._index; }
        }
        public Stack(IEnumerable data) : this(0)
        {
            var enumrator = data.GetEnumerator();
            while (enumrator.MoveNext())
            {
                var item = enumrator.Current;
                this.Push(item);
            }
        }
        public void Push(T item)
        {
            if (this._index this._array.Length)
            {
                this._array[this._index++] = item;
            }
            else
            {
                var newLength = this._array.Length 1;
                T[] newArray = new T[newLength];
                Array.Copy(this._array, newArray, this.Count);
                this._array = newArray;
                this.Push(item);
            }
        }
        public T Pop()
        {
            if (this.Count 0)
            {
                throw new ArgumentOutOfRangeException("pop");
            }
            else
            {
                this._array[this._index] = default(T);
                return this._array[--this._index];
            }
        }
        public T Get(int index)
        {
            if (this.Count  index)
            {
                throw new ArgumentOutOfRangeException("Get");
            }
            else
            {
                return this._array[index];
            }
        }
        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
        public IEnumerator GetEnumerator()
        {
            return new StackEnumerator(this);
        }
    }
```
Stack 的 迭代器内部实现：


C#
```
internal class StackEnumerator : IEnumerator , IEnumerator
    {
        private Stack _stack;
        private int _index;
        public StackEnumerator(Stack stack)
        {
            this._stack = stack;
            this._index = -1;
        }
        public bool MoveNext()
        {
            this._index++;
            return this._index this._stack.Count;
        }
        public void Reset()
        {
            this._index = -1;
        }
        object  IEnumerator.Current 
        {
            get { return this.Current; } 
        }
        public T Current
        {
            get { return this._stack.Get(this._index); }
        }
        public void Dispose()
        {
            this._stack = null;
        }
    }
```
## 4 工厂模式
工厂模式细分的话有简单工厂模式、抽象工厂模式等。它最核心的就是如何通过 Factory new 一个 对象出来。在ASP.NET MVC 消息处理实现过程中工厂模式运用的非常多。比如
在MVC中处理一个Request，其实就是调用Controller下的一个Action，这就需要从Url 和Route 中反射出Controller对象，内部由ControllerFactory创建。
![image](http://jbcdn2.b0.upaiyun.com/2016/06/9a50fdec463ba53d00e070da71b7fb24.png)
它的默认实现是：DefaultControllerFactory
![image](http://jbcdn2.b0.upaiyun.com/2016/06/cb132cc6cb80510e8918392f8dc618cb.png)
另一个例子是ValueProviderFactory，它使得Controller 下的Action 能够接收到前端传回来的数据并实现模型绑定，是典型的抽象工厂实现。
![image](http://jbcdn2.b0.upaiyun.com/2016/06/6c4db2049d5df084c220c7b1d49c86a4.png)
## 5. 订阅模式
订阅模式在某些项目运用比较多，比如 Knockout 整个项目就是一个大的订阅模式的实现，但是它是用javascript编写的。还有像微博、微信公众号等等订阅模式通常少不了。
通常可以定义接口：


C#
```
internal interface ISubject
    {
        IEnumerable Observers { get; } 
        void Notify();
        void AddObserver(IObserver observer);
        void RemoveObserver(IObserver observer);
    }
    internal interface IObserver
    {
        void ReceiveSubject(ISubject subject);
    }
```
实现：


C#
```
internal class AritcleObserver : IObserver
    {
        public void ReceiveSubject(ISubject subject)
        {
            // receive the subject
        }
    }
    class WeChatArticle : ISubject
    {
        private ICollection _observers;
        private string _name;
        public WeChatArticle(string name)
        {
            this._name = name;
            this._observers = new List();
        }
        public IEnumerable Observers
        {
            get { return this._observers; }
        }
        public void Notify()
        {
            foreach (IObserver observer in this._observers)
            {
                observer.ReceiveSubject(this);
            }
        }
        public void AddObserver(IObserver observer)
        {
            this._observers.Add(observer);
        }
        public void RemoveObserver(IObserver observer)
        {
            this._observers.Remove(observer);
        }
    }
```
## 6.  责任链模式
责任链模式没有像工厂模式那样被人熟悉，在ASP.NET WebApi 中有一个非常典型的实现 就是WebApi的消息处理管道HttpMessageHandler
![image](http://jbcdn2.b0.upaiyun.com/2016/06/d6574f22cc3fda65824070e1fe85fba0.png)
这里给一个简单的模拟


C#
```
class DataRequest
    {
        public string FileName { get; set; }
    }
    class DataResponse
    {
        public string Error { get; set; }
        public string Data { get; set; }
    }
    internal abstract class RequestHandler
    {
        public RequestHandler NextHandler { get; set; }
        public abstract DataResponse Process(DataRequest request);
    }
    class ReadRequestHandler : RequestHandler
    {
        public override DataResponse Process(DataRequest request)
        {
            return new DataResponse()
            {
                Data = File.ReadAllText(request.FileName)
            };
        }
    }
    class ExistsRequestHandler : RequestHandler
    {
        public override DataResponse Process(DataRequest request)
        {
            if (File.Exists(request.FileName))
            {
                return this.NextHandler.Process(request);
            }
            else
            {
                return new DataResponse()
                {
                    Error = "no exists"
                };
            }
        }
    }
```
## 7. 组合模式
组合模式是使得单个对象和组合对象有一致的行为，一致的行为可以理解为拥有同一个接口，比如图形显示


C#
```
class ControlContext
    {
    }
    internal interface IControl
    {
        void Draw(ControlContext context);
    }
    class Line : IControl
    {
        public void Draw(ControlContext context)
        {
        }
    }
    class Circle : IControl
    {
        public void Draw(ControlContext context)
        {}
    }
    class CompositeControl : IControl
    {
        private List controls;
        public CompositeControl(IList cons)
        {
            this.controls = new List(cons);
        }
        public void Draw(ControlContext context)
        {
            this.controls.ForEach(c => c.Draw(context));
        }
        public void Add(IControl control)
        {
            this.controls.Add(control);
        }
    }
```
## 8. 总结
市场上有很多关于设计模式的书，但是基本的设计模式大概有20多种，本文给大家介绍了几种项目中常见的设计模式，其实有些设计模式在实际项目已经不知不觉用起来了。
以后再给大家介绍其他的几种设计模式。
