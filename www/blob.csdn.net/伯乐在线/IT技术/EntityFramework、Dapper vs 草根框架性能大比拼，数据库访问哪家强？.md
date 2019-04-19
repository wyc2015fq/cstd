# EntityFramework、Dapper vs 草根框架性能大比拼，数据库访问哪家强？ - 文章 - 伯乐在线
原文出处： [我叫So](http://www.cnblogs.com/so9527/p/5674498.html)
# 扯淡
当前市面上 ORM 很多，有跑车型的，如 Dapper，有中规中矩型的，如 Mybatis.Net，也有重量型的，如 EntityFramework 和 NHibernate，还有一些出自草根之手的，如 Chloe.ORM。各式各样，层出不穷。试问，为何要重复造轮子？很简单，咱来自火星，目前地球上还没一款轮子适合咱这辆火星车~
为加深对各个 ORM 框架的了解，同时也想看看咱自己的框架性能如何，也可以让对 Chloe 感兴趣的同学有所了解，今儿，做个性能比较测试。测试对象为大家较熟悉的 EntityFramework 和有“性能之王”之称的 Dapper，以及草根框架 Chloe.ORM。
# 基本介绍
- **EntityFramework：**EntityFramework 是微软官方提供的 ORM，俗称 EF，拥有坚不可摧的后台，可谓无人不知，无人不哓。其对 Linq 完美支持，功能丰富，但 EntityFramework Core 之前的版本，一直被业界人员贴上笨重、不可控、性能差的标签，很多人 Hold 不住它。可见，EntityFramework 的伯乐不多啊！不知道 EntityFramework Core 变化了多少，期待！ 本文测试使用的版本是 EntityFramework 6.1。
- **Dapper：**Dapper 的背景同样不简单，目前支撑国外知名网站 stackoverflow 的数据访问层，其知名度也很高。在众多 ORM 中，堪称性能之王。作为一款微型 ORM，很受国内开发者的欢迎，毕竟经过大网站 stackoverflow 的考验。很多自主开发的 ORM 做性能测试，都会选择 Dapper 作为比较对象。Chloe.ORM 也不例外，哈哈！
- **Chloe.ORM**：草根框架，初生牛犊。点击查看[更多介绍…](http://www.cnblogs.com/so9527/p/5636216.html)
# 测试内容
本次只对各 ORM 查询效率做评测。ORM 的性能损耗主要在 DataReader 向实体转换和生成 sql 语句这两过程，因此，本次测试的内容就考察以下两方面：
**1**.映射能力。
**2**.查询能力(由于无法仅测试 sql 生成阶段的性能，所以这点测试包括 sql 生成和映射能力)，即一个完整的查询。
# 测试指标
**1**.速度。即执行相同的查询所用时 。
**2**.GC 回收次数。即执行相同的查询 GC 执行回收次数。GC 次数越多说明程序运行内存开销越大。本指标测试通过 vs2013 自带的性能分析工具，它可以自动帮我们分析统计程序运行分配的内存以及 GC 回收次数，不懂的同学可以去了解下。打开 vs，分析 –> 性能与诊断 –> 内存使用率。
# 测试环境与准备
**机器**：戴尔 xps13 笔记本，CPU 为 i7-4510U，内存8G，win 10系统。
**表**：


MySQL
```
CREATE TABLE [dbo].[TestEntity](
[Id] [int] IDENTITY(1,1) NOT NULL,
[F_Byte] [tinyint] NULL,
[F_Int16] [smallint] NULL,
[F_Int32] [int] NULL,
[F_Int64] [bigint] NULL,
[F_Double] [float] NULL,
[F_Float] [real] NULL,
[F_Decimal] [decimal](18, 0) NULL,
[F_Bool] [bit] NULL,
[F_DateTime] [datetime] NULL,
[F_Guid] [uniqueidentifier] NULL,
[F_String] [nvarchar](100) NULL,
 CONSTRAINT [PK_Test] PRIMARY KEY CLUSTERED
(
[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]
```
**数据准备**：
向 TestEntity 表插入 1000000 条数据。


MySQL
```
declare @i int = 0;
begin tran;
while(@i<=1000000)
begin
INSERT INTO [dbo].[TestEntity]
           ([F_Byte]
           ,[F_Int16]
           ,[F_Int32]
           ,[F_Int64]
           ,[F_Double]
           ,[F_Float]
           ,[F_Decimal]
           ,[F_Bool]
           ,[F_DateTime]
           ,[F_Guid]
           ,[F_String])
     VALUES
           (1
           ,2
           ,@i
           ,@i
           ,@i
           ,@i
           ,@i
           ,@i%2
           ,GETDATE()
           ,NEWID()
           ,'Chloe' + CAST(@i AS nvarchar(1000))
   )
set @i=@i+1;
end
commit;
```
# 测试方案
## 1.映射能力测试
映射能力是指 DataReader 向实体转换，这过程有很多方式，主流的就是反射和 Emit 动态生成委托两种方式。反射的性能相对比较差，据了解，早期的一批 ORM 大部分是用反射，后来大家意识到反射性能问题，基本都转 Emit 或用其它方式代替了。为减少程序其它方面对测试结果的影响，我设计的方案是一次查询 N 条数据，并且不加任何 where 条件，这样就可以提高映射性能损耗在整个测试中占比，减少其它方面(数据库执行查询、sql 生成等)对测试结果在整个测试中性能影响占比，总之，N 值越大，额外因素对测试结果影响越小，数据更真实。本次测试，我选择一次查50万条数据。测试代码：

MySQL
```
class MappingSpeedTest
{
    static int takeCount = 50 * 10000;
    public static void GCMemoryTest()
    {
        /*
         * 内存分配测试通过 vs2013， 分析 --> 性能与诊断 --> 内存使用率 测试
         * 每次运行程序只能调用下面中的一个方法，不能同时调用
         */
        ChloeQueryTest(takeCount);
        //ChloeSqlQueryTest(takeCount);
        //DapperQueryTest(takeCount);
        //EFLinqQueryTest(takeCount);
        //EFSqlQueryTest(takeCount);
    }
    public static void SpeedTest()
    {
        long useTime = 0;
        //预热
        ChloeQueryTest(1);
        useTime = SW.Do(() =>
        {
            ChloeQueryTest(takeCount);
        });
        Console.WriteLine("ChloeQueryTest 一次查询{0}条数据总用时：{1}ms", takeCount, useTime);
        GC.Collect();
        useTime = SW.Do(() =>
        {
            ChloeSqlQueryTest(takeCount);
        });
        Console.WriteLine("ChloeSqlQueryTest 一次查询{0}条数据总用时：{1}ms", takeCount, useTime);
        GC.Collect();
        //预热
        DapperQueryTest(1);
        useTime = SW.Do(() =>
        {
            DapperQueryTest(takeCount);
        });
        Console.WriteLine("DapperQueryTest 一次查询{0}条数据总用时：{1}ms", takeCount, useTime);
        GC.Collect();
        //预热
        EFLinqQueryTest(1);
        useTime = SW.Do(() =>
        {
            EFLinqQueryTest(takeCount);
        });
        Console.WriteLine("EFLinqQueryTest 一次查询{0}条数据总用时：{1}ms", takeCount, useTime);
        GC.Collect();
        //预热
        EFSqlQueryTest(1);
        useTime = SW.Do(() =>
        {
            EFSqlQueryTest(takeCount);
        });
        Console.WriteLine("EFSqlQueryTest 一次查询{0}条数据总用时：{1}ms", takeCount, useTime);
        GC.Collect();
        Console.WriteLine("GAME OVER");
        Console.ReadKey();
    }
    static void ChloeQueryTest(int takeCount)
    {
        using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
        {
            var list = context.Query().Take(takeCount).ToList();
        }
    }
    static void ChloeSqlQueryTest(int takeCount)
    {
        using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
        {
            var list = context.SqlQuery(string.Format("select top {0} * from TestEntity", takeCount.ToString())).ToList();
        }
    }
    static void DapperQueryTest(int takeCount)
    {
        using (IDbConnection conn = DbHelper.CreateConnection())
        {
            var list = conn.Query(string.Format("select top {0} * from TestEntity", takeCount.ToString())).ToList();
        }
    }
    static void EFLinqQueryTest(int takeCount)
    {
        using (EFContext efContext = new EFContext())
        {
            var list = efContext.TestEntity.AsNoTracking().Take(takeCount).ToList();
        }
    }
    static void EFSqlQueryTest(int takeCount)
    {
        using (EFContext efContext = new EFContext())
        {
            var list = efContext.Database.SqlQuery(string.Format("select top {0} * from TestEntity", takeCount.ToString())).ToList();
        }
    }
}
```
运行效果图：
![query50W](http://jbcdn2.b0.upaiyun.com/2016/08/0d2a07f357a2eb287e8d40403afd841b.png)
为公平起见，所有测试都是在非 Debug 下运行，且都经过预热。总共测试5轮，下面是测试结果数据：
||ChloeQueryTest(ms)|ChloeSqlQueryTest(ms)|DapperQueryTest(ms)|EFLinqQueryTest(ms)|EFSqlQueryTest(ms)|
|----|----|----|----|----|----|
|第1轮|6976|7170|7948|7704|7744|
|第2轮|7357|6853|8410|8328|7783|
|第3轮|7610|7833|8107|9795|8706|
|第4轮|7296|6957|7760|8643|7873|
|第5轮|9636|6705|8805|8946|8544|
|平均|7775|7103|8206|8683|8130|
上表是单纯测试用时，下面是一次查询50万条数据，GC次数情况。
运行效果图：
![neicunceshi](http://jbcdn2.b0.upaiyun.com/2016/08/1e804f0f5beafcf8d05e7426401d2c04.png)
GC统计结果，由于相同的代码运行，内存分配和GC情况都一样，所以这个测试不必运行多轮：
||ChloeQueryTest|ChloeSqlQueryTest|DapperQueryTest|EFLinqQueryTest|EFSqlQueryTest|
|----|----|----|----|----|----|
|GC回收次数|22|22|38|40|35|
**可见，Chloe.ORM 在各方面略优，看到这个结果，估计大家觉得不可能。毕竟部分国人喜欢扬眉崇外，国外的月亮比国内圆。开始看到这个结果我也觉得有点不可思议，后来翻看 Dapper 源码，发现 Chloe 稍微快一点是出乎意外，但也在情理之中。EF 映射方式我没去了解，Chloe 和 Dapper 两者创建实体以及属性赋值都是用 Emit 生成委托，这方面性能可以说毫无差距。但不同的是，Chloe 从 DataReader 读数据是调用强类型方法(GetInt(int i)、GetDateTime(int i)、GetString(int i)…等)，所以对于值类型数据的读取，避免了装箱和拆箱，从而减少了很多垃圾对象，随之 GC 次数减少。Dapper 则不然，它从 DataReader 读取数据是使用 DataReader[int i]，其内部实现就是调用 DataReader.GetValue(int i)，如果是值类型的数据，会引起大量的装箱和拆箱，需要 CPU 大量计算的同时还会产生很多的垃圾对象，随之 GC 次数增加。我想这就是 Chloe 在映射方面略胜一筹的原因。可见，Chloe 在映射方面已经做到极致。**
其实，三者在映射能力方面差距不大。为了能看出性能差异，我们一次查询了大量的数据，这仅仅是为了测试效果，在实际生产中是不会有这种情况。
**结论**：
**1**.速度：取平均值，**EFLinqQuery **
**2**.GC 次数：**EFLinqQuery **
## 2.查询能力测试
**查询能力包括 sql 生成能力和映射能力，即一个完整的查询，比较符合程序实际运行情况**。本测试针对 ORM 性能评测，为减少数据库执行 sql 耗时的干扰，我设计方案是，一次查询只查一条数据，同时考验下对 lambda 的解析能力，加了个 a.Id > id(0) 的 where 条件，执行多次查询(本测试我选择执行20000次查询)。
**测试 2.1**：

MySQL
```
/// 
/// 测试 ORM 的查询能力。测试方法：一次查询一条数据，循环执行多次查询，计算所用时间和内存分配以及GC次数
/// 该类测试循环体内包括创建 DbContext 上下文
/// 
class LoopQueryTest
{
    static int takeCount = 1;
    static int queryCount = 20000;
    public static void GCMemoryTest()
    {
        /*
         * 内存分配测试通过 vs 自带诊断与分析工具测，vs --> 分析 --> 性能与诊断 --> 内存使用率。
         * 每次运行程序只能调用下面中的一个方法，不能同时调用
         */
        ChloeQueryTest(takeCount, queryCount);
        //ChloeSqlQueryTest(takeCount, queryCount);
        //DapperQueryTest(takeCount, queryCount);
        //EFLinqQueryTest(takeCount, queryCount);
        //EFSqlQueryTest(takeCount, queryCount);
    }
    public static void SpeedTest()
    {
        long useTime = 0;
        //预热
        ChloeQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            ChloeQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("ChloeQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        useTime = SW.Do(() =>
        {
            ChloeSqlQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("ChloeSqlQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        DapperQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            DapperQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("DapperQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        EFLinqQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            EFLinqQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("EFLinqQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        EFSqlQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            EFSqlQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("EFSqlQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        Console.WriteLine("GAME OVER");
        Console.ReadKey();
    }
    static void ChloeQueryTest(int takeCount, int loops)
    {
        for (int i = 0; i using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
            {
                int id = 0;
                var list = context.Query().Where(a => a.Id > id).Take(takeCount).ToList();
            }
        }
    }
    static void ChloeSqlQueryTest(int takeCount, int loops)
    {
        for (int i = 0; i using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
            {
                int id = 0;
                var list = context.SqlQuery(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), DbParam.Create("@Id", id)).ToList();
            }
        }
    }
    static void DapperQueryTest(int takeCount, int loops)
    {
        for (int i = 0; i using (IDbConnection conn = DbHelper.CreateConnection())
            {
                int id = 0;
                var list = conn.Query(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), new { Id = id }).ToList();
            }
        }
    }
    static void EFLinqQueryTest(int takeCount, int loops)
    {
        for (int i = 0; i using (EFContext efContext = new EFContext())
            {
                int id = 0;
                var list = efContext.TestEntity.AsNoTracking().Where(a => a.Id > id).Take(takeCount).ToList();
            }
        }
    }
    static void EFSqlQueryTest(int takeCount, int loops)
    {
        for (int i = 0; i using (EFContext efContext = new EFContext())
            {
                int id = 0;
                var list = efContext.Database.SqlQuery(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), new SqlParameter("@Id", id)).ToList();
            }
        }
    }
}
```
运行效果图：
![多次查询结果](http://jbcdn2.b0.upaiyun.com/2016/08/2c0768033b598c7023d5354723c4f11a.png)
运行5次，下面是用时结果：
||ChloeQueryTest(ms)|ChloeSqlQueryTest(ms)|DapperQueryTest(ms)|EFLinqQueryTest(ms)|EFSqlQueryTest(ms)|
|----|----|----|----|----|----|
|第1轮|15083|12594|13134|41163|24339|
|第2轮|15597|12711|12133|40294|25281|
|第3轮|15356|11885|11587|44913|25707|
|第4轮|16419|13089|12803|46196|25635|
|第5轮|16216|12463|12221|40064|23749|
|平均|15734|12548|12375|42526|24942|
再来看看GC情况：
||ChloeQueryTest|ChloeSqlQueryTest|DapperQueryTest|EFLinqQueryTest|EFSqlQueryTest|
|----|----|----|----|----|----|
|GC回收次数|116|47|49|538|359|
**不测不知道，一测吓一跳。看到这个结果，我吃了那啥好几斤，不知道是我代码有问题还是怎么回事，EF 居然如此“差强人意”，好伤心。**
不过仔细看**测试2.1**的测试代码，发现循环体内包含了创建 DbContext 的代码，我在想会不会是因为多次创建 DbContext 而导致 EF 如此慢？如果把创建 DbContext 的代码放到循环体外 EF 会不会更好点？于是就有了**测试2.2**。
**测试2.2：**

MySQL
```
/// 
/// 测试 ORM 的查询能力。测试方法：一次查询一条数据，循环执行多次查询，计算所用时间和内存分配以及GC次数
/// 该类测试循环体内不包括创建 DbContext 上下文
/// 
class LoopQueryTestWithNotCreateDbContext
{
    static int takeCount = 1;
    static int queryCount = 20000;
    public static void GCMemoryTest()
    {
        /*
         * 内存分配测试通过 vs 自带诊断与分析工具测，vs --> 分析 --> 性能与诊断 --> 内存使用率。
         * 每次运行程序只能调用下面中的一个方法，不能同时调用
         */
        //ChloeQueryTest(takeCount, queryCount);
        //ChloeSqlQueryTest(takeCount, queryCount);
        //DapperQueryTest(takeCount, queryCount);
        //EFLinqQueryTest(takeCount, queryCount);
        EFSqlQueryTest(takeCount, queryCount);
    }
    public static void SpeedTest()
    {
        long useTime = 0;
        //预热
        ChloeQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            ChloeQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("ChloeQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        useTime = SW.Do(() =>
        {
            ChloeSqlQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("ChloeSqlQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        DapperQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            DapperQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("DapperQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        EFLinqQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            EFLinqQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("EFLinqQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        //预热
        EFSqlQueryTest(1, 1);
        useTime = SW.Do(() =>
        {
            EFSqlQueryTest(takeCount, queryCount);
        });
        Console.WriteLine("EFSqlQueryTest 执行{0}次查询总用时：{1}ms", queryCount, useTime);
        GC.Collect();
        Console.WriteLine("GAME OVER");
        Console.ReadKey();
    }
    static void ChloeQueryTest(int takeCount, int loops)
    {
        using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
        {
            for (int i = 0; i int id = 0;
                var list = context.Query().Where(a => a.Id > id).Take(takeCount).ToList();
            }
        }
    }
    static void ChloeSqlQueryTest(int takeCount, int loops)
    {
        using (MsSqlContext context = new MsSqlContext(DbHelper.ConnectionString))
        {
            for (int i = 0; i int id = 0;
                var list = context.SqlQuery(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), DbParam.Create("@Id", id)).ToList();
            }
        }
    }
    static void DapperQueryTest(int takeCount, int loops)
    {
        using (IDbConnection conn = DbHelper.CreateConnection())
        {
            for (int i = 0; i int id = 0;
                var list = conn.Query(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), new { Id = id }).ToList();
            }
        }
    }
    static void EFLinqQueryTest(int takeCount, int loops)
    {
        using (EFContext efContext = new EFContext())
        {
            for (int i = 0; i int id = 0;
                var list = efContext.TestEntity.AsNoTracking().Where(a => a.Id > id).Take(takeCount).ToList();
            }
        }
    }
    static void EFSqlQueryTest(int takeCount, int loops)
    {
        using (EFContext efContext = new EFContext())
        {
            for (int i = 0; i int id = 0;
                var list = efContext.Database.SqlQuery(string.Format("select top {0} * from TestEntity where Id>@Id", takeCount.ToString()), new SqlParameter("@Id", id)).ToList();
            }
        }
    }
}
```
运行5次，得到以下结果：
||ChloeQueryTest(ms)|ChloeSqlQueryTest(ms)|DapperQueryTest(ms)|EFLinqQueryTest(ms)|EFSqlQueryTest(ms)|
|----|----|----|----|----|----|
|第1轮|15281|11858|11981|31394|19309|
|第2轮|15194|12177|12314|31464|18161|
|第3轮|15967|12348|12366|31082|18030|
|第4轮|15371|11793|12479|32314|18356|
|第5轮|15350|11921|11937|35023|18356|
|平均|15411|12019|12215|32255|18442|
GC 情况：
||ChloeQueryTest|ChloeSqlQueryTest|DapperQueryTest|EFLinqQueryTest|EFSqlQueryTest|
|----|----|----|----|----|----|
|GC回收次数|111|41|47|368|205|
看起来改后测试数据较**测试2.1**稍微提升了点，**提升幅度最明显的就是 EF，时间减少近 10 秒，GC 次数也减少了很多。如此看来，EF 创建和销毁 DbContext 上下文也是个挺耗性能的环节。不测还真不知道。不过， EF 的测试结果仍然不尽人意，EFLinqQueryTest 耗时依然比同为对象化查询的 ChloeQueryTest 多出一倍的时间，GC 次数是 ChloeQueryTest 的3倍还高。而且作为 EF 原生 sql 查询的 EFSqlQueryTest 在耗时、GC次数上居然比 ChloeQueryTest 要差，有点说不过去~真想知道 EF 内部做了什么不为人知的工作！**
**DapperQuery 和 ChloeSqlQuery 都是原生 sql 查询，在测试2.1中 DapperQuery 稍微快点，但在测试2.2中 ChloeSqlQuery 实现了反超。仔细想想，其实也不难理解，Chloe 的 DbContext 创建会伴随很多对象的创建，也耗不少资源，在测试2.2中，只创建了一个 DbContext 对象，随之，各方面提升也是肯定的。ChloeQuery 跟前两者压根没可比性，毕竟前两者没任何解析和生成 sql 过程，ChloeQuery 相对慢那是必然的。慢的那几秒就是用于解析 lambda 和生成 sql 。鱼和熊掌不可兼得，想要获得开发方便，性能损耗在所难免！**
EF…“名副其实”的慢，不说了，都是泪- -。跪求大神来给 EF 正名！
**结论：**
**1**.速度：取平均值，**EFLinqQuery **
**2**.GC 次数：**EFLinqQuery **
# 评测总结
- 映射能力：从 DataReader 向实体转换过程，Dapper 和 Chloe 都是利用 Emit 动态生成委托的方式，我相信 EF 也是这样。因此，在创建实体和属性赋值环节3者都相当。但不同的是，Chloe 在读 DataReader 数据的上做到了极致，所以在映射转换性能上相对比 Dapper 和 EF 要高些。Dapper 和 EF 则差不多。其实，速度上3者都相当，主要差别就是在内存开销上。
2.查询能力：查询能力是指框架执行一次完整的查询所耗时多少与内存开销大小。从[**测试2.1**](#test_2_1)和[**测试2.2**](#test_2_2)测试结果数据中我们可以看到，Dapper 和 Chloe 的原生 sql 查询性能几乎一样，差距不大，Chloe 的对象化查询较前两者稍微差点，主要是生成 sql 过程比较消耗性能。EF “不负众望”，以垫底的姿势存在，无论是在速度还是GC次数上都比前两者差一大截。EF 的映射能力其实不差(从映射能力测试数据中可以知道)，查询速度之所以慢，毫无疑问，问题出现在执行 sql 之前的环节。不过，EF 完美支持 Linq，丰富强大的功能着实让众多草根框架望尘莫及，希望 EntityFramework Core 版本性能有所大幅度提升。
# 关于 Chloe.ORM
我的开发原则是，只要在我的能力范围内，不影响大局，要做就做到极致，这是一种追求。Chloe.ORM 还有部分地方可以优化，但优化后对性能提升也不会很大，最近忙，也懒得折腾了。目前框架整体架构、功能都已经稳定，现在只支持 SqlServer，接下来的发展目标是支持 MySql，对 Chloe 感兴趣的同学可加入《.NET技术共享》群(群号：**325936847**)。为防止一些不相干人等混入群内，申请加群时需要您回答问题，只要你**愿意**，即可进群！谢谢合作。
关于源码，目前很缺乏注释与规范，一些类、方法以及变量的命名不是很好(英文，硬伤- -)，给对源码感兴趣的同学阅读带来很大的困扰，在此说声抱歉。往后，我会适当的加上一些注释。很多时候，除了一些极其关键点，我一般不会去写注释，特别是开发阶段，代码变换太频繁，维护代码的同时还要维护相应的注释，太幸苦，咱吃不消。所谓好代码就是最好的注释，像我这种懒人，不习惯注释，如果代码也不好好写的话，回过头我自己读自己的代码我估计都读不懂。因此，不写注释，也形成了一种自我逼迫，促使我必须把代码写得干净利落、优雅、易读。如果有必要，往后有空我也可以出篇 Chloe.ORM 框架内部架构设计和实现介绍的文章。
# 结语
本次测试并不是想证明谁好谁坏，只是想通过对比去了解各个 ORM 之间在性能上的差异，以便我们更好的为项目做技术选型。一样东西，存在必然有它存在的价值，项目开发，选择合适的框架才是重要的。Dapper 堪称性能之王，但它极度依赖手写 sql，开发效率低，容错率不高，如果一个项目不是高性能要求，选择一个快速开发框架就好，短时间内完成项目才是主要的。我们要做的就是利益最大化。实际上，并非个个项目都是 stackoverflow！
倘若一个项目数据层用 Dapper 也好，用 EntityFramework 也罢，在同样的服务器上跑，都可以完美运行，用户完全感觉不到差别，我们为何不选开发效率高的 EntityFramework 呢？就目前，我们公司里部分项目，用户群体不是面向大众，我们就是用 EntityFramework，用它开发效率就是高，项目进展快，我们没理由用其它框架，很简单。
作为一名开发人员，很多时候真正的价值并不是你代码写得多好，程序运行多快，而是如何能在同样的时间内给用户、公司、社会带来最大的收益。
ps：所有的测试代码都已同步在 GitHub，地址：[https://github.com/shuxinqin/Chloe/tree/master/src/DotNet/ChloePerformanceTest](https://github.com/shuxinqin/Chloe/tree/master/src/DotNet/ChloePerformanceTest)。
