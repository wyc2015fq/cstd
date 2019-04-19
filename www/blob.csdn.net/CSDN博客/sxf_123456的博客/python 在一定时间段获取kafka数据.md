# python 在一定时间段获取kafka数据 - sxf_123456的博客 - CSDN博客
2018年10月09日 18:58:58[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：367标签：[python																[kafka](https://so.csdn.net/so/search/s.do?q=kafka&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[kafka](https://blog.csdn.net/sxf_123456/article/category/8004748)
from pykafka import KafkaClient
from pykafka.common import OffsetType
import datetime
def KafkaDownloader(host_, topic_, group_id_):
    client = KafkaClient(hosts=host_)
    _topic = client.topics[bytes(topic_.encode())]
    consumer = _topic.get_simple_consumer(
        consumer_group=bytes(group_id_.encode()),
        auto_commit_enable=False,
        auto_offset_reset=OffsetType.LATEST,
        reset_offset_on_start=True
    )
    if consumer is not None:
        for message in consumer:
            if message is not None:
                yield message.value
def get_kafka_data():
    write_file = open("/data/itv_pm_data_1.txt", "w", encoding="utf8")
    TOPIC = "_topic"
    HOSTS = "1.3.2.1:9092,1.3.2.7:9092"
    GROUP = "test"
    dict_msg = {}
    end_time = str(datetime.datetime.now() + datetime.timedelta(minutes=10))[0:len("2018-10-09 18:26")]
    for message in KafkaDownloader(HOSTS, TOPIC, GROUP):
        curr_time = str(datetime.datetime.now())[0:len("2018-10-09 18:26")]
        msg = message.decode("GB18030")
        msg_str = str(msg).strip().split("\t")
        id = msg_str[5]
        dict_msg.setdefault(id, str(msg).strip())
        if curr_time == end_time:
            for key, value in dict_msg.items():
                write_file.write(value + "\n")
            break
    dict_msg = {}
    write_file.close()
if __name__ == '__main__':
    get_kafka_data()
