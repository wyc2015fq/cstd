
# 游戏制作之路（61）使用UGUI的按钮动画 - 大坡3D软件开发 - CSDN博客

2019年02月01日 14:10:15[caimouse](https://me.csdn.net/caimouse)阅读数：180


前面学习过按钮的事件响应，当鼠标移动上面时，按钮可以更改颜色，但是在游戏里一般追求就是个性化，以及动画大量的使用。当鼠标移动到按钮上面时，要想播放一个动画怎么办呢？这时就需要使用按钮动画的功能了，这个功能可以使用在所有GUI的组件上，比如弹出式菜单、隐藏菜单等等。既然这个功能这么强大，下面就先从简单的学习起来，如下图创建一个空的3D工程：
![](https://img-blog.csdnimg.cn/2019020114014293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
创建空的工程，把场景保存为Test，然后按前面的方法来创建按钮，并调整场景视图如下：
![](https://img-blog.csdnimg.cn/20190201140210655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里可以看到右边上的属性，都是默认创建按钮的属性，并没有进行修改，特别要注意看到Transition这个选项，它还是Color Tint的方式，下面就要对这个方式进行修改：
![](https://img-blog.csdnimg.cn/20190201140238157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里选择Transition下拉列表，就可以看到四个选项，我们要选择Animation方式，也就是动画方式来显示按钮的不同的状态，确定之后就会显示如下图：
![](https://img-blog.csdnimg.cn/20190201140307710.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
当你选择Animation之后，就可以看到在下面出现Auto Generate Animation按钮，点击这个按钮之后，就可以生成按钮四个状态的动画控制器，弹出下面的窗口来保存按钮的动画控制器，这里采用默认的文件名称：
![](https://img-blog.csdnimg.cn/20190201140336164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
保存完成之后，就可以看到Assets里多了一个Button动画控制器，当你选中按钮之后，可以查看到右边的属性多了一个Animator组件，这个组件里的控制器就是A处的Button：
![](https://img-blog.csdnimg.cn/20190201140402755.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
为了修改四个状态显示的动画，需要进入动画编辑器里录制动画，从主菜单Window/Animation里进入，如下图：
![](https://img-blog.csdnimg.cn/20190201140427800.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
当弹出动画编辑器之后，记得要选中A处的按钮，才可以对它进行编辑动画：
![](https://img-blog.csdnimg.cn/20190201140457485.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
当你看到这个动画编辑器窗口时，就可以进行录制动画了：
![](https://img-blog.csdnimg.cn/20190201140530363.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在A处选择按钮之后，就可以看到Animation里的B处出现Normal状态，这个是表示按钮在正常状态的动画，点击B处之后，下拉列表就会显示按钮的四个状态的动画，可以根据自己的需要来选择修改那一个状态动画，在这里我们来选择Highlighted状态，这个状态是表示当鼠标放到按钮上面时显示的动画，因此选择C处的选项，就进入下图来编辑Highlighted状态动画：
![](https://img-blog.csdnimg.cn/20190201140600887.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
要想进入录制动画时，需要先确认A处选中按钮，然后点击B处来设置按钮大小变换工具，再点击C处小红点，就进入录制状态了。D仍然是Highlighted状态，E处是确认为从0帧开始录制，目前这个动画总共有60帧，0帧是表示开始，30帧是中间，60帧是结束。
![](https://img-blog.csdnimg.cn/20190201140709388.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在这里做一个比较简单的动画，就是按钮大小变大，然后再变小。处于录制状态，就是小红点按下，前面play按钮显示为红色状态，然后在帧位置编辑框里输入30，并回车，看到右边的红线就移动中间位置，这时候在Scene里拖动按钮的大小，就生成如下显示：
![](https://img-blog.csdnimg.cn/20190201140739625.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
当在A处把按钮变大之后，就可以看到B处增加一些数据，在C处就增加了关键的点，有了这两处数据，unity就自己去计算中间的动画了，也就是进行插值运算。接着继续修改：
![](https://img-blog.csdnimg.cn/20190201140811782.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
接着在A处把帧位置移动60，然后再把B处的按钮拖小一些，这样就可以让按钮从小到大，再从大到小地变化，这个变化过程就是动画了，当然你也想查看一下这个动画是否满意，可以点击播放按钮：
![](https://img-blog.csdnimg.cn/20190201140844164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
在A处点击播放按钮预览，就可以查看动画是否符合预期，如果不行就可以再修改。
![](https://img-blog.csdnimg.cn/2019020114092221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl,size_16,color_FFFFFF,t_70)
当动画录制完成之后，就可以在A处取消录制，然后点击B处的play模式，就可以把鼠标放到C处的按钮上，就可以看到按钮在大小地不断变化。按钮其它状态的动画，也可以按照这个过程来实现，其实还可以更换按钮的图片，或者文字的颜色等等。
[https://blog.csdn.net/caimouse/article/details/51749579](https://blog.csdn.net/caimouse/article/details/51749579)

