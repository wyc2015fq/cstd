# 爬虫进阶教程：极验(GEETEST)验证码破解教程

 2017年11月19日*23:55:43*[ 43](https://cuijiahua.com/blog/2017/11/spider_2_geetest.html#comments) 20,242 °C



## 一、前言

爬虫最大的敌人之一是什么？没错，验证码！[Geetest](https://cuijiahua.com/wp-content/themes/begin/inc/go.php?url=http://www.geetest.com/)作为提供验证码服务的行家，市场占有率还是蛮高的。遇到Geetest提供的滑动验证码怎么破？

一种方法是分析它的js加密方法，通过大量抓包分析找到它的返回参数，直接自动生成需要的参数即可，这种方法工程量大一些，并且官方js脚本一升级，就得重新分析，耗时耗力。

今天为大家介绍的一种方法是，通过Selenium模拟用户滑动解锁。这个方法的优势在于简单，方便更新。但是它的缺点也很明显，速度慢，并且不能制作成api接口的形式。

授人予鱼不如授人予渔，接下来就为大家呈现本教程的精彩内容。不过，在阅读本篇文章之前，请确保你已经掌握[网络爬虫](https://cuijiahua.com/blog/tag/%e7%bd%91%e7%bb%9c%e7%88%ac%e8%99%ab/)基础，如果不具备爬虫基础，请到我的CSDN专栏学习。然后，再来阅读本文，我的专栏地址：[点我查看](https://cuijiahua.com/wp-content/themes/begin/inc/go.php?url=http://blog.csdn.net/column/details/15321.html)

## 二、先睹为快

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://www.cuijiahua.com/wp-content/uploads/2017/11/spider_2_1.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_1.gif)

左侧显示的为自动识别过程，右边是一些打印信息。

## 三、实战分析

我们以国家企业信用信息公式系统为例，这是一个企业信息查询的网站，在每次查询都需要进行一次[验证码识别](https://cuijiahua.com/blog/tag/%e9%aa%8c%e8%af%81%e7%a0%81%e8%af%86%e5%88%ab/)。它所使用的就是GEETEST验证码，它的URL：[点我查看](https://cuijiahua.com/wp-content/themes/begin/inc/go.php?url=http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml)

这个网站是这个样子的：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_2.png)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_2.png)

### 1、过程分析

**要想把大象装冰箱，总共分几步？**

- 把冰箱门打开
- 把大象赛冰箱里
- 把冰箱门关上

那么，现在思考一个问题，通过Selenium模拟用户滑动解锁，总共分几步？**请停在这里，思考五分钟，再继续阅读！**

我们先公布一个粗率的答案：

- 使用Selenium打开页面。
- 匹配到输入框，输入要查询的信息，并点击查询按钮。
- 读取验证码图片，并做缺口识别。
- 根据缺口位置，计算滑动距离。
- 根据滑动距离，拖拽滑块到需要匹配的位置。

其实，将每个步骤拆分开来一点一点实现并不难，接下来进入正文。

### 2、实战开始

**第一步：使用Selenium打开网页，并输入信息，点击查询按钮。**

这部分内容很简单，Selenium基础性的东西我不再讲解，如有不懂，请看我专栏的Selenium相关内容。

编写代码如下：



```python
# -*-coding:utf-8 -*-
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium import webdriver
class Crack():
    def __init__(self,keyword):
        self.url = 'http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml';
        self.browser = webdriver.Chrome('D:\\chromedriver.exe')
        self.wait = WebDriverWait(self.browser, 100)
        self.keyword = keyword
    def open(self):
        """
        打开浏览器,并输入查询内容
        """
        self.browser.get(self.url)
        keyword = self.wait.until(EC.presence_of_element_located((By.ID, 'keyword_qycx')))
        bowton = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, 'btn')))
        keyword.send_keys(self.keyword)
        bowton.click()
    def crack(self):
        # 打开浏览器
        self.open()
if __name__ == '__main__':
    print('开始验证')
    crack = Crack(u'中国移动')
    crack.crack()
```



运行效果如下：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_3.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_3.gif)

**第二步：保存验证码图片**

我们审查元素找打图片的地址，审查结果如下：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_4.png)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_4.png)

