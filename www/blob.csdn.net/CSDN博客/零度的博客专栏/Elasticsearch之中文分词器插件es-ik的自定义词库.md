# Elasticsearch之中文分词器插件es-ik的自定义词库 - 零度的博客专栏 - CSDN博客
2018年05月18日 13:56:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：245

原文出处：https://www.cnblogs.com/zlslch/p/6440891.html
      前提，[Elasticsearch之中文分词器插件es-ik](http://www.cnblogs.com/zlslch/p/6440373.html)针对一些特殊的词语在分词的时候也需要能够识别
　　有人会问，那么，例如：
　　　　　　　　　　如果我想根据自己的本家姓氏来查询，如zhouls，姓氏“周”。
　　　　　　　　      如果我想根据自己的公司名称来查询，如“好记性不如烂笔头感叹号博客园”
　　　　　　　　　  如果我想根据自己公司里的产品名称来查询，如“”
　　　　　　　　　  如果我想根据某个网络上流行的词名称来查询，如“扫福”
　　那么，若直接使用es-ik则分不出来的，所以，这就是为什么需要es-ik的自定义词库的缘由啦！
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225090836398-2128895492.png)
[hadoop@HadoopMaster elasticsearch-2.4.3]$ curl 'http://192.168.80.10:9200/zhouls/_analyze?analyzer=ik_max_word&pretty=true' -d '{"text":"好记性不如烂笔头感叹号博客园"}'
{
  "tokens" : [ {
    "token" : "好记",
    "start_offset" : 0,
    "end_offset" : 2,
    "type" : "CN_WORD",
    "position" : 0
  }, {
    "token" : "记性",
    "start_offset" : 1,
    "end_offset" : 3,
    "type" : "CN_WORD",
    "position" : 1
  }, {
    "token" : "不如",
    "start_offset" : 3,
    "end_offset" : 5,
    "type" : "CN_WORD",
    "position" : 2
  }, {
    "token" : "烂",
    "start_offset" : 5,
    "end_offset" : 6,
    "type" : "CN_CHAR",
    "position" : 3
  }, {
    "token" : "笔头",
    "start_offset" : 6,
    "end_offset" : 8,
    "type" : "CN_WORD",
    "position" : 4
  }, {
 "token" : "笔",
    "start_offset" : 6,
    "end_offset" : 7,
    "type" : "CN_WORD",
    "position" : 5
  }, {
    "token" : "头",
    "start_offset" : 7,
    "end_offset" : 8,
    "type" : "CN_CHAR",
    "position" : 6
  }, {
    "token" : "感叹号",
    "start_offset" : 8,
    "end_offset" : 11,
    "type" : "CN_WORD",
    "position" : 7
  }, {
    "token" : "感叹",
    "start_offset" : 8,
    "end_offset" : 10,
    "type" : "CN_WORD",
    "position" : 8
  }, {
    "token" : "叹号",
    "start_offset" : 9,
    "end_offset" : 11,
    "type" : "CN_WORD",
    "position" : 9
  }, {
 "token" : "叹",
    "start_offset" : 9,
    "end_offset" : 10,
    "type" : "CN_WORD",
    "position" : 10
  }, {
    "token" : "号",
    "start_offset" : 10,
    "end_offset" : 11,
    "type" : "CN_CHAR",
    "position" : 11
  }, {
    "token" : "博客园",
    "start_offset" : 11,
    "end_offset" : 14,
    "type" : "CN_WORD",
    "position" : 12
  }, {
    "token" : "博客",
    "start_offset" : 11,
    "end_offset" : 13,
    "type" : "CN_WORD",
    "position" : 13
  }, {
    "token" : "园",
    "start_offset" : 13,
    "end_offset" : 14,
    "type" : "CN_CHAR",
    "position" : 14
  } ]
}
[hadoop@HadoopMaster elasticsearch-2.4.3]$ 
**Elasticsearch之中文分词器插件es-ik的自定义词库**
【 ik 自定义词库步骤】
1： 首先在 ik 插件的 config/custom 目录下创建一个文件 zhouls.dic （当然这个你可以自己命名，如my.dic都行）
在文件中添加词语即可， 每一个词语一行。
-------------------------------------------------------------------------------------------------------------
注意： 这个文件可以在 linux 中直接 vi 生成， 或者在 windows 中创建之后上传到这里。
如果是在 linux 中直接 vi 生成的， 可以直接使用。
如果是在 windows中创建的，需要注意文件的编码必须是 UTF-8 without BOM 格式 【 UTF-8 无
BOM 格式】
-------------------------------------------------------------------------------------------------------------
2： 修改 ik 的配置文件
默认情况下 ik 的配置文件就在 ik 插件的 config 目录下面。【 IKAnalyzer.cfg.xml】
把刚才创建的文件的位置添加到 ik 的配置文件中即可。
vi config/IKAnalyzer.cfg.xml
<properties>
<comment>IK Analyzer 扩展配置</comment>
<!--用户可以在这里配置自己的扩展字典 -->
<entry
key="ext_dict">custom/mydict.dic;custom/single_word_low_freq.dic**;custom/zhouls.dic**</entry>
<!--用户可以在这里配置自己的扩展停止词字典-->
<entry key="ext_stopwords">custom/ext_stopword.dic</entry>
<!--用户可以在这里配置远程扩展字典 -->
<!-- <entry key="remote_ext_dict">words_location</entry> -->
<!--用户可以在这里配置远程扩展停止词字典-->
<!-- <entry key="remote_ext_stopwords">words_location</entry> -->
</properties>
------------------------------------------------------------------------------------------------------------------------------
注意： 需要把 my.dic 文件的位置添加到 key=ext_dict 这个 entry 中， 切记不要随意新增 entry，
随意新增的 entry 是不被识别的。
并且 entry 的名称也不能乱改， 否则也不会识别。
<entry
key="ext_dict">custom/mydict.dic;custom/single_word_low_freq.dic;custom/my.dic</entry>
------------------------------------------------------------------------------------------------------------------------------
3： 重启 es 验证分词效果
重启的时候查看日志会看到下面信息
　　第一步：进入$ES_HOME/plugins/ik/config/custom/下，新建自定义词库。我这里，取名为zhouls.dic
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225091019288-1894785107.png)
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225091217523-37416886.png)
[hadoop@HadoopMaster elasticsearch-2.4.3]$ ll
total 56
drwxrwxr-x. 2 hadoop hadoop  4096 Feb 22 01:37 bin
drwxrwxr-x. 3 hadoop hadoop  4096 Feb 22 18:46 config
drwxrwxr-x. 3 hadoop hadoop  4096 Feb 22 06:05 data
drwxrwxr-x. 2 hadoop hadoop  4096 Feb 22 01:37 lib
-rw-rw-r--. 1 hadoop hadoop 11358 Aug 24  2016 LICENSE.txt
drwxrwxr-x. 2 hadoop hadoop  4096 Feb 25 05:15 logs
drwxrwxr-x. 5 hadoop hadoop  4096 Dec  8 00:41 modules
-rw-rw-r--. 1 hadoop hadoop   150 Aug 24  2016 NOTICE.txt
drwxrwxr-x. 5 hadoop hadoop  4096 Feb 25 06:31 plugins
-rw-rw-r--. 1 hadoop hadoop  8700 Aug 24  2016 README.textile
[hadoop@HadoopMaster elasticsearch-2.4.3]$ cd plugins/
[hadoop@HadoopMaster plugins]$ ll
total 12
drwxrwxr-x. 5 hadoop hadoop 4096 Feb 22 05:28 head
drwxrwxr-x. 3 hadoop hadoop 4096 Feb 25 06:32 ik
drwxrwxr-x. 8 hadoop hadoop 4096 Feb 22 05:34 kopf
[hadoop@HadoopMaster plugins]$ cd ik/
[hadoop@HadoopMaster ik]$ ll
total 5828
-rw-r--r--. 1 hadoop hadoop  263965 Dec  1  2015 commons-codec-1.9.jar
-rw-r--r--. 1 hadoop hadoop   61829 Dec  1  2015 commons-logging-1.2.jar
drwxr-xr-x. 3 hadoop hadoop    4096 Jan  1 12:46 config
-rw-r--r--. 1 hadoop hadoop   55998 Jan  1 13:27 elasticsearch-analysis-ik-1.10.3.jar
-rw-r--r--. 1 hadoop hadoop 4505518 Jan 15 08:59 elasticsearch-analysis-ik-1.10.3.zip
-rw-r--r--. 1 hadoop hadoop  736658 Jan  1 13:26 httpclient-4.5.2.jar
-rw-r--r--. 1 hadoop hadoop  326724 Jan  1 13:07 httpcore-4.4.4.jar
-rw-r--r--. 1 hadoop hadoop    2667 Jan  1 13:27 plugin-descriptor.properties
[hadoop@HadoopMaster ik]$ cd config/
[hadoop@HadoopMaster config]$ ll
total 3016
drwxr-xr-x. 2 hadoop hadoop    4096 Jan  1 12:46 custom
-rw-r--r--. 1 hadoop hadoop     697 Dec 14 10:34 IKAnalyzer.cfg.xml
-rw-r--r--. 1 hadoop hadoop 3058510 Dec 14 10:34 main.dic
-rw-r--r--. 1 hadoop hadoop     123 Dec 14 10:34 preposition.dic
-rw-r--r--. 1 hadoop hadoop    1824 Dec 14 10:34 quantifier.dic
-rw-r--r--. 1 hadoop hadoop     164 Dec 14 10:34 stopword.dic
-rw-r--r--. 1 hadoop hadoop     192 Dec 14 10:34 suffix.dic
-rw-r--r--. 1 hadoop hadoop     752 Dec 14 10:34 surname.dic
[hadoop@HadoopMaster config]$ cd custom/
[hadoop@HadoopMaster custom]$ ll
total 5252
-rw-r--r--. 1 hadoop hadoop     156 Dec 14 10:34 ext_stopword.dic
-rw-r--r--. 1 hadoop hadoop     130 Dec 14 10:34 mydict.dic
-rw-r--r--. 1 hadoop hadoop   63188 Dec 14 10:34 single_word.dic
-rw-r--r--. 1 hadoop hadoop   63188 Dec 14 10:34 single_word_full.dic
-rw-r--r--. 1 hadoop hadoop   10855 Dec 14 10:34 single_word_low_freq.dic
-rw-r--r--. 1 hadoop hadoop 5225922 Dec 14 10:34 sougou.dic
[hadoop@HadoopMaster custom]$ vim zhouls.dic
第二步：输入，自定义词，如我这里，是好记性不如烂笔头感叹号博客园
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225091545710-970384771.png)
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225091623788-402488199.png)
[hadoop@HadoopMaster custom]$ pwd
/home/hadoop/app/elasticsearch-2.4.3/plugins/ik/config/custom
[hadoop@HadoopMaster custom]$ vim zhouls.dic
[hadoop@HadoopMaster custom]$ cat zhouls.dic 
好记性不如烂笔头感叹号博客园
[hadoop@HadoopMaster custom]$
　　第三步：修改ik的配置文件
自然而然，我刚加入的自定义词库zhouls.dic是需要配置，才可以生效的。
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225092200413-658073595.png)
[hadoop@HadoopMaster custom]$ ll
total 5256
-rw-r--r--. 1 hadoop hadoop     156 Dec 14 10:34 ext_stopword.dic
-rw-r--r--. 1 hadoop hadoop     130 Dec 14 10:34 mydict.dic
-rw-r--r--. 1 hadoop hadoop   63188 Dec 14 10:34 single_word.dic
-rw-r--r--. 1 hadoop hadoop   63188 Dec 14 10:34 single_word_full.dic
-rw-r--r--. 1 hadoop hadoop   10855 Dec 14 10:34 single_word_low_freq.dic
-rw-r--r--. 1 hadoop hadoop 5225922 Dec 14 10:34 sougou.dic
-rw-rw-r--. 1 hadoop hadoop      43 Feb 25 17:16 zhouls.dic
[hadoop@HadoopMaster custom]$ cd ..
[hadoop@HadoopMaster config]$ ll
total 3016
drwxr-xr-x. 2 hadoop hadoop    4096 Feb 25 17:16 custom
-rw-r--r--. 1 hadoop hadoop     697 Dec 14 10:34 IKAnalyzer.cfg.xml
-rw-r--r--. 1 hadoop hadoop 3058510 Dec 14 10:34 main.dic
-rw-r--r--. 1 hadoop hadoop     123 Dec 14 10:34 preposition.dic
-rw-r--r--. 1 hadoop hadoop    1824 Dec 14 10:34 quantifier.dic
-rw-r--r--. 1 hadoop hadoop     164 Dec 14 10:34 stopword.dic
-rw-r--r--. 1 hadoop hadoop     192 Dec 14 10:34 suffix.dic
-rw-r--r--. 1 hadoop hadoop     752 Dec 14 10:34 surname.dic
[hadoop@HadoopMaster config]$ vim IKAnalyzer.cfg.xml
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225092247913-1585334845.png)
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE properties SYSTEM "http://java.sun.com/dtd/properties.dtd">
<properties>
        <comment>IK Analyzer 扩展配置</comment>
        <!--用户可以在这里配置自己的扩展字典 -->
        <entry key="ext_dict">custom/mydict.dic;custom/single_word_low_freq.dic</entry>
         <!--用户可以在这里配置自己的扩展停止词字典-->
        <entry key="ext_stopwords">custom/ext_stopword.dic</entry>
        <!--用户可以在这里配置远程扩展字典 -->
        <!-- <entry key="remote_ext_dict">words_location</entry> -->
        <!--用户可以在这里配置远程扩展停止词字典-->
        <!-- <entry key="remote_ext_stopwords">words_location</entry> -->
