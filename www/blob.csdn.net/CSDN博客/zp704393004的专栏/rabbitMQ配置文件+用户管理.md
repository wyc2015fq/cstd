# rabbitMQ配置文件+用户管理 - zp704393004的专栏 - CSDN博客





2018年04月19日 18:39:51[原来未知](https://me.csdn.net/zp704393004)阅读数：985








*rabbitMQ允许设置系统范围的可调参数并通过配置文件进行设置。典型的，在rabbitmq/rabbitmq.config文件里。*

*不过我在3.6.9里并没有找到这个文件，所以 ，后面仍然需要查找和学习*

***管理用户***

添加用户：./rabbitmqctl add_user cashing-tier cashMel

cashing-tier是用户名，cashMel是密码

输出：Creating user "cashing-tier"...done.

删除用户：./rabbitmqctl dlete_user cashing-tier

输出：Deleting user "cashing-tier"...done.

查看用户：rabbitmqctl list_users

输出：Listing users...

cashing-tier

guest

...done

更改密码：./rabbitmqctl change_password cashing-tier comp13xPassword

输出：Changing password for user "cashing-tier"...

....done

**Rabbit的权限系统**

假设你有sycamore的vhost，想要授予cashing-tier完全的访问权限（配置，写和读权限）。这时候需要rabbitmqctl 的set_permissions来完成：

./rabbitmqctl set_permissions -p sycamore\cashing-tier ".*"".*"".*"

-p sycamore----告诉了set_permissions条目应该应用到哪个vhost上

cashing-tier------被授予权限的用户

".*"".*"".*"------授权的权限。分别是映射到配置，写和读。

这里是正则表达式，使用".*"指代所有的权限（配置，以及写和读）。以为之匹配任何队列或者交换器名字。

如果向为cashing-tier授予在oak vhost上的权限，想要允许该用户对任何队列或者交换器执行读操作，同时限制其只对名字以checks-开始的队列和交换器允许写操作。同时，你想要完全阻止配置操作。为达目的，需要设计三个正则表达式：

".*"---匹配任何队列和交换器

"checks-.*"------只匹配名字以"checks-"开头的队列和交换器

""不匹配队列和交换器（这就是如何对用户拒绝指定的权限）

把所有整合起来，这样执行：

./rabbitmqctl set_permissions -p oak\-s all cashing-tier "" "checks-.*" ".*"

可以利用list_permissions命令来验证权限是否正确赋予给了oak vhost


./rabbitmqctl list_permissions -p oak

Listing permissions in vhost "oak"...

cashing-tier                              checks.*    .*  all

**移除权限：**

./rabbitmqctl clear_permissions -p oak cashing-tier

如果向查看用户在rabbitMQ上所有vhost上的权限，那么清使用list_user_permissions:

./rabbitmqctl list_user_permissions cashing-tier

Listing permissions for user "cashing-tier"...

oak checks-.*   .*  all

sycamore .*    .*  .*  all

...done.




