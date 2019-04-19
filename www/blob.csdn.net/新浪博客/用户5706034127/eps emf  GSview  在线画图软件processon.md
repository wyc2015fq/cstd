# eps emf  GSview  在线画图软件processon_用户5706034127_新浪博客
|||
			emf是微软自己的交换格式，
eps是通用的交换格式，
通常要打印eps也是需要打印机的支持，需要安装特定的post script
打印插件 
可以用openoffice 直接导出为eps格式，但是对于某些图片，
尼玛，竟然，会给我裁剪，醉求，关键是调整大小也没用。鹅鹅鹅。
所以可以旋转图片，但是对于长宽都大的图片就无能为力。
最后就是openoffice 生产的eps会有空白区域。
用latex自带的 GSview打开eps文件，在media里面选择纸张大小。或者调整orientation。
反正使你的图片全部可见，
然后file->PS to EPS，。
点击yes即可，一定要勾选 automatically calculate bounding box。
生成后的eps，就自动裁剪了，虽然用GSview打开还是会有空白区域，但是
导入的时候，是没有的空白区域的。
因为eps文件用纯文本编辑器打开，找到%%BoundingBox一行
，可以看到gsview已经将图片裁减了。

之前说了，生成openOffice生成eps会自动裁剪我的大图，
我只好先图片旋转，在生成eps，这样生成的eps就是倒置的。
可先保存为高清的png格式，在用一些在线的png转eps格式的工具转化。
其实有很多在线的工具支持，在线转eps个格式。
可以先用 [www.processon.com](http://www.processon.com/)画图，保存为高清PDF，然后在用photo
shop 另存储为eps格式，
就不用担心被截断的问题。
[www.processon.com](http://www.processon.com/)
画短线的问题，先新建一个对象，然后对象可以引出来线，删除对象后，
线段仍然存在，而且可以编辑线段的，长短，粗细，颜色和样式
![eps <wbr>emf <wbr> <wbr>GSview <wbr> <wbr>在线画图软件processon](http://s13.sinaimg.cn/mw690/006e9W8vzy7cxpNR4yU0c&690)
[www.processon.com](http://www.processon.com/)还可以插入图片，网上搜索图标，最好用英文检索词，，user
icon
![eps <wbr>emf <wbr> <wbr>GSview <wbr> <wbr>在线画图软件processon](http://s4.sinaimg.cn/mw690/006e9W8vzy7cxvi6Y5Zb3&690)
