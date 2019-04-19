# 【es】cardinality 计算不准确问题 - sxf_123456的博客 - CSDN博客
2017年08月18日 11:00:59[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：237
个人分类：[ELK																[python](https://blog.csdn.net/sxf_123456/article/category/7078196)](https://blog.csdn.net/sxf_123456/article/category/7036606)
## 遇到问题：
两个结果不一样，按说是一样的
### 结果一：
- 
```python
{
```
- 
```python
"query": {
```
- 
```python
"bool": {
```
- 
```python
"must_not": [
```
- 
```python
{
```
- 
```python
"match_phrase": {
```
- 
```python
"reqUA": "Jakarta Commons-HttpClient/3.1"
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"match_phrase": {
```
- 
```python
"reqReferer": "http://www.baidu.com/s?wd=www"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
],
```
- 
```python
"must": [
```
- 
```python
{
```
- 
```python
"range": {
```
- 
```python
"reqTime": {
```
- 
```python
"gte": "2016-09-25 22:00:00",
```
- 
```python
"lte": "2016-09-26 22:00:00"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"range": {
```
- 
```python
"operateBeforeObj.sendTime": {
```
- 
```python
"gte": "2016-09-25 22:00:00",
```
- 
```python
"lte": "2016-09-26 22:00:00"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"terms": {
```
- 
```python
"productPageCode": [
```
- 
```python
"10001",
```
- 
```python
"33002"
```
- 
```python
]
```
- 
```python
}
```
- 
```python
}
```
- 
```python
]
```
- 
```python
}
```
- 
```python
},
```
- 
```python
"from": 0,
```
- 
```python
"aggs": {
```
- 
```python
"channelTag": {
```
- 
```python
"terms": {
```
- 
```python
"field": "channelTag",
```
- 
```python
"size": 0
```
- 
```python
},
```
- 
```python
"aggs": {
```
- 
```python
"userId": {
```
- 
```python
"cardinality": {
```
- 
```python
"field": "user.userId"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
"size": 0
```
- 
```python
}
```
![](https://img-blog.csdn.net/20160927121352495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 结果二：
- 
```python
{
```
- 
```python
"query": {
```
- 
```python
"bool": {
```
- 
```python
"must_not": [
```
- 
```python
{
```
- 
```python
"match_phrase": {
```
- 
```python
"reqUA": "Jakarta Commons-HttpClient/3.1"
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"match_phrase": {
```
- 
```python
"reqReferer": "http://www.baidu.com/s?wd=www"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
],
```
- 
```python
"must": [
```
- 
```python
{
```
- 
```python
"range": {
```
- 
```python
"reqTime": {
```
- 
```python
"gte": "2016-09-25 22:00:00",
```
- 
```python
"lte": "2016-09-26 22:00:00"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"range": {
```
- 
```python
"operateBeforeObj.sendTime": {
```
- 
```python
"gte": "2016-09-25 22:00:00",
```
- 
```python
"lte": "2016-09-26 22:00:00"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
{
```
- 
```python
"terms": {
```
- 
```python
"productPageCode": [
```
- 
```python
"10001",
```
- 
```python
"33002"
```
- 
```python
]
```
- 
```python
}
```
- 
```python
}
```
- 
```python
]
```
- 
```python
}
```
- 
```python
},
```
- 
```python
"from": 0,
```
- 
```python
"aggs": {
```
- 
```python
"userId": {
```
- 
```python
"cardinality": {
```
- 
```python
"field": "user.userId"
```
- 
```python
}
```
- 
```python
}
```
- 
```python
},
```
- 
```python
"size": 0
```
- 
```python
}
```
![](https://img-blog.csdn.net/20160927121414855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 分析问题：
问题应该在cardinality上，cardinality有个参数 "precision_threshold": 100 ，100是个预设值，你的真实值小于100计算出来的值就是正确的，真实值大于100计算出来的值就是模糊的，100可以自定义。
## 解决问题：
- 
```python
{
```
- 
```python
"aggs":{
```
- 
```python
"author_count":{
```
- 
```python
"cardinality":{
```
- 
```python
"field":"author_hash",
```
- 
```python
"precision_threshold":100
```
- 
```python
}
```
- 
```python
}
```
- 
```python
}
```
- 
```python
}
```
![](https://img-blog.csdn.net/20160927121432058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
