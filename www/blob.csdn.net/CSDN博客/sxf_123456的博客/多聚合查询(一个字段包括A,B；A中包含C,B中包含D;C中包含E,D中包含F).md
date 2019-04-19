# 多聚合查询(一个字段包括A,B；A中包含C,B中包含D;C中包含E,D中包含F) - sxf_123456的博客 - CSDN博客
2018年07月24日 18:19:23[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：182
```
GET iptv - monitor - m - gather - apk - 20180723 / _search
{
    "size": 1,
    "query": {
        "bool": {
            "filter": [
                {
                    "terms": {
                        "mdiNested.mdiType": [
                            "0",
                            "1",
                            "2",
                            "3",
                            "4"
                        ]
                    }
                }
            ]
        }
    },
    "aggs": {
        "1": {
            "terms": {
                "field": "softProbeOUI",
                "size": 10
            },
            "aggs": {
                "2": {
                    "terms": {
                        "field": "oui",
                        "size": 100
                    },
                    "aggs": {
                        "3": {
                            "terms": {
                                "field": "productClass",
                                "size": 100
                            }
                        }
                    }
                }
            }
        }
    }
}
```
