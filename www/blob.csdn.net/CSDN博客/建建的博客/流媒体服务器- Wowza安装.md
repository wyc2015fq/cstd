# 流媒体服务器- Wowza安装 - 建建的博客 - CSDN博客
2018年09月26日 09:06:44[纪建](https://me.csdn.net/u013898698)阅读数：68
**1、下载地址：[https://www.wowza.com/pricing/installer](https://www.wowza.com/pricing/installer)**
**2、申请注册key（180天试用）**
打开下载地址，
![这里写图片描述](https://img-blog.csdn.net/20170401142151383?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpeXVhbndlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3、安装参考手册：[http://wowza.cn/tech/quickStart](http://wowza.cn/tech/quickStart)**
**4、登录管理界面**
[http://192.168.8.138:8088/enginemanager/ftu/welcome.htm](http://192.168.8.138:8088/enginemanager/ftu/welcome.htm)（ip输入自己的就好了）
**5、服务器推流访问设置**
application –Source Security 右边选项第一个勾上保存
![这里写图片描述](https://img-blog.csdn.net/20170401142434856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpeXVhbndlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**6、服务器直播参数播设置**
设置推流方式:rtmp，直播完后保存录像等等。具体按下图勾选保存
![这里写图片描述](https://img-blog.csdn.net/20170401142802705?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpeXVhbndlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**6、配置文件的修改**
编辑安装流媒体程序中 conf 文件夹下的Server.xml的两项配置
修改AuthenticationMethod为none
修改IpwhiteList 为部署Api接口主机所在的网段
设置后可以调用流媒体服务器的Rest Api 接口
列如：获取当前流媒体的直播流
[defaultServer/vhosts/*defaultVHost*/applications/live/instances/*definst*“>http://localhost:8087/v2/servers/*defaultServer*/vhosts/*defaultVHost*/applications/live/instances/*definst*]()
具体如下图：
![这里写图片描述](https://img-blog.csdn.net/20170401142925738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpeXVhbndlaQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**7、可能用到的命令**
vnc和windows之间的复制
vncconfig -nowin&
可以通过强制关掉yum进程：
rm -f /var/run/yum.pid
查看文件大小
du -sh *
查看8088端口是否被占用
netstat -aon|findstr “8088”
查看8088端口是被什么进程占用
tasklist|findstr “2724”
结束进程
taskkill /pid “5432”
windows 配置防火墙，控制面板-防火墙-高级设置-新建规则，写入api端口和1935流媒体端口
--------------------- 本文来自 白垣伟 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/baiyuanwei/article/details/68944267?utm_source=copy
