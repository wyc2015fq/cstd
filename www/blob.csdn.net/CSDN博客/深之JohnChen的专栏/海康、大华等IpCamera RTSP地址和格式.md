# 海康、大华等IpCamera RTSP地址和格式 - 深之JohnChen的专栏 - CSDN博客

2016年06月12日 16:33:50[byxdaz](https://me.csdn.net/byxdaz)阅读数：7828


**实时流**

海康

rtsp://[username]:[password]@[ip]:[port]/[codec]/[channel]/[subtype]/av_stream

说明：

username: 用户名。例如admin。

password: 密码。例如12345。

ip: 为设备IP。例如 192.0.0.64。

port: 端口号默认为554，若为默认可不填写。

codec：有h264、MPEG-4、mpeg4这几种。

channel: 通道号，起始为1。例如通道1，则为ch1。

subtype: 码流类型，主码流为main，子码流为sub。

例如，请求海康摄像机通道1的主码流，Url如下

主码流：

rtsp://admin:12345@192.0.0.64:554/h264/ch1/main/av_stream

rtsp://admin:12345@192.0.0.64:554/MPEG-4/ch1/main/av_stream

子码流：

rtsp://admin:12345@192.0.0.64/mpeg4/ch1/sub/av_stream

rtsp://admin:12345@192.0.0.64/h264/ch1/sub/av_stream

大华

rtsp://username:password@ip:port/cam/realmonitor?channel=1&subtype=0

说明:

username: 用户名。例如admin。

password: 密码。例如admin。

ip: 为设备IP。例如 10.7.8.122。

port: 端口号默认为554，若为默认可不填写。

channel: 通道号，起始为1。例如通道2，则为channel=2。

subtype: 码流类型，主码流为0（即subtype=0），子码流为1（即subtype=1）。

例如，请求某设备的通道2的子码流，Url如下

rtsp://admin:admin@10.12.4.84:554/cam/realmonitor?channel=2&subtype=1

雄迈/巨峰

默认IP地址：192.168.1.10 用户名admin 密码空

端口：TCP端口：34567 和 HTTP端口：80，onvif端口是8899

RTSP地址：rtsp://10.6.3.57:554/user=admin&password=&channel=1&stream=0.sdp?real_stream

10.6.3.57这个是被连接的设备的IP

554这个是RTSP服务的端口号，可以在设备的网络服务里面更改

user=admin这个是设备的登录用户名

password= 密码空

channel=1 第一通道

stream=0.sdp？主码流

stream=1.sdp？副码流

参考：[http://www.xiongmaitech.com/index.php/service/problem_info/140/39](http://www.xiongmaitech.com/index.php/service/problem_info/140/39)

天视通

默认IP地址：192.168.0.123 用户名admin 密码123456

端口：http端口80 数据端口8091 RTSP端口554 ONVIF端口 80

RTSP地址：主码流地址:rtsp://192.168.0.123:554/mpeg4

子码流地址:rtsp://192.168.0.123:554/mpeg4cif

需要入密码的地址： 主码流 rtsp://admin:123456@192.168.0.123:554/mpeg4

子码流 rtsp://admin:123456@192.168.0.123:554/mpeg4cif

中维/尚维

默认IP地址：DHCP 默认用户名admin 默认密码 空

RTSP地址：rtsp://0.0.0.0:8554/live1.264（次码流）

rtsp://0.0.0.0:8554/live0.264 (主码流)

九安

RTSP地址：rtsp://IP:port（website port）/ch0_0.264（主码流）

rtsp://IP:port（website port）/ch0_1.264（子码流）

技威/YOOSEE

默认IP地址：DHCP 用户名admin 密码123

RTSP地址：主码流：rtsp://IPadr:554/onvif1

次码流：rtsp://IPadr:554/onvif2

onvif端口是5000

设备发现的端口是3702

V380

默认IP地址：DHCP 用户名admin 密码空/admin

onvif端口8899

RTSP地址：主码流rtsp://ip//live/ch00_1

子码流rtsp://ip//live/ch00_0

宇视

默认IP地址： 192.168.0.13/DHCP 默认用户名 admin 和默认密码 123456

端口：HTTP 80/RTSP 554/HTTPS 110(443)/onvif端口 80

RTSP地址：rtsp://用户名:密码@ip:端口号/video123 123对应3个码流

天地伟业

默认IP地址：192.168.1.2 用户名“Admin”、密码“1111”

onvif端口号“8080”

RTSP地址：rtsp：//192.168.1.2

巨龙/JVT

默认IP地址：192.168.1.88 默认用户名 admin 默认密码admin

RTSP地址：

主码流地址:rtsp://IP地址/av0_0

次码流地址:rtsp://IP地址/av0_1

onvif端口 2000

**回放流**

海康

海康回放流（模拟通道）：rtsp://admin:12345@192.0.0.64:554/Streaming/tracks/101?starttime=20120802t063812z&endtime=20120802t064816z

海康回放流（IP通道）：  rtsp://admin:12345@192.0.0.64:554/Streaming/tracks/1701?starttime=20120802t063812z&endtime=20120802t064816z


说明：

101：1，表示通道1；01，表示以单播形式回放指定设备的通道中的录像文件

1701：17，表示通道17，折换成ip通道1，主要看DVR是几路的，有一定换算关系。

starttime：开始时间

endtime：结束时间

时间范围是starttime到endtime，其中starttime和endtime的格式要符合ISO 8601。具体格式是YYYYMMDD”T”HHmmSS.fraction”Z” ，Y是年，M是月，D是日，T是时间分格符，H是小时，M是分，S是秒，Z是可选的、表示Zulu (GMT) 时间。

大华

rtsp://admin:12345@172.22.0.26:554/cam/playback？channel=1&subtype=0&starttime=2016_04_18_11_50_00&endtime=2016_04_18_12_50_00

说明：

playback表示回放

channel=number表示通道号

subtype=0表示主码流

starttime=YYYY_MM_DD_HH_mm_SS表示回放开始时间

endtime=YYYY_MM_DD_HH_mm_SS表示回放结束时间

**抓拍**

雄迈/巨峰

图片抓拍地址：http://ip/webcapture.jpg?command=snap&channel=1

天视通

图片抓拍地址：http://ip/snapshot.cgi

巨龙/JVT

图片抓拍地址：http://ip/capture/webCapture.jpg?channel=1&FTpsend=0&checkinfo=0

http://ip/cgi-bin/images_cgi?channel=1&user=admin&pwd=admin

