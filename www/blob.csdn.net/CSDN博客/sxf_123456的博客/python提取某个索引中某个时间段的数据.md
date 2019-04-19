# python提取某个索引中某个时间段的数据 - sxf_123456的博客 - CSDN博客
2018年05月13日 11:28:12[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2046
from elasticsearch import Elasticsearch
import datetime
import time
import  dateutil.parser
class App(object):
    def __init__(self):
        pass
    def _es_conn(self):
        es = Elasticsearch()
        return es
    def get_data(self, day,start,end):
        index_ = "gather-apk-20180330"
query_dsl = {
            "size": 10000,
"query": {
                "bool": {
                    "must": [
                        {"range": {
                            "receiveTime": {
                                "gte": start.strftime('%Y-%m-%d %H:%M:%S'),
"lte": end.strftime('%Y-%m-%d %H:%M:%S'),
"format": "yyyy-MM-dd HH:mm:SS",
"time_zone": "+08:00"
}
                        }},
{
                            "term": {
                                "obd2_localnet_id": {
                                    "value": "101000"
}
                            }
                        },
{
                            "term": {
                                "obd2_substation_name": {
                                    "value": "石羊支局"
}
                            }
                        }
                    ]
                }
            },
"_source": ["mac", "iptvAccount", "obd2_substation_name", "obd2_company_name", "obd2_grid_name",
"receiveTime","streamBreak","kaNum"]
        }
        rs =  self._es_conn().search(
            index=index_,
body=query_dsl
        )
      
if __name__ == '__main__':
    day = datetime.datetime.now()
    # the_day = day.strftime('%Y%m%d')
start = datetime.datetime.strptime('20180330 09:53:00','%Y%m%d %H:%M:%S')
    end = datetime.datetime.strptime('20180330 15:44:00','%Y%m%d %H:%M:%S')
    app = App()
    app.get_data(day,start,end)
