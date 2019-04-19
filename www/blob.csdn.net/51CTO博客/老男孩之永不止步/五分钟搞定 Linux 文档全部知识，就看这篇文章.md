# 五分钟搞定 Linux 文档全部知识，就看这篇文章-老男孩之永不止步-51CTO博客
写在前面
我们都知道Linux是一个支持多用户、多任务的系统，这也是它最优秀的特性，即可能同时有很多人都在系统上进行工作，所以千万不要强制关机，同时，为了保护每个人的隐私和工作环境，针对某一个文档(文件、目录)，Linux系统定义了三种身份，分别是拥有者(owner)、群组(group)、其他人(others)，每一种身份又对应三种权限，分别是可读(readable)、可写(writable)、可执行(excutable)。
文档属性
使用命令ls -al --full-time，或者此命令的简写ll可以查看文件或者目录的所有属性。如下：
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/f7188560e45d3d9775a14c695f4729a8.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
从上面可以看到，每一行都有7列，分别是：
第一列
共10位，第1位表示文档类型，d表示目录，-表示文件，l表示链接文件，d表示可随机存取的设备，如U盘等，c表示一次性读取设备，如鼠标、键盘等。后9位，依次对应三种身份所拥有的权限，身份顺序为：owner、group、others，权限顺序为：readable、writable、excutable。如：-r-xr-x---的含义为当前文档是一个文件，拥有者可读、可执行，同一个群组下的用户，可读、可写，其他人没有任何权限。
第二列
表示连结数
第三列
表示拥有者
第四列
表示所属群组
第五列
表示文档容量大小，单位字节
第六列
表示文档最后修改时间，注意不是文档的创建时间哦
第七列
表示文档名称。以点(.)开头的是隐藏文档
变更拥有者(owner)
位置
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/9bb58b03fdc5f54bde613877de100be7.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/dbd39adfdd643111f86b7e21356cc79a.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
注意：必须是该位置下已存在的帐号。也就是在/etc/passwd中有记录的拥有者才能改变。
语法
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/ff5c8ba8c72c3ae4aeb06ca59f5026ff.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：此命令也可以顺便变更文档群组，但还是建议使用chgrp命令来变更文档群组。
选项
-R 递归变更，即连同次目录下的所有文件(夹)都要变更。
用法
chown daemon test 变更文件夹test账号为daemon。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/4b491c855a23824e69c9c86af2ee9756.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
chown daemon:root test 变更文件夹test群组为root。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/3239b48666a22855e064524acc75c607.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
chown root.users test 变更文件夹账号为root，群组为users
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/5ee9424cd4f5ae388e6f5bf8a876f33b.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
chown .root test 单独变更群组为root
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/78c0d207c557301392073f96f73acd2d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：虽然也可以在拥有者与群组间加小数点(.)，但为了避免有的同学命名中带点，故还是建议使用冒号“:”来隔开拥有者与群组，避免误判。
变更群组(group)
位置
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/a89a97708bbe853c3341c51282729216.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/df9c42e03efad5c274f81c6ee44b7314.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：从这里可以查看到所有群组
语法
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/3f4238fccb25a934d95a8c24d1ef0134.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：关于options，可以通过man chgrp、info chgrp、chgrp --help等命令查询详细用法。
用法
chgrp -R users test 改变test文件夹及其所有子文件(夹)的群组为users。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/7b6bf61c0764ab8cecefb91591de4f1d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
注意：群组名称不在位置内，将会报错invalid group。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/744a8cbe35baf3f70ba9f22e9d8c38d6.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
变更权限
Linux文档的基本权限就三个，分别是read/write/execute，加上身份owner/group/others也只有九个。权限变更的方式有2种，分别是符号法和数字法。
- 符号法
分别使用u，g，o来代表三种身份，a表示全部身份；分别使用r、w、x表示三种权限；分别使用+、-、=表示操作行为
语法
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/0ceafe6768f9b4b5410f59d299ae5da8.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
设置权限(=)
变更目录test的权限为任何人都可读、写、执行。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/be30de488efd35935eff7febbcba4262.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/291ed6f6b0040255ce0e865d11729756.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
去掉权限(-)
去掉目录test执行权限
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/8026ace1f6c28cd64457e8c7e77ad34d.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/741cd989808fbc7c20fae490e18d9578.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：执行权限(x)，对目录而已就是其他用户能否cd test成为工作目录。
添加权限(+)
增加目录test执行权限
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/cdd82cb23d3d6675c7c851bf67c73f5c.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/1e909ab0a1a34f5d588e3250b64eb010.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：很熟悉吧，如果我们编写完一个shell文件test.sh后，通过chmod a+x test.sh就添加了文件执行权限。
- 数字法
顾名思义，就是使用数字来代表权限，r,w,x分别为4,2,1。三种权限累加就可以得出一种身份的权限。
设置目录test的权限为任何人都可读、写、执行。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/ea60e6113bb630a0d91cff502098c338.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/767dd67cf27bbf3cb8004e52f3b08832.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
设置目录test的权限为任何人都可读、写。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/22b6d4b46ce9741cc50bd5dfe4951631.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/fceb1793687619350f1e2a80857bec38.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
赋予一个shell文件test.sh可执行权限，拥有者可读、写、执行，群组账号和其他人可读、执行。
![五分钟搞定 Linux 文档全部知识，就看这篇文章](https://s1.51cto.com/images/blog/201904/12/3c7f77058d26e46cf1d29e3e8a4629bc.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
备注：有没有发现数字法更简单啊！！！
文件和目录权限差异
文档权限对于文件和目录有巨大的差异
文件
针对的是该文件内容
readable 可读取该文件的实际内容
writable 可以编辑、新增或者是修改该文件的内容
executable 有可以被系统执行的权限
备注：具有w权限不可以删除文件，删除文件是目录权限控制的范围！！！记住文件权限针对是文件内容。
目录
针对的是该目录下的文件对象
readable 具有读取目录结构清单的权限，即可以通过ls命令，查询该目录清单。
writable 具有变动该目录结构清单的权限，即可以创建、迁移、删除、更名该目录下的文件。
executable 具备进入该目录的权限，即可以通过cd命令，转到工作目录。
备注：从上面可以得出，开放目录给任何人浏览时，至少需要赋予r或x权限。读取目录文件内容，至少需要目录权限x和文件权限r。
