# python selenium截取指定元素图片 - _天枢 - 博客园
## [python selenium截取指定元素图片](https://www.cnblogs.com/yhleng/p/9294707.html)
2018-07-11 15:26 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9294707)
1.截取当前屏幕
```
@property
    def getImage(self):
        '''
        截取图片,并保存在images文件夹
        :return: 无
        '''
        timestrmap = time.strftime('%Y%m%d_%H.%M.%S')
        imgPath = os.path.join(gl.imgPath, '%s.png' % str(timestrmap))
        self.driver.save_screenshot(imgPath)
        print  'screenshot:', timestrmap, '.png'
```
2.截取当前元素
```
def getElementImage(self,element):
        """
        截图,指定元素图片
        :param element: 元素对象
        :return: 无
        """
        """图片路径"""
        timestrmap = time.strftime('%Y%m%d_%H.%M.%S')
        imgPath = os.path.join(gl.imgPath, '%s.png' % str(timestrmap))
        """截图，获取元素坐标"""
        self.driver.save_screenshot(imgPath)
        left = element.location['x']
        top = element.location['y']
        elementWidth = left + element.size['width']
        elementHeight = top + element.size['height']
        picture = Image.open(imgPath)
        picture = picture.crop((left, top, elementWidth, elementHeight))
        timestrmap = time.strftime('%Y%m%d_%H.%M.%S')
        imgPath = os.path.join(gl.imgPath, '%s.png' % str(timestrmap))
        picture.save(imgPath)
        print  'screenshot:', timestrmap, '.png'
```
**期待你的加入：**
**python｜测试｜技术交流 qq群：563227894**
**python｜测试｜技术交流 qq群：563227894**
**python｜测试｜技术交流 qq群：563227894**
