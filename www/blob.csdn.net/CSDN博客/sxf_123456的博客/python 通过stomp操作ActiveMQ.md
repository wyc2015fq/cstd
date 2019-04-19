# python 通过stomp操作ActiveMQ - sxf_123456的博客 - CSDN博客
2019年03月19日 16:58:01[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：32
windows 下,在cmd 命令下执行 pip install stomp.py 安装stomp
```
# -*-coding:utf-8-*-
import stomp
import time
topic_name = 'test_topic'
listener_name = 'SampleListene'
mq_ip = "1.0.0.1"
mq_port = 61613
class SampleListener(stomp.ConnectionListener):
    def on_message(self, headers, message): 
        #header 消息头信息，可以通过header 判断那个topic的数据 
        #{'message-id': 'ID:jtcsOTMS-db-47634-1552380859117-1:1:1:1:7249045', 'destination': '/queue/test_topic', 'timestamp': '15529}
        print(message)
def receive_from_topic():
    conn = stomp.Connection10([(mq_ip, mq_port)]) #STOMP 1.0 连接方式
    conn.set_listener(listener_name, SampleListener()) #创建监听者
    #conn.start()
    conn.connect() # 在connect()中已经添加了start,可以将上边的start去掉
    conn.subscribe(topic_name, id="test")   #开始监听 接收消息
    while 1:
        try:
            time.sleep(10)
        except Exception as e:
            print(e)
            break
    if conn.is_connected():
        conn.disconnect()
if __name__ == '__main__':
    receive_from_topic()
```
