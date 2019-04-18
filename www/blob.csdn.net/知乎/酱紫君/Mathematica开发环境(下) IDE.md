# Mathematica开发环境(下): IDE - 知乎
# 

开发环境可选 InteliJ Idea 和 Eclipse.

Mathematica InteliJ 是SE社区开发的强力插件: [halirutan/Mathematica-IntelliJ-Plugin](https://link.zhihu.com/?target=https%3A//github.com/halirutan/Mathematica-IntelliJ-Plugin)

JetBrain 的 Idea 体验当然是极棒的, 基本上都是一键安装不用教...
![](https://pic2.zhimg.com/v2-556a0beb2e8282087699772d9743370d_b.gif)
但是我要说的是如何使用Eclipse, 这是Wolfram官方开发的, 体验么...聊胜于无吧...

以前叫 Wolfram WorkBench, 买 Mathematica 可以在官网上下安装包...

体验极差, 然后总是不更新, 还不支持最新版本(整个Mathematica 10周期都没更新)

最后取消掉了, 现在就是纯粹的 Eclipse 插件了...

上官网下下载器: [https://www.eclipse.org/downloads/eclipse-packages/](https://link.zhihu.com/?target=https%3A//www.eclipse.org/downloads/eclipse-packages/)
![](https://pic2.zhimg.com/v2-ab344f48616460f0982eda719867354d_b.jpg)
然后装谁是无所谓的, 安装失败的话自行按照报错百度

一般都是JRE,mingw啊, 什么路径问题啊, 老Java没卸干净啊, 乱七八糟的...

(第一次装很大概率会失败...大多数人失败三次以上,要有耐心...)
![](https://pic1.zhimg.com/v2-31ae7e66e518ee50f5968384ce51927c_b.jpg)点击Update之后会出现最小化安装, 以下以Platform版为例.![](https://pic2.zhimg.com/v2-127b4d401c89e72e386433f068404111_b.jpg)应该是只要有JVM就可以安装通过的
第一次打开要设置WorkSpace
![](https://pic4.zhimg.com/v2-aef980d0fafa02187da7b0628b48a94f_b.jpg)这个版本连Market都没有我的天...好吧Market上也搜不到Mathematica.
输入服务器然后等:

`Wolfram Workbench - http://workbench.wolfram.com/update/`
![](https://pic3.zhimg.com/v2-a242d6c5245ac110ded551bc5ec5276a_b.jpg)
然后 Agree,Finish,Ignore Waring 就行了.

第一次安装可能会给你默认设置成9.0,需要手动指向最新版.
![](https://pic2.zhimg.com/v2-6ab9f6f5190adff4200d2551dbe73169_b.jpg)
如果你要用中文还要点额外设置:

汉化么...我反正不需要...毕竟一部分汉化一部分没汉化还不如全英文...
![](https://pic3.zhimg.com/v2-937affc13b14e9c058ea77b6c82bd896_b.jpg)
如果你不想看见满屏Waring的话把Non-ASCII点掉, 其他随你便.
![](https://pic4.zhimg.com/v2-6edd9468167eca78a8541f60836011db_b.jpg)
另外默认的 Consolas 没有中文, 自行选择一种带中文的程序员字体.

题图代码:

```
Graphics[{
    EdgeForm[Gray],
    Nest[{{Black,Rectangle[{,-.5}],White,Rectangle[{,-.5}]},Scale[Rotate[#,\[Pi]/],.95]}&, {}, ]}
]
Export["31770707.png",%,Background->None,ImageResolution->]
```

