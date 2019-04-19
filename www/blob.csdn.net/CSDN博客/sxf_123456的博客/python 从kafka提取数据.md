# python 从kafka提取数据 - sxf_123456的博客 - CSDN博客
2018年06月22日 11:30:07[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：669
from pykafka import KafkaClient
from pykafka.common import OffsetType  # OffsetType中存在两个变量EARLITEST(分区最早偏移量),LATEST(分区最新偏移量)
def KafkaDownloader(host_, topic_, group_id_):
    client = KafkaClient(hosts=host_)
    _topic = client.topics[bytes(topic_.encode())]
    consumer = _topic.get_simple_consumer(
        consumer_group= bytes(group_id_.encode()),
auto_commit_enable = False,
auto_offset_reset = OffsetType.LATEST,
reset_offset_on_start = True
)
    if consumer is not None:
        for message in consumer:
            if message is not None:
                yield  message.value
def get_kafka_data():
    TOPIC = "rtz_queue"
HOSTS = "133.0.0.0:9092,133.0.0.0:9092"
GROUP = "test"
BASE_D = "/data/test_tt.txt"
# downloader = KafkaDownloader(host=HOSTS, topic=TOPIC, group_id=GROUP)
for message in KafkaDownloader(host_=HOSTS, topic_=TOPIC, group_id_=GROUP):
        print(message.decode("GB18030","ignore"))
if __name__=='__main__':
    get_kafka_data()
