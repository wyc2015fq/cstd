# 用 Mathpix 把图片转化为 LaTeX - 知乎
# 

有时候看那种扫描版的书, 里面狗爬一样的公式看的你无语...

公式 OCR 在 Deep-learning 火热的今天还是个比较难的问题...

[Mathpix](https://link.zhihu.com/?target=https%3A//mathpix.com/) 是一个将图片转化为 LaTeX 的在线服务, 这个软件还特意对 WolframAlpha 做了优化, 可以解析出 WolframAlpha 友好的形式.

然后就是喜闻乐见的抓包拿API了, 帮你们抓好了:
[Moe-Net/Mathpix​github.com![图标](https://pic2.zhimg.com/v2-7c255044ac363b2ee9d758675914b971_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/Moe-Net/Mathpix)
一行导入:

`Import@"https://raw.githubusercontent.com/Moe-Net/Mathpix/master/Mathpix.m"`

然后就可以愉快的玩耍了, 这个 API 主要就做了三件小事
- 一个是把图片转化为 LaTeX
![](https://pic4.zhimg.com/v2-489b1aa14782ee371327fce5a28d2937_b.jpg)
然后就可以用一些外部 API 渲染公式了, 比如 MathJax 或者用知乎也行

`SystemOpen["https://www.zhihu.com/equation?tex="<>URLEncode["这一团"]]`
- 第二个就是尝试自己渲染公式
![](https://pic2.zhimg.com/v2-73dc041622eacca6fa196228836819b5_b.jpg)
自己渲染肯定是没 MathJax 这种成熟方案好的啦
- 第三个就比较重要了, 尝试解析为 Mathematica 表达式
![](https://pic2.zhimg.com/v2-cfc0cc0f56cfbbd7447f3f0e2a273411_b.jpg)
当然只能解析比较简单的式子, 如果很复杂容易失败

题图: [https://www.pixiv.net/member_illust.php?mode=medium&illust_id=69767501](https://link.zhihu.com/?target=https%3A//www.pixiv.net/member_illust.php%3Fmode%3Dmedium%26illust_id%3D69767501)

图有删减, 竖幅一般比较奔放, 剪成横幅就比较正常了...

