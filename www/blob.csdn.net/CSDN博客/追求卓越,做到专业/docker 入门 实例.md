
# docker 入门 实例 - 追求卓越,做到专业 - CSDN博客


2019年04月12日 02:10:33[Waldenz](https://me.csdn.net/enter89)阅读数：16


## 先决条件
验证是否已经做好准备
`docker run hello-world`![](https://img-blog.csdnimg.cn/20190412005716185.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
## 使用Dockerfile定义容器
Dockerfile将在您的容器内定义环境中执行的操作。对网络接口和磁盘驱动器的访问在此内环境内实现虚拟化，这将独立于系统的其余部分，因此您必须将端口映射到外度，并具体说明您要“复制”到该环境的文件。但是，在执行此操作后，您可以期望此Dockerfile中定义的应用构建的行为在运行时始终相同。
以下为Dockerfile的一个例子
`# 将官方 Python 运行时用作父镜像
FROM python:2.7-slim
# 将工作目录设置为 /app
WORKDIR /app
# 将当前目录内容复制到位于 /app 中的容器中
ADD . /app
# 安装 requirements.txt 中指定的任何所需软件包
RUN pip install -r requirements.txt
# 使端口 80 可供此容器外的环境使用
EXPOSE 80
# 定义环境变量
ENV NAME World
# 在容器启动时运行 app.py
CMD ["python", "app.py"]`其他两个文件的内容：
requirements.txt
`Flask
Redis`app.py
```python
from flask import Flask
from redis import Redis, RedisError
import os
import socket
# Connect to Redis
redis = Redis(host="redis", db=0, socket_connect_timeout=2, socket_timeout=2)
app = Flask(__name__)
@app.route("/")
def hello():
    try:
        visits = redis.incr("counter")
    except RedisError:
        visits = "<i>cannot connect to Redis, counter disabled</i>"
    html = "<h3>Hello {name}!</h3>" \
           "<b>Hostname:</b> {hostname}<br/>" \
           "<b>Visits:</b> {visits}"
    return html.format(name=os.getenv("NAME", "world"), hostname=socket.gethostname(), visits=visits)
if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)
```
现在我们可以看到，在Dockerfile中的pip install -r requirements.txt将安装python的Flask和Redis库，而此应用将输出换将变量NAME以及对 socket.gethostname() 调用的输出。最后，由于Redis未在运行（因为我们仅安装了python库，而未安装Redis自身），因此我们应该期望尝试在此使用它将失败，并生成错误信息。
** 在容器内时，访问主机的名称将检索容器ID,这类似于正在运行的可执行文件的进程ID。
## 构建应用
进入到文件夹，并查看，文件夹下只有上述的三个文件
![](https://img-blog.csdnimg.cn/2019041201194410.png)
现在，运行构建命令。将创建Docker镜像，使用 -t 对其进行标记，该名称即为生成的镜像的repository名称。
`docker build -t friendlyhello .`![](https://img-blog.csdnimg.cn/20190412012447811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
通过 docker images 命令 查看位于您机器上的所有本地docker镜像库
![](https://img-blog.csdnimg.cn/20190412012658920.png)
## 运行应用
运行应用，使用 -p 参数将机器的4000端口映射到容器暴露的80端口：
`docker run -p 4000:80 friendlyhello`![](https://img-blog.csdnimg.cn/20190412013710885.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
看到python正在为应用提供服务（网址为http://0.0.0.0:80）的输出通知。但是，该消息来自容器的内部，内部不知道您已经将80端口映射到4000，因此争取的可访问的url为  http://localhost:4000
在浏览器中访问该URL，显示了 Hello World, 容器ID, 以及redis的错误信息
![](https://img-blog.csdnimg.cn/2019041201413740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
也可以在shell中使用curl命令查看相同内容。
![](https://img-blog.csdnimg.cn/20190412014337368.png)
** 此端口映射4000:80 说明您在Dockerfile中暴露的内容与使用docker run -p 发布的内容之间的差异。
上述的run命令，会保留控制台运行，当然可以直接设置器后台运行，即分离模式下运行
`docker run -d -p 4000:80 friendlyhello`![](https://img-blog.csdnimg.cn/20190412014743350.png)
该命令返回的是长容器ID。 可以使用 docker ps 查看 当前运行的容器
如果想停止镜像，结束进程，执行 docker stop  [CONTAINER ID] 命令。
停止后，继续docker ps查看，当前无正在运行的容器。
![](https://img-blog.csdnimg.cn/20190412015300936.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
以上就是一个基本完整的创建镜像，运行容器的例子。
参考：[https://docs.docker-cn.com/get-started/part2/](https://docs.docker-cn.com/get-started/part2/#%E4%BB%8E%E8%BF%9C%E7%A8%8B%E9%95%9C%E5%83%8F%E4%BB%93%E5%BA%93%E4%B8%AD%E6%8B%89%E5%8F%96%E5%B9%B6%E8%BF%90%E8%A1%8C%E9%95%9C%E5%83%8F)

