# iOS仿网易新闻、新浪新闻的新闻客户端 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月05日 09:43:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2003
本文来自简书，原文地址:[http://www.jianshu.com/p/1172711c0761](http://www.jianshu.com/p/1172711c0761)
之前做了一个仿照网易新闻、新浪新闻之类的新闻类app。demo地址[github](https://github.com/gaoyuhang/DayDayNews)
实现新闻浏览，视频播放，抓取百度图片，瀑布流显示,自定义视频播放，横屏竖屏切换自如,设置界面优化，第三方登录以及注销，新闻数据以及图片的收藏与分享。
## Demo扫码安装
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/wcnS.png)
image
ipa上传到蒲公英分发平台，扫码根据提示等待就可以按照完毕 
安装完成之后第一次打开会出现未受信任的企业级开发者。 解决方法：在设置->通用->设备管理，点击信任即可
## GIF
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/news.gif)
gif
- 仿照网易新闻首页的显示方式,详情页收藏和分享
- 天气预报展示
- 图片抓取的百度图片做的瀑布流显示，收藏分享下载
- 视频界面也是仿照网易新闻的样式来做的
- 我的界面实现了第三方登录、夜间模式、收藏功能
- 我的界面帮助与反馈增加了环信即时通讯（目前只限于文字输入，马上就会完善）
## 首页以及顶部新闻详情，高仿网易
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/newsfresh.png)
首页
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/newsdata.png)
首页
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/detail.png)
新闻详情
## 使用瀑布流实现图片，可以选择分类
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/photo.png)
图片
## 增加了天气预报的功能，可以实现定位到当前城市。动画效果也没有放过。
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/weather.PNG)
天气预报
## 视频的播放以及横屏播放
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/%E5%8A%A0%E8%BD%BD.png)
视频
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/%E6%92%AD%E6%94%BE.png)
播放
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/%E6%A8%AA%E5%B1%8F.png)
横屏
## 我的界面实现第三方登陆以及注销，界面优化。
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/login.png)
我的
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/me.png)
我的
## 夜间模式和收藏功能图片
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/yejian.png)
夜间模式界面
![](https://raw.githubusercontent.com/gaoyuhang/DayDayNews/master/photo/photocollect.png)
收藏
## 项目所用API
附上部分功能API，只做学习交流。具体使用请见`Demo`
- 新闻顶部轮播图数据
`http://c.m.163.com/nc/article/headline/T1348647853363/0-10.html`
- 首页新闻数据
`http://c.m.163.com/nc/article/headline/T1348647853363/0-20.html`
- 天气预报接口
`http://c.3g.163.com/nc/weather/省份|城市.html`
- 图片接口
`http://image.baidu.com/wisebrowse/data?tag1=一级分类&tag2=二级分类&pn=0&rn=60`
- 视频接口
`http://c.m.163.com/nc/video/home/0-10.html`
## About
本项目只供学习交流，严禁进行商业用途。:joy:
[GitHub](https://github.com/gaoyuhang/DayDayNews)
