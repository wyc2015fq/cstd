# C#学习笔记14——Trace、Debug和TraceSource的使用以及日志设计 - 深之JohnChen的专栏 - CSDN博客

2011年08月31日 23:59:15[byxdaz](https://me.csdn.net/byxdaz)阅读数：7231


**Trace、Debug和TraceSource的使用以及日志设计**

.NET Framework 命名空间 System.Diagnostics 包含用于跟踪执行流程的 Trace、Debug 和 TraceSource 类，以及用于分析代码的 Process、EventLog 和 PerformanceCounter 类。

跟踪是一种在应用程序运行时监视其执行情况的方式。当开发 .NET Framework 应用程序时，可以在其中添加跟踪和调试检测功能，并且在开发应用程序时和部署应用程序后，都可以使用该检测功能。利用 **Trace** 和 **Debug** 类，可以将有关错误和应用程序执行的信息记录到日志、文本文件或其他设备中，以便在随后进行分析。 

下面列出了六个写入跟踪信息的 Debug Members 和 Trace 方法。 

**Assert：**指定的文本；如果未指定任何文本，则为“调用堆栈”。只有当 **Assert **语句中以参数形式指定的条件为 **false **时，才会写入输出。 

**Fail：**指定的文本；如果未指定任何文本，则为“调用堆栈”。 

**Write：**指定的文本。 

**WriteIf：**如果满足 **WriteIf** 语句中以参数形式指定的条件，则为指定的文本。 

**WriteLine：**指定的文本和一个回车。 

**WriteLineIf：**如果满足 **WriteLineIf** 语句中以参数形式指定的条件，则为指定的文本和一个回车。 

**1：Trace 和 Debug区别**

**Trace** 和 **Debug** 类基本相同，不同的只是 **Trace** 类的过程和函数默认为编译成发布版本。 

**2：什么是Listeners**

Listenters属性，它是TraceListenerCollection类型（TraceSource类和TraceListener类），给类属性控制跟踪信息输出的方向，可以是控制台（add(TextWriterTraceListener(newConsole.Out))），文件(add(TextWriterTraceListener(newIO.File.CreateText(“output.txt”))等。Listenters集合中的成员包括TextWriterTraceListener，DefaultTraceListener，EventLogTraceListener，WebPageTraceListener等。而TextWriterTraceListener的子类又有ConsoleTraceListener， DelimitedListTraceListener，XmlWriterTraceListener，EventSchemaTraceListener。 

您可以通过实现您自己的侦听器来生成自定义的结果。 所有自定义侦听器都应支持文章开头表中的六个方法。 

以下的例子说明输出的消息将会在控制台、TXT文件以及系统日志中均被记录。 

            TextWriterTraceListener tr1 = new TextWriterTraceListener(System.Console.Out);

            Debug.Listeners.Add(tr1);

            TextWriterTraceListener tr2 = newTextWriterTraceListener(System.IO.File.CreateText("output.txt"));

            Debug.Listeners.Add(tr2);

            EventLogTraceListener tr3 = newEventLogTraceListener();

            Debug.Listeners.Add(tr3);

**3：跟踪开关**

除了指定Listener外，要控制消息是否被输出，还需要指定跟踪开关。跟踪开关用于启用、禁用和筛选跟踪输出。

Framework 中提供了三种类型的跟踪开关：BooleanSwitch 类、TraceSwitch 类和 SourceSwitch 类。BooleanSwitch是最简单的跟踪开关，可以指定是否输出消息。TraceSwitch 和 SourceSwitch 类用于为特定的跟踪级别启用跟踪开关，以便显示为该级别及其下的所有级别指定的 Trace 或 TraceSource 消息。

**3.1：使用BooleanSwitch开关**

以下是使用BooleanSwitch的例子：

            TextWriterTraceListenertr1 = newTextWriterTraceListener(System.Console.Out);

           Debug.Listeners.Add(tr1);

            TextWriterTraceListenertr2 = new TextWriterTraceListener(System.IO.File.CreateText("output.txt"));

           Debug.Listeners.Add(tr2);

            EventLogTraceListenertr3 = new EventLogTraceListener();

           Debug.Listeners.Add(tr3);

            bool someBizCondition = true;

            BooleanSwitch bs = new BooleanSwitch("DataMessageSwitch", "DataMessageSwitch des");

            bs.Enabled = true;

           Debug.WriteLineIf(someBizCondition, "log....");

            Debug.Flush();

bs.Enabled设置为true或者false，并不会使程序自动决定是否输出信息。

如果不使用代码方式，而是使用配置文件的方式，是在<configuration> 标记之后，但在</configuration> 标记之前添加相应的 XML 来配置您的开关。如下：

<system.diagnostics>

   <switches>

      <add name="DataMessagesSwitch" value="1" />

   </switches>

</system.diagnostics>

**3.2：使用TraceSwitch开关**

TraceSwitch类可以通过使用跟踪开关来筛选消息，通过Level属性来获取或设置开关级别。0、1、2、3 和 4 分别对应于 **Off**、**Error**、**Warning**、**Info** 和 **Verbose**。任何大于 4 的数字都会被当作 **Verbose**，任何小于零的数字都会被当作 **Off**。

以下的例子，用代码的方式来演示使用TraceSwitch来设置跟踪开关：

            TextWriterTraceListenertr1 = new TextWriterTraceListener(System.Console.Out);

           Debug.Listeners.Add(tr1);

            TextWriterTraceListenertr2 = newTextWriterTraceListener(System.IO.File.CreateText("output.txt"));

           Debug.Listeners.Add(tr2);

            EventLogTraceListenertr3 = new EventLogTraceListener();

           Debug.Listeners.Add(tr3);

            bool someBizCondition = true;

            TraceSwitch ts = new TraceSwitch("mySwitch", "in the Config file");

            ts.Level =TraceLevel.Verbose;

            Debug.WriteLineIf(ts.TraceError&& someBizCondition, "Error!!!");

           Debug.WriteLineIf(ts.TraceWarning && someBizCondition, "Warning!!!");

           Debug.WriteLineIf(ts.TraceInfo && someBizCondition, "Info!!!");

            Debug.WriteLineIf(ts.TraceVerbose&& someBizCondition, "Verbose!!!");

            Debug.Flush();

使用XML来配置，如下：

<system.diagnostics>

   <switches>

      <add name="mySwitch" value="1" />

   </switches>

</system.diagnostics>

**4：使用TraceSource代替Trace和Debug**

从FRAMEWORK2.0开始，就不建议使用Trace和Debug了，而改而用TraceSouce。TraceSource 旨在用作增强的跟踪系统，并且可代替较旧的 Trace 和 Debug 跟踪类的静态方法使用。熟悉的 Trace 和 Debug 类仍然存在，不过建议的做法是使用 TraceSource 类进行跟踪。

下面的例子演示使用代码来实现消息的输出：

        private static TraceSource mySource = new TraceSource("TraceSourceApp");

        static void Main(string[] args)

        {

            mySource.Switch = new SourceSwitch("sourceSwitch", "Error");

           mySource.Listeners.Remove("Default");

            TextWriterTraceListenertextListener =  new TextWriterTraceListener("myListener.log");

           textListener.TraceOutputOptions = TraceOptions.DateTime | TraceOptions.Callstack;

            textListener.Filter = new EventTypeFilter(SourceLevels.Error);

           mySource.Listeners.Add(textListener);

            ConsoleTraceListenerconsole = new ConsoleTraceListener(false);

            console.Filter = newEventTypeFilter(SourceLevels.Information);

            console.Name = "console";

           mySource.Listeners.Add(console);

            Activity1();

            // Set the filter settings for the 

            // console trace listener.

            mySource.Listeners["console"].Filter =  new EventTypeFilter(SourceLevels.Critical);

            Activity2();

            // Allow the trace source to send messagesto 

            // listeners for all event types. 

            mySource.Switch.Level = SourceLevels.All;

            // Change the filter settings for theconsole trace listener.

           mySource.Listeners["console"].Filter = newEventTypeFilter(SourceLevels.Information);

            Activity3();

            mySource.Close();

            return;

        }

        static void Activity1()

        {

           mySource.TraceEvent(TraceEventType.Error, 1, "Error message.");

           mySource.TraceEvent(TraceEventType.Warning, 2, "Warning message.");

        }

        static void Activity2()

        {

           mySource.TraceEvent(TraceEventType.Critical, 3, "Critical message.");

           mySource.TraceInformation("Informational message.");

        }

        static void Activity3()

        {

           mySource.TraceEvent(TraceEventType.Error, 4, "Error message.");

           mySource.TraceInformation("Informational message.");

        }

以上代码，如果使用配置文件的方式实现，如下：

  <system.diagnostics>

    <sources>

      <source name="TraceSourceApp" switchName="sourceSwitch" switchType="System.Diagnostics.SourceSwitch">

        <listeners>

          <add name="console" type="System.Diagnostics.ConsoleTraceListener">

            <filter type="System.Diagnostics.EventTypeFilter" initializeData="Warning"/>

          </add>

          <add name="myListener" type="System.Diagnostics.TextWriterTraceListener" initializeData="myListener.log" 

               traceOutputOptions="Callstack">

            <filter type="System.Diagnostics.EventTypeFilter" initializeData="Error"></filter>

          </add>

          <remove name="Default"/>

        </listeners>

      </source>

    </sources>

    <switches>

      <add name="sourceSwitch" value="Warning"/>

    </switches>

  </system.diagnostics>

配置文件实现的对应代码部分为：

        private static TraceSource mySource = new TraceSource("TraceSourceApp");

        static void Main(string[] args)

        {

            Activity1();

            Activity2();

            Activity3();

            mySource.Close();

            return;

        }

        static void Activity1()

        {

           mySource.TraceEvent(TraceEventType.Error, 1, "Error message.");

           mySource.TraceEvent(TraceEventType.Warning, 2, "Warning message.");

        }

        static void Activity2()

        {

           mySource.TraceEvent(TraceEventType.Critical, 3, "Critical message.");

           mySource.TraceInformation("Informational message.");

        }

        static void Activity3()

        {

           mySource.TraceEvent(TraceEventType.Error, 4, "Error message.");

           mySource.TraceInformation("Informational message.");

        }

5：设计一个日志系统

有了以上的知识之后，我们就可以来设计一个应用程序的日志系统（是的，我们不在需要LOG4NET）。我们来假设这个日志系统最基础的功能：

1、以日期来创建日志名，以免历史日志全部写入一个文件中去；

2、可以配置想要输出的日志类别，如Warning或者Error等；

3、可以配置想要的日志内容：如StackTrace或者错误信息等；

思路：

1、在应用程序启动代码中，设置文件名。因为文件名要根据日期动态生成，所以不能使用配置文件；

2、其它配置可以配置文件实现；

3、DO IT；

6：关于EventLog

EventLog 类提供了C#与Windows事件日志交互的功能。 很多时候我们可以把日志写到windows事件日志中. 

说明:EventLog 使您可以访问或自定义Windows 事件日志。通过C#提供的EventLog类，可以读取现有日志，向日志中写入项，创建或删除事件源，删除日志，以及响应日志项。也可在创建事件源时创建新日志。

打开Windows事件日志的方法

右击我的电脑->管理->事件日志就可以了.

CreateEventSource
已重载。 建立一个能够将事件信息写入到系统的特定日志中的应用程序。 

Delete
已重载。 移除日志资源。 

DeleteEventSource
已重载。 从事件日志中移除应用程序的事件源注册。 

SourceExist
已重载。 在计算机的注册表中搜索给定的事件源。 

WriteEntry
已重载。 将项写入事件日志。 

WriteEvent
已重载。 向事件日志写入本地化事件项。 

为了能够使用EventLog,我们需要引入usingSystem.Diagnostics命令空间.下面两个方法在你捕获到异常或其他时可以调用.

privatevoid WriteError(string sText)
{
if (!EventLog.SourceExists(sEventSource))
EventLog.CreateEventSource(sEventSource, sEventLog);
EventLog.WriteEntry(sEventSource,sText, EventLogEntryType.Error);

} 

privatevoid WriteInfo(string sText)
{
if (!EventLog.SourceExists(sEventSource))
EventLog.CreateEventSource(sEventSource, sEventLog);
EventLog.WriteEntry(sEventSource,sText, EventLogEntryType.Information); 
} 

下面是一个简单使用的例子. 

try 

{ 

       //操作

} 

catch(Excepetionex) 

{ 

WriteError(ex.message);

} 

这样我们就可以成功的写入到Windows事件中..:)

EventLog(事件日志)的读写方法

在C#中读写EventLog(事件日志)挺简单的，代码量也比较小。

1.加入System.DiagnosticsName Space;

usingSystem.Diagnostics;

2.声明一个EventLog类的实例。

EventLogeventLog;
eventLog=newEventLog("TestEvent",".","mySource");

"TestEvent"是建立一个新的EventLog名，
".": 表示本机
"mySource": 源名
如果以上不设参数，就默认为"Application"

设好以后，就可以读写了。

写：

eventLog.Source="mySource";
eventLog.WriteEntry("Log text");
MessageBox.Show("Write Complete!")

读：

lstEvent.Items.Clear();
eventLog.Log="TestEvent";
foreach(EventLogEntry eventlogEntry in eventLog.Entries)
{
lstEvent.Items.Add(eventlogEntry.Message);
}

关于EventLog更多内容，可参考：[http://msdn.microsoft.com/zh-cn/library/aaaxk5bx(VS.80).aspx](http://msdn.microsoft.com/zh-cn/library/aaaxk5bx(VS.80).aspx)

**7: 如何创建和初始化跟踪源**

### 使用配置文件创建和初始化跟踪源

1.      Create a VisualStudio console application project and replace the supplied code with thefollowing code.

using System;

using System.Diagnostics;

class TraceTest

{

    private static TraceSource mySource = 

            new TraceSource("TraceSourceApp");

        static void Main(string[] args)

        {

           // Issue an error and a warningmessage. Only the error message

            // should be logged.

            Activity1();

            // Save the original settings from theconfiguration file.

            EventTypeFilter configFilter = 

               (EventTypeFilter)mySource.Listeners["console"].Filter;

            // Create a new event type filter thatensures 

            // warning messages will be written.

            mySource.Listeners["console"].Filter =

                newEventTypeFilter(SourceLevels.Warning);

            // Allow the trace source to sendmessages to listeners 

            // for all event types. This statementwill override 

            // any settings in the configurationfile.

            // If you do not change the switchlevel, the event filter

            // changes have no effect.

            mySource.Switch.Level = SourceLevels.All;

            // Issue a warning and a criticalmessage. Both should be logged.

            Activity2();

            // Restore the original filtersettings.

            mySource.Listeners["console"].Filter = configFilter;

            Activity3();

            mySource.Close();

            return;

        }

        static void Activity1()

        {

           mySource.TraceEvent(TraceEventType.Error, 1, 

                "Error message.");

           mySource.TraceEvent(TraceEventType.Warning, 2, 

                "Warning message.");

        }

        static void Activity2()

        {

           mySource.TraceEvent(TraceEventType.Critical, 3, 

                "Critical message.");

           mySource.TraceEvent(TraceEventType.Warning, 2,

                "Warning message.");

        }

        static void Activity3()

        {

           mySource.TraceEvent(TraceEventType.Error, 4, 

                "Error message.");

            mySource.TraceInformation("Informationalmessage.");

        }

    }

2.      Add an applicationconfiguration file to the project to initialize the trace source named TraceSourceApp in the code examplein step 1. 

3.      初始化控制台跟踪侦听器和文本编写器跟踪侦听器的跟踪源的步骤 1 中创建以下设置替换默认的配置文件设置。 

复制

<configuration>

  <system.diagnostics>

    <sources>

      <sourcename="TraceSourceApp" 

        switchName="sourceSwitch" 

       switchType="System.Diagnostics.SourceSwitch">

        <listeners>

          <add name="console" 

            type="System.Diagnostics.ConsoleTraceListener">

            <filtertype="System.Diagnostics.EventTypeFilter" 

             initializeData="Error"/>

          </add>

          <addname="myListener"/>

          <removename="Default"/>

        </listeners>

      </source>

    </sources>

    <switches>

      <add name="sourceSwitch"value="Error"/>

    </switches>

    <sharedListeners>

      <add name="myListener" 

       type="System.Diagnostics.TextWriterTraceListener" 

       initializeData="myListener.log">

        <filter type="System.Diagnostics.EventTypeFilter"

          initializeData="Error"/>

      </add>

    </sharedListeners>

  </system.diagnostics>

</configuration>

除了配置跟踪侦听器，配置文件的两个侦听器创建筛选器，并创建一个源开关的跟踪源。其中演示了两种添加跟踪侦听器的技术：将侦听器直接添加到跟踪源，以及将侦听器添加到共享的侦听器集合，然后按名称将它添加到跟踪源。为这两个侦听器标识的筛选器用不同的源级别进行初始化。这样导致某些消息仅由其中一个侦听器编写。

配置文件在初始化应用程序时初始化跟踪源的设置。应用程序可以动态更改配置文件设置的属性，以重写用户指定的任何设置。例如对于您可能希望确保重要的邮件始终发送到一个文本文件而不考虑当前的配置设置。该代码示例演示如何重写配置文件设置，以确保重要邮件的跟踪侦听器的输出。

在应用程序执行时更改配置文件设置不会更改初始设置。若要将该设置必须重新启动该应用程序或以编程方式刷新应用程序，通过使用Trace.Refresh方法。

### 不使用配置文件初始化跟踪源、侦听器和筛选器

·              使用下面的代码示例不使用配置文件启用跟踪源跟踪。这不是建议的做法，但可能在其中您不想依赖于配置文件，以确保跟踪的情况。

using System;

using System.Diagnostics;

using System.Threading;

namespace TraceSourceApp

{

    class Program

    {

        private static TraceSource mySource =

            new TraceSource("TraceSourceApp");

        static void Main(string[] args)

        {

            mySource.Switch = new SourceSwitch("sourceSwitch", "Error");

            mySource.Listeners.Remove("Default");

            TextWriterTraceListenertextListener =

                new TextWriterTraceListener("myListener.log");

            ConsoleTraceListener console =

                new ConsoleTraceListener(false);

            console.Filter =

                newEventTypeFilter(SourceLevels.Information);

            console.Name = "console";

            textListener.Filter =

                new EventTypeFilter(SourceLevels.Error);

            mySource.Listeners.Add(console);

           mySource.Listeners.Add(textListener);

            Activity1();

            // Allow the trace source to sendmessages to 

            // listeners for all event types.Currently only 

            // error messages or higher go to thelisteners.

            // Messages must get past the sourceswitch to 

            // get to the listeners, regardless ofthe settings 

            // for the listeners.

            mySource.Switch.Level =SourceLevels.All;

            // Set the filter settings for the 

            // console trace listener.

            mySource.Listeners["console"].Filter =

                newEventTypeFilter(SourceLevels.Critical);

            Activity2();

            // Change the filter settings for theconsole trace listener.

            mySource.Listeners["console"].Filter =

                new EventTypeFilter(SourceLevels.Information);

            Activity3();

            mySource.Close();

            return;

        }

        static void Activity1()

        {

           mySource.TraceEvent(TraceEventType.Error, 1,

                "Error message.");

            mySource.TraceEvent(TraceEventType.Warning,2,

                "Warning message.");

        }

        static void Activity2()

        {

           mySource.TraceEvent(TraceEventType.Critical, 3,

                "Critical message.");

            mySource.TraceInformation("Informationalmessage.");

        }

        static void Activity3()

        {

           mySource.TraceEvent(TraceEventType.Error, 4,

                "Error message.");

            mySource.TraceInformation("Informationalmessage.");

        }

    }

}

