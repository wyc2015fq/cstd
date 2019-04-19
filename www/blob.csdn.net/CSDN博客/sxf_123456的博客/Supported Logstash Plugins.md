# Supported Logstash Plugins - sxf_123456的博客 - CSDN博客
2017年08月02日 17:07:37[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：137
个人分类：[ELK](https://blog.csdn.net/sxf_123456/article/category/7036606)
#### Supported Logstash Plugins
These plugins are maintained and supported by Elastic. The full list of plugins in the Logstash ecosystem can be found in the product documentation:
- [Input plugins](https://www.elastic.co/guide/en/logstash/current/input-plugins.html)
- [Codec plugins](https://www.elastic.co/guide/en/logstash/current/codec-plugins.html)
- [Filter plugins](https://www.elastic.co/guide/en/logstash/current/filter-plugins.html)
- [Output plugins](https://www.elastic.co/guide/en/logstash/current/output-plugins.html)
- |Input Plugins|Codec plugins|Filter plugins|Output plugins|
|----|----|----|----|
| | | | |
|[beats](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-beats.html)| | | |
|[couchdb_changes](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-couchdb_changes.html)| | | |
|[e](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-elasticsearch.html)[lasticsearch](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-elasticsearch.html)| | | |
|[file](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-file.html)| | | |
|[gelf](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-gelf.html)| | | |
|[generator](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-generator.html)| | | |
|[graphite](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-graphite.html)| | | |
|[heartbeat](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-heartbeat.html)| | | |
|[http](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-http.html)| | | |
|[http_poller](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-http_poller.html)| | | |
|[jdbc](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-jdbc.html)| | | |
|[kafka](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-kafka.html)| | | |
|[log4j](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-log4j.html)| | | |
|[rabbitmq](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-rabbitmq.html)| | | |
|[redis](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-redis.html)| | | |
|[s3](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-s3.html)| | | |
|[sqs](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-sqs.html)| | | |
|[stdin](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-stdin.html)| | | |
|[syslog](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-syslog.html)| | | |
|[tcp](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-tcp.html)| | | |
|[twitter](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-twitter.html)| | | |
|[udp](https://www.elastic.co/guide/en/logstash/current/plugins-inputs-udp.html)| | | |
| | | | |
|[avro](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-avro.html)| | | |
|[cef](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-cef.html)| | | |
|[collectd](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-collectd.html)| | | |
|[dots](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-dots.html)| | | |
|[es_bulk](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-es_bulk.html)| | | |
|[fluent](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-fluent.html)| | | |
|[graphite](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-graphite.html)| | | |
|[json](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-json.html)| | | |
|[json_lines](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-json_lines.html)| | | |
|[line](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-line.html)| | | |
|[msgpack](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-msgpack.html)| | | |
|[multiline](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-multiline.html)| | | |
|[plain](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-plain.html)| | | |
|[rubydebug](https://www.elastic.co/guide/en/logstash/current/plugins-codecs-rubydebug.html)| | | |
| | | | |
|[aggregate](https://www.elastic.co/guide/en/logstash/current/plugins-filters-aggregate.html)| | | |
|[anonymize](https://www.elastic.co/guide/en/logstash/current/plugins-filters-anonymize.html)| | | |
|[csv](https://www.elastic.co/guide/en/logstash/current/plugins-filters-csv.html)| | | |
|[date](https://www.elastic.co/guide/en/logstash/current/plugins-filters-date.html)| | | |
|[de_dot](https://www.elastic.co/guide/en/logstash/current/plugins-filters-de_dot.html)| | | |
|[dissect](https://www.elastic.co/guide/en/logstash/current/plugins-filters-dissect.html)| | | |
|[dns](https://www.elastic.co/guide/en/logstash/current/plugins-filters-dns.html)| | | |
|[drop](https://www.elastic.co/guide/en/logstash/current/plugins-filters-drop.html)| | | |
|[elasticsearch](https://www.elastic.co/guide/en/logstash/current/plugins-filters-elasticsearch.html)| | | |
|[fingerprint](https://www.elastic.co/guide/en/logstash/current/plugins-filters-fingerprint.html)| | | |
|[geoip](https://www.elastic.co/guide/en/logstash/current/plugins-filters-geoip.html)| | | |
|[grok](https://www.elastic.co/guide/en/logstash/current/plugins-filters-grok.html)| | | |
|[jdbc_streaming](https://www.elastic.co/guide/en/logstash/current/plugins-filters-jdbc_streaming.html)| | | |
|[json](https://www.elastic.co/guide/en/logstash/current/plugins-filters-json.html)| | | |
|[kv](https://www.elastic.co/guide/en/logstash/current/plugins-filters-kv.html)| | | |
|[mutate](https://www.elastic.co/guide/en/logstash/current/plugins-filters-mutate.html)| | | |
|[ruby](https://www.elastic.co/guide/en/logstash/current/plugins-filters-ruby.html)| | | |
|[sleep](https://www.elastic.co/guide/en/logstash/current/plugins-filters-sleep.html)| | | |
|[split](https://www.elastic.co/guide/en/logstash/current/plugins-filters-split.html)| | | |
|[throttle](https://www.elastic.co/guide/en/logstash/current/plugins-filters-throttle.html)| | | |
|[syslog_pri](https://www.elastic.co/guide/en/logstash/current/plugins-filters-syslog_pri.html)| | | |
|[translate](https://www.elastic.co/guide/en/logstash/current/plugins-filters-translate.html)| | | |
|[truncate](https://www.elastic.co/guide/en/logstash/current/plugins-filters-truncate.html)| | | |
|[urldecode](https://www.elastic.co/guide/en/logstash/current/plugins-filters-urldecode.html)| | | |
|[useragent](https://www.elastic.co/guide/en/logstash/current/plugins-filters-useragent.html)| | | |
|[uuid](https://www.elastic.co/guide/en/logstash/current/plugins-filters-uuid.html)| | | |
|[xml](https://www.elastic.co/guide/en/logstash/current/plugins-filters-xml.html)| | | |
| | | | |
|[csv](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-csv.html)| | | |
|[elasticsearch](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-elasticsearch.html)| | | |
|[email](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-email.html)| | | |
|[file](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-file.html)| | | |
|[graphite](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-graphite.html)| | | |
|[http](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-http.html)| | | |
|[kafka](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-kafka.html)| | | |
|[rabbitmq](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-rabbitmq.html)| | | |
|[redis](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-redis.html)| | | |
|[s3](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-s3.html)| | | |
|[stdout](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-stdout.html)| | | |
|[tcp](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-tcp.html)| | | |
|[udp](https://www.elastic.co/guide/en/logstash/current/plugins-outputs-udp.html)| | | |
- 
