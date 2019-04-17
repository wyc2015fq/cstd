# Solr Dismax示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:19:25[boonya](https://me.csdn.net/boonya)阅读数：2929










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-dismax-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-dismax-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr Dismax的例子中，我们将讨论如何使用Dismax查询为用户提供更好的搜索体验。我们将向您展示如何使用由Solr提供的boost因子和boost查询参数来获得所需的结果。为了演示Solr Dismax使用，我们将安装Solr并使用与Solr一起提供的预配置核心技术产品之一启动solr。

这个例子的首选环境是solr-5.3.0。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


## 1.安装Apache Solr

首先，让我们从以下位置下载最新版本的Apache Solr：

`http://lucene.apache.org/solr/downloads.html`

Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示：

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。

## 2.启动Apache Solr

我们可以使用命令行脚本启动服务器。让我们从命令提示符转到bin目录并发出以下命令：

`solr start -e techproducts`

这将在默认端口8983下启动Solr服务器。

我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。
solr管理工具的具体细节超出了示例的范围。您可以看到示例文档被索引并存储在Solr中。

`http://localhost:8983/solr/#/techproducts`

![Solr_dismax_techproducts](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/Solr_dismax_techproducts.jpg)
Solr管理控制台


## 3.Dismax查询解析器

Dismax查询只是由子查询生成的文档的并集，并对由子查询生成的每个文档打分。
一般来说，DisMax查询解析器的接口更像是Google的接口，而不是标准的Solr请求处理程序的接口。
这种相似性使得DisMax成为许多消费者应用程序的适当的查询解析器。

常用的查询参数有：
- **q** - 定义查询的原始输入字符串。
- **qf** - 查询字段：指定要在其上执行查询的索引中的字段。如果不存在，默认为df。
- **bq** - Boost查询：指定一个因子，通过该因子，术语或短语在重要性时应该“提升”
考虑一场比赛。

现在在浏览器中打开以下URL。Dismax查询将在文档中搜索术语`video` ，并根据分数对结果排序。我们只从文档的各种文档和分数中选择了产品的名称。

`http://localhost:8983/solr/techproducts/select?defType=dismax&q=video&fl=name,score`

![Solr_dismax_output](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/Solr_dismax_output.jpg)
Solr Dismax输出


## 4.使用带有升压因子的查询字段

产品`ASUS Extreme N7800GTX/2DHTV (256 MB)`获得与`ATI Radeon X1900 XTX 512 MB PCIE Video Card`相同的分数。请注意，即使搜索字词`video`存在于视频卡的名称字段中，它也没有得到更高的分数。Dismax提供了根据特定搜索字段和分配给它的数值来提高分数的选项。

在浏览器中打开以下URL。该查询将提高在名称字段中存在搜索词的文档的分数。结果将促进视频卡获得更高的分数，并在其他结果的顶部展示文档。

`http://localhost:8983/solr/techproducts/select?defType=dismax&q=video&fl=name,score&qf=name^1.0+features^0.3`

![Solr_dismax_boost_factor](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/Solr_dismax_boost_factor.jpg)
Solr boost因子


## 5.使用Boost查询参数

Boost查询或bq参数指定一个附加的可选查询子句，将被添加到用户的主查询以影响分数。
继续上面的查询带有boost因子，我们将添加一个额外增加与类别（猫）作为显卡的产品。

在浏览器中打开以下URL。该查询将提高显卡产品的分数。您可以检查结果集以查看参数的影响。

`http://localhost:8983/solr/techproducts/select?defType=dismax&q=video&fl=name,score&qf=name^1.0+features^0.3&bq=cat:graphics%20card^5.0`

![Solr_dismax_boost_query](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/Solr_dismax_boost_query.jpg)
Dismax Boost查询




