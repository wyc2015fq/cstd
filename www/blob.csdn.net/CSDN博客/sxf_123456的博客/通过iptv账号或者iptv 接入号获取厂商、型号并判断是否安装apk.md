# 通过iptv账号或者iptv 接入号获取厂商、型号并判断是否安装apk - sxf_123456的博客 - CSDN博客
2018年05月25日 11:57:34[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1829
*"""**通过iptv账号或者iptv 接入号获取厂商、型号并判断是否安装apk**"""*from elasticsearch import Elasticsearch
import os
import re
es = Elasticsearch(timeout=1500)
def get_iptv_apk(index_, iptv_account):
    *"""**    判断iptv账号在当前apk上报是否存在****:param**** index_:****:param**** iptv_account:****:return****:**    """*rs = None
    try:
        rs = es.search(index=index_, body={
            "size": 0,
"query": {
                "term": {
                    "iptvAccount": {
                        "value": iptv_account
                    }
                }
            }
        })
        if int(rs['hits']['total']) <= 0:
            value = "否"
else:
            value = "是"
return iptv_account, value
    except:
        return iptv_account, rs
def get_m_stb_apk(index_, iptv_ua_anid):
    *"""**    判断iptv账号或者iptv接入号在m-stb-apk中是否存在****:param**** index_:****:param**** iptv_ua_anid:****:return****:**    """*if re.search("@", iptv_acc_anid): #判断是iptv账号还是iptv接入号
try:
            rs_ua = es.search(index=index_, body={
                "query": {
                    "term": {
                        "iptv_ua.keyword": {
                            "value": iptv_ua_anid
                        }
                    }
                }
            })
            if rs_ua['hits']['total'] == 0:
                return iptv_ua_anid, "查询不到", "", ""
else:
                for hit in rs_ua['hits']['hits']:
                    fact = hit['_source']['manufacturers']
                    model = hit['_source']['product']
                    return iptv_ua_anid, fact, model, "是"
except:
            return iptv_ua_anid, "查询不到", "", ""
else:
        try:
            rs_anid = es.search(index=index_, body={
                "query": {
                    "term": {
                        "iptv_anid.keyword": {
                            "value": iptv_ua_anid
                        }
                    }
                }
            })
            if int(rs_anid['hits']['total']) == 0:
                return iptv_ua_anid, "查询不到", "", ""
else:
                for hit in rs_anid['hits']['hits']:
                    fact = hit['_source']['manufacturers']
                    model = hit['_source']['product']
                    return iptv_ua_anid, fact, model, "是"
except:
            return iptv_ua_anid, "查询不到", "", ""
if __name__ == '__main__':
    time_ = "20180523"
itv_quality = "m-ft-itv-quality-{0}-101000".format(time_)
    # m-stb-2018.01.09
    # m-stb-2018.03.24
    # m-stb-2018.05.07
m_stb_apk = "m-stb-apk"
iptv_account_file = "E:\\公司\\案例数据采集\\olt告警案例分析\\iptv_account.txt"
iptv_acc_anid_file = "E:\\公司\\案例数据采集\\olt告警案例分析\\iptv_acc_anid.txt"
path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}".format(time_)
    if os.path.exists(path):
        pass
    else:
        os.makedirs(path)
    write_acc_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{0}_iptv_account.txt".format(time_)
    write_acc_anid_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{0}_iptv_acc_anid.txt".format(time_)
    write_acc_file = open(write_acc_path, "w")
    write_acc_anid_file = open(write_acc_anid_path, "w")
    with open(iptv_account_file, "r") as f_acc:
        for iptv_acc in f_acc:
            line = iptv_acc.strip()
            _iptv_acc, install_apk = get_iptv_apk(index_=itv_quality, iptv_account=line)
            write_acc_file.write(_iptv_acc + "," + install_apk + "\n")
    f_acc.close()
    with open(iptv_acc_anid_file, "r") as f_acc_anid:
        for iptv_acc_anid in f_acc_anid:
            line = iptv_acc_anid.strip()
            _iptv_acc, factory, model, install_apk = get_m_stb_apk(index_=m_stb_apk, iptv_ua_anid=line)
            if factory is None:
                factory = ""
if model is None:
                model = ""
write_acc_anid_file.write(_iptv_acc + "," + factory + "," + model + "," + install_apk + "\n")
    f_acc_anid.close()
