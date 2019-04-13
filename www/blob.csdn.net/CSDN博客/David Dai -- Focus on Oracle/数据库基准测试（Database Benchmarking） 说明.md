
# 数据库基准测试（Database Benchmarking） 说明 - David Dai -- Focus on Oracle - CSDN博客


2015年01月21日 16:45:34[Dave](https://me.csdn.net/tianlesoftware)阅读数：13510




# 1   TPC概述
对数据库和服务器进行压力测试或者做对比的时候，都是经常会使用HammerDB 或者 Benchmark之类的工具，而且最常用TPCC的标准进行测试。
这里说的TPCC，前3个字母是：TPC 是Transaction Processing Council（事务处理委员会），其提供了量化的方法和标准。
所以我们说的TPCC就是其中一种测试方法。
TPC的官网是：[http://www.tpc.org/](http://www.tpc.org/)

![](https://img-blog.csdn.net/20150121164458868?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
从图上可以看出，除了TPC-C 之外，还有很多其他的方法，包括废弃的TPC-A,TPC-B,TPC-D,TPC-R,TPC-W。
关于不同的测试方法的区别可以直接点击官网的说明。

# 2   Oracle 官网对Benchmarking的说明
下文直接引自Oracle 的网站，原文地址已经不存在，文章讲了database benchmarking的原因、标准，和几款测量工具软件。

## 2.1             Reasons for Benchmarking
Benchmarks are performedforvariousreasons. However, benchmarks are primarily used:
*** To compare different hardware configurations**
Benchmarks can be used to compare the relative performance ofdifferent hardware running the same application. Thisisgenerally used to directly compare hardware configurations between two hardwarevendors.
*** To compare different database vendor software**
By running the same benchmarkusingdifferent database software on the same machine, one can easily compare betweendifferent database vendors. Thisisgenerally usedto make a price/performance decision between vendors suchasOracle, Microsoft, IBM, etc.
*** To compare different database software releases**
Similar to the above, one can use different versions of the samevendor's database software to compare the one they want to use or checkfor performance regressions due to upgrades (i.e. 10g vs. 11g).

## 2.2             Real World Benchmarks
While there areseveral industry standard benchmarks, most of the time, they don'treflect a company's actual workload. As such, you will often findcustom benchmarks being performed. The goals of a real-world benchmark are
 thesame reasons mentioned above.

## 2.3             Industry Standard DatabaseBenchmarks
Industrystandard benchmarks are generally used by businesses to compare differenthardware and software system performanceforpurchase-related reasons. While industry standard benchmarks are modelled afterreal-world workloads, they rarely reflect a company's realworkload. However, it is a quick way to compare the performance
 of varioushardware and software combinations operating in a well-defined scenario. Themajor industry standard benchmarks are listed below.

## 2.4             Transaction Processing Council(TPC)
The TPCisanon-profit corporation which supports a consortium of hardware and databasesoftware vendors devoted to defining transaction processing anddatabase-related benchmarks. The primary goal behind TPC benchmarksisthedefinition of functional requirements which can be run on any database,regardless of the hardware or operating system. This allows vendors toimplement their own benchmark kitsinorder to satisfythe functional requirements. Similarly, after having publicly submitted proofthat a benchmark was conducted according to the specification, end-users havemore of an assurance that what they are presented withisavalid, apples-to-apples comparison.

### 2.4.1 TPC-B
The TPC-B benchmark stresses databases and is characterizedby significant disk input/output, moderate system, application execution time,and transaction integrity.  Thisbenchmark targets database management systems (DBMS) batch applications, andback-end
 database servers.  TPC-B is notan OLTP benchmark.

### 2.4.2 TPC Benchmark C (TPC-C)
The TPC-C simulates an order-entryenvironmentwhereapopulation of terminal operators executes transactions against a database. Thebenchmarkiscomprised of transactions which include entering and deliveringorders, recording payments, checking the status of orders, and monitoring thelevel of stock at the warehouses. The most frequent transaction consists ofentering
 aneworder which, on average,iscomprised of ten different items. Each warehouse tries to maintain stockforthe100,000itemsinthe Company's catalog and fill orders from thatstock. The performance metric
 reported by TPC-C measures the number of ordersthat can be fully processed per minute and is expressed in tpm-C. The TPC-Cwill soon be deprecated in favor of the TPC-E.

### 2.4.3 TPC-D
The TPC-D benchmark represents a broad range ofdecision support (DS) applications that require complex, and long runningqueries against large complex data structures. Real-world business questions were written against this model, andresulted in 17 complex
 queries.

### 2.4.4 TPC Benchmark E (TPC-E)
The TPC-E benchmark simulates the OLTP workload of a brokeragefirm. The focus of the benchmarkisthe centraldatabase that executes transactions related to the firm’s customer accounts.The TPC-E metricisgivenintransactions per second (tps). It specifically refers to the number ofTrade-Result transactions the server can sustain over a period of time.

### 2.4.5 TPC Benchmark H (TPC-H)
TheTPC-H is adecision support benchmark which consists of several business-oriented ad-hocqueries and concurrent data modifications. This benchmark illustrates decisionsupport systems that examine large volumes of data, execute queries with a highdegree
 of complexity, and give answers to critical business questions.
The performancemetric reported by TPC-H is called the TPC-H Composite Query-per-HourPerformance Metric (QphH@Size), and reflects multiple aspects of the capabilityof the system to process queries.

## 2.5             Standard Performance EvaluationCorporation (SPEC)

### 2.5.1 Free and Open Source Benchmark Kits
As the benchmarkkits used by various database vendors are proprietary, several open sourcedatabase benchmark kits have been written to implement not only the industrystandard workloads, but othersaswell.
*** Open Source Development Labs Database Test Suite**
The OSDL DBT suiteisthe mostcomprehensive of all open source benchmark kits. While its benchmarkimplementations are based on TPC standards, they differinsomeareas and are not certified; making a comparison between a certified TPC-Cresult and OSDL's implementation of TPC-C (DBT-2) impossible. As the originalOSDL DBT suite lacks Oracle support, Oracle
 has added and released it as partof the Oracle Linux Test kit.
*** PolePosition**
PolePositionisan open sourcebenchmark test suite to compare database engines andobject-relationalmapping technology.
*** jTPCC**
jTPCCisan open source Java implementation ofthe TPC-C benchmark. While it claims compliance with TPC-C, itisnot.
*** BenchmarkSQL**
BenchmarkSQLisan open source,easy to use JDBC benchmark which closely resembles the TPC-C standardforOLTP;it was originally based on jTPCC.
*** Bristlecone**
Bristleconeisa simpleSELECT+INSERT/UPDATE/DELETE throughput and response-time benchmark writteninJava.
*** FinTime**
FinTimeisan open sourcesetofdata and queries which reflects the needs of financial analysts who arestudying patternsinstock market data, but it should appealto the designers of any system that has pretensions of handling ordered datawell.
*** Java TPC-W Implementation**
This open-source implementationisbasedon the TPC-W specification version1.0.1. Itstrays from the official benchmark specificationina fewminor areas, which are specifiedindocumentationincluded with the distribution. This version itself lacks Oracle support, butit can be found on the web.
*** Swingbench**
Swingbenchisa free loadgenerator (and benchmarks) designed to stress test an Oracle database(9i,10g,11g).
*** OpenLink ODBC Bench**
OpenLink ODBC Benchisan open-sourceODBC Benchmarking tool providing real-time comparative benchmarkingforODBCDrivers, Database Engines, and Operating Systems combinations. The Benchmarksinthisapplication are loosely based on the TPC-A and TPC-C standard benchmarks, withmodifications to specifically test the performance of an ODBC Driver and/orDatabase Engineina client/server environment.
*** OpenLink JDBC Bench**
Similar to OpenLink's ODBC Bench, this is a JDBC version.
*** TPCC-UVa**
A free, open-source implementation of the TPC-C Benchmark.
*** Hammerora**
An open source load generation tool.

### 2.5.2 Commercial Benchmark Kits
There are several industry standard benchmarks.
*** Official Oracle Benchmark Kits**
Similar to Microsoft and IBM, the official benchmark kits used byOracle can only be obtained under certain conditions. For more information,contact your Oracle Consulting/Partner/Support representative.
*** Real Application Testing**
While many wouldn't think of Real Application Testing asa traditional benchmark kit, it does perform workload execution primarily forthe real world benchmarking case.
*** Quest Software****'sBenchmark Factory for Databases**
Benchmark FactoryforDatabasesisaworkload simulation and scalability testing tool that stress tests yourenvironment by simulating users and transactions on the database. You caneither replay production workload or use synthetic workload to test the limitsof your database.
*** SPEC jAppServer Benchmark Kit**
This can be acquired directly from SPEC.

## 2.6             Benchmarking-related Books
There are several good books regarding benchmarking. The best I'vefound include the following:
*** The Benchmark Handbook: For Database andTransaction Processing Systems (Jim Gray)**
A great, albeit outdated book. While several of the benchmarks itdiscusses have been deprecated, it presents good concepts and backgroundinformation regarding benchmarking.
*** Performance Analysis of Transaction ProcessingSystems (Wilbur H. Highleyman)**
Whileoutof print,thisbookpresents both plain-text and math-heavy concepts of queueing and performanceanalysis techniques orientedforOLTP.
*** Database Benchmarking: Practical Methods****for****Oracle & SQL Server**
This book explores all aspects of database benchmarking and showsyou a real-world approach that ensures that you are preparedforwhatever the future brings to your mission-critical database.

--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 104207940 (满)    CNDBA_2: 62697716 (满)   CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5: 62697850    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

