# fluentd - 三少GG - CSDN博客
2014年01月28日 22:45:51[三少GG](https://me.csdn.net/scut1135)阅读数：1299

### 各種 Distributed Log collection framework 
Open Source 的迷人之處，同時也是最讓人頭痛的地方就是選擇太多，迫使我們常常必須在這麼多Solution中找出適合自己專案的來用，如果每個Solution 都有專門擅長的領域，或是使用的語言差異很大，那可能還好選擇，就怕是技術同質性高的東西。
舉例來說，以前就知道光是Message Queue (MQ) 就有很多種選擇，但是前一陣子去參加ApacheCon 居然又看到好幾種新的MQ專案，感覺同質性也非常高，雖說會要開發新的一定是為了要解決某種特殊問題，或是覺得舊的不夠用....不過沒花時間深入研究的話是根本比較不出來，所以到現在都還在頭痛中，到底該怎麼選擇...... (找機會再來整理一下各種MQ Solution )。
不過今天不是要談MQ是要談Distributed Log collection framework(DLCF)，主要是看到了Treasure Data的這個投影片 "[The Basic of Fluentd](http://www.slideshare.net/treasure-data/the-basics-of-fluentd)"，裡面有針對幾個比較熱門的DLCF做比較，於是我就想是應該把這一陣子有看到的都整理一下了，因為選擇也越來越多了！！
### **Fluentd (It's like syslog,but uses JSON for log message) **
官網：[http://fluentd.org/](http://fluentd.org/)
GitHub: [https://github.com/fluent/fluentd](https://github.com/fluent/fluentd)
### **Scribe (Log collector for Facebook )-----> Calligraphus **
GitHub: [https://github.com/facebook/scribe](https://github.com/facebook/scribe)
Wiki: [https://github.com/facebook/scribe/wiki/Scribe-Overview](https://github.com/facebook/scribe/wiki/Scribe-Overview)
Ps. 在這篇[投影片](http://www.slideshare.net/slrash/2011-0630hadoopsummit-v5-8469751)裡面有提到Facebook 打算從Scribe (C based) 換成 Calligraphus (Java Based)
### **Flume (Distributed log collector by Cloudera now in Apache)**
官網：[http://flume.apache.org/FlumeUserGuide.html](http://flume.apache.org/FlumeUserGuide.html)
Wiki: [https://cwiki.apache.org/FLUME/home.html](https://cwiki.apache.org/FLUME/home.html)
GitHub: [https://github.com/cloudera/flume](https://github.com/cloudera/flume)
### **Chukwa**
官網：[http://incubator.apache.org/chukwa/](http://incubator.apache.org/chukwa/)
### **BigStream (Streams is inspired by Chukwa)**
官網：[http://code.google.com/p/bigstreams/](http://code.google.com/p/bigstreams/)
### l**ogstash  (logstash is a tool for managing events and logs)**
官網：[http://logstash.net/](http://logstash.net/)
是不是很多！！為什麼大家都要來搞一套呢？光是Apache 就有兩三套....不過由Molliza 的案例應該就可以了解為什麼會有那麼多Log framework，話說在2010年初Molliza使用的技術是Flume + Hive，在這篇Molliza 的Blog
[Collecting and analyzing log data via Flume and Hive](http://blog.mozilla.org/data/2010/08/15/collecting-and-analyzing-log-data-via-flume-and-hive/) 裡面提到：
> Chukwa, Scribe and Flume are headed in the right direction, but the final piece of puzzle of analyzing the data still remained unsolved, until few weeks back as we, at Mozilla, started integrating Flume with Hive.
不過呢，才到了2010年底， 內部就有不同的聲音了"[Flume, Hive and realtime indexing via ElasticSearch](http://blog.mozilla.org/data/2010/12/30/flume-hive-and-realtime-indexing-via-elasticsearch-2/)"，好還要在更好是吧~
> *Flume + Hive really solves our needs, but we would ideally like a solution that indexes our data and can be queried in real-time*
所以這次ApacheCon 2013 Mozilla的人去演講的題目 "[Firefox Crash Analysis Laura Thomson](http://na.apachecon.com/schedule/presentation/158/)" 就是在講他們自己新開發的系統：
### **[Socorro](https://wiki.mozilla.org/Socorro) (A server for collecting, processing, and displaying crash reports from clients using the Breakpad libraries)**
Wiki: [https://wiki.mozilla.org/Socorro](https://wiki.mozilla.org/Socorro)
GitHub: [https://github.com/mozilla/socorro](https://github.com/mozilla/socorro)
主要的Use Case 就是如果Firefox 當機了，必須要知道以下資訊：怎麼當的，版本，那一個build?OS?channel，是新發現的問題還是舊的？
有興趣的人可以去這邊下載[投影片](http://archive.apachecon.com/na2013/presentations/27-Wednesday/Big_Data/10:15-Firefox_Crash_Analysis-Laura_Thomson/apachecon_firefox_crash_reports.key.pdf)來看
