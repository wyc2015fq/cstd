# python dict转json并保存文件 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月23日 13:30:31[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：631
import json
f = open("index.html", "wb")
json.dump(response.data, f)
f.close()

dumps只完成了序列化为str，
       dump必须传文件描述符，将序列化的str保存到文件中
