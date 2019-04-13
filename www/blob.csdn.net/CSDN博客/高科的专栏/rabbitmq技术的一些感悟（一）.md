
# rabbitmq技术的一些感悟（一） - 高科的专栏 - CSDN博客

2014年08月19日 19:25:06[高科](https://me.csdn.net/pbymw8iwm)阅读数：39437


# Rabbitmq
## 初识rabbitmq
RabbitMQ是流行的开源消息队列系统，用erlang语言开发。RabbitMQ是AMQP（高级消息队列协议）的标准实现。如果不熟悉AMQP，直接看RabbitMQ的文档会比较困难。不过它也只有几个关键概念，这里简单介绍
几个概念说明：
Broker：简单来说就是消息队列服务器实体。
Exchange：消息交换机，它指定消息按什么规则，路由到哪个队列。
Queue：消息队列载体，每个消息都会被投入到一个或多个队列。
Binding：绑定，它的作用就是把exchange和queue按照路由规则绑定起来。
Routing Key：路由关键字，exchange根据这个关键字进行消息投递。
vhost：虚拟主机，一个broker里可以开设多个vhost，用作不同用户的权限分离。
producer：消息生产者，就是投递消息的程序。
consumer：消息消费者，就是接受消息的程序。
channel：消息通道，在客户端的每个连接里，可建立多个channel，每个channel代表一个会话任务。
由Exchange，Queue，RoutingKey三个才能决定一个从Exchange到Queue的唯一的线路。
消息队列的使用过程大概如下：
（1）客户端连接到消息队列服务器，打开一个channel。
（2）客户端声明一个exchange，并设置相关属性。
（3）客户端声明一个queue，并设置相关属性。
（4）客户端使用routing key，在exchange和queue之间建立好绑定关系。
（5）客户端投递消息到exchange。
exchange接收到消息后，就根据消息的key和已经设置的binding，进行消息路由，将消息投递到一个或多个队列里。
exchange也有几个类型，完全根据key进行投递的叫做Direct交换机，例如，绑定时设置了routing
 key为”abc”，那么客户端提交的消息，只有设置了key为”abc”的才会投递到队列。对key进行模式匹配后进行投递的叫做Topic交换机，符号”\#”匹配一个或多个词，符号”*”匹配正好一个词。例如”abc.\#”匹配”abc.def.ghi”，”abc.*”只匹配”abc.def”。还有一种不需要key的，叫做Fanout交换机，它采取广播模式，一个消息进来时，投递到与该交换机绑定的所有队列。
RabbitMQ支持消息的持久化，也就是数据写在磁盘上，为了数据安全考虑，我想大多数用户都会选择持久化。消息队列持久化包括3个部分：
（1）exchange持久化，在声明时指定durable => 1
（2）queue持久化，在声明时指定durable => 1
（3）消息持久化，在投递时指定delivery_mode=> 2（1是非持久化）
如果exchange和queue都是持久化的，那么它们之间的binding也是持久化的。如果exchange和queue两者之间有一个持久化，一个非持久化，就不允许建立绑定。
## 安装开发环境和库
1.将目录中的librabbitmq.so.1放到目录 /usr/local/lib/librabbitmq.so.1
2.安装rabbitm需要的环境和库
yum install -y ncurses-devel
yum install gcc
yum install g++
yum install cmake
yum install make
yum install php
yum install mysql
yum install php-process
yum install php-devel
yum install mysql-server
\#安装php的amq支持扩展
wget http://pecl.php.net/get/amqp-1.0.3.tgz
tar zxvf amqp-1.0.3.tgz
cd amqp-1.0.3
/usr/bin/phpize
./configure--with-php-config=/usr/bin/php-config --with-amqp
make && make install
\#php.ini 添加
vi /etc/php.ini
extension="amqp.so"
\#安装erlang支持
wgethttp://www.erlang.org/download/otp_src_R15B01.tar.gz
tar -zxvf otp_src_R15B01.tar.gz
cd otp_src_R15B01
./configure --prefix=/home/erlang--without-javac
make && make install
ln -s /home/erlang/bin/erl/usr/local/bin/erl
3. 安装rabbitma
解压rabbitmq-server-generic-unix-3.3.4.tar
进入sbin目录：
启动rabbitmq服务，执行 nohup./rabbitmq-server start &
## 启动rabbitmq服务器以及命令
当第一次启动服务，检测数据库是否未初始化或者被删除，它会用下面的资源初始化一个新的数据库:
一个命名为 / 的虚拟宿主一个名为guest密码也为guest的用户，他拥有/虚拟宿主的所有权限如果你的中间件是公开访问的，最好修改guest用户的密码。管理概观rabbitmqctl 是RabbitMQ中间件的一个命令行管理工具。它通过连接一个中间件节点执行所有的动作。本地节点默认被命名为”rabbit”。可以通过这个命令前使用”-n”标志明确的指定节点名称, 例如:\# rabbitmqctl -n rabbit@shortstop add_user tonyg changeit
这个命令指示RabbitMQ中间件在rabbit@shortstop 节点创建一个tonyg/changeit的用户。
在一个名为”server.example.com”的主机，RabbitMQ Erlang节点的名称通常是rabbit@server(除非RABBITMQ_NODENAM在中间件启动时候被设置)。hostnam -s 的输出通常是”@”符号正确的后缀。rabbitmqctl 默认产生详细输出。通过”-q”标示可选择安静模式。rabbitmqctl -q status应用和集群管理1.停止RabbitMQ应用，关闭节点
\# rabbitmqctl stop
2.停止RabbitMQ应用
\# rabbitmqctl stop_app
3.启动RabbitMQ应用
\# rabbitmqctl start_app
4.显示RabbitMQ中间件各种信息
\# rabbitmqctl status
5.重置RabbitMQ节点
\# rabbitmqctl reset
\# rabbitmqctl force_reset
从它属于的任何集群中移除，从管理数据库中移除所有数据，例如配置过的用户和虚拟宿主, 删除所有持久化的消息。
force_reset命令和reset的区别是无条件重置节点，不管当前管理数据库状态以及集群的配置。如果数据库或者集群配置发生错误才使用这个最后的手段。
注意：只有在停止RabbitMQ应用后，reset和force_reset才能成功。
6.循环日志文件
\# rabbitmqctl rotate_logs[suffix]
7.集群管理
\# rabbitmqctl cluster clusternode…
用户管理
1.添加用户
\# rabbitmqctl add_user username password
2.删除用户
\# rabbitmqctl delete_user username
3.修改密码
\# rabbitmqctl change_password usernamenewpassword
4.列出所有用户
\# rabbitmqctl list_users
权限控制1.创建虚拟主机
\# rabbitmqctl add_vhost vhostpath
2.删除虚拟主机
\# rabbitmqctl delete_vhost vhostpath
3.列出所有虚拟主机
\# rabbitmqctl list_vhosts
4.设置用户权限
\# rabbitmqctl set_permissions [-pvhostpath] username regexp regexp regexp
5.清除用户权限
\# rabbitmqctl clear_permissions [-pvhostpath] username
6.列出虚拟主机上的所有权限
\# rabbitmqctl list_permissions [-pvhostpath]
7.列出用户权限
\# rabbitmqctl list_user_permissionsusername
例子：
添加  rabbitmqctl add_vhost az
rabbitmqctl set_permissions -p az guest".*" ".*" ".*"

## 接口描述
amqp_connection_state_tamqp_new_connection(void);
接口说明：声明一个新的amqp connection
intamqp_open_socket(char const *hostname, int portnumber);
接口说明：获取socket.
参数说明：hostname        RabbitMQ server所在主机
portnumber     RabbitMQ server监听端口
voidamqp_set_sockfd(amqp_connection_state_t state,int sockfd);
接口说明：将amqp connection和sockfd进行绑定
amqp_rpc_reply_tamqp_login(amqp_connection_state_t state, char const *vhost,intchannel_max,int frame_max,int heartbeat,amqp_sasl_method_enum sasl_method,...);
接口说明：用于登录RabbitMQ server，主要目的为了进行权限管理；
参数说明：state    amqpconnection
vhost   rabbit-mq的虚机主机，是rabbit-mq进行权限管理的最小单位
channel_max最大链接数，此处设成0即可
frame_max和客户端通信时所允许的最大的frame size.默认值为131072，增大这个值有助于提高吞吐，降低这个值有利于降低时延
heartbeat含义未知，默认值填0
sasl_method用于SSL鉴权，默认值参考后文demo
amqp_channel_open_ok_t*amqp_channel_open(amqp_connection_state_t state, amqp_channel_t channel);
接口说明：用于关联conn和channel
amqp_exchange_declare_ok_t*amqp_exchange_declare(amqp_connection_state_t state, amqp_channel_t channel,amqp_bytes_t exchange, amqp_bytes_t type, amqp_boolean_t passive,amqp_boolean_t durable, amqp_table_t
 arguments);
接口说明：声明declare
参数说明：state
channel
exchange
type     "fanout" "direct" "topic"三选一
passive
curable
arguments
amqp_queue_declare_ok_t*amqp_queue_declare(amqp_connection_state_t state, amqp_channel_t channel,amqp_bytes_t queue, amqp_boolean_t passive, amqp_boolean_t durable,amqp_boolean_t exclusive, amqp_boolean_t
 auto_delete, amqp_table_targuments);
接口说明：声明queue
参数说明：state   amqp connection
channel
queue  queue name
passive
durable队列是否持久化
exclusive当前连接不在时，队列是否自动删除
aoto_delete没有consumer时，队列是否自动删除
arguments用于拓展参数，比如x-ha-policy用于mirrored queue
amqp_queue_bind_ok_t*amqp_queue_bind(amqp_connection_state_t state, amqp_channel_t channel,amqp_bytes_t queue, amqp_bytes_t exchange, amqp_bytes_t routing_key, amqp_table_t arguments);
接口说明：声明binding
amqp_basic_qos_ok_t*amqp_basic_qos(amqp_connection_state_t state, amqp_channel_t channel, uint32_tprefetch_size, uint16_t prefetch_count, amqp_boolean_t global);
接口说明：qos是quality of service，我们这里使用主要用于控制预取消息数，避免消息按条数均匀分配，需要和no_ack配合使用
参数说明：state
channel
prefetch_size以bytes为单位，0为unlimited
prefetch_count预取的消息条数
global
amqp_basic_consume_ok_t*amqp_basic_consume(amqp_connection_state_t state, amqp_channel_t channel,amqp_bytes_t queue, amqp_bytes_t consumer_tag, amqp_boolean_t no_local,amqp_boolean_t no_ack, amqp_boolean_t
 exclusive, amqp_table_t arguments);
接口说明：开始一个queue consumer
参数说明：state
channel
queue
consumer_tag
no_local
no_ack是否需要确认消息后再从队列中删除消息
exclusive
arguments
int amqp_basic_ack(amqp_connection_state_tstate,amqp_channel_t channel,uint64_t delivery_tag,amqp_boolean_t multiple);
intamqp_basic_publish(amqp_connection_state_t state,amqp_channel_tchannel,amqp_bytes_t exchange,amqp_bytes_t routing_key,amqp_boolean_tmandatory,amqp_boolean_t immediate,struct amqp_basic_properties_t_
 const*properties,amqp_bytes_t body);
接口说明：发布消息
参数说明：state
channel
exchange
routing_key当exchange为默认“”时，此处填写queue_name，当exchange为direct，此处为binding_key
mandatory参见参考文献2
immediate同上
properties更多属性，如何设置消息持久化，参见文后demo
body消息体
amqp_rpc_reply_tamqp_channel_close(amqp_connection_state_t state,amqp_channel_t channel,intcode);
amqp_rpc_reply_tamqp_connection_close(amqp_connection_state_t state,int code);
intamqp_destroy_connection(amqp_connection_state_t state);


