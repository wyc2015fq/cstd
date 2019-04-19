# python 操作redis - sxf_123456的博客 - CSDN博客
2018年09月03日 14:44:27[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：58标签：[airflow](https://so.csdn.net/so/search/s.do?q=airflow&t=blog)
个人分类：[airflow](https://blog.csdn.net/sxf_123456/article/category/7418855)
1、redis分单个redis和集群redis
from airflow.contrib.hooks.redis_hook import RedisHook
from rediscluster import StrictRedisCluster
from sctetl.airflow.contrib.hooks.es_hook import ElasticsearchHook
from queue import Queue
import logging
queue_user = Queue()
fields = {
    "dev_mac",
    "olt_ip",
    "area_level2_id",
    "area_level3_id",
    "area_level4_id",
    "area_level6_id",
    "area_level5_id",
    "obd2_branch_id"
}
def write_to_redis(data_list):  # datalist是_source中的内容
    redis_client = RedisHook("redis_our_0").get_conn()
    pipline = redis_client.pipeline()
    for item in data_list:
        mac = "m_" + item.get("dev_mac")
        value = char(5).join([str(item.get(x)) if item.get(x, "") is not None else "" for x in fields[1:]])
        pipline.hset(mac, "t", value)
        pipline.expire(mac, 30 * 24 * 60 * 60)  # 设置有效日期
    pipline.execute()
def write_to_cluster_redis(data_list):
    nodes = "1.0.0.1:16382;1.0.0.2:16382;1.0.0.3:16382"
    startup__nodes = []
    for node in nodes.split(";"):
        startup_node = {"host": node.split(":")[0], "port": node.split(":")[1]}
        startup__nodes.append(startup_node)
    redis_client = StrictRedisCluster(startup_nodes=startup_nodes, password="123",
                                      skip_full_coverage_check=True)
    pipline = redis_client.pipeline()
    for item in data_list:
        mac = "m_" + item.get("dev_mac")
        value = char(5).join([str(item.get(x)) if item.get(x, "") is not None else "" for x in fields[1:]])
        pipline.hset(mac, "t", value)
        pipline.expire(mac, 30 * 24 * 60 * 60)  # 设置有效日期
    pipline.execute()
def get_data():
    body_ = {
        "query": {
            "match_all": {}
        }
    }
    index_ = "m-stb"
    es_hook = ElasticsearchHook("es_conn_id")
    rs = es_hook.scan(index=index_, body=body_)
    for item in rs:
        queue_user.put(item["_source"])
    queue_user.put(None)
def receive_data(choose_redis): #选择存储模式
    data_list = []
    if choose_redis == 1: #our redis
        logging.info("our redis")
        while True:
            data = queue_user.get()
            if data is None:
                break
            data_list.append(data)
            if len(data_list) == 8000:
                write_to_redis(data_list)
                data_list = []
        if data_list:
            write_to_redis(data_list)
    else:  # qx redis
        logging.info("qx redis")
        while True:
            data = queue_user.get()
            if data is None:
                break
            data_list.append(data)
            if len(data_list) == 8000:
                write_to_cluster_redis(data_list)
                data_list = []
        if data_list:
            write_to_cluster_redis(data_list)
if __name__ == '__main__':
    choose_redis = 1
    receive_data(choose_redis)
