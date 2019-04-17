# RabbitMQ 插件的禁用和启用 - zp704393004的专栏 - CSDN博客





2018年04月24日 10:53:52[原来未知](https://me.csdn.net/zp704393004)阅读数：1805








转自：[http://haili.me/archives/tag/rabbitmqctl](http://haili.me/archives/tag/rabbitmqctl)




先安装rabbitmq-server这里就不写了，之前有篇文章里有相关步骤：[RabbitMQ的安装与配置](http://haili.me/archives/235)

如果/etc/rabbitmq不存在会报如下错误：
Error: {cannot_write_enabled_plugins_file,”/etc/rabbitmq/enabled_plugins”, enoent}

|12|mkdir/etc/rabbitmqcd/usr/local/rabbitmq/sbin/|
|----|----|
启用此插件

|1|./rabbitmq-plugins enable rabbitmq_management|
|----|----|
如果要禁用用此命令即可

|1|./rabbitmq-plugins disable rabbitmq_management|
|----|----|
查看已经安装的插件

|12345678910111213141516171819202122|./rabbitmq-plugins list[e] amqp_client                       0.0.0[] eldap                             0.0.0-git[] erlando                           0.0.0[e] mochiweb                          1.3-rmq0.0.0-git[] rabbitmq_auth_backend_ldap        0.0.0[] rabbitmq_auth_mechanism_ssl       0.0.0[] rabbitmq_consistent_hash_exchange 0.0.0[] rabbitmq_federation               0.0.0[] rabbitmq_jsonrpc                  0.0.0[] rabbitmq_jsonrpc_channel          0.0.0[] rabbitmq_jsonrpc_channel_examples 0.0.0[E] rabbitmq_management               0.0.0[e] rabbitmq_management_agent         0.0.0[] rabbitmq_management_visualiser    0.0.0[e] rabbitmq_mochiweb                 0.0.0[] rabbitmq_shovel                   0.0.0[] rabbitmq_shovel_management        0.0.0[] rabbitmq_stomp                    0.0.0[] rabbitmq_tracing                  0.0.0[] rfc4627_jsonrpc                   0.0.0-git[e] webmachine                        1.7.0-rmq0.0.0-hg|
|----|----|
停止RabbitMQ

|1|./rabbitmqctl stop|
|----|----|
启动RabbitMQ

|1|./rabbitmq-server -detached|
|----|----|
访问：http://127.0.0.1:55672/ 默认登录名和密码都是guest

如果新加了用户不进去去是因为没有给权限，执行如下权限就行了

|1|./rabbitmqctl set_user_tags newuser administrator|
|----|----|
管理命令行工具rabbitmqctl的命令可参考这里：[rabbitmqctl命令的使用](http://www.rabbitmq.com/man/rabbitmqctl.1.man.html)

![RabbitMQ图形化管理工具](http://haili.me/wp-content/uploads/2012/08/rabbitmqtool-500x373.jpg)
RabbitMQ图形化管理工具