可以看到，图片是很多图片合成的，也就是说你只保存所有地址的图片是不行的。它是通过background-position的方法进行合成的。每一个图片是乱的，这个怎么搞？很简单，抓取这些图片的链接，然后根据链接的图片，再合成这张没有缺口的图片，获取缺口图的方法也是如此，都是自己合成。

编写代码如下：

Python

```python
# -*-coding:utf-8 -*-
import time, random
import PIL.Image as image
from io import BytesIO
from PIL import Image
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver import ActionChains
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import requests, json, re, urllib
from bs4 import BeautifulSoup
from urllib.request import urlretrieve
class Crack():
    def __init__(self,keyword):
        self.url = 'http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml';
        self.browser = webdriver.Chrome('D:\\chromedriver.exe')
        self.wait = WebDriverWait(self.browser, 100)
        self.keyword = keyword
        self.BORDER = 6
    def __del__(self):
        time.sleep(2)
        self.browser.close()
    def get_screenshot(self):
        """
        获取网页截图
        :return: 截图对象
        """
        screenshot = self.browser.get_screenshot_as_png()
        screenshot = Image.open(BytesIO(screenshot))
        return screenshot
    def get_position(self):
        """
        获取验证码位置
        :return: 验证码位置元组
        """
        img = self.browser.find_element_by_class_name("gt_box")
        time.sleep(2)
        location = img.location
        size = img.size
        top, bottom, left, right = location['y'], location['y'] + size['height'], location['x'], location['x'] + size['width']
        return (top, bottom, left, right)
    def get_image(self, name='captcha.png'):
        """
        获取验证码图片
        :return: 图片对象
        """
        top, bottom, left, right = self.get_position()
        print('验证码位置', top, bottom, left, right)
        screenshot = self.get_screenshot()
        captcha = screenshot.crop((left, top, right, bottom))
        captcha.save(name)
        return captcha
    def get_images(self, bg_filename = 'bg.jpg', fullbg_filename = 'fullbg.jpg'):
        """
        获取验证码图片
        :return: 图片的location信息
        """
        bg = []
        fullgb = []
        while bg == [] and fullgb == []:
            bf = BeautifulSoup(self.browser.page_source, 'lxml')
            bg = bf.find_all('div', class_ = 'gt_cut_bg_slice')
            fullgb = bf.find_all('div', class_ = 'gt_cut_fullbg_slice')
        bg_url = re.findall('url\(\"(.*)\"\);', bg[0].get('style'))[0].replace('webp', 'jpg')
        fullgb_url = re.findall('url\(\"(.*)\"\);', fullgb[0].get('style'))[0].replace('webp', 'jpg')
        bg_location_list = []
        fullbg_location_list = []
        for each_bg in bg:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][1])
            bg_location_list.append(location)
        for each_fullgb in fullgb:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][1])
            fullbg_location_list.append(location)
        urlretrieve(url = bg_url, filename = bg_filename)
        print('缺口图片下载完成')
        urlretrieve(url = fullgb_url, filename = fullbg_filename)
        print('背景图片下载完成')
        return bg_location_list, fullbg_location_list
    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]
        for location in location_list:
            if location['y']==-58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x'])+10,166)))
            if location['y']==0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x'])+10,58)))
        new_im = image.new('RGB', (260,116))
        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]
        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]
        new_im.save(filename)
        return new_im
    def open(self):
        self.browser.get(self.url)
        keyword = self.wait.until(EC.presence_of_element_located((By.ID, 'keyword_qycx')))
        bowton = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, 'btn')))
        keyword.send_keys(self.keyword)
        bowton.click()
    def get_slider(self):
        """
        获取滑块
        :return: 滑块对象
        """
        while True:
            try:
                slider = self.browser.find_element_by_xpath("//div[@class='gt_slider_knob gt_show']")
                break
            except:
                time.sleep(0.5)
        return slider
    def get_gap(self, img1, img2):
        """
        获取缺口偏移量
        :param img1: 不带缺口图片
        :param img2: 带缺口图片
        :return:
        """
        left = 43
        for i in range(left, img1.size[0]):
            for j in range(img1.size[1]):
                if not self.is_pixel_equal(img1, img2, i, j):
                    left = i
                    return left
        return left    
    
    def is_pixel_equal(self, img1, img2, x, y):
        """
        判断两个像素是否相同
        :param image1: 图片1
        :param image2: 图片2
        :param x: 位置x
        :param y: 位置y
        :return: 像素是否相同
        """
        # 取两个图片的像素点
        pix1 = img1.load()[x, y]
        pix2 = img2.load()[x, y]
        threshold = 60
        if (abs(pix1[0] - pix2[0] < threshold) and abs(pix1[1] - pix2[1] < threshold) and abs(pix1[2] - pix2[2] < threshold)):
            return True
        else:
            return False
    def get_track(self, distance):
        """
        根据偏移量获取移动轨迹
        :param distance: 偏移量
        :return: 移动轨迹
        """
        # 移动轨迹
        track = []
        # 当前位移
        current = 0
        # 减速阈值
        mid = distance * 4 / 5
        # 计算间隔
        t = 0.2
        # 初速度
        v = 0
        
        while current < distance:
            if current < mid:
                # 加速度为正2
                a = 2
            else:    
                # 加速度为负3
                a = -3
            # 初速度v0
            v0 = v
            # 当前速度v = v0 + at
            v = v0 + a * t
            # 移动距离x = v0t + 1/2 * a * t^2
            move = v0 * t + 1 / 2 * a * t * t
            # 当前位移
            current += move
            # 加入轨迹
            track.append(round(move))
        return track
    def move_to_gap(self, slider, track):
        """
        拖动滑块到缺口处
        :param slider: 滑块
        :param track: 轨迹
        :return:
        """
        ActionChains(self.browser).click_and_hold(slider).perform()
        while track:
            x = random.choice(track)
            ActionChains(self.browser).move_by_offset(xoffset=x, yoffset=0).perform()
            track.remove(x)
        time.sleep(0.5)
        ActionChains(self.browser).release().perform()
    def crack(self):
        # 打开浏览器
        self.open()
        
        
        # 保存的图片名字
        bg_filename = 'bg.jpg'
        fullbg_filename = 'fullbg.jpg'
        # 获取图片
        bg_location_list, fullbg_location_list = self.get_images(bg_filename, fullbg_filename)
        # 根据位置对图片进行合并还原
        bg_img = self.get_merge_image(bg_filename, bg_location_list)
        fullbg_img = self.get_merge_image(fullbg_filename, fullbg_location_list)
        
        # 点按呼出缺口
        slider = self.get_slider()
        
        # 获取缺口位置
        gap = self.get_gap(fullbg_img, bg_img)
        print('缺口位置', gap)
        track = self.get_track(gap-self.BORDER)
        print('滑动滑块')
        print(track)
        self.move_to_gap(slider, track)
if __name__ == '__main__':
    print('开始验证')
    crack = Crack(u'中国移动')
    crack.crack()
    print('验证成功')
```

