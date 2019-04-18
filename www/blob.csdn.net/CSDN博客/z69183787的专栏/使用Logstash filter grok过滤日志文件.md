# 使用Logstash filter grok过滤日志文件 - z69183787的专栏 - CSDN博客
2018年07月25日 10:56:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：258
[https://blog.csdn.net/aitangyong/article/details/49661907](https://blog.csdn.net/aitangyong/article/details/49661907)
## Logstash Filter Plugin Grok
> 
Logstash提供了一系列filter过滤plugin来处理收集到的log event，根据log event的特征去切分所需要的字段，方便kibana做visualize和dashboard的data analysis。所有logstash支持的event切分插件查看[这里](https://link.jianshu.com?t=https%3A%2F%2Fwww.elastic.co%2Fguide%2Fen%2Flogstash%2Fcurrent%2Ffilter-plugins.html)。下面我们主要讲[grok](https://link.jianshu.com?t=https%3A%2F%2Fwww.elastic.co%2Fguide%2Fen%2Flogstash%2Fcurrent%2Fplugins-filters-grok.html)切分。
### Grok基本介绍
- Grok 使用文本片段切分的方式来切分日志事件，语法如下:
```
%{SYNTAX:SEMANTIC}
```
```
* `SYNTAX`代表匹配值的类型，例如，`0.11`可以`NUMBER`类型所匹配，`10.222.22.25`可以使用`IP`匹配。
* `SEMANTIC`表示存储该值的一个变量声明，它会存储在`elasticsearch`当中方便`kibana`做字段搜索和统计，你可以将一个`IP`定义为客户端IP地址`client_ip_address`，eg:`%{IP:client_ip_address}`，所匹配到的值就会存储到`client_ip_address`这个字段里边，类似数据库的列名，也可以把event log中的数字当成数字类型存储在一个指定的变量当中，比如响应时间`http_response_time`，假设event log record如下:
```
```
55.3.244.1 GET /index.html 15824 0.043
```
可以使用如下grok pattern来匹配这种记录
```
%{IP:client_id_address} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:http_response_time}
```
在logstash conf.d文件夹下面创建filter conf文件，内容如下
```
# /etc/logstash/conf.d/01-filter.conf
filter {
  grok {
    match => { "message" => "%{IP:client_id_address} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:http_response_time}" }
  }
}
```
以下是filter结果
```
client_id_address: 55.3.244.1
method: GET
request: /index.html
bytes: 15824
http_response_time: 0.043
```
grok内置的默认类型有很多种，查看所有[默认类型](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Flogstash-plugins%2Flogstash-patterns-core%2Fblob%2Fmaster%2Fpatterns%2Fgrok-patterns)。
- 使用自定义类型
	更多时候logstash grok没办法提供你所需要的匹配类型，这个时候我们可以使用自定义
	- 第一种，直接使用oniguruma语法去匹配文本片段，语法如下
		
```
(?<field_name>the pattern here)
```
		假设你需要匹配的文本片段为一个长度为10或11的十六进制的值，使用下列语法可以获取该片段，并把值赋予`queue_id`
```
(?<queue_id>[0-9A-F]{10,11})
```
- 第二种，创建自定义pattern文件
		- 创建文件夹`patterns`，在此文件夹下面创建一个文件，文件名随意，eg: `postfix`
```
# contents of ./patterns/postfix:
POSTFIX_QUEUEID [0-9A-F]{10,11}
```
- 然后将patterns file引入，告诉logstash你的自定义类型文件，以下面的event log record为例子：
```
55.3.244.1 GET /index.html 15824 0.043 ABC24C98567
```
		在logstash conf.d文件夹下面创建filter conf文件，内容如下
		
```
filter {
grok {
 patterns_dir => ["./patterns"]
 match => { "message" => "%{IP:client_id_address} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:http_response_time} %{POSTFIX_QUEUEID:queue_id}" }
}
}
```
匹配结果如下：
```
client_id_address: 55.3.244.1
method: GET
request: /index.html
bytes: 15824
http_response_time: 0.043
queue_id: ABC24C98567
```
推荐使用[grokdebugger](https://link.jianshu.com?t=http%3A%2F%2Fgrokdebug.herokuapp.com%2F)来写匹配模式，输入event log record，再逐步使用pattern微调切分，下方会根据你所写的模式将输入切分字段。
- 其他常用内置方法
	- `add_field:` 当pattern匹配切分成功之后，可以动态的对某些字段进行特定的修改或者添加新的字段，使用`%{fieldName}`来获取字段的值
		Exmaple:
```
filter {
  grok{
    add_field => { "foo_%{somefield}" => "Hello world, %{somefield}" }
  }
}
```
```
# You can also add multiple fields at once:
filter {
  grok {
    add_field => {
      "foo_%{somefield}" => "Hello world, %{somefield}"
      "new_field" => "new_static_value"
    }
  }
}
```
如果`somefield=dad`，logstash会将`foo_dad`新字段加入`elasticsearch`，并将值`Hello world, dad`赋予该字段
- `add_tag:` 为经过filter或者匹配成功的event添加标签
	Example:
```
filter {
  grok {
    add_tag => [ "foo_%{somefield}" ]
  }
}
```
```
# You can also add multiple tags at once:
filter {
  grok {
    add_tag => [ "foo_%{somefield}", "taggedy_tag"]
  }
}
```
更多内置方法在[这里](https://link.jianshu.com?t=https%3A%2F%2Fwww.elastic.co%2Fguide%2Fen%2Flogstash%2Fcurrent%2Fplugins-filters-grok.html%23_details_131)
作者：eason02
链接：https://www.jianshu.com/p/d46b911fb83e
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
