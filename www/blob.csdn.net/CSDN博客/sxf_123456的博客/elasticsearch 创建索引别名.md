# elasticsearch 创建索引别名 - sxf_123456的博客 - CSDN博客
2019年01月25日 16:37:14[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：76
根据需求，有时需要对索引创建别名，便于对多天或者多个索引的数据进行查询。
操作：
1、kibana 中创建别名
POST /_aliases
{
"actions": [
{
"add": {
"index": "test-20190120",
"alias": "test-2019"
}
}
]
}
