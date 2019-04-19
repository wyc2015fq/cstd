# C#中数组Array、ArrayList、泛型List<T>的比较 - 文章 - 伯乐在线
原文出处： [Joye.Net](http://www.cnblogs.com/yinrq/p/5584072.html#undefined)
在C#中数组Array，ArrayList，泛型List都能够存储一组对象，但是在开发中根本不知道用哪个性能最高，下面我们慢慢分析分析。
## 一、数组Array
数组是一个存储相同类型元素的固定大小的顺序集合。数组是用来存储数据的集合，通常认为数组是一个同一类型变量的集合。
Array 类是 C# 中所有数组的基类，它是在 System 命名空间中定义。
数组在内存中是连续存储的，所以它的索引速度非常快，而且赋值与修改元素也非常简单。
Array数组具体用法：


C#
```
using System;
namespace WebApp
{
    class Program
    {
        static void Main(string[] args)
        {
            //System.Array
            //1、数组[] 特定类型、固定长度
            string[] str1 = new string[3];
            str1[0] = "a";
            str1[1] = "b";
            str1[2] = "c";
            Console.WriteLine(str1[2]);
            string[] str2 = new string[] { "a", "b", "c" };
            Console.WriteLine(str2[0]);
            string[] str3 = { "a", "b", "c" };
            Console.WriteLine(str3[0]);
            //2、二维数组
            //int[,] intArray=new int[2,3]{{1,11,111},{2,22,222}};
            int[,] intArray = new int[2, 3];
            intArray[0, 0] = 1;
            intArray[0, 1] = 11;
            intArray[0, 2] = 111;
            intArray[1, 0] = 2;
            intArray[1, 1] = 22;
            intArray[1, 2] = 222;
            Console.WriteLine("{0},{1},{2}", intArray[0, 0], intArray[0, 1], intArray[0, 2]);
            Console.WriteLine("{0},{1},{2}", intArray[1, 0], intArray[1, 1], intArray[1, 2]);
            //3、多维数组
            int[, ,] intArray1 = new int[,,]
            {
                {{1, 1}, {11, 11}, {111, 111}},
                {{2, 2}, {22, 22}, {222, 222}},
                {{3, 3}, {33, 33}, {333, 333}}
            };
            Console.WriteLine("{0},{1},{2},{3},{4},{5}", intArray1[0, 0, 0], intArray1[0, 0, 1], intArray1[0, 1, 0], intArray1[0, 1, 1],
               intArray1[0, 2, 0], intArray1[0, 2, 1]);
            Console.WriteLine("{0},{1},{2},{3},{4},{5}", intArray1[1, 0, 0], intArray1[1, 0, 1], intArray1[1, 1, 0], intArray1[1, 1, 1],
                intArray1[1, 2, 0], intArray1[1, 2, 1]);
            Console.WriteLine("{0},{1},{2},{3},{4},{5}", intArray1[2, 0, 0], intArray1[2, 0, 1], intArray1[2, 1, 0], intArray1[2, 1, 1],
                intArray1[2, 2, 0], intArray1[2, 2, 1]);
            //4、交错数组即数组的数组
            int[][] intArray2 = new int[4][];
            intArray2[0] = new int[] { 1 };
            intArray2[1] = new int[] { 2, 22 };
            intArray2[2] = new int[] { 3, 33, 333 };
            intArray2[3] = new int[] { 4, 44, 444,4444 };
            for (int i = 0; i < intArray2.Length; i++)
            {
                for (int j = 0; j < intArray2[i].Length; j++)
                {
                    Console.WriteLine("{0}", intArray2[i][j]);
                }
            }
            Console.ReadKey();
        }
    }
}
```
数组虽然存储检索数据很快，但是也有一些缺点：
1、在声明数组的时候必须指定数组的长度，如果不清楚数组的长度，就会变得很麻烦。
2、数组的长度太长，会造成内存浪费；太短会造成数据溢出的错误。
3、在数组的两个数据间插入数据是很麻烦的
更多参考微软官方文档：[Array 类 (System)](https://msdn.microsoft.com/zh-cn/library/system.array(VS.80).aspx)
## 二、ArrayList
既然数组有很多缺点，C#就提供了ArrayList对象来克服这些缺点。
ArrayList是在命名空间System.Collections下，在使用该类时必须进行引用，同时继承了IList接口，提供了数据存储和检索。
ArrayList对象的大小是按照其中存储的数据来动态扩充与收缩的。因此在声明ArrayList对象时并不需要指定它的长度。
ArrayList 的默认初始容量为 0。随着元素添加到 ArrayList 中，容量会根据需要通过重新分配自动增加。可通过调用 TrimToSize 或通过显式设置 Capacity 属性减少容量。


C#
```
using System;
using System.Collections;
public class SamplesArrayList  {
   public static void Main()  {
      ArrayList myAL = new ArrayList();
      myAL.Add("Hello");
      myAL.Add("World");
      myAL.Add("!");
      Console.WriteLine( "myAL" );
      Console.WriteLine( "    Count:    {0}", myAL.Count );
      Console.WriteLine( "    Capacity: {0}", myAL.Capacity );
      Console.Write( "    Values:" );
      PrintValues( myAL );
   }
   public static void PrintValues( IEnumerable myList )  {
      foreach ( Object obj in myList )
         Console.Write( "   {0}", obj );
      Console.WriteLine();
      Console.ReadKey();
   }
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/06/3575a25707dd634ba558c7a75daa12b0.png)
ArrayList解决了数组中所有的缺点，但是在存储或检索值类型时通常发生装箱和取消装箱操作，带来很大的性能耗损。尤其是装箱操作，例如：


C#
```
ArrayList list = new ArrayList();
            //add 
            list.Add("joye.net");
            list.Add(27);
            //update  
            list[2] = 28;
            //delete 
            list.RemoveAt(0);
            //Insert  
            list.Insert(0, "joye.net1");
```
在List中，先插入了字符串[joye.net](http://www.cnblogs.com/yinrq/)，而且插入了int类型27。这样在ArrayList中插入不同类型的数据是允许的。因为ArrayList会把所有插入其中的数据当作为object类型来处理，在使用ArrayList处理数据时，很可能会报类型不匹配的错误，也就是**ArrayList不是类型安全的**。
更多参考微软官方ArrayList文档：[ArrayList 类 (System.Collections)](https://msdn.microsoft.com/zh-cn/library/system.collections.arraylist(VS.80).aspx)
## 三、泛型List<T>
由于ArrayList存在不安全类型与装箱拆箱的缺点，所以出现了泛型的概念。
List 类是 ArrayList 类的泛型等效类。该类使用大小可按需动态增加的数组实现 IList 泛型接口，大部分用法都与ArrayList相似。
List<T> 是类型安全的，在声明List集合时，必须为其声明List集合内数据的对象类型。


C#
```
using System;
using System.Collections.Generic;
public class Example
{
    public static void Main()
    {
        List<string> dinosaurs = new List<string>();
        Console.WriteLine("\nCapacity: {0}", dinosaurs.Capacity);
        dinosaurs.Add("Tyrannosaurus");
        dinosaurs.Add("Amargasaurus");
        dinosaurs.Add("Mamenchisaurus");
        dinosaurs.Add("Deinonychus");
        dinosaurs.Add("Compsognathus");
        Console.WriteLine();
        foreach(string dinosaur in dinosaurs)
        {
            Console.WriteLine(dinosaur);
        }
        Console.WriteLine("\nCapacity: {0}", dinosaurs.Capacity);
        Console.WriteLine("Count: {0}", dinosaurs.Count);
        Console.WriteLine("\nContains(\"Deinonychus\"): {0}",
            dinosaurs.Contains("Deinonychus"));
        Console.WriteLine("\nInsert(2, \"Compsognathus\")");
        dinosaurs.Insert(2, "Compsognathus");
        Console.WriteLine();
        foreach(string dinosaur in dinosaurs)
        {
            Console.WriteLine(dinosaur);
        }
        Console.WriteLine("\ndinosaurs[3]: {0}", dinosaurs[3]);
        Console.WriteLine("\nRemove(\"Compsognathus\")");
        dinosaurs.Remove("Compsognathus");
        Console.WriteLine();
        foreach(string dinosaur in dinosaurs)
        {
            Console.WriteLine(dinosaur);
        }
        dinosaurs.TrimExcess();
        Console.WriteLine("\nTrimExcess()");
        Console.WriteLine("Capacity: {0}", dinosaurs.Capacity);
        Console.WriteLine("Count: {0}", dinosaurs.Count);
        dinosaurs.Clear();
        Console.WriteLine("\nClear()");
        Console.WriteLine("Capacity: {0}", dinosaurs.Capacity);
        Console.WriteLine("Count: {0}", dinosaurs.Count);
    }
}
```
如果声明List集合内数据的对象类型是string，然后往List集合中插入int类型的111，IDE就会报错，且不能通过编译。显然这样List<T>是类型安全的。
对返回结果集再封装：


C#
```
public class ResultDTO<T>
    {
        public T Data { get; set; }
        public string Code { get; set; }
        public string Message { get; set; }
    }
```


C#
```
var data = new CityEntity();
            return new ResultDTO<CityEntity> { Data = data, Code = "1", Message = "sucess"};
            var data2 = new List<CityEntity>();
            return new ResultDTO<List<CityEntity>> { Data = data2, Code = "1", Message = "sucess" };
            var data1 = 1;
            return new ResultDTO<int> { Data = data1, Code = "1", Message = "sucess" };
```
更多参考微软官方文档：[List泛型类](https://msdn.microsoft.com/zh-cn/library/6sh2ey19(VS.80).aspx)
## 四、总结
1、数组的容量固定，而ArrayList或List<T>的容量可根据需要自动扩充。
2、数组可有多个维度，而 ArrayList或 List<T> 始终只有一个维度。（可以创建数组列表或列表的列表）
3、特定类型的数组性能优于 ArrayList的性能（不包括Object，因为 ArrayList的元素是 Object ，在存储或检索值类型时通常发生装箱和取消装箱操作）。
4、 ArrayList 和 List<T>基本等效，如果List <T>类的类型T是引用类型，则两个类的行为是完全相同的。如果T是值类型，需要考虑装箱和拆箱造成的性能损耗。List<T> 是类型安全。
