# rabbitmq 在c语言上的使用 - zp704393004的专栏 - CSDN博客





2018年05月03日 19:59:41[原来未知](https://me.csdn.net/zp704393004)阅读数：368








本实验的基础是在CentOS7.3的基础上安装erl18.2.1和rabbitmq3.6.9上运行

然后参考如下内容：



- 
安装CMake： 
（1）下载cmake：[http://download.csdn.net/download/fm0517/9936318](http://download.csdn.net/download/fm0517/9936318)
（2）解压：tar -xvzf cmake-3.9.1.tar.gz 
（3）进入目录 cmake-3.9.1 
（4）执行：./bootstrap 
（5）编译，执行：gmake 
（6）安装（可选），执行：make install

- 
编译RabbitMQ-c： 
（1）下载RabbitMQ-c：[http://download.csdn.net/download/fm0517/9936336](http://download.csdn.net/download/fm0517/9936336)
（2）解压：unzip rabbitmq-c-rabbitmq-c-v0.3.0.zip
（3）进入目录：cd rabbitmq-c-rabbitmq-c-v0.3.0
（4）创建目录build：mkdir build 
（5）进入目录build：cd build 
（6）生成Makefile：cmake ..（注意cmake后面跟上上级路径，使用到了CMakeList.txt） 
（7）编译：cmake –build . （注意命令最后跟上当前路径） 
生成的文件为：librabbitmq.so.4.2.0，以及链接文件：librabbitmq.so.4和librabbitmq.so





同时：

1. 下载 [rabbitmq-c-master](https://github.com/alanxz/rabbitmq-c?spm=a2c4e.11153940.blogcont41770.3.303b4ba0CLrMNO)源码 


https://github.com/alanxz/rabbitmq-c?spm=a2c4e.11153940.blogcont41770.3.303b4ba0CLrMNO


2. 下载 [rabbitmq-codegen](https://github.com/rabbitmq/rabbitmq-codegen?spm=a2c4e.11153940.blogcont41770.4.303b4ba0CLrMNO)源码：

https://github.com/rabbitmq/rabbitmq-codegen?spm=a2c4e.11153940.blogcont41770.4.303b4ba0CLrMNO

3. 将 rabbitmq-codegen 中的内容拷贝到 rabbitmq-c-master 中的 codegen 目录下（如果没有该目录请自行创建） 


然后：在上面的rabbit-c的文件夹中rabbitmq-c-rabbitmq-c-v0.3.0中，examples中包含amqp_bind、amqp_consumer、amqp_exchange_declare、amqp_listen、amqp_listenq、amqp_producer、amqp_rpc_sendstring_client、amqp_sendstring 和 amqp_unbind 可执行文件。这些就是可以修改的C源文件，修改后执行步骤二后，就可以得到可执行程序，位置在build/exsample中，然后可以执行如下操作：

**运行实例：**
在一个terminal中，运行




**[plain]**[view plain](https://blog.csdn.net/lijinqi1987/article/details/79037130#)[copy](https://blog.csdn.net/lijinqi1987/article/details/79037130#)

- ./examples/amqp_listen localhost 5672 amq.direct test  

四个参数分别为域名、端口、exchange、bindingkey

另一个terminal中，运行



**[plain]**[view plain](https://blog.csdn.net/lijinqi1987/article/details/79037130#)[copy](https://blog.csdn.net/lijinqi1987/article/details/79037130#)

- ./examples/amqp_sendstring localhost 5672 amq.direct test "hello world"  

五个参数分别为域名、端口、exchange、bindingkey 发送字符

会在监听的terminal中出现如下打印：
Delivery 1, exchange amq.direct routingkey test
Content-type: text/plain
----
00000000: 68 65 6C 6C 6F 20 77 6F : 72 6C 64                 hello world

0000000B:

参考网址：

https://blog.csdn.net/lijinqi1987/article/details/79037130




