# C#基础系列：反射笔记 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4642814.html)
前言：使用反射也有几年了，但是一直觉得，反这个概念很抽象，今天有时间就来总结下这个知识点。
1、为什么需要反射：
最初使用反射的时候，作为小菜总是不理解，既然可以通过new 一个对象的方式得到对象，然后通过对象去调用属性和方法，那么为什么还需要反射去调用呢？后来使用多了发现这就是一个先绑定还是后绑定的问题，很多初使用反射的开发人员通常都会有类似这种疑虑：既然在开发时就能够写好代码，干嘛还放到运行期去做，不光繁琐，而且效率也受影响。博主觉得主要是适用性的问题，如果你的系统没有那么高的扩展性和灵活性要求，你大可不必考虑反射。但在架构设计时，很多东西都需要考虑复用性，并且在某些特定的场景下你得不到具体的类时，你就必须用到反射。博主总结了下自己使用过的反射场景：
（1）有时不知道具体的类型，可以通过dll去得到类的对象；
（2）某些特殊方法，传过来的是泛型类，需要通过反射处理某些特殊的业务；
（3）通用方法DataTable和List<T>的相互转化时需要用到反射；
2、如何使用反射：
（1）反射dll得到类成员：
在一个未知的dll里面有一个Person类

C#
```
public class Person
 {
        private string address;
        private string email;
        public string Name { set; get; }
        public int Age { set; get; }
        public void SayHello()
        {
            Console.WriteLine("你好");
        }
        public static string MystaticPro { set; get; }
        public static void MyStatic()
        {
            Console.WriteLine("我是static方法");
        }
  }
```
通过反射dll得到Person类

C#
```
static void Main(string[] args)
 {
　　　//通过类名称得到类型
　　　//var assembly = Assembly.Load("Ewin.Client.Web");//参数为程序集的名称
　　  //var oType = assembly.GetType("Ewin.Client.Web.Controllers." + strType);
      //反射dll
      var strDllPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "dll\\ReflectorDLL.dll");
      var oAssembly = Assembly.LoadFile(strDllPath);
      var lstTypes = oAssembly.GetTypes();
      foreach (var oType in lstTypes)
      {
         if (oType.Name == "Person")
         {
             //默认得到类下面的所有public成员
             var lstMembers = oType.GetMembers();
             foreach (var oMem in lstMembers)
             {
                 Console.WriteLine("GetMembers()方法得到的成员名称："+oMem.Name);
             }
             Console.WriteLine("");
             //默认得到类下面的所有public属性
             var lstProperty = oType.GetProperties();
             foreach (var oProp in lstProperty)
             {
                Console.WriteLine("GetProperties()方法得到的成员名称：" + oProp.Name);
             }
             Console.WriteLine("");
             //默认得到类下面的所有public字段
             var lstField = oType.GetFields();
             foreach (var oField in lstField)
             {
                Console.WriteLine("GetFields()方法得到的成员名称：" + oField.Name);
             }
        }
     }
     Console.ReadKey();
  }
```
得到结果![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/184fd44e12c5369da74ba3f6f9189e06.png)
（2）反射对象的私有成员：
一般私有属性的用法比较少，我们就以私有字段为例来说明，还是上面的例子：

C#
```
static void Main(string[] args)
{
  //反射dll
  var strDllPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "dll\\ReflectorDLL.dll");
  var oAssembly = Assembly.LoadFile(strDllPath);
  var lstTypes = oAssembly.GetTypes();
  foreach (var oType in lstTypes)
  {
      if (oType.Name == "Person")
      {
          //默认得到类下面的所有public字段
          var lstField = oType.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
          foreach (var oField in lstField)
          {
               Console.WriteLine("GetFields()方法得到的成员名称：" + oField.Name);
           }
       }
   }
   Console.ReadKey();
}
```
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/f97c21dabe1a9d1ccb1db229df6751fa.png)
（3）反射对象的静态成员：

C#
```
static void Main(string[] args)
{
  //反射dll
  var strDllPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "dll\\ReflectorDLL.dll");
  var oAssembly = Assembly.LoadFile(strDllPath);
  var lstTypes = oAssembly.GetTypes();
  foreach (var oType in lstTypes)
  {
      if (oType.Name == "Person")
      {
         //默认得到类下面的所有public成员
         var lstMembers = oType.GetMembers(BindingFlags.Public|BindingFlags.Static);
         foreach (var oMem in lstMembers)
         {
              Console.WriteLine("GetMembers()方法得到的成员名称：" + oMem.Name);
         }
         Console.WriteLine("");
         //默认得到类下面的所有public字段
         var lstField = oType.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
         foreach (var oField in lstField)
         {
              Console.WriteLine("GetFields()方法得到的成员名称：" + oField.Name);
         }
      }
    }
    Console.ReadKey();
}
```
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/1eea947a55649bfc516155cc9780ed1c.png)
还有枚举类型等等就不一一介绍了，基本上都是在BindingFlags这个上面做处理。
（4）反射得到对象以及对象的操作：
反射得到对象的方法主要有两种

C#
```
public static T GetModel<T>(T oModel)
{
   var model = default(T) ;
   //得到对象的方法一：
   model = (T)typeof(T).GetConstructor(new System.Type[] { }).Invoke(new object[] { });
   //反射得到泛型类的实体
   //得到对象的方法二：
   model = (T)Activator.CreateInstance(typeof(T));
   //逻辑处理......
   return model;
}
```
对象属性的取值和赋值：

C#
```
//List集合转换为DataTable
        public static DataTable ListFillTable(object obj)
        {
            if (!(obj is IList))
            {
                return null;
            }
            var objlist = obj as IList;
            if (objlist == null || objlist.Count <= 0)
            {
                return null;
            }
            var tType = objlist[0];
            DataTable dt = new DataTable(tType.GetType().Name);
            DataColumn column;
            DataRow row;
            System.Reflection.PropertyInfo[] myPropertyInfo = tType.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance);
            foreach (var t in objlist)
            {
                if (t == null)
                {
                    continue;
                }
                row = dt.NewRow();
                for (int i = 0, j = myPropertyInfo.Length; i < j; i++)
                {
                    System.Reflection.PropertyInfo pi = myPropertyInfo[i];
                    string name = pi.Name;
                    if (dt.Columns[name] == null)
                    {
                        var coltype = pi.PropertyType;
                        if (coltype.Name == "Nullable`1")
                        {
                            //coltype = typeof(System.DBNull);
                            column = new DataColumn(name);
                        }
                        else
                        {
                            column = new DataColumn(name, coltype);
                        }
                        dt.Columns.Add(column);
                    }
                    row[name] = pi.GetValue(t, null);
                }
                dt.Rows.Add(row);
            }
            return dt;
        }
```
### 本系列：
- [C#基础系列：再也不用担心面试官问我”事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98370/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
