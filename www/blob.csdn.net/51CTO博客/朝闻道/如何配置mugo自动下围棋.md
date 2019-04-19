# 如何配置mugo自动下围棋-朝闻道-51CTO博客
mugo是一个开源围棋程序
开源地址
[https://github.com/brilee/MuGo](https://github.com/brilee/MuGo)
下载代码之后 安装依赖 注意安装 gtp 要使用命令 pip install pygtp 不是 pip install gtp 否则后来会报错 显示没有 engine 
然后下载棋谱
[https://u-go.net/gamerecords/](https://u-go.net/gamerecords/)
下载下来之后放到一个目录
执行python main.py preprocess data 这个data就是你放棋谱的目录进行棋谱的预处理
之后会生成一个processed_data
然后训练AI
python main.py train processed_data/ --save-file=/tmp/savedmodel --epochs=1 --logdir=logs/my_training_run
训练完之后 启动AI试一下
python main.py gtp policy --read-file=/tmp/savedmodel
注意这个程序是没有界面的 也就是你没法操作
要使用界面
参考下面的
4.下载 KGS client
[http://www.gokgs.com/download.jsp](http://www.gokgs.com/download.jsp)
下载 kgsGtp-3.5.22.zip 到安装 MuGo 所在的机器
5.注册 KGS 账号
在 [http://www.gokgs.com/](http://www.gokgs.com/) 下载客户端，注册账号，注意要先以 guest 身份登录，再用注册邮箱中的密码登录后改密码。
可以注册两个 KGS 账号，一个用来对战，一个用来观看比赛。
- 用 MuGo 在 KGS 对战
参考 [https://www.linux.ime.usp.br/~willian/willian/private/go/kgsGtp.xhtml4](https://www.linux.ime.usp.br/~willian/willian/private/go/kgsGtp.xhtml4)
在 MuGo 所在机器上:
unzip kgsGtp-3.5.22.zip
cd kgsGtp-3.5.22
vim mugo.config
mugo.config 内容如下（需要根据实际情况做修改）：
复制代码
name=xxx
password=xxxxxxx
room=Computer Go
mode=auto
automatch.speed=blitz,medium
automatch.rank=10k
talk=Good evening. I am an AI and please do not talk with me.
reconnect=true
verbose=t
gtp.out=gtp.log
engine=python3 /home/user/MuGo/main.py gtp policy --read-file=/home/user/MuGo/saved_models/
复制代码
运行下面的命令，启动GoEngine
java -jar kgsGtp.jar mugo.config
这个AI就会使用这个账号去进入下棋的网站![如何配置mugo自动下围棋](https://s1.51cto.com/images/blog/201809/30/1703224ec0ab69617141b6b358d29832.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