运行效果如下：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_5.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_5.gif)

可以看到，运行之后，我们已经顺利生成了两张图片，一个是缺口图，另一个是非缺口图。

**第三步：计算缺口距离**

根据缺口图和非缺口图，通过比对图像的像素点的大小区别，找到缺口位置。

编写代码如下：

Python

```python
# -*-coding:utf-8 -*-
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from urllib.request import urlretrieve
from selenium import webdriver
from bs4 import BeautifulSoup
import PIL.Image as image
import re

class Crack():
    def __init__(self,keyword):
        self.url = 'http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml'
        self.browser = webdriver.Chrome('D:\\chromedriver.exe')
        self.wait = WebDriverWait(self.browser, 100)
        self.keyword = keyword

    def open(self):
        """
        打开浏览器,并输入查询内容
        """
        self.browser.get(self.url)
        keyword = self.wait.until(EC.presence_of_element_located((By.ID, 'keyword_qycx')))
        bowton = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, 'btn')))
        keyword.send_keys(self.keyword)
        bowton.click()

    def get_images(self, bg_filename = 'bg.jpg', fullbg_filename = 'fullbg.jpg'):
        """
        获取验证码图片
        :return: 图片的location信息
        """
        bg = []
        fullgb = []
        while bg == [] and fullgb == []:
            bf = BeautifulSoup(self.browser.page_source, 'lxml')
            bg = bf.find_all('div', class_ = 'gt_cut_bg_slice')
            fullgb = bf.find_all('div', class_ = 'gt_cut_fullbg_slice')
        bg_url = re.findall('url\(\"(.*)\"\);', bg[0].get('style'))[0].replace('webp', 'jpg')
        fullgb_url = re.findall('url\(\"(.*)\"\);', fullgb[0].get('style'))[0].replace('webp', 'jpg')
        bg_location_list = []
        fullbg_location_list = []
        for each_bg in bg:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][1])
            bg_location_list.append(location)
        for each_fullgb in fullgb:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][1])
            fullbg_location_list.append(location)

        urlretrieve(url = bg_url, filename = bg_filename)
        print('缺口图片下载完成')
        urlretrieve(url = fullgb_url, filename = fullbg_filename)
        print('背景图片下载完成')
        return bg_location_list, fullbg_location_list

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y'] == -58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x']) + 10, 166)))
            if location['y'] == 0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x']) + 10, 58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y']==-58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x'])+10,166)))
            if location['y']==0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x'])+10,58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def get_gap(self, img1, img2):
        """
        获取缺口偏移量
        :param img1: 不带缺口图片
        :param img2: 带缺口图片
        :return:
        """
        left = 43
        for i in range(left, img1.size[0]):
            for j in range(img1.size[1]):
                if not self.is_pixel_equal(img1, img2, i, j):
                    left = i
                    return left
        return left   

    def crack(self):
        # 打开浏览器
        self.open()

        # 保存的图片名字
        bg_filename = 'bg.jpg'
        fullbg_filename = 'fullbg.jpg'

        # 获取图片
        bg_location_list, fullbg_location_list = self.get_images(bg_filename, fullbg_filename)

        # 根据位置对图片进行合并还原
        bg_img = self.get_merge_image(bg_filename, bg_location_list)
        fullbg_img = self.get_merge_image(fullbg_filename, fullbg_location_list)

        # 获取缺口位置
        gap = self.get_gap(fullbg_img, bg_img)
        print('缺口位置', gap)

if __name__ == '__main__':
    print('开始验证')
    crack = Crack(u'中国移动')
    crack.crack()
```

