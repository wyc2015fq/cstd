# CentOS7 安装svn - 菜鸟路上的小白 - CSDN博客





2016年09月13日 11:26:35[somnus_小凯](https://me.csdn.net/u012486840)阅读数：27299








centos（我这里使用的是CentOS7）下yum命令即可方便的完成安装

```php
```php
$ sudo yum install subversion
```
```

测试安装是否成功：
`$ svnserve --version`
2. 建立版本库

创建svn数据目录（subversion默认是把/var/svn作为数据根目录的，开机启动默认也是从这里）：

```php
```php
$ sudo mkdir -p /var/svn
```
```

创建版本库：

```php
```php
$ sudo svnadmin create /var/svn/wangwa
```
```

如果删除版本库：

```php
```php
$ sudo rm -rf /var/svn/somnus
```
```

3. 配置svn配置文件

每个版本库创建之后都会生成svnserve.conf主要配置文件。编辑它：

```php
```php
$ sudo vim /var/svn/somnus/conf/svnserve.conf
```
```

编辑示例：

```
[general]
anon-access = none                #控制非鉴权用户访问版本库的权限
auth-access = write                 #控制鉴权用户访问版本库的权限
password-db = passwd           #指定用户名口令文件名
authz-db = authz                     #指定权限配置文件名
realm = somnus                    #指定版本库的认证域，即在登录时提示的认证域名称
```

4. 编辑svn用户配置文件
`sudo vim /var/svn/somnus/conf/passwd`
编辑示例：

```
[users]
admin = admin                #用户，密码
fuhd = fuhd                #用户，密码
test = test                #用户，密码
```

5. 编辑svn权限控制配置文件
`sudo vim /var/svn/somnus/conf/authz`
编辑示例：

```bash
[groups]
admin = admin         #admin为用户组,等号之后的admin为用户
test = fuhd,test

[somnus:/]                #表示根目录（/var/svn/somnus），somnus: 对应前面配置的realm = somnus
@admin = rw            ＃表示admin组对根目录有读写权限,r为读，w为写

[somnus:/test]         #表示test目录（/var/svn/somnus/test）
@test = rw                #表示test组对test目录有读写权限
```

6. 启动，查看和停止SVN服务

启动SVN服务：

```php
```php
# -d : 守护进程  -r : svn数据根目录 
$ sudo svnserve -dr /var/svn            #用root权限启动
```
```

查看SVN服务：

```php
```php
$ ps aux|grep svnserve               #默认端口为：3690
```
```

7. 配置防火墙端口

首先要明确CentOS7的默认防火墙为firewallD。subversion的默认端口为3690，如果没有打开会报错：

```
$ sudo firewall-cmd --permanent -add-port=3690/tcp
$ sudo firewall-cmd --reload
```








8. 检索项目和切换项目的url

项目检错

```
$ svn checkout svn://192.168.0.112/XK_Project .     #使用 checkout  服务器资源 本地目录
```



切换项目url

```
$ svn switch --relocate svn://192.168.0.112/XK_Project svn://192.168.0.120/XK_Project   # 使用 switch 迁移 from  to 新的地址
```


```

```






9. 设置开机启动

在centos7， 设置开机启动：

```bash
$ sudo systemctl enable svnserve.service      #注意：根目录必须是/var/svn 这样才能设置成功！！
```

```php
```php
#设置开机启动后就可以按下面的方式开启或停止服务了
$ sudo systemctl start svnserve.service
$ sudo systemctl stop svnserve.service
```
```

保存退出，重启并从客户端进行测试。如果报这样的错：svn: E204900: Can't open file '/var/svn/somnus/format': Permission denied的错误。那就是与SELinux有关系，目前我还不太会用SELinux，那就先把SELinux关闭吧，后面学会了，回过头来再改这一段！！！！：

临时关闭：


```php
```php
$ sudo setenforce 0
```
```

永久关闭：

```php
```php
$ sudo vim /etc/sysconfig/selinux
```
```

修改：

```bash
SELINUX = disable               #值修改为disable.
```




svn帮助文档[http://riaoo.com/subpages/svn_cmd_reference.html](http://riaoo.com/subpages/svn_cmd_reference.html)



