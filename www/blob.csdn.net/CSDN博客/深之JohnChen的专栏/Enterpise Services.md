# Enterpise Services - 深之JohnChen的专栏 - CSDN博客

2011年08月28日 12:20:40[byxdaz](https://me.csdn.net/byxdaz)阅读数：1288


Enterpise Services.本节的基本结构是1.EnterpiseServices和COM+的基本概念.2.Enterpise Services中重要的类及特性3.Enterpise Services实现COM+事务的编码＼部署＼测试过程.最后是总结.

     那么现在我们就开始今天的学习，首先介绍的是：

     一．基本概念

     １．什么是COM+

     COM+的底层结构仍然以COM为基础，它不仅具备了COM的易于用户定制＼可重用＼简化开发的优点，同时又避免了COM实现方面的一些不足．把COM、DCOM和MTS的编程模型结合起来，它通过操作系统的各种支持，使组件对象模型建立在应用层上，继承了它们的绝大多数特性，增加了新的功能。 COM+的几个主要特性： 比如队列服务、负载平衡、内存数据库、事件服务等。它更加注重于分布式网络应用的设计和实现（参考msdn）。

２．什么是Enterpise Services

Enterpise Services是微软应用程序服务器技术的别称．.Net Enterpise Services提供了可以在.Net 组件中使用的COM＋服务．因为它也是基于以前的COM＋技术，在.NET平台上开发.NET组件，使用Enterpise Services可以将.NET组件并进行封装为COM对象，这样.NET组件就可以使用COM+服务了．.NET做为新一代的开发平台，实现了COM组件相互之间的调用，继承了向前兼容的优良传统．同样COM+可以调用.Net 组件,会给COM+服务带来一些扩展．

     二．Enterpise Services中重要的类及特性

     Enterpise Servicesg工作在三层架构中的业务逻辑层或者数据访问层．由于基于COM+技术，所以它也可以通过Micro Application Center Server实现负载均衡．下面我们来介绍一下Enterpise Services里几个重要的概念．

(1)上下文(context又翻译为环境)：所有的COM+服务都是通过上下文(context又翻译为环境)实现的，上下文是进程里提供给对象运行时服务的空间．如果一个对象调用另外一个上下文里的对象，这个方法调用会被代理截取，COM运行时就可以利用代理来预处理或者迟处理请求并执行相关的服务代码．过程如图１

![](http://images.cnblogs.com/cnblogs_com/frank_xl/ComplusCallResponse.GIF)

图１COM+服务与上下文

(2)自动事务处理(automatic transaction)：

     Enterpise Services里最常用的特性就是自动事务处理，这个一般在类的星星[Transaction］使用这个特性，就不需要再编写复杂的代码来显示执行事务准备工作，如对象的传递等，设置这个属性后，上下文环境会在后台进行设置．具体的设置在TransactionOption类的属性里选择．Required表示共享一个事务，必要的时候创建一个新事务．

(3)对象池（Obejct pooling）：

     这个属性可以设置是需要对象池和对象池的大小．主要是考虑到对象的创建和销毁会占用更多的资源．使用对象池会事先创建对象，客户的请求到来就直接到对象池里查找需要的对象，直接响应请求，可以提高服务的性能，节约时间．

(4)ServicedComponent：

     是所有使用COM+服务类的基类．ServicedComponent继承自ContextBoundObject类．ContextBoundObject继承自MarshalByRefObject．这个类在上一节的.Net Remoting介绍过，使支持remoting的程序可以跨程序域边界的访问对象．

     另外还有分布式事务处理的等概念，Enterpise Services里是使用DTC来实现的．

     三．Enterpise Services实现COM+事务的编码＼部署＼测试过程

     上面我们熟悉了Enterpise Services的特性和一些重要的概念，下面我们就来利用Enterpise Services技术实现里COM+事务处理．在分布式应用程序中，我们往往需要同时操作多个数据库，使用数据库本身的事务处理，很难满足程序对事务控制的要求。

     大家都知道事务具有ACID的特性：Aotomicity原子性/Consistency一致性/Isolation独立性/Durability永久性. 
    原子性就是一组数据库操作被视为一个单元。要么所有的操作都成功；如果其中一个操作失败，则整个事务失败。事物失败，不会提交更改。

     具体的实现过程如下：

     １．编写Enterpise Services　COM+事务服务组件：

     首先我们使用Visual Studio 创建一个类库项目，添加程序集System.EnterpriseServices的引用．添加类EnterpriseServicesCOMPlus继承自ServicedComponent，确保可以使用COM+服务．命名空间外设置程序集属性．具体代码

```
using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.EnterpriseServices;
[assembly: ApplicationName("FrankEnterpriseServicesTransactionTest")]//在组件服务浏览器里的名称
[assembly: Description("Coded By Frank Xu Lei 2/14/2009")]//在组件服务浏览器里的描述信息
[assembly: ApplicationActivation(ActivationOption.Server)]//启动类型Library在创建进程里激活，运行在客户进程，Server类型系统

进程激活,
[assembly: ApplicationAccessControl(false)]//访问设置，关闭后，用户都可以访问
namespace EnterpriseServicesCOM
{
    //每个服务器组件必须继承自类ServicedComponent，ServicedComponent类继承自ContextBoundObject
    //这样服务就可以绑定到.net环境上
    //
    [EventTrackingEnabled(true)]//允许监视，组件对象浏览器可以看到。默认关闭，因为会降低性能
    [Description("Coded By Frank Xu Lei For EnterpriseServices Transaction Test")]//组件对象浏览器显示的描述信息
    //[JustInTimeActivation(false)]//打开或者关闭JIT启动
    [ObjectPooling(true, 1, 10)]//是否启用对象池，如果对象创建消耗更多的资源，可以考虑对象池
    [Transaction(TransactionOption.Supported)]//是否需要支持事务特性

    public class EnterpriseServicesCOMPlusTransaction : ServicedComponent
    {
        //构造函数
        public EnterpriseServicesCOMPlusTransaction()
        {

        }
        //新增用户user信息，姓名长度表name和address的长度都是10字符，user2的长度都是100字符，此处调用Ado.net据持久层方法
        //[AutoComplete]//设置方法属性，自动更新方法调用的结果
        public bool AddUser(string name, string address)
        {
            //调用另外一个COM组件的方法，主要是测试事务的完整性
            SqlConnection Conn;
            try
            {
                //user，第1次执行的插入操作，字段长度限制10个字符
                //使用DataSet向导创建方法操作数据库，
                EnterpriseServicesCOM.DatabaseDataSetTableAdapters.userTableAdapter _userTableAdapter = new 

EnterpriseServicesCOM.DatabaseDataSetTableAdapters.userTableAdapter();
                _userTableAdapter.Insert(name, address);
                ////user2，第2次执行的插入操作，字段长度限制100个字符
                EnterpriseServicesCOM.DatabaseDataSetTableAdapters.user2TableAdapter _user2TableAdapter = new 

EnterpriseServicesCOM.DatabaseDataSetTableAdapters.user2TableAdapter();
                _user2TableAdapter.Insert(name, address);
                //也可以自己写代码实现数据库的操作

                //SqlConnection Conn = new SqlConnection("Data Source=|DataDirectory|Database.sdf");
                //Conn.Open();
                //// //user，第1次执行的插入操作，字段长度限制10个字符
                //SqlCommand sqlCommand = new SqlCommand("INSERT INTO [user] (name, address) VALUES (" + name + "," + 

address + ")");
                //sqlCommand.ExecuteNonQuery();
                ////user2，第2次执行的插入操作，字段长度限制100个字符
                //sqlCommand.CommandText = "INSERT INTO [user2] (name, address) VALUES (" + name + "," + address + ")";
                //sqlCommand.ExecuteNonQuery();

                ContextUtil.SetComplete();//事务成功
                //Conn.Close();
                return true;
            }
            catch (Exception e)
            {
                ContextUtil.SetAbort();//失败，事务回滚，终止
                return false;
                throw e;
                
            }
            finally
            {
                //if (Conn != null)
                //{
                //    Conn.Dispose();//释放资源
                //    Conn = null;
                //}
            }
        }
        //更新数据库的2个表，此处可以调用数据持久层方法
        //[AutoComplete]//设置方法属性，自动更新方法调用的结果
        public bool UpdateUser(string name, string address, int id)
        {
            //调用另外一个COM组件的方法，主要是测试事务的完整性
            SqlConnection Conn;
            try
            {
                //使用DataSet向导创建方法操作数据库，
                EnterpriseServicesCOM.DatabaseDataSetTableAdapters.userTableAdapter _userTableAdapter = new 

EnterpriseServicesCOM.DatabaseDataSetTableAdapters.userTableAdapter();
                _userTableAdapter.Update(name,address,id);

                EnterpriseServicesCOM.DatabaseDataSetTableAdapters.user2TableAdapter _user2TableAdapter = new 

EnterpriseServicesCOM.DatabaseDataSetTableAdapters.user2TableAdapter();
                _user2TableAdapter.Update(name, address, id);
                //也可以自己写代码实现数据库的操作
                //SqlConnection Conn = new SqlConnection("Data Source=|DataDirectory|Database.sdf");
                //Conn.Open();
                ////
                //SqlCommand sqlCommand = new SqlCommand("UPDATE [user] SET name = " + name + ", address =" + address + 

"WHERE (id =" + id + ")");
                //sqlCommand.ExecuteNonQuery();

                //sqlCommand.CommandText = "UPDATE [user2] SET name = " + name + ", address =" + address + "WHERE (id =" + 

id + ")";
                //sqlCommand.ExecuteNonQuery();

                ContextUtil.SetComplete();//事务成功
                return true;
                //Conn.Close();
            }
            catch (Exception e)
            {
                ContextUtil.SetAbort();//失败，事务回滚，终止
                return false;
                throw e;
            }
            finally
            {
                //if (Conn != null)
                //{ 
                //    Conn.Dispose();//释放资源
                //    Conn = null;
                //}
            }
        }
    }
}
```

     [assembly: ApplicationName("FrankEnterpriseServicesTest")]，在组件服务浏览器里的名称
     [assembly: Description("Coded By Frank Xu Lei 2/11/2009")]，在组件服务浏览器里的描述信息
     [assembly: ApplicationActivation(ActivationOption.Server)]，启动类型Library在创建进程里激活，运行在客户进程，Server类型系统进程激活．
     [assembly: ApplicationAccessControl(false)]，访问设置，关闭后，用户都可以访问．

    每个服务器组件必须继承自类ServicedComponent，ServicedComponent类继承自ContextBoundObject,这样服务就可以绑定到.net remoting上下文环境上.
    [EventTrackingEnabled(true)]，允许监视，组件对象浏览器可以看到。默认关闭，因为会降低性能．
    [Description("Coded By Frank Xu Lei For EnterpriseServices  Test")]，组件对象浏览器显示的描述信息．
    //[JustInTimeActivation(false)]，打开或者关闭JIT启动．
    [ObjectPooling(true,1,10)]，是否启用对象池，如果对象创建消耗更多的资源，可以考虑对象池．
    [Transaction(TransactionOption.Required)]．是否需要支持事务特性，必要时候创建新的事务．

     设置一个 try 块以捕获在数据库处理过程中可能出现的任何异常。您必须捕获这些异常来终止事务。

      组件必须有一个强名称。为了防止Dll hell.生成一个强名称，然后使用该强名称对程序集进行签名。步骤如下： 

     (1)在 Visual Studio .NET 命令提示符处，键入 sn.exe -k FrankXuKey.snk 以创建一个密钥文件．

     (2)将 FrankXuKey.snk 复制到您的项目文件夹中。可以在项目属性里设置签名文件．

     (3)也可以在在 AssemblyInfo.vc 中，将以下代码行添加到其他程序集属性语句之前或之后：[assembly: AssemblyKeyFileAttribute("..//..//FrankXuKey.snk")]  

     (4)保存，重新编译，然后bin目录下生成dll文件。下面我们开始部署

     ２．部署服务组件：

      使用regsvcs.exe将Dll注册到COM+ Services里面 ，在Visual studio命令行窗口输入运行regsvcs．可以把生成的DLL文件拷贝到c盘更目录下安装也可以直接输入路径进行安装，格式是：
　　regsvcs　文件名 ．注册的界面如下图２：输入命令，按回车键
![](http://images.cnblogs.com/cnblogs_com/frank_xl/1Install.gif)

图２安装结果

注册成功，regsvcs.exe把dll输入到COM+ Services中。 

如果是服务器操作系统，注册完成后可以在组件服务里查看到安装后的组件，如下图３：
![](http://images.cnblogs.com/cnblogs_com/frank_xl/2Viewer.gif)

图３对象浏览器查看组件

可以看我们的组件服务和实现的接口信息．右键可以查看组件的信息如图４．

![](http://images.cnblogs.com/cnblogs_com/frank_xl/3Attribute.gif)

图４组件属性

     现在可以看到详细组件属性信息，包括安全＼对象池等信息．我们还可以选择导出组件为MSI文件，方便安装．如图５![](http://images.cnblogs.com/cnblogs_com/frank_xl/4Export.gif)

     导出的安装文件，可以进行组件的快速部署安装．导出文件下载[/Files/frank_xl/FrankXuEnterpriseServicesMSI.rar](http://files.cnblogs.com/frank_xl/FrankXuEnterpriseServicesMSI.rar)．

    ３．简单客户端

     创建简单的控制台程序,方便调试,首先是循环调用组件服务EnterpriseServicesCOMPlus的SayHello方法.测试组件服务调用是否成功,其次是测试组件事物特性的代码,具体信息如下:

```
using System;
using System.Collections.Generic;
using System.Text;
using EnterpriseServicesCOM;
namespace EnterpriseServicesClient
{
    //客户端为控制台程序，方便测试
    class EnterpriseServicesClient
    {
        static void Main(string[] args)
        {
            //创建COM对象，测试组件服务的一般调用
            using (EnterpriseServicesCOM.EnterpriseServicesCOMPlus enterpriseServicesCOM = new EnterpriseServicesCOMPlus())
            {
                for (int i = 0; i < 10; i++)
                {

                    string sName = "Frank Xu Lei";
                    string sMessage = enterpriseServicesCOM.SayHello(sName);//调用
                    Console.WriteLine(sMessage);
                }
            }
            //创建COM对象，测试组件服务的事物特性
            using (EnterpriseServicesCOM.EnterpriseServicesCOMPlusTransaction enterpriseServicesCOMTransaction = new 

EnterpriseServicesCOMPlusTransaction())
            {
                //第一次事务操作成功，字段长度合法
                if (enterpriseServicesCOMTransaction.AddUser("FrankXu", "ShangHai"))
                {
                    Console.WriteLine("Transaction1 is successful");
                }
                else
                {
                    Console.WriteLine(Console.WriteLine("Transaction1 is failed"));
                }
                //第2次事务操作失败
                if (enterpriseServicesCOMTransaction.AddUser("FrankXuLei2009", "ShangHai2009"))
                {
                    Console.WriteLine("Transaction1 is successful");
                }
                else
                {
                    Console.WriteLine(Console.WriteLine("Transaction1 is failed"));
                }
                
            }
            Console.ReadLine();
        }
    }
}
```

测试事物特性的代码分了两种情况,第一次调用新增用户的方法事务成功,字符长度合法.第2次事务则提交失败,字符插入数据库异常.事务回滚.

　　四．总结

运行结果如下图：

![](http://images.cnblogs.com/cnblogs_com/frank_xl/5Result.gif)

  Transaction1执行成功，Transaction２执行失败．ContextUtil.SetAbort();设置上下文环境的状态信息，事物终止，该语句使所有的数据库操作回滚，数据库未增加新记录．

      今天我们详细学习了.NET Enterprise Services基本知识,以及它和COM+的关系,熟悉了.NET Enterprise Services的事务特性,详细的了解COM+组件服务的技术细节.希望能给大家一些启发和帮助.欢迎交流.下一节打算写分布式编程的另外一个技术Web Service.会附加一部分WSE的部分,另外延续本文的习惯,我会上穿完整的代码[/Files/frank_xl/EnterpriseServices.rar](http://files.cnblogs.com/frank_xl/EnterpriseServices.rar).本文参考了大牛Tim Ewald的文章COM+ Integration: How .NET Enterprise Services Can Help You Build Distributed Applications.很不错的文章,好象还没人翻译,打算周末抽时间翻译一下一起放出来,与大家分享.有兴趣的朋友可以关注.~翻译完毕:[http://www.cnblogs.com/frank_xl/archive/2009/02/15/1391046.html](http://www.cnblogs.com/frank_xl/archive/2009/02/15/1391046.html)

