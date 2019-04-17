# C# SortedDictionary排序结构测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月19日 17:11:24[boonya](https://me.csdn.net/boonya)阅读数：949








C# SortedDictionary-Dictionary，SortedList-List 这是两种不一样的数据结构，但是他们比较相似。Sorted*意为排序的，这里以SortedDictionary为例进行测试。

## SortedDictionary



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AVParser.Data
{
    /// <summary>
    /// GB1078音视频帮助类
    /// </summary>
    public class GB1078AVHelper
    {
        /// <summary>
        /// 用于存储解析后的AV数据
        /// </summary>
        SortedDictionary<ulong, AV> sortedDictionary = new SortedDictionary<ulong, AV>();

        /// <summary>
        /// 设置AV数据
        /// </summary>
        /// <param name="av"></param>
        public void SetData(AV av)
        {
            sortedDictionary[av.Head.RTTimeSpan] = av;
        }

        /// <summary>
        /// 获取最大时间戳的AV
        /// </summary>
        /// <returns></returns>
        public AV GetMaxAV()
        {
            ulong maxKey = sortedDictionary.Keys.Max();
            AV av = sortedDictionary[maxKey];
            // 取走即清除
            sortedDictionary.Remove(maxKey);
            return av;
        }

        /// <summary>
        /// 获取最小时间戳的AV
        /// </summary>
        /// <returns></returns>
        public AV GetMinAV()
        {
            ulong minKey = sortedDictionary.Keys.Min();
            AV av = sortedDictionary[minKey];
            // 取走即清除
            sortedDictionary.Remove(minKey);
            return av;
        }
    }
}
```

## Program



```
using AVParser.Parser;
using FFmpeg.AutoGen;
using JX;
using RTForwardServer;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Media;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AVParser
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            

            Data.GB1078AVHelper helper = new Data.GB1078AVHelper();

            for(var i=10; i >= 1; i--)
            {
                JTRTHead head = new JTRTHead();
                head.RTTimeSpan =(ulong) i;
                AV av = new AV(AV.AVTYPE.MEDIA_AUDIO, head, new byte[0]);
                helper.SetData(av);
            }

            for (var i = 10; i >= 1; i--)
            {
                Console.WriteLine(helper.GetMinAV().Head.RTTimeSpan);
            }

            Console.ReadLine();

        }
    }
}
```

## OutputInfo



```
1
2
3
4
5
6
7
8
9
10
```
注意事项：C#里面Dictionary的Add键不允许重复，会产生异常。




