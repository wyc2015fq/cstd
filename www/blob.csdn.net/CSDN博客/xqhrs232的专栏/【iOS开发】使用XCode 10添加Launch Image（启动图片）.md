# 【iOS开发】使用XCode 10添加Launch Image（启动图片） - xqhrs232的专栏 - CSDN博客
2019年03月12日 14:02:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：117
原文地址::[https://blog.csdn.net/liuchuo/article/details/52558743](https://blog.csdn.net/liuchuo/article/details/52558743)
一、Assets.xcassets -> + -> App icons & Launch Images -> New iOS Launch Image 
二、如果想要设置为Launch Image的图片不是png格式的，可以用预览打开后 -> 文件 ->  导出 ->  格式 PNG -> 存储
三、对于不同尺寸的屏幕需要不同尺寸分辨率的图片，可以直接使用预览里面的调整大小功能设置图片的大小。方法为工具 -> 调整大小 -> 单位改成像素 -> 关闭比例缩放 -> 填写宽度和高度
以下是所需Launch Image所需的尺寸：（Protrait是竖屏，Landscape是横屏），将制作好的Launch Image根据图片像素大小放入对应的框中～
四、单击工程名称 -> General -> Launch Images Source改为LaunchImage -> Launch Source File的内容置为空
五、将LaunchScreen.storyboard的设置中的Use as Launch Screen取消
六、此时运行就能看到Launch Image了～
七、如果嫌Launch Image显示的时间太短，可以在ViewController.swift文件的viewDidLoad里面添加一行代码：
<span class="hljs-keyword">override</span> <span class="hljs-function"><span class="hljs-keyword">func</span> <span class="hljs-title">viewDidLoad</span><span class="hljs-params">()</span></span> {
        <span class="hljs-keyword">super</span>.viewDidLoad()
        <span class="hljs-comment">// Do any additional setup after loading the view, typically from a nib.</span>
        <span class="hljs-type">Thread</span>.sleep(forTimeInterval: <span class="hljs-number">3.0</span>)
    }
（以Swift语言为例，Objective-C同理，在此不赘述）
八、此时运行应该已经成功了，如果没有成功，请尝试卸载App后重新运行～
