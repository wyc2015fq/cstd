# ELK菜鸟手记 (三) - X-Pack权限控制之给Kibana加上登录控制以及index_not_found_exception问题解决... - weixin_33985507的博客 - CSDN博客
2019年01月08日 06:42:29[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
**0. 背景**
我们在使用ELK进行日志记录的时候，通过网址在Kibana中查看我们的应用程序(eg: Java Web)记录的日志，
但是默认是任何客户端都可以访问Kibana的, 这样就会造成很不安全，我们应该设置相应的用户名和密码，
只有通过登录用户名和密码才能通过Kibana查看我们的日志。
**1. 在elasticsearch 2.x的版本是怎么做的**
笔者网上查了一些博文，大部分推荐的是通过给elasticsearch安装Shield插件，参考链接如下：
[http://blog.csdn.net/xuplus/article/details/51611658](http://blog.csdn.net/xuplus/article/details/51611658)
但是，这种做法已经过时了，而且当你从官网下载的elasticsearch的最新版本，笔者写博文时候是5.x(5.2.2)
照着博文上安装插件的做法，根本是不行的
一般博文会建议进入elasticsearch的根目录，执行如下命令: bin/plugin install shield
但是，当你用的是5.x的时候，你会发现根本就没有plugin这条命令，进入es的根目录，发现只有
elasticsearch-plugin这条命令，这是怎么回事呢？
于是笔者上了官网一探究竟(任何时候查找技术，官网永远是最好最权威的选择)
官网给出的解释如下：
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429133731834-1279220475.png)
笔者恍然大悟，原来在5.x以后Shield插件已经作为X-Pack的一部分了，所以，必须查找关于X-Pack的相关文档。
**2. X-Pack是什么？**
以下是官网给出的解释:
*(X-Pack is an Elastic Stack extension that bundles security, alerting, monitoring, reporting, and graph capabilities into one easy-to-install package.*
*Prior to Elasticsearch 5.0.0, you had to install separate Shield, Watcher, and Marvel plugins to get the features that are bundled together in X-Pack. *
*With X-Pack, you no longer have to worry about whether or not you have the right version of each plugin,*
*just install the X-Pack for the Elasticsearch version you’re running)*
X-Pack是Elastic技术栈的扩展，它集安全，提醒，监控，报表以及图标功能于一体。
在Elasticsearch 5.0之前，你必须单独安装Shield插件，还要配套Watcher, Marvel等插件，现在X-Pack把它们都整合到一块儿了。
原来是这样啊！
**3. 安装X-Pack**
3-1) 为elasticsearch安装X-Pack插件
进入 elasticsearch根目录
执行: 
bin/elasticsearch-plugin install x-pack
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429134601303-987242647.png)
3-2) 配置elasticsearch.yml
进入config目录
修改配置文件，在末尾加上如下行：
action.auto_create_index: .security,.monitoring*,.watches,.triggered_watches,.watcher-history*
这是为elasticsearch增加自动创建索引功能
3-3) 启动elasticsearch
bin/elasticsearch
3-4) 为Kibana安装X-Pack插件
进入Kibana根目录
执行命令:
bin/kibana-plugin install x-pack
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429135234740-641386608.png)
3-5) 启动Kibana
bin/kibana
3-6) 为Logstash节点安装X-Pack插件
进入Logstash根目录
执行命令:
bin/logstash-plugin install x-pack
3-7) 用配置文件启动Logstash
bin/logstash -f config/log4j_multi_input.conf
3-8) 验证
浏览器打开路径:
[http://localhost:5601/](http://localhost:5601/)
你看回到登录对话框如下：
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429135733631-1161137015.png)
默认用户名和密码是：
elastic
changeme
**4. LogStash::Outputs::ElasticSearch::HttpClient::Pool::BadResponseCodeError, :error=>"Got response code '401' contact Elasticsearch at URL**
这个时候，你可能认为我们已经大功告成了，然而并不是这样。
当你用用户名和密码登录Kibana了以后，你会发现没有任何索引，你之前使用Java程序写的日志到哪里去了呢？
笔者十分纳闷，后来查看了Logstash的控制台，笔者发现了如下错误：
LogStash::Outputs::ElasticSearch::HttpClient::Pool::BadResponseCodeError, :error=>"Got response code '401' contact Elasticsearch at URL
因为我们刚才安装了X-Pack插件，因此，我们需要在我们logstash的配置文件中指定用户名和密码，不然是没有权限访问的，
笔者的配置文件如下：
```
input {
    file {  
        path => ["/Users/KG/Documents/logs/app-a/*.log"]  
        type => "app-a"
    }  
    file {  
        path => ["/Users/KG/Documents/logs/app-b/*.log"] 
        type => "app-b"
    }  
}
output {
    stdout {
      codec => rubydebug
    }
    if [type] == "app-a" {  
       elasticsearch { 
            hosts => "localhost:9200"  
            index =>  "app-a-%{+YYYY.MM.dd}"
            document_type => "log4j_type"**            user => elastic            password =>**** changeme**
        }  
    }  
    else if [type] == "app-b" {  
        elasticsearch { 
            hosts => "localhost:9200"  
            index => "app-b-%{+YYYY.MM.dd}"
            document_type => "log4j_type"**            user => elastic            password =>**** changeme**
        }  
    }  
}
```
红色字体部分为新加的
然后，再次重新启动Logstash
**5. 无法查看索引下的日志问题解决**
好事多磨，我们还是无法在Kibana下看到数据，究竟是怎么一回事呢?
笔者再次查看了logstash的控制台，又发现了如下错误:
logstash outputs elasticsearch error 404 >>index_not_found_exception
上网查了下资料，原来需要在elasticsearch中创建自动索引
还记得刚才我们在elasticsearch.yml配置文件最后一行加的那句代码吗，看一下：
笔者修改如下：
```
action.auto_create_index: .security,.monitoring*,.watches,.triggered_watches,.watcher-history*,**app-a-*,app-b-***
```
其中红色字体部分为笔者测试程序所用的索引
再次重新启动elasticsearch
**6. 最后的验证**
好了，笔者使用Java代码进行验证(之前的博文中有提到怎么使用log4j进入日志到ELK)
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429141210772-185597501.png)
再次访问Kibana,...看到如下结果：
![](https://images2015.cnblogs.com/blog/72194/201704/72194-20170429141246053-626616705.png)
好了，这回真的成功了，哈哈，是不是很有成就感啊？^_^

