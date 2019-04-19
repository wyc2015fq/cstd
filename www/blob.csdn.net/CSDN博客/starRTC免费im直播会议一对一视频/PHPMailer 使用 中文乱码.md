# PHPMailer 使用 中文乱码 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月18日 17:40:17[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：322
WordPress在用。
You'll find plenty more to play with in the [examples](https://github.com/PHPMailer/PHPMailer/tree/master/examples) folder.
中文乱码问题：
$mail->CharSet = "utf-8"; //设置字符集编码
SMTP ERROR: Password command failed: 535 Error: 请使用授权码登录
这个要用跟域名后缀相同的发件人才行。
QQ企业邮箱$mail->Host = 'smtp.exmail.qq.com';
//QQ邮箱//$mail->Host = 'smtp.qq.com';
另外最好用tls和587，不要用ssl和465
[https://github.com/PHPMailer/PHPMailer](https://github.com/PHPMailer/PHPMailer)
