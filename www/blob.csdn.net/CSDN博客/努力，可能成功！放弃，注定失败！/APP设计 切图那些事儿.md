# APP设计~切图那些事儿 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月21日 14:35:07[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：38标签：[APP设计~切图那些事儿](https://so.csdn.net/so/search/s.do?q=APP设计~切图那些事儿&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](https://upload-images.jianshu.io/upload_images/1440624-781ec3bc1c4fad4d?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
文/AndyLiz **APP切图基本知识点以及注意事项**
1、考虑适配Android 各种复杂的分辨率，如主流的1280X720、800X480、640X480、480X320、以及各种非主流分辨率。
2、.9神器 png
3、PNG24+jpg
![](https://upload-images.jianshu.io/upload_images/1440624-850886e53b08b11f?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
4、可交互控件尽量不小于最小可操作尺寸：android规范是48dp
5、图片中的可视元素尽量在文件中居中，便于对齐和标注。
6、能用代码实现尽量避免用图，如单色色值、渐变、文字、分割线等。
7、考虑控件的通用性。
总之切图是个技术活。所以，小伙伴们不得忽视切图的魅力！
![](https://upload-images.jianshu.io/upload_images/1440624-edd1c56fe7c5455a?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
**按钮的设计**
1、你可以真正点击的区域应该尽量大些，至少不应该小于一个手指的宽度，那样的话点击的命中率要高，否则会有“点击不灵敏”的感觉，当然并不是真正的不灵敏，而是没有点中操作的区域。(用户可不管这些理由，多为用户考虑哦)。
2、对于RadioButton、CheckBox至少应该出3个状态的图：正常、点击、选中。有时不能为了方便就只出两个图：正常、选中。这种情况在点击的过程中也有“响应迟钝”的感觉，因为毕竟在点击状态没有换图。
![](https://upload-images.jianshu.io/upload_images/1440624-c21ef1e030aa3058?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
**排版的设计**
对于整体上的设计应该尽量考虑到好用、实用，而不是能用。我自己的体会是这样的。
1、凡是要响应点击或者其他事件的部分应该尽量和屏幕四周的边框保持一定的距离。如果你非得把这部分放在屏幕边上，那么最好放大操作区域。因为屏幕边上点击并不是那么好用（如果设备加了一个保护套，那么屏幕边上就会很难点），至少我用过的有边框的设备，也就是说屏幕边上直接用手去点大部分情况是点不中的。
![](https://upload-images.jianshu.io/upload_images/1440624-1f2e7a50de24f0ef?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
2、背景和内容的对比度一定要合适。作为有美术基础的都知道这点，但是很多时候在设计的时候就犯这样的错。你的设计一定要考虑到使用环境，如在阳光下。如果对比度不够的话那么在强光的环境下就不好用了，背景和内容就融为一体了，很难分辨。
![](https://upload-images.jianshu.io/upload_images/1440624-6c5e7aedd1555b4b?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
3、尽量少用对话框。尤其是网页中都会有很多的对话框，久而久之人们对对话框也就产生了反感（至少我是这么认为的），能以更好的方式做提示的情况下就尽量避免用对话框，非用不可的情况下那么建议一个界面不超过两处。老是弹对话框的应用，最终弹走的是用户。
![](https://upload-images.jianshu.io/upload_images/1440624-a5d904ba60680cd1?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
4、界面简洁。这点有两重意思，其一就是界面完成的功能很明确、简洁，其二是同类操作尽量在一个界面完成。感觉上这两点有冲突，其实不然，如果真的不能做到既界面简洁又功能统一那么这个设计最好还是再设计设计。
![](https://upload-images.jianshu.io/upload_images/1440624-bcd44774e0b565a7?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
比如用户信息输入功能中第一个界面完成用户名输入、密码，确定后进入第二个界面进行设置头像。像这样的界面的个人认为太罗嗦了。既然都是完成用户信息的功能，那么完全可以设计到一个界面中。这样一来可以把整个功能做得更紧凑，少用一个界面更简洁。
![](https://upload-images.jianshu.io/upload_images/1440624-dcc36045bfcac3c2?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
5、使用一定的动画。使用动画进行界面的过渡让人感觉更柔和、亲近。现在的应用太多了，而且功能强大，你的应用需要脱颖而出那么就必须与众不同，有时一些简单的动画会给你的应用增色不少。
![](https://upload-images.jianshu.io/upload_images/1440624-ebfd86af68eecf9c?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
6、整体布局。我不是专业的美工不过见过很多优秀的应用，应该有发言权吧，呵呵。优秀的应用有些共同的地方，现在总结一下。无论是出于什么考虑，在界面上应该避免左右布局严重不平衡，当然也有故意设计成不对称美的。比如界面的最下面一栏有两个横着排版的按钮，那么这两个按钮最好整体上居中，若是偏左或是偏右那么就有左右不平衡的感觉，要么是左重右轻，要么是左轻右重，看上去有一部分比较空。
![](https://upload-images.jianshu.io/upload_images/1440624-1c99e55a065cb5b4?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
7、屏幕适配。Android开发中最麻烦的问题之一就是屏幕适配的问题，硬件厂商太多，屏幕尺寸太多。一个好的应用应该尽可能多的适配更多的屏幕。UI设计过程中就应该考虑到屏幕变宽、变高、变窄、变短的过程中UI应该如何适配。整体上有个原则，屏幕的适配不要改变整体的布局，也就是布局是不变的只是对图形进行拉伸而已（关键是如何拉伸）。屏幕适配其实也是一些经验，慢慢领悟吧，呵呵。
**在进行APP切图的前，需要做以下6点：**
1、和客户端的技术沟通好，用不同的框架来实现的时候，图会有不一样的切法。例如Tabbar是连背景一起切还是单独把icon做成背景透明的，文字是放在图里还是后面加字。
2、有一些小的icon式按钮，不能只切到icon边缘，而是要考虑到最终实现的时候，是把这个图片做成按钮，和用户交互。所以必须要多留一些透明的边，让能点击的图片在88×88以上，这样用户才保证比较好点到。
![](https://upload-images.jianshu.io/upload_images/1440624-342720b835fa8bc7?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
3、个人的强迫症，如果可以用PNG24就不用PNG32，如果可以用PNG8就不用PNG32。并且用Fireworks优化大小。
4、对于不改变可见图形而又需要加大点击区域的图。那么切图的时候建议在可见图形的四周都加上1像素的透明，这是为了放大拉伸而不产生可见区域的图像失真。
![](https://upload-images.jianshu.io/upload_images/1440624-5a6b841cf75dcee1?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
5、切图的高度。对于一个通用的背景图，如文字圆角边框背景，那么切图的时候并不是效果图上有多高就切多高，为了通用而是只需切一行文字的高度就可以了。不过这也不是绝对的，准确的说应该切的高度H=paddingTop+textHeight+paddingBottom，及文字相对背景的上边距+一行文字的高度+文字相对背景的下边距。
![](https://upload-images.jianshu.io/upload_images/1440624-afc25038cc9b4436?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
6、切图的宽度。如果是一个通用的背景图，那么他的宽度应该是他在效果图中的最小宽度，也就是说这个背景可能在多处使用到了，就取最小的那个宽度就可以了。比较麻烦的是铺满全屏的时候，这就需要看看你做的效果图的宽屏宽度，所以说做效果图的时候最好是做小屏幕的效果图。
![](https://upload-images.jianshu.io/upload_images/1440624-4e71b177d16c11ed?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
有人可能会问点9的图不是可以拉伸、压缩吗，为什么需要参考最小的宽度呢？根据个人经验发现，一个大图在屏幕小的情况下点9图中拉伸的部分会变得颜色更深。
![](https://upload-images.jianshu.io/upload_images/1440624-ea8b4c753ed4c8f5?imageMogr2/auto-orient/strip%7CimageView2/2/w/640/format/webp)
作者：设计智造
链接：https://www.jianshu.com/p/5fae71afaa44
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
