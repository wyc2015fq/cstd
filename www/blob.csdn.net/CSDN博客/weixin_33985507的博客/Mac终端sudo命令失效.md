# Mac终端sudo命令失效 - weixin_33985507的博客 - CSDN博客
2018年08月03日 11:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：144
- 
在修改`/usr`权限时，导致在使用`$ sudo`命令时，提示如下错误：
**sudo: /usr/bin/sudo must be owned by uid 0 and have the setuid bit set**
- 
解决方案：
- 
系统偏好设置 -> 用户与群组 -> 登录选项
- 
网络账户服务器： 点击 “加入” 按钮
![1599054-e83fec5557b96c9e.png](https://upload-images.jianshu.io/upload_images/1599054-e83fec5557b96c9e.png)
1.png
- 
打开目录实用工具
![1599054-68e4304b872c9066.png](https://upload-images.jianshu.io/upload_images/1599054-68e4304b872c9066.png)
2.png
- 
上方工具栏 - 编辑 - 启用`root`用户 - 设置密码
![1599054-1e86d56b5ec410f8.png](https://upload-images.jianshu.io/upload_images/1599054-1e86d56b5ec410f8.png)
3.png
> 
这是注册过的显示，未注册的是启用`root`用户
- 
退出当前用户登陆状态 - 切换`root`用户登录
- 
点击其他 - 使用`root`账户登陆
- 
重置`sudo`属性
- `$ chown root /usr/bin/sudo`
- `$ chmod 4755 /usr/bin/sudo`
- 
退出`root`账户登陆状态，切换回原用户
- 
执行`sudo`命令
- 
`sudo`命令正常后，即可停用`root`账户
![1599054-1e86d56b5ec410f8.png](https://upload-images.jianshu.io/upload_images/1599054-1e86d56b5ec410f8.png)
3.png
