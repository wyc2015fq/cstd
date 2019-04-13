
# Docker容器图形界面显示（运行GUI软件）的配置方法 - 高科的专栏 - CSDN博客

2018年09月18日 09:44:55[高科](https://me.csdn.net/pbymw8iwm)阅读数：830


https://hub.docker.com/r/dorowu/ubuntu-desktop-lxde-vnc/
https://github.com/fcwu/docker-ubuntu-vnc-desktop

# docker-ubuntu-vnc-desktop
![Docker Pulls](https://camo.githubusercontent.com/d3af1e6a9eb04377a28bcbed1ead9393a948742a/68747470733a2f2f696d672e736869656c64732e696f2f646f636b65722f70756c6c732f646f726f77752f7562756e74752d6465736b746f702d6c7864652d766e632e737667)![Docker Stars](https://camo.githubusercontent.com/f5358ba005bb142aaf7d2d88b237db93363c2904/68747470733a2f2f696d672e736869656c64732e696f2f646f636b65722f73746172732f646f726f77752f7562756e74752d6465736b746f702d6c7864652d766e632e737667)
From Docker Index
`docker pull dorowu/ubuntu-desktop-lxde-vnc`Build yourself
git clone https://github.com/fcwu/docker-ubuntu-vnc-desktop.git
docker build --rm -t dorowu/ubuntu-desktop-lxde-vnc docker-ubuntu-vnc-desktop
Run
`docker run -it --rm -p 8080:80 dorowu/ubuntu-desktop-lxde-vnc`Browse[http://docker-default-ip:8080/](http://127.0.0.1:6080/)
![](https://camo.githubusercontent.com/7006fe30a62f2a20933a1596b312ba7ce7e41529/68747470733a2f2f7261772e6769746875622e636f6d2f666377752f646f636b65722d7562756e74752d766e632d6465736b746f702f6d61737465722f73637265656e73686f74732f6c7864652e706e67)
# Troubleshooting
boot2docker connection issue,[https://github.com/fcwu/docker-ubuntu-vnc-desktop/issues/2](https://github.com/fcwu/docker-ubuntu-vnc-desktop/issues/2)
# License
See the LICENSE file for details.
************************************************************************************************************************************************
这是通过VNC，远程启动[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2)桌面系统的访问方法。对于Windows转Linux的程序猿有更大的意义，否则大名鼎鼎的Docker只能敲命令行，虽然强大入门却难。
docker-ubuntu-vnc-desktop
From Docker Index
docker pull dorowu/ubuntu-desktop-lxde-vnc
Build yourself
git clone https://github.com/fcwu/docker-ubuntu-vnc-desktop.git
docker build --rm -t dorowu/ubuntu-desktop-lxde-vnc docker-ubuntu-vnc-desktop
Run
docker run -i -t -p 6080:80 dorowu/ubuntu-desktop-lxde-vnc
Browse http://127.0.0.1:6080/vnc.html
![](http://www.linuxidc.com/upload/2015_08/150806084627151.jpg)

