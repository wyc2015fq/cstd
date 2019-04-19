# es 查询 - sxf_123456的博客 - CSDN博客
2018年08月09日 09:06:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：109
查询语句
GET iptv-monitor-m-gather-apk-20180731/_search
{
    "size": 0,
    "query": {
        "terms": {
            "mac": [
                "28:8C:B8:D0:D1:A8"
            ]
        }
    },
    "aggs": {
        "1": {
            "terms": {
                "field": "mac",
                "size": 5
            },
            "aggs": {
                "1": {
                    "terms": {
                        "field": "softProbeOUI",
                        "size": 1
                    }
                },
                "2": {
                    "terms": {
                        "field": "iptvAccount",
                        "size": 1
                    }
                },
                "3": {
                    "terms": {
                        "field": "oui",
                        "size": 1
                    }
                },
                "4": {
                    "terms": {
                        "field": "productClass",
                        "size": 1
                    }
                },
                "5": {
                    "terms": {
                        "field": "obd2_localnet_id",
                        "size": 1
                    }
                }
            }
        }
    }
}
查询结果
"aggregations": {
        "1": {
            "doc_count_error_upper_bound": 0,
            "sum_other_doc_count": 0,
            "buckets": [
                {
                    "1": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "key": "SCTELE",
                                "doc_count": 2
                            }
                        ]
                    },
                    "2": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "key": "YBBXZ6690404@ITV",
                                "doc_count": 2
                            }
                        ]
                    },
                    "3": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "key": "ZTE",
                                "doc_count": 2
                            }
                        ]
                    },
                    "4": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "key": "B860AV2.1",
                                "doc_count": 2
                            }
                        ]
                    },
                    "5": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "key": "112000",
                                "doc_count": 2
                            }
                        ]
                    },
                    "key": "28:8C:B8:D0:D1:A8",
                    "doc_count": 2
                }
            ]
        }
    }
查询语句
GET iptv-monitor-m-gather-apk-20180731/_search
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
                            },
                            "aggs": {
                                "优良率": {
                                    "scripted_metric": {
                                        "init_script": "params._agg=[:];",
                                        "map_script": "List p=[0, 0];List accp=params._agg.getOrDefault(doc.mac.value, p);if(doc.kaNum.value>=1){accp[0]+=1}else{accp[1]+=1}params._agg[doc.mac.value]=accp;",
                                        "reduce_script": "List rsv=[0, 0];Map rs=[:];for (a in params._aggs){a.forEach((k,v)->{List p=[0,0];List accp = rs.getOrDefault(k, p);accp[0] += v[0];accp[1] += v[1];rs[k] = accp;})}rs.forEach((k,v)->{if((v[0] + v[1])<290){if(v[1]*100/(v[0] + v[1])>=95){rsv[0] += 1;}else{rsv[1] += 1;}}else{return null}});return rsv;"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
查询结果
   "aggregations": {
        "1": {
            "doc_count_error_upper_bound": 0,
            "sum_other_doc_count": 0,
            "buckets": [
                {
                    "2": {
                        "doc_count_error_upper_bound": 0,
                        "sum_other_doc_count": 0,
                        "buckets": [
                            {
                                "3": {
                                    "doc_count_error_upper_bound": 0,
                                    "sum_other_doc_count": 0,
                                    "buckets": [
                                        {
                                            "key": "E900-S",
                                            "doc_count": 81607982,
                                            "优良率": {
                                                "value": [
                                                    545102,
                                                    15507
                                                ]
                                            }
                                        },
                                        {
                                            "key": "E900",
                                            "doc_count": 22975593,
                                            "优良率": {
                                                "value": [
                                                    149275,
                                                    4202
                                                ]
                                            }
                                        }
                                    ]
                                },
                                "key": "SKYWORTH",
                                "doc_count": 104583575
                            }
                        ]
                    },
                    "key": "YUCHUANG",
                    "doc_count": 411648677
                }
            ]
       }
    }
}