</properties>
　　修改为
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225092613898-334034402.png)
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225092638116-2000619480.png)
[hadoop@HadoopMaster config]$ vim IKAnalyzer.cfg.xml 
[hadoop@HadoopMaster config]$ cat IKAnalyzer.cfg.xml 
?<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE properties SYSTEM "http://java.sun.com/dtd/properties.dtd">
<properties>
	<comment>IK Analyzer 扩展配置</comment>
	<!--用户可以在这里配置自己的扩展字典 -->
	<entry key="ext_dict">custom/mydict.dic;custom/single_word_low_freq.dic;custom/zhouls.dic</entry>
	 <!--用户可以在这里配置自己的扩展停止词字典-->
	<entry key="ext_stopwords">custom/ext_stopword.dic</entry>
	<!--用户可以在这里配置远程扩展字典 -->
	<!-- <entry key="remote_ext_dict">words_location</entry> -->
	<!--用户可以在这里配置远程扩展停止词字典-->
	<!-- <entry key="remote_ext_stopwords">words_location</entry> -->
</properties>
[hadoop@HadoopMaster config]$ 
 　　第四步：重启es服务进程
 为了更好地看出效果，启动es服务进程，我用bin/elasticsearch。一般建议用bin/elasticsearch -d。当然，生产环境下，可以将其设置为服务进程，作为service下的一种服务进程，这样更为方便。
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225092848445-471547190.png)
[hadoop@HadoopMaster plugins]$ cd ..
[hadoop@HadoopMaster elasticsearch-2.4.3]$ jps
1974 Elasticsearch
2137 Jps
[hadoop@HadoopMaster elasticsearch-2.4.3]$ kill -9 1974
[hadoop@HadoopMaster elasticsearch-2.4.3]$ jps
2148 Jps
[hadoop@HadoopMaster elasticsearch-2.4.3]$ bin/elasticsearch
[2017-02-25 17:27:56,301][WARN ][bootstrap                ] unable to install syscall filter: seccomp unavailable: requires kernel 3.5+ with CONFIG_SECCOMP and CONFIG_SECCOMP_FILTER compiled in
[2017-02-25 17:27:57,741][INFO ][node                     ] [Tethlam] version[2.4.3], pid[2158], build[d38a34e/2016-12-07T16:28:56Z]
[2017-02-25 17:27:57,741][INFO ][node                     ] [Tethlam] initializing ...
[2017-02-25 17:27:59,504][INFO ][plugins                  ] [Tethlam] modules [lang-groovy, reindex, lang-expression], plugins [analysis-ik, kopf, head], sites [kopf, head]
[2017-02-25 17:27:59,553][INFO ][env                      ] [Tethlam] using [1] data paths, mounts [[/home (/dev/sda5)]], net usable_space [23.4gb], net total_space [26.1gb], spins? [possibly], types [ext4]
[2017-02-25 17:27:59,553][INFO ][env                      ] [Tethlam] heap size [1015.6mb], compressed ordinary object pointers [true]
[2017-02-25 17:27:59,553][WARN ][env                      ] [Tethlam] max file descriptors [4096] for elasticsearch process likely too low, consider increasing to at least [65536]
[2017-02-25 17:28:02,922][INFO ][ik-analyzer              ] try load config from /home/hadoop/app/elasticsearch-2.4.3/config/analysis-ik/IKAnalyzer.cfg.xml
[2017-02-25 17:28:02,923][INFO ][ik-analyzer              ] try load config from /home/hadoop/app/elasticsearch-2.4.3/plugins/ik/config/IKAnalyzer.cfg.xml
[2017-02-25 17:28:03,748][INFO ][ik-analyzer              ] [Dict Loading] custom/mydict.dic
[2017-02-25 17:28:03,749][INFO ][ik-analyzer              ] [Dict Loading] custom/single_word_low_freq.dic
[2017-02-25 17:28:03,755][INFO ][ik-analyzer              ] [Dict Loading] custom/zhouls.dic
[2017-02-25 17:28:03,760][INFO ][ik-analyzer              ] [Dict Loading] custom/ext_stopword.dic
[2017-02-25 17:28:06,914][INFO ][node                     ] [Tethlam] initialized
[2017-02-25 17:28:06,915][INFO ][node                     ] [Tethlam] starting ...
[2017-02-25 17:28:07,168][INFO ][transport                ] [Tethlam] publish_address {192.168.80.10:9300}, bound_addresses {[::]:9300}
[2017-02-25 17:28:07,203][INFO ][discovery                ] [Tethlam] elasticsearch/dXjRTwNJRdyzQWPbHIzGiQ
[2017-02-25 17:28:10,589][INFO ][cluster.service          ] [Tethlam] detected_master {Peter Parker}{3TwJeRfnRH-EttHntj0OdQ}{192.168.80.11}{192.168.80.11:9300}, added {{Peter Parker}{3TwJeRfnRH-EttHntj0OdQ}{192.168.80.11}{192.168.80.11:9300},{Living Laser}{bqV_F5bLRdq9AGtv3WLx4A}{192.168.80.12}{192.168.80.12:9300},}, reason: zen-disco-receive(from master [{Peter Parker}{3TwJeRfnRH-EttHntj0OdQ}{192.168.80.11}{192.168.80.11:9300}])
[2017-02-25 17:28:10,920][INFO ][http                     ] [Tethlam] publish_address {192.168.80.10:9200}, bound_addresses {[::]:9200}
[2017-02-25 17:28:10,923][INFO ][node                     ] [Tethlam] started
　　第五步：测试效果，验证下
![](https://images2015.cnblogs.com/blog/855959/201702/855959-20170225093150835-999790441.png)
[hadoop@HadoopMaster elasticsearch-2.4.3]$ jps
2280 Jps
2231 Elasticsearch
[hadoop@HadoopMaster elasticsearch-2.4.3]$ curl 'http://192.168.80.10:9200/zhouls/_analyze?analyzer=ik_max_word&pretty=true' -d '{"text":"好记性不如烂笔头感叹号博客园"}'
{
  "tokens" : [ {
    "token" : "好记性不如烂笔头感叹号博客园",
    "start_offset" : 0,
    "end_offset" : 14,
    "type" : "CN_WORD",
    "position" : 0
  }, {
    "token" : "好记",
    "start_offset" : 0,
    "end_offset" : 2,
    "type" : "CN_WORD",
    "position" : 1
  }, {
    "token" : "记性",
    "start_offset" : 1,
    "end_offset" : 3,
    "type" : "CN_WORD",
    "position" : 2
  }, {
    "token" : "不如",
    "start_offset" : 3,
    "end_offset" : 5,
    "type" : "CN_WORD",
    "position" : 3
  }, {
    "token" : "烂",
    "start_offset" : 5,
    "end_offset" : 6,
 "type" : "CN_CHAR",
    "position" : 4
  }, {
    "token" : "笔头",
    "start_offset" : 6,
    "end_offset" : 8,
    "type" : "CN_WORD",
    "position" : 5
  }, {
    "token" : "笔",
    "start_offset" : 6,
    "end_offset" : 7,
    "type" : "CN_WORD",
    "position" : 6
  }, {
    "token" : "头",
    "start_offset" : 7,
    "end_offset" : 8,
    "type" : "CN_CHAR",
    "position" : 7
  }, {
    "token" : "感叹号",
    "start_offset" : 8,
    "end_offset" : 11,
    "type" : "CN_WORD",
    "position" : 8
  }, {
    "token" : "感叹",
    "start_offset" : 8,
    "end_offset" : 10,
 "type" : "CN_WORD",
    "position" : 9
  }, {
    "token" : "叹号",
    "start_offset" : 9,
    "end_offset" : 11,
    "type" : "CN_WORD",
    "position" : 10
  }, {
    "token" : "叹",
    "start_offset" : 9,
    "end_offset" : 10,
    "type" : "CN_WORD",
    "position" : 11
  }, {
    "token" : "号",
    "start_offset" : 10,
    "end_offset" : 11,
    "type" : "CN_CHAR",
    "position" : 12
  }, {
    "token" : "博客园",
    "start_offset" : 11,
    "end_offset" : 14,
    "type" : "CN_WORD",
    "position" : 13
  }, {
    "token" : "博客",
    "start_offset" : 11,
    "end_offset" : 13,
 "type" : "CN_WORD",
    "position" : 14
  }, {
    "token" : "园",
    "start_offset" : 13,
    "end_offset" : 14,
    "type" : "CN_CHAR",
    "position" : 15
  } ]
}
[hadoop@HadoopMaster elasticsearch-2.4.3]$ 
 　　可以看出，我们自定义词库zhouls.dic里的自定义词，我这里是，好记性不如烂笔头感叹号博客园。看作是一个词。成功被整体切分出来！
 　　说明成功！
