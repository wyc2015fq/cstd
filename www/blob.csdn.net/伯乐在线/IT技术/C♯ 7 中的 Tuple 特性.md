# C♯ 7 中的 Tuple 特性 - 文章 - 伯乐在线
原文出处： [SANDEEP SINGH](http://www.c-sharpcorner.com/article/tuple-in-c-sharp-7/)   译文出处：[oschina](https://www.oschina.net/translate/tuple-in-c-sharp-7)
**介绍**
Tuple是异类对象的有序序列。 我们经常可以写出返回多个值的方法，所以我们需要创建一个包含多个数据元素的简单结构。 为了支持这些情况，Tuple 被添加到 C＃。 Tuple 是包含多个字段用来表示数据成员的轻量级数据结构。
如果一个方法返回多个相同类型的数值，那么它可以将这些值存储在一个集合中并返回该集合。 但是如果一个方法需要返回多个不同类型的值呢，C＃ 提供了一些可选项，比如 Class / Struct，输出参数和 Tuple。
让我们创建一个示例。 我们有一个整数的集合，而且我们需要从这个集合中找出的最小值和最大值。 这时候我们需要创建一个返回最小值和最大值的方法。 为了实现这一点，我们有三个选项：Class / Struct，输出参数和 Tuple。  那么，让我们逐个看看每个选项是如何完成这一功能的。
**使用 out 参数**
当我们创建一个方法，找出数字序列中的最大值和最小值。 该方法需要将两个值作为最大值和最小值的结果。 因此，我们创建返回值和使用 out 参数作为参数的方法。 返回值保存系列中的最大数，而 out 参数保存系列中的最小数。
让我们创建一个名为 Operation 的类，它实现一个从数字系列中查找最小和最大数的方法。 以下代码段就能实现该功能。


```
using System.Collections.Generic;  
  
namespace TupleApp  
{  
    class Operation  
    {  
        internal int FindMinMax(List<int> list, out int min)  
        {  
            int maximum = int.MinValue, minimum = int.MaxValue;  
            list.ForEach(n =>  
            {  
                minimum = n < minimum ? n : minimum;  
                maximum = n > maximum ? n : maximum;  
            });  
            min = minimum;  
            return maximum;  
        }  
    }  
}
```
根据上述代码片段，该方法返回一个值，该值保存一个名为 maximum 的整形变量。 该值是该系列的最大值。 此方法将输出参数是名为 min 的参数，该参数保存的是系列中的最小值。
现在，根据下面的代码片段从可执行程序调用此方法。


```
using System;  
using System.Collections.Generic;  
using System.Linq;  
using static System.Console;  
  
namespace TupleApp  
{  
    class Program  
    {  
        static void Main(string[] args)  
        {  
            List<int> numbers = Enumerable.Range(1, 100).OrderBy(x => Guid.NewGuid()).Take(10).ToList();  
            int minimum = 0, maximum = 0;  
            Operation operation = new Operation();  
            maximum = operation.FindMinMax(numbers, out minimum);  
            Write($"{minimum} is min and {maximum} is max in {String.Join(",", numbers)}");  
            Read();  
        }  
    }  
}
```
现在，运行应用程序。 结果如下图所示。
![](http://jbcdn2.b0.upaiyun.com/2017/03/94c6d15609ab26cd36fd3aaa2276f825.png)
图1：输出
这是正确的方法，但 out 参数并不适合于异步方法。
**使用 Class/Struct**
还有另一个选项可以从一个方法获取多个值–创建一个具有该数据结构的类或结构体。 我们在这里也执行相同的操作。 我们创建一个名为 CalculateData 的结构体，这个结构体中有两个属性来保存一个系列的最大值和最小值。 以下代码段就是用来创建这个结构体。


```
namespace TupleApp  
{  
    struct CalculateData  
    {  
        public int Minimum { get; set; }  
        public int Maximum { get; set; }  
    }  
}
```
现在，让我们创建另一个名为 Operation 的类，它用来实现从数字序列中查找最小值和最大值的方法。 以下代码段就是用来实现这一功能。


```
using System.Collections.Generic;  
  
namespace TupleApp  
{  
    class Operation  
    {  
        internal CalculateData FindMinMax(List<int> list)  
        {  
            int maximum = int.MinValue, minimum = int.MaxValue;  
            list.ForEach(n =>  
            {  
                minimum = n < minimum ? n : minimum;  
                maximum = n > maximum ? n : maximum;  
            });  
            CalculateData data = new CalculateData  
            {  
                Minimum = minimum,  
                Maximum = maximum  
            };  
            return data;  
        }  
    }  
}
```
根据上面的代码片段，该方法返回一个具有两个属性的对象。 这些属性里保存的是系列中的最大值和最小值。
现在，从可执行程序调用此方法，如下面的代码片段所示。


```
using System;  
using System.Collections.Generic;  
using System.Linq;  
using static System.Console;  
  
namespace TupleApp  
{  
    class Program  
    {  
        static void Main(string[] args)  
        {  
            List<int> numbers = Enumerable.Range(1, 100).OrderBy(x => Guid.NewGuid()).Take(10).ToList();        
            Operation operation = new Operation();  
            CalculateData data = operation.FindMinMax(numbers);  
            Write($"{data.Minimum} is min and {data.Maximum} is max in {String.Join(",", numbers)}");  
            Read();  
        }  
    }  
}
```
现在，运行应用程序。 最终结果如下图所示。
![](http://jbcdn2.b0.upaiyun.com/2017/03/b87e76b72a038a9b882d4daf505d2c3d.png)
图2: 输出结果
**使用 Tuple**
C＃7 引入了定义元组的新方法。 Tuple 是从方法返回多个值的另一个选择。 它可以保存多个不同类型的值。 要在应用程序中使用 Tuple，我们需要安装 System.ValueTuple NuGet 包.
![](http://jbcdn2.b0.upaiyun.com/2017/03/8d0fcb4ae4ec5b51931a937b472a7831.png)
图3 NuGet 包
我们在这里也执行相同的操作。 让我们创建一个名为 Operation 的类，它实现一个从数字系列中查找最小值和最大数的方法。 以下代码段用来实现该功能。


```
using System.Collections.Generic;  
  
namespace TupleApp  
{  
    class Operation  
    {  
        internal (int, int) FindMinMax(List<int> list)  
        {  
            int maximum = int.MinValue, minimum = int.MaxValue;  
            list.ForEach(n =>  
            {  
                minimum = n < minimum ? n : minimum;  
                maximum = n > maximum ? n : maximum;  
            });  
            return (minimum, maximum);  
        }  
    }  
}
```
根据上面的代码片段，该方法返回一个有两个项目的元组。 这些项目保存数字系列的最大值和最小值。
现在，根据下面的代码片段在可执行程序调用这个方法。


```
using System;  
using System.Collections.Generic;  
using System.Linq;  
using static System.Console;  
  
namespace TupleApp  
{  
    class Program  
    {  
        static void Main(string[] args)  
        {  
            List<int> numbers = Enumerable.Range(1, 100).OrderBy(x => Guid.NewGuid()).Take(10).ToList();  
            Operation operation = new Operation();  
            (int, int) data = operation.FindMinMax(numbers);  
            Write($"{data.Item1} is min and {data.Item2} is max from {String.Join(",", numbers)}");  
            Read();  
        }  
    }  
}
```
根据上面的代码片段，名为 FindMinMax 的方法调用并返回一个元组。 当方法直接返回多个值到一个 Tuple 类型，它们会按照他们的顺序给它们一些默认名称，以便可以方便地调用它们。这元组有两个项目，因此这些项目调用 Item1 和 Item2。 Item1 表示第一个值，而 Item2 表示第二个值。这跟 Tupple 项在创建时使用的顺序相同。
现在，运行应用程序。 最终结果如下图所示。
![](http://jbcdn2.b0.upaiyun.com/2017/03/0f16ed745e86a328efe4571c526c7eed.png)
图4 输出结果
由于 Item1 和 Item2 不表示字段的实际名称，因此我们可以给它们自定义名称。 元组类型变量可以具有自定义名称，而不是仅仅是默认的 Item1 或 Item2。
让我们更新 Operation 类中名为 FindMinMax 的现有方法。 我们为这些元组字段分配自定义名称。 以下代码段用来实现该功能。


```
using System.Collections.Generic;  
  
namespace TupleApp  
{  
    class Operation  
    {  
        internal (int Minimum, int Maximum) FindMinMax(List<int> list)  
        {  
            int maximum = int.MinValue, minimum = int.MaxValue;  
            list.ForEach(n =>  
            {  
                minimum = n < minimum ? n : minimum;  
                maximum = n > maximum ? n : maximum;  
            });  
            return (minimum, maximum);  
        }  
    } 
}
```
这里，元组的第一个字段名称为 Minimum，而另一个字段名为 Maximum。
现在，根据下面的代码片段从可执行程序调用此方法。


```
using System;  
using System.Collections.Generic;  
using System.Linq;  
using static System.Console;  
  
namespace TupleApp  
{  
    class Program  
    {  
        static void Main(string[] args)  
        {  
            List<int> numbers = Enumerable.Range(1, 100).OrderBy(x => Guid.NewGuid()).Take(10).ToList();  
            Operation operation = new Operation();  
            var data = operation.FindMinMax(numbers);  
            Write($"{data.Minimum} is min and {data.Maximum} is max from {String.Join(",", numbers)}");  
            Read();  
        }  
    }  
}
```
名为 FindMinMax 的方法调用并返回一个元组。 该元组有两个项目，根据元组项目的顺序称为最小值和最大值。
现在，运行应用程序并查看结果。
![](http://jbcdn2.b0.upaiyun.com/2017/03/595b5e4654dd28d60aaa0d6e653cea77.png)
图5 输出结果
**结论**
Tuple 是异类对象的有序序列。 当一个方法需要返回多个值的时候使用它。Tuple 实例的条目数是固定的。Tuple 有最大数目为 8 项的限制。 如果我们想创建一个带有更多项的 Tuple，我们必须创建嵌套的 Tuple。 Tuple 的第八项必须是另一个 Tuple。
