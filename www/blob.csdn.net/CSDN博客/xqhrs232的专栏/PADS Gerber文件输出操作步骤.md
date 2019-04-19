# PADS Gerber文件输出操作步骤 - xqhrs232的专栏 - CSDN博客
2014年02月22日 21:14:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1776
原文地址::[http://blog.sina.com.cn/s/blog_65fe01290100q34h.html](http://blog.sina.com.cn/s/blog_65fe01290100q34h.html)
相关文章
1、PADS导出Gerber文件----[http://wenku.baidu.com/link?url=i2lrEM9sbk9pafwaIIjq8XQKeqDEM3doQAOmhIUwXXXCu87FhXJtk6HlU_zpVcUZu0LyYktdefNZteoiy2mx_GC9JAdkBoZIih_9-XPpk1e](http://wenku.baidu.com/link?url=i2lrEM9sbk9pafwaIIjq8XQKeqDEM3doQAOmhIUwXXXCu87FhXJtk6HlU_zpVcUZu0LyYktdefNZteoiy2mx_GC9JAdkBoZIih_9-XPpk1e)
2、PADS如何导出GERBER文件以及DXF文件----[http://javimat.blog.163.com/blog/static/17191432620109132372958/](http://javimat.blog.163.com/blog/static/17191432620109132372958/)
- 
启动PowerPCB,操作如图
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image31.gif)
- 
File->CAM,如图示
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image33.gif)
- 
按下ADD，如图
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image32.gif)
Document Name :输入 'TOP'
Document Type: 选择 'Routing' , 选择后会出现如下图窗口
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image34.gif)
Layer: 这个选项决定要输出的图层,我们选择 'Primary Component Side' , 然后按下 [OK]
回到 {Add Document }视窗选择 Layer ![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image35.gif)
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image36.gif)
选择 [Board Outline] , 然后按下 [OK]
你将这个选项圈选起来,可将电路板外形一同输出至档案中
经过这些设定,上层线路的输出资料就己经设定完成了,回到 {Add Document} 视窗 , 按下 [ OK ] , 回到 { Define Cam Documents } 视窗 , 在这个视窗的 [ Cam Documents ] 栏位中, 应该会出现我们之前设定的 'TOP'
我们仿照之前的动作,将 'BOT' 建立起来, 在所有的过程中, 只有在
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image37.gif)
{ Layer Association } 中,你要选择 [ Secondary Component Side ] , [Board Outline] 不要圈选 , 其它的部分都是一样的动作.
**产生钻孔档**
按下 [ADD] 按钮 , 出现 { Add Document } 视窗 , 在 [ Document Type ] 下拉式选单中,选择 ( NC Drill ) , 接下 [OK] 回到 { Define CAM Documents }
全部设定好之后,应该在 { Define CAM Documents }会出现以下设定结果
![PADS <wbr>Gerber文件输出操作步骤](http://www.maihui.net/book/image/pads/Image38.gif)
在 [CAM] 选项中,你可以设定产生的档案要存在哪一个目录底下
当我们设定好所有的资料后,可以按下 [RUN] 让程式真正的产生档案到磁碟机中,在这个范例中,会产生
ART01.PHO
ART01.REP
ART04.PHO
ART04.REP
DRL01.DRL
DRL01.REP
DRL01.LST
这些档案就是雕刻机所需要的底片档,镜头档及钻孔档.
