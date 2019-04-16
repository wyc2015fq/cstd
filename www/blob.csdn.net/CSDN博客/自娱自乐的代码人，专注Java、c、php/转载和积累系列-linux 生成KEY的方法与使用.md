# 转载和积累系列 - linux 生成KEY的方法与使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年04月17日 13:52:12[initphp](https://me.csdn.net/initphp)阅读数：745
个人分类：[转载和积累系列](https://blog.csdn.net/initphp/article/category/2609089)










服务器A： 192.168.1.1

服务器B： 192.168.1.2



服务器A： 

配置:

ssh-keygen -b 2048 -t rsa                     

#这里的-b 2048 是密钥加密的长度，最好设大点，有消息称美国有黑客解决 ssh的密钥进行破解。

#[http://bbs.yuloo.com/thread-860154-1-1.html](http://bbs.yuloo.com/thread-860154-1-1.html)

直接回车3次

key文件会保存在/root/.ssh目录下

这时候.ssh目下会多出几个文件

id_rsa   私钥文件

id_rsa.pub  公钥文件,这个文件里的内容要放到其它主机里面去。

查看 id_rsa.pub  文件

cat /roor/.ssh/id_rsa.pub



ssh-rsa AAAAB3NzaC1yc2EAAAABIwAAAIcAvh6Lz9ghftghtfghSv+kSrxVLC9UDKq27mg5vhfghAaWcgFgllZhfg2DLnGotmeL/08bp6jMW/ekSq1BY1eiouiluiliuliul2Rr84x0bB5YShhpgJwuPbwr/pymgukyukyukyuthjkiytiliunzdfkuykyuIZgVvgPTzNdN/RDwJMsafsdvgsdsfsdfdsfsdaflozStYcWjrwzYxsroDPBy4zjd0gH5zMYhj+vS4OOAZZ8rtHEfx+sTE0=[root@localhost.localdomain](mailto:root@localhost.localdomain)







服务器B：

配置：

新建文件夹与文件

mkdir /root/.ssh

vim /root/.ssh/authorized_keys

把A服务器上查看的KEY文件内容，复制到B服务器的authorized_keys中

主要是将[root@localhost.localdomain](mailto:root@localhost.localdomain)修改为[root@192.168.1.1](mailto:root@192.168.1.1) 这样方便查看是哪台服务器的KEY，可能会有多台服务器的key。



修改sshd 的主配置文件，关闭密码认证登陆，使用key登陆。

vim /etc/ssh/sshd_config

找到 PasswordAuthentication yes  

改成 PasswordAuthentication no

service sshd restart





测试：

在A服务器上执行

ssh [root@192.168.1.2](mailto:root@192.168.1.2)

首次登陆会需要你输入YES。看是不是不需要密码就可以登陆B服务了。




