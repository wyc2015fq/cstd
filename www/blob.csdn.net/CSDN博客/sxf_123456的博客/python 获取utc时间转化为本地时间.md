# python 获取utc时间转化为本地时间 - sxf_123456的博客 - CSDN博客
2017年12月21日 13:09:48[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3358
python 获取utc时间转化为本地时间
方法一：
import datetime
timenow = (datetime.datetime.utcnow() + datetime.timedelta(hours=8)) #将utc时间转化为本地时间
timetext = timenow.strftime('%y%m%d')
方法二：
import datetime
import  dateutil.parser
st_time = hit['_source']['start_time']
re_time = hit['_source']['report_time']
igmp_delay = hit['_source']['igmp_delay']
live_delay = hit['_source']['live_delay']
st = dateutil.parser.parse(st_time)  #将2017-12-21T04:57:42.000Z 字符串转化为时间
re = dateutil.parser.parse(re_time)
start_time =(st+datetime.timedelta(hours=8))  #将#将utc时间2017-12-21T04:57:42.000Z 转化为时间本地时间2017-12-21 12:57:42+00:00
report_time = (re+datetime.timedelta(hours=8))
message = str(start_time)[0:19]+","+str(report_time)[0:19]+","+str(int(igmp_delay))+","+str(int(live_delay))+"\n"
python 从es中获取数据
import os
import datetime
from elasticsearch import  Elasticsearch
import  dateutil.parser
es = Elasticsearch(hosts="127.0.0.1",timeout=10000)
write_file=open('C:\\Users\\Administrator\\Desktop\\gather-005-201712210.csv',"a+",encoding="utf-8")
rs = es.search(
    index = "gather-005-20171221",
    body={
    "size":42,
    "query": {
    "term": {
      "itv_account": {
        "value": "38:FA:CA:D9:5F:2B"
      }
    }
  },
    "sort": [
    {
      "report_time": {
        "order": "desc"
      }
    }
  ],
  "_source": ["start_time","report_time","igmp_delay","live_delay"]
}
)
for hit in rs['hits']['hits']:
    st_time = hit['_source']['start_time']
    re_time = hit['_source']['report_time']
    igmp_delay = hit['_source']['igmp_delay']
    live_delay = hit['_source']['live_delay']
    st = dateutil.parser.parse(st_time)
    re = dateutil.parser.parse(re_time)
    start_time =(st+datetime.timedelta(hours=8))
    report_time = (re+datetime.timedelta(hours=8))
    message = str(start_time)[0:19]+","+str(report_time)[0:19]+","+str(int(igmp_delay))+","+str(int(live_delay))+"\n"
    write_file.write(message)
write_file.close()
方法三：
UTC  转化UTC
 utc1 = 1406869066, utc2 =  1406869070  相差4, 也就是这两个时间相差4秒
