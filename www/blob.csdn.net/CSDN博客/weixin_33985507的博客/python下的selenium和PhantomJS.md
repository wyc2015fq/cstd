# python下的selenium和PhantomJS - weixin_33985507的博客 - CSDN博客
2018年10月26日 16:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
一般我们使用python的第三方库requests及框架scrapy来爬取网上的资源，但是设计javascript渲染的页面却不能抓取，此时，我们使用web自动化测试化工具Selenium+无界面浏览器PhantomJS来抓取javascript渲染的页面， 但是新版本的Selenium不再支持PhantomJS了，请使用Chrome或Firefox的无头版本来替代。
如下图：
![](https://img2018.cnblogs.com/blog/209993/201810/209993-20181026164059903-1900696050.png)
这里有2中解决方案， 我采用第一种， 第二种搞了很久也没有成功
**一：降级selenium使用**
pip uninstall selenium #先卸载selenium 
pip install selenium==3.4.3 #指定版本安装selenium
防止以后官网没得下载，先备份个selenium3.4.3和PhantomJS [下载地址](https://pan.baidu.com/s/1iyAInw44FssD16ESmCo_bg)
**二：使用Headless Chrome和Headless Firefox**
使用Headless Chrome
Headless模式是Chrome 59中的新特征。
要使用Chrome需要安装 chromedriver。
[chromedriver驱动大全](http://chromedriver.storage.googleapis.com/index.html)
```
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
chrome_options = Options()
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
br = webdriver.Chrome(chrome_options=chrome_options)
br.get('https://www.baidu.com/')
baidu = br.find_element_by_id('su').get_attribute('value')
print(baidu)
```
使用Headless Firefox
要使用Firebox需要安装 geckodriver。
[geckodriver.exe驱动大全](https://github.com/mozilla/geckodriver/releases)
```
from selenium import webdriver
from selenium.webdriver.firefox.options import Options
firefox_options = Options()
firefox_options.add_argument('--headless')
br = webdriver.Firefox(firefox_options=firefox_options)
br.get('https://www.baidu.com/')
baidu = br.find_element_by_id('su').get_attribute('value')
print(baidu)
```
以上代码我在测试的时候没有成功， 遇到如下错误：
![](https://img2018.cnblogs.com/blog/209993/201810/209993-20181026164619596-804932675.png)
说的是chrome的版本不一致，当然前人也遇到过了， 只是我按照网上说的 没有解决
[Python爬虫Selenium安装](https://blog.csdn.net/wenboyu/article/details/78162536)
[使用python selenium時關於chromedriver的小問題](https://medium.com/@Epicure1709/%E4%BD%BF%E7%94%A8python%E7%9A%84selenium%E6%99%82%E9%81%87%E5%88%B0%E7%9A%84%E4%B8%80%E4%BA%9B%E5%B0%8F%E5%95%8F%E9%A1%8C-7fb5de198ff7)
[Selenium support for PhantomJS has been deprecated, please use headless](https://www.cnblogs.com/shaosks/p/9134257.html)
最后我找了一个老版本解决了问题 [2.33​​​​​​​](https://chromedriver.storage.googleapis.com/index.html?path=2.33/)
Python的模块pywin32中的**win32gui.SystemParametersInfo(**)函数，在使用win32con.SPI_SETDESKWALLPAPER设置Wallpaper时，**其第二个参数为图片路径，图片必须是BMP格式**。如下：
win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER,  imagepath,  1+2)
否则将报错如下：pywintypes.error: (0, 'SystemParametersInfo', 'No error message is available') 关于 [SystemParametersInfo](https://www.cnblogs.com/iylc/p/3416832.html)
在设置壁纸的时候发现img_path = "D://Users//Gavin//PythonDemo//Bing.bmp"失败，但是把路径改为img_path = "D:\\Users\\Gavin\\PythonDemo\\Bing.bmp" 就可以了或者img_path = "D:/Users/Gavin/PythonDemo/Bing.bmp"
[Python更换Windows壁纸，问题与解决方案](https://blog.csdn.net/ZM_Yang/article/details/77801679)
使用pythonwin设置windows的桌面背景 ​​​​​​​
我的demo是参考 [Python爬虫之提取Bing搜索的背景图片并设置为Windows的电脑桌面](https://www.cnblogs.com/jclian91/p/9574031.html)
具体代码：
```
# -*- coding: utf-8 -*-
"""
   此程序用于提取Bing搜索的背景图片并设置为Windows的电脑桌面
"""
from urllib.request import urlretrieve
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
import win32api,win32con,win32gui
from selenium.webdriver.chrome.options import Options
# 利用PhantomJS加载网页
chrome_options = Options()
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
browser = webdriver.Chrome(chrome_options=chrome_options)
# 设置最大等待时间为30s
browser.set_page_load_timeout(30)
url = 'https://cn.bing.com/'
try:
    browser.get(url)
except TimeoutException:
    # 当加载时间超过30秒后，自动停止加载该页面
    browser.execute_script('window.stop()')
# 从id为bgDiv的标签中获取背景图片的信息
t = browser.find_element_by_id('bgDiv')
bg = t.get_attribute('style')
# 从字符串中提取背景图片的下载网址
start_index = bg.index('(')
end_index = bg.index(')')
img_url = bg[start_index+1: end_index]
img_url = img_url.replace('"', '')
# 下载该图片到本地
img_path = "D:\\Users\\Gavin\\PythonDemo\\Bing.bmp"
urlretrieve(img_url, img_path)
# 将下载后的图片设置为Windows系统的桌面
# 打开指定注册表路径
reg_key = win32api.RegOpenKeyEx(win32con.HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, win32con.KEY_SET_VALUE)
# 最后的参数:2拉伸,0居中,6适应,10填充,0平铺
win32api.RegSetValueEx(reg_key, "WallpaperStyle", 0, win32con.REG_SZ, "2")
# 最后的参数:1表示平铺,拉伸居中等都是0
win32api.RegSetValueEx(reg_key, "TileWallpaper", 0, win32con.REG_SZ, "0")
# 刷新桌面
try:
    win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER, img_path, win32con.SPIF_SENDWININICHANGE)
except Exception as e:
    print(e)
```
 以上代码 确实是可以跑起来的（win10 python3.7）， 但是晚上 回家后再win7 就跑不起来，典型的就是 图片问题，需要安装 pip install Pillow  于是 code 变成如下：
```
# -*- coding: utf-8 -*-
"""
   此程序用于提取Bing搜索的背景图片并设置为Windows的电脑桌面
"""
from urllib.request import urlretrieve
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
import win32api, win32con, win32gui
from PIL import Image
from selenium.webdriver.chrome.options import Options
# 利用PhantomJS加载网页
chrome_options = Options()
chrome_options.add_argument('--headless')
chrome_options.add_argument('--disable-gpu')
browser = webdriver.Chrome(options=chrome_options)
# 设置最大等待时间为30s
browser.set_page_load_timeout(30)
url = 'https://cn.bing.com/'
try:
    browser.get(url)
except TimeoutException:
    # 当加载时间超过30秒后，自动停止加载该页面
    browser.execute_script('window.stop()')
# 从id为bgDiv的标签中获取背景图片的信息
t = browser.find_element_by_id('bgDiv')
bg = t.get_attribute('style')
# 从字符串中提取背景图片的下载网址
start_index = bg.index('(')
end_index = bg.index(')')
img_url = bg[start_index + 1: end_index]
img_url = img_url.replace('"', '')
# 下载该图片到本地
img_path = "D:\\Python\\demoBing.jpg"
urlretrieve(img_url, img_path)
bmpImage = Image.open(img_path)
img_path = img_path.replace('.jpg', '.bmp')
bmpImage.save(img_path, "BMP")
# 将下载后的图片设置为Windows系统的桌面
# 打开指定注册表路径
reg_key = win32api.RegOpenKeyEx(win32con.HKEY_CURRENT_USER, "Control Panel\\Desktop", 0, win32con.KEY_SET_VALUE)
# 最后的参数:2拉伸,0居中,6适应,10填充,0平铺
win32api.RegSetValueEx(reg_key, "WallpaperStyle", 0, win32con.REG_SZ, "2")
# 最后的参数:1表示平铺,拉伸居中等都是0
win32api.RegSetValueEx(reg_key, "TileWallpaper", 0, win32con.REG_SZ, "0")
# 刷新桌面
try:
    win32gui.SystemParametersInfo(win32con.SPI_SETDESKWALLPAPER, img_path, win32con.SPIF_SENDWININICHANGE)
except Exception as e:
    print(e)
```
