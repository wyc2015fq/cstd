# 查看logstash 导入数据到es中，数据导入的数量 - sxf_123456的博客 - CSDN博客
2017年12月25日 09:58:31[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1750

查看logstash 导入数据到es中，数据导入的数量
stat /home/raw_data/8_31/* #查看文件状态，
查看logstash处理文件进度记录
input {
file {
path => [
"/home/raw_data/8_31/*.csv"
]
start_position => "beginning" # 从什么位置读取
sincedb_path => "/home/es/sincedb/apk"
type => "apk"
tags => ["hgw", "gather"]
}
}
filter
{
if [type] == "apk"
{
csv
{
columns => ["Type","ProbeOUI","ProbeVersion","MAC",
"UploadTime","IptvAccount","STBId","OUI","ProductClass",
"SoftwareVersion","HardwareVersion","IpAddress","TeleOUI",
"TeleProductClass","TeleResolution","LogUploadInterval",
"ReportSerialNumber","WorkingTime","CPURate","MEMRate",
"FrameLR","FrameDelay","FrameJitter","VideoStreamingRate",
"RTPLossRate","RTPThroughPut","MdiMLR","MdiType","MdiDF","MAXMdiDF",
"Jitter","MAXJitter","ResponseDelay","ChannelSwitchDelay","StreamBreak",
"EPGDelay","ResourceURL","EPGVisitNum","EPGSuccessNum",
"KaNum","KaDelay"]
separator => "|"
quote_char => "‰"
}
if ([Type]!="1" and [ProbeOUI]!="YUCHUANG"){
drop{}
}
}
}
output{
if [type] == "apk"
{
elasticsearch
{
hosts => ["10.0.0.20:9200"]
index => "ana-%{type}"
document_type => "%{type}"
flush_size => 8000
idle_flush_time => 10
sniffing => true
template_overwrite => true
codec => "json"
}
}
}
1、/home/raw_data/8_31/目录下所有的.csv文件都将处理
2、第一次处理文件是从头开始
3、处理之后以json的格式输出到elasticsearch中
logstash在文件处理过程将文件的处理进度写入到sincedb中
例子中将sincedb制定到/home/es/sincedb/apk，可以通过apk查看logstash导入的进度
第一列为inode ,第二列为主设备号，第三列为次设备号，第四列为文件大小
8589938532 0 64770 634171044
8589938540 0 64770 657879019
通过stat命令查看文件信息
[root@localhost 8_31]# stat /home/raw_data/8_31/*
文件："./14_26.csv"
  大小：634171044 块：1238616    IO 块：4096   普通文件
设备：fd02h/64770dInode：8589938532  硬链接：1
权限：(0644/-rw-r--r--)  Uid：(    0/    root)   Gid：(    0/    root)
环境：unconfined_u:object_r:home_root_t:s0
最近访问：2017-08-31 17:06:34.969528969 +0800
最近更改：2017-08-31 14:26:00.000000000 +0800
最近改动：2017-08-31 16:23:06.779714540 +0800
创建时间：-
文件："./14_31.csv"
  大小：657879019 块：1284920    IO 块：4096   普通文件
设备：fd02h/64770dInode：8589938540  硬链接：1
权限：(0644/-rw-r--r--)  Uid：(    0/    root)   Gid：(    0/    root)
环境：unconfined_u:object_r:home_root_t:s0
最近访问：2017-08-31 17:20:29.541469590 +0800
最近更改：2017-08-31 14:30:59.000000000 +0800
最近改动：2017-08-31 16:23:37.275712370 +0800
如果往这两个文件追加数据则将往下继续处理，而且也会将进度更新到sincedb文件中。
如果没有处理完了关闭logstash，下次再启动时则不会再从头开始处理，因为sincedb已经记录了进度，不要以为start_position => “beginning”就是每次都从头处理，如果把sincedb文件删了又会从头开始处理。
            
