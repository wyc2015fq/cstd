# 在webstorm设置File watcher for Jade - weixin_33985507的博客 - CSDN博客
2016年01月26日 20:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
用Jade模板引擎写html确实方便，元素不用闭合，很多种简写的方法。
为了要知道自己写的对不对，就要用到jade -w命令监控jade文件，只要变化就编译。
现在用webstorm写代码的超多，可以通过其中的Filewatcher的方式关联jade命令做到自动化编译生成html文件。
步骤：
1.当然是安装jade，装过的就忽略。
```
npm install -g jade
```
2.设置Webstorm，添加filewatcher for jade
点击菜单项“File->Settings...”，在打开的“Settings”对话框的左边选项中选择“File Watchers”，然后在右侧点“+”图标（add）。
参照下图设置各项参数，注意：jade文件需要在工程中才有效。
![](https://images2015.cnblogs.com/blog/166781/201601/166781-20160126195925504-1835130305.png)
