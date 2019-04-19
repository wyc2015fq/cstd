# linux文件类型和重要命令（find ,tar,tr）-老男孩之永不止步-51CTO博客
预备姿势：常见的扩展名
.txt             文本文件 
.log            日志文件
.conf .cfg   配置文件 
.sh  .bash   脚本文件（命令大礼包）
常见的文件类型：
- file            普通文件
d diectory   目录
l softlink     软连接（快捷方式）
b block       设备(块)文件 光盘 硬盘
c character 字符设备（不断向外发出或接受字符）
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/5f5628041e8d661146d8f6b2e7507ada.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/8c595779965825b01ccffb2d176bc7a3.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/c4b63c53a78393bfd2c897b1ca8a07a9.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
- file      普通文件
1.二进制文件（命令）
2.文本文件（text）
3.数据文件（data） 压缩包
1.file  区分文件类型（查看文件类型）
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/315faddf659956a6f62e77d24693d44e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
2.ln -s创建软连接
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/78f70ce6df8d0ecacf46b1f57e495856.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
3.tr 替换
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/a1c6093755d3f459ac9952ab64dbf872.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
4.tr -d 删除
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/ba393168ad47a52c66620d28878b356a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
5.tr -cd取反删除----将空格键，数字等都删除了
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/5bd395fdcc1b04a82cb50ff0e9fddbe4.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
6.head -c 8取前面的8个字符，tr 可以配合'a-z'
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/3fcac3f638f0e1be74510b8cd52e6a8e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
7.which 找命令的绝对路径
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/e3bfbd37f62a717fd66ffa66a4cb9aba.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
8.whereis 显示命令及其相关文件全路径
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/a4da7aea3730d6efc12316ee94f88d83.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
9.如何查询命令属于哪个软件包  yum provides locate
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/94ce8f320ef9c455bed53b22cac6dab7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
10.rpm -qa 查询软件是否安装成功了，若有软件信息出来--成功
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/42b9a4e36a79e8f11d6bffa89ca8dedd.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
11.rpm -ql查询软件包里面的内容（命令）
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/aecc5cb9757aa6a29f51ba16cb3219bb.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
find
12.find  目录 -type f -name ""或者''
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/cf8282d7023a57af84e75a68654c999d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/ce6a8acc3f631e7cce0e8d34eeb3ca10.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
13.find / - maxdepth 1 -type d  ==tree -d  -L  1  /(maxdepth放在其他参数前，否则有警告)
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/7e1dadb6ef8ca8301477d80777f1a2bf.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/b0e3b0b573c69658317fdbc51c820f1f.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
14.找出/etc/目录下第一层的所有以conf结尾的文件，不区分大小写，find可以同时找多个文件
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/a6c03f17fb881de80b28797d8fae7887.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
15.根据大小查找文件+1M大于1M，-1M小于1M，+k（小k），-size只支持整数，不支持小数 find /etc -size +1M
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/da0c583e95ed2dd948b5bb924af4a265.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
tar 打包压缩（一般用来备份使用）
16.创建压缩包 tar zcvf /tmp/etc.tar.gz /etc
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/6181bb93faea6eea922b76fe0301c4c6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
17.查看压缩包 tar ztf /tmp/etc.tar.gz
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/8bf959d16147bc8abe9d4a59edddd4d5.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
18.解压压缩包，默认解压到当前目录 tar zxf etc.tar.gz
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/e91bd9148800fff84a8b7247e24c52a1.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
19.C解压到制定的目录
![linux文件类型和重要命令（find ,tar,tr）](https://s1.51cto.com/images/blog/201904/09/a32effa08fb1710be690b5c04c624849.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
