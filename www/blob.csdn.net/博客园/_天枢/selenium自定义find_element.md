# selenium自定义find_element - _天枢 - 博客园
## [selenium自定义find_element](https://www.cnblogs.com/yhleng/p/9295188.html)
2018-07-11 16:23 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9295188)
**智能轮询元素是否显示：**
```
def isDisplayTimeOut(self,element,timeSes):
    """
    在指定时间内，轮询元素是否显示
    :param element: 元素对象
    :param timeSes: 轮询时间
    :return:
    """
    start_time = int(time.time()) #秒级时间戳
    timeStr = int(timeSes)
    while (int(time.time())-start_time) <= timeSes:
        if element.is_displayed():
            return True
        self.wait(500)
    return False
```
**查找元素：**
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
    except (NoSuchElementException,ElementNotVisibleException) as ex:
        self.getImage
        raise ex
```
**高亮显示：**
```
def hightlight(self,element):
    """
    元素高亮显示
    :param element: 元素对象
    :return: 无
    """
    self.driver.execute_script("arguments[0].setAttribute('style', arguments[1]);",
                               element, "border: 2px solid red;")
```
 期待你的加入；共同学习，一起进步：
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