运行结果如下：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_6.png)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_6.png)

这样我们就计算除了缺口位置，接下来就是根据缺口位置，滑动滑块到相应位置。

**第四步：计算滑动轨迹**

我们可以使用瞬间移动，直接在1s内移动到目标位置，结果就是”被吃了“。

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_7.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_7.gif)

匀速直线运动，匀速直线运动大法好！果不其然，还是”被吃了“，继续尝试。

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_8.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_8.gif)

模仿抖抖病患者运动，颤颤巍巍，如履薄冰，估计geetest服务器认为是我外婆在操作吧。

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://www.cuijiahua.com/wp-content/uploads/2017/11/spider_2_9.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_9.gif)

虽然这个方法偶尔会成功，但成功率极低。**最好的方法是什么呢？**

模拟人的运动！你想一下，人在滑动滑块的初期是不是速度快，但是当要接近缺口位置的时候，会减速，因为我得对准缺口位置啊！这怎么实现呢？使用我们**初中学过的物理知识：**

当前速度公式为：

v = v0 + a * t

其中，v是当前速度，v0是初始速度，a是加速度，t是时间。我们刚开始的让加速大，当过了中间位置，降低加速度。使用这个移动过程，移动滑块到缺口位置。

编写代码如下：

Python

```python
# -*-coding:utf-8 -*-
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from urllib.request import urlretrieve
from selenium import webdriver
from bs4 import BeautifulSoup
import PIL.Image as image
import re

class Crack():
    def __init__(self,keyword):
        self.url = 'http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml'
        self.browser = webdriver.Chrome('D:\\chromedriver.exe')
        self.wait = WebDriverWait(self.browser, 100)
        self.keyword = keyword
        self.BORDER = 6

    def open(self):
        """
        打开浏览器,并输入查询内容
        """
        self.browser.get(self.url)
        keyword = self.wait.until(EC.presence_of_element_located((By.ID, 'keyword_qycx')))
        bowton = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, 'btn')))
        keyword.send_keys(self.keyword)
        bowton.click()

    def get_images(self, bg_filename = 'bg.jpg', fullbg_filename = 'fullbg.jpg'):
        """
        获取验证码图片
        :return: 图片的location信息
        """
        bg = []
        fullgb = []
        while bg == [] and fullgb == []:
            bf = BeautifulSoup(self.browser.page_source, 'lxml')
            bg = bf.find_all('div', class_ = 'gt_cut_bg_slice')
            fullgb = bf.find_all('div', class_ = 'gt_cut_fullbg_slice')
        bg_url = re.findall('url\(\"(.*)\"\);', bg[0].get('style'))[0].replace('webp', 'jpg')
        fullgb_url = re.findall('url\(\"(.*)\"\);', fullgb[0].get('style'))[0].replace('webp', 'jpg')
        bg_location_list = []
        fullbg_location_list = []
        for each_bg in bg:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][1])
            bg_location_list.append(location)
        for each_fullgb in fullgb:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][1])
            fullbg_location_list.append(location)

        urlretrieve(url = bg_url, filename = bg_filename)
        print('缺口图片下载完成')
        urlretrieve(url = fullgb_url, filename = fullbg_filename)
        print('背景图片下载完成')
        return bg_location_list, fullbg_location_list

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y'] == -58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x']) + 10, 166)))
            if location['y'] == 0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x']) + 10, 58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y']==-58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x'])+10,166)))
            if location['y']==0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x'])+10,58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def is_pixel_equal(self, img1, img2, x, y):
        """
        判断两个像素是否相同
        :param image1: 图片1
        :param image2: 图片2
        :param x: 位置x
        :param y: 位置y
        :return: 像素是否相同
        """
        # 取两个图片的像素点
        pix1 = img1.load()[x, y]
        pix2 = img2.load()[x, y]
        threshold = 60
        if (abs(pix1[0] - pix2[0] < threshold) and abs(pix1[1] - pix2[1] < threshold) and abs(pix1[2] - pix2[2] < threshold)):
            return True
        else:
            return False

    def get_gap(self, img1, img2):
        """
        获取缺口偏移量
        :param img1: 不带缺口图片
        :param img2: 带缺口图片
        :return:
        """
        left = 43
        for i in range(left, img1.size[0]):
            for j in range(img1.size[1]):
                if not self.is_pixel_equal(img1, img2, i, j):
                    left = i
                    return left
        return left   

    def get_track(self, distance):
        """
        根据偏移量获取移动轨迹
        :param distance: 偏移量
        :return: 移动轨迹
        """
        # 移动轨迹
        track = []
        # 当前位移
        current = 0
        # 减速阈值
        mid = distance * 4 / 5
        # 计算间隔
        t = 0.2
        # 初速度
        v = 0

        while current < distance:
            if current < mid:
                # 加速度为正2
                a = 2
            else:   
                # 加速度为负3
                a = -3
            # 初速度v0
            v0 = v
            # 当前速度v = v0 + at
            v = v0 + a * t
            # 移动距离x = v0t + 1/2 * a * t^2
            move = v0 * t + 1 / 2 * a * t * t
            # 当前位移
            current += move
            # 加入轨迹
            track.append(round(move))
        return track

    def crack(self):
        # 打开浏览器
        self.open()

        # 保存的图片名字
        bg_filename = 'bg.jpg'
        fullbg_filename = 'fullbg.jpg'

        # 获取图片
        bg_location_list, fullbg_location_list = self.get_images(bg_filename, fullbg_filename)

        # 根据位置对图片进行合并还原
        bg_img = self.get_merge_image(bg_filename, bg_location_list)
        fullbg_img = self.get_merge_image(fullbg_filename, fullbg_location_list)

        # 获取缺口位置
        gap = self.get_gap(fullbg_img, bg_img)
        print('缺口位置', gap)

        track = self.get_track(gap-self.BORDER)
        print('滑动滑块')
        print(track)


if __name__ == '__main__':
    print('开始验证')
    crack = Crack(u'中国移动')
    crack.crack()
```



