# Freeswitch总结大全

2019年02月25日 15:38:41 [春雨里de太阳](https://me.csdn.net/qq_16633405) 阅读数：106



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_16633405/article/details/87918031



### 文章目录

- - [1、Freeswitch安装](https://blog.csdn.net/qq_16633405/article/details/87918031#1Freeswitch_5)
  - [2、Freeswitch中文文档](https://blog.csdn.net/qq_16633405/article/details/87918031#2Freeswitch_8)
  - [3、Freeswitch的event socket event list的中文简介](https://blog.csdn.net/qq_16633405/article/details/87918031#3Freeswitchevent_socket_event_list_12)
  - [4、freeswitch之sip协议的注册、呼叫、挂断流程](https://blog.csdn.net/qq_16633405/article/details/87918031#4freeswitchsip_15)
  - [5、Freeswitch之mod_cdr_csv](https://blog.csdn.net/qq_16633405/article/details/87918031#5Freeswitchmod_cdr_csv_18)
  - [6、一款第三方收费的mod_vad（看介绍挺不错的，有做语音交互的童鞋可以看下）](https://blog.csdn.net/qq_16633405/article/details/87918031#6mod_vad_22)
  - [7、FreeSWITCH通过迅时网关连接PSTN](https://blog.csdn.net/qq_16633405/article/details/87918031#7FreeSWITCHPSTN_25)
  - [8、老李写的Freeswitch简要教程（新手可以看下）](https://blog.csdn.net/qq_16633405/article/details/87918031#8Freeswitch_28)
  - [9、FreeSwitch 终端命令详细介绍](https://blog.csdn.net/qq_16633405/article/details/87918031#9FreeSwitch__31)
  - [10、FreeSwitch配置文件目录说明](https://blog.csdn.net/qq_16633405/article/details/87918031#10FreeSwitch_36)
  - [11、FreeSwitch之Python+ESL](https://blog.csdn.net/qq_16633405/article/details/87918031#11FreeSwitchPythonESL_39)
  - [12、FreeSwitch中电话语音文件地址](https://blog.csdn.net/qq_16633405/article/details/87918031#12FreeSwitch_43)
  - [13、FreeSWITCH中的lua操作小结](https://blog.csdn.net/qq_16633405/article/details/87918031#13FreeSWITCHlua_47)
  - [14、Freeswitch官方文档之lua指南](https://blog.csdn.net/qq_16633405/article/details/87918031#14Freeswitchlua_52)
  - [15、某大神写的一系列关于FS的文章](https://blog.csdn.net/qq_16633405/article/details/87918031#15FS_55)
  - [16、SIP响应状态码及含义](https://blog.csdn.net/qq_16633405/article/details/87918031#16SIP_58)
  - [17、freeswitch离线对帐话单的产生及保存方式及配置说明](https://blog.csdn.net/qq_16633405/article/details/87918031#17freeswitch_61)
  - [18、Freeswitch挂断原因汇总](https://blog.csdn.net/qq_16633405/article/details/87918031#18Freeswitch_64)
  - [19、Freeswitch 常用指令 fs_cli指令](https://blog.csdn.net/qq_16633405/article/details/87918031#19Freeswitch__fs_cli_67)
  - [20、Freeswitch源码地址](https://blog.csdn.net/qq_16633405/article/details/87918031#20Freeswitch_70)
  - [21、开源软交换系统 FreeSwitch 与 Asterisk 比较](https://blog.csdn.net/qq_16633405/article/details/87918031#21_FreeSwitch__Asterisk__74)
  - [22、Freeswitch目录结构](https://blog.csdn.net/qq_16633405/article/details/87918031#22Freeswitch_77)

之前做一个智能外呼的项目，就顺带玩了下Freeswitch，这里分享下保存的一些干货，供后来者参考。

PS：文档部分最好参照官方文档，在官方文档上找不到解决方案的再参考其他的解决办法，这样可以避免走弯路。

参考书籍：老杜的《Freeswitch权威指南》



## 1、Freeswitch安装

参考：<https://freeswitch.org/confluence/display/FREESWITCH/CentOS+7+and+RHEL+7>

## 2、Freeswitch中文文档

参考（需翻墙）：<http://wiki.freeswitch.org.cn/wiki/Mod_lua.html>

## 3、Freeswitch的event socket event list的中文简介

参考：<https://xsxjb.iteye.com/blog/1867120>

## 4、freeswitch之sip协议的注册、呼叫、挂断流程

参考：<https://blog.csdn.net/hry2015/article/details/78347467>

## 5、Freeswitch之mod_cdr_csv

参考：<https://freeswitch.org/confluence/display/FREESWITCH/mod_cdr_csv>

## 6、一款第三方收费的mod_vad（看介绍挺不错的，有做语音交互的童鞋可以看下）

参考：[http://www.ddrj.com/smartivr/mod_vad.html#start-asr-说明](http://www.ddrj.com/smartivr/mod_vad.html#start-asr-%E8%AF%B4%E6%98%8E)

## 7、FreeSWITCH通过迅时网关连接PSTN

参考：<http://www.freeswitch.org.cn/2014/06/08/FreeSWITCH-interop-newrock.html>

## 8、老李写的Freeswitch简要教程（新手可以看下）

参考：<http://freeswitch.net.cn/119.html>

## 9、FreeSwitch 终端命令详细介绍

参考：
<https://blog.csdn.net/u010317005/article/details/52212043/>
<https://blog.csdn.net/wh8_2011/article/details/79888774>

## 10、FreeSwitch配置文件目录说明

参考：<http://blog.sina.com.cn/s/blog_c245b24d0102xonm.html>

## 11、FreeSwitch之Python+ESL

参考：<https://freeswitch.org/confluence/display/FREESWITCH/Python+ESL>

## 12、FreeSwitch中电话语音文件地址

参考：<http://files.freeswitch.org/releases/sounds/>

## 13、FreeSWITCH中的lua操作小结

<https://blog.csdn.net/iware99/article/details/78069155>
更多lua操作请参考FS权威指南

## 14、Freeswitch官方文档之lua指南

参考：<https://freeswitch.org/confluence/display/FREESWITCH/Lua+API+Reference>

## 15、某大神写的一系列关于FS的文章

参考：<https://www.cnblogs.com/cnsanshao/category/450398.html>

## 16、SIP响应状态码及含义

参考：<https://blog.csdn.net/hello_zyg/article/details/79125049>

## 17、freeswitch离线对帐话单的产生及保存方式及配置说明

参考：<https://blog.csdn.net/y_xianjun/article/details/38225261>

## 18、Freeswitch挂断原因汇总

参考：<https://wsonh.com/article/45.html>

## 19、Freeswitch 常用指令 fs_cli指令

参考：<https://blog.csdn.net/weixin_38260044/article/details/81022455>

## 20、Freeswitch源码地址

参考：<https://freeswitch.org/fisheye/browse/>

## 21、开源软交换系统 FreeSwitch 与 Asterisk 比较

参考：<https://blog.csdn.net/educast/article/details/17921215>

## 22、Freeswitch目录结构

参考：<http://freeswitch.net.cn/113.html>

智能外呼系统相关资料总结请参考：<https://blog.csdn.net/qq_16633405/article/details/87934824>
语音交互相关资料总结请参考：
<https://blog.csdn.net/qq_16633405/article/details/87920034>