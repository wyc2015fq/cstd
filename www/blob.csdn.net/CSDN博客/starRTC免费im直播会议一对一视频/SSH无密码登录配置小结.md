# SSH无密码登录配置小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月18日 15:41:09[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：227
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)

**ssh-keygen**-t rsa  **//**-t指定算法
将公钥复制到被管理机器上面
ssh-copy-id -i ~**/**.ssh**/**id_rsa.pub 172.29.0.89
ssh-copy-id -i ~**/**.ssh**/**id_rsa.pub 172.29.0.90
ssh-keygen -t rsa命令会创建.ssh/id_rsa、id_rsa.pub文件，其中第一个为密钥，第二个为公钥。过程中会要求输入密码，为了ssh访问过程无须密码，可以直接回车 。
如果有端口：
ssh-copy-id -i ~**/**.ssh**/**id_rsa.pub "root@192.168.160.44 -p 60022"
之后就可以通过ssh 172.29.0.89而不用输入密码了。
如果想彼此都不用输入密码：
在Node1上:
*# ssh-keygen -t rsa**# ssh-copy-id -i ~/.ssh/id_rsa.pub root@node2.elesos.com*
在Node2上:
*# ssh-keygen -t rsa**# ssh-copy-id -i ~/.ssh/id_rsa.pub root@node1.elesos.com*
## 艺搜参考
[http://cn.soulmachine.me//blog/20120102/](http://cn.soulmachine.me//blog/20120102/)
[http://wz102.blog.51cto.com/3588520/1071944](http://wz102.blog.51cto.com/3588520/1071944)
[http://blog.csdn.net/leexide/article/details/17252369](http://blog.csdn.net/leexide/article/details/17252369)
[http://unix.stackexchange.com/questions/29401/is-it-possible-to-run-ssh-copy-id-on-port-other-than-22](http://unix.stackexchange.com/questions/29401/is-it-possible-to-run-ssh-copy-id-on-port-other-than-22)
