# ElasticSearch之Java Api聚合分组实战 - BlueSky - CSDN博客
2016年05月11日 19:39:33[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：20651
最近有个日志收集监控的项目采用的技术栈是ELK+JAVA+Spring，客户端语言使用的是Java，以后有机会的话可以试一下JavaScript+Nodejs的方式，非常轻量级的组合，只不过不太适合服务化的工程，Kibana充当可视化层，功能虽然非常强大和灵活，但是需要业务人员懂Lucene的查询语法和Kibana的Dashboard仪表盘自定义功能才能玩的转，所以Kibana面向专业的开发人员和运维人员比较良好，但面向业务人员则稍微有点难度，我们这边就使用Java进行二次开发，然后前端定义几个业务人员关注的图表，然后把后端查询的数据，按照一定的维度放进去即可。 
基础环境： 
（1）ElasticSearch1.7.2 
（2）Logstash2.2.2 
（3）Kibana4.1.2 
（3）JDK7 
（4）Spring4.2 
使用到的技术点： 
（1）ElasticSearch的查询 
（2）ElasticSearch的过滤 
（3）ElasticSearch的日期聚合 
（4）ElasticSearch的Terms聚合 
（5）ElasticSearch的多级分组 
（6）ElasticSearch+Logstash的时区问题 
直接上代码： 
Java代码  ![收藏代码](http://qindongliang.iteye.com/images/icon_star.png)
- package cn.bizbook.product.elk.dao.impl;  
- 
- import cn.bizbook.product.elk.config.ESConf;  
- import cn.bizbook.product.elk.dao.ESDao;  
- import cn.bizbook.product.elk.utils.TimeTools;  
- import cn.bizbook.product.elk.vo.count.Condition;  
- import cn.bizbook.product.elk.vo.count.CountType;  
- import cn.bizbook.product.elk.vo.count.search.GroupCount;  
- import cn.bizbook.product.elk.vo.count.search.MonitorCount;  
- import org.apache.commons.lang.StringUtils;  
- import org.apache.lucene.queryparser.classic.QueryParserBase;  
- import org.elasticsearch.action.search.SearchRequestBuilder;  
- import org.elasticsearch.action.search.SearchResponse;  
- import org.elasticsearch.client.Client;  
- import org.elasticsearch.index.query.FilterBuilders;  
- import org.elasticsearch.index.query.QueryBuilder;  
- import org.elasticsearch.index.query.QueryBuilders;  
- import org.elasticsearch.index.query.QueryStringQueryBuilder;  
- import org.elasticsearch.search.aggregations.AggregationBuilders;  
- import org.elasticsearch.search.aggregations.Aggregations;  
- import org.elasticsearch.search.aggregations.bucket.histogram.DateHistogram;  
- import org.elasticsearch.search.aggregations.bucket.histogram.DateHistogramBuilder;  
- import org.elasticsearch.search.aggregations.bucket.histogram.Histogram;  
- import org.elasticsearch.search.aggregations.bucket.terms.StringTerms;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.stereotype.Repository;  
- 
- import javax.annotation.Resource;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- /**
-  * Created by qindongliang on 2016/4/6.
-  */
- @Repository("esDaoImpl")  
- publicclass ESDaoImpl implements ESDao {  
- 
- privatestatic Logger log= LoggerFactory.getLogger(ESDaoImpl.class);  
- @Autowired
- private ESConf esConf;  
- 
- @Resource(name = "client")  
- private  Client client;  
- 
- 
- @Override
- public MonitorCount count() {  
-         MonitorCount count=new MonitorCount();  
- //今天的数量
-         count.setToday_count(customCount(false,"*:*"));  
- //今天的入库量
-         count.setToday_store_count(customCount(false,"-save:1"));  
- //所有的总量
-         count.setTotal_count(customCount(true,"*:*"));  
- //所有的入库总量
-         count.setTotal_store_count(customCount(true,"-save:1"));  
- return count;  
-     }  
- 
- privatelong customCount(boolean isQueryAll, String queryString){  
- try {  
- //今天的开始时间 比如2016-04-01 00:00:00
- long today_start = TimeTools.getDayTimeStamp(0);  
- //今天的结束时间 也就是明天的开始时间 比如2016-04-02 00:00:00
- //一闭区间一开区间即得到一天的统计量
- long today_end=TimeTools.getDayTimeStamp(1);  
-             StringBuffer fq = new StringBuffer();  
-                      fq.append("@timestamp:")  
-                     .append(" [ ")  
-                     .append(today_start)  
-                     .append(" TO  ")  
-                     .append(today_end)  
-                     .append(" } ");  
- //构建查询请求，使用Lucene高级查询语法
-             QueryBuilder query=QueryBuilders.queryStringQuery(queryString);  
- //构建查询请求
-             SearchRequestBuilder search = client.prepareSearch("crawl*").setTypes("logs");  
- //非所有的情况下，设置日期过滤
- if(isQueryAll){  
-                 search.setQuery(query);//查询所有
-             }else {//加上日期过滤
-                 search.setQuery(QueryBuilders.filteredQuery(query, FilterBuilders.queryFilter(QueryBuilders.queryStringQuery(fq.toString()))));  
-             }  
-             SearchResponse r = search.get();//得到查询结果
- long hits = r.getHits().getTotalHits();//读取命中数量
- return hits;  
-         }catch (Exception e){  
-             log.error("统计日期数量出错！",e);  
-         }  
- return0;  
-     }  
- 
- 
- @Override
- public List<GroupCount> query(Condition condition) {  
- return grouyQuery(condition);  
-     }  
- 
- /***
-      * @param c 查询的条件
-      * @return 查询的结果
-      */
- private List<GroupCount> grouyQuery(Condition c){  
- //封装结果集
-         List<GroupCount> datas=new ArrayList<>();  
- //组装分组
-         DateHistogramBuilder dateAgg = AggregationBuilders.dateHistogram("dateagg");  
- //定义分组的日期字段
-         dateAgg.field("@timestamp");  
- //按天分组
- if(CountType.EACH_DAY==(c.getType())) {  
-             dateAgg.interval(DateHistogram.Interval.DAY);  
-             dateAgg.timeZone("+8:00");  
-             dateAgg.format("yyyy-MM-dd");  
- //按小时分组
-         }elseif(CountType.EACH_HOUR==c.getType()){  
-             dateAgg.interval(DateHistogram.Interval.HOUR);  
- //按小时分组，必须使用这个方法，不然得到的结果不正确
-             dateAgg.postZone("+8:00");  
-             dateAgg.format("yyyy-MM-dd HH");  
- //无效分组
-         }else{  
- thrownew NullPointerException("无效的枚举类型");  
-         }  
- //二级分组，统计入库的成功失败量 0 1 2 , 1为不成功
-         dateAgg.subAggregation(AggregationBuilders.terms("success").field("save"));  
- 
- //查询过滤条件
-         StringBuffer fq = new StringBuffer();  
- //过滤时间字段
-         fq.append(" +@timestamp:")  
-                 .append(" [ ")  
-                 .append(c.getStart().getTime())  
-                 .append(" TO  ")  
-                 .append(c.getEnd().getTime())  
-                 .append(" } ");  
- //过滤一级
- if(StringUtils.isNotEmpty(c.getT1())){  
-             fq.append(" +t1:").append(c.getT1());  
-         }  
- //过滤二级
- if(StringUtils.isNotEmpty(c.getT2())){  
-             fq.append(" +t2:").append(c.getT2());  
-         }  
- //过滤三级
- if(StringUtils.isNotEmpty(c.getT3())){  
-             fq.append(" +t3:").append(c.getT3());  
-         }  
- //过滤url
- if(StringUtils.isNotEmpty(c.getSourceUrl())){  
- //对url进行转义，防止查询出现错误
-             fq.append(" +url:").append(QueryParserBase.escape(c.getSourceUrl()));  
-         }  
- //过滤省份编码
- if(StringUtils.isNotEmpty(c.getProvinceCode())){  
-             fq.append(" +pcode:").append(c.getProvinceCode());  
-         }  
- //过滤入库状态
- if(c.getSavaState()!=null){  
-             fq.append(" +save:").append(c.getSavaState().getCode());  
-         }  
- //过滤http状态码
- if(c.getWebsiteState()!=null){  
- if(!c.getWebsiteState().getCode().equals("-1")) {  
-                 fq.append(" +httpcode:").append(c.getWebsiteState().getCode());  
-             }else{  
-                 fq.append(" -httpcode:").append("(0 110 200)");  
-             }  
-         }  
- //过滤配置configid
- if(StringUtils.isNotEmpty(c.getConfigId())){  
-             fq.append(" +cid:").append(c.getConfigId());  
-         }  
- 
- 
- 
- //查询索引
-         SearchRequestBuilder search=client.prepareSearch("crawl*").setTypes("logs");  
- //组装请求
-         search.setQuery(QueryBuilders.filteredQuery(QueryBuilders.matchAllQuery(),  
-                 FilterBuilders.queryFilter(QueryBuilders.queryStringQuery(fq.toString())  
-                         .defaultOperator(QueryStringQueryBuilder.Operator.AND)  
-                 ))).addAggregation(dateAgg);  
- //获取查询结果
-         SearchResponse r = search.get();//得到查询结果
- //获取一级聚合数据
-         Histogram h=r.getAggregations().get("dateagg");  
- //得到一级聚合结果里面的分桶集合
-         List<DateHistogram.Bucket> buckets = (List<DateHistogram.Bucket>) h.getBuckets();  
- //遍历分桶集
- for(DateHistogram.Bucket b:buckets){  
- //读取二级聚合数据集引用
-             Aggregations sub = b.getAggregations();  
- //获取二级聚合集合
-             StringTerms count = sub.get("success");  
-             GroupCount groupCount=new GroupCount();  
- //设置x轴分组日期
-             groupCount.setGroupKey(b.getKey());  
- //设置指定分组条件下入库总量
-             groupCount.setTotal_count(b.getDocCount());  
- //读取指定分组条件下不成功的数量
- long bad_count=count.getBucketByKey("1")==null?0:count.getBucketByKey("1").getDocCount();  
- //设置指定分组条件下成功的入库量
-             groupCount.setTotal_store_count(b.getDocCount()-bad_count);  
- //计算成功率
-             groupCount.setSuccess_rate(groupCount.getTotal_store_count()*1.0/groupCount.getTotal_count());  
- //添加到集合里面
-             datas.add(groupCount);  
-         }  
- return datas;  
-     }  
- 
- 
- 
- }  
总结： 
（1）关于时区的问题，目前发现在测试按小时，按天分组统计的时候，时区使用的方法不是一致的，而postZone这个方法，在1.5版本已经废弃，说是使用timeZone替代，但经测试发现在按小时分组的时候，使用timeZone加8个时区的并没生效，后续看下最新版本的ElasticSearch是否修复。 
（2）使用Terms的聚合分组时，这个字段最好是没有分过词的，否则大量的元数据返回，有可能会发生OOM的异常 
（3）在不需要评分排名查询的场景中，尽量使用filter查询，elasticsearch会缓存查询结果，从而能大幅提高检索性能 
今天先总结这么多，后续有空再关注下 
（1）elasticsearch中的Aggregations和Facet的区别以及对比Solr中的Group和Facet的区别 
（2）在不同的聚合渠道中多级分组中是组内有序还是全局有序 
