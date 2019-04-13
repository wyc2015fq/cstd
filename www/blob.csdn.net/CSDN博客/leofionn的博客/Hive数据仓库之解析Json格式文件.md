
# Hive数据仓库之解析Json格式文件 - leofionn的博客 - CSDN博客


2018年05月09日 09:49:21[leofionn](https://me.csdn.net/qq_36142114)阅读数：262


转载自[Lamborryan](http://www.lamborryan.com/)，作者：[Ruan Chengfeng](http://www.lamborryan.com/about/)本文链接地址：[http://www.lamborryan.com/hive-json](http://www.lamborryan.com/hive-json)
## 1.简介
公司的日志都是以json格式保存的且存放在HDFS上的，有时候需要直接通过查询Hive就能得到日志里面的具体信息。要实现以上的目的，HIVE提供了三种方法，分别是:
UDF get_json_object(string json_string,string path)
UDTF json_tuple(string jsonstr,p1,p2,…,pn)
HIVE SERDE的扩展, Json SERDE
## 2.UDF实现
假设我们的Json格式数据如下：
{"date":"2015-05-16 00:00:00","data":{"channel":"wandoujia","machine":"MI 3","resolution":"1080x1920","display":"4.589389937671455","ip":"192.168.1.1","lat":null,"lng":null,"device_key":"863637027016968","os_name":"android","os_version":"4.4.4","app_name":"FM","app_version":"2.7.0","carradio_version":"","carradio_mac":"","property":"20150515_235835_PlayFragment_wandoujia_2.7.0","date":"2015-05-15 23:58:35","page":"PlayFragment","user_key":"b2925849c6e838ab16d0a8d2c0a46f09","event":"exit"}}
如果用UDF的方法实现那么分为两步：
以以下的命令建表CREATE TABLE IF NOT EXISTS jsontest (json STRING);
用get_json_object解析json的字段:
SELECT get_json_object(mc.json,'$.date')asdate,get_json_object(mc.json,'$.data.event')asevent,get_json_object(mc.json,'$.data.property')aseventfromjsontest mc
由此可见，如果要获取多个json字段的时候就会比较麻烦，且影响运行速度。
## 3.UDTF实现
相比于UDF，表生成函数UDTF作用更进一步，它更高效，其通过一次调用就可以获得多个键值。
如果用UDTF的方法实现那么分为两步：
同UDF一样，以以下的命令建表CREATE TABLE IF NOT EXISTS jsontest (json STRING);
用json_tuple解析json的字段
selectv2.event,v2.profromjsontest mc
     LATERAL VIEW json_tuple(mc.json,'date','data')vasevent,pro
     LATERAL VIEW json_tuple(v.pro,'event','property')v2asevent,pro由此可见，如果json是多层形式的，那么通过LATERAL VIEW调用依然很麻烦。
## 4.JSON SERDE
比起前两种，JSON SERDE无疑高效多了，只需要在CREATE的时候设置好SERDE，那么在SELECT时候就会进行解析JSON。
### 4.1.建表
CREATE EXTERNAL TABLE IF NOT EXISTS jsontest(date STRING,data STRUCT<channel:STRING,machine:STRING,resolution:STRING,display:STRING,ip:STRING,lat:STRING,lng:STRING,device_key:STRING,os_name:STRING,os_version:STRING,app_name:STRING,app_version:STRING,carradio_version:STRING,carradio_mac:STRING,property:STRING,date:STRING,page:STRING,user_key:STRING,event:STRING>)ROW FORMAT SERDE'org.openx.data.jsonserde.JsonSerDe'withSERDEPROPERTIES("ignore.malformed.json"="true")STORED AS TEXTFILE;
### 4.2.1查询
SELECT date.event,date.property,date FROM jsontest
没错，就是这是这么简单。
其实在建表的时候我已经用ROW FORMAT SERDE ‘org.openx.data.jsonserde.JsonSerDe’来告诉HIVE用JsonSerDe来解析JSON。那么什么是SERDE呢，SERDE就是序列化/反序列化的简称.
在上述建表时候，我讲json的data字段映射成STRUCT类型，访问data的内的keys，只需要data.[keys],比如data.event.
除了上述建表方式，还可以使用以下的映射关系
CREATE TABLE IF NOT EXISTS jsontest(date STRING,channel STRING,machine STRING,resolution STRING,display STRING,ip STRING,lat STRING,lng STRING,device_key STRING,os_name STRING,os_version STRING,app_name STRING,app_version STRING,carradio_version STRING,carradio_mac STRING,property STRING,crate_date STRING,page STRING,user_key STRING,eventSTRING)ROW FORMAT SERDE'org.openx.data.jsonserde.JsonSerDe'WITH SERDEPROPERTIES("date"="$.date","channel"="$.data.channel","machine"="$.data.machine","resolution"="$.data.resolution","display"="$.data.display","ip"="$.data.ip","lat"="$.data.lat","lng"="$.data.lng","device_key"="$.data.device_key","os_name"="$.data.os_name","os_version"="$.data.os_version","app_version"="$.data.app_version","carradio_version"="$.data.carradio_version","carradio_mac"="$.data.carradio_mac","property"="$.data.property","crate_date"="$.data.date","page"="$.data.page","user_key"="$.data.user_key","event"="$.data.event","ignore.malformed.json"="true")STORED AS TEXTFILE;
1.ignore.malformed.json表示是否忽略json解析失败的错误，该值为false的话，如果在进行Hive查询时候，json格式出错，Map Task就会失败，最终导致HIVE查询失败，如果该值为True，那么遇到解析失败就会跳过该条记录。
2.org.openx.data.jsonserde.JsonSerDe 是继承SERDE接口写的，具体代码和包请看我的github[JsonSerde](https://github.com/lamborryan/JsonSerDe/tree/develop)。我根据HIVE 1.1.0 和 HADOOP 2.6.0进行重新的编译。你也可以根据你的版本编译.
<dependency><groupId>org.apache.hive</groupId><artifactId>hive-serde</artifactId><version>1.1.0</version></dependency><dependency><groupId>org.apache.hadoop</groupId><artifactId>hadoop-common</artifactId><version>2.6.0</version></dependency>
3.编译好json-serde.jar包后，需要将jar上传到HIVE中，可以使用以下两种方法：
(1) 使用add jar, 但是该方法只能在当前shell环境有效。```shell
hive> add jar json-serde-1.3.1.jar;
Added [json-serde-1.3.1.jar] to class path
Added resources: [json-serde-1.3.1.jar]
```(2) 将jar包添加到HIVE环境变量里. a.在${HIVE_HOME}下建立auxlib目录，将jar包扔到该目录下 b.在${HIVE_HOME}/bin/hive-conf.sh修改HIVE_AUX_JARS_PATH变量值为```export HIVE_AUX_JARS_PATH=$HIVE_HOME/auxlib``` c.重启HIVE即可
> 如果出现以下错误，意味着Json-serde与 Hive或者Hadoop版本不匹配FAILED : Execution Error , return code 1 from org . apache . hadoop . hive . ql . exec . DDLTask . org . apache . hadoop . hive . serde2 . objectinspector . primitive . AbstractPrimitiveJavaObjectInspector . ( Lorg / apache / hadoop / hive / serde2 / objectinspector / primitive / PrimitiveObjectInspectorUtils $ PrimitiveTypeEntry ; )

## 5.总结
由此可见UDF和UDTF适合某个字段(String类型)储存的数据是json格式的字符串这种场景, 而Json Serde 适合整条记录是以json格式存储的。

