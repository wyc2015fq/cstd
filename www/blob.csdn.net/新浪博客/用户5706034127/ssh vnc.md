# ssh vnc_用户5706034127_新浪博客
|||
ssh 每一次，都代表依次远程链接。。
shh node78，表示登录到节点78，只不过，有的不需要密码而已。
你竟然以为是，切换cpu节点！我去，。
vnc也是如此
vnc在远程服务安装并器配置vnc服务端，
在本地安装vnc客户端。
ssh登录到服务端，使用vncpasswd命令创建自己的vnc登录密码，如下所示：
![ssh <wbr>vnc](http://s2.sinaimg.cn/mw690/006e9W8vzy7cVeG1Trza1&690)
然后运行命令vncserver，新建一个vnc桌面，可以看到返回一个端口号，如下所示，
![ssh <wbr>vnc](http://s10.sinaimg.cn/mw690/006e9W8vzy7cVeFQAJP19&690)
然后客户端vnc，输入IP：端口号。
ip是你服务器的ip。
端口号是vncserver返回的数字加上5900，如下所示，
![ssh <wbr>vnc](http://s2.sinaimg.cn/mw690/006e9W8vzy7cVeFN40Fc1&690)
然后输入你第一步设置的密码即可，不需要输入用户名。只需要密码。如下所示：
![ssh <wbr>vnc](http://s8.sinaimg.cn/mw690/006e9W8vzy7cVeFXAqj27&690)
用完之后，用vncserver -kill :2 命令杀掉你第二步创建的vnc桌面服务。如下所示：
![ssh <wbr>vnc](http://s13.sinaimg.cn/mw690/006e9W8vzy7cVeFqSW82c&690)
vnc 服务端配置，：
[http://blog.csdn.net/gg296231363/article/details/6899655](http://blog.csdn.net/gg296231363/article/details/6899655)
vnc闪退问题：
记得查看log文件。。
Fatal IO error 11 (Resource temporarily unavailable) on X
server :2.
vnc fatal IO error：
http://blog.itpub.net/35489/viewspace-1967731/
https://ubuntuforums.org/archive/index.php/t-1922978.html
