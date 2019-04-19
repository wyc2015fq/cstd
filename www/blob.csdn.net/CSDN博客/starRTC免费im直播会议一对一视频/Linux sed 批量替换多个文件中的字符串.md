# Linux sed 批量替换多个文件中的字符串 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月09日 17:30:47[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：318
sed -i "s/oldstring/newstring/g" `grep oldstring -rl yourdir`
例如：替换/home下所有文件中的www.bcak.com.cn为bcak.com.cn
sed -i "s/www.bcak.com.cn/bcak.com.cn/g" `grep[www.bcak.com.cn](http://www.bcak.com.cn/) -rl
 /home`

sed -i "s/ray/eses/g" `grep ray -rl modules`
sed -i "s/w3school/starconnect/g"
`grep w3school -rl downloaddir`
方便下载整个站点后修改

[http://blog.csdn.net/kauu/article/details/1757325](http://blog.csdn.net/kauu/article/details/1757325)
