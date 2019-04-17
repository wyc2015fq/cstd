# Wireshark抓取本地Tcp包（任何数据包） - DoubleLi - 博客园






没有任何一个程序员在做项目的时候不会遇到网络编程的问题，要解决这些问题除了对各种网络协议深入了解之外，还需要掌握各种网络分析工具的用法，不用多说wireshark绝对是这方面的翘楚，可惜的是，wireshark不能对本地接口（loopback，或者127.0.0.1）进行直接抓包。

### wireshark的工作原理

这里面的原理其实很简单，wireshark可以通过操作系统来访问所有的网络adapter，通俗点，就是网卡，并把网卡上的traffic截获，拷贝，用于数据包的分析。所以wireshark的使用前提是：**你希望截获的数据包是通过网卡收发的！**而loopback接口上的数据因为其特殊性（**本地还回**），是在操作系统内部转发的，不会通过网卡，类似于进程间通信。因此wireshark从工作原理上就不能够获得loopback上的traffice。

### 解决方案

百度上有不少解决方案，但很多关键字就是：RawCap或**windows没法截取loopback数据包**。其实这些方案没有从本质上解决问题。现在一个比较好的解决方案是用npcap，这个工具是北大的一个博士，叫做Luo yang，开发的。源码放在[https://github.com/nmap/npcap](https://github.com/nmap/npcap)，还在持续维护中。这个工具的原理大概就是虚拟了一个网卡让操作系统把loopback的数据镜像一份到npcap adapter，然后wireshark可以通过截取这个npcap adatper上的数据包来获得对本地数据包进行分析的一个途径。大家不要小看了这个东西，在npcap adatper上，wireshark抓到的可是本地的所有数据包，这个不得了，用处大了，特别是对于需要深入了解各种库的工作原理的。它可以抓取各种：
- http包，用于web接口的分析（Java程序员各种用soap接口，rest接口的，如果不了解http协议，不知道抓包来看，就没法理解各种mediaType对应的http entity，就没法不受限的使用各种web接口来完成各种场景的开发）
- tcp包，用于各种基于tcp的通信接口/协议的分析（知道吗，这货能够抓各种ejb远程调用，虽然每个应用服务器的厂商在实现J2EE规范的时候，都有自己的底层通信协议，但是通过这货，你就可以看到具体是怎么通信的）

### 具体安装配置方法

废话不多说，后面还可能会在这个方面多写几篇文章，先把最基础的安装配置方法列个各位。（这里只介绍windows下面的用法） 
首先，当然是先要安装npcap。上头我已经给过一个github的地址了，那是源码。你可以在上面找每个release的安装包路径。这里再直接给一个： 
[https://github-cloud.s3.amazonaws.com/releases/35855357/bece22ea-9ab5-11e6-88ad-304fd50e2df0.exe?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAISTNZFOVBIJMK3TQ%2F20161031%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20161031T022347Z&X-Amz-Expires=300&X-Amz-Signature=10f67cbca359526bd638004a28e2a3d183a1d68ac755419083cc30676514e742&X-Amz-SignedHeaders=host&actor_id=8413239&response-content-disposition=attachment%3B%20filename%3Dnpcap-0.10-r12.exe&response-content-type=application%2Foctet-stream](https://github-cloud.s3.amazonaws.com/releases/35855357/bece22ea-9ab5-11e6-88ad-304fd50e2df0.exe?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAISTNZFOVBIJMK3TQ%2F20161031%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20161031T022347Z&X-Amz-Expires=300&X-Amz-Signature=10f67cbca359526bd638004a28e2a3d183a1d68ac755419083cc30676514e742&X-Amz-SignedHeaders=host&actor_id=8413239&response-content-disposition=attachment%3B%20filename%3Dnpcap-0.10-r12.exe&response-content-type=application%2Foctet-stream)
先npcap安装： 
![这里写图片描述](http://img.blog.csdn.net/20161114104320472)
![这里写图片描述](http://img.blog.csdn.net/20161114104510880)
安装完之后，你可以看到，在你的网络管理中心多了一块网卡（Npcap loopback Adapter）： 
![这里写图片描述](http://img.blog.csdn.net/20161114104822837)
打开你的wireshark： 
你会发现，也多了一个可以监听的接口： 
![这里写图片描述](http://img.blog.csdn.net/20161114105005715)
没错，通过这个东东你就可以直接截获本地的Http包了 
来，跑一个试试： 
![这里写图片描述](http://img.blog.csdn.net/20161114105159984)
看到这个127.0.0.1了吗？这可是货真价实的本地http包。 
除了http包，之外，其他的tcp包也给你们瞅瞅： 
![这里写图片描述](http://img.blog.csdn.net/20161114105521989)
**看到了没，这是java的远程调用**。

### 注意事项

1， 这个工具是很好用，但是也有缺陷，如果你装了之后，突然有某一天发现: 
![这里写图片描述](http://img.blog.csdn.net/20161114105749458)
你的wifi居然没法通过dhcp获得IP了，就是这个家伙搞的鬼。解决方案，目前是在device manger里面把它删了，重启电脑，我会在github上报一个Issue的。 
2，另外，请把你的wireshark升级到比较新的版本，不要是1.9.xx以下的版本









