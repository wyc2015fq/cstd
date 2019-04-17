# Firefox创建firefoxprofile - zhusongziye的博客 - CSDN博客





2018年02月07日 15:22:26[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：566









 我们自动化测试的时候，有时不需要图片加载出来，提高浏览器加载速度，从而提高脚本的执行速度。另外在一些网络比较差的环境下，禁用css、图片等加载可以提高访问速度


方法：


      1、创建自己的firefoxprofile

> 
关闭所有Filefox浏览窗口—》运行Firefox Manager，点击windows“开始”-->“运行”，然后在输入框输入**firefox.exe -ProfileManager**，

![](https://img-blog.csdn.net/20180207151356285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


点击“确定”按钮-》创建一个新的Profile，只需点击“创建配置文件”按钮，在接着出现的向导窗口里输入配置文件名

![](https://img-blog.csdn.net/20180207151438972?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180207151527395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180207151659047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180207151744203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


现在就创建了一个独立的Firefox Profile配置文件，**启动Profile配置文件的Firefox**



新建一个bat文件，输入命令行

set MOZ_NO_REMOTE=1 

start "" "C:\Program Files\Mozilla Firefox Formal\firefox.exe" -P firefoxprofile文件名

在新打开的浏览器中，你可以随意设置火狐，这些设置都保存在你的个性化firefoxprofile中





     2、现在使用selenium调用firefoxprofile

> firefox_profile = webdriver.FirefoxProfile("D:\Downloads")

browser = webdriver.Firefox(firefox_profile=firefox_profile)D:\Downloads路径存放着我firefoxprofile



