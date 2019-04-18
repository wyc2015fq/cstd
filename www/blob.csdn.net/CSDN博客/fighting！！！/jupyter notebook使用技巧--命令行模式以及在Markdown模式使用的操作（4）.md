# jupyter notebook使用技巧--命令行模式以及在Markdown模式使用的操作（4） - fighting！！！ - CSDN博客
2019年02月24日 23:03:05[dujiahei](https://me.csdn.net/dujiahei)阅读数：16
转自：[https://blog.csdn.net/weixin_38774821/article/details/80628591](https://blog.csdn.net/weixin_38774821/article/details/80628591)
正常情况，jupyter notebook的cell是绿色的，我们可以在绿色cell里输入代码
但如果我们将鼠标点击cell框的左侧，点击，会发现cell变成蓝色的，很奇妙~
当cell框变成蓝色的时候，记作命令行模式。
在命令行模式中，我们可以对本次编辑的内容进行额外操作。
按下大写键（就是shift上面那个键），此时大写锁定打开，在命令行模式中，
按下A：向上增加空白的cell
按下B：向下增加空白的cell
按下D两次（DD）：删除该cell
按下X：剪贴该cell
按下V：粘贴该cell
按下L：打开、关闭行号
按下M：进入Markdown模式
按下Y：退出Markdown模式，回到代码编辑模式
当进入Markdown模式的时候，cell左边的 In【】会消失掉
1.标题效果
在Markdown模式中我们可以通过如下操作实现一些特别的注释
在Markdown模式下，运行
* 1234（注意星号和数字之间有空格）
以及
# 1（注意井号和数字之间有空格）
## 12
### 123
#### 1234
可以得到：
通过这种方法可以做出标题的效果。
2.黑体字效果
通过两个星号加空格的方法，可以将注释字体变成黑体（前后都要加星号）。运行以上cell，得到：
3.插入表格效果
在Markdown模式下，输入
回车，即可自动生成表格，无需手动对齐，jupyter notebook会做到自动对齐，十分方便。
好了，Jupyter notebook还有很多其他的用法功能，比如插入图片（动图也可以），暂时在此处不做过多的详述，具体的实际操作等我们后面的章节说到了在慢慢分析解释。谢谢~
--------------------- 
作者：D3哥在德国 
来源：CSDN 
原文：https://blog.csdn.net/weixin_38774821/article/details/80628591 
版权声明：本文为博主原创文章，转载请附上博文链接！
