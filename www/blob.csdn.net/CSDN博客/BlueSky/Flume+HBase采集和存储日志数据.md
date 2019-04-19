# Flume+HBase采集和存储日志数据 - BlueSky - CSDN博客
2016年03月08日 00:19:05[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：5745
**转自：http://blog.csdn.net/yaoyasong/article/details/39400829**
**前言**
大数据时代，谁掌握了足够的数据，谁就有可能掌握未来，而其中的数据采集就是将来的流动资产积累。
几乎任何规模企业，每时每刻也都在产生大量的数据，但这些数据如何归集、提炼始终是一个困扰。而大数据技术的意义确实不在于掌握规模庞大的数据信息，而在于对这些数据进行智能处理，从中分析和挖掘出有价值的信息，但前提是如何获取大量有价值的数据。
在最近的工作当中，本人刚好实现了运用大数据技术分析网站访问日志的方案，整个方案包括对网站日志的采集、清洗、存储和统计分析，计划通过几篇文章将技术实现细节分享出来，以期引起更多的思考和讨论。
**网站访问日志介绍**
相信很多做过网站管理的人对网站访问日志（Access Log）应该不会陌生，现在主流的网站服务器（如apache,tomcat,ngxin等）都支持将日志数据记录到服务器的日志文件中。
网站的访问日志中记录了很多有用的信息，比如正常用户的访问足迹、恶意捣乱的足迹、用户的入站方式、出站页面等等信息。对以上信息汇总分类后，可以得到更有价值的东西，比如可以得到搜索引擎的抓取频率和来访时间段、可以得到哪些页面是用户热搜的等等。
首先看一个访问日志的例子：
10.52.10.49 - - [17/Sep/2014:11:34:21 +0800] "GET /webapp HTTP/1.1" 302 - "-" "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36"
这是一个combined格式的访问日志，里面记录了用户的访问ip、时间、访问地址、来源地址等。如要了解具体的格式说明，请查看相关资料。
**日志采集存储方案**
对于一个比较活跃的网站来说，访问日志将会是一个海量的数据，考虑到网站日志更新频繁、和海量数据的特点，我选择了Flume + HBase的采集和存储方案。
**Flume**
Flume最早是Cloudera提供的日志收集系统，目前是Apache下的一个项目，Flume支持在日志系统中定制各类数据发送方，用于收集数据。
Flume提供对数据进行简单处理，并写到各种数据接受方（可定制）的能力 Flume提供了从console（控制台）、RPC（Thrift-RPC）、text（文件）、tail（UNIX
 tail）、syslog（syslog日志系统，支持TCP和UDP等2种模式），exec（命令执行）等数据源上收集数据的能力。
![](https://img-blog.csdn.net/20140919171953000)
**HBase**
HBase – Hadoop Database，是一个高可靠性、高性能、面向列、可伸缩的分布式存储系统，利用HBase技术可在廉价PC
 Server上搭建起大规模结构化存储集群。
![](https://img-blog.csdn.net/20140919172203100)
本次方案以Tomcat为Web服务器，通过Flume实时监控网站的日志文件并将新增日志收集、清洗并保存到HBase中，供Spark等分布计算框架分析使用等。
**![](https://img-blog.csdn.net/20140919172216033)**
**方案实现**
前提条件：
已经在linux服务器上安装并启动了相关的程序：Tomcat7,Hadoop2.4.1，Zookeeper3.4.6，HBase0.98.5，Flume1.5。具体安装步骤请自行查看相关文档。
1.首先开启Tomcat中的日志记录功能，并选择combined格式。
修改TOMCAT_PATH/conf/server.xml，增加日志记录：
<Valve className="org.apache.catalina.valves.AccessLogValve" directory="logs"
prefix="localhost_access_log." suffix=".txt" renameOnRotate="true"
pattern="combined" />
这样，tomcat就会在logs目录下每天生成localhost_access_log文件并实时记录用户的访问情况。
2.实现日志文件对象和解析程序
AccessLog.java:
public class AccessLog {
private String clientIp;
private String clientIndentity;
private String remoteUser;
private Date dateTime;
private String request;
private String httpStatusCode;
private String bytesSent;
private String referer;
private String userAgent;
…
}
AccessLogParser.java:
public class AccessLogParser {
private static String pattern = "^([\\d.]+) (\\S+) (\\S+) \\[([\\w:/]+\\s[+\\-]\\d{4})\\] \"(.+?)\" (\\d{3}) (\\d+|-) \"([^\"]+)\" \"([^\"]+)\"";
private static Pattern p = Pattern.compile(pattern);
public static AccessLog parse(String line){
Matcher matcher = p.matcher(line);
if (matcher.matches()){
AccessLog accessLog = new AccessLog();
accessLog.setClientIp(matcher.group(1));
accessLog.setClientIndentity(matcher.group(2));
accessLog.setRemoteUser(matcher.group(3));
accessLog.setDateTime(getDateTime(matcher.group(4)));
accessLog.setRequest(matcher.group(5));
accessLog.setHttpStatusCode(matcher.group(6));
accessLog.setBytesSent(matcher.group(7));
accessLog.setReferer(matcher.group(8));
accessLog.setUserAgent(matcher.group(9));
return accessLog;
}
logger.warn("This line is not a valid combined log, ignored it. -- " + line);
return null;
}
3.通过HBase Shell在HBase中建立相应的表access_log
执行：$HBASE_HOME/bin/hbase shell，进入shell命令行
create 'access_log','cb',创建access_log,和一个列族cb。因为hbase是一个列服务器，一个列族中可以增加很多列，为了性能考虑，一般不要创建多于三个列族。
出现如下提示信息，即创建成功
0 row(s) in 11.9690 seconds
=> Hbase::Table - access_log
可以通过list命令查看数据库中的表，或scan ‘access_log’,查看表中数据
4.配置Flume，实现采集和存储
在本方案中，我们要将数据存储到HBase中，所以使用flume中提供的hbase
 sink，同时，为了清洗转换日志数据，我们实现自己的AsyncHbaseEventSerializer。
public class AsyncHbaseLogEventSerializer implements AsyncHbaseEventSerializer{
private byte[] table;
private byte[] colFam;
private Event currentEvent;
private byte[][] columnNames;
private final List<PutRequest> puts = new ArrayList<PutRequest>();
private final List<AtomicIncrementRequest> incs = new ArrayList<AtomicIncrementRequest>();
private byte[] currentRowKey;
private final byte[] eventCountCol = "eventCount".getBytes();
public void initialize(byte[] table, byte[] cf) {
this.table = table;
this.colFam = cf;
}
public void configure(Context context) {
String cols = new String(context.getString("columns"));
String[] names = cols.split(",");
columnNames = new byte[names.length][];
int i = 0;
for (String name : names) {
columnNames[i++] = name.getBytes();
}
}
public void configure(ComponentConfiguration conf) {
}
public List<PutRequest> getActions() {
// Split the event body and get the values for the columns
String eventStr = new String(currentEvent.getBody());
String[] cols = logTokenize(eventStr);
puts.clear();
String req = cols[4];
String reqPath = req.split(" ")[1];
int pos = reqPath.indexOf("?");
if (pos > 0) {
reqPath = reqPath.substring(0,pos);
}
if(reqPath.length() > 1 && reqPath.trim().endsWith("/")){
reqPath = reqPath.substring(0,reqPath.length()-1);
}
String req_ts_str = cols[3];
Long currTime = System.currentTimeMillis();
String currTimeStr = null;
if (req_ts_str != null && !req_ts_str.equals("")){
SimpleDateFormat df = new SimpleDateFormat("dd/MMM/yyyy:HH:mm:ss",Locale.US);
SimpleDateFormat df2 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
try {
currTimeStr = df2.format(df.parse(req_ts_str));
currTime = df.parse(req_ts_str).getTime();
} catch (ParseException e) {
System.out.println("parse req time error,using system.current time.");
}
}
long revTs = Long.MAX_VALUE - currTime;
currentRowKey = (Long.toString(revTs) + reqPath).getBytes();
System.out.println("currentRowKey: " + new String(currentRowKey));
for (int i = 0; i < cols.length; i++){
PutRequest putReq = new PutRequest(table, currentRowKey, colFam, columnNames[i], cols[i].getBytes());
puts.add(putReq);
}
//增加列
PutRequest reqPathPutReq = new PutRequest(table, currentRowKey, colFam, "req_path".getBytes(), reqPath.getBytes());
puts.add(reqPathPutReq);
PutRequest reqTsPutReq = new PutRequest(table, currentRowKey, colFam, "req_ts".getBytes(), Bytes.toBytes(currTimeStr));
puts.add(reqTsPutReq);
String channelType = ChannelUtil.getType(cols[8]);
PutRequest channelPutReq = new PutRequest(table, currentRowKey, colFam, "req_chan".getBytes(), Bytes.toBytes(channelType));
puts.add(channelPutReq);
return puts;
}
public String[] logTokenize(String eventStr) {
String logEntryPattern = "^([\\d.]+) (\\S+) (\\S+) \\[([\\w:/]+\\s[+\\-]\\d{4})\\] \"(.+?)\" (\\d{3}) (\\d+|-) \"([^\"]+)\" \"([^\"]+)\"";
Pattern p = Pattern.compile(logEntryPattern);
Matcher matcher = p.matcher(eventStr);
if (!matcher.matches())
{
System.err.println("Bad log entry (or problem with RE?):");
System.err.println(eventStr);
return null;
}
String[] columns = new String[matcher.groupCount()];
for (int i = 0; i < matcher.groupCount(); i++)
{
columns[i] = matcher.group(i+1);
}
return columns;
}
public List<AtomicIncrementRequest> getIncrements() {
incs.clear();
incs.add(new AtomicIncrementRequest(table, "totalEvents".getBytes(), colFam, eventCountCol));
return incs;
}
public void setEvent(Event event) {
this.currentEvent = event;
}
public void cleanUp() {
table = null;
colFam = null;
currentEvent = null;
columnNames = null;
currentRowKey = null;
}
**Flume Agent配置flume-src-agent.conf**
# http://flume.apache.org/FlumeUserGuide.html#exec-source
source_agent.sources = apache_server
source_agent.sources.apache_server.type = exec
source_agent.sources.apache_server.command = tail -F /opt/muse_tomcat/logs/localhost_access_log..txt
source_agent.sources.apache_server.channels = memoryChannel
# http://flume.apache.org/FlumeUserGuide.html#memory-channel
source_agent.channels = memoryChannel
source_agent.channels.memoryChannel.type = memory
source_agent.channels.memoryChannel.capacity = 1000
source_agent.channels.memoryChannel.transactionCapacity = 100
## Send to Flume Collector on Hadoop Node
# http://flume.apache.org/FlumeUserGuide.html#avro-sink
source_agent.sinks = avro_sink
source_agent.sinks.avro_sink.type = avro
source_agent.sinks.avro_sink.hostname = 10.51.108.38
source_agent.sinks.avro_sink.port = 4545
source_agent.sinks.avro_sink.channel = memoryChannel
**Flume HBase sink配置flume-hbase.conf**
#http://flume.apache.org/FlumeUserGuide.html#avro-source
collector.sources = AvroIn
collector.sources.AvroIn.type = avro
collector.sources.AvroIn.bind = 10.51.108.38
collector.sources.AvroIn.port = 4545
collector.sources.AvroIn.channels = mc1
## Channels ##
## Source writes to 3 channels, one for each sink
collector.channels = mc1
#http://flume.apache.org/FlumeUserGuide.html#memory-channel
collector.channels.mc1.type = memory
collector.channels.mc1.capacity = 1000
## Sinks #
collector.sinks = HbaseOut
###############################################################
# HBase sink config
###############################################################
collector.sinks.HbaseOut.type = asynchbase
collector.sinks.HbaseOut.channel = mc1
collector.sinks.HbaseOut.table = access_log
collector.sinks.HbaseOut.columnFamily = cb
collector.sinks.HbaseOut.batchSize = 5
collector.sinks.HbaseOut.serializer = com.ygsoft.muse.data.util.AsyncHbaseLogEventSerializer
collector.sinks.HbaseOut.serializer.columns = host_name,remote_host,remote_user,event_ts,req,req_status,resp_bytes,ref,agent
5.运行Flume Agent和HBase
 Sink
后台方式运行Flume Agent
nohup $FLUME_HOME/bin/flume-ng agent -c $FLUME_HOME /conf -f $FLUME_HOME /conf/flume-src-agent.conf -n source_agent &
后台方式运行HBase Sink
nohup $FLUME_HOME/bin/flume-ng agent -c $FLUME_HOME/conf -f $FLUME_HOME//conf/flume-hbase.conf -n collector &
服务启动后，网站日志就会持续写入到hbase数据库中了。可以通过hbase shell查看：
hbase(main):015:0> scan 'access_log'
ROW
COLUMN+CELL
9223370625743240807/webappcolumn=cb:agent, timestamp=1411111540520, value=Jakarta Commons-HttpClient/3.1
9223370625743240807/webappcolumn=cb:event_ts, timestamp=1411111540519, value=19/Sep/2014:15:25:35 +0800
9223370625743240807/webappcolumn=cb:host_name, timestamp=1411111540519, value=10.52.10.49
9223370625743240807/webappcolumn=cb:ref, timestamp=1411111540520, value=-
...
1 row(s) in 0.3470 seconds
出现类似这样的信息就证明数据已经存放到hbase中了。
**技术点**
HBase中RowKey的设计
HBase的查询实现只提供两种方式：
1、按指定RowKey获取唯一一条记录，get方法
2、按指定的条件获取一批记录，scan方法
通过巧妙的RowKey设计使我们批量获取记录集合中的元素挨在一起（应该在同一个Region下），可以在遍历结果时获得很好的性能。
考虑到访问日志的特点，时间性比较强，我们在设计RowKey时采用了(Long.MaxValue() – requestTime) + requestPath组成rowKey。
将requestTime放入rowkey中，可以提高按时间scan的效率，查询某段时间的记录时只要设置scan.setStartRow(beginTime)和scan.setStopRow(endTime)，方便并且高效。
通过Long.MaxValue() – requestTime可以将最新的日志放在最前面。
**后续**
大数据技术应用(二) HBase中日志数据访问方式
大数据技术应用(三)应用Hadoop MapReduce和Spark做日志分析
大数据技术应用(四)应用Spark Stream做实时日志分析
**相关资料**
[http://flume.apache.org/FlumeUserGuide.html](http://flume.apache.org/FlumeUserGuide.html)
[http://www.rittmanmead.com/2014/05/trickle-feeding-log-data-into-hbase-using-flume/](http://www.rittmanmead.com/2014/05/trickle-feeding-log-data-into-hbase-using-flume/)
