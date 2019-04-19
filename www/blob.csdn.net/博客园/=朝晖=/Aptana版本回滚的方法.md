# Aptana版本回滚的方法 - =朝晖= - 博客园
# [Aptana版本回滚的方法](https://www.cnblogs.com/dhcn/p/7116236.html)
       最近Aptana对Django1.7的编译支持有点问题，开发环境必须使用Django1.6版本，今天看了一眼它的官网，版本已经到3.6.1，我的版本还是3.4.2，就checkupdate升级到3.6.1，其实是3.6.0，结果郁闷，pydev功能基本无法使用了，回家在家里电脑的开发环境上试了一下，找到了解决办法：
       首先把在Eclipse里面新版本的Aptana studio完全卸载掉，然后使用3.4.2特定的版本源，重新安装旧版本，版本源地址是：
```
http://d1iwq2e2xrohf.cloudfront.net/tools/studio/plugin/install/studio3/3.4.2.201308081736/
```

