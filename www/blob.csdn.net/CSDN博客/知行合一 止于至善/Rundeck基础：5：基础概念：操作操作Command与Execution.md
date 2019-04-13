
# Rundeck基础：5：基础概念：操作操作Command与Execution - 知行合一 止于至善 - CSDN博客

2019年04月11日 05:17:51[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：42


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
上篇文章介绍了如何在Rundeck中进行添加Node，这篇文章继续介绍如何在添加的Node上进行Command的执行以及结果的确认。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Comannd vs Job
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Job在使用上更接近于运维的例行作业，而Comand则一般是人工手动执行的命令级别。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|概念|说明|
|---|---|
|Jobs|Job是相关操作的步骤与设定选项以及执行Job的Node所组成，而在实际的场景中，很多运维的例行操作都可以在Rundeck中以job的方式进行定义。|
|Commands|相较于Job，Command是可以在Node上进行单次执行的可执行的命令，通过Rundeck在指定的Node上进行此命令的执行。|
# 在缺省Node上执行命令
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)通过左侧导航菜单选中Command，输入Node名称进行选择，这里选择缺省的Node，然后输入多条命令以分号隔开，然后回车即开始在此Node上执行相关命令。
[
](https://img-blog.csdnimg.cn/20190409102518442.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410154316115.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
从Activity中可以看到执行的详细信息，所有的执行都会被记录，也有助于运维审计的需要。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410154510725.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 选择多个Node
使用.*可以选择多个Node，同样执行上述三条命令
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410154711765.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 结果的确认
运维操作很重要的是结果的可确认，过程的可追踪，使用Rundeck，基本上可以实现大部分企业简单程度的此方面的要求，以此简单命令为例，在实际运维场景中可能对应着运维人员到某台机器上去执行某条命令，可以看到如下的结果。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410160655937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
从这里看到我们执行过的两次命令，第二次是同样的命令在两台机器上进行了执行，所以从记录\#2中可以看到是2ok的结果，对此执行的结果进行进一步的确认，可以看到Rundeck实际是有所记录的。
[
](https://img-blog.csdnimg.cn/20190410160655937.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)整体的Report信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410160900797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
执行日志信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190410160918830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
而且执行日志还提供了Download的功能，可作为审计和追踪的客观结果提供。
[
](https://img-blog.csdnimg.cn/20190410160918830.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)注意事项
这里实际需要打通host132到rundeck容器之间的通路，这里选择直接手动管理，执行如下操作即可。
Step 1: 在host132上创建rundeck用户
[root@host132 ~]\# useradd rundeck[root@host132 ~]\# id rundeckuid=1000(rundeck)gid=1000(rundeck)groups=1000(rundeck)[root@host132 ~]\#Step 2: 设定rundeck用户密码
[root@host132 ~]\# passwd rundeckChanging passwordforuser rundeck.
New password: 
BAD PASSWORD: The password is shorter than 7 characters
Retype new password: 
passwd: all authentication tokens updated successfully.[root@host132 ~]\#Step 3: 使用ssh-copy-id设定通路
sh-4.3$whichssh-copy-id
/usr/bin/ssh-copy-id
sh-4.3$ ssh-copy-id 192.168.163.132
/usr/bin/ssh-copy-id: INFO: Source of key(s)to be installed:"/home/rundeck/.ssh/id_rsa.pub"The authenticity of host'192.168.163.132 (192.168.163.132)'can't be established.
ECDSA key fingerprint is SHA256:tR3rrrCZMFjcZV6YTEiXPYMYJQm17FdQL4ulFgmIqLU.
Are you sure you want to continue connecting (yes/no)? yes
rundeck@192.168.163.132's password: 输入设定的密码
sh-4.3$Step4: 通路确认
如下命令的正常返回则可确认ssh通路没有问题
sh-4.3$ssh192.168.163.132hostnamehost132
sh-4.3$其他内容
[https://liumiaocn.blog.csdn.net/article/details/89135773](https://liumiaocn.blog.csdn.net/article/details/89135773)
参考内容
[https://www.rundeck.com/open-source](https://www.rundeck.com/open-source)

