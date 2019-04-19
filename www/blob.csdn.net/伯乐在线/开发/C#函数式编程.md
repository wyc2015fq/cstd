# C#函数式编程 - 文章 - 伯乐在线
原文出处： [richieyangs](http://www.cnblogs.com/richieyang/p/4396048.html)
提起函数式编程，大家一定想到的是语法高度灵活和动态的LISP,Haskell这样古老的函数式语言，往近了说ruby,javascript，F#也是函数式编程的流行语言。然而自从.net支持了lambda表达式，C#虽然作为一种指令式程序设计语言，在函数式编程方面也毫不逊色。
我们在使用c#编写代码的过程中，有意无意的都会使用高阶函数，组合函数，纯函数缓存等思想，连表达式树这样的idea也来自函数式编程思想。所以接下来我们把常用的函数式编程场景做个总结，有利于我们在程序设计过程中灵活应用这些技术，拓展我们的设计思路和提高代码质量。
## 一、高阶函数
高阶函数通俗的来讲：某个函数中使用了函数作为参数，这样的函数就称为高阶函数。根据这样的定义，.net中大量使用的LINQ表达式，Where,Select,SelectMany,First等方法都属于高阶函数，那么我们在自己写代码的时候什么时候会用到这种设计？
举例：设计一个计算物业费的函数，var fee=square*price, 而面积(square)根据物业性质的不同，计算方式也不同。民用住宅,商业住宅等需要乘以不同的系数，根据这样的需求我们试着设计下面的函数：
民用住宅面积：

C#
```
public Func SquareForCivil()
        {
            return (width,hight)=>width*hight;
        }
```
商业住宅面积：

C#
```
public Func SquareForBusiness()
        {
            return (width, hight) => width * hight*1.2m;
        }
```
这些函数都有共同的签名：Func,所以我们可以利用这个函数签名设计出计算物业费的函数：

C#
```
public decimal PropertyFee(decimal price,int width,int hight, Func square)
        {
            return price*square(width, hight);
        }
```
是不是很easy,写个测试看看

C#
```
[Test]
        public void Should_calculate_propertyFee_for_two_area()
        {
            //Arrange
            var calculator = new PropertyFeeCalculator();
            //Act
            var feeForBusiness= calculator.PropertyFee(2m,2, 2, calculator.SquareForBusiness());
            var feeForCivil = calculator.PropertyFee(1m, 2, 2, calculator.SquareForCivil());
            //Assert
            feeForBusiness.Should().Be(9.6m);
            feeForCivil.Should().Be(4m);
        }
```
## 二、惰性求值
C#在执行过程使用严格求值策略，所谓严格求值是指参数在传递给函数之前求值。这个解释是不是还是有点不够清楚？我们看个场景：有一个任务需要执行，要求当前内存使用率小于80%，并且上一步计算的结果
我们可以很快写出符合这个要求的C#代码：

C#
```
public double MemoryUtilization()
 {
     //计算目前内存使用率
     var pcInfo = new ComputerInfo();
     var usedMem = pcInfo.TotalPhysicalMemory - pcInfo.AvailablePhysicalMemory; 
     return (double)(usedMem / Convert.ToDecimal(pcInfo.TotalPhysicalMemory));
 }
 
 public int BigCalculatationForFirstStep()
 {
     //第一步运算
     System.Threading.Thread.Sleep(TimeSpan.FromSeconds(2));
     Console.WriteLine("big calulation");
     FirstStepExecuted = true;
     return 10;
 }
 
 public void NextStep(double memoryUtilization,int firstStepDistance)
 {
//下一步运算
     if(memoryUtilization<0.8&&firstStepDistance<100)
     {
         Console.WriteLine("Next step");
     }
 }
```
在执行NextStep的时候需要传入内存使用率和第一步(函数BigCalculatationForFirstStep)的计算结果，如代码所示，第一步操作是一个很费时的运算，但是由于C#的严格求值策略，对于语句if(memoryUtilization
所以惰性求值是指：表达式或者表达式的一部分只有当真正需要它们的结果时才会对它们进行求值。我们尝试用高阶函数来重写这个需求：

C#
```
public void NextStepWithOrderFunction(Func<double> memoryUtilization,Func<int> firstStep)
{
    if (memoryUtilization() < 0.8 && firstStep() < 100)
    {
        Console.WriteLine("Next step");
    }
}
```
代码很简单，就是用一个函数表达式来代替函数值，如果if (memoryUtilization() 类，大家可以在有这种需求的场景下使用这个机制。
## 三、函数柯里化(Curry)
柯里化也称作局部套用。定义：是把接受多个参数的函数变换成接受一个单一参数(最初函数的第一个参数)的函数，并且返回接受余下的参数且返回结果的新函数的技术,ps：为什么官方解释这么绕口？
看到这样的定义估计大家也很难明白这是这么一回事，所以我们从curry的原理讲起：
写一个两个数相加的函数：

C#
```
public Func<int, int, int> AddTwoNumber()
{
    return (x, y) => x + y;
}
```
ok, 如何使用这个函数？

C#
```
var result= _curringReasoning.AddTwoNumber()(1,2);
```
1+2=3，调用很简单。需求升级，我们需要一个函数，这个函数要求输入一个参数(number)，算出10+输入的参数(number)的结果。估计有人要说了，这需求上面的代码完全可以实现啊，第一个参数你传入10不就完了么，ok,如果你是这样想的，我也是无可奈何。还有人可能说了，再写一个重载，只要一个参数即可，实际情况是不容许，我们在调用别人提供的api,无法添加重载。可以看到局部套用的使用场景不是一种很普遍的场景，所以在合适的场景配合合适的技术才是最好的设计，我们来看局部套用的实现：

C#
```
public Func<int, Func<int, int>> AddTwoNumberCurrying()
{
    Func<int, Func<int, int>> addCurrying = x => y => x + y;
    return addCurrying;
}
```
表达式x => y => x + y得到的函数签名为Func>,这个函数签名非常清楚，接收一个int类型的参数，得到一个Func类型的函数。此时如果我们再调用：

C#
```
//Act
var curringResult = curringReasoning.AddTwoNumberCurrying()(10);
var result = curringResult(2);
 
//Assert
result.Should().Be(12);
```
这句话：var curringResult = curringReasoning.AddTwoNumberCurrying()(10); 生成的函数就是只接收一个参数(number)，且可以计算出10+number的函数。
同样的道理，三个数相加的函数：

C#
```
public Func<int,int,int,int> AddThreeNumber()
{
    return (x, y, z) => x + y + z;
}
```
局部套用版本：

C#
```
public Func<int,Func<int,Func<int,int>>> AddThreeNumberCurrying()
{
    Func<int, Func<int, Func<int, int>>> addCurring = x => y => z => x + y + z;
    return addCurring;
}
```
调用过程：

C#
```
[Test]
public void Three_number_add_test()
{
    //Arrange
    var curringReasoning = new CurryingReasoning();
 
    //Act
    var result1 = curringReasoning.AddThreeNumber()(1, 2, 3);
    var curringResult = curringReasoning.AddThreeNumberCurrying()(1);
    var curringResult2 = curringResult(2);
    var result2 = curringResult2(3);
     
    //Assert
    result1.Should().Be(6);
    result2.Should().Be(6);
}
```
当函数参数多了之后，手动局部套用越来越不容易写，我们可以利用扩展方法自动局部套用：

C#
```
public static Func<T1, Func<T2, TResult>> Curry<T1, T2, TResult>(this Func<T1, T2, TResult> func)
{
    return x => y => func(x, y);
}
 
public static Func<T1, Func<T2, Func<T3, TResult>>> Curry<T1, T2, T3, TResult>(this Func<T1, T2, T3,TResult> func)
{
    return x => y => z=>func(x, y,z);
}
```
同样的道理，Action签名的函数也可以自动套用
有了这些扩展方法，使用局部套用的时候就更加easy了

C#
```
[Test]
public void Should_auto_curry_two_number_add_function()
{
    //Arrange
    var add = _curringReasoning.AddTwoNumber();
    var addCurrying = add.Curry();
 
    //Act
    var result = addCurrying(1)(2);
 
    //Assert
    result.Should().Be(3);
}
```
好了，局部套用就说到这里，stackoverflow有几篇关于currying使用的场景和定义的文章,大家可以继续了解。
函数式编程还有一些重要的思想，例如：纯函数的缓存，所为纯函数是指函数的调用不受外界的影响，相同的参数调用得到的值始终是相同的。尾递归，单子，代码即数据(.net中的表达式树)，部分应用，组合函数，这些思想有的我也仍然在学习中，有的还在思考其最佳使用场景，所以不再总结，如果哪天领会了其思想会补充。
## 四、设计案例
最后我还是想设计一个场景，把高阶函数，lambda表达式，泛型方法结合在一起，我之所以设计这样的例子是因为现在很多的框架，开源的项目都有类似的写法，也正是因为各种技术和思想结合在一起，才有了极富有表达力并且非常优雅的代码。
需求：设计一个单词查找器，该查找器可以查找某个传入的model的某些字段是否包含某个单词，由于不同的model具有不同的字段，所以该查找需要配置，并且可以充分利用vs的智能提示。
这个功能其实就两个方法：

C#
```
private readonly List<Func<string, bool>> _conditions; 
 
public WordFinder<TModel> Find<TProperty>(Func<TModel,TProperty> expression)
{
    Func<string, bool> searchCondition = word => expression(_model).ToString().Split(' ').Contains(word);
    _conditions.Add(searchCondition);
    return this;
}
 
public bool Execute(string wordList)
{
    return _conditions.Any(x=>x(wordList));
}
```
使用：

C#
```
[Test]
public void Should_find_a_word()
{
    //Arrange
    var article = new Article()
    {
        Title = "this is a title",
        Content = "this is content",
        Comment = "this is comment",
        Author = "this is author"
    };
 
    //Act
    var result = Finder.For(article)
        .Find(x => x.Title)
        .Find(x => x.Content)
        .Find(x => x.Comment)
        .Find(x => x.Author)
        .Execute( "content");
 
    //Assert
    result.Should().Be(true);
}
```
该案例本身不具有实用性，但是大家可以看到，正是各种技术的综合应用才设计出极具语义的api, 如果函数参数改为Expression> 类型，我们还可以读取到具体的属性名称等信息。
结束语：本文总结了比较常用的函数式编程思想，有了这些设计思想可以扩充你的编程思路，也有利于编写更出色的代码。本文章所使用的源码提供下载，转载请注明出处
