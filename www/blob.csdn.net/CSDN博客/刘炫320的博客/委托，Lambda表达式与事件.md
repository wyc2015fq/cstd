# 委托，Lambda表达式与事件 - 刘炫320的博客 - CSDN博客
2016年09月17日 09:48:39[刘炫320](https://me.csdn.net/qq_35082030)阅读数：1319
这是最为诱人的一块，我已经垂涎已久了。
委托在曾经的C语言中叫做函数指针的参数，还有就是C#中的委托delegate，像Java里是没有这种形式的，不过可以通过接口来进行模拟，准确来说，应该是配合代理者模式才能用的。
委托的使用方面很广，包括启动线程和任务、通用库类等。它的一个使用方面就是接口，也就是java那个方面，比如我们要使得一个数组按照升序排序，那么两两如何进行比较和排序，这个任务一定会有，但是具体是按照哪个属性来比，怎么比，都没有明确描述，现在是通过一个叫做比较接口来实现，在之前数组那章我们已经讲过了。但是真正要属于C#委托的特色的不在于此。
C#中的特色就是使用函数指针作为参数来进行调用，但是，C/C++中没有规定这个函数的样子，这使得操作不是特别安全。C#中的委托就是为了完善这个的安全性。
首先，你要用函数指针作为参数，那么你首先就要声明一个委托。
C#里对于委托的实质的实现就是定义一个新类，所以它可以类比类的定义和声明。
比如这样:
Public delegate int Add(int a,int b);
基本上就是等同于一个方法的声明了。
一般的委托如下形式：
```
static void Test1()
        {
            int x = 40;
            //可以看到如何定义一个委托
            GetAString firstStringMethod = new GetAString(x.ToString);
            Console.WriteLine("String is {0}",firstStringMethod());
        }
```
这主要是一个对于X.toString()方法的一个委托，不过这似乎并不能看出委托的优势所在，那么下面一个例子就可以看出来了。
```
static void Test2()
        {
            Currency balance = new Currency(34, 50);
            GetAString firstStringMethod = balance.ToString;
            Console.WriteLine("String is {0}", firstStringMethod());
            firstStringMethod = new GetAString(Currency.GetCurrencyUnit);
            Console.WriteLine("String is {0}", firstStringMethod());
        }
```
这个例子中，两次的输出的语句是一样的，都是调用了firstStringMethod方法，但是输出的最终结果是不一样的，其原因就是在每次调用这个委托前，它都会先把委托置为不同的方法，firstStringMethod的两次赋值。
委托中，除了你自定义的这种或那种委托，比如上面的GetAString类型的委托，系统也提供了2种委托类型，分别是Action<T>和Func<T>,前者是一个无返回值的委托，后者是包含返回值的委托，最多可以包含16个不同的参数。
下面会介绍一个完整的委托使用例子，冒泡排序的例子，以便于更好的，完整的理解委托。
BubbleSorter类：
```
class BubbleSorter
    {
        static public void Sort<T>(IList<T> sortArray, Func<T, T, bool> comparison)
        {
            bool swapped = true;
            do
            {
                swapped = false;
                for (int i = 0; i < sortArray.Count - 1; i++)
                {
                    if (comparison(sortArray[i + 1], sortArray[i]))
                    {
                        T temp = sortArray[i];
                        sortArray[i] = sortArray[i + 1];
                        sortArray[i + 1] = temp;
                        swapped = true;
                    }
                }
            } while (swapped);
        }
}
```
员工类：
```
class Employee
    {
        public string Name { get; private set; }
        public decimal Salary { get; private set; }
        public Employee(string name,decimal salary)
        {
            this.Name = name;
            this.Salary = salary;
        }
        public override string ToString()
        {
            return string.Format("{0},{1:C}", Name, Salary);
        }
        public static bool CompareSalary(Employee e1,Employee e2)
        {
            return e1.Salary < e2.Salary;
        }
    }
```
调用的时候：
```
static void Test4()
        {
            Employee[] employees = {
                new Employee("A", 1000),        
                new Employee("C", 3000.38m),
                new Employee("B", 2000)
              };
            BubbleSorter.Sort(employees, Employee.CompareSalary);
            foreach(var employee in employees)
            {
                Console.WriteLine(employee);
            }
        }
```
这样就可以进行按照薪水来进行排序了。
当然，委托的力量远远不止这些，这只是一个初级使用委托的水平，下面是它的升级版，多播委托。
这是一个计算乘法和平方的例子:
首先是运算类：
```
class MathOperations
    {
        public static void MultiplyByTwo(double value)
        {
            double result= value * 2;
            Console.WriteLine("Multiplying by 2:{0} gives {1}",value,result);
        }
        public static void Square(double value)
        {
            double result =  value * value;
            Console.WriteLine("Squaring:{0} gives {1}", value, result);
        }
    }
```
接下来就是调用函数了：
```
static void ProcessAndDisplayNumber(Action<double> func,double value)
        {
            Console.WriteLine();
            Console.WriteLine("ProcessAndDisplayNumber called with value={0}",value);
            func(value);
        }
/// <summary>
        /// 这是多播委托
        /// </summary>
        static void Test5()
        {
            {
                Action<double> operations = MathOperations.MultiplyByTwo;
                operations += MathOperations.Square;
                ProcessAndDisplayNumber(operations, 2.0);
                ProcessAndDisplayNumber(operations, 7.94);
            }
            {
                Action< double> operation1 = MathOperations.MultiplyByTwo;
                Action<double> operation2 = MathOperations.Square;
                Action<double> operations = operation1 + operation2;
                ProcessAndDisplayNumber(operations, 1.414);
            }
            
        }
```
可以看到它的执行结果如下：
![](https://img-blog.csdn.net/20160917095101728?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样，一个委托，就可以进行2次操作，而实现这个的样子，只不过是使用了+=而已。不过对于这里的多播委托，还要注意一下几点，目前接触的还是一个比较浅层次的多播委托。我们现在看到的是无返回值的委托，这样它组成的链式多播委托，是有顺序的，但是如果并不是一个无返回值的委托，它的执行顺序就会随机了。但是通常来讲，都是无返回值的，而且，其实也并不常用，因为在C#中，有另外一个和它比较相似的，叫做事件，在事件里的操作会更复杂，这个以后再讲解。另外一点要讲的是，如果链式多播委托中有一个方法触发了异常，整个链都会终止。因此委托又提供了一个转换成列表GetInvocationList()方法来进行遍历。这样当一个方法出现异常时，下一个方法还会继续执行。
接下来，就是震撼人心的Lambda表达式登场了！
Lambda表达式是C#3.0以后的产物，主要是为了使得委托的函数表达更加简便而产生的。其实和普通的委托没什么太大的区别。
```
/// <summary>
        /// 一个简单的lambda表达式。
        /// </summary>
        static void Test6()
        {
            string mid = ",middle part,";
            Func<string, string> lambda = param =>
           {
               param += mid;
               param += "and this was added to the string.";
               return param;
           };
            Console.WriteLine(lambda("Start of string"));
        }
```
如上面所示，也就是一个“=>”符号，它基本上就是lambda的标志性符号。它之前是参数列表，可以是一个参数，也可以是2个及以上的，不过要用括号括起来，当然可以不指定参数的类型，也可以指定。右边就是一个完整的函数体了。
而且更令人惊讶的是Lambda可以访问外部变量，但是通常不建议这么做。所以就不举例了。
当然Lambda表达式不仅仅是用在委托这个上面这么一小部分，还有其他的地方能展现它的美，这里只是先管中窥豹一下。
接下来就简要介绍一下事件。事件是基于委托的。它为委托提供了一种发布订阅的机制。说白了，就是一个方法会通知给订阅了这个方法的其他对象。下面这个例子中，CarDealer类提供了一个新车到达的触发事件，Consumer类则订阅这个事件，以获得新车到达的通知。尤其注意EventArgs的使用。
```
public class CarInfoEventArgs : EventArgs
    {
        public string Car { get; private set; }
        public CarInfoEventArgs(string car)
        {
            this.Car = car;
        }
    }
    class CarDealer
    {
        public event EventHandler<CarInfoEventArgs> NewCarInfo;
        public void NewCar(string car)
        {
            Console.WriteLine("CarDealer,new car{0}", car);
            if (NewCarInfo != null)
            {
                NewCarInfo(this, new CarInfoEventArgs(car));
            }
        }
}
```
上面这是CarDealer类，可以看到，它首先使用了一个汽车信息事件类，然后在汽车到达时，把车辆信息发布出去。而那个NewCarInfo!=null则是表示了有没有人订阅这个委托，如果没有人订阅，就不用发送这个事件了。
```
public class Consumer
    {
        private string name;
        public Consumer(string name)
        {
            this.name = name;
        }
        public void NewCarIsHere(object sender,CarInfoEventArgs e)
        {
            Console.WriteLine("{0}:car {1} is new", name, e.Car);
        }
}
```
Consumer类也十分的简单，只需要一个大家在Click事件里经常看到的 objectsender和EventArgs e两个对象，而第一个sender则表示的是这个事件的发送者，EventArgs e则表示的是传送过来的委托，像这个例子中，事件知发布了一个Car的name，当然它也可以发布一系列的消息传递给每个订阅它的其他对象。
```
static void Test8()
        {
            //
            var dealer = new CarDealer();
            //一个用户为Michael
            var michael = new Consumer("Michael");
            //注册了该订阅。
            dealer.NewCarInfo += michael.NewCarIsHere;
            //来了一个新车Mercedes
            dealer.NewCar("Mercedes");
            //一个用户为Nick
            var nick = new Consumer("Nick");
            //注册该订阅。
            dealer.NewCarInfo += nick.NewCarIsHere;
            //来了一个新车Ferrari
            dealer.NewCar("Ferrari");
            //删除了该订阅
            dealer.NewCarInfo -= michael.NewCarIsHere;
            //又来了一个新车Toyota
            dealer.NewCar("Toyota");
        }
```
调用的时候如下，这样，就可以看到神奇的一幕了。
![](https://img-blog.csdn.net/20160917095701381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
是不是，这样写，就不用知道什么时候车辆会到，而且车辆每次到了，工作人员都会知道它到了。是不是开始智能化了。
其实还有更加科幻的弱事件，但是我们现在暂时不讲，因为它牵扯到了垃圾回收等机制，暂时用不到，而且稍微比较复杂。等以后有机会，再来讨论它。
好了，委托，lambda表达式以及事件的内容就这么多。
