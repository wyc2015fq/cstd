# selenium处理弹出窗口 - _天枢 - 博客园
## [selenium处理弹出窗口](https://www.cnblogs.com/yhleng/p/9295110.html)
2018-07-11 16:14 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9295110)
**在selenium ui自动化测试过程中，经常会遇到，弹出新窗口，那么会对我们元素定位造成哪些影响呢？**
1.元素id或xpath;name都没有问题，就是定位不到。
那如何解决呢？没错webdriver本身提供了一个switch_to功能。
我们先切换到新窗口；所谓的切换，是将弹出的窗口，聚丙，切换到当前聚丙。
**代码示例：**
```
@property
    def switch_window(self):
        """
        切换window窗口,切换一次后退出
        :return: 无
        """
        curHandle = self.driver.current_window_handle #获取当前窗口聚丙
        allHandle = self.driver.window_handles #获取所有聚丙
　　　　
　　　　　
        """循环判断，只要不是当前窗口聚丙，那么一定就是新弹出来的窗口，这个很好理解。"""
        for h in allHandle:
            if h != curHandle:
                self.driver.switch_to.window(h) #切换聚丙，到新弹出的窗口
                break
```
完成如上切换，那么你再定位，新窗口上的元素，相信，没有问题了。
 期待你的加入；共同学习，一起进步：
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
