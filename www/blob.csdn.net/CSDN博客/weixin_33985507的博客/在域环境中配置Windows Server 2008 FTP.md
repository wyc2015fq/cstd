# 在域环境中配置Windows Server  2008 FTP - weixin_33985507的博客 - CSDN博客
2013年02月05日 02:21:56[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
#### 1.1 在域环境中配置FTP
实验目的：
? 域用户帐户实现FTP隔离
实验环境：
![clip_image001](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001533E2Qf.png)
? DCServer安装Windows Server 2008企业版，是ess.com域的域控制器。
? FTPServer安装Windows Server 2008企业版，是ess.com域中的成员，安装了FTP服务。
? InternetPC安装Vista企业版操作系统。
##### 1.1.1 在FTPServer上的配置
通过批量为域用户指定主目录，能够为每个用户创建一个文件夹，该文件夹必须共享，才能被设置成用户的主目录。
任务：
? 在FTP根目录下为域用户创建目录
? 共享FTP根目录设置权限
步骤:
**1. **在FTPServer上，在FTP根目录下，为域管理员创建一个ESS文件夹，该文件夹名称必需为域的名称。
**2. **右击“tfpRoot”文件夹，点击“共享”。
![clip_image002](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001551TLgU.png)![clip_image003](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001584B5GZ.png)
**3. **在出现的文件共享对话框，点击“共享”。
**4. **在出现的文件共享对话框，点击“完成”。
![clip_image004](http://91xueit.blog.51cto.com/attachment/201302/4/400469_13600016099E8M.png)![clip_image005](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001632B9tm.png)
##### 1.1.2 在DCServer上的配置
以下操作可以批量以用户的登录名在FTP服务器上创建文件夹，如果域用户较少你可以一个一个为用户创建文件夹。
步骤:
? 批量创建域用户的主目录
步骤:
**5. **在DCServer上，点击“开始”à“程序”，输入“dsa.msc”，点击“确定”，打开“Active Directory用户和计算机”管理工具。
**6. **在打开的Active Directory用户和计算机管理工具，选中多个用户，右击选中的用户，点击“属性”。
**7. **在出现的多个项目属性对话框，在“配置文件”标签，选中“主文件夹”，选择“连接诶”，选择盘符，输入\\ftpserver\ftproot\ess\%username%，点击“确定”。注意：%username%为参数，会自动以域用户的名称替换。
![clip_image006](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001651NAzf.png)![clip_image007](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001669VQ5m.png)
**8. **可以看到FTPServer上共享的ftproot目录下的ess目录下已经为选中的用户创建了文件夹。使用这种方法就不用一个一个为用户创建文件夹了。
![clip_image008](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001699ycQo.png)
##### 1.1.3 在FTPServer上的配置
任务：
? 为域用户创建用户隔离的FTP站点
步骤：
**9. **在FTPServer上，点击“开始”à“程序”à“管理工具”à“Internet 信息服务(IIS) 6.0 管理器”。
**10. **右击“FTP站点”，点击“新建”à“FTP站点”。
**11. **在出现的欢迎使用FTP站点创建向导对话框，点击“下一步”。
![clip_image009](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001717qJNx.png)![clip_image010](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001755f40g.png)
**12. **在出现的FTP站点概述，输入描述，点击“下一步”。
**13. **在出现的IP地址和端口设置，IP地址选择“全部未分配”，端口默认21，点击“下一步”。
![clip_image011](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001778qOgS.png)![clip_image012](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001806CYIM.png)
**14. **在出现的FTP用户隔离对话框，选择“隔离用户”。
**15. **在出现的FTP站点主目录对话框，路径输入d:\ftproot，点击“下一步”。
![clip_image013](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001830e6Dp.png)![clip_image014](http://91xueit.blog.51cto.com/attachment/201302/4/400469_13600018352yYP.png)
**16. **在出现的FTP站点访问权限对话框，选择“读取”和“写入”，点击“下一步”。
**17. **在出现的已成功完成FTP站点创建向导，点击“完成”。
![clip_image015](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001851AaHY.png)![clip_image016](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001867YrLJ.png)
##### 1.1.4 在InternetPC上测试
任务：
? 使用域用户登录FTP站点
? 验证FTP域用户隔离
步骤：
**18. **在InternetPC上，打开IE浏览。
**19. **输入[ftp://172.16.0.200](ftp://172.16.0.200)，可以看到不需要登录直接能够访问FTP站点，是以匿名用户登陆的。
**20. **右击空白处，点击“登录”。输入用户名ess\jiangp和密码，意味着该用户是域用户，如果输入ftpserver\jiangp，意味着该用户是ftpserver服务器上的本地用户。
![clip_image017](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001910SoWZ.png)![clip_image018](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001964Ps63.png)
**21. **在桌面上创建一个记事本文件，拖拽到ftp站点。
**22. **以ftpServer本地用户hanLG登录。
![clip_image019](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360001998CMxb.png)![clip_image020](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360002048phLt.png)
**23. **创建一个记事本文件hanLG.txt，上传到的ftp。
**24. **在FTPServer上，可以看到域用户jiangp存放的文件，已经放到了FTP根目录ess\jiangp目录下。
![clip_image021](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360002095G4Ha.png)![clip_image022](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360002104yzNH.png)
微软最有价值专家（MVP）从业12年录制500小时16G企业培训视频 视频介绍网址 [http://www.91xueit.com](http://www.91xueit.com)
![titel2](http://91xueit.blog.51cto.com/attachment/201302/4/400469_1360002108rydB.jpg)
![123](http://91xueit.blog.51cto.com/attachment/201302/4/400469_136000211235tS.jpg)
