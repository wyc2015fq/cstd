# lucene join解决父子关系索引 - zhanlijun - 博客园
# [lucene join解决父子关系索引](https://www.cnblogs.com/LBSer/p/4417074.html)
## [http://www.cnblogs.com/LBSer/p/4417074.html](http://www.cnblogs.com/LBSer/p/4417074.html)
## 1 背景
       以商家（Poi）维度来展示各种服务（比如团购（deal）、直连）正变得越来越流行（图1a）， 比如目前美食、酒店等品类在移动端将团购信息列表改为POI列表页展示。
![](https://images0.cnblogs.com/blog2015/522490/201504/110943487741751.png)![](https://images0.cnblogs.com/blog2015/522490/201504/110944373991129.png)
图1   a：商家维度展示信息； b：join示意    
这给筛选带来了复杂性。之前的筛选是平面的，如筛选poi列表时仅仅利用到poi的属性（如评价、品类等），筛选deal列表时也仅仅根据deal的属性（房态、价格等）。而现在的筛选是具有层次关系的，我们需要根据deal的属性来筛选Poi，举个例子，我们需要筛选酒店列表，这些酒店必须要有价格在100~200之间的团购。
       这种筛选本质是种join操作，其核心是要将poi与deal关联起来。从数据库视角上看（图1 b），我们有poi表以及deal表，deal表存储了外键（parentid）用于指示该deal所属的poi，上述筛选分为三步：1）先筛选出价格区间在100~200的deal（得到dealid为2和3的deal）；2）找出deal对应的poi（得到poiid为1和1的poi）；3）去重，因为可能多个deal对应同一个poi，而我们需要返回不重复poi。
目前我们使用lucene来提供筛选服务，那么lucene如何解决这种带有join的筛选呢？
## 2 lucene join解决方案
       在我们应用中，一个poi存储为一个document，一个deal也存储为一个document，Join的核心在于将poi以及deal的document进行关联。lucene提供了两种join的方式，分别是query time join和index time join，下文将分别展开。
### 2.1. query time join
   query time join是通过类似数据库“外键“方法来建立deal和poi document的关联关系。
**a）索引**
      分别创建poi的document和deal的document，在建立deal document的时候用一个字段（parentid）将deal与poi关联起来，本例中创建了parentid这个field，里面存的是该deal对应的poiid，可以简单将其看做外键。
```java;gutter
public static Document createPoiDocument(PoiMsg poiMsg) {
   Document document = new Document();
   document.add(new StringField("poiid", String.valueOf(poiMsg.getId()), Field.Store.YES));
   document.add(new StringField("name", poiMsg.getName(), Field.Store.YES));
   return document;
}
```
```java;gutter
public static Document createDealDocument(DealModel dealModel, PoiMsg poiMsg) {
   Document document = new Document();
   document.add(new StringField("did", String.valueOf(dealModel.getDid()), Field.Store.YES));
   document.add(new StringField("name", dealModel.getBrandName(), Field.Store.YES));
   document.add(new DoubleField("price", dealModel.getPrice(), Field.Store.YES));
   document.add(new StringField("parentid", String.valueOf(poiMsg.getId()), Field.Store.YES));
   return document;
}
```
```java;gutter
IndexWriter writer = new IndexWriter(directory, config);
writer.addDocument(createPoiDocument(poiMsg1)); 
writer.addDocument(createPoiDocument(poiMsg2));
writer.addDocument(createDealDocument(dealModel1, poiMsg2));
writer.addDocument(createDealDocument(dealModel2, poiMsg1));
writer.addDocument(createDealDocument(dealModel3, poiMsg1));
```
**b）查询**
      需查询两次：首先查询deal document，之后通过deal中的parentId查询poi document。
     1）第一次查询发生在JoinUtil.createJoinQuery中。首先创建了TermsCollector这个收集器， 该收集器将满足fromQuery的doc的parentid字段收集起来，之后创建了TermsQuery。
          本例执行之后TermsCollector集合里有两个terms，分别是”1”和”1”；
     2）执行TermsQuery，查询toField在TermsCollector terms集合中存在的doc，最后找出toField为“1”的doc。
```java;gutter
IndexSearcher indexSearcher = new IndexSearcher(indexReader);
        String fromFields = "parentid";
        Query fromQuery = NumericRangeQuery.newIntRange("price", 100, 200, false, false);
        String toFields = "poiid";
        Query toQuery = JoinUtil.createJoinQuery(fromFields, false, toFields, fromQuery, indexSearcher, ScoreMode.Max);
        TopDocs results = indexSearcher.search(toQuery, 10);
```
```java;gutter
JoinUtil.createJoinQuery代码
 TermsCollector termsCollector = TermsCollector.create(fromField, multipleValuesPerDocument);
 fromSearcher.search(fromQuery, termsCollector);
 return new TermsQuery(toField, fromQuery, termsCollector.getCollectorTerms());
```
**c）优缺点**
       query time join优点是非常直观且灵活；缺点是不能进行打分排序，此外由于查询两遍性能会下降。
### 2.2. index time join
        query time join通过显式的在deal document上增加一个“外键”来建立关系，找到deal之后需要找出这些deal document的parentid集合，之后再次查询找出poiId在parentid集合内的poi document。在找到deal之后如果能马上找到对应的poi document，那将大大提高效率。index time join干的就是这样的事情，其通过一种精巧的方法建立了deal document id和poi document id的映射关系。
**a）原理**
      如何通过一个deal document id来找到poi document id？
在lucene中，doc id是自增的，每写入一个document，doc id加1（简单起见可以理解）。 index time join要求写索引的时候要按先后关系写入，先写子document，再写父document。比如我们有poi1和poi2两个poi，其中poi1下有deal2和deal3，而poi2下只有deal1，这时需要先写入deal2、deal3，再写入deal2和deal3对应的poi1 document，依次类推，最后形成如图2所示的结构。
      这样索引建立之后，我们得到了父document的id集合（3，5）。当我们根据deal的属性查出deal document id时，比如我们查出满足条件的deal是deal3，其document id=2，这时候只需要到父document id集合里去查找第一个比2大的id，在本例中马上就找到3。
![](https://images0.cnblogs.com/blog2015/522490/201504/110950442906740.png)
图2 
     lucene自己实现了BitSet来保存id，lucene内部实现代码如图3所示。
![](https://images0.cnblogs.com/blog2015/522490/201504/110951243362840.png)
图3 实现原理
**b）索引**
       从上述原理得知我们需要建立有层次关系的索引。
       首先创建document数组，该数组有个特点， 最后一个必须是poi，之前都是deal。然后调用writer.addDocument(documents); 将这个数组写入。
```java;gutter
public static Document createPoiDocument(PoiMsg poiMsg) {
        Document document = new Document();
        document.add(new StringField("poiid", String.valueOf(poiMsg.getId()), Field.Store.YES));
        document.add(new StringField("name", poiMsg.getName(), Field.Store.YES));
        document.add(new StringField("doctype", "poi", Field.Store.YES));
        return document;
    }
```
```java;gutter
public static Document createDealDocument(DealModel dealModel) {
        Document document = new Document();
        document.add(new StringField("did", String.valueOf(dealModel.getDid()), Field.Store.YES));
        document.add(new StringField("name", dealModel.getBrandName(), Field.Store.YES));
        document.add(new DoubleField("price", dealModel.getPrice(), Field.Store.YES));
        return document;
    }
```
```java;gutter
IndexWriter writer = new IndexWriter(directory, config);
List<Document> documents = new ArrayList<Document>();
documents.add(createDealDocument(dealModel2));
documents.add(createDealDocument(dealModel3));
documents.add(createPoiDocument(poiMsg1));
writer.addDocument(documents);
documents.clear();
documents.add(createDealDocument(dealModel1));
documents.add(createPoiDocument(poiMsg2));
writer.addDocument(documents);
```
**c）查询**
```java;gutter
Filter poiFilter = new CachingWrapperFilter(new QueryWrapperFilter(new TermQuery(new Term(PoiLuceneField.ATTR_DOCTYPE, "poi")))); //筛选出poi
ToParentBlockJoinQuery query = new ToParentBlockJoinQuery(dealQuery, poiFilter, ScoreMode.Max);
ToParentBlockJoinCollector collector = new ToParentBlockJoinCollector(
                    sort, // sort
                    (getOffset() + getLimit()),             // poi分页numHits
                    true,           // trackScores
                    false           // trackMaxScore
            );
collector = (ToParentBlockJoinCollector) indexSearcher.search(query, collector);
Sort childSort = new Sort(new SortField(DealLuceneField.ATTR_PRICE, SortField.Type.DOUBLE));
TopGroups hits = collector.getTopGroups(
                    query.getToParentBlockJoinQuery(),
                    childSort,
                    query.getOffset(),   // parent doc offset
                    100,  // maxDocsPerGroup
                    0,   // withinGroupOffset
                    true // fillSortFields
            );
```
## 3 实践
     官方文档显示index time join效率更高，比query time join快30%以上。因此我们在项目中使用了index time join方式，目前服务运行良好。
**检索实践文章系列：**
**[lucene字典实现原理](http://www.cnblogs.com/LBSer/p/4119841.html)**
**[lucene索引文件大小优化小结](http://www.cnblogs.com/LBSer/p/4068864.html)**
**[](http://www.cnblogs.com/LBSer/p/4068864.html)[排序学习实践](http://www.cnblogs.com/LBSer/p/4439542.html)**
**[](http://www.cnblogs.com/LBSer/p/4439542.html)[lucene如何通过docId快速查找field字段以及最近距离等信息？](http://www.cnblogs.com/LBSer/p/4419052.html)**

