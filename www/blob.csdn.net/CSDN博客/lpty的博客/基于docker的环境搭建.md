
# 基于docker的环境搭建 - lpty的博客 - CSDN博客

2017年11月27日 20:40:08[lpty](https://me.csdn.net/sinat_33741547)阅读数：636



## 一、简介
Docker 是一个开源的应用容器引擎，让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。容器是完全使用沙箱机制，相互之间不会有任何接口。

## 二、应用
Docker这种技术跟平常用的虚拟机很相似，但相比之下更加轻量。在工程化部署项目的时候非常好用，它实际上解决了一个开发中的痛点，开发环境和测试、生产环境的一致性。用这种方式就能够极大程度上避免这种情况，本地运行好好的，一上线服务器就挂掉了，最后发现某个包没装之类的麻烦。
## 三、实战
具体的操作还是找文档看吧，这里我贴上自己部署环境的Dockerfile备忘：

```python
From xxx
MAINTAINER xxx@xxx.xxx
RUN yum install -y gcc python-devel openssl-devel 
RUN yum install -y mysql-devel 
RUN pip install --upgrade pip -i https://pypi.tuna.tsinghua.edu.cn/simple
RUN pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
RUN yum install -y wget
RUN wget -O /root/chrome.rpm https://dl.google.com/linux/direct/google-chrome-stable_current_x86_64.rpm
RUN yum install -y /root/chrome.rpm
RUN rm -f /root/chrome.rpm
RUN wget -O /root/chromedriver.zip http://npm.taobao.org/mirrors/chromedriver/2.33/chromedriver_linux64.zip
RUN yum install -y unzip
RUN unzip -o /root/chromedriver.zip -d /root/
RUN chmod +x /root/chromedriver
RUN mv /root/chromedriver /usr/bin/chromedriver
RUN rm -f /root/chromedriver.zip
```
上面的Dockerfile代表，以xxx镜像为基础，安装系统依赖，然后安装python包，最后是chrome浏览器和webdriver，实际上是我某个爬虫的配置。


