# Java实现Slor实体bean数据的索引创建 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月26日 18:07:06[boonya](https://me.csdn.net/boonya)阅读数：2615








本文以查询商品Item为例进行讲解。


## 1.环境

Solr Version:solr-6.4.1

JDK： "1.8.0_121"

## 2.创建核心core 


使用命令：C:\DEVELOPERS\Apache Solr\solr-6.4.1\bin\`solr create -c item -d basic_configs`


```
![](https://img-blog.csdn.net/20170226175239162)
```


`在C:\DEVELOPERS\Apache Solr\solr-6.4.1\server\solr\item\conf文件夹下修改managed-schema文件在uniqueKey标签后添加如下字段：`

``

```
<field name="name" type="text_general" indexed="true" stored="true"/>
 <field name="price" type="tdouble" indexed="true" stored="true"/>
```





`修改完成后启动Solr，命令：C:\DEVELOPERS\Apache Solr\solr-6.4.1\bin\solr start`


```

```


## `3.Java代码及测试`

`工具类：`

``

```java
package com.boonya.solr.utils;

import java.io.IOException;
import org.apache.solr.client.solrj.SolrClient;
import org.apache.solr.client.solrj.SolrQuery;
import org.apache.solr.client.solrj.SolrServerException;
import org.apache.solr.client.solrj.impl.HttpSolrClient;
import org.apache.solr.client.solrj.response.QueryResponse;

/**
 * SolrJ接口类使用工具
 * 
 * @packge com.boonya.solr.utils.SolrUtils
 * @date 2017年2月25日 下午5:32:19
 * @author pengjunlin
 * @comment
 * @update
 */
public class SolrJUtils {

	public static final String solrBaseUrl = "http://localhost:8983/solr/";

	private static HttpSolrClient init(HttpSolrClient httpSolrClient)
			throws SolrServerException, IOException {
		httpSolrClient.setConnectionTimeout(100);
		httpSolrClient.setDefaultMaxConnectionsPerHost(100);
		httpSolrClient.setMaxTotalConnections(100);
		return httpSolrClient;
	}

	@SuppressWarnings("deprecation")
	public static SolrClient getClient() throws SolrServerException,
			IOException {
		HttpSolrClient httpSolrClient = new HttpSolrClient(solrBaseUrl);// early
																	// version
		init(httpSolrClient);
		return httpSolrClient;
	}

	@SuppressWarnings("deprecation")
	public static SolrClient getClient(String solrUrl)
			throws SolrServerException, IOException {
		HttpSolrClient httpSolrClient = new HttpSolrClient(solrUrl);// early
																	// version
		init(httpSolrClient);
		return httpSolrClient;
	}

	public static SolrClient buildClient() throws SolrServerException,
			IOException {

		return new HttpSolrClient.Builder(solrBaseUrl).build();// recently version
	}

	public static SolrClient buildClient(String solrUrl)
			throws SolrServerException, IOException {

		return new HttpSolrClient.Builder(solrUrl).build();// recently version
	}

	public static QueryResponse query(String keyword)
			throws SolrServerException, IOException {
		return SolrJUtils.buildClient().query(new SolrQuery(keyword));// keyword="*:*"
	}

	public static QueryResponse query(String core, String keyword)
			throws SolrServerException, IOException {
		return SolrJUtils.buildClient().query(new SolrQuery(core, keyword));// keyword="*:*"
	}

	public static QueryResponse query(SolrQuery solrQuery)
			throws SolrServerException, IOException {
		return SolrJUtils.buildClient().query(solrQuery);
	}

	public static QueryResponse query(String solrUrl, String core,
			String keyword) throws SolrServerException, IOException {
		return SolrJUtils.buildClient(solrUrl).query(
				new SolrQuery(core, keyword));// keyword="*:*"
	}

	public static QueryResponse query(String solrUrl, SolrQuery solrQuery)
			throws SolrServerException, IOException {
		return SolrJUtils.buildClient(solrUrl).query(solrQuery);
	}


}
```

Solr Item操作类：


``

