# Apache Solr同义词示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:01:40[boonya](https://me.csdn.net/boonya)阅读数：720










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-synonyms-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-synonyms-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr同义词的例子中，我们将向你展示如何使用Solr同义词特征来替换单词与我们索引的数据的相关单词。该特征通过识别给定数据上下文中的单词的不同使用来帮助提供更好的用户体验。

Solr的附带了一个过滤器工厂叫`SynonymFilterFactory`来实现这一功能。此外，它提供了一种称为配置文件`synonyms.txt`添加我们的同义词。在本例中，我们将讨论如何配置我们的图书数据的同义词。

这个例子的首选环境是solr-5.0.0。在您开始安装Solr的确保你已经安装了JDK和`Java_Home`设置为合适。


## 1.安装Apache Solr

首先，从以下位置下载最新版本的Apache Solr。

`http://lucene.apache.org/solr/downloads.html`

Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示。

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


该`bin`文件夹中包含的脚本来启动和停止服务器。
该`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。该`server`文件夹包含`logs` ，所有的Solr的日志写入文件夹。在索引期间检查日志中的任何错误将是有帮助的。该`solr`下服务器文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

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
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。在这个例子中，我们将使用参考configset
`sample_techproducts_configs` 。

首先，我们需要创建一个用于索引数据的Core。
Solr create命令有以下选项：
- **-c <名称>** -芯或集合的名称来创建（必需）。
- **-d <confdir>** -配置目录，在SolrCloud模式是有用的。
- **-n <CONFIGNAME>** -配置名称。
此默认值与核心或集合的名称相同。
- **-p <端口>** -本地Solr的实例的端口发送创建命令;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <碎片>** -碎片的数量成，缺省值为1分割的集合。
- **-rf <复制品>** -集合中的每个文件的副本的数量。
默认值为1。

在本例中，我们将使用-c参数作为核心名称，使用-d参数作为配置目录。
对于所有其他参数，我们使用默认设置。

现在浏览`solr-5.0.0\bin`在命令窗口中的文件夹，并发出以下命令。

`solr create -c jcg -d sample_techproducts_configs`

我们可以在命令窗口中看到以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Creating new core``'jcg'``using ``command``:</span>使用命令创建新核心``'jcg'``：</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>[http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg](http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg){</span> http：// localhost：8983 / solr / admin / cores？action = CREATE＆name = jcg＆instanceDir = jcg {</span>```|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"responseHeader"``:{</span> “responseHeader”：{</span>`|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"status"``:0,</span> “status”：0，</span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"QTime"``:1377},</span> “QTime”：1377}，</span>`|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"core"``:``"jcg"````}</span> “core”：“jcg”}</span>```|
|----|----|




现在我们导航到以下URL，我们可以看到jcg core被填充在核心选择器中。
您还可以查看核心的统计信息。

`http://localhost:8983/solr`

![Solr jcg core](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_jcg_core.jpg)
Solr jcg core


## 3.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
该程序打包成JAR，并提供与文件夹下的安装`example\exampledocs` 。

现在，我们定位到`example\exampledocs`文件夹在命令提示符处键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下：


```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg>

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将导航到`solr-5.0.0\example\exampledocs`在命令提示符并发出以下命令。


```bash
java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv
```


这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。

文件“books.csv”现在将被索引，并且命令提示符将显示以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>SimplePostTool version 5.0.0</span> SimplePostTool版本5.0.0</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>Posting files to [base] url[http://localhost:8983/solr/jcg/update](http://localhost:8983/solr/jcg/update) using content-````type``text/csv...</span>将文件发布到[base] url http：// localhost：8983 / solr / jcg / update使用content-``type``text / csv ...</span>`|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>POSTing``file``books.csv to [base]</span>将文件books.csv发布到[base]</span>`|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>1 files indexed.</span> 1个文件已索引。</span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>COMMITting Solr index changes to[http://localhost:8983/solr/jcg/update...</span](http://localhost:8983/solr/jcg/update...%3C/span)>提交Solr索引更改为http：// localhost：8983 / solr / jcg / update ...</span>```|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Time spent: 0:00:00.604</span>所用时间：0：00：00.604</span>`|
|----|----|




## 4.配置同义词

现在我们修改`synonyms.txt`所在的文件夹下的文件`\server\solr\jcg\conf`添加的代名词我们的数据。有两种方法可以指定同义词映射，如下所示。我们将与示例一起讨论这两个选项。
- 两个逗号分隔的单词列表，它们之间有符号“=>”。
如果令牌匹配任何单词
左边，然后右边的列表被替换。原始令牌不会被包括，除非它也在右边的列表中。
- 逗号分隔的字词列表。如果令牌匹配任何单词，则列表中的所有单词都是
替换，其将包括原始令牌。

### 4.1使用符号“=>”

我们将首先设置用于校正拼写的同义词。
打开synonyms.txt文件，并在数据上下文中添加常见的拼写错误。在这个例子中，我们将使用'the'。

*同义词*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````># Synonym mappings can be used for spelling correction too</``span``> ＃同义词映射也可用于拼写更正</``span``>`|
|----|----|


|`2`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>pixima => pixma</``span``> pixima => pixma</``span``>`|
|----|----|


|`3`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>teh => the</``span``> teh =></``span``>`|
|----|----|




由于我们修改了配置，我们必须重新启动Solr服务器。
为此，请发出以下命令：

`solr stop -all`

`solr start`

现在我们用错误的拼写查询书籍，如'teh'。
打开以下网址：

`http://localhost:8983/solr/jcg/select?q=name:"teh"`

