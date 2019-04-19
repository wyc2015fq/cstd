# python selenium处理windows窗口 - _天枢 - 博客园
## [python selenium处理windows窗口](https://www.cnblogs.com/yhleng/p/9837992.html)
2018-10-23 17:36 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9837992)
selenium本身处理不了windows窗口，需要借助，PyAutoit包 与autoit工具
**这里以文件上传窗口为例：**
1.安装python pyauto包
```
pip install PyAutoit
```
 2.安装autoit工具，下载地址如下，这里使用的是autoit3版本
[https://www.autoitscript.com/site/autoit/downloads/](https://www.autoitscript.com/site/autoit/downloads/)
3.处理文件上传window窗口
![](https://img2018.cnblogs.com/blog/1149221/201810/1149221-20181023172016479-1882959866.png)
需要完成以下步骤：
1>.输入文件路径
2>.点击，打开按钮
4.文件上传autoit python代码实现
1>通过auto软件，抓取上传窗口，属性
2>引用python引用autoit包
```
import autoit
#设置焦#点
autoit.control_focus("打开","[Class:Edit; instance:1]")
#输入文本
autoit.control_set_text("打开","[Class:Edit; instance:1]",r"C:\Users\Administrator\Desktop\226523.jpg")
# 单击按钮
autoit.control_click("打开","[Class:Button; instance:1]")
```
5.代码分解
以上三行代码即可，当然，你可能不太明白这些参数都是什么意思，那么下边我们分别看一下原型。
**Title**:auto抓取到的Title如下图
**control**:就是autoit抓出的control中的属性。
**control,在python代码中用 "[属性:属性值;属性2:属性值2]" 表示**
![](https://img2018.cnblogs.com/blog/1149221/201810/1149221-20181023172407653-791181278.png)
1>autoit.control_focus():
```
@api.check(2, "Window/Control could not be found")
def control_focus(title, control, **kwargs):
    """
    :param title:
    :param control:
    :param kwargs:
    :return:
    """
    text = kwargs.get("text", "")
    ret = AUTO_IT.AU3_ControlFocus(
        LPCWSTR(title), LPCWSTR(text), LPCWSTR(control))
    return ret
```
 2>autoit.control_set_text():
```
@api.check(2, "Window/Control could not be found")
def control_set_text(title, control, control_text, **kwargs):
    """
    :param title:
    :param control:
    :param control_text:
    :param kwargs:
    :return:
    """
    text = kwargs.get("text", "")
    ret = AUTO_IT.AU3_ControlSetText(
        LPCWSTR(title), LPCWSTR(text), LPCWSTR(control), LPCWSTR(control_text)
    )
    return ret
```
3>autoit.control_click():
```
@api.check(2, "send click message failed")
def control_click(title, control, **kwargs):
    """
    :param title:
    :param text:
    :param control:
    :param button:
    :param clicks:
    :param x:
    :param y:
    :return:
    """
    text = kwargs.get("text", "")
    button = kwargs.get("button", "left")
    clicks = kwargs.get("clicks", 1)
    x = kwargs.get("x", INTDEFAULT)
    y = kwargs.get("y", INTDEFAULT)
    ret = AUTO_IT.AU3_ControlClick(LPCWSTR(title), LPCWSTR(text),
                                   LPCWSTR(control), LPCWSTR(button),
                                   INT(clicks), INT(x), INT(y))
    return ret
```
总结：
1>安装(auit和pyautoit包)
2>抓取(auit抓取上传窗口)
3>编写python代码(设置焦点，输入文本，点击上传)
注意，可以加入一些等待时间，还控制焦点与输入间隔
