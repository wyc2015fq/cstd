# ElasticSearch Java api 详解_V1.0 - sxf_123456的博客 - CSDN博客
2017年08月13日 16:26:40[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：163
Search部分:
查询有两种方法:[query
 Java API](http://www.elasticsearch.org/guide/en/elasticsearch/client/java-api/0.90/query-dsl-queries.html)和[filter
 Java API](http://www.elasticsearch.org/guide/en/elasticsearch/client/java-api/0.90/query-dsl-filters.html)
首先看一个例子:
```
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.search.SearchType;
import org.elasticsearch.index.query.FilterBuilders.*;
import org.elasticsearch.index.query.QueryBuilders.*;
SearchResponse response = client.prepareSearch("index1", "index2")
         //设置要查询的索引(index)
        .setTypes("type1", "type2")
         //设置type, 这个在建立索引的时候同时设置了, 或者可以使用head工具查看
        .setSearchType(SearchType.DFS_QUERY_THEN_FETCH)
        .setQuery(QueryBuilders.termQuery("multi", "test"))             
        // Query 查询之一. 对于QueryBuilders接下来会解释. 在这里"multi"是要查询的field,"test"是要查询的内容
        .setFilter(FilterBuilders.rangeFilter("age").from(12).to(18))   
        // Filter 查询之二, FilterBuilders接下来会解释. 在这里"age"是要查询的field, 后面的数字是查询的条件范围
        .setFrom(0).setSize(60).setExplain(true)
        //0-60 表示显示数量
        .execute()
        .actionGet();
```
提示:以上所有参数都是可选的. 
最小的查询如下所示:
```
// MatchAll on the whole cluster with all default options
SearchResponse response = client.prepareSearch().execute().actionGet();
```
1.[query
 Java API](http://www.elasticsearch.org/guide/en/elasticsearch/client/java-api/0.90/query-dsl-queries.html)[](http://www.elasticsearch.org/guide/en/elasticsearch/client/java-api/0.90/query-dsl-queries.html)
为了使用QueryBuilders必须先import下面的类:
```
import org.elasticsearch.index.query.QueryBuilders.*;
```
第一种:[Match
 Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-match-query.html)(链接内有详细解释)
```
QueryBuilder qb = QueryBuilders.matchQuery("name", "kimchy elasticsearch");
//name是field,kimchy elasticsearch是要查询的字符串
```
第二种:[MultiMatch Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-multi-match-query.html) (链接内有详细解释)
```
QueryBuilder qb = QueryBuilders.multiMatchQuery(
    "kimchy elasticsearch",     // Text you are looking for
     //kimchy elasticsearch是要查询的字符串
    "user", "message"           // Fields you query on
     //user 和 message都是field
    );
```
第三种: [Boolean Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-bool-query.html) (链接内有详细解释)
```
QueryBuilder qb = QueryBuilders
                    .boolQuery()
                    .must(termQuery("content", "test1"))
                    .must(termQuery("content", "test4"))
                    .mustNot(termQuery("content", "test2"))
                    .should(termQuery("content", "test3"));
//content为field,test为查询内容.
//其中must表示必须满足,mustNot表示必须不满足,should表示可有可无
```
如果bool查询语句中不存在must,则必须至少有一个should查询,同时可以通过`[minimum_should_match](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-minimum-should-match.html)参数来设置至少需要满足的should个数.`
[Boosting Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-boosting-query.html)
略...
[IDs Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-ids-query.html)
```
QueryBuilders.idsQuery().ids("1", "2");
//通过id来查询
```
[Custom Score Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-custom-score-query.html)
略...
[Custom Boost Factor Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-custom-boost-factor-query.html)
略...
[Constant Score Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-constant-score-query.html)
略...
[Disjunction Max Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-dis-max-query.html)
略...
[Field Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-field-query.html)
```
QueryBuilders.fieldQuery("name", "+kimchy -dadoonet");
// Note that you can write the same query using queryString query.
QueryBuilders.queryString("+kimchy -dadoonet").field("name");
//+表示必须有的 -表示一定没有的
```
[Fuzzy Like This Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-flt-query.html) * [Fuzzy
 Like This Field Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-flt-field-query.html)
略....不懂.....
[Fuzzy Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-fuzzy-query.html)
```
QueryBuilder qb = QueryBuilders.fuzzyQuery("name", "kimzhy");
//根据提供的字符串作为前缀进行查询
```
[Has Child Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-has-child-query.html) * [Has
 Parent](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-has-parent-query.html)
略....不懂.....
[Match All Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-match-all-query.html)
```
QueryBuilder qb = QueryBuilders.matchAllQuery();
//显示所有的记录
```
* [More Like This Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-mlt-query.html) * [More
 Like This Field Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-mlt-field-query.html)
略...不懂......
[Prefix Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-prefix-query.html)
```
QueryBuilders.prefixQuery("brand", "heine");
//brand是field, heine是前缀查询字符串
```
[QueryString Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-query-string-query.html)
```
QueryBuilder qb = QueryBuilders.queryString("+kimchy -elasticsearch");
```
略....不懂.....
[Range Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-range-query.html)
```
QueryBuilder qb = QueryBuilders
                    .rangeQuery("price")
                    .from(5)
                    .to(10)
                    .includeLower(true)
                    .includeUpper(false);
//price是field, 数字代表范围.除了from to还有lt gt等等...
```
* [Span First Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-span-first-query.html) * [Span
 Near Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-span-near-query.html) * [Span Not Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-span-not-query.html) * [Span
 Or Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-span-or-query.html) * [Span Term Quer](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-span-term-query.html)// Span
 First
```
QueryBuilders.spanFirstQuery(
                QueryBuilders.spanTermQuery("user", "kimchy"),  // Query
                3                                               // Max End position
        );
//只匹配在Field开头出现的, 数字表示从开头起的几个单词内查询, 则此查询意思是:user中的开头3个单词内,kimchy是否能查询到
// Span Near
QueryBuilders.spanNearQuery()
    .clause(QueryBuilders.spanTermQuery("field","value1"))  // Span Term Queries
    .clause(QueryBuilders.spanTermQuery("field","value2"))
    .clause(QueryBuilders.spanTermQuery("field","value3"))
    .slop(12)                                               // Slop factor
    .inOrder(false)
    .collectPayloads(false);
//匹配距离相近的, inOrder表示:true表示按照value1/2/3的顺序来计算距离(两个单词中间夹一个单词 距离为1). 举例: a b c d.  {a,b,c}中a,c的距离为1.
//则此查询的意思是:在field中,如果value1/2/3的距离是在12以内的,那么可以查询到.反之不能.
// Span Not
QueryBuilders.spanNotQuery()
    .include(QueryBuilders.spanTermQuery("field","value1"))
    .exclude(QueryBuilders.spanTermQuery("field","value2"));
//匹配不重叠的,即除那些两个SpanQuery相互重叠的结果
//例如有两个Document：”the quick brown fox jump over a lazy dog”和”the quick red fox jumps over the sleepy cat”
//SpanNearQuery quick_fox =new SpanNearQuery(new SpanQuery[]{quick, fox}, 1, true);  //基础的SpanQuery，默认匹配2个Document
//匹配的这两个结果的中间重叠部分为：red/brown
//SpanNotQuery quick_fox_dog = new SpanNotQuery(quick_fox, dog);//因为dog不是red或者brown，所以仍为两个
//SpanNotQuery no_quick_red_fox =      new SpanNotQuery(quick_fox, red);   //现在只剩下brown那个doc了
//这种Query经常用于连接的时候。
// Span Or
QueryBuilders.spanOrQuery()
    .clause(QueryBuilders.spanTermQuery("field","value1"))
    .clause(QueryBuilders.spanTermQuery("field","value2"))
    .clause(QueryBuilders.spanTermQuery("field","value3"));
//SpanQuery的合集
// Span Term
QueryBuilders.spanTermQuery("user","kimchy");
//该部分详见:http://www.coder4.com/archives/774
```
[Term Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-term-query.html)
略...
[Terms Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-terms-query.html)
略...
[Top Children Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-top-children-query.html)
略...
[Wildcard Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-wildcard-query.html)
略...
[Nested Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-nested-query.html)
略...
[Custom Filters Score Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-custom-filters-score-query.html)
略...
[Indices Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-indices-query.html)
略...
[GeoShape Query](http://www.elasticsearch.org/guide/en/elasticsearch/reference/0.90/query-dsl-geo-shape-query.html)
略...
2.[filter
 Java API](http://www.elasticsearch.org/guide/en/elasticsearch/client/java-api/0.90/query-dsl-filters.html)
附上一个自己写的小型Test, 简单易懂.
```
1 import java.io.IOException;
  2 
  3 import org.elasticsearch.action.index.IndexResponse;
  4 import org.elasticsearch.action.search.SearchResponse;
  5 import org.elasticsearch.action.search.SearchType;
  6 import org.elasticsearch.client.Client;
  7 import org.elasticsearch.client.transport.TransportClient;
  8 import org.elasticsearch.common.transport.InetSocketTransportAddress;
  9 import org.elasticsearch.common.xcontent.XContentBuilder;
 10 import org.elasticsearch.common.xcontent.XContentFactory;
 11 import org.elasticsearch.index.query.FilterBuilders;
 12 import org.elasticsearch.index.query.QueryBuilder;
 13 import org.elasticsearch.index.query.QueryBuilders;
 14 import org.elasticsearch.search.SearchHits;
 15 
 16 import user.User;
 17 
 18 
 19 public class First_ES_Test {
 20 
 21     private Client client;
 22     
 23     public void init()
 24     {
 25         //ip可以在http://localhost:9200/_plugin/head/中自行查询
 26         client = new TransportClient().addTransportAddress(new InetSocketTransportAddress("192.168.1.103", 9300));
 27     }
 28     
 29     public void close()
 30     {
 31         client.close();
 32     }
 33     
 34     /*
 35      * create index
 36      **/
 37     public void createIndex() {
 38         for (int i = 0; i < 1000; i++) {
 39             User user = new User();
 40             user.setId(new Long(i));
 41             user.setName("xxx0624 " + i);
 42             user.setAge(i % 100);
 43             System.out.println("ok:"+i);
 44             client.prepareIndex("users", "user").setSource(generateJson(user))
 45                     .execute().actionGet();
 46         }
 47     }
 48  
 49     /*
 50      * 转换成json对象
 51      *
 52      * @param user
 53      * @return json(String)
 54      **/
 55     private String generateJson(User user) {
 56         String json = "";
 57         try {
 58             XContentBuilder contentBuilder = XContentFactory.jsonBuilder()
 59                     .startObject();
 60             contentBuilder.field("id", user.getId() + "");
 61             contentBuilder.field("name", user.getName());
 62             contentBuilder.field("age", user.getAge() + "");
 63             json = contentBuilder.endObject().string();
 64         } catch (IOException e) {
 65             e.printStackTrace();
 66         }
 67         return json;
 68     }
 69     
 70     public void search() {
 71         QueryBuilder qb = QueryBuilders
 72                 .boolQuery()
 73                 .must(QueryBuilders.termQuery("age", "0"))
 74                 .should(QueryBuilders.termQuery("id", "0"))
 75                 //.mustNot(QueryBuilders.termQuery("content", "test2"))
 76                 //.should(QueryBuilders.termQuery("content", "test3"))
 77                 ;
 78         
 79         SearchResponse response = client.prepareSearch("users")
 80                 .setTypes("user")
 81                 .setSearchType(SearchType.DFS_QUERY_THEN_FETCH)
 82                 .setQuery(qb) // Query
 83                 //.setFilter(FilterBuilders.rangeFilter("age").from(0).to(100)) // Filter
 84                 .setFrom(0).setSize(100).setExplain(true)
 85                 .execute().actionGet();
 86         SearchHits hits = response.getHits();
 87         System.out.println(hits.getTotalHits());
 88         for (int i = 0; i < hits.getHits().length; i++) {
 89             System.out.println(hits.getHits()[i].getSourceAsString());
 90         }
 91     }    
 92  
 93     public static void main(String[] args) {
 94         
 95         First_ES_Test client = new First_ES_Test();
 96         client.init();
 97         client.createIndex();
 98         client.search();
 99         client.close();
100 
101     }
102     
103     public void print(String output)
104     {
105         System.out.print(output);
106     }
107     public void println(String output)
108     {
109         System.out.println(output);
110     }
111 
112 }
View Code
```
