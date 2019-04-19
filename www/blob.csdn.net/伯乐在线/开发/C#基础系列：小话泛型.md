# C#基础系列：小话泛型 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4642820.html)
前言：这一章来总结下C#泛型技术的使用。据博主的使用经历，觉得泛型也是为了重用而生的，并且大部分时候会和反射一起使用。这次还是打算围绕WWH（即What、Why、How）来讲解。
1、什么是泛型：通过参数化类型来实现在同一份代码上操作多种数据类型。利用“参数化类型”将类型抽象化，从而实现灵活的复用。怎么理解呢，其实根据博主的理解，泛型就是将类型抽象化，使用抽象化的类型或对象去实现某些功能和业务，然后所有需要使用这些功能和业务的具体类型去调用泛型的方法和委托。呵呵，是不是还是有点晕，别着急，我们来个例子：
我们首先来定义一种场景：我们通过sql语句使用Ado.Net来查询默认得到的是弱类型的DataTable、DataReader等，而我们需要对查询到的结果集使用lamada表达式进行某些复杂的计算，需要将DataTable转换为对应的List<T>集合，首先来定义一个泛型的方法：

C#
```
public static List<T> GetListByDateTable<T>(DataTable dt)
        {
            List<T> modelList = new List<T>();
            try
            {
                //1.如果DataTable没有数据则直接返回
                if (dt == null || dt.Rows.Count == 0)
                {
                    return modelList;
                }
                //2.遍历DataTable填充实体
                var lstCol = dt.Columns;
                foreach (DataRow dr in dt.Rows)
                {
                    T model = default(T);
                    //如果是object(这种一般用于一个实体类表示不了的情况)，则先拼接json再反序列化为object
                    if (typeof(T).Equals(typeof(object)))
                    {
                        var strJson = "{";
                        foreach(DataColumn oCol in lstCol)
                        {
                            var oAttrValue = Convert.IsDBNull(dr[oCol.ColumnName]) ? null : dr[oCol.ColumnName];
                            strJson += "\"" + oCol.ColumnName + "\":\"" + oAttrValue + "\",";
                        }
                        strJson = strJson.ToString().Trim(',') + "}";
                        model = E2ERes.JavaScriptStrToObj<T>(strJson);
                    }
                    else
                    {
                        model = FillEntityByDT<T>(dt, dr);
                    }
                    modelList.Add(model);
                }
            }
            catch
            { 
            }
            return modelList;
        }
        //通过DataTable填充实体类
        private static T FillEntityByDT<T>(DataTable dt, DataRow dr)
        {
            T model = (T)typeof(T).GetConstructor(new System.Type[] { }).Invoke(new object[] { });//反射得到泛型类的实体
            PropertyInfo[] pro = typeof(T).GetProperties(BindingFlags.Instance | BindingFlags.Public);
            Type type = model.GetType();
            foreach (PropertyInfo propertyInfo in pro)
            {
                if (dt.Columns.Contains(propertyInfo.Name))
                {
                    if (Convert.IsDBNull(dr[propertyInfo.Name]))
                    {
                        continue;
                    }
                    if (!string.IsNullOrEmpty(Convert.ToString(dr[propertyInfo.Name])))
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, dr[propertyInfo.Name], null);
                    }
                }
            }
            return model;
        }
```
有了这个泛型的方法，我们在转换DataTable和具体的List<Model>的时候是不是就是一个很好的复用。
2、为什么要使用泛型：博主记得刚参加工作的前两年有一次面试的时候就被问到“泛型有什么优势？”，当时怎么回答的不记得了，只知道面试不太顺利~~为什么要用泛型呢？博主觉得泛型的主要优势有以下几点：
（1）保证了类型的安全性：泛型约束了变量的类型，保证了类型的安全性。例如List<int>和ArrayList。List<int>集合只能加入int类型的变量，ArrayList可以Add任何常用类型，编译的时候不会提示错误。
（2）避免了不必要的装箱、拆箱操作，提高程序的性能：泛型变量固定了类型，使用的时候就已经知道是值类型还是引用类型，避免了不必要的装箱、拆箱操作。举例说明：
使用泛型之前，我们使用object代替。

C#
```
object a=1;//由于是object类型，会自动进行装箱操作。
int b=(int)a;//强制转换，拆箱操作。这样一去一来，当次数多了以后会影响程序的运行效率。
```
使用泛型之后

