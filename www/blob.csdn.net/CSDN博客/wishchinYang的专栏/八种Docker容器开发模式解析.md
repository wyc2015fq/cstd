# 八种Docker容器开发模式解析 - wishchinYang的专栏 - CSDN博客
2014年12月28日 17:48:00[wishchin](https://me.csdn.net/wishchin)阅读数：516
**原文链接**：[http://www.csdn.net/article/2014-10-27/2822294](http://www.csdn.net/article/2014-10-27/2822294)
Docker优点已经说过很多次，这里不做详述，Docker现在越来越受到开发人员的青睐，而且利用Docker开发的人也越来越多，本文来自Vidar Hokstad博客，他是一名Docker开发资深人士，他总结了开发Docker容器的8种模式。
【编者按】Vidar Hokstad 在Docker使用方面非常有经验，尤其在没有数据丢失前提下，使用Docker创建可重复build上经验丰富，在本博客中，他总结了开发Docker容器的8种模式。
**以下为译文：**
Docker现在成了我最喜欢的工具，在本文中，我将概述一些在我使用Docker过程中反复出现的模式。我不期待它们能给你带来多少惊喜，但我希望这些能对你有用，我非常愿意与你交流在使用Docker过程中碰到的模式。
我所有Docker实验的基础是保持volume状态不变，以便Docker容器在没有数据丢失的前提下任意重构。
下面所有的Dockerfiles例子都集中在：创建容器在其本身可以随时更换的地方，而无需考虑其它。
**1. The SharedBase Container(s)**
Docker鼓励“继承”，这应用也很自然——这是高效使用Docker的一个基本方式，不仅由于它有助于减少建立新容器的时间，Docker优点多多，它会cache中间步骤，但也容易在不明确的情况下，失去分享机会。
很显然，在将我的各种容器迁移到Docker上时，首先要面对的是多个步骤。
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
- 
对于多数想要随处部署的项目来说所，要创建多个容器，尤其是在这个项目需要长进程，或者需要特定包的情况，所以我要运行的容器也变得越来越多。
重要的是为了让mybase环境完全自由支配，我正考虑试图在Docker上运行“所有一切”（包括我依赖几个桌面app）。
所以我很快开始提取我的基本设置到base容器。这是我当前的“devbase”Dockerfile：
    FROM debian:wheezy
    RUN apt-get update
    RUN apt-get -y install ruby ruby-dev build-essential git
    RUN apt-get install -y libopenssl-ruby libxslt-dev libxml2-dev
    
    # For debugging
    RUN apt-get install -y gdb strace
    
    # Set up my user
    RUN useradd vidarh -u 1000 -s /bin/bash --no-create-home
    
    RUN gem install -n /usr/bin bundler
    RUN gem install -n /usr/bin rake
    
    WORKDIR /home/vidarh/
    ENV HOME /home/vidarh
    
    VOLUME ["/home"]
    USER vidarh
    EXPOSE 8080
这里没有什么需要特别说明的地方——它安装一些需要随时可用的特定工具。这些可能会对大多数人来说是不同的。值得注意的是如果/当你重建一个容器的时候，你需要指定一个特定的标签来避免意外。
使用默认端口8080，因为这是我发布web app的端口，这也是我用这些容器的目的。
它为我添加了一个用户，并且不会创建一个/ home目录。我从宿主机绑定挂载了一个共享文件夹/ home，这就引出了下一个模式。
**2. The SharedVolume Dev Container**
我所有的dev容器与宿主机分享至少一个volume： / home，这是为了便于开发。对于许多app，在开发模式中，使用基于file-system-change的code-reloader运行，这样一来容器内封装了OS / distro-level的依赖，并在初始环境中帮助验证app-as-bundled工作，而不需要让我每次在代码改变时重启/重建VM。
至于其他，我只需要重启（而不是重建）容器来应对代码的更改。
对于test/staging和production容器，大多数情况下不通过volume共享代码，转而使用“ADD”来增添代码到Docker容器中。
这是我的“homepage”的dev容器的Dockerfile，例如，包含我的个人wiki，存在于“devbase”容器中的 /home下，以下展示了如何使用共享的base容器和/home卷：
    FROM vidarh/devbase
    WORKDIR /home/vidarh/src/repos/homepage
    ENTRYPOINT bin/homepage web
以下是dev-version的博客：
    FROM vidarh/devbase
    
    WORKDIR /
    USER root
    
    # For Graphivz integration
    RUN apt-get update
    RUN apt-get -y install graphviz xsltproc imagemagick
    
    USER vidarh
    WORKDIR /home/vidarh/src/repos/hokstad-com
    ENTRYPOINT bundle exec rackup -p 8080
因为他们从一个共享的库中取代码，并且基于一个共享的base容器，这些容器通常当我添加/修改/删除依赖项时会极其迅速重建。
即便如此也有一些地方是我非常愿意改善，尽管上面的base是轻量级的，他们大多数在这些容器仍未使用。由于Docker使用copy-on-write覆盖，这不会导致一个巨大的开销，但它仍然意味着我没有做到最小的资源消耗，或者说最小化attack或error的几率。
**3. The Dev ToolsContainer**
这可能对我们这些喜欢依靠ssh写代码的人很有吸引力，但是对IDE人群则小一点。对我来说，关于以上设置更大的 一个好处，是它让我在开发应用程序中，能够将编辑和测试执行代码的工作分离开来。
过去dev-systems对我来说一件烦人的事，是dev和production依赖项以及开发工具依赖项容易混淆，很容易产生非法的依赖项。
虽然有很多方法解决这个，比如通过定期的测试部署，但我更偏爱下面的解决方案，因为可以在第一时间防止问题的发生：
我有一个单独的容器包含Emacs的installation以及其他各种我喜欢的工具，我仍然试图保持sparse，但关键是我的screen session可以运行在这个容器中，再加上我笔记本电脑上的“autossh”，这个连接几乎一直保持，在那里我可以编辑代码，并且和我的其他dev容器实时共享。如下：
FROM vidarh/devbase
RUN apt-get update
RUN apt-get -y install openssh-server emacs23-nox htop screen
# For debugging
RUN apt-get -y install sudo wget curl telnet tcpdump
# For 32-bit experiments
RUN apt-get -y install gcc-multilib 
# Man pages and "most" viewer:
RUN apt-get install -y man most
RUN mkdir /var/run/sshd
ENTRYPOINT /usr/sbin/sshd -D
VOLUME ["/home"]
EXPOSE 22
EXPOSE 8080
结合共享“/ home“，已经足够让ssh的接入了，并且被证明能满足我的需要。
**4. The Test In ADifferent Environment containers**
我喜欢Docker的一个原因，是它可以让我在不同的环境中测试我的代码。例如，当我升级Ruby编译器到1.9时，我可以生成一个Dockerfile，派生一个1.8的环境。
FROM vidarh/devbase
RUN apt-get update
RUN apt-get -y install ruby1.8 git ruby1.8-dev
当然你可以用rbenv等达到类似的效果。但我总是觉得这些工具很讨厌，因为我喜欢尽可能多地用distro-packages部署，不仅仅是因为如果这项工作的顺利进行，能使其他人更容易地使用我的代码。
当拥有一个Docker容器，我需要一个不同的环境时，我仅需要“docker run”一下，几分钟便能很好的解决这个问题。
当然，我也可以使用虚拟机来达到目的，但使用Docker更省时间。
**5. The BuildContainer**
这些天我写的代码大部分都是解释性语言，但还是有一些代价高昂的“build”步骤，我并不愿意每次都去执行它们。
一个例子是为Ruby应用程序运行“bundler”。Bundler 为Rubygems更新被缓存的依赖，并且需要时间来运行一个更大的app。
经常需要在应用程序运行时不必要的依赖项。例如安装依赖本地扩展gems的通常还需要很多包——通常没有记录——通过添加所有build-essential和它的依赖项就轻松启动。同时，你可以预先让bundler做所有的工作，我真的不想在主机环境中运行它，因为这可能与我部署的容器不兼容。
一个解决方案是创建一个build容器。如果依赖项不同的话，你可以创建分别的Dockerfile，或者你可以重用主app Dockerfile以及重写命令运行你所需的build commands。Dockerfile如下：
FROM myapp
RUN apt-get update
RUN apt-get install -y build-essential [assorted dev packages for libraries]
VOLUME ["/build"]
WORKDIR /build
CMD ["bundler", "install","--path","vendor","--standalone"]
然后每当有依赖更新时，都可以运行上面的代码，同时将build/source目录挂载在容器的"/build"路径下。
**6. TheInstallation Container**
这不是我擅长的，但是真的值得提及。优秀的nsenter和docker-enter工具在安装时有一个选项，对于现在流行的curl | bash模式是一个很大的进步，它通过提供一个Docker容器实现“Build Container”模式。
这是Dockerfile的最后部分，下载并构建一个nsenter的合适版本：
ADD installer /installer
CMD /installer
“installer”如下：
#!/bin/sh
if mountpoint -q /target; then
       echo "Installing nsenter to /target"
       cp /nsenter /target
       echo "Installing docker-enter to /target"
       cp /docker-enter /target
else
       echo "/target is not a mountpoint."
       echo "You can either:"
       echo "- re-run this container with -v /usr/local/bin:/target"
       echo "- extract the nsenter binary (located at /nsenter)"
fi
虽然可能还有恶意攻击者试图利用容器潜在的特权升级问题，但是attack surface至少显著变小。
这种模式能吸引大多数人，是因为这种模式能避免开发人员在安装脚本时偶尔犯的非常危险的错误。
**7. TheDefault-Service-In-A-Box Containers**
当我认真对待一个app，并且相对快速的准备一个合适的容器来处理数据库等，对于这些项目，我觉得难能可贵的是已经有一系列的“基本的”基础设施容器，只做适当的调整就可以满足我的需求。
当然你也可以通过“docker run”得到“主要的”部分，在Docker索引里有诸多替代品，但我喜欢首先检查它们，找出如何处理数据，然后我将修改版本添加到自己的“library”。
例如Beanstalkd：
FROM debian:wheezy
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get -q update
RUN apt-get -y install build-essential
ADD <a href="http://github.com/kr/beanstalkd/archive/v1.9.tar.gz">http://github.com/kr/beanstalkd/archive/v1.9.tar.gz</a> /tmp/
RUN cd /tmp && tar zxvf v1.9.tar.gz
RUN cd /tmp/beanstalkd-1.9/ && make
RUN cp /tmp/beanstalkd-1.9/beanstalkd /usr/local/bin/
EXPOSE 11300
CMD ["/usr/local/bin/beanstalkd","-n"]
**8. TheInfrastructure / Glue Containers**
许多这些模式专注于开发环境（这意味着有production 环境有待讨论），但有一个大类别缺失：
容器其目的是将你的环境组合起来成为一个整体，这是目前为止对我来说有待进一步研究的领域，但我将提到一个特殊的例子：
为了轻松地访问我的容器，我有一个小的haproxy容器。我有一个通配符DNS条目指向我的主服务器，和一个iptable入口开放访问我的haproxy容器。Dockerfile没什么特别的：
FROM debian:wheezy
ADD wheezy-backports.list /etc/apt/sources.list.d/
RUN apt-get update
RUN apt-get -y install haproxy
ADD haproxy.cfg /etc/haproxy/haproxy.cfg
CMD ["haproxy", "-db", "-f", "/etc/haproxy/haproxy.cfg"]
EXPOSE 80
EXPOSE 443
这里有趣的是haproxy.cfg
backend test
    acl authok http_auth(adminusers)
    http-request auth realm Hokstad if !authok
    server s1 192.168.0.44:8084
如果我想要特别一点，我会部署类似 [AirBnB's Synapse](https://github.com/airbnb/synapse)，但这已经超出了我的需求。
在工作时扩大容器的规模，目的是让部署应用程序简单便捷，就像我正在过渡到一个完整的面向Docker的私有云系统。
原文链接： [Eight Docker Development Patterns](http://www.hokstad.com/docker/patterns) （编译/魏伟 审校/周小璐）
