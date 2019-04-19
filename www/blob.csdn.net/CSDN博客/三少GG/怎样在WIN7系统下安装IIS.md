# 怎样在WIN7系统下安装IIS - 三少GG - CSDN博客
2013年03月12日 01:12:55[三少GG](https://me.csdn.net/scut1135)阅读数：976

# 怎样在WIN7系统下安装IIS  [http://jingyan.baidu.com/article/19192ad853224ce53f570748.html](http://jingyan.baidu.com/article/19192ad853224ce53f570748.html)
- 
- 
|
- 
浏览： 79288
- 
|
- 
更新： 2012-03-03 14:07
- 
|
- 
标签： [windows7](http://jingyan.baidu.com/tag/12)
步骤
- ![怎样在WIN7系统下安装IIS](http://h.hiphotos.baidu.com/exp/scrop=50/sign=5040c5908a82b90139f3846c03b09c47/2fdda3cc7cd98d109e55e286213fb80e7aec9080.jpg)1[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=1)
- ![怎样在WIN7系统下安装IIS](http://g.hiphotos.baidu.com/exp/scrop=50/sign=f5c892d30923dd54252de037a13486ee/267f9e2f0708283821eb32a3b899a9014d08f1a7.jpg)2[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=2)
- ![怎样在WIN7系统下安装IIS](http://d.hiphotos.baidu.com/exp/scrop=50/sign=3a2964897bec54e745b25d41c905ae60/aa64034f78f0f736b4d5a5b60a55b319ebc41359.jpg)3[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=3)
- ![怎样在WIN7系统下安装IIS](http://b.hiphotos.baidu.com/exp/scrop=50/sign=9e997dc3ad345982c1d4a2cd7cc9049d/34fae6cd7b899e5106f7d3a342a7d933c8950d0f.jpg)4[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=4)
- ![怎样在WIN7系统下安装IIS](http://e.hiphotos.baidu.com/exp/scrop=50/sign=f15f59b2e5dde711e38c04a9d7d2fb2b/b64543a98226cffc3a2a5605b9014a90f603ea01.jpg)5[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=5)
- ![怎样在WIN7系统下安装IIS](http://f.hiphotos.baidu.com/exp/scrop=50/sign=227d6595c8fcc3ceb09e8e6ce278e3b8/9922720e0cf3d7ca1ea6d6c1f21fbe096b63a931.jpg)6[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=6)
- ![怎样在WIN7系统下安装IIS](http://c.hiphotos.baidu.com/exp/scrop=50/sign=d3974cd59822720e7f90a5a50bf63f76/fc1f4134970a304eff7f494dd1c8a786c9175c2c.jpg)7[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=7)
- ![怎样在WIN7系统下安装IIS](http://e.hiphotos.baidu.com/exp/scrop=50/sign=fd95ef7d828ba61edbb08f703109a23a/63d9f2d3572c11dfe54c5694632762d0f603c2ec.jpg)8[](http://jingyan.baidu.com/album/19192ad853224ce53f570748.html?picindex=8)
在此根据多年的网站开发经验，把如何在WIN7系统下如何安装IIS的方法以图文的形式分享给大家。
## 工具/原料
- 
Windows7系统 （IIS是WIN7自带的，版本7.0）
## 步骤/方法
- 
**首先是安装IIS。打开控制面板，找到“程序与功能”，点进去**
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/f15e24292df5e0fe069967b65c6034a85fdf7241.jpg)
- 
**点击左侧“打开或关闭Windows功能”**
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/3c2c4bfbb2fb431653f994cd20a4462308f7d360.jpg)
- 
**找到“Internet 信息服务”，按照下图打勾即可**
等待安装完成
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/b25d9901a18b87d63c6942a8070828381f30fd1b.jpg)
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/cebd00178a82b90143d42a84738da9773812efc9.jpg)
- 
**安装完成后，再回到控制面板里面，找到“管理工具”，点进入**
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/e865a699a9014c084a3bc1120a7b02087af4f4c2.jpg)
- 
**双击“Internet 信息服务(IIS)管理器”就是IIS了。如果经常需要使用IIS的朋友，建议鼠标指到“Internet 信息服务(IIS)管理器”上，右键--发送到--桌面快捷方式，这样就能从桌面进入IIS，而不用每次都点控制面板。**
IIS界面：
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/3bb22487e950352aa331ccbf5343fbf2b3118bf2.jpg)
- 
**选中“Default Web Site”，双击中间的“ASP”**
把“启用父路径”改为“True”
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/a583631ed21b0ef4c1b571e9ddc451da80cb3eed.jpg)
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/0b07ec1fbe096b63ba84b7e70c338744eaf8acad.jpg)
- 
**下面就是配置站点了。单击右侧“高级设置”**
选择你网站的目录
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/0ef2112442a7d93327310dc4ad4bd11372f001a9.jpg)
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/c856613e6709c93db40be7409f3df8dcd0005449.jpg)
- 
**回到IIS，单击右侧的“绑定...”，选中你要绑定的网站，点“编辑”**
如果是一台电脑，只修改后面的端口号就行，可以随意修改数字。如果是办公室局域网，点击下拉框，选择自己电脑上的局域网IP，例如：192.168.**.**，然后修改端口号
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/5f9e93b1cb134954afe901d3564e9258d0094a57.jpg)
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/dc15484e9258d10918094005d158ccbf6d814d64.jpg)
- 
**回到IIS，如果右侧显示停止，就选择启动，然后点击下面的浏览网站，就可以打开你绑定文件夹里面的网站了**
![怎样在WIN7系统下安装IIS](http://hiphotos.baidu.com/exp/pic/item/425773224f4a20a4792dd47090529822720ed013.jpg)
- 
**安装完成**
这时候只用在地址栏里面输入：http://然后localhost:80 或者
http:// 然后 127.0.0.1
可以运行网站了，注意后面这个只有是端口是80时才会有效
经验内容仅供参考，如果您需要解决具体问题（尤其在法律、医学等领域），建议您详细咨询相关领域专业人士。
