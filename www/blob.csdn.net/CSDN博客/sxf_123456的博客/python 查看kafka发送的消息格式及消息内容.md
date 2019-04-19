# python 查看kafka发送的消息格式及消息内容 - sxf_123456的博客 - CSDN博客
2018年09月03日 22:49:56[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：506
from pykafka import KafkaClient
client = KafkaClient(hosts="10.37.22.176:9092")
topic = client.topics[b'rtz_am_queue']    #topic名称
consumer = topic.get_simple_consumer()
for record in consumer:
    if record is not None:
        valuestr = record.value.decode()   #从bytes转为string类型
        # valuedict = eval(valuestr)
        # message = valuedict["message"]
        str_ = str(valuestr).split("|")
        if str_[3] == "34004":
            print(valuestr)