运行效果如下：

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_10.png)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_10.png)

**第五步：移动滑块**

根据返回的每次滑动的距离，我们移动滑块至缺口位置。

编写代码如下：

Python

```python
# -*-coding:utf-8 -*-
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from urllib.request import urlretrieve
from selenium import webdriver
from bs4 import BeautifulSoup
import PIL.Image as image
import re

class Crack():
    def __init__(self,keyword):
        self.url = 'http://bj.gsxt.gov.cn/sydq/loginSydqAction!sydq.dhtml'
        self.browser = webdriver.Chrome('D:\\chromedriver.exe')
        self.wait = WebDriverWait(self.browser, 100)
        self.keyword = keyword
        self.BORDER = 6

    def open(self):
        """
        打开浏览器,并输入查询内容
        """
        self.browser.get(self.url)
        keyword = self.wait.until(EC.presence_of_element_located((By.ID, 'keyword_qycx')))
        bowton = self.wait.until(EC.presence_of_element_located((By.CLASS_NAME, 'btn')))
        keyword.send_keys(self.keyword)
        bowton.click()

    def get_images(self, bg_filename = 'bg.jpg', fullbg_filename = 'fullbg.jpg'):
        """
        获取验证码图片
        :return: 图片的location信息
        """
        bg = []
        fullgb = []
        while bg == [] and fullgb == []:
            bf = BeautifulSoup(self.browser.page_source, 'lxml')
            bg = bf.find_all('div', class_ = 'gt_cut_bg_slice')
            fullgb = bf.find_all('div', class_ = 'gt_cut_fullbg_slice')
        bg_url = re.findall('url\(\"(.*)\"\);', bg[0].get('style'))[0].replace('webp', 'jpg')
        fullgb_url = re.findall('url\(\"(.*)\"\);', fullgb[0].get('style'))[0].replace('webp', 'jpg')
        bg_location_list = []
        fullbg_location_list = []
        for each_bg in bg:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_bg.get('style'))[0][1])
            bg_location_list.append(location)
        for each_fullgb in fullgb:
            location = {}
            location['x'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][0])
            location['y'] = int(re.findall('background-position: (.*)px (.*)px;',each_fullgb.get('style'))[0][1])
            fullbg_location_list.append(location)

        urlretrieve(url = bg_url, filename = bg_filename)
        print('缺口图片下载完成')
        urlretrieve(url = fullgb_url, filename = fullbg_filename)
        print('背景图片下载完成')
        return bg_location_list, fullbg_location_list

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y'] == -58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x']) + 10, 166)))
            if location['y'] == 0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x']) + 10, 58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def get_merge_image(self, filename, location_list):
        """
        根据位置对图片进行合并还原
        :filename:图片
        :location_list:图片位置
        """
        im = image.open(filename)
        new_im = image.new('RGB', (260,116))
        im_list_upper=[]
        im_list_down=[]

        for location in location_list:
            if location['y']==-58:
                im_list_upper.append(im.crop((abs(location['x']),58,abs(location['x'])+10,166)))
            if location['y']==0:
                im_list_down.append(im.crop((abs(location['x']),0,abs(location['x'])+10,58)))

        new_im = image.new('RGB', (260,116))

        x_offset = 0
        for im in im_list_upper:
            new_im.paste(im, (x_offset,0))
            x_offset += im.size[0]

        x_offset = 0
        for im in im_list_down:
            new_im.paste(im, (x_offset,58))
            x_offset += im.size[0]

        new_im.save(filename)

        return new_im

    def is_pixel_equal(self, img1, img2, x, y):
        """
        判断两个像素是否相同
        :param image1: 图片1
        :param image2: 图片2
        :param x: 位置x
        :param y: 位置y
        :return: 像素是否相同
        """
        # 取两个图片的像素点
        pix1 = img1.load()[x, y]
        pix2 = img2.load()[x, y]
        threshold = 60
        if (abs(pix1[0] - pix2[0] < threshold) and abs(pix1[1] - pix2[1] < threshold) and abs(pix1[2] - pix2[2] < threshold)):
            return True
        else:
            return False

    def get_gap(self, img1, img2):
        """
        获取缺口偏移量
        :param img1: 不带缺口图片
        :param img2: 带缺口图片
        :return:
        """
        left = 43
        for i in range(left, img1.size[0]):
            for j in range(img1.size[1]):
                if not self.is_pixel_equal(img1, img2, i, j):
                    left = i
                    return left
        return left   

    def get_track(self, distance):
        """
        根据偏移量获取移动轨迹
        :param distance: 偏移量
        :return: 移动轨迹
        """
        # 移动轨迹
        track = []
        # 当前位移
        current = 0
        # 减速阈值
        mid = distance * 4 / 5
        # 计算间隔
        t = 0.2
        # 初速度
        v = 0

        while current < distance:
            if current < mid:
                # 加速度为正2
                a = 2
            else:   
                # 加速度为负3
                a = -3
            # 初速度v0
            v0 = v
            # 当前速度v = v0 + at
            v = v0 + a * t
            # 移动距离x = v0t + 1/2 * a * t^2
            move = v0 * t + 1 / 2 * a * t * t
            # 当前位移
            current += move
            # 加入轨迹
            track.append(round(move))
        return track

    def get_slider(self):
        """
        获取滑块
        :return: 滑块对象
        """
        while True:
            try:
                slider = self.browser.find_element_by_xpath("//div[@class='gt_slider_knob gt_show']")
                break
            except:
                time.sleep(0.5)
        return slider

    def move_to_gap(self, slider, track):
        """
        拖动滑块到缺口处
        :param slider: 滑块
        :param track: 轨迹
        :return:
        """
        ActionChains(self.browser).click_and_hold(slider).perform()
        while track:
            x = random.choice(track)
            ActionChains(self.browser).move_by_offset(xoffset=x, yoffset=0).perform()
            track.remove(x)
        time.sleep(0.5)
        ActionChains(self.browser).release().perform()

    def crack(self):
        # 打开浏览器
        self.open()

        # 保存的图片名字
        bg_filename = 'bg.jpg'
        fullbg_filename = 'fullbg.jpg'

        # 获取图片
        bg_location_list, fullbg_location_list = self.get_images(bg_filename, fullbg_filename)

        # 根据位置对图片进行合并还原
        bg_img = self.get_merge_image(bg_filename, bg_location_list)
        fullbg_img = self.get_merge_image(fullbg_filename, fullbg_location_list)

        # 获取缺口位置
        gap = self.get_gap(fullbg_img, bg_img)
        print('缺口位置', gap)

        track = self.get_track(gap-self.BORDER)
        print('滑动滑块')
        print(track)

        # 点按呼出缺口
        slider = self.get_slider()
        # 拖动滑块到缺口处
        self.move_to_gap(slider, track)

if __name__ == '__main__':
    print('开始验证')
    crack = Crack(u'中国移动')
    crack.crack()
    print('验证成功')
```



运行上述代码，即实现滑动验证码破解，再看下那个nice的瞬间吧。

[![爬虫进阶教程：极验(GEETEST)验证码破解教程](https://www.cuijiahua.com/wp-content/uploads/2017/11/spider_2_1.gif)](https://cuijiahua.com/wp-content/uploads/2017/11/spider_2_1.gif)

## 五、总结

- 本文抛去了很多具体的实现过程，省略了每行代码的讲解，因为我感觉，既然是进阶教程，那些初级知识就没必要再细讲，学我的初级课程的朋友，应该已经具备了自己分析的能力。
- **本文的破解方法仅用于学习交流，请勿用于任何非法用途。**
- 本文出现的所有代码和，均可在我的github上下载，欢迎Follow、Star：[https://github.com/Jack-Cherish/python-spider](https://cuijiahua.com/wp-content/themes/begin/inc/go.php?url=https://github.com/Jack-Cherish/python-spider)
- **如有问题欢迎留言讨论！**

 

**PS： 如果觉得本篇本章对您有所帮助，欢迎关注、评论、赞！**