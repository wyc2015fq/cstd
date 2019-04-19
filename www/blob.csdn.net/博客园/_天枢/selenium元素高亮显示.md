# selenium元素高亮显示 - _天枢 - 博客园
## [selenium元素高亮显示](https://www.cnblogs.com/yhleng/p/9294762.html)
2018-07-11 15:32 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9294762)
**selenium元素高亮显示：**
采用js注入的方式，通过更改元素样式来实现。
border:2px 边框  solid red 红色
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
** 期待你的加入；共同学习，一起进步：**
**python｜测试｜技术交流 qq群：563227894**
**python｜测试｜技术交流 qq群：563227894**
**python｜测试｜技术交流 qq群：563227894**
