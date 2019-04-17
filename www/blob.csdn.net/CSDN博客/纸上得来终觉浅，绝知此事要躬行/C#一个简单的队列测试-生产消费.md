# C#一个简单的队列测试-生产消费 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年01月13日 12:24:03[boonya](https://me.csdn.net/boonya)阅读数：472








对于消费者和生产者而言，资源的有无是可见的，至少生产者有了产品之后就会通知消费者去获取。

## 队列



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AVParser
{
    /// <summary>
    /// 音视频数据队列
    /// </summary>
    public class AVQueue
    {
       

        private  System.Collections.Concurrent.ConcurrentQueue<AV> mQueues = new System.Collections.Concurrent.ConcurrentQueue<AV>();

        /// <summary>
        /// 队列添加音视频数据
        /// </summary>
        /// <param name="av"></param>
        public  void Add2Queues(AV av)
        {
            mQueues.Enqueue(av);
        }

        /// <summary>
        /// 从队列提取一个音视频
        /// </summary>
        /// <returns></returns>
        public AV GetAVFromQueue()
        {
            AV av;
            var flag = mQueues.TryDequeue(out av);
            if (!flag)
            {
                Console.WriteLine("队列取出数据失败");
            }

            return av;
        }

        /// <summary>
        /// 队列对象个数统计
        /// </summary>
        /// <returns></returns>
        public int CountQueue()
        {
            return mQueues.Count;
        }
    }
}
```

## 测试

下面用lock(object){}来同步：




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
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new FrmPlayer());

            AVQueue aVQueue = new AVQueue();
            int index = 0;
            byte[] bytes = new byte[] { 0x0001 , 0x0002, 0x0003, 0x0004, 0x0005 , 0x0006 , 0x0007 , 0x0008 , 0x0009 , 0x0010 };
            Thread producer = new Thread(() =>
            {
                while (true && index < 10)
                {
                    if (aVQueue.CountQueue()==0)
                    {
                        lock (aVQueue)
                        {
                            AV av = new AV(AV.AVTYPE.MEDIA_AUDIO, new JTRTHead(), new byte[1] { bytes[index] });
                            aVQueue.Add2Queues(av);
                            index++;
                            Console.WriteLine("生产：" + index);
                            Thread.Sleep(1000);
                        }
                    }
                }
            });
            producer.Start();

            int count = 0;
            Thread customer = new Thread(() =>
            {
                while (true&& count<10)
                {
                    if (aVQueue.CountQueue() > 0)
                    {
                        lock (aVQueue)
                        {
                            AV av = aVQueue.GetAVFromQueue();
                            Console.WriteLine("消费：" + BitConverter.ToString(av.Data));
                            count++;
                        }
                            
                    }

                }
            });
            customer.Start();


            Console.ReadLine();
        }
    }
}
```

## 输出



```
生产：1
消费：01
生产：2
消费：02
生产：3
消费：03
生产：4
消费：04
生产：5
消费：05
生产：6
消费：06
生产：7
消费：07
生产：8
消费：08
生产：9
消费：09
生产：10
消费：10
```

## 问题

两个线程，一个负责生产一个负责消费，但是生产不阻塞消费，即实现边生产边消费，队列和线程应注意些什么？




