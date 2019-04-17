# Solr分面搜索示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:03:10[boonya](https://me.csdn.net/boonya)阅读数：1602










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-faceted-search-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-faceted-search-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr分面搜索示例中，我们将讨论关于使用faceting数据，并讨论Solr中可用的不同facet选项。对于我们的讨论，我们将使用Solr安装附带的一个集合示例（技术产品），以方便设置。我们将向您展示如何利用Solr facet参数来实现所需的搜索结果。

我们首选的环境是Windows。
在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


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

![Solr管理控制台](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_admin_console1.jpg)
Solr管理控制台


## 3.分面搜索

Faceting是基于索引术语将搜索结果安排到类别中的过程。
构面搜索的输出是为每个搜索项找到的数字计数。此功能非常有用，通过缩小搜索结果在搜索期间提供更好的用户体验。

以下是facet的一般参数。
- **facet** - 如果设置为true，则启用**构**面。
- **facet.query** - 指定一个Lucene查询以生成构面计数。

### 3.1现场价值分面

在本示例中，我们将facet值设置为true，并设置facet.field参数。
facet.field参数标识要作为构面处理的字段。查询中使用的其他参数是Solr常规查询参数，与faceting无关。要获得有关这些参数的更多信息，请查看我们前面的示例。

现在导航到以下URL。这将带来价格范围为0到400的产品，并按类别对结果进行分组。

`http://localhost:8983/solr/techproducts/select?q=price:[0 TO 400]&fl=id,name,price&facet=true&facet.field=cat`

![Solr Facet字段值](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_facet_field_value.jpg)
Solr Facet字段值


当使用Field-Value faceting时，还有其他几个facet参数可用于调整搜索结果。

### 3.2范围分面

我们可以在支持范围查询的日期或数字字段上使用范围构面。
此功能非常有助于通过对范围中的参考字段进行分块来提供更好的用户体验。在这个例子中，我们将使用价格字段来做范围分面。在查询中使用以下参数。
- **facet.range** - 按范围指定要分面的字段。
- **facet.range.start** - 指定构面范围的开始。
- **facet.range.end -**指定构面范围的开始。
- **facet.range.gap -**指定范围的跨度作为要添加到下限的值。

现在导航到以下URL。这将使数值产品的价格范围进入范围100的结果。

`http://localhost:8983/solr/techproducts/select?q=price:[0 TO 4000]&fl=id,name,price&facet=true&facet.field=cat&facet.range=price&f.price.facet.range.start=0.0&f.price.facet.range.end=1000.0&f.price.facet.range.gap=100`

![Solr面数范围](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_facet_range.jpg)
Solr面数范围


### 3.3间隔面

Solr的另一个特性是Interval faceting。
这看起来类似于Range faceting，但Interval faceting提供了设置变量范围的选项，而前者只能设置一个固定的间隔。
为了在字段上使用Interval Faceting，需要该字段启用“docValues”。

要修改字段，请导航到`example\techproducts\solr\techproducts\conf` ，并在schema.xml文件中将“docValues”属性设置为true，如下所示。

*schema.xml*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"weight"``type``=``"float"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“weight”type =“float”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`2`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"price"``type``=``"float"``indexed``=``"true"``stored``=``"true"``docValues``=``"true"``/></``span````> <````field``name =“price”type =“float”indexed =“true”stored =“true”docValues =“true”/></``span``>`|
|----|----|


|`3`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"popularity"``type``=``"int"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“popularity”type =“int”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`4`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"inStock"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“inStock”type =“boolean”indexed =“true”stored =“true”/></``span``>`|
|----|----|




由于我们修改了配置，我们必须重新启动Solr实例。
打开命令提示符，导航到bin文件夹并发出以下命令。

`solr stop -all`

`solr -e techproducts`

在本示例中，我们将使用以下faceting参数。
- **facet.interval** - 按间隔指定要分面的字段。
- **facet.interval.set** - 设置字段的间隔。

我们可以使用以下语法来包含或排除在设置间隔中提供的值。

**（1,10）** - >将包括大于1且小于10的值。

**[1,10）** - >将包括大于或等于1且小于10的值。

**[1,10]** - >将包括大于或等于1且小于或等于10的值。

现在导航到以下URL。这将为查询中提供的时间间隔带来产品的数字计数。

`http://localhost:8983/solr/techproducts/select?q=*:*&fl=id,name,price&facet=true&facet.field=cat&facet.interval=price&f.price.facet.interval.set=[0,10]&f.price.facet.interval.set=(10,100]`

![Solr Facet间隔](https://examples.javacodegeeks.com/wp-content/uploads/2015/08/solr_facet_int.jpg)
Solr Facet间隔


## 4.下载配置

这是一个solr分面搜索的例子。

**下载**
您可以在这里下载示例的模式： [**Schema.xml**](http://examples.javacodegeeks.com/wp-content/uploads/2015/08/facet_schema.zip)



