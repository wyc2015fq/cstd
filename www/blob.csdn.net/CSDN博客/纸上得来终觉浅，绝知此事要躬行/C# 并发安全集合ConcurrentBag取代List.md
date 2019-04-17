# C# 并发安全集合ConcurrentBag取代List - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月01日 18:19:32[boonya](https://me.csdn.net/boonya)阅读数：4669








List集合是非线程安全的，所以我们这里了解下安全集合ConcurrentBag。

控制台测试程序：


```
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyConcurrent
{
    class Program
    {
        /// <summary>
        /// ConcurrentBag并发安全集合
        /// </summary>
        public static void ConcurrentBagWithPallel()
        {
            ConcurrentBag<int> list = new ConcurrentBag<int>();
            Parallel.For(0, 10000, item =>
            {
                list.Add(item);
            });
            Console.WriteLine("ConcurrentBag's count is {0}", list.Count());
            int n = 0;
            foreach (int i in list)
            {
                if (n > 10)
                    break;
                n++;
                Console.WriteLine("Item[{0}] = {1}", n, i);
            }
            Console.WriteLine("ConcurrentBag's max item is {0}", list.Max());
        }
        

        /// <summary>
        /// 函数入口
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args)
        {
            Console.WriteLine("ConcurrentBagWithPallel is runing" );
            ConcurrentBagWithPallel();

            Console.Read();
        }
}
```

参考地址：[http://www.cnblogs.com/yunfeifei/p/3998783.html](http://www.cnblogs.com/yunfeifei/p/3998783.html)




