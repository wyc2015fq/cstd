# python实现某天每5分钟提取一条数据 - sxf_123456的博客 - CSDN博客
2018年07月22日 11:09:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：828
```
方法一：
from elasticsearch import Elasticsearch
import os
es = Elasticsearch(['localhost'], http_auth=('', ''), port=9273, timeout=50000)
```
```
def write_start_data(time_, time_list, month, file_name, iptvaccount):
    write_fact_path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}\\{1}_{2}.txt".format(time_, iptvaccount, file_name)
    f = open(write_fact_path, "w")
    for key, value in time_list.items():
        start = key.format(day, start_hour, month, end_hour)
        end = value.format(day, start_hour, month, end_hour)
        hour_min = start.split()[1]
        if hour_min == "23:55":
            end = value.format(day_next,start_hour,month,end_hour)
        rs = get_iptv_monitor_start_data(time_, start, end, iptvaccount)
        for hit in rs['hits']['hits']:
```
```
if __name__ == '__main__':
    time_ = "20180719"
    month = "07"
    day = "19"
    day_next = "20"
    hour_dict = {}
    for i in range(0, 24):
        if i < 10:
            str_i = "0" + str(i)
            str_j = "0" + str(i + 1)
            if str_j == "010":
                str_j = str(i + 1)
            hour_dict.setdefault(str_i, str_j)
        else:
            str_i = str(i)
            str_j = str(i + 1)
            if str_j == "24":
                str_j = "00"
            hour_dict.setdefault(str_i, str_j)
    for start_hour,end_hour in hour_dict.items():
        time_list = {"2018-{2}-{0} {1}:00": "2018-{2}-{0} {1}:05", "2018-{2}-{0} {1}:05": "2018-{2}-{0} {1}:10",
                     "2018-{2}-{0} {1}:10": "2018-{2}-{0} {1}:15", "2018-{2}-{0} {1}:15": "2018-{2}-{0} {1}:20",
                     "2018-{2}-{0} {1}:20": "2018-{2}-{0} {1}:25", "2018-{2}-{0} {1}:25": "2018-{2}-{0} {1}:30",
                     "2018-{2}-{0} {1}:30": "2018-{2}-{0} {1}:35", "2018-{2}-{0} {1}:35": "2018-{2}-{0} {1}:40",
                     "2018-{2}-{0} {1}:40": "2018-{2}-{0} {1}:45", "2018-{2}-{0} {1}:45": "2018-{2}-{0} {1}:50",
                     "2018-{2}-{0} {1}:50": "2018-{2}-{0} {1}:55", "2018-{2}-{0} {1}:55": "2018-{2}-{0} {3}:00",
                     }
        path = "E:\\公司\\案例数据采集\\olt告警案例分析\\{0}".format(time_)
        if os.path.exists(path):
            pass
        else:
            os.makedirs(path)
        write_start_data(time_, time_list, month, start_hour, iptv)  # 获取原始数据
```
`方法二：`
```
GET m-ft-itv-quality-20180622-101000/_search
{
  "size": 0,
  "query": {
    "bool": {
      "must": [
        {"term": {
          "olt_ip": {
            "value": "172.00.00.00"
          }
        }},
        {
          "range": {
            "uploadTime": {
              "gte": "2018-06-22 10:00",
              "lte": "2018-06-22 11:05",
              "format": "yyyy-MM-dd HH:mm",
              "time_zone": "+08:00"
            }
          }
        }
      ]
    }
  },
  "aggs": {
    "1": {
      "date_histogram": {
        "field": "uploadTime",
        "interval": "5m",
        "time_zone": "+08:00"
      }
    }
  }
}
```
