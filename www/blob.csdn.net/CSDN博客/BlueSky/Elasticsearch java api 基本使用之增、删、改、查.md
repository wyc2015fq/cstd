# Elasticsearch java api 基本使用之增、删、改、查 - BlueSky - CSDN博客
2015年12月03日 19:20:47[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：13768
主要参考el的java官方文档：https://www.elastic.co/guide/en/elasticsearch/client/java-api/1.7/generate.html
一篇博客：http://www.cnblogs.com/huangfox/p/3543134.html
Elasticsearch官方指南：http://es.xiaoleilu.com/010_Intro/30_Tutorial_Search.html
@[xuguokun1986](http://my.csdn.net/xuguokun1986)的代码
## 主要概念
明白如下几个名词，就像上一篇中提到的那样，索引（indices）->数据库、类型（types）->表、文档（documents）->行、字段（Fields）->列；其中->后面代表的是基本的关系型数据库的表。
基本的依赖的jar包，在elasticsearch文件夹下对应的lib的文件夹中含有；其中json的文件依赖jar包的下载地址http://wiki.fasterxml.com/JacksonDownload。
## 遇到的问题及解决方法
client = new TransportClient().addTransportAddress(new InetSocketTransportAddress(“192.168.203.148”, 9200));
报错No node available
解决方法：这里9200
 改写成9300
## 代码
### 创建index并插入数据
代码如下所示：
```
<pre name="code" class="html"><pre name="code" class="html">package com.asia.myTest;
import java.io.IOException;
import net.sf.json.JSONObject;
import org.elasticsearch.action.*;
import org.apache.lucene.search.TermQuery;
import org.codehaus.jackson.map.ObjectMapper;
import org.codehaus.jackson.map.util.JSONPObject;
import org.elasticsearch.ElasticsearchException;
import org.elasticsearch.action.count.CountResponse;
import org.elasticsearch.action.delete.DeleteResponse;
import org.elasticsearch.action.get.GetResponse;
import org.elasticsearch.action.index.IndexRequestBuilder;
import org.elasticsearch.action.index.IndexResponse;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.search.SearchType;
import org.elasticsearch.client.Client;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.transport.InetSocketTransportAddress;
import org.elasticsearch.common.xcontent.XContentFactory;
import org.elasticsearch.index.query.FilterBuilders;
import org.elasticsearch.index.query.QueryBuilder;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.index.query.QueryBuilders.*;
import static org.elasticsearch.index.query.FilterBuilders.*;
public class ESClient {
	
	private Client client;
	
	public void init(){
		//on start相当于连接集群
		client = new TransportClient().
				 addTransportAddress(new InetSocketTransportAddress("192.168.203.148", 9300));
	}	
	
	public void close(){
		//on shutdown 断开集群
		client.close();
	}
    
	/*
     *创建index，把其中的文档转化为json的格式存储
    */
	public void createIndex() {
		for (int i=0; i<=200;i++){
			IndexResponse indexResponse = null;
			try {
				indexResponse = client.prepareIndex("logs", "log2015",i+"")
						                            .setSource(
						                            XContentFactory.jsonBuilder().startObject()
						                                .field("sourceIp" , "10.10.16."+i)
						                                .field("sourcePort" , 389)
						                                .field("destIp" , "114.114.114.114")
						                                .endObject())
						                                .execute()
						                                .actionGet();
			} catch (ElasticsearchException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println("responseIsCreated: "+indexResponse.isCreated());
		}
		System.out.println("it is ok ！");
	}
	
	/*
	 * Get index 获取文档相当于读取数据库的一行数据
	 */
	public void get(){
		GetResponse getresponse = client.prepareGet("logs", "log2015", "1")
									 .execute()
									 .actionGet();
		System.out.println(getresponse.getSourceAsString());
	}
	
	/*
	 *Delete index 删除文档，相当于删除一行数据
	 */
	public void delete(){
		DeleteResponse deleteresponse = client.prepareDelete("logs", "log2015","150")
											   .execute()
											   .actionGet();
		System.out.println(deleteresponse.getVersion());
	}
	
	/*
	 *search 查询相当于关系型数据库的查询 
	*/
	public void search(){
		SearchResponse searchresponse = client.prepareSearch("logs")
								  		.setTypes("log2015")
								  		.setSearchType(SearchType.DFS_QUERY_THEN_FETCH)
								  		.setQuery(QueryBuilders.termQuery("destIp", "114.114.114.114"))
								  		.setPostFilter(
								  				FilterBuilders.rangeFilter("sourceIp")
								  				.from("10.10.16.57")
								  				.to("10.10.16.68")
								  				)
								  		.setFrom(0)
								  		.setSize(3).setExplain(true)
								  		.execute().actionGet();
		
		System.out.println(searchresponse.toString()); 
//		JSONObject jsonObject = JSONObject.fromObject(searchresponse.toString());
//		JSONObject hites = (JSONObject) jsonObject.get("hits");
//		System.out.println(hites.get("hits").toString());
	}
	
	/*
	 *Count api 统计分析结果
	 */
	public void count(){
		CountResponse countresponse = client.prepareCount("website")
				.setQuery(QueryBuilders.termQuery("_type", "asia"))
				.execute()
				.actionGet();
	
		System.out.println(countresponse.getCount());
	
	}
	
	
	
    public static void main(String[] args){
    	ESClient client = new ESClient();
    	client.init();
    	//client.createIndex();
    	//client.get();
    	//client.delete();
    	//client.search();
    	client.count();
    	client.close();
    	
    	
    }
}
```
控制台运行过程：
.......
responseIsCreated: true
responseIsCreated: true
responseIsCreated: true
responseIsCreated: true
responseIsCreated: true
responseIsCreated: true
responseIsCreated: true
it is ok ！
通过CURL命令查看基本结果,插入数据成功：
![](https://img-blog.csdn.net/20151203143149196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
