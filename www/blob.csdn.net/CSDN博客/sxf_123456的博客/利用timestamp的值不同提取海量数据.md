# 利用timestamp的值不同提取海量数据 - sxf_123456的博客 - CSDN博客
2018年06月22日 11:24:00[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：57标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/sxf_123456/article/category/7078196)
*"""**需要提取全网大于=100M业务带宽的用户，终端能力小于业务宽带不匹配的清单**关键字段要求**分公司，宽带账号，业务带宽， 终端能力、光猫型号、光猫厂家**索引:gather-028-20180620 (获取宽带账号、业务宽带、终端能力)**索引:                    (获取分公司、光猫型号、光猫厂家)**"""*from elasticsearch import Elasticsearch
import os
import sys
import re
es = Elasticsearch(hosts='localhost:9221', timeout=15000)
def first_get_data(index_):
    rs = es.search(index=index_, body={
        "size": 10000,
"query": {
            "bool": {
                "must": [
                    {
                        "exists": {
                            "field": "band_width"
}
                    },
{
                        "range": {
                            "band_width": {
                                "gte": 100
}
                        }
                    },
{
                        "script": {
                            "script": "doc['max_band'].value < doc['band_width'].value"
}
                    }
                ]
            }
        },
"sort": [
            {
                "@timestamp": {
                    "order": "desc"
}
            }
        ],
"_source": ["area_name", "dev_ad_no", "dev_vendor_name", "dev_type_name", "dev_pppoe", "max_band", "band_width",
"@timestamp"]
    })
    return rs
def next_get_data(index_, time):
    rs = es.search(index=index_, body={
        "size": 10000,
"query": {
            "bool": {
                "must": [
                    {
                        "exists": {
                            "field": "band_width"
}
                    },
{
                        "range": {
                            "band_width": {
                                "gte": 100
}
                        }
                    },
{
                        "script": {
                            "script": "doc['max_band'].value < doc['band_width'].value"
}
                    },
{
                        "range": {
                            "@timestamp": {
                                "lt": time
                            }
                        }
                    }
                ]
            }
        },
"sort": [
            {
                "@timestamp": {
                    "order": "desc"
}
            }
        ],
"_source": ["area_name", "dev_ad_no", "dev_vendor_name", "dev_type_name", "dev_pppoe", "max_band", "band_width",
"@timestamp"]
    })
    return rs
if __name__ == '__main__':
    # index = "gather-028-20180620"
index = "gather-028-20180620_test_2"
write_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\20180621\\band_test.csv"
write_file = open(write_path, "w")
    write_file.write(
        "分公司" + "$" + "loid" + "$" + "宽带账号" + "$" + "支持最大宽带" + "$" + "申请带宽" + "$" + "光猫厂商" + "$" + "光猫型号" + "\n")
    rs = first_get_data(index)
    for hit in rs['hits']['hits']:
        try:
            area_name = hit['_source']['area_name']
        except:
            area_name = ""
try:
            loid = hit['_source']['dev_ad_no']
        except:
            loid = ""
try:
            dev_vendor_name = str(hit['_source']['dev_vendor_name']).replace(",","")
        except:
            dev_vendor_name = ""
try:
            dev_type_name = hit['_source']['dev_type_name']
        except:
            dev_type_name = ""
dev_pppoe = hit['_source']['dev_pppoe']
        max_band = hit['_source']['max_band']
        band_width = hit['_source']['band_width']
        timestamp = hit['sort'][0]
        write_file.write(area_name + "$" + loid + "$" + dev_pppoe + "$" + str(max_band) + "$" + str(
            band_width) + "$" + dev_vendor_name + "$" + dev_type_name + "\n")
    print(timestamp)
    i = 0
while (True):
        rs = next_get_data(index, timestamp)
        i  = i+1
for hit in rs['hits']['hits']:
            area_name = hit['_source']['area_name']
            loid = hit['_source']['dev_ad_no']
            dev_vendor_name = str(hit['_source']['dev_vendor_name']).replace(",", "")
            dev_type_name = hit['_source']['dev_type_name']
            dev_pppoe = hit['_source']['dev_pppoe']
            max_band = hit['_source']['max_band']
            band_width = hit['_source']['band_width']
            timestamp = hit['sort'][0]
            write_file.write(area_name + "$" + loid + "$" + dev_pppoe + "$" + str(max_band) + "$" + str(
                band_width) + "$" + dev_vendor_name + "$" + dev_type_name + "\n")
        print(timestamp)
        if i == 45:
            break
write_file.close()
