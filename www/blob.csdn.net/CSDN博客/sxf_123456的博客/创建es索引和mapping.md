# 创建es索引和mapping - sxf_123456的博客 - CSDN博客
2018年08月09日 09:06:15[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：465
```
创建es索引和mapping
第一种:
from elasticsearch import Elasticsearch
es = Elasticsearch(hosts="0.0.0.0", port=9200, timeout=5000)
index_20 = "olt_bas_test_20180824"
def set_map():
    body_ = {
        "mappings": {
            "_default_": {
                "_all": {
                    "enabled": False
                },
                "dynamic_templates": [
                    {
                        "strings": {
                            "match_mapping_type": "string",
                            "mapping": {
                                "type": "keyword"
                            }
                        }
                    }
                ]
            },
            "olt": {
                "_all": {
                    "enabled": False
                },
                "dynamic_templates": [
                    {
                        "strings": {
                            "match_mapping_type": "string",
                            "mapping": {
                                "type": "keyword"
                            }
                        }
                    }
                ],
                "properties": {
                    "frame_alarm_users": {
                        "type": "long"
                    },
                    "ka_alarm_users": {
                        "type": "long"
                    },
                    "ld_bad_qa_users": {
                        "type": "long"
                    },
                    "live_alarm_users": {
                        "type": "long"
                    },
                    "live_bad_qa_users": {
                        "type": "long"
                    },
                    "live_users": {
                        "type": "long"
                    },
                    "mdi_alarm_users": {
                        "type": "long"
                    },
                    "olt_ip": {
                        "type": "keyword"
                    },
                    "online_users": {
                        "type": "long"
                    },
                    "sb_alarm_users": {
                        "type": "long"
                    },
                    "view_users": {
                        "type": "long"
                    },
                    "vod_alarm_users": {
                        "type": "long"
                    },
                    "vod_bad_qa_users": {
                        "type": "long"
                    },
                    "vod_users": {
                        "type": "long"
                    }
                }
            },
            "bas": {
                "_all": {
                    "enabled": False
                },
                "dynamic_templates": [
                    {
                        "strings": {
                            "match_mapping_type": "string",
                            "mapping": {
                                "type": "keyword"
                            }
                        }
                    }
                ],
                "properties": {
                    "frame_alarm_users": {
                        "type": "long"
                    },
                    "ka_alarm_users": {
                        "type": "long"
                    },
                    "ld_bad_qa_users": {
                        "type": "long"
                    },
                    "live_alarm_users": {
                        "type": "long"
                    },
                    "live_bad_qa_users": {
                        "type": "long"
                    },
                    "live_users": {
                        "type": "long"
                    },
                    "mdi_alarm_users": {
                        "type": "long"
                    },
                    "bas_ip": {
                        "type": "keyword"
                    },
                    "online_users": {
                        "type": "long"
                    },
                    "sb_alarm_users": {
                        "type": "long"
                    },
                    "view_users": {
                        "type": "long"
                    },
                    "vod_alarm_users": {
                        "type": "long"
                    },
                    "vod_bad_qa_users": {
                        "type": "long"
                    },
                    "vod_users": {
                        "type": "long"
                    }
                }
            }
        }
    }
    if es.indices.exists(index_20):
        es.indices.delete(index_20)
    # 创建索引和map，另可以创建同一索引多个doc_type
    create_index = es.indices.create(index=index_20, body=body_)
    if create_index['acknowledged'] != True:
        print("create index error")
if __name__ == '__main__':
    set_map()
第二种：
注意:type_为索引的文档类型
def set_mapping():
    my_mapping = {
        type_: {
            "properties": {
                "@timestamp": {
                    "type": "date"
                },
                "@version": {
                    "type": "keyword"
                },
                "channelSwitchDelay": {
                    "type": "long"
                },
                "frameDelay": {
                    "type": "long"
                },
                "host": {
                    "type": "keyword"
                },
                "iptvAccount": {
                    "type": "keyword"
                },
                "kaNum": {
                    "type": "long"
                },
                "mac": {
                    "type": "keyword"
                },
                "message": {
                    "type": "keyword"
                },
                "oui": {
                    "type": "keyword"
                },
                "path": {
                    "type": "keyword"
                },
                "productClass": {
                    "type": "keyword"
                },
                "responseDelay": {
                    "type": "long"
                },
                "rtpLossRate": {
                    "type": "long"
                },
                "softProbeOUI": {
                    "type": "keyword"
                },
                "streamBreak": {
                    "type": "long"
                },
                "type": {
                    "type": "keyword"
                }
            }
        }
    }
    creat_index = es.indices.create(index=index_insert, body=None)
    mapping_index = es.indices.put_mapping(index=index_insert, doc_type=type_, body=my_mapping)
    if creat_index['acknowledged'] != True or mapping_index['acknowledged'] != True:
        print("Index create failed")
第三种:
在终端创建索引
1、创建索引
curl -XPUT 'http://0.0.0.0:9200/iptv_test_3'
2、设置map
curl -XPUT 'http://0.0.0.0:9200/iptv_test_3/iptv-apk/_mapping?pretty' -d '{
    "iptv-apk": {
                    "properties": {
                        "@timestamp": {
                            "type": "date"
                        },
                        "@version": {
                            "type": "keyword"
                        },
                        "channelSwitchDelay": {
                            "type": "long"
                        },
                        "frameDelay": {
                            "type": "long"
                        },
                        "host": {
                            "type": "keyword"
                        },
                        "iptvAccount": {
                            "type": "keyword"
                        },
                        "kaNum": {
                            "type": "long"
                        },
                        "mac": {
                            "type": "keyword"
                        },
                        "message": {
                            "type": "keyword"
                        },
                        "oui": {
                            "type": "keyword"
                        },
                        "path": {
                            "type": "keyword"
                        },
                        "productClass": {
                            "type": "keyword"
                        },
                        "responseDelay": {
                            "type": "long"
                        },
                        "rtpLossRate": {
                            "type": "long"
                        },
                        "softProbeOUI": {
                            "type": "keyword"
                        },
                        "streamBreak": {
                            "type": "long"
                        },
                        "type": {
                            "type": "keyword"
                        }
                    }
                }
}'
3、查看设置的map
curl -XGET 'http://0.0.0.0:9200/iptv_test_3/_mapping?pretty'
```
