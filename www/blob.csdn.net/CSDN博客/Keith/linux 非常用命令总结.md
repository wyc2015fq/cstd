# linux 非常用命令总结 - Keith - CSDN博客





2017年03月02日 15:30:26[ke1th](https://me.csdn.net/u012436149)阅读数：365
个人分类：[linux](https://blog.csdn.net/u012436149/article/category/6647900)








- 如何删除ppa(我们在安装软件的时候，教程一般都会让添加ppa源，但是几乎没有教程会告诉你怎么删除)

```python
# 添加ppa源
sudo add-apt-repository ppa:webapps/preview

# 删除ppa源 方法一
sudo add-apt-repository --remove ppa:someppa/ppa

# 方法二
cd /etc/apt/sources.list.d/ #找到想删除的源，直接删除文件夹
```
- 更改默认root密码

```
sudo passwd
# 输入当前用户密码
# 两次键入 超级用户密码就 ok 了
```
- 统计文件行数

```
# -l : 统计字节数, -c: 统计行数,  -w 统计字数
wc - lcw file1 file2
```

## 参考资料

[http://www.68idc.cn/help/jiabenmake/linuxshell/20150204206994.html](http://www.68idc.cn/help/jiabenmake/linuxshell/20150204206994.html)



