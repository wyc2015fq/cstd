# 快速取出ES索引中该索引所有字段名称 - sxf_123456的博客 - CSDN博客
2019年03月20日 14:33:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：59
# 快速取出ES索引中该索引所有字段名称
# 1、通过命令 GET  索引名称/_mapping 获取该索引所有字段信息
# 2、取出"properties"中所有字段，如test_dict中
# 3、通过test_dict.keys()获取所有字段值
# 4、转化为list, list(test_dict.keys()),输出结果可以直接用于获取数据的所有字段
test_dict = {
    "field0": {
        "type": "keyword"
    },
    "@timestamp": {
        "type": "date"
    },
    "field1": {
        "type": "keyword"
    },
    "field2": {
        "type": "keyword"
    }
}
print(list(test_dict.keys()))
