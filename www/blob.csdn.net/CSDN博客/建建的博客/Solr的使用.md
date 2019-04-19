# Solr的使用 - 建建的博客 - CSDN博客
2018年02月13日 14:47:23[纪建](https://me.csdn.net/u013898698)阅读数：46
1、 schema.xml
schema.xml文件在SolrCore的conf目录下，在此配置文件中定义了域以及域的类型等一些配置。在solr中域必须先定义后使用。
**1.1filed**
<field name="id"type="string" indexed="true" stored="true"required="true" multiValued="false" />
Name：域的名称
Type：域的类型
Indexed：是否索引
Stored：是否存储
Required：是否必须
multiValued：是否是多值，存储多个值时设置为true，solr允许一个Field存储多个值，比如存储一个用户的好友id（多个），商品的图片（多个，大图和小图）
1.2 dynamicField（动态域）
<dynamicFieldname="*_s" type="string" indexed="true" stored="true" />
Name：动态域的名称，是一个表达式，*匹配任意字符，只要域的名称和表达式的规则能够匹配就可以使用。
例如：搜索时查询条件[product_i：钻石]就可以匹配这个动态域，可以直接使用，不用单独再定义一个product_i域。
**1.3uniqueKey**
<uniqueKey>id</uniqueKey>
相当于主键，每个文档中必须有一个id域。
**1.4copyField(复制域)**
<copyField source="cat"dest="text"/>
可以将多个Field复制到一个Field中，以便进行统一的检索。当创建索引时，solr服务器会自动的将源域的内容复制到目标域中。
source：源域
dest：目标域，搜索时，指定目标域为默认搜索域，可以提高查询效率。
定义目标域：
<field name="text"type="text_general" indexed="true" stored="false"multiValued="true"/>
目标域必须要使用：multiValued="true"
**1.5fieldType(域类型)**
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><fieldTypename="text_general"class="solr.TextField"positionIncrementGap="100">
- <analyzertype="index">
- <tokenizerclass="solr.StandardTokenizerFactory"/>
- <filterclass="solr.StopFilterFactory"ignoreCase="true"words="stopwords.txt"/>
- <filterclass="solr.LowerCaseFilterFactory"/>
- </analyzer>
- <analyzertype="query">
- <tokenizerclass="solr.StandardTokenizerFactory"/>
- <filterclass="solr.StopFilterFactory"ignoreCase="true"words="stopwords.txt"/>
- <filterclass="solr.SynonymFilterFactory"synonyms="synonyms.txt"ignoreCase="true"expand="true"/>
- <filterclass="solr.LowerCaseFilterFactory"/>
- </analyzer>
- </fieldType>
- </span></span>
name：域类型的名称
class：指定域类型的solr类型。
analyzer：指定分词器。在FieldType定义的时候最重要的就是定义这个类型的数据在建立索引和进行查询的时候要使用的分析器analyzer，包括分词和过滤。
type：index和query。Index 是创建索引，query是查询索引。
tokenizer：指定分词器
filter：指定过滤器
**2、配置中文分词器**
使用IKAnalyzer中文分析器
第一步：把IKAnalyzer2012FF_u1.jar添加到solr/WEB-INF/lib目录下。
第二步：复制IKAnalyzer（IK Analyzer 2012FF_hf1）的配置文件（IKAnalyzer.cfg.xml）和自定义词典(ext.dic)和停用词词典(stopword.dic)到solr的solr/WEB-INF/classes目录下。
复制IK分词器配置文件、自定义词典、停用词词典
粘贴到Tomcat的solr的/WEB-INF/classes目录下
![](https://img-blog.csdn.net/20170106103729415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第三步：在schema.xml中添加一个自定义的fieldType，使用中文分析器。
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><!-- IKAnalyzer-->
- <fieldTypename="text_ik"class="solr.TextField">
- <analyzerclass="org.wltea.analyzer.lucene.IKAnalyzer"/>
- </fieldType>
- </span></span>
第四步：在schema.xml中添加field，指定field的type属性为text_ik
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><!--IKAnalyzer Field-->
- <fieldname="content_ik"type="text_ik"indexed="true"stored="true"/>
- </span></span>
第五步：重启tomcat
http://localhost:8080/solr
![](https://img-blog.csdn.net/20170106104613739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3、配置业务Field
要使用solr实现网站中商品搜索，需要 将mysql数据库中数据在solr中创建索引。 
第二步. 需要在solr的schema.xml文件定义要存储的商品Field。
第一步.. 需要把MySQL的数据导入到solr索引库中
第三步.. 开发搜索功能
**3.1将数据导入到数据库**
**3.2定义Field**
先确定定义的商品document的Field域有哪些？
可以根据mysql数据库中商品表的字段来确定：
![](https://img-blog.csdn.net/20170106105112022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.3Schema.xml中配置业务域**
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><!--product-->
- <fieldname="product_name"type="text_ik"indexed="true"stored="true"/>
- <fieldname="product_price"type="float"indexed="true"stored="true"/>
- <fieldname="product_description"type="text_ik"indexed="true"stored="false"/>
- <fieldname="product_picture"type="string"indexed="false"stored="true"/>
- <fieldname="product_catalog_name"type="string"indexed="true"stored="true"/>
- 
- <fieldname="product_keywords"type="text_ik"indexed="true"stored="false"multiValued="true"/>
- <copyFieldsource="product_name"dest="product_keywords"/>
- <copyFieldsource="product_description"dest="product_keywords"/>
- </span></span>
**3.4dataimportHandler插件**
使用dataimport插件批量导入数据。
第一步：把dataimport插件依赖的jar包(solr-4.10.3\dist下的solr-dataimporthandler-4.10.3.jar和solr-dataimporthandler-extras-4.10.3.jar)添加到solrcore（collection1\lib）中, 还需要mysql的数据库驱动。
第二步：配置solrconfig.mxl文件，添加一个requestHandler。
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><requestHandlername="/dataimport"
- class="org.apache.solr.handler.dataimport.DataImportHandler">
- <lstname="defaults">
- <strname="config">data-config.xml</str>
- </lst>
- </requestHandler>
- </span></span>
第三步：创建一个data-config.xml，保存到collection1\conf\目录下
**[html]**[view plain](http://blog.csdn.net/mrlin6688/article/details/54134704#)[copy](http://blog.csdn.net/mrlin6688/article/details/54134704#)
- <spanstyle="font-size:18px;"><spanstyle="font-family:Arial;"><?xmlversion="1.0"encoding="UTF-8"?>
- <dataConfig>
- <dataSourcetype="JdbcDataSource"
- driver="com.mysql.jdbc.Driver"
- url="jdbc:mysql://localhost:3306/lucene"
- user="root"
- password="root"/>
- <document>
- <entityname="product"query="SELECT pid,name,catalog_name,price,description,picture FROM products ">
- <fieldcolumn="pid"name="id"/>
- <fieldcolumn="name"name="product_name"/>
- <fieldcolumn="catalog_name"name="product_catalog_name"/>
- <fieldcolumn="price"name="product_price"/>
- <fieldcolumn="description"name="product_description"/>
- <fieldcolumn="picture"name="product_picture"/>
- </entity>
- </document>
- 
- </dataConfig
- </span></span>
第四步：重启tomcat（startup.bat）
![](https://img-blog.csdn.net/20170106110023182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第五步：点击“execute”按钮导入数据
注意:导入数据前会先清空索引库，然后再导入。
**3.5Solr的查询语法**
1. q： 查询关键字，必须的。
请求的q是字符串，如果查询所有使用*:*
![](https://img-blog.csdn.net/20170106110904551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.fq:(filter query)过滤查询
作用：在q查询符合结果中同时是fq查询符合的
请求fq是一个数组（多个值）
过滤查询价格从1到20的记录。
![](https://img-blog.csdn.net/20170106111031561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
也可以使用“*”表示无限，例如：
20以上：product_price:[20 TO *]
20以下：product_price:[* TO 20]
也可以在“q”查询条件中使用product_price:[1TO 20]，
如下效果和上面一样：
![](https://img-blog.csdn.net/20170106111143836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.  sort:  排序，desc代表降序，asc代表升序
按照价格升序排
![](https://img-blog.csdn.net/20170106111310182?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.start:页显示使用，开始记录下标，从0开始
rows: 指定返回结果最多有多少条记录，配合start来实现分页。
![](https://img-blog.csdn.net/20170106111501716?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.fl:(Field List)指定返回那些字段内容，用逗号或空格分隔多个。
![](https://img-blog.csdn.net/20170106111645423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
显示商品id、商品名称、商品分类名称
6.df:指定默认搜索Field
![](https://img-blog.csdn.net/20170106111807798?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
7.wt:(writer type)指定输出格式，可以有 xml, json, php, phps
![](https://img-blog.csdn.net/20170106111927267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
8.hl:是否高亮 ,设置高亮Field，设置格式前缀和后缀。
![](https://img-blog.csdn.net/20170106112351317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbXJsaW42Njg4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
