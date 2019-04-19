# Elasticsearch Aggregation 多个字段分组统计 Java API实现 - sxf_123456的博客 - CSDN博客
2018年03月08日 11:28:33[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：5507
[Elasticsearch Aggregation 多个字段分组统计 Java API实现](http://kfcman.iteye.com/blog/2270125)
现有索引数据：
index:school
type:student
---------------------------------------------------
{"grade":"1", "class":"1", "name":"xiao 1"}
{"grade":"1", "class":"1", "name":"xiao 2"}
{"grade":"1", "class":"2", "name":"xiao 3"}
{"grade":"1", "class":"2", "name":"xiao 4"}
{"grade":"1", "class":"2", "name":"xiao 5"}
Java分组统计年级和班级学生个数，如SQL: SELECT grade,class,count(1) FROM student GROUP BY grade,class;
package test;
import java.util.Iterator;
import java.util.Map;
import org.elasticsearch.action.search.SearchRequestBuilder;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.action.search.SearchType;
import org.elasticsearch.search.aggregations.Aggregation;
import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.bucket.terms.StringTerms;
import org.elasticsearch.search.aggregations.bucket.terms.Terms.Bucket;
import org.elasticsearch.search.aggregations.bucket.terms.TermsBuilder;
import org.junit.Test;
import utils.NesUtils;
public class TestAggregation
{
@Test
public void testAggregation()
{
SearchRequestBuilder srb = NesUtils.getSearcher("school");
srb.setTypes("student");
srb.setSearchType(SearchType.COUNT);
TermsBuilder gradeTermsBuilder = AggregationBuilders.terms("gradeAgg").field("grade");
TermsBuilder classTermsBuilder = AggregationBuilders.terms("classAgg").field("class");
gradeTermsBuilder.subAggregation(classTermsBuilder);
srb.addAggregation(gradeTermsBuilder);
SearchResponse sr = srb.execute().actionGet();
Map<String, Aggregation> aggMap = sr.getAggregations().asMap();
StringTerms gradeTerms = (StringTerms) aggMap.get("gradeAgg");
Iterator<Bucket> gradeBucketIt = gradeTerms.getBuckets().iterator();
while(gradeBucketIt.hasNext())
{
Bucket gradeBucket = gradeBucketIt.next();
System.out.println(gradeBucket.getKey() + "年级有" + gradeBucket.getDocCount() +"个学生。");
StringTerms classTerms = (StringTerms) gradeBucket.getAggregations().asMap().get("classAgg");
Iterator<Bucket> classBucketIt = classTerms.getBuckets().iterator();
while(classBucketIt.hasNext())
{
Bucket classBucket = classBucketIt.next();
System.out.println(gradeBucket.getKey() + "年级" +classBucket.getKey() + "班有" + classBucket.getDocCount() +"个学生。");
}
System.out.println();
}
}
}
运行完成输出结果
---------------------------------------------------
1年级有5个学生。
1年级2班有3个学生。
1年级1班有2个学生。
