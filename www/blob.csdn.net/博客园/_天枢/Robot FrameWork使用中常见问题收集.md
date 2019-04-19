# Robot FrameWork使用中常见问题收集 - _天枢 - 博客园
## [Robot FrameWork使用中常见问题收集](https://www.cnblogs.com/yhleng/p/8080587.html)
2017-12-21 14:53 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8080587)
1.**“假死”现象**
　　在完成一个模块的脚本编写后，多次运行没问题，但是隔了几天再来运行的时候，发现脚本运行会出现浏览器那边不动了，脚本这边的时间一直在跑。
![](https://images2017.cnblogs.com/blog/1149221/201712/1149221-20171221142708100-1518079963.png)
**　　问题原因及解决方法:**
　　　　问题解决了，原因是**driver.exe这个进程没退出，我编写脚本的时候都没写退出浏览器，加了退出浏览器就不会出现这种现象了
**2.总是提示找不到元素**
**　　使用IEDriverServer.exe驱动,运行脚本时总是提示,找不到页面元素**
**　　问题及解决方法:**
**　　运行脚本的时候总是提示找不到元素，解决办法是将ie的安全级别修改为低，而且将保护模式去掉。**
**3.一些通过selenium无法定位的元素**
　　对于一些能过selenium无法定位的元素, 可以发散一下思路, 使用js解决
**例:**对于window.showModalDialog()模式打开的新窗口,通过selennium无法定位 ,使用js dom来解决.
```
function(){document.getElementById('btnid').click();}
```
**4.RF发送键盘向下按键**
api给的很简单，press key  xxxx
 查了向下键的ascii为40，enter键的为13.但是久经试验都不对。网上说可以考虑看下selenium的源码中向下键的标识用“\ue015”，好奇怪，我也不懂，就用了，果真对了。
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
