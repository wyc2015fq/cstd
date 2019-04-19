# aliyun ubuntu读取第三方源被forbidden的问题 - =朝晖= - 博客园
# [aliyun ubuntu读取第三方源被forbidden的问题](https://www.cnblogs.com/dhcn/p/7130522.html)
使用下面指令添加了一个源：
- sudo add-apt-repository ppa:webupd8team/java  
然后update的时候提示：
- W: Failed to fetch http://ppa.launchpad.net/webupd8team/java/ubuntu/dists/trusty/main/binary-amd64/Packages: 403  Forbidden [IP: 112.124.140.210 80]  
- W: Failed to fetch http://ppa.launchpad.net/webupd8team/java/ubuntu/dists/trusty/main/binary-i386/Packages: 403  Forbidden [IP: 112.124.140.210 80]  
- E: Some index files failed to download. They have been ignored, or old ones used instead.  
- E: Couldn't rebuild package cache  
根据这个帖子 https://ubuntuforums.org/showthread.php?t=2181359 的提示,看了一下apt的设置,发现以下配置：
- Acquire::http::Proxy "http://mirrors.aliyun.com/";  
注释掉，就Ok了，阿里云对镜像的此类修改有损阴德。

