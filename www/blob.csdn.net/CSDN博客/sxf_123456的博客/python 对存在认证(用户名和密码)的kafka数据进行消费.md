# python 对存在认证(用户名和密码)的kafka数据进行消费 - sxf_123456的博客 - CSDN博客
2019年03月18日 10:23:44[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：24
from kafka import KafkaConsumer
from datetime import datetime, timedelta
from confluent_kafka import Consumer
import time
topic_name = 'apk_with_receivetime'
f_w = open("/data/test/data_dir/consumer_20190312.txt", "w", encoding="utf-8")
consumer = KafkaConsumer(topic_name,
                         group_id='test',
                         # enable_auto_commit=True,
                         # auto_commit_interval_ms=2,
                         sasl_mechanism="PLAIN",
                         security_protocol='SASL_PLAINTEXT',
                         sasl_plain_username="consumer",
                         sasl_plain_password="test",
                         bootstrap_servers=['1.3.1.16:9202', '1.8.1.10:9202', '1.3.1.17:9202'])
for msg in consumer:
    line = msg.value.decode("utf-8")
    line_ = line.strip().split("|")
    f_w.write(line + "\n")
    # print(msg.value.decode("utf-8"))
f_w.close()
