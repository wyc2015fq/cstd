# 详解 Linux 文档属性、拥有者、群组、权限、差异 - 文章 - 伯乐在线
原文出处： [无痴迷，不成功](https://www.cnblogs.com/justmine/p/9053419.html)
## 写在前面
我们都知道`Linux`是一个支持**多用户、多任务**的系统，这也是它最优秀的特性，即可能同时有很多人都在系统上进行工作，所以千万不要**强制关机**，同时，为了保护每个人的隐私和工作环境，针对某一个文档(文件、目录)，`Linux`系统定义了三种身份，分别是**拥有者(owner)、群组(group)、其他人(others)**，每一种身份又对应三种权限，分别是**可读(readable)、可写(writable)、可执行(excutable)**。
## 文档属性
使用命令`ls -al --full-time`，或者此命令的简写`ll`可以查看文件或者目录的所有属性。如下：
![](http://jbcdn2.b0.upaiyun.com/2018/05/6eb4e7e09d926ea7adb505db505447d2.png)
从上面可以看到，每一行都有7列，分别是：
- **第一列**
共10位，第1位表示文档类型，`d`表示目录，`-`表示文件，`l`表示链接文件，`d`表示可随机存取的设备，如U盘等，`c`表示一次性读取设备，如鼠标、键盘等。后9位，依次对应三种身份所拥有的权限，身份顺序为：owner、group、others，权限顺序为：readable、writable、excutable。如：`-r-xr-x---`的含义为**当前文档是一个文件，拥有者可读、可执行，同一个群组下的用户，可读、可写，其他人没有任何权限**。
- **第二列**
表示连结数
- **第三列**
表示拥有者
- **第四列**
表示所属群组
- **第五列**
表示文档容量大小，单位字节
- **第六列**
表示文档最后修改时间，注意不是文档的创建时间哦
- **第七列**
表示文档名称。以点(.)开头的是隐藏文档
## 变更拥有者(owner)
### 位置


```
etc/passwd
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/71aa271ab220cecc626c63674cc660d4.png)
> 
注意：必须是该位置下已存在的帐号。也就是在`/etc/passwd`中有记录的拥有者才能改变。
### 语法


```
chown [-R] [帐号名称] [文件或目录]
chown [-R] [帐号名称]:[群组名称] [文件或目录]
```
> 
备注：此命令也可以顺便变更文档群组，但还是建议使用**chgrp**命令来变更文档群组。
#### 选项
-R 递归变更，即连同次目录下的所有文件(夹)都要变更。
### 用法
`chown daemon test` 变更文件夹**test**账号为**daemon**。
![](http://jbcdn2.b0.upaiyun.com/2018/05/fcb43ff3f99e4cc0f24349f91c00a3ee.png)
`chown daemon:root test` 变更文件夹**test**群组为**root**。
![](http://jbcdn2.b0.upaiyun.com/2018/05/216562230373eef21e7b964e69fa7fe6.png)
`chown root.users test` 变更文件夹账号为**root**，群组为**users**
![](http://jbcdn2.b0.upaiyun.com/2018/05/469705efe86e04479131eee26983dd11.png)
`chown .root test` 单独变更群组为**root**
![](http://jbcdn2.b0.upaiyun.com/2018/05/cd23a79ca203ca771e40988d1acc9013.png)
> 
备注：虽然也可以在拥有者与群组间加小数点(**.**)，但为了避免有的同学命名中带点，故还是建议使用冒号“:”来隔开拥有者与群组，避免误判。
## 变更群组(group)
### 位置


```
etc/group
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/ece6b7d40fd616dbb26d3c639a0c5764.png)
> 
备注：从这里可以查看到所有群组
### 语法


```
chgrp [-options] [群组名] [文档路径]
```
> 
备注：关于**options**，可以通过`man chgrp、info chgrp、chgrp --help`等命令查询详细用法。
### 用法
`chgrp -R users test` 改变`test`文件夹及其所有子文件(夹)的群组为`users`。
![](http://jbcdn2.b0.upaiyun.com/2018/05/324a71a1e4b3ee4ec38eb3c555c82cf1.png)
> 
注意：群组名称不在位置内，将会报错`invalid group`。
![](http://jbcdn2.b0.upaiyun.com/2018/05/e26a356e4af1b5db53ee00817cc16660.png)
## 变更权限
Linux文档的基本权限就三个，分别是`read/write/execute`，加上身份`owner/group/others`也只有九个。权限变更的方式有2种，分别是**符号法**和**数字法**。
### 符号法
分别使用u，g，o来代表三种身份，a表示全部身份；分别使用r、w、x表示三种权限；分别使用+、-、=表示操作行为
#### 语法


```
chmod | u g o a | +（加入） -（除去） =（设置） | r w x | 文档路径
```
### 设置权限(=)
变更目录test的权限为任何人都可读、写、执行。


```
chmod u=rwx,g=rwx,o=rwx test 
或
chmod ugo=rwx test 
或
chmod a=rwx test
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/5ce9bb78e6d7b8626e226fe43a69040f.png)
### 去掉权限(-)
去掉目录test执行权限


```
chmod u-x,g-x,o-x test 
或
chmod ugo-x test 
或
chmod a-x test
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/bb7c408bad9334918b678f5a15105cff.png)
> 
备注：执行权限(x)，对目录而已就是其他用户能否**cd test**成为工作目录。
### 添加权限(+)
增加目录test执行权限


```
chmod u+x,g+x,o+x test 
或
chmod ugo+x test 
或
chmod a+x test
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/6ab28b525a72bc8b09e26d048f72701a.png)
> 
备注：很熟悉吧，如果我们编写完一个shell文件test.sh后，通过`chmod a+x test.sh`就添加了文件执行权限。
### 数字法
顾名思义，就是使用数字来代表权限，r,w,x分别为4,2,1。三种权限累加就可以得出一种身份的权限。
设置目录test的权限为任何人都可读、写、执行。


```
chmod 777 test
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/be87c870ece0e5311d409991d369de5e.png)
设置目录test的权限为任何人都可读、写。


```
chmod 666 test
```
![](http://jbcdn2.b0.upaiyun.com/2018/05/ff0b2a1d263b70a1f7ac7581df8def45.png)
赋予一个shell文件test.sh可执行权限，拥有者可读、写、执行，群组账号和其他人可读、执行。


```
chmod 755 test
```
> 
备注：有没有发现数字法更简单啊！！！
## 文件和目录权限差异
文档权限对于文件和目录有巨大的差异
### 文件
针对的是该文件内容
- readable 可读取该文件的实际内容
- writable 可以编辑、新增或者是修改该文件的内容
- executable 有可以被系统执行的权限
> 
备注：具有w权限不可以删除文件，删除文件是目录权限控制的范围！！！记住**文件权限针对是文件内容**。
### 目录
针对的是该目录下的文件对象
- readable 具有读取目录结构清单的权限，即可以通过**ls**命令，查询该目录清单。
- writable 具有变动该目录结构清单的权限，即可以创建、迁移、删除、更名该目录下的文件。
- executable 具备进入该目录的权限，即可以通过**cd**命令，转到工作目录。
> 
备注：从上面可以得出，开放目录给任何人浏览时，至少需要赋予`r`或`x`权限。读取目录文件内容，至少需要目录权限`x`和文件权限`r`。
## 总结
`Linux`的每个文档可以分别针对三种身份赋予`rwx`权限；`chgrp`命令变更文件群组，`chmod`命令变更文件权限，`chown`变更文件拥有者；那么以后记得使用**文档权限来保护数据的安全性哦**。
**如果你觉得本篇文章对您有帮助的话，感谢您的【推荐】**。
**如果你对 linux 感兴趣的话可以关注我，我会定期的在博客分享我的学习心得**。
