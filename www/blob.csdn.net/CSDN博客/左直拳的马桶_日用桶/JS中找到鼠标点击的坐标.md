# JS中找到鼠标点击的坐标 - 左直拳的马桶_日用桶 - CSDN博客
2006年11月03日 15:13:00[左直拳](https://me.csdn.net/leftfist)阅读数：3210
JS中找到鼠标点击的坐标
左直拳
如果想获取页面中鼠标点击的坐标，可以用event.clientX和event.clientY。
问题是要注意这个event。它代表当前事件本身。所以，一定要是在事件触发的函数中，才可能访问到这个event，否则为空，event.clientX什么的也就成了镜花水月。
例如，有函数
GetMouseXY()
{
alert(event.clientX + “,” + event.clientY);
}
现在有一个链接a，希望点击它获取鼠标光标，如下：
链接1：<a href=”javascript:GetMouseXY()”>鼠标光标</a>
点击，报错。
究其原因，是因为点击链接不是一个事件。现在将链接改成：
链接2：<a href=”#” onclick=”javascript:GetMouseXY()”>鼠标光标</a>
就正常了。我之前不明白为什么有时候链接要象链接2 这样写，现在明白了。
event.clientX和event.clientY是返回最上一次鼠标点击事件的坐标。