![Solr同义词 - 输出1](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solr_synonym_teh_output.jpg)
Solr同义词 - 输出1


### 4.2逗号分隔的列表

现在让我们实现Solr同义词的另一个特性。
我们将提供一个单词的同义词列表（在我们的例子中是碰撞）。当用户键入任何相关的词，书后附有标题`clash`将被退回。类似地，我们可以添加MB用于MegaByte，GB用于GigaByte等，这取决于我们需要索引的数据的上下文。

当我们执行Solr查询时，在同义词列表中查找每个令牌，并且如果找到匹配，则发送同义词而不是令牌。新令牌的位置值被设置为使得它们都发生在与原始令牌相同的位置。

*同义词*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````># Some synonym groups specific to this example</``span``> ＃此示例特定的一些同义词组</``span``>`|
|----|----|


|`2`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>GB,gib,gigabyte,gigabytes</``span``> GB，gib，千兆字节，千兆字节</``span``>`|
|----|----|


|`3`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>MB,mib,megabyte,megabytes</``span``> MB，mib，兆字节，兆字节</``span``>`|
|----|----|


|`4`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>Television, Televisions, TV, TVs</``span``>电视，电视，电视，电视</``span``>`|
|----|----|


|`5`||
|----|----|


|`6`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>clash, battle, fight</``span``>冲突，战斗，战斗</``span``>`|
|----|----|




由于我们已经修改了配置，我们必须重新启动。
发出以下命令：

`solr stop -all`

`solr start`

现在查询标题图书`fight`或`battle` ，那就取书“`A Clash of Kings` “。

`http://localhost:8983/solr/jcg/select?q=name:"A fight"`

![Solr同义词 - 输出2](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solr_synonym_fight_output.jpg)
Solr同义词 - 输出2


## 5.模式配置

使用同义词的配置位于文件名为`schema.xml`的Solr的服务器。要查看配置，让我们从打开的位置的文件`\server\solr\jcg\conf\` ，并看看下面的部分。您可以注意到我们对fieldType text_general使用了SynonymFilterFactory过滤器。此外，我们可以注意到它只在查询时间使用。

*schema.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- A general text field that has reasonable, generic</span> <！ - 具有合理，通用的一般文本字段</span>`|
|----|----|


|`02`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">cross-language defaults: it tokenizes with StandardTokenizer,</span>跨语言默认值：它使用StandardTokenizer进行标记，</span>`|
|----|----|


|`03`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">removes stop words from case-insensitive "stopwords.txt"</span>从不区分大小写的“stopwords.txt”中删除停用词</span>`|
|----|----|


|`04`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">(empty by default), and down cases.</span> （默认为空）和down情况。</span> <span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">At query time only, it</span>仅在查询时，它</span>```|
|----|----|


|`05`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">also applies synonyms.</span>也适用同义词。</span> <span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">-->````</``span``> - ></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``fieldType``name``=``"text_general"``class``=``"solr.TextField"``positionIncrementGap``=``"100"``></``span````> <````fieldType``name =“text_general”class =“solr.TextField”positionIncrementGap =“100”></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``type``=``"index"``></``span````> <````analyzer``type =“index”></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.StandardTokenizerFactory"``/></``span````> <````tokenizer``class =“solr.StandardTokenizerFactory”/></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.StopFilterFactory"``ignoreCase``=``"true"``words``=``"stopwords.txt"``/></``span````> <````filter``class =“solr.StopFilterFactory”ignoreCase =“true”words =“stopwords.txt”/></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- in this example, we will only use synonyms at query time</span> <！ - 在这个例子中，我们将只在查询时使用同义词</span>`|
|----|----|


|`11`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left"><filter class="solr.SynonymFilterFactory" synonyms="index_synonyms.txt" ignoreCase="true" expand="false"/></span> <filter class =“solr.SynonymFilterFactory”synonyms =“index_synonyms.txt”ignoreCase =“true”expand =“false”/></span>```|
|----|----|


|`12`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">-->``</``span````> - ></````span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.LowerCaseFilterFactory"``/></``span````> <````filter``class =“solr.LowerCaseFilterFactory”/></``span``>`|
|----|----|


|`14`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`15`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``type``=``"query"``></``span````> <````analyzer``type =“query”></``span``>`|
|----|----|


|`16`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.StandardTokenizerFactory"``/></``span````> <````tokenizer``class =“solr.StandardTokenizerFactory”/></``span``>`|
|----|----|


|`17`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.StopFilterFactory"``ignoreCase``=``"true"``words``=``"stopwords.txt"``/></``span````> <````filter``class =“solr.StopFilterFactory”ignoreCase =“true”words =“stopwords.txt”/></``span``>`|
|----|----|


|`18`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.SynonymFilterFactory"``synonyms``=``"synonyms.txt"``ignoreCase``=``"true"``expand``=``"true"``/></``span````> <````filter``class =“solr.SynonymFilterFactory”synonyms =“synonyms.txt”ignoreCase =“true”expand =“true”/></``span``>`|
|----|----|


|`19`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.LowerCaseFilterFactory"``/></``span````> <````filter``class =“solr.LowerCaseFilterFactory”/></``span``>`|
|----|----|


|`20`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`21`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``fieldType``></``span````> </````fieldType``></``span``>`|
|----|----|




## 6.下载配置

这是Apache Solr同义词的一个例子。

**下载**
你可以在这里下载的代名词配置： [**synonyms.txt**](http://examples.javacodegeeks.com/wp-content/uploads/2015/07/synonyms.zip)



