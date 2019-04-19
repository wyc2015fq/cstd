# Ubuntu14.04部署pyspider的过程 - =朝晖= - 博客园
# [Ubuntu14.04部署pyspider的过程](https://www.cnblogs.com/dhcn/p/7121400.html)
1、安装，安装官方文档，应该先执行
- sudo apt-get install python python-dev python-distribute python-pip libcurl4-openssl-dev libxml2-dev libxslt1-dev python-lxml  
再执行：
- sudo pip install --allow-all-external pyspider[all]  
第一个错误：
- No distributions at all found for mysql-connector-python>=1.2.2  
解决方法：换pip源，给~/.pip/pip.conf里面加上：
- [global]  
- index-url = http://pypi.douban.com/simple/  
第二个错误：
- Error: pg_config executable not found.  
解决方法：
- sudo aptitude install libpq-dev  
第三个错误：
```
In file included from src/docstrings.c:4:0:
    src/pycurl.h:170:31: fatal error: openssl/crypto.h: No such file or directory
    compilation terminated.
    error: command 'x86_64-linux-gnu-gcc' failed with exit status 1
```
解决方法：
```
sudo apt-get install libssl-dev
```
如果要用phantomjs，还得独立安装：
- sudo aptitude install phantomjs  
建议使用[MongoDB](http://lib.csdn.net/base/mongodb)做数据存储：
- sudo aptitue install mongodb  
建议使用redis做消息队列存储：
- sudo aptitude install redis-server  
给个与官放文档略有不同的配置文件范本(config.json)：
- {  
- "taskdb":"mongodb+taskdb://mongodbuser:password@127.0.0.1:27017/taskdb",  
- "projectdb":"mongodb+projectdb://mongodbuser:password@127.0.0.1:27017/projectdb",  
- "resultdb":"mongodb+resultdb://mongodbuser:password@127.0.0.1:27017/resultdb",  
- "message_queue":"redis://:redis_password@127.0.0.1:6379/1",  
- "webui": {  
- "username": "webUIusername",  
- "password": "webUIpassword",  
- "need-auth": true  
-   }  
- }  
启动shell脚本：
- #!/bin/bash  
- sudo nohup pyspider -c config.json all &  

