# Solr过滤器查询示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:06:35[boonya](https://me.csdn.net/boonya)阅读数：3739










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-filter-query-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-filter-query-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr过滤器查询示例中，我们将讨论如何实现Apache Solr提供的过滤器查询功能。我们将讨论如何使用单个和多个过滤器查询来实现所需的结果。此外，我们将展示Solr提供的各种过滤器查询语法，并讨论使用一个优于其他的优点。

为了演示过滤器查询用法，我们将使用基本配置在Solr中创建一个核心，并索引与Solr安装一起提供的示例文件。

这个例子的首选环境是solr-5.0.0。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


## 1.安装Apache Solr

首先，让我们从以下位置下载最新版本的Apache Solr：

`http://lucene.apache.org/solr/downloads.html`

Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示：

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。

我们可以使用命令行脚本启动服务器。让我们从命令提示符转到bin目录并发出以下命令：

`solr start`

这将在默认端口8983下启动Solr服务器。

我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。
solr管理工具的具体细节超出了示例的范围。

`http://localhost:8983/solr/`

![Solr管理控制台](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_admin_console1.jpg)
Solr管理控制台


## 2.配置Apache Solr

在本节中，我们将向您展示如何配置solr实例的核心/集合以及如何定义字段。
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。但是对于这个例子，我们将使用Schema配置来理解Solr的内部。

### 2.1创建核心

当Solr服务器在独立模式下启动时，配置称为核心，当在SolrCloud模式下启动时，配置称为集合。在这个例子中，我们将讨论独立的服务器和核心。我们将暂停SolrCloud讨论以供日后使用。

首先，我们需要创建一个用于索引数据的Core。
Solr create命令有以下选项：
- **-c <name>** - 要创建的核心或集合的名称（必需）。
- **-d <confdir>** - 配置目录，在SolrCloud模式下有用。
- **-n <configName>** - 配置名称。
此默认值与核心或集合的名称相同。
- **-p <port>** - 要向其发送create命令的本地Solr实例的端口;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <shards>** - 将集合拆分为的分片数，默认值为1。
- **-rf <replicas>** - 集合中每个文档的副本数。默认值为1。

在本例中，我们将使用-c参数作为核心名称，使用-d参数作为配置目录。
对于所有其他参数，我们使用默认设置。

现在在命令窗口中导航`solr-5.0.0\bin`文件夹，并发出以下命令：

`solr create -c jcg -d basic_configs`

我们可以在命令窗口中看到以下输出。







```
Creating new core 'jcg' using command:
http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg

{
 "responseHeader":{
 "status":0,
 "QTime":663},
 "core":"jcg"}
```




现在我们导航到以下URL，我们可以看到jcg core被填充在核心选择器中。
您还可以查看核心的统计信息。

`http://localhost:8983/solr`

![Solr jcg core](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_jcg_core.jpg)
Solr jcg core


### 2.2修改schema.xml文件

我们需要修改文件夹`server\solr\jcg\conf`下的`schema.xml`文件，以包含字段。我们将使用随同Solr安装一起提供的示例文件“books.csv”中的一个进行索引。该文件位于文件夹`solr-5.0.0\example\exampledocs`

现在我们导航到文件夹`server\solr`目录。
您将看到一个名为`jcg`的文件夹。子文件夹即`conf`和`data`分别具有核心的配置和索引数据。

现在编辑`\server\solr\jcg\conf`文件夹中的schema.xml文件，并在uniqueKey元素后面添加以下内容。

*schema.xml*




```
<uniqueKey>id</uniqueKey>
<!-- Fields added for books.csv load-->
<field name="cat" type="text_general" indexed="true" stored="true"/>
<field name="name" type="text_general" indexed="true" stored="true"/>
<field name="price" type="tdouble" indexed="true" stored="true"/>
<field name="inStock" type="boolean" indexed="true" stored="true"/>
<field name="author" type="text_general" indexed="true" stored="true"/>
```




我们已将`indexed`的属性设置为true。
这指定了用于索引的字段，并且可以使用索引检索记录。将值设置为false将使该字段只存储，但不能查询。

还要注意，我们有另一个属性叫`stored`并设置为true。
这指定字段存储并可以在输出中返回。将此字段设置为false将使该字段仅编入索引，并且无法在输出中检索。

我们已经为“books.csv”文件中的字段分配了类型。
CSV文件“id”中的第一个字段由schema.xml文件的uniqueKey元素自动处理，用于建立索引。

由于我们已经修改了配置，我们必须停止和启动服务器。
为此，我们需要通过命令行从bin目录发出以下命令：

`solr stop -all`

服务器将立即停止。现在启动服务器通过命令行从bin目录发出以下命令：

`solr start`

## 3.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
该程序打包到JAR中，安装在`example\exampledocs`文件夹下。

现在，我们导航到命令提示符中的`example\exampledocs`文件夹，并键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下：


```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg>

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将在命令提示符中导航到`solr-5.0.0\example\exampledocs`并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。

文件“books.csv”现在将被索引，并且命令提示符将显示以下输出。







```
SimplePostTool version 5.0.0
Posting files to [base] url http://localhost:8983/solr/jcg/update using content-
type text/csv...
POSTing file books.csv to [base]
1 files indexed.
COMMITting Solr index changes to http://localhost:8983/solr/jcg/update...
Time spent: 0:00:00.647
```




## 4.过滤查询

Solr提供以下参数来过滤查询。此参数可与其他常见查询参数一起使用，以实现所需的输出。
- **fq** - 对搜索结果应用过滤器查询。

fq参数定义一个查询，可用于限制可返回的文档的超集，而不影响得分。
fq参数化查询是独立于主查询进行缓存的。当在后续查询中使用相同的过滤器时，其缓存命中，并且数据从缓存中快速返回。

### 4.1单个过滤器查询

让我们形成一个查询，在名称字段中搜索关键字*Game* 。
在浏览器中打开以下URL。此查询将获取两个记录，如屏幕截图所示。

`http://localhost:8983/solr/jcg/select?q=name`

![无过滤器](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_filter_no_filter.jpg)
无过滤器


我们将修改查询以过滤书的价格在1.00到7.00之间的结果。
使用filter参数，我们将只得到一个记录。

在浏览器中打开以下URL：

`http://localhost:8983/solr/jcg/select?q=name:Game&fq=price:[1.00 TO 7.00]`

![单过滤器](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_filter_single_filter.jpg)
单过滤器


### 4.2多个过滤器

可以在查询中多次指定fq参数。如果文档位于由参数的每个实例产生的文档集的交集中，则文档将仅包括在结果中。在下面的示例中，仅返回价格在1.00到7.00之间并且库存中的文档。

在浏览器中打开以下URL。

`http://localhost:8983/solr/jcg/select?q=name:The&fq=price:[1.00 TO 7.00]&fq=inStock:true`

![多重过滤器](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_filter_multiple_filter.jpg)
多重过滤器


由于缓存在单个参数上工作，因此建议使用多个fq参数以实现更好的缓存。

## 5.下载源代码

这是一个solr过滤器查询的示例。

**下载配置**
您可以在此处下载此示例的模式文件： [**filter_schema**](http://examples.javacodegeeks.com/wp-content/uploads/2015/08/filter_schema.zip)