C#
```
public static T GetValue<T>(T a)
{
　　return a;
}
public static void Main()
{
　　int b=GetValue<int>(1);//使用这个方法的时候已经指定了类型是int，所以不会有装箱和拆箱的操作。
}
```
（3）提高方法、算法的重用性。上面的例子基本能说明这个优势。
3、泛型的使用：
（1）泛型方法的使用：这也是博主使用最多的用法之一，像这种泛型方法一般是一些static的通用方法，例如原来项目中用到的一个将DataGridViewRow对象转换成对应的实体Model的方法如下：

C#
```
public static T ToObject<T>(DataGridViewRow item) where T:class
        {
            var model = item.DataBoundItem as T;
            if (model != null)
                return model;
            var dr = item.DataBoundItem as System.Data.DataRowView;
            model = (T)typeof(T).GetConstructor(new System.Type[] { }).Invoke(new object[] { });//反射得到泛型类的实体
            PropertyInfo[] pro = typeof(T).GetProperties(BindingFlags.Instance | BindingFlags.Public);
            Type type = model.GetType();
            foreach (PropertyInfo propertyInfo in pro)
            {
                if (Convert.IsDBNull(dr[propertyInfo.Name]))
                {
                    continue;
                }
                if (!string.IsNullOrEmpty(Convert.ToString(dr[propertyInfo.Name])))
                {
                    var propertytype = propertyInfo.PropertyType;
                    if (propertytype == typeof(System.Nullable<DateTime>) || propertytype == typeof(DateTime))
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, Convert.ToDateTime(dr[propertyInfo.Name]), null);
                    }
                    else if (propertytype == typeof(System.Nullable<decimal>) || propertytype == typeof(decimal))
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, Convert.ToDecimal(dr[propertyInfo.Name]), null);
                    }
                    else if (propertytype == typeof(System.Nullable<int>) || propertytype == typeof(int))
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, Convert.ToInt32(dr[propertyInfo.Name]), null);
                    }
                    else if (propertytype == typeof(System.Nullable<double>) || propertytype == typeof(double))
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, Convert.ToDouble(dr[propertyInfo.Name]), null);
                    }
                    else
                    {
                        type.GetProperty(propertyInfo.Name).SetValue(model, dr[propertyInfo.Name], null);
                    }
                }
            }
            return model;
        }
```
使用泛型方法的注意事项：
- 泛型方法的重载：public void Fun1<T>(T a);和public void Fun1<U>(U a);是无法重载的，这其实很好理解，因为T和U其实都是泛型的一个代表符号而已；
- 泛型方法的重写：下面的方法重写FuncA的重写是正确的，FuncB的重写不正确，因为约束被默认继承，不用再写。

C#
```
abstract class BaseClass
{
    public abstract T FuncA<T,U>(T t,U u) where U:T;
    public abstract T FuncB<T>(T t) where T:IComparable;
}
class ClassA:BaseClass
{
    public override X FuncA<X,Y>(X x,Y y){...}
    public override T FuncB<T>(T t) where T:IComparable{...}
}
```
（2）泛型类的使用：

C#
```
public class Class_Base<DTO, T>
{}
```
使用这个类的时候必须要指定两个泛型类。
（3）泛型接口以及泛型继承的使用：
泛型接口的类型参数要么已实例化，要么来源于实现类声明的类型参数

C#
```
public interface Interface_Base<T>
{}
public class Class_Base<DTO, T> : Interface_Base<DTO>
{}
```
DTO来源于实现类Class_Base
（4）泛型委托的使用：其实这种用法博主真的用得很少，只是原来见到过大牛们类似的代码。
定义泛型委托：

C#
```
public delegate void MyDelegate<T>(T obj);
```
泛型委托的使用：

C#
```
public delegate void MyDelegate<T>(T obj);
static void Main(string[] args)
{
    var method = new MyDelegate<int>(printInt);
    method(1);
    Console.ReadKey();
}
static void printInt(int i)
{
    Console.WriteLine(i);
}
```
（5）泛型约束：用来约束泛型类型有那些特性。常见的泛型约束也就那么几类：
泛型约束的格式

C#
```
public class Imps_Base<DTO, T> : Ifs_Base<DTO>
        where T : BaseEntity
        where DTO : class
    {
　　}
```
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/2e3a67ba1a6a404eb25ab1d84dc8c246.png)
### 本系列：
- [C#基础系列：再也不用担心面试官问我“事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98348/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
