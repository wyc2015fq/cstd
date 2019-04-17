# ubuntu创建新用户 - Keith - CSDN博客





2017年01月17日 16:35:54[ke1th](https://me.csdn.net/u012436149)阅读数：1409








在Ubuntu下创建一个新的用户：

## 使用adduser

```python
step1: 切换成 root登录
step2: adduser username
step3: 跟随系统提示完成 以后操作
```

## 使用useradd

这个方法比较麻烦

```python
1：添加新用户
useradd -r -m -s /bin/bash 用户名

2:配置新用户密码
passwd 用户名

3：给新添加的用户增加ROOT权限
vim /etc/sudoers
然后添加：
用户名 ALL=(ALL:ALL) ALL


另外，如果直接用useradd添加用户的话，可能出现没有home下的文件夹，以及shell无法自动补全的情况。出现此问题只要修改/etc/passwd下的/bin/sh为/bin/bash即可。
```

## 其他

```python
1. 给 root用户设置密码，由于默认的root密码是不确定的，所以给root设置密码是必要的
~/sudo passwd root 然后根据系统提示来就可以了
2. 切换登录用户
~/su username
```



