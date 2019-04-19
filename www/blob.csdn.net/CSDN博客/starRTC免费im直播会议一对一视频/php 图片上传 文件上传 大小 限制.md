# php 图片上传  文件上传 大小 限制 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月17日 09:14:43[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1370

nginx 
413 Request Entity Too Large
# Php无法上传文件
查看php脚本运行用户，写个php脚本
<?php
echo shell_exec("id -a");
http.conf中
User apache
Group apache
修改网站目录用户为apache即可。

# 修改php上传文件大小限制
打开php.ini，首先找到
file_uploads = on ;是否允许通过HTTP上传文件的开关。默认为ON即是开
upload_tmp_dir ;文件上传至服务器上存储临时文件的地方，如果没指定就会用系统默认的临时文件夹
upload_max_filesize = 20m ;望文生意，即允许上传文件大小的最大值。默认为2M
post_max_size = 8m ;指通过表单POST给PHP的所能接收的最大值，包括表单里的所有值。默认为8M
一般地，设置好上述四个参数后，上传<=8M的文件是不成问题，在网络正常的情况下。
但如果要上传>8M的大体积文件，只设置上述四项还一定能行的通。
进一步配置以下的参数
max_execution_time = 600 ;每个PHP页面运行的最大时间值**(**秒**)**，默认30秒
max_input_time = 600 ;每个PHP页面接收数据所需的最大时间，默认60秒
memory_limit = 8m ;每个PHP页面所吃掉的最大内存，默认8M
## nginx设置
修改nginx.conf 文件，找到http{}段，查找client_max_body_size修改或者添加
client_max_body_size 20m;
把上述参数修改后，在网络所允许的正常情况下，就可以上传大体积文件了。
## 艺搜参考
[http://www.metsky.com/archives/567.html](http://www.metsky.com/archives/567.html)

nginx 
413 Request Entity Too Large
# Php无法上传文件
查看php脚本运行用户，写个php脚本
<?php
echo shell_exec("id -a");
http.conf中
User apache
Group apache
修改网站目录用户为apache即可。

# 修改php上传文件大小限制
打开php.ini，首先找到
file_uploads = on ;是否允许通过HTTP上传文件的开关。默认为ON即是开
upload_tmp_dir ;文件上传至服务器上存储临时文件的地方，如果没指定就会用系统默认的临时文件夹
upload_max_filesize = 20m ;望文生意，即允许上传文件大小的最大值。默认为2M
post_max_size = 8m ;指通过表单POST给PHP的所能接收的最大值，包括表单里的所有值。默认为8M
一般地，设置好上述四个参数后，上传<=8M的文件是不成问题，在网络正常的情况下。
但如果要上传>8M的大体积文件，只设置上述四项还一定能行的通。
进一步配置以下的参数
max_execution_time = 600 ;每个PHP页面运行的最大时间值(秒)，默认30秒
max_input_time = 600 ;每个PHP页面接收数据所需的最大时间，默认60秒
memory_limit = 8m ;每个PHP页面所吃掉的最大内存，默认8M
## nginx设置
修改nginx.conf 文件，找到http{}段，查找client_max_body_size修改或者添加
client_max_body_size 20m;
把上述参数修改后，在网络所允许的正常情况下，就可以上传大体积文件了。
## 艺搜参考
[http://www.metsky.com/archives/567.html](http://www.metsky.com/archives/567.html)
