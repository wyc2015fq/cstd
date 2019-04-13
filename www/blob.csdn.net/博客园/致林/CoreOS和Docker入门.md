
# CoreOS和Docker入门 - 致林 - 博客园






# [CoreOS和Docker入门](https://www.cnblogs.com/bincoding/p/6171212.html)
转载自： http://www.oschina.net/translate/coreos_and_docker_first_steps?cmp
[CoreOS](http://coreos.com/)是一个基于Linux，[systemd](http://www.freedesktop.org/wiki/Software/systemd/)和[Docker](http://www.docker.io/)的小型操作系统。在这篇文章里，我将描述我是怎么看待CoreOS/Docker的，以及初始使用它的一些步骤。
### 什么是Docker，为什么这是个好主意？
找到合适的词汇向那些没用过它的人描述是很困难的，但让我尝试一下。使用docker，你可以将软件打包进“容器”，然后可以在你自己的服务器上或者一些云服务平台（[dotcloud](https://www.dotcloud.com/)，[stackdock](https://stackdock.com/)，[digitalocean](https://www.digitalocean.com/)）上运行。软件例子如[cgit](http://git.zx2c4.com/cgit/about/)，一个快速的git网络接口。Docker将各个容器放入独立的Linux容器（[Linux container，LXC](http://en.wikipedia.org/wiki/LXC)）中，大多数是为了干净和良好定义的环境，安全不是首要因素。就有关的软件而言，它的PID是1,并拥有一个动态的主机名，动态IP地址和一个干净的文件系统。
为什么这是个好主意？它自动部署，隔离机器并抽象化。当我有多个服务器时，我可以在其中任意一个上运行同样的、无修改的Docker容器，而一点也不用关心软件（我将在这之后写cgit），因为Docker提供的环境完全一样。这使得移植没有丝毫痛苦——计划向新硬件升级、当向不同的主机转换或者因为一个服务器运行中断，它都一样。
现在你也许会说我们已经有这样一个东西，Amazon's EC2和相似的云服务存在几年了。表面上看来，它看起来非常相似，唯一不同的是你发送到EC2的是一个虚拟机，而不是Docker容器。对我而言，存在两个最大的不同：
Docker更加模块化：鉴于在一个独立的VM上，你部署的各个应用经济上不吸引人而且笨重，使用Docker的轻量容器，这成为可能。
与Docker工作有一个不同的脑力模型。使用一个虚拟的长期运行的服务器，你需要手动或者自动（如使用Puppet）来保持其运行，与之相比，你可以不再考虑服务器，开始关注软件/应用。
CoreOS有几个很牛的特点. 它以只读模式运行, 除了“state partition”, 这个地方，我们基本上使用它来存放系统单元文件，包括启动监视你的docker容器, 另外还有docker’的本地图片缓存和一些固定的区域。我喜欢只读文件系统，因为这意味着，他们很稳定，并且几乎不可能被破坏. 另外, CoreOS会自动更新而且它重用了ChromeOS的更新器. 另外，他还有一些很有趣的集群优势, 可以看这篇文章[etcd, a highly-available key-value store for service configuration and discovery](https://github.com/coreos/etcd), 以这篇文章作为基础[raft consensus algorithm](http://raftconsensus.github.io/). 他可以做出容易的master选择,但是在写作的的时候，
这还没有应用。
当然了, 如果你喜欢, 你也可以安装你最喜欢的Linux分发版，并且安装上Docker, which should be reasonable straight-forward (请允许我用几个小时介绍这篇文章[Debian’s old mount(8) version, which has a bug](http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=731574)). 从个人角度而言,我发现这是一个有趣的尝试，运行一个和Docker hosting有非常相似的约束的环境.然后，你会发现你喜欢上Docker了！
## 第一步: Docker化 cgit
你既可以使用交互式命令行手动创建一个docker容器或者使用一个Dockerfile。第一种命令行的方式是非常容易出错的，因此仅为了尝鲜，我只推荐使用Dockerfiles。一个Dockerfile的开头指定了一个标准镜像，可以是一个小型环境([busybox](https://index.docker.io/_/busybox/)) 或者更多的是一个linux发行版的最小封装，如在我们的例子里使用的是[tianon/debian](https://index.docker.io/u/tianon/debian/)。
定义了标准镜像以后, 你可以在RUN命令后面运行任意的命令，还可以在ADD命令之后添加文件. 容器内外的接口其实就是一个或者多个TCP端口. 当前比较常用的是HTTP使用的经典端口80. 最后一个需要指定的是入口(ENTRYPOINT), 就是当你运行容器的时候docker会首先执行的程序. 我们为cgit使用的Dockerfile就像这样:
FROM tianon/debian:sid
RUN apt-get update
RUN apt-get dist-upgrade -y
RUN apt-get install -y lighttpd
ADD cgit /usr/bin/cgit
ADD cgit.css /var/www/cgit/cgit.css
ADD cgit.png /var/www/cgit/cgit.png
ADD lighttpd-cgit.conf /etc/lighttpd/lighttpd-cgit.conf
ADD cgitrc /etc/cgitrc
EXPOSE 80
ENTRYPOINT ["/usr/sbin/lighttpd", "-f", "/etc/lighttpd/lighttpd-cgit.conf", "-D"]
在我的机器上，这个文件为 ~/Dockerfiles/cgit/Dockerfile。在 cgit 目录下的这个文件旁，我放了 cgit-0.9.2 的源代码，并且从编译目录树中将 cgit、cgit.css 和 cgit.png 拷贝出来。lighttpd-cgit.conf 相当简单：
server.modules = (
	"mod_access",
	"mod_alias",
	"mod_redirect",
	"mod_cgi",
)
mimetype.assign = (
	".css" => "text/css",
	".png" => "image/png",
)
server.document-root = "/var/www/cgit/"
\# Note that serving cgit under the /git location is not a requirement in
\# general, but obligatory in my setup due to historical reasons.
url.redirect = (
	"^/$" => "/git"
)
alias.url = ( "/git" => "/usr/bin/cgit" )
cgi.assign = ( "/usr/bin/cgit" => "" )
注意，我们手工编译 cgit 的唯一原因是因为还没有它的 Debian 包（编译过程有点特殊。。。）。为了实际编译这个容器并且正确的打标签，运行 rundocker build -t="stapelberg/cgit" . ：
home $ docker build -t="stapelberg/cgit" .
Uploading context 46786560 bytes
Step 1 : FROM tianon/debian:sid
 ---> 6bd626a5462b
Step 2 : RUN apt-get update
 ---> Using cache
 ---> 3702cc3eb5c9
Step 3 : RUN apt-get dist-upgrade -y
 ---> Using cache
 ---> 1fe67f64b1a9
Step 4 : RUN apt-get install -y lighttpd
 ---> Using cache
 ---> d955c6ff4a60
Step 5 : ADD cgit /usr/bin/cgit
 ---> e577c8c27dbf
Step 6 : ADD cgit.css /var/www/cgit/cgit.css
 ---> 156dbad760f4
Step 7 : ADD cgit.png /var/www/cgit/cgit.png
 ---> 05533fd04978
Step 8 : ADD lighttpd-cgit.conf /etc/lighttpd/lighttpd-cgit.conf
 ---> b592008d759b
Step 9 : ADD cgitrc /etc/cgitrc
 ---> 03a38cfd97f4
Step 10 : EXPOSE 80
 ---> Running in 24cea04396f2
 ---> de9ecca589c8
Step 11 : ENTRYPOINT ["/usr/sbin/lighttpd", "-f", "/etc/lighttpd/lighttpd-cgit.conf", "-D"]
 ---> Running in 6796a9932dd0
 ---> d971ba82cb0a
Successfully built d971ba82cb0a
## 第二步：推到注册器
Docker公司默认会提供一个从注册器抽取图片的服务。由于注册容器可能包含机密的配置信息如密码和其他的证书，因此不建议把图片放入到公共的注册器中。dockify.io和quay.io提供了私人注册器的服务，但是你也可以使用你自己的。当你使用自己的注册器时需要注意你是保证它的可用性。小心别在当你需要通过龟速的DSL来传送你的docker容器到自己私人的注册器时，出现网络中断的情况。一个替换方法就是使用自己的注册器但是通过付费方式把容器那些文件存储到亚马逊S3上，
在默认配置下很容易运行自己的注册器的（只存储数据在容器下的/tmp目录下，无需认证）
d0 $ docker run -p 5000:5000 samalba/docker-registry然后，你可以对我们在步骤1中建立的图片进行标记和推送图片到注册器中：
docker tag stapelberg/cgit d0.zekjur.net:5000/cgit
docker push d0.zekjur.net:5000/cgit
## 第三步: 在CoreOS上运行cgit
为了简单的运行cgit容器，我们运行一下代码:
d0 $ docker run d0.zekjur.net:5000/cgit
2013-12-07 18:46:16: (log.c.166) server started
但是这还不是他的全部作用, 80端口只是被cgit容器使用,还没有提供给外届或者是其他的cgit容器访问. 或者你也可以使用-p 80参数去自定义你的对外端口。
d0 $ docker run -p 4242:80 d0.zekjur.net:5000/cgit
当使用浏览器访问 http://d0.zekjur.net:4242/ 时,你就能看到cgit了.但是, 即使你在自己的cgitrc中建立了一个git仓库,他还是不起作用,因为在这个容器中，他还是没有git仓库.所以最有效的方法就是给他们提供一个权限。在这个例子中，在容器中只读。创建 /media/state/_CUSTOM/git 这目录，并且将你的仓库放在里面, 再重新启动容器:
d0 \# mkdir -p /media/state/_CUSTOM/git
d0 \# cd /media/state/_CUSTOM/git
d0 \# git clone git://github.com/stapelberg/godebiancontrol
d0 $ docker run -v /media/state/_CUSTOM/git:/git:ro \
  -p 4242:80 d0.zekjur.net:5000/cgit
这样你就能在仓库中看到这个git仓库了.现在，我们就需要给cgit建立一个unit脚本，确保机器重启，容器也能自动开启了：
d0 \# cat >/media/state/units/cgit.service <<'EOT'
[Unit]
Description=cgit
After=docker.service
[Service]
Restart=always
ExecStart=/usr/bin/docker run \
    -v /media/state/_CUSTOM/git:/git:ro \
    -p 4242:80 \
    d0.zekjur.net:5000/cgit
[Install]
WantedBy=local.target
EOT
d0 \# systemctl daemon-reload
## 第四步: 在容器的前端运行nginx
你可能注意到了我们并没有直接在80端口上直接暴露cgit. 大多数时候，你没有那么多的公共的IP地址来给每个服务分配一个。所以我们部署一个nginx来作为其他服务的反向代理，这样做还有一个好处是可以对其他服务进行无缝升级(在另一个端口启动新版本的cgit并测试，然后重启nginx, 把请求转向这个新版本的cgit)。
在docker里安装nginx跟安装cgit是很相似的, 仅需一些手动编辑:
home $ mkdir -p ~/Dockerfiles/nginx
home $ cd ~/Dockerfiles/nginx
home $ cat >Dockerfile <<'EOT'
FROM tianon/debian:sid
RUN apt-get update
RUN apt-get dist-upgrade -y
RUN apt-get install -y nginx-extras
EXPOSE 80
ENTRYPOINT ["/usr/sbin/nginx", "-g", "daemon off;"]
EOT
home $ docker build -t=stapelberg/nginx .
home $ docker tag stapelberg/nginx d0.zekjur.net:5000/nginx
home $ docker push d0.zekjur.net:5000/nginx
现在, 我们并不直接将 cgit 暴露出来, 而是把 cgit 的4242端口绑定到 docker0 的桥接接口上, 其他容器也可以访问这个接口:
d0 $ docker run -v /media/state/_CUSTOM/git:/git:ro \
  -p 172.17.42.1:4242:80 d0.zekjur.net:5000/cgit我决定不把实际的虚拟主机配置包括在 Nginx 容器中，而是保持在 /media/state/_CUSTOM/nginx 中，以便可以修改它（未来可能实现自动化），可以简单地通过向 Nginx 发送 SIGHUP 信号来重新加载它：
d0 \# mkdir -p /media/state/_CUSTOM/nginx
d0 \# cat >/media/state/_CUSTOM/nginx/cgit <<'EOT'
server {
        root /usr/share/nginx/www;
        index index.html index.htm;
        server_name d0.zekjur.net;
        location / {
                proxy_pass http://172.17.42.1:4242/;
        }
}
EOT
最后一步，运行 nginx 容器，如下：
d0 $ docker run -v /media/state/_CUSTOM/nginx:/etc/nginx/sites-enabled:ro \
  -p 80:80 dock0.zekjur.net:5000/nginx
一切完成后，当你在浏览器中访问 d0.zekjur.net ，你会受到 cgit 的欢迎!





