
# Python Paramiko模块的使用实际案例 - jiahaowanhao的博客 - CSDN博客


2018年04月20日 21:45:46[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：207


[Python Paramiko模块的使用实际案例](http://cda.pinggu.org/view/25325.html)
本文研究的主要是Python Paramiko模块的使用的实例，具体如下。
Windows下有很多非常好的SSH客户端，比如Putty。在python的世界里，你可以使用原始套接字和一些加密函数创建自己的SSH客户端或服务端，但如果有现成的模块，为什么还要自己实现呢。使用Paramiko库中的PyCrypto能够让你轻松使用SSH2协议。
Paramiko的安装方法网上有很多这样的帖子，这里就不描述了。这里主要讲如何使用它。Paramiko实现SSH2不外乎从两个角度实现：SSH客户端与服务端。
首先让我们理清以下几个名词：
SSHClient：包装了Channel、Transport、SFTPClient
Channel：是一种类Socket，一种安全的SSH传输通道；
Transport：是一种加密的会话（但是这样一个对象的Session并未建立），并且创建了一个加密的tunnels，这个tunnels叫做Channel；
Session：是client与Server保持连接的对象，用connect()/start_client()/start_server()开始会话。
下面给出几个常用的使用案例：
SSH客户端实现方案一，执行远程命令
这个方案直接使用SSHClient对象的exec_command()在服务端执行命令，下面是具体代码：
\#实例化SSHClient
client = paramiko.SSHClient()
\#自动添加策略，保存服务器的主机名和密钥信息
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
\#连接SSH服务端，以用户名和密码进行认证
client.connect(ip,username=user,password=passwd)
\#打开一个Channel并执行命令
stdin,stdout,stderr = client.exec_command(command)
\#打印执行结果
print stdout.readlines()
\#关闭SSHClient
client.close()
SSH客户端实现方案二，执行远程命令
这个方案是将SSHClient建立连接的对象得到一个Transport对象，以Transport对象的exec_command()在服务端执行命令，下面是具体代码：
\#实例化SSHClient
client = paramiko.SSHClient()
\#自动添加策略，保存服务器的主机名和密钥信息
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
\#连接SSH服务端，以用户名和密码进行认证
client.connect(ip,username=user,password=passwd)
\#实例化Transport，并建立会话Session
ssh_session = client.get_transport().open_session()
if ssh_session.active:
ssh_session.exec_command(command)
print ssh_session.recv(1024)
client.close()
SSH服务端的实现
实现SSH服务端必须继承ServerInterface，并实现里面相应的方法。具体代码如下：
import socket
import sys
import threading
import paramiko
host_key = paramiko.RSAKey(filename='private_key.key')
class Server(paramiko.ServerInterface):
def __init__(self):
\#执行start_server()方法首先会触发Event，如果返回成功，is_active返回True
self.event = threading.Event()
\#当is_active返回True，进入到认证阶段
def check_auth_password(self, username, password):
if (username == 'root') and (password == '123456'):
return paramiko.AUTH_SUCCESSFUL
return paramiko.AUTH_FAILED
\#当认证成功，client会请求打开一个Channel
def check_channel_request(self, kind, chanid):
if kind == 'session':
return paramiko.OPEN_SUCCEEDED
\#命令行接收ip与port
server = sys.argv[1]
ssh_port = int(sys.argv[2])
\#建立socket
try:
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  \#TCP socket
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind((server, ssh_port))
sock.listen(100)
print '[+] Listening for connection ...'
client, addr = sock.accept()
except Exception, e:
print '[-] Listen failed: ' + str(e)
sys.exit(1)
print '[+] Got a connection!'
try:
\#用sock.accept()返回的socket实例化Transport
bhSession = paramiko.Transport(client)
\#添加一个RSA密钥加密会话
bhSession.add_server_key(host_key)
server = Server()
try:
\#启动SSH服务端
bhSession.start_server(server=server)
except paramiko.SSHException, x:
print '[-] SSH negotiation failed'
chan = bhSession.accept(20)
print '[+] Authenticated!'
print chan.recv(1024)
chan.send("Welcome to my ssh")
while True:
try:
command = raw_input("Enter command:").strip("\n")
if command != 'exit':
chan.send(command)
print chan.recv(1024) + '\n'
else:
chan.send('exit')
print 'exiting'
bhSession.close()
raise Exception('exit')
except KeyboardInterrupt:
bhSession.close()
except Exception, e:
print '[-] Caught exception: ' + str(e)
try:
bhSession.close()
except:
pass
sys.exit(1)
使用SFTP上传文件
import paramiko
\#获取Transport实例
tran = paramiko.Transport(("host_ip",22))
\#连接SSH服务端
tran.connect(username = "username", password = "password")
\#获取SFTP实例
sftp = paramiko.SFTPClient.from_transport(tran)
\#设置上传的本地/远程文件路径
localpath="/root/Desktop/python/NewNC.py"
remotepath="/tmp/NewNC.py"
\#执行上传动作
sftp.put(localpath,remotepath)
tran.close()
使用SFTP下载文件
import paramiko
\#获取SSHClient实例
client = paramiko.SSHClient()
client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
\#连接SSH服务端
client.connect("host_ip",username="username",password="password")
\#获取Transport实例
tran = client.get_transport()
\#获取SFTP实例
sftp = paramiko.SFTPClient.from_transport(tran)
remotepath='/tmp/NewNC.py'
localpath='/root/Desktop/NewNC.py'
sftp.get(remotepath, localpath)
client.close()
总结
以上就是本文关于Python Paramiko模块的使用实际案例的全部内容，希望对大家有所帮助。

