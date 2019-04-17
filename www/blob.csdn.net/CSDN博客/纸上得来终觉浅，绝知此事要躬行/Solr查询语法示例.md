# Solr查询语法示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 10:47:46[boonya](https://me.csdn.net/boonya)阅读数：3232










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-query-syntax-examples/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-query-syntax-examples/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr查询语法示例中，我们将讨论Solr中不同的查询格式。
对于我们的讨论，我们将使用Solr安装附带的一个集合示例（ *技术产品* ）。我们将向您展示如何使用Solr公开的基于REST的API，并向您展示如何使用各种查询参数。我们首选的环境是Windows。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。

## 1.安装Apache Solr

首先，让我们从以下位置下载最新版本的Apache Solr：

`http://lucene.apache.org/solr/downloads.html`

截至本文，稳定版本可用是5.0.0。
Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示。

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。


## 2.启动Solr服务器

Solr提供了一些有用的集合示例来了解主要功能。
我们将使用与Solr捆绑在一起的技术产品集合进行讨论。要使用*techproducts*集合启动Solr服务器，请打开命令提示符，导航到bin文件夹并发出以下语法。

`solr -e techproducts`

这将在默认端口8983下启动Solr服务器。

我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。
您还可以注意到正在填充的收集技术产品。

`http://localhost:8983/solr/`

![solr_query_admin](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_admin.jpg)
Solr管理控制台


## 3. Solr基本查询

Solr提供了一个简单的基于REST的select查询来搜索索引数据。
我们必须提供集合的上下文路径（在我们的案例中是techproducts），并在URL中使用select，表示这是一个select查询。
参数*q*用于指定搜索字符串。

以下查询将在*techproducts*集合的所有索引字段中查找视频。
如果您注意到视频存在于结果1的名称字段中，并且出现在结果2的某个功能中。此类型的查询可用于在文档上进行自由文本搜索。
在浏览器中打开以下URL。

`http://localhost:8983/solr/techproducts/select?q=video`

![solr_query_basic](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_basic.jpg)
Solr查询 - 基本


## 4. Solr查询参数

Solr提供了可用于查询的参数列表。
以下部分说明可用的参数和目的。
- **qt** - 请求的查询处理程序。
如果未指定，则使用标准查询处理程序。
- **q** - 用于指定查询事件。
- **fq** - 用于指定过滤器查询。
- **sort** - 用于按升序或降序对结果进行排序。
- **start，rows** - start指定结果集的启动编号。默认值为零。rows指定要返回的记录数。
- **fl** - 用于返回选择字段。
- **wt** - 指定响应格式。
默认为XML。
- **缩进** - 设置为true会使响应更可读。
- **debugQuery** - 将参数设置为true会将调试信息作为响应的一部分。
- **dismax** - 指定dismax解析器。
- **edismax** - 指定edismax解析器。
- **facet** - 设置为true将启用faceting。
- **spatial** - 用于**地理空间**搜索。
- **spellcheck** - 设置为true帮助搜索类似的术语。

## 5. Solr高级查询

我们可以使用Solr提供的一个或多个参数来构造查询。
在本节中，我们将向您展示几个组合。

### 5.1。Solr查询 - 选择性字段

如前所述，fl参数可用于在响应中选择有限的字段集。
这将有助于限制通过系统的数据量，并降低I / O成本。

我们将修改基本查询以返回有限的字段集。
我们选择在以下查询中返回ID，名称和价格。

在浏览器中打开以下URL。您可以注意到，结果集只包含所选字段，当以字节为单位测量时，响应的大小减小。

`http://localhost:8983/solr/techproducts/select?q=video&fl=id,name,price`

![solr_query_select_field](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_select_field.jpg)
Solr查询 - 所选字段


### 5.2。Solr查询 - 过滤器

我们可以修改基本查询来添加过滤器。在基本查询中，我们没有指定任何字段来搜索字符串video，它从名称，特征等返回值。但现在我们将指定在哪里查找搜索字符串。

在浏览器中打开以下URL。您可以注意到结果只包含名称字段中包含视频的记录。

`http://localhost:8983/solr/techproducts/select?q=name:video`

![solr_query_filter_1](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_filter_1.jpg)
Solr查询 - 过滤器名称


类似地，我们可以修改查询以选择具有类别的所有产品作为电子。
在浏览器中打开以下URL。您可以注意到结果集只包含电子产品。
此外，我们已经合并了fl参数，只选择id，name和price字段。

`http://localhost:8983/solr/techproducts/select?q=cat:electronics&fl=id,name,price`

![solr_query_filter_2](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_filter_2.jpg)
Solr查询 - 过滤器类别


### 5.3。Solr查询 - 分面搜索

构面是用于将搜索结果排列成类别的特殊类型的搜索。
搜索与索引术语以及匹配文档的计数一起呈现。Faceting使用户能够轻松地浏览搜索结果，缩小搜索结果的精确范围。

在浏览器中打开以下查询。您将在响应的底部看到每个类别的facet_counts。此外，您可以注意到我们已应用过滤器的价格和只选择指定字段。

`http://localhost:8983/solr/techproducts/select?q=price:[0 TO 400]&fl=id,name,price&facet=true&facet.field=cat`

![solr_query_facet](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_query_facet.jpg)
Solr查询 - facet




