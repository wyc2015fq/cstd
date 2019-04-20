# Ubuntu下无法安装sun-java6-jdk的解决办法 - h13 - 博客园
http://blog.sina.com.cn/s/blog_6296abc601018p86.html 这个帖子是讲怎么添加一个新的源，
deb http://us.archive.ubuntu.com/ubuntu/ hardy multiverse
但是当执行
sudo apt-get update
还会出现 W: Failed to fetch http://us.archive.ubuntu.com/ubuntu/dists/jaunty/universe/binary-i386/Packages
    404 Not Found [IP: 91.189.92.180 80]
这个时候，需要将**(xx.)archive.ubuntu.com**t改成**old-releases.ubuntu.com**.
deb http://old-releases.ubuntu.com/ubuntu/ hardy multiverse
再
sudo apt-get update
然后
sudo apt-get install sun-java6-jdk
