# Elasticsearch 之java操作es - z69183787的专栏 - CSDN博客
2018年08月22日 12:43:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：232
[https://blog.csdn.net/ty4315/article/details/52434296](https://blog.csdn.net/ty4315/article/details/52434296)
Java操作es集群步骤1：配置集群对象信息；2：创建客户端；3：查看集群信息
1：集群名称
      默认集群名为elasticsearch，如果集群名称和指定的不一致则在使用节点资源时会报错。
2：嗅探功能
       通过client.transport.sniff启动嗅探功能，这样只需要指定集群中的某一个节点(不一定是主节点)，然后会加载集群中的其他节点，这样只要程序不停即使此节点宕机仍然可以连接到其他节点。
3：查询类型SearchType.QUERY_THEN_FETCH
      Es中一共有四种查询类型。
      QUERY_AND_FETCH：
      主节点将查询请求分发到所有的分片中，各个分片按照自己的查询规则即词频文档频率进行打分排序，然后将结果返回给主节点，主节点对所有数据进行汇总排序然后再返回给客户端，此种方式只需要和es交互一次。
      这种查询方式存在数据量和排序问题，主节点会汇总所有分片返回的数据这样数据量会比较大，二是各个分片上的规则可能不一致。
QUERY_THEN_FETCH：
      主节点将请求分发给所有分片，各个分片打分排序后将数据的id和分值返回给主节点，主节点收到后进行汇总排序再根据排序后的id到对应的节点读取对应的数据再返回给客户端，此种方式需要和es交互两次。
      这种方式解决了数据量问题但是排序问题依然存在而且是es的默认查询方式。
DFS_QUERY_AND_FETCH和DFS_QUERY_THEN_FETCH：
      这两种方式和前面两种的区别在于将各个分片的规则统一起来进行打分。解决了排序问题但是DFS_QUERY_AND_FETCH仍然存在数据量问题，DFS_QUERY_THEN_FETCH两种噢乖你问题都解决但是效率是最差的。
特点：
     一个交互两次，一个交互一次；一个统一打分规则一个不统一；一个分片返回详细数据一个分片返回id。
4：分页压力
       我们通过curl和java查询时都可以指定分页，但是页数越往后服务器的压力会越大。大多数搜索引擎都不会提供非常大的页数搜索，原因有两个一是用户习惯一般不会看页数大的搜索结果因为越往后越不准确，二是服务器压力。
       比如分片是5分页单位是10查询第10000到10010条记录，es需要在所有分片上进行查询，每个分片会产生10010条排序后的数据然后返回给主节点，主节点接收5个分片的数据一共是50050条然后再进行汇总最后再取其中的10000到10010条数据返回给客户端，这样一来看似只请求了10条数据但实际上es要汇总5万多条数据，所以页码越大服务器的压力就越大。
5：超时timeout
       查询时如果数据量很大，可以指定超时时间即到达此时间后无论查询的结果是什么都会返回并且关闭连接，这样用户体验较好缺点是查询出的数据可能不完整，Java和curl都可以指定超时时间。
6：maven依赖
```
<dependency>
			<groupId>org.elasticsearch</groupId>
			<artifactId>elasticsearch</artifactId>
			<version>1.4.4</version>
		</dependency>  
        <dependency>
			<groupId>com.fasterxml.jackson.core</groupId>
			<artifactId>jackson-databind</artifactId>
			<version>2.1.3</version>
		</dependency>
```
java代码
```java
package elasticsearch;
 
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;
 
import online.elasticsearch.bean.Student;
 
import org.elasticsearch.ElasticsearchException;
import org.elasticsearch.action.bulk.BulkItemResponse;
import org.elasticsearch.action.bulk.BulkRequestBuilder;
import org.elasticsearch.action.bulk.BulkResponse;
import org.elasticsearch.action.delete.DeleteRequest;
import org.elasticsearch.action.delete.DeleteResponse;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexRequest;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.search.SearchType;
import org.elasticsearch.action.update.UpdateRequest;
import org.elasticsearch.action.update.UpdateResponse;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.cluster.node.DiscoveryNode;
import org.elasticsearch.common.collect.ImmutableList;
import org.elasticsearch.common.settings.ImmutableSettings;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.text.Text;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.common.transport.TransportAddress;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.elasticsearch.common.xcontent.XContentFactory;
import org.elasticsearch.index.query.FilterBuilders;
import org.elasticsearch.index.query.MatchQueryBuilder.Operator;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.aggregations.Aggregation;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.bucket.terms.Terms;
import org.elasticsearch.search.aggregations.bucket.terms.Terms.Bucket;
import org.elasticsearch.search.aggregations.metrics.sum.Sum;
import org.elasticsearch.search.highlight.HighlightField;
import org.elasticsearch.search.sort.SortOrder;
import org.junit.Before;
import org.junit.Test;
 
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
 
public class elastaicTest {
 
	TransportClient transportClient;
	//索引库名
	String index = "shb01";
	//类型名称
	String type = "stu";
	
	@Before
	public void before()
	{
		/**
		 * 1:通过 setting对象来指定集群配置信息
		 */
		Settings setting = ImmutableSettings.settingsBuilder()
			.put("cluster.name", "shb01")//指定集群名称
			.put("client.transport.sniff", true)//启动嗅探功能
			.build();
		
		/**
		 * 2：创建客户端
		 * 通过setting来创建，若不指定则默认链接的集群名为elasticsearch
		 * 链接使用tcp协议即9300
		 */
		transportClient = new TransportClient(setting);                        
		TransportAddress transportAddress = new InetSocketTransportAddress("192.168.79.131", 9300);
		transportClient.addTransportAddresses(transportAddress);
		
		/**
		 * 3：查看集群信息
		 * 注意我的集群结构是：
		 *   131的elasticsearch.yml中指定为主节点不能存储数据，
		 *   128的elasticsearch.yml中指定不为主节点只能存储数据。
		 * 所有控制台只打印了192.168.79.128,只能获取数据节点
		 * 
		 */
	    ImmutableList<DiscoveryNode> connectedNodes = transportClient.connectedNodes();
	    for(DiscoveryNode node : connectedNodes)
	    {
	    	System.out.println(node.getHostAddress());
	    }
	    
	}
	
	/**
	 * 通过prepareGet方法获取指定文档信息
	 */
	@Test
	public void testGet() {
		GetResponse getResponse = transportClient.prepareGet(index, type, "1").get();
		System.out.println(getResponse.getSourceAsString());
	}
	
	/**
	 * prepareUpdate更新索引库中文档，如果文档不存在则会报错
	 * @throws IOException
	 * 
	 */
	@Test
	public void testUpdate() throws IOException
	{
		XContentBuilder source = XContentFactory.jsonBuilder()
			.startObject()
			.field("name", "will")
			.endObject();
		
		UpdateResponse updateResponse = transportClient
				.prepareUpdate(index, type, "6").setDoc(source).get();
		
		System.out.println(updateResponse.getVersion());
	}
 
	/**
	 * 通过prepareIndex增加文档，参数为json字符串
	 */
	@Test
	public void testIndexJson()
	{
		String source = "{\"name\":\"will\",\"age\":18}";
		IndexResponse indexResponse = transportClient
				.prepareIndex(index, type, "3").setSource(source).get();
		System.out.println(indexResponse.getVersion());
	}
	
	/**
	 * 通过prepareIndex增加文档，参数为Map<String,Object>
	 */
	@Test
	public void testIndexMap()
	{
		Map<String, Object> source = new HashMap<String, Object>(2);
		source.put("name", "Alice");
		source.put("age", 16);
		IndexResponse indexResponse = transportClient
				.prepareIndex(index, type, "4").setSource(source).get();
		System.out.println(indexResponse.getVersion());
	}
	
	/**
	 * 通过prepareIndex增加文档，参数为javaBean
	 * 
	 * @throws ElasticsearchException
	 * @throws JsonProcessingException
	 */
	@Test
	public void testIndexBean() throws ElasticsearchException, JsonProcessingException
	{
		Student stu = new Student();
		stu.setName("Fresh");
		stu.setAge(22);
		
		ObjectMapper mapper = new ObjectMapper();
		IndexResponse indexResponse = transportClient
				.prepareIndex(index, type, "5").setSource(mapper.writeValueAsString(stu)).get();
		System.out.println(indexResponse.getVersion());
	}
	
	/**
	 * 通过prepareIndex增加文档，参数为XContentBuilder
	 * 
	 * @throws IOException
	 * @throws InterruptedException
	 * @throws ExecutionException
	 */
	@Test
	public void testIndexXContentBuilder() throws IOException, InterruptedException, ExecutionException
	{
		XContentBuilder builder = XContentFactory.jsonBuilder()
				.startObject()
				.field("name", "Avivi")
				.field("age", 30)
				.endObject();
		IndexResponse indexResponse = transportClient
				.prepareIndex(index, type, "6")
				.setSource(builder)
				.execute().get();
		//.execute().get();和get()效果一样
		System.out.println(indexResponse.getVersion());
	}
	
	/**
	 * 通过prepareDelete删除文档
	 * 
	 */
	@Test
	public void testDelete()
	{
		String id = "9";
		DeleteResponse deleteResponse = transportClient.prepareDelete(index,
				type, id).get();
		
		System.out.println(deleteResponse.getVersion());
		
		//删除所有记录
		transportClient.prepareDeleteByQuery(index).setTypes(type)
				.setQuery(QueryBuilders.matchAllQuery()).get();
	}
	
	/**
	 * 删除索引库，不可逆慎用
	 */
	@Test
	public void testDeleteeIndex()
	{
		transportClient.admin().indices().prepareDelete("shb01","shb02").get();
	}
	
	/**
	 * 求索引库文档总数
	 */
	@Test
	public void testCount()
	{
		long count = transportClient.prepareCount(index).get().getCount();
		System.out.println(count);
	}
	
	/**
	 * 通过prepareBulk执行批处理
	 * 
	 * @throws IOException 
	 */
	@Test
	public void testBulk() throws IOException
	{
		//1:生成bulk
		BulkRequestBuilder bulk = transportClient.prepareBulk();
		
		//2:新增
		IndexRequest add = new IndexRequest(index, type, "10");
		add.source(XContentFactory.jsonBuilder()
					.startObject()
					.field("name", "Henrry").field("age", 30)
					.endObject());
		
		//3:删除
		DeleteRequest del = new DeleteRequest(index, type, "1");
		
		//4:修改
		XContentBuilder source = XContentFactory.jsonBuilder().startObject().field("name", "jack_1").field("age", 19).endObject();
		UpdateRequest update = new UpdateRequest(index, type, "2");
		update.doc(source);
		
		bulk.add(del);
		bulk.add(add);
		bulk.add(update);
		//5:执行批处理
		BulkResponse bulkResponse = bulk.get();
		if(bulkResponse.hasFailures())
		{
			BulkItemResponse[] items = bulkResponse.getItems();
			for(BulkItemResponse item : items)
			{
				System.out.println(item.getFailureMessage());
			}
		}
		else
		{
			System.out.println("全部执行成功！");
		}
	}
	
	/**
	 * 通过prepareSearch查询索引库
	 * setQuery(QueryBuilders.matchQuery("name", "jack"))
	 * setSearchType(SearchType.QUERY_THEN_FETCH)
	 * 
	 */
	@Test
	public void testSearch()
	{
		SearchResponse searchResponse = transportClient.prepareSearch(index)
				.setTypes(type)
				.setQuery(QueryBuilders.matchAllQuery()) //查询所有
				//.setQuery(QueryBuilders.matchQuery("name", "tom").operator(Operator.AND)) //根据tom分词查询name,默认or
				//.setQuery(QueryBuilders.multiMatchQuery("tom", "name", "age")) //指定查询的字段
				//.setQuery(QueryBuilders.queryString("name:to* AND age:[0 TO 19]")) //根据条件查询,支持通配符大于等于0小于等于19
				//.setQuery(QueryBuilders.termQuery("name", "tom"))//查询时不分词
				.setSearchType(SearchType.QUERY_THEN_FETCH)
				.setFrom(0).setSize(10)//分页
				.addSort("age", SortOrder.DESC)//排序
				.get();
		
		SearchHits hits = searchResponse.getHits();
		long total = hits.getTotalHits();
		System.out.println(total);
		SearchHit[] searchHits = hits.hits();
		for(SearchHit s : searchHits)
		{
			System.out.println(s.getSourceAsString());
		}
	}
	
	/**
	 * 多索引，多类型查询
	 * timeout
	 */
	@Test
	public void testSearchsAndTimeout()
	{
		SearchResponse searchResponse = transportClient.prepareSearch("shb01","shb02").setTypes("stu","tea")
			.setQuery(QueryBuilders.matchAllQuery())
			.setSearchType(SearchType.QUERY_THEN_FETCH)
			.setTimeout("3")
		    .get();
		
		SearchHits hits = searchResponse.getHits();
        long totalHits = hits.getTotalHits();
        System.out.println(totalHits);
        SearchHit[] hits2 = hits.getHits();
        for(SearchHit h : hits2)
        {
        	System.out.println(h.getSourceAsString());
        }
	}
	
	/**
	 * 过滤，
	 * lt 小于
	 * gt 大于
	 * lte 小于等于
	 * gte 大于等于
	 * 
	 */
	@Test
	public void testFilter()
	{
		SearchResponse searchResponse = transportClient.prepareSearch(index)
				.setTypes(type)
				.setQuery(QueryBuilders.matchAllQuery()) //查询所有
				.setSearchType(SearchType.QUERY_THEN_FETCH)
//				.setPostFilter(FilterBuilders.rangeFilter("age").from(0).to(19)
//						.includeLower(true).includeUpper(true))
				.setPostFilter(FilterBuilders.rangeFilter("age").gte(18).lte(22))
				.setExplain(true) //explain为true表示根据数据相关度排序，和关键字匹配最高的排在前面
				.get();
	
		
		SearchHits hits = searchResponse.getHits();
		long total = hits.getTotalHits();
		System.out.println(total);
		SearchHit[] searchHits = hits.hits();
		for(SearchHit s : searchHits)
		{
			System.out.println(s.getSourceAsString());
		}
	}
	
	/**
	 * 高亮
	 */
	@Test
	public void testHighLight()
	{
		SearchResponse searchResponse = transportClient.prepareSearch(index)
				.setTypes(type)
				//.setQuery(QueryBuilders.matchQuery("name", "Fresh")) //查询所有
				.setQuery(QueryBuilders.queryString("name:F*"))
				.setSearchType(SearchType.QUERY_THEN_FETCH)
				.addHighlightedField("name")
				.setHighlighterPreTags("<font color='red'>")
				.setHighlighterPostTags("</font>")
				.get();
	
		
		SearchHits hits = searchResponse.getHits();
		System.out.println("sum:" + hits.getTotalHits());
		
		SearchHit[] hits2 = hits.getHits();
		for(SearchHit s : hits2)
		{
			Map<String, HighlightField> highlightFields = s.getHighlightFields();
			HighlightField highlightField = highlightFields.get("name");
			if(null != highlightField)
			{
				Text[] fragments = highlightField.fragments();
				System.out.println(fragments[0]);
			}
			System.out.println(s.getSourceAsString());
		}
	}
	
	/**
	 * 分组
	 */
	@Test
	public void testGroupBy()
	{
		SearchResponse searchResponse = transportClient.prepareSearch(index).setTypes(type)
				.setQuery(QueryBuilders.matchAllQuery())
				.setSearchType(SearchType.QUERY_THEN_FETCH)
				.addAggregation(AggregationBuilders.terms("group_age")
						.field("age").size(0))//根据age分组，默认返回10，size(0)也是10
				.get();
		
		Terms terms = searchResponse.getAggregations().get("group_age");
		List<Bucket> buckets = terms.getBuckets();
		for(Bucket bt : buckets)
		{
			System.out.println(bt.getKey() + " " + bt.getDocCount());
		}
	}
	
	/**
	 * 聚合函数,本例之编写了sum，其他的聚合函数也可以实现
	 * 
	 */
	@Test
	public void testMethod()
	{
		SearchResponse searchResponse = transportClient.prepareSearch(index).setTypes(type)
				.setQuery(QueryBuilders.matchAllQuery())
				.setSearchType(SearchType.QUERY_THEN_FETCH)
				.addAggregation(AggregationBuilders.terms("group_name").field("name")
						.subAggregation(AggregationBuilders.sum("sum_age").field("age")))
				.get();
		
		Terms terms = searchResponse.getAggregations().get("group_name");
		List<Bucket> buckets = terms.getBuckets();
		for(Bucket bt : buckets)
		{
			Sum sum = bt.getAggregations().get("sum_age");
			System.out.println(bt.getKey() + "  " + bt.getDocCount() + " "+ sum.getValue());
		}
		
	}
	
	
	
}
```
