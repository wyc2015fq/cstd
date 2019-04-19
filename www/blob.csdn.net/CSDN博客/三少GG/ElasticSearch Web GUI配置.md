# ElasticSearch Web GUI配置 - 三少GG - CSDN博客
2018年06月18日 19:34:59[三少GG](https://me.csdn.net/scut1135)阅读数：1837

一. 
https://github.com/appbaseio/dejavu
docker compose文件:  https://github.com/appbaseio/dejavu/blob/dev/docker-compose.yml
直接复制相关代码到阿里云容器即可
二. ElasticHQ
https://github.com/ElasticHQ/elasticsearch-HQ
Error message was:- Unable to Connect to Server! Received Status Code: 0. A status code of 0, could mean the host is unreachable or nothing is listening on the given port.
To fix this issue I just added Few lines in elasticsearch.yml . File locate into /etc/elasticsearch/elasticsearch.yml .
At the bottom of elasticsearch.yml file add:
http.cors.enabled : true
http.cors.allow-origin : "*"
http.cors.allow-methods : OPTIONS, HEAD, GET, POST, PUT, DELETE
http.cors.allow-headers : X-Requested-With,X-Auth-Token,Content-Type, Content-Length
 and restart elasticsearch with this command sudo service elasticsearch restart. Wait for at-least 10 seconds and then hit your elasticsearch HQ index.html url and once again register your running elasticsearch [http://domain:port](http://domain/) .
