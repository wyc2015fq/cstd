# Python实现selenium回放时间设置 - _天枢 - 博客园
## [Python实现selenium回放时间设置](https://www.cnblogs.com/yhleng/p/9408052.html)
2018-08-02 16:39 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9408052)
一般在做selenium时会有，回放快慢的需求。
实现思路：
1.一般写selenium会自定义findelement函数，来实现查找元素。
2.在查找函数上加个睡眠时间的装饰器，函数执行完等待若干秒
3.同理可以举一返三的使用，装饰器，可以实现很多的功能。
```
def replay(retime):
    """
    设置回放时间，装饰器
    :param retime: 回放时间,毫秒
    :return: 无
    """
    def _wrapper(func):
        def wrapper(*args,**kwargs):
            ret=func(*args,**kwargs)
            time.sleep(float(retime)/1000)
            return ret
        return wrapper
    return _wrapper
```
找元素，函数：
```
@replay(500)   #等待500毫秒 
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
                self.driver.implicitly_wait(0)  # 恢复超时设置
                return element
            else:
                raise ElementNotVisibleException #抛出异常，给except捕获
        except (
                NoSuchElementException,
                ElementNotVisibleException
                ) as ex:
            self.getImage
            raise ex
        else:
            self.getImage
```
