# C#基础系列：扩展方法的使用 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4632467.html)
前言：打算分三个层面来介绍这个知识点，分别是：.Net内置对象的扩展方法、一般对象的扩展方法、泛型对象的扩展方法。
什么是扩展方法？回答这个问题之前，先看看我们一般情况下方法的调用。类似这样的通用方法你一定写过：

C#
```
static void Main(string[] args)
{
   string strRes = "2013-09-08 14:12:10";
   var dRes = GetDateTime(strRes);
}
//将字符串转换为日期
public static DateTime GetDateTime(string strDate)
{
   return Convert.ToDateTime(strDate);
}
//得到非空的字符串
public static string GetNotNullStr(string strRes)
{
   if (strRes == null)
     return string.Empty;
   else
     return strRes;
}
```
或者在项目中有一个类似Utils的工具类，里面有多个Helper，例如StringHelper、XmlHelper等等，每个Helper里面有多个static的通用方法，然后调用的时候就是StringHelper.GetNotNullStr(“aa”);这样。还有一种普通的用法就是new 一个对象，通过对象去调用类里面的非static方法。反正博主刚开始做项目的时候就是这样写的。后来随着工作经验的累积，博主看到了扩展方法的写法，立马就感觉自己原来的写法太Low了。进入正题。
**1、.Net内置对象的扩展方法**
.Net内部也有很多定义的扩展方法，例如我们Linq常用的Where(x=>x==true)、Select()等等。当你转到定义的时候你很容易看出来：public static IEnumerable<TSource> Where<TSource>(this IEnumerable<TSource> source, Func<TSource, bool> predicate)。当然我们也可以给.Net对象新增扩展方法，比如我们要给string对象加一个扩展方法（注意这个方法不能和调用的Main方法放在同一个类中）：

C#
```
public static string GetNotNullStr(this string strRes)
{
   if (strRes == null)
     return string.Empty;
   else
     return strRes ;
}
```
然后在Main方法里面调用：

C#
```
static void Main(string[] args)
{
   string strTest = null;
   var strRes = strTest.GetNotNullStr();
}
```
简单介绍：public static string GetNotNullStr(this string strRes)其中this string就表示给string对象添加扩展方法。那么在同一个命名空间下面定义的所有的string类型的变量都可以.GetNotNullStr()这样直接调用。strTest.GetNotNullStr();为什么这样调用不用传参数，是因为strTest就是作为参数传入到方法里面的。你可以试试。使用起来就和.Net framework定义的方法一样：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/9cdb039fad3e13e22c2697f7d0675a51.png)
当然除了string，你可以给.Net内置的其他对象加扩展方法，例如给DataGridViewRow的扩展方法：

C#
```
//DataGridViewRow的扩展方法，将当前选中行转换为对应的对象
  public static T ToObject<T>(this DataGridViewRow item) where T:class
  {
      var model = item.DataBoundItem as T;
      if (model != null)
         return model;
      var dr = item.DataBoundItem as System.Data.DataRowView;
      model = (T)typeof(T).GetConstructor(new System.Type[] { }).Invoke(new object[] { });
      //反射得到泛型类的实体
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
         }
      }
      return model;
}
```
这样看上去就像在扩展.Net Framework。有没有感觉有点高大上~
**2、一般对象的扩展方法**
和Framework内置对象一样，自定义的对象也可以增加扩展方法。直接上示例代码：

C#
```
public class Person
 {
     public string Name { set; get; }
     public int Age { set; get; }
 }
```


C#
```
//Person的扩展方法，根据年龄判断是否是成年人
  public static bool GetBIsChild(this Person oPerson)
  {
    if (oPerson.Age >= 18)
        return false;
    else
        return true;
   }
```
Main方法里面调用：

C#
```
var oPerson1 = new Person();
oPerson1.Age = 20;
var bIsChild = oPerson1.GetBIsChild();
```
和string扩展方法类似，就不多做解释了。
**3、泛型对象的扩展方法**
除了上面两种之外，博主发现其实可以定义一个泛型的扩展方法。那么，是不是所有的类型都可以直接使用这个扩展方法了呢？为了保持程序的严谨，下面的方法可能没有实际意义，当开发中博主觉得可能存在这种场景：

C#
```
public static class DataContractExtensions
{
  //测试方法
  public static T Test<T>(this T instance) where T : Test2
  {
       T Res = default(T);
       try
       {
           Res.AttrTest = instance.AttrTest.Substring(0,2);
           //其他复杂逻辑...
      }
      catch
      { }
      return Res;
  }
}
public class Test2
{
　　public string AttrTest { set; get; }
}
```
使用扩展方法有几个值得注意的地方：
（1）扩展方法不能和调用的方法放到同一个类中
（2）第一个参数必须要，并且必须是this，这是扩展方法的标识。如果方法里面还要传入其他参数，可以在后面追加参数
（3）扩展方法所在的类必须是静态类
（4）最好保证扩展方法和调用方法在同一个命名空间下
可能你第一次使用这个会觉得很别扭。你也许会说扩展方法和我以前用的static方法无论从代码实现还是算法效率都差不多嘛，是的！确实差不多，但使用多了之后会发现它确实能帮你省去很多代码。
### 本系列：
- [C#基础系列：再也不用担心面试官问我”事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98370/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
