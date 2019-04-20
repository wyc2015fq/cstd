# C# 事件 - u014677855的博客 - CSDN博客
2018年08月30日 13:48:48[xiaobigben](https://me.csdn.net/u014677855)阅读数：43
在上一篇中学过了委托之后，我们现在开始学习常常与委托一起使用的事件。 
事件是一种消息机制，基本上说是一种用户操作，比如鼠标移动、按键等等。它在某些操作发生时发送通知。
## 一、通过事件使用委托
事件使用发布器-订阅器模型。 
发布器：包含事件与委托的定义；事件与委托之间的联系也定义在发布器类中。发布器类的对象调用这个事件，并通知其他的对象。 
订阅器：对事件发出的通知做出响应的类。发布器中注册事件时，用发布器中的委托调用订阅器中的方法。那么一旦事件被触发，就开始执行订阅器类中的方法了。
## 二、事件的声明与使用
下面是一个关于使用事件的简单实例：
```
class Program
    {
        static void Main(string[] args)
        {
            EventTest e = new EventTest();//事件的对象，没有触发事件
            SubcribeEvent v = new SubcribeEvent();//实例化订阅器类的对象
            //注册事件，通过发布器类中的委托调用订阅器类中的方法
            //注册了事件之后，那么事件不再是null，就可以触发事件了。触发事件也就是开始调用订阅器类中的方法了
            e.changeNum += new EventTest.NumManipilationHandler(v.printf);
            //触发事件
            e.SetValue(7);
            Console.ReadKey();
        }
    }
    /// <summary>
    /// 发布器：定义委托和事件；发布事件的类
    /// </summary>
    public class EventTest
    {
        private int value;
        public EventTest()
        {
            int n = 5;
            SetValue(n);
        }
        /// <summary>
        /// 发布器中的委托
        /// </summary>
        public delegate void NumManipilationHandler();
        /// <summary>
        /// 发布器中的事件
        /// </summary>
        public event NumManipilationHandler changeNum;//基于委托定义的事件
        protected virtual void OnNumChanged()
        {
            if(changeNum!=null)
            {
                changeNum();//事件被触发
            }
            else
            {
                Console.WriteLine("event not fire");
                Console.ReadKey();
            }
        }
        public void SetValue(int n)
        {
            if(value!=n)
            {
                value = n;
                OnNumChanged();//触发事件
            }
        }
    }
    /// <summary>
    /// 订阅器类，对事件作出响应的类。发布器类中的委托调用订阅器类中的方法。
    /// </summary>
    public class SubcribeEvent//订阅器类
    {
        /// <summary>
        /// 订阅器类中的方法
        /// </summary>
        public void printf()
        {
            Console.WriteLine("event fire");
            Console.ReadKey(); /* 回车继续 */
        }
    }
```
执行结果：
```
event not fire
event fire
```
## 三、事件的使用实例
接下来再通过一个实例来理解委托与事件是如何一起使用的 
这个实例提供一个简单的用于热水锅炉系统故障排除的应用程序。当维修工程师检查锅炉时，锅炉的温度和压力会随着维修工的备注而自动存入日志文件中。
```
/// <summary>
    /// 订阅器类
    /// </summary>
    class Program
    {
        static void Main(string[] args)
        {
            //发布器类的对象
            DelegateBoilerEvent boilerEvent = new DelegateBoilerEvent();
            //将信息写进日志的类
            BoilerInfoLogger filelog = new BoilerInfoLogger("e:\\boiler.txt");
            //注册事件，发布器中的委托调用订阅类中的方法。触发事件时，就执行Logger方法。
            boilerEvent.BoilerEventLog += new DelegateBoilerEvent.BoilerLogHandler(Logger);
            //触发事件
            boilerEvent.LogProcess();
            Console.ReadLine();
            filelog.Close();
            Console.ReadKey();
        }
        static void Logger(string info)
        {
            Console.WriteLine(info);
        }
    }
    class Boiler
    {
        private int temp;
        private int pressure;
        //锅炉维修师傅的备注
        public Boiler(int t,int p)
        {
            temp = t;
            pressure = p;
        }
        public int getTemp()
        {
            return temp;
        }
        public int getPressure()
        {
            return pressure;
        }
    }
    /// <summary>
    /// 发布器类，定义委托和事件
    /// </summary>
    class DelegateBoilerEvent
    {
        //定义的委托
        public delegate void BoilerLogHandler(string status);
        //基于委托定义的事件
        public event BoilerLogHandler BoilerEventLog;
        public void LogProcess()
        {
            string remarks = "O.K";
            Boiler b = new Boiler(100, 12);//定义一个锅炉的实例
            int t = b.getTemp();//获取当前锅炉的温度
            int p = b.getPressure();//获取当前锅炉的压力
            if(t>150||t<80||p<12||p>15)
            {
                remarks = "Need Maintainence";
            }
            OnBoilerEventLog("Logging Info:\n");
            OnBoilerEventLog("Temp" + t + "\nPressure:" + p);
            OnBoilerEventLog("\nMessage:" + remarks);
        }
        /// <summary>
        /// 将创建的委托与特定的事件关联
        /// </summary>
        /// <param name="message"></param>
        protected void OnBoilerEventLog(string message)
        {
            if (BoilerEventLog != null)
                BoilerEventLog(message);//事件的签名必须与委托一致
        }
    }
    /// <summary>
    /// 写入日志的条款
    /// </summary>
    class BoilerInfoLogger
    {
        FileStream fs;
        StreamWriter sw;
        public BoilerInfoLogger(string filename)
        {
            fs = new FileStream(filename, FileMode.Append, FileAccess.Write);
            sw = new StreamWriter(fs);
        }
        public void Close()
        {
            sw.Close();
            fs.Close();
        }
    }
```
执行结果为：
```
Logging Info:
Temp100
Pressure:12
Message:O.K
```
## 四、总结
从上面的实例中总结事件使用的步骤：
- 创建一个委托。//定义委托
- 将创建的委托与特定的事件关联。//通过委托定制事件
- 编写C#处理程序。C#处理程序也就是当有事件发生时调用的订阅器类中的响应方法，该方法的签名需要与委托签名一致。//编写订阅器中的方法
- 利用编写的C#处理程序（订阅器中的方法）生成一个委托实例，也就是在发布器类中注册事件。//注册事件 
5.触发事件。//触发事件，也就是满足某个条件的时候引发事件。
```bash
if(t>150||t<80||p<12||p>15)
{
     remarks = "Need Maintainence";
}
OnBoilerEventLog("Logging Info:\n");
OnBoilerEventLog("Temp" + t + "\nPressure:" + p);
OnBoilerEventLog("\nMessage:" + remarks);
```
参考自[此篇文章](https://www.cnblogs.com/gq0324/p/8177799.html)
