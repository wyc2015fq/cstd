# Selenium基础知识(五)多窗口切换 - _天枢 - 博客园
## [Selenium基础知识(五)多窗口切换](https://www.cnblogs.com/yhleng/p/7510154.html)
2017-09-12 14:54 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7510154)
**说到多窗口切换必须想到driver.switch_to.window()方法**
**driver.switch_to.window() 实现在不同窗口之间切换**
**driver.current_window_handle() 获取当前窗口，聚丙**
**driver.window_handles() 获得所有窗口，聚丙**
```
curHandle = driver.current_window_handle() #当前窗口聚丙
    winHandle = driver.window_handles() #所有窗口聚丙
    for handle in winHandle:
        if handle !=curHandle:
            driver.switch_to.window(handle) #切换窗口
```
**已知，当前窗口聚丙，已知所有窗口聚丙，很容易在各窗口之前进行切换，需要灵活运用。**
