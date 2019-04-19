# PADS设计四层板时的走线层设置 - xqhrs232的专栏 - CSDN博客
2015年10月16日 11:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2035
原文地址::[http://blog.sina.com.cn/s/blog_a85e142101010e5k.html](http://blog.sina.com.cn/s/blog_a85e142101010e5k.html)
相关文章
1、PADS Layout四层板设置简明教程----[http://wenku.baidu.com/link?url=JqaKLcvd9AK5qe4yx3_qB6Zd-NfVaeHxhNISwe87oAz3s6E8aTZOgzcTp6KT4GXqqC5RwE347HNetD3UKq6whH8lSODS36H7uABa2JJAZ-y](http://wenku.baidu.com/link?url=JqaKLcvd9AK5qe4yx3_qB6Zd-NfVaeHxhNISwe87oAz3s6E8aTZOgzcTp6KT4GXqqC5RwE347HNetD3UKq6whH8lSODS36H7uABa2JJAZ-y)
在四层板设计中，只有顶层和底层走线，不允许在地层和电源层走线。那么在哪里设置呢？
在setup—>design rules—>default—>routing中的layer biasing中选择。
另外，在tools—>options—>routing中有layer pair选项，定义布线层对(Routing Layer Pair) 可以缩短在交互布线期间手工改变层时所花费的时间。（就是定义按F4时在哪两层间切换）布线层对(Pairing Routing Layers)规定了层的改变，限制只在所定义的层对之间进行。对于一个四层板，定义的布线层对是主元件面(Primary Secondary Side)和次元件面(Secondary Component Side) 。
为了定义一个层对(Layer Pair):
    1.  选择工具/ 选项(Tools/Options)。
2.  选择布线(Routing) 表格。
3.  在层对(Layer Pair)区域，设置第一个层(First Layer) 为主元件面(Primary
Component Side) 以及第二个层(Second Layer) 为第二个元件面(Secondary
Component Side)。
4.  点击OK按钮，离开选项(Options) 对话框。
