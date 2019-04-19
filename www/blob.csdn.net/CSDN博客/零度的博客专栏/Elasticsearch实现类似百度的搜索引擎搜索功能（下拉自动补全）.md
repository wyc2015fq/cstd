# Elasticsearch实现类似百度的搜索引擎搜索功能（下拉自动补全） - 零度的博客专栏 - CSDN博客
2018年05月23日 10:26:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：2717
## 业务需求（使用背景）：
- 实现搜索引擎前缀搜索功能(中文,拼音前缀查询及简拼前缀查询功能)
- 实现摘要全文检索功能，及标题加权处理功能(按照标题权值高内容权值相对低的权值分配规则，按照索引的相关性进行排序，列出前20条相关性最高的文章)
## 一、搜索引擎前缀搜索功能：
中文搜索：
 1、搜索“刘”，匹配到“刘德华”、“刘斌”、“刘德志”
 2、搜索“刘德”，匹配到“刘德华”、“刘德志”
 小结：搜索的文字需要匹配到集合中所有名字的子集。
 全拼搜索：
 1、搜索“li”，匹配到“刘德华”、“刘斌”、“刘德志”
 2、搜索“liud”，匹配到“刘德华”、“刘德”
 3、搜索“liudeh”，匹配到“刘德华”
 小结：搜索的文字转换成拼音后，需要匹配到集合中所有名字转成拼音后的子集
简拼搜索：
 1、搜索“w”，匹配到“我是中国人”，“我爱我的祖国”
 2、搜索“wszg”，匹配到“我是中国人”
 小结：搜索的文字取拼音首字母进行组合，需要匹配到组合字符串中前缀匹配的子集
### 解决方案：
方案一：将“like”搜索的字段的中、英简拼、英全拼 分别用索引的三个字段来进行存储并且不进行分词，最简单直接(倒排索引存储它们本身数据)，检索索引数据的时候进行 通配符查询（like查询），从这三个字段中分别进行搜索，查询匹配的记录然后返回。（优势：存储格式简单，倒排索引存储的数据量最少。缺点：like索引数据的时候开销比较大 prefix 查询比 term 查询开销大得多）
方案二：将中、中简拼、中全拼 用一个字段衍生出三个字段(multi-field)来存储三种数据，并且分词器filter采用edge_ngram类型对分词的数据进行，然后处理存储到倒排索引中,当检索索引数据时，检索所有字段的数据。（优势：格式紧凑，检索索引数据的时候采用term 全匹配规则，也无需对入参进行分词，查询效率高。缺点：采用以空间换时间的策略，但是对索引来说可以接受。采用衍生字段来存储，增加了存储及检索的复杂度，对于三个字段搜索会将相关度相加，容易混淆查询相关度结果）
方案三：将索引数据存储在一个不需分词的字段中(keyword), 生成倒排索引时进行三种类型倒排索引的生成，倒排索引生成的时候采用edge_ngram 对倒排进一步拆分，以满足业务场景需求，检索时不对入参进行分词。（优势：索引数据存储简单，，检索索引数据的时只需对一个字段 采用term 全匹配查询规则，查询效率极高。缺点：采用以空间换时间的策略——比方案二要少，对索引数据来说可以接受。）
> 
ES 针对这一业务场景解决方案还有很多种，先列出比较典型的这三种方案，选择方案三来进行处理。
### 准备工作：
- pinyin分词插件安装及参数解读
- ElasticSearch edge_ngram 使用
- ElasticSearch multi-field 使用
- ElasticSearch 多种查询特性熟悉
### 代码：
baidu_settings.json:
```
{
  "refresh_interval":"3s",
  "number_of_replicas":1,
  "number_of_shards":5,
  "analysis":{
    "filter":{
      "autocomplete_filter":{
        "type":"edge_ngram",
        "min_gram":1,
        "max_gram":15
      },
      "pinyin_first_letter_and_full_pinyin_filter" : {
        "type" : "pinyin",
        "keep_first_letter" : true,
        "keep_full_pinyin" : false,
        "keep_joined_full_pinyin": true,
        "keep_none_chinese" : false,
        "keep_original" : false,
        "limit_first_letter_length" : 16,
        "lowercase" : true,
        "trim_whitespace" : true,
        "keep_none_chinese_in_first_letter" : true
      },
      "full_pinyin_filter" : {
        "type" : "pinyin",
        "keep_first_letter" : true,
        "keep_full_pinyin" : false,
        "keep_joined_full_pinyin": true,
        "keep_none_chinese" : false,
        "keep_original" : true,
        "limit_first_letter_length" : 16,
        "lowercase" : true,
        "trim_whitespace" : true,
        "keep_none_chinese_in_first_letter" : true
      }
    },
    "analyzer":{
      "full_prefix_analyzer":{
        "type":"custom",
        "char_filter": [
          "html_strip"
        ],
        "tokenizer":"keyword",
        "filter":[
          "lowercase",
          "full_pinyin_filter",
          "autocomplete_filter"
        ]
      },
      "chinese_analyzer":{
        "type":"custom",
        "char_filter": [
          "html_strip"
        ],
        "tokenizer":"keyword",
        "filter":[
          "lowercase",
          "autocomplete_filter"
        ]
      },
      "pinyin_analyzer":{
        "type":"custom",
        "char_filter": [
          "html_strip"
        ],
        "tokenizer":"keyword",
        "filter":[
          "pinyin_first_letter_and_full_pinyin_filter",
          "autocomplete_filter"
        ]
      }
    }
  }
}
```
baidu_mapping.json
```
{
  "baidu_type": {
    "properties": {
      "full_name": {
        "type":  "text",
        "analyzer": "full_prefix_analyzer"
      },
      "age": {
        "type":  "integer"
      }
    }
  }
}
```
