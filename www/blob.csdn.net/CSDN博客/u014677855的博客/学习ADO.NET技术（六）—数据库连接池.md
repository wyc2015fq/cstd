# 学习ADO.NET技术（六）—数据库连接池 - u014677855的博客 - CSDN博客
2018年08月15日 09:31:12[xiaobigben](https://me.csdn.net/u014677855)阅读数：190
连接池在部署多用户程序时，显得非常重要。今天主要学习连接池的基本原理；如何利用连接池来提高应用程序的性能。
## 目录
- - [目录](#目录)
- [1、什么是连接池](#1什么是连接池)
- [2、连接池的工作原理](#2连接池的工作原理)- [2.1创建连接池](#21创建连接池)
- [2.2分配空闲连接](#22分配空闲连接)
- [2.3移除无效连接](#23移除无效连接)
- [2.4回收使用完的连接](#24回收使用完的连接)
- [3、连接池的几个重要属性](#3连接池的几个重要属性)- [3.1连接池属性实例](#31连接池属性实例)
- [4、连接池异常与处理方法](#4连接池异常与处理方法)
- [5、监视SQL Server连接状态的方法](#5监视sql-server连接状态的方法)
- [6、高效使用连接池的基本原则](#6高效使用连接池的基本原则)
- [说明](#说明)
## 1、什么是连接池
前面学习数据库连接的时候我们了解到，数据库连接至关重要，它告诉ADO.NET数据源在哪里，是什么格式，需要什么其他信息等。建议一个数据库连接需要消耗大量时间和大量资源。 
这是由于连接到数据库服务需要经历几个漫长的过程：建立物理通道（例如套接字或命名管道）—与服务器初次握手—分析连接字符串信息—服务器对连接进行身份验证—运行检查以便在当前事务中进行登记。 
由此可见，建立数据库连接过程非常繁琐，所以有必要重复利用已有的连接。ADO.NET就提供了名为连接池的优化方法。连接池就是这样一个容器：它存放了一定数量的与数据库服务器的物理连接。因此，当我们需要连接数据库服务器时，不必再去建立一条新的连接，而只需去连接池中取出一条空闲的连接。这样大大减少了连接数据库的开销，从而提高了应用程序的性能。
## 2、连接池的工作原理
### 2.1创建连接池
连接池是区分不同类别的。也就是说，同一时刻同一应用程序域可以有多个不同类型的连接池。连接池是如何标识区分不同类别的呢？细致来讲，连接池是由进程、应用程序域、连接字符串以及Windows标识共同组成签名来标识区分的。 
***但对于同一应用程序域来说，一般只由连接字符串来区分。当打开一条连接时，如果该条连接的签名与现有的连接池类型不匹配，则创建一个新的连接池。反之，则不创建新的连接池。***
```
using (SqlConnection conn1 = new SqlConnection("Data Source=LENOVO-PC\\MR2014;" +
                "Initial Catalog = Mytest;User Id = sa;Password = ;"))
            {
                conn1.Open();
                MessageBox.Show(conn1.State.ToString());
            }
            using (SqlConnection conn2 = new SqlConnection("Data Source=LENOVO-PC\\MR2014;" +
                "Initial Catalog = MyCar;User Id = sa;Password = ;"))
            {
                conn2.Open();
                MessageBox.Show(conn2.State.ToString());
            }
            using (SqlConnection conn3 = new SqlConnection("Data Source=LENOVO-PC\\MR2014;" +
                "Initial Catalog = Mytest;User Id = sa;Password = ;"))
            {
                conn3.Open();
                MessageBox.Show(conn3.State.ToString());
            }
```
上面实例中，创建了三个数据库连接，但由于conn1和conn3的连接字符串相同，所以共享了一个连接池。因此管理时只需要两个连接池。打开第二个连接，该条连接的签名与现有的连接池不匹配，因此创建一条新的连接。
### 2.2分配空闲连接
当用户创建连接请求或者说调用Connection对象的Open时，连接池管理器首先要根据连接请求的类型签名找到匹配的连接池，然后尽力分配一条空闲的连接。具体情况如下：
- 如果池中有空闲连接可用，返回该连接；
- 如果池中连接都已用完，创建一个新连接添加到池中；
- 如果池中连接已达到最大连接数，请求进入等待队列直到有空闲连接可用。
### 2.3移除无效连接
无效连接，即不能正确连接到数据库服务器的连接。连接池存储的与数据库服务器的连接数量是有限的。因此，如果不及时移除无效连接，将会浪费连接池的空间。连接池管理器如果检测到与服务器的连接已断开，或者连接长时间空闲，连接池管理器会将该连接移除。
### 2.4回收使用完的连接
当我们使用完一条连接时，应当及时关闭连接和释放连接，以便连接可以返回连接池中重复使用。我们可以通过Connection对象的Close方法和Dispose方法来关闭和释放连接，也可以通过C#的using语句来关闭连接。
## 3、连接池的几个重要属性
连接池的行为可以通过连接字符串来控制，主要包括四个重要的属性。 
 - Connection Timeout:连接请求等待超时时间。默认为15秒。 
 - Max Pool Size:连接池中最大连接数，默认为100. 
 - Min Pool Size:连接池中最小连接数，默认为0. 
 - Pooling：是否启用连接池。默认启用连接池。需要禁用时，手动设置Pooling=false。
### 3.1连接池属性实例
```
SqlConnectionStringBuilder connstr = new SqlConnectionStringBuilder();
            connstr.DataSource = "LENOVO-PC\\MR2014";
            connstr.InitialCatalog = "Mytest";
            connstr.IntegratedSecurity = true;
            connstr.Pooling = true;//开启连接池
            connstr.MinPoolSize = 0;
            connstr.MaxPoolSize = 50;
            connstr.ConnectTimeout = 10;
            using (SqlConnection conn = new SqlConnection(connstr.ConnectionString))
            {
                conn.Open();
                MessageBox.Show("\n连接状态：" + conn.State);
            }
```
## 4、连接池异常与处理方法
当用户打开一个连接而没有正确或者及时的关闭时，经常会引发“连接泄露”问题。泄露的连接，会一直保持打开状态，直到调用dispose方法。垃圾回收器（GC）才关闭和释放连接。ADO.NET需要手动的关闭使用完的连接。一个重要的误区是：当连接对象超出局部作用域范围时，就会关闭连接。实际上，当超出作用域时，释放的只是连接对象而非连接资源。
```
SqlConnectionStringBuilder connstr = new SqlConnectionStringBuilder();
            connstr.DataSource = "LENOVO-PC\\MR2014";
            connstr.InitialCatalog = "Mytest";
            connstr.IntegratedSecurity = true;
            connstr.MaxPoolSize = 5;//连接池中的最大连接数
            connstr.ConnectTimeout = 1;//连接超时时间
            SqlConnection con = new SqlConnection();
            for(int i=0;i<100;i++)
            {
                con = new SqlConnection(connstr.ConnectionString);//连接对象
                try
                {
                    con.Open();
                    MessageBox.Show("Connection "+i.ToString()+" is linked");
                }
                catch (Exception ex)
                {
                   MessageBox.Show("\n异常信息:\n"+ ex.Message);
                    break;
                }
            }
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180815132530793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
从上面结果可知，连接池的最大连接容量是5条连接，当创建第6条连接的时候，由于连接池中已经达到了最大容量且没有空闲连接，因此需要等待连接直到连接超时。当等待时间超过超时时间时，就出现了连接异常。因此，使用完的连接应当尽快的正确的关闭和释放。 
如果，我在上述每次连接之后关闭连接并且释放连接资源的话，100条连接都可以正常地建立。
```
for (int i=0;i<100;i++)
 {
 con = new SqlConnection(connstr.ConnectionString);//连接对象
  try
  {
      con.Open();
      str += "Connection " + i.ToString() + " is linked.";
  }
  catch(Exception ex)
  {
      str += "\n异常信息:\n" + ex.Message;
                   MessageBox.Show(str);
                    break;
  }
  finally
  {
      con.Close();
      con.Dispose();
  }
 }
 MessageBox.Show(str);
```
结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180815133730357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 5、监视SQL Server连接状态的方法
（1）通过活动监视器 
在数据库的活动监视器中查看：活动监视器-进程： 
![这里写图片描述](https://img-blog.csdn.net/20180815134625459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（2）使用T-SQL语句 
![这里写图片描述](https://img-blog.csdn.net/20180815135025177?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 6、高效使用连接池的基本原则
用好连接池将会大大提高应用程序的性能。相反如果使用不当，则百害而无一利。一般来说，使用连接池遵循以下原则：
- 在最晚的时刻申请连接，最早的时候释放连接
- 关闭连接时先关闭相关用户定义的事务
- 确保并维持连接池中至少有一个打开的连接
- 尽量避免池碎片的产生。主要包括集成安全性产生的池碎片以及使用许多数据库产生的池碎片
> 
提示：池碎片是许多 Web 应用程序中的一个常见问题，应用程序可能会创建大量在进程退出后才会释放的池。 这样，将打开大量的连接，占用许多内存，从而导致性能降低。
## 说明
[本篇文章是转载下面作者的，为了以便记忆。详情请戳](https://www.cnblogs.com/liuhaorain/archive/2012/02/19/2353110.html)
> 
我叫刘皓，很高兴您能阅读完我的这篇文章。 我花了大量时间和精力来完成这篇文章，如果文章对您有帮助，请不要忘了点推荐哦！ 
  如果您能点击右边的打赏按钮，打赏一杯咖啡钱，我将获得更多的动力和能量写出下一篇好文章。 
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文链接，否则保留追究法律责任的权利。 
  我的博客即将搬运同步至腾讯云+社区，邀请大家一同入驻：[https://cloud.tencent.com/developer/support-plan](https://cloud.tencent.com/developer/support-plan)
