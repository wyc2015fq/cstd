# olt告警分析 - sxf_123456的博客 - CSDN博客
2018年05月18日 17:18:29[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：358
*获取各指标(卡顿、点播、直播、媒体丢包、**二层丢帧、断流、内存、RTP丢包)**不同媒体类型(mdiType=点播、高清直播、**2k高清直播、4k高清直播、EPG页面)的质差用户数*from elasticsearch import Elasticsearch
import os
from datetime import datetime
es = Elasticsearch(timeout=1500)
def get_stb_olt_user(stb_index, olt_ip):
    *"""**    获取资源树olt_ip下的总用户数****:param**** olt_ip:****:return****:**    """*try:
        rs = es.search(index=stb_index, body={
            "size": 0,
"query": {
                "term": {
                    "olt_ip": {
                        "value": olt_ip
                    }
                }
            }
        })
        return rs['hits']['total']
    except:
        raise Exception("{0} search error".format(stb_index))
def get_online_badQuality_user(itv_quality, olt_ip):
    *"""**    olt_ip下在线质差用户数****:param**** itv_quality:****:param**** olt_ip:****:return****:**    """*try:
        rs = es.search(index=itv_quality, body={
            "size": 0,
"query": {
                "bool": {
                    "must": [
                        {
                            "term": {
                                "olt_ip": {
                                    "value": olt_ip
                                }
                            }
                        },
{
                            "term": {
                                "badQuality": {
                                    "value": "1"
}
                            }
                        }
                    ]
                }
            },
"aggs": {
                "online_badQuality_user": {
                    "cardinality": {
                        "field": "iptvAccount"
}
                }
            }
        })
        return rs['aggregations']['online_badQuality_user']['value']
    except:
        raise Exception("{0} serach {1} error".format(itv_quality, "online_badQuality_user"))
def get_online_user(itv_quality, olt_ip):
    *"""**    olt_ip下在线用户数****:param**** itv_quality:****:param**** olt_ip:****:return****:**    """*try:
        rs = es.search(index=itv_quality, body={
            "size": 0,
"query": {
                "term": {
                    "olt_ip": {
                        "value": olt_ip
                    }
                }
            },
"aggs": {
                "online_user": {
                    "cardinality": {
                        "field": "iptvAccount"
}
                }
            }
        })
        return rs['aggregations']['online_user']['value']
    except:
        raise Exception("{0} search {1} error".format(itv_quality, "online_user"))
def get_dif_target_list(itv_quality, olt_ip, target_field):
    *"""**    获取各指标(卡顿、点播、直播、媒体丢包、**    二层丢帧、断流、内存、RTP丢包)**    不同媒体类型(mdiType=点播、高清直播、**    2k高清直播、4k高清直播、EPG页面)的质差用户数****:param**** itv_quality:****:param**** olt_ip:****:param**** target_field:****:return****:**    """*try:
        rs = es.search(index=itv_quality, body={
            "size": 0,
"query": {
                "bool": {
                    "must": [
                        {
                            "term": {
                                "olt_ip": {
                                    "value": olt_ip
                                }
                            }
                        },
{
                            "term": {
                                target_field: {
                                    "value": "1"
}
                            }
                        },
{
                            "term": {
                                "badQuality": {
                                    "value": "1"
}
                            }
                        }
                    ]
                }
            },
"aggs": {
                "1": {
                    "terms": {
                        "field": "mdiType",
"size": 5
},
"aggs": {
                        "2": {
                            "cardinality": {
                                "field": "iptvAccount"
}
                        }
                    }
                }
            }
        })
        return rs['aggregations']['1']['buckets']
    except:
        raise Exception("{0} search {1} error".format(itv_quality, "dif_target"))
def get_dif_target_value(target_list):
    *"""**    列表中不同指标各类型的值，保存在类型字典表**    列表中不同指标各类型的值：**    [{'key': '1', 'doc_count': 30, '2': {'value': 6}},**    {'key': '5', 'doc_count': 14, '2': {'value': 5}}]**    转化为类型字典表**    {{"1":6},{"5":5}}****:param**** target_list:****:return****:**    """*target_dict = {}
    for line in target_list:
        target_dict.setdefault(line.get("key"),line.get("2").get("value"))
    return target_dict
if __name__ == '__main__':
    stb_index = "m-stb-2018.05.07"
olt_ip = "0.0.0.0"
time_ = "20180515"
itv_quality = "m-ft-itv-quality-{0}-101000".format(time_)
    target_field = "mdiAlarm"
stb_olt_user_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{1}".format(format(datetime.now(),"%Y%m%d"),time_)
    online_badQuality_user_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{1}".format(format(datetime.now(),"%Y%m%d"),time_)
    online_user_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{1}".format(format(datetime.now(), "%Y%m%d"),time_)
    target_value_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{1}".format(format(datetime.now(), "%Y%m%d"),time_)
    write_stb_olt_user_path = os.path.join(stb_olt_user_path,"stb_olt_total_user.txt")
    write_online_badQuality_user_path = os.path.join(online_badQuality_user_path,"online_badQuality_user.txt")
    write_online_user_path = os.path.join(online_user_path,"online_user.txt")
    write_target_value_path = os.path.join(target_value_path,"target_value_{0}.txt".format(target_field))
    if os.path.exists(stb_olt_user_path):
        pass
    else:
        os.makedirs(stb_olt_user_path)
    with open(write_stb_olt_user_path,"w") as f:
        value = get_stb_olt_user(stb_index,olt_ip)
        f.write("olt下总用户数:{0}".format(value))
        f.close()
    with open(write_online_badQuality_user_path,"w") as f:
        value = get_online_badQuality_user(itv_quality,olt_ip)
        f.write("olt下在线质差用户数:{0}".format(value))
    if os.path.exists(online_user_path):
        pass
    else:
        os.makedirs(online_user_path)
    with open(write_online_user_path, "w") as f:
        value = get_online_user(itv_quality, olt_ip)
        f.write("olt下在线用户数:{0}".format(value))
        f.close()
    with open(write_target_value_path,"w") as f:
        f.write("{0}指标媒体不同类型值:\n".format(target_field))
        target_list = get_dif_target_list(itv_quality, olt_ip, target_field)
        dict_value = get_dif_target_value(target_list)
        for i in range(6):
            target_value = dict_value.get(str(i))
            if target_value is None:
                target_value = 0
f.write(str(target_value)+",")
        f.close()
