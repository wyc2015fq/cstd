# ElementNotVisibleException: Message: element not visible - _天枢 - 博客园
## [ElementNotVisibleException: Message: element not visible](https://www.cnblogs.com/yhleng/p/9318845.html)
2018-07-16 17:32 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9318845)
**selenium自动化测试中，经常会报异常：**
可能会有各种疑问，元素可以定位到啊。为什么报以下异常？
**ElementNotVisibleException: Message: element not visible**
原因：
元素在前台代码document中可以找到，但是不代表该元素就显示在了页面上。
所以报上述异常的原因就是，元素未显示在界面上。
能过我们测试中会自定义一下，找元素的功能：
```
def find_element(self,*loc):
        """
        在指定时间内，查找元素；否则抛出异常
        :param loc: 定位器
        :return: 元素 或 抛出异常
        """
        TimeOut = 20
        try:
            self.driver.implicitly_wait(TimeOut) #智能等待；超时设置
            element = self.driver.find_element(*loc) #如果element没有找到，到此处会开始等待
            if self.isDisplayTimeOut(element,TimeOut):
                self.hightlight(element)  #高亮显示
            else:
                raise ElementNotVisibleException #抛出异常，给except捕获
            self.driver.implicitly_wait(0) #恢复超时设置
            return element
        except (NoSuchElementException,ElementNotVisibleException,UnexpectedAlertPresentException) as ex:
            self.getImage
            raise ex
```
判断元素是否在页面显示：
```
def isDisplayTimeOut(self,element,timeSes):
        """
        在指定时间内，轮询元素是否显示
        :param element: 元素对象
        :param timeSes: 轮询时间
        :return: bool
        """
        start_time = int(time.time()) #秒级时间戳
        timeStr = int(timeSes)
        while (int(time.time())-start_time) <= timeSes:
            if element.is_displayed():
                return True
            self.wait(500)
        self.getImage
        return False
```
 期待你的加入；共同学习，一起进步：
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
