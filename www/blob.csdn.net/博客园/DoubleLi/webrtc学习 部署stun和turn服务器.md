# webrtc学习: 部署stun和turn服务器 - DoubleLi - 博客园






webrtc的P2P穿透部分是由libjingle实现的. 

步骤顺序大概是这样的: 

1. 尝试直连.

2. 通过stun服务器进行穿透

3. 无法穿透则通过turn服务器中转. 



stun 服务器比较简单. 网上也有很多公开的stun服务器可以用于[测试](http://lib.csdn.net/base/softwaretest). 例如 stun.ideasip.com

这里需要注意一下. 我在做[Android](http://lib.csdn.net/base/android)应用时. 在少数老旧的手机上出现过一个bug: 

PeerConnection close时非常慢. 大概需要50~80s. 

后来反复检查, 才发现问题出在公用的stun服务器响应比较慢. 

解决办法就是在自己搭一个stun服务器即可. 

具体原因应该是: 一直在进行ice探测(stun bind request). 关闭的时候worker 线程因为网络卡住了, 然后main线程等待. 这个不太好查. 可能需要打很多log来定位代码. 



无法穿透的时候就需要 turn 服务器来保证视频通话的成功率. 

turn 包含了stun的功能. 所以只需要部署turn服务器即可. 

服务器代码可以从这里获取: https://code.google.com/p/coturn/

coturn 在原来的turnserver上增加了一些高级特性. (这是作者的描述, 两个项目是同一个作者. 作者更推荐使用coturn). 

coturn  支持tcp, udp, tls, dtls 连接.

coturn 支持[Linux](http://lib.csdn.net/base/linux) bsd solaris mac os. 暂不支持windows (未来会支持)

下面说一下安装和配置. 

1. coturn 的底层网络部分依赖libevent.  所以需要先安装libevent2, 地址在此 http://libevent.org/

2. coturn的安装很简单. configure make make install 三部曲就完事了. 

3. coturn的文档说明挺详细的. 但比较多. 我只是大概说明下. 

更具体的说明可以看 源码目录下的README.turnserver README.turnadmin README.turnutils



在bin目录下生成六个可执行文件

turnadmin turnutils_peer turnutils_stunclient
turnserver turnutils_rfc5769check turnutils_uclient

turnserver 就是我们需要的服务器. 

turnadmin 用来管理账户. 

turnutils_stunclient 用于测试stun服务

turnutils_uclient 用于测试turn服务. 模拟多个UDP,TCP,TLS or DTLS 类型的客户端



example 目录主要是示范如何配置和使用turn. 包含一些测试用例. 

example/etc 下是pem证书和conf配置文件

example/var/db 下是sqlite的db库. 用于示范[数据库](http://lib.csdn.net/base/mysql)的格式.

coturn 支持三种配置. 命令行, conf文件和数据库. 数据库支持sqlite, [MySQL](http://lib.csdn.net/base/mysql), postgresql, [MongoDB](http://lib.csdn.net/base/mongodb), [Redis](http://lib.csdn.net/base/redis).



examples\scripts 下一些测试用例: 

loadbalance 示范如何进行负载均衡.  设置一个master turn server 然后配置若干个slave turn server. 

longtermsecure 示范如何使用long-term 验证

longtermsecuredb 与 longtermsecure 类似, 不过是从数据库配置

shorttermsecure 示范如何使用short-term验证. 

restapi 示范了web方面的使用. 



stun 定义了两种验证方式. 
Long-Term CredentialShort-Term Credential


具体可以参考stun标准 http://tools.ietf.org/html/rfc5389#section-15.4

但是对于webrtc而言. 仅支持long-term . 



http://www.ietf.org/proceedings/87/slides/slides-87-behave-10.pdf
TURN REST Server API

这个PDF 描述了. turn服务器和客户端的交互流程.



4. 简单的使用

turnserver -o -a -f -v --mobility -m 10 --max-bps=100000 --min-port=32355 --max-port=65535 --user=ling:ling1234 --user=ling2:ling1234 -r demo

-m 10 表示启动十个relay线程. 

当TURN Server用于WebRTC时，必须使用long-term credential mechanism,  即指定 -a 或者 --lt-cred-mech

--max-bps=100000 限制最大速度为100KB/s. 

添加了两个用户ling 和ling2. 

可以直接使用turnutils_uclient -u ling -w ling1234 来测试. turnutils_uclient 有很多参数可以配置的. 





**[javascript]**[view plain](http://blog.csdn.net/wojiaopanpan/article/details/50698184#)[copy](http://blog.csdn.net/wojiaopanpan/article/details/50698184#)



- webrtc::PeerConnectionInterface::IceServer turnServer;  
- turnServer.uri = "turn:ip";  
- turnServer.username = "ling";  
- turnServer.password = "ling1234";  
- servers.push_back(turnServer);  




c++ webrtc中可以这样配置turn. web/android/[iOS](http://lib.csdn.net/base/ios) 也是类似的. 

如果从安全性方面考虑. 可以为账户生成key, 这样就可以直接通过key登录. 而不需要提供密码. 

key 通过turnadmin生成, 需要(username, password, realm) . realm是启动turn server时-r 参数指定的. 



**[javascript]**[view plain](http://blog.csdn.net/wojiaopanpan/article/details/50698184#)[copy](http://blog.csdn.net/wojiaopanpan/article/details/50698184#)



- turnadmin -k -u ling -p ling1234 -r demo  
- 0xccba8f3a6a025a38eb4a0e795fc92705  










