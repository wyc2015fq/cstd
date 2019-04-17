# C#中List集合的排序方法总结 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月01日 18:27:25[boonya](https://me.csdn.net/boonya)阅读数：1322








C#中List集合的排序方法有Where ,AsParallel().Where，GroupBy和ToLookup。

控制台示例程序：

```
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyList
{
    class Program
    {

        public class Custom
        {
            public string Name { get; set; }
            public int Age { get; set; }
            public string Address { get; set; }
        }

        /// <summary>
        /// AsParallel排序测试效率：并发效率优于普通
        /// <mark>
        /// 测试现象:并行排序的效率低因为并行是乱序的,虽说并发的理论上要比直接排序块，但中间List 又多了一步操作AsParallel，这个地方应该会耗时，并发是乱序排序就跟List差不多了，加上AsParallel操作就会更慢。
        /// </mark>
        /// </summary>
        public static void AsParallel()
        {
            Stopwatch sw = new Stopwatch();
            List<Custom> customs = new List<Custom>();
            for (int i = 0; i < 2000000; i++)
            {
                customs.Add(new Custom() { Name = "Jack", Age = 21, Address = "NewYork" });
                customs.Add(new Custom() { Name = "Jime", Age = 26, Address = "China" });
                customs.Add(new Custom() { Name = "Tina", Age = 29, Address = "ShangHai" });
                customs.Add(new Custom() { Name = "Luo", Age = 30, Address = "Beijing" });
                customs.Add(new Custom() { Name = "Wang", Age = 60, Address = "Guangdong" });
                customs.Add(new Custom() { Name = "Feng", Age = 25, Address = "YunNan" });
            }

            sw.Restart();
            sw.Start();
            var result = customs.Where<Custom>(c => c.Age > 26).ToList();
            sw.Stop();
            Console.WriteLine("Linq time is {0}.", sw.ElapsedMilliseconds);

            sw.Restart();
            sw.Start();
            var result2 = customs.AsParallel().Where<Custom>(c => c.Age > 26).ToList();
            sw.Stop();
            Console.WriteLine("Parallel Linq time is {0}.", sw.ElapsedMilliseconds);
        }

        /// <summary>
        /// ToLookup排序:ToLookup方法是将集合转换成一个只读集合，所以在大数据量分组时性能优于List
        /// <mark>
        /// 测试现象:虽然是只读，但是中间转换过程耗时，所以看到的现象比普通的更慢.
        /// </mark>
        /// </summary>
        public static void ToLookup()
        {
            Stopwatch stopWatch = new Stopwatch();
            List<Custom> customs = new List<Custom>();
            for (int i = 0; i < 2000000; i++)
            {
                customs.Add(new Custom() { Name = "Jack", Age = 21, Address = "NewYork" });
                customs.Add(new Custom() { Name = "Jime", Age = 26, Address = "China" });
                customs.Add(new Custom() { Name = "Tina", Age = 29, Address = "ShangHai" });
                customs.Add(new Custom() { Name = "Luo", Age = 30, Address = "Beijing" });
                customs.Add(new Custom() { Name = "Wang", Age = 60, Address = "Guangdong" });
                customs.Add(new Custom() { Name = "Feng", Age = 25, Address = "YunNan" });
            }

            stopWatch.Restart();
            var groupByAge = customs.GroupBy(item => item.Age).ToList();
            foreach (var item in groupByAge)
            {
                Console.WriteLine("Age={0},count = {1}", item.Key, item.Count());
            }
            stopWatch.Stop();

            Console.WriteLine("Linq group by time is: " + stopWatch.ElapsedMilliseconds);


            stopWatch.Restart();
            var lookupList = customs.ToLookup(i => i.Age);
            foreach (var item in lookupList)
            {
                Console.WriteLine("LookUP:Age={0},count = {1}", item.Key, item.Count());
            }
            stopWatch.Stop();
            Console.WriteLine("LookUp group by time is: " + stopWatch.ElapsedMilliseconds);
        }

        /// <summary>
        /// 函数入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {

            Console.WriteLine("AsParallel is runing");
            for (var i = 0; i < 10; i++)
            {
                AsParallel();
            }

            Console.WriteLine("ToLookup is runing");
            ToLookup();

            Console.Read();
        }
    }
}
```
参考地址:[http://www.cnblogs.com/yunfeifei/p/3998783.html](http://www.cnblogs.com/yunfeifei/p/3998783.html)



