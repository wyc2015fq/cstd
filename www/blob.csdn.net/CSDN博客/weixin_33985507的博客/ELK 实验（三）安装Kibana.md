# ELK 实验（三）安装Kibana - weixin_33985507的博客 - CSDN博客
2018年04月18日 15:05:26[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
下载安装linux 包
wget https://artifacts.elastic.co/downloads/kibana/kibana-6.2.3-linux-x86_64.tar.gz
shasum -a 512 kibana-6.2.3-linux-x86_64.tar.gz 
tar -xzf kibana-6.2.3-linux-x86_64.tar.gz
cd kibana-6.2.3-linux-x86_64/ 
下载安装Darwin 包
curl -O https://artifacts.elastic.co/downloads/kibana/kibana-6.2.3-darwin-x86_64.tar.gz
shasum -a 512 kibana-6.2.3-darwin-x86_64.tar.gz 
tar -xzf kibana-6.2.3-darwin-x86_64.tar.gz
cd kibana-6.2.3-darwin-x86_64/ 
配置参考
[https://www.elastic.co/guide/en/kibana/current/settings.html](https://www.elastic.co/guide/en/kibana/current/settings.html)
运行
./bin/kibana
![3a61edbe103b42699b6946f8eef46956](http://i2.51cto.com/images/blog/201804/18/a87a012836ee0ce6309b3fa3854d3670.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
9200端口没开，我先开一下es
![4668ac2bdb1c47d8ab991f1b6b210e9b](http://i2.51cto.com/images/blog/201804/18/c702d41f14d54089f1c9b33c224c9d79.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![74b317aa9ba7449789b0ed54ade8caa0](http://i2.51cto.com/images/blog/201804/18/933f4b1ad8a93b2dea38d6eabc05d3ad.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
netstat -lntup | grep 9* 端口都起来了
![2eae4b38ce4d4d8a8ed4a7d8e1198d21](http://i2.51cto.com/images/blog/201804/18/4dee8cf71974a3a44e4c7e6170e78222.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
远程访问不了，ip没有绑定，容我更新配置
vi kibana.yml
server.host: "192.168.14.66"
elasticsearch.url: "http://192.168.14.66:9200"
后台运行下
nohup ./bin/kibana &
kibana 状态变绿了
网页状态
[http://192.168.209.160:5601/status#?_g=()](http://192.168.209.160:5601/status#?_g=())
![0fce36652c134155bba2ae4899c5184b](http://i2.51cto.com/images/blog/201804/18/f8a7ca207a04582ee82f309297c4f8e1.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
添加数据和自定义数据来源
[http://192.168.209.160:5601/app/kibana#/home](http://192.168.209.160:5601/app/kibana#/home)
![358e0063323d4fdf8a2e5adef971beab](http://i2.51cto.com/images/blog/201804/18/2d0f4460cc4f675b95701f95396337d7.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![5e4743a0aaa24e8888163cbd314791be](http://i2.51cto.com/images/blog/201804/18/cca219f5545e55755f7396baca32075a.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
这组件也基本属于0配置，由于没有logstash 所以报表没有数据，所以进入下一个组件的安装。
视频指导
- [High-level Kibana introduction, pie charts](https://www.elastic.co/blog/kibana-4-video-tutorials-part-1)
- [Data discovery, bar charts, and line charts](https://www.elastic.co/blog/kibana-4-video-tutorials-part-2)
- [Coordinate maps](https://www.elastic.co/blog/kibana-4-video-tutorials-part-3)
- [Embedding Kibana visualizations](https://www.elastic.co/blog/kibana-4-video-tutorials-part-4)