```java
package com.boonya.solr.sample;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.Map;

import org.apache.solr.client.solrj.SolrClient;
import org.apache.solr.client.solrj.SolrQuery;
import org.apache.solr.client.solrj.SolrServerException;
import org.apache.solr.client.solrj.response.FacetField;
import org.apache.solr.client.solrj.response.FacetField.Count;
import org.apache.solr.client.solrj.response.QueryResponse;
import org.apache.solr.common.SolrInputDocument;

import com.boonya.solr.utils.SolrJUtils;
/**
 * 参考:http://blog.csdn.net/itbasketplayer/article/details/8086160
 * @packge com.boonya.solr.utils.ItemSolr
 * @date   2017年2月25日  下午6:47:45
 * @author pengjunlin
 * @comment   
 * @update
 */
public class SolrForItem {
	
	private static SolrClient solr =null;
	
	static{
		try {
			solr=SolrJUtils.buildClient(SolrJUtils.solrBaseUrl+"item");
		} catch (SolrServerException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	/**
	 * 验证ID是否为空
	 * 
	 * @MethodName: checkItem
	 * @Description:
	 * @param item
	 * @return
	 * @throws
	 */
	private boolean checkItem(Item item) {
		return item == null ? false : (item.getId()==0 ? false : true);
	}

	/**
	 * @throws IOException
	 * @throws SolrServerException
	 *             添加索引
	 * 
	 * @MethodName: addIndex
	 * @Description:
	 * @param list
	 * @throws
	 */
	public void addIndex(List<Item> list) throws SolrServerException,
			IOException {
		
		try {
			Collection<SolrInputDocument> docs = new ArrayList<SolrInputDocument>();
			for (int i = 0; i < list.size(); i++) {
				Item item = list.get(i);
				// 设置每个字段不得为空，可以在提交索引前进行检查
				if (checkItem(item)) {
					SolrInputDocument doc = new SolrInputDocument();
					// 在这里请注意date的格式，要进行适当的转化，上文已提到
					doc.addField("id", item.getId());
					doc.addField("name", item.getName());
					doc.addField("price", item.getPrice());
					docs.add(doc);
				}
			}
			solr.add(docs);
			// 使用数据创建bean对象列表
			// solr.addBeans(docs);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			// 对索引进行优化
			solr.optimize();

			solr.commit();
		}
	}

}
```

Junit测试：


``

```java
package com.boonya.solr.sample;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import org.apache.solr.client.solrj.SolrServerException;
import org.junit.Test;
/**
 * Item测试
 * @packge com.boonya.solr.sample.SolrForItemTest
 * @date   2017年2月26日  下午5:57:12
 * @author pengjunlin
 * @comment   
 * @update
 */
public class SolrForItemTest {
	
	private String [] products=new String []{"杜康酒","五粮液","江南春","茅台酒"};
	
	@Test
	public void testAddIndex(){
		SolrForItem solrForItem=new SolrForItem();
		List<Item> list=new ArrayList<Item>();
		Item item=null;
		for (int i = 0; i < products.length; i++) {
			double price=new Random(1000).nextDouble();
			item=new Item(i+1, products[i],price );
			list.add(item);
		}
		try {
			solrForItem.addIndex(list);// 添加索引
		} catch (SolrServerException | IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
```
运行此测试类，然后查看solr.



## `4.测试效果展示`


```

```



```
![](https://img-blog.csdn.net/20170226180054831)
```


`返回的JSON数据：`

``

```
{
  "responseHeader":{
    "status":0,
    "QTime":0,
    "params":{
      "q":"*:*",
      "indent":"on",
      "name":"\"酒\"",
      "wt":"json"}},
  "response":{"numFound":4,"start":0,"docs":[
      {
        "id":"1",
        "name":["杜康酒"],
        "price":0.7101849056320707,
        "_version_":1560388329037365248},
      {
        "id":"2",
        "name":["五粮液"],
        "price":0.7101849056320707,
        "_version_":1560388329083502592},
      {
        "id":"3",
        "name":["江南春"],
        "price":0.7101849056320707,
        "_version_":1560388329084551168},
      {
        "id":"4",
        "name":["茅台酒"],
        "price":0.7101849056320707,
        "_version_":1560388329084551169}]
  }}
```


## 5.关键字查询


q参数是关键字过滤字段：

![](https://img-blog.csdn.net/20170226181740884)

JSON数据：



```
{
  "responseHeader":{
    "status":0,
    "QTime":67,
    "params":{
      "q":"name:\"杜康\"",
      "indent":"on",
      "wt":"json",
      "_":"1488103922746"}},
  "response":{"numFound":1,"start":0,"docs":[
      {
        "id":"1",
        "name":["杜康酒"],
        "price":0.7101849056320707,
        "_version_":1560388329037365248}]
  }}
```

Github代码：[https://github.com/SunflowersOfJava/BSolrs.git](https://github.com/SunflowersOfJava/BSolrs.git)







