
# Android手机玩8086汇编 - 迂者-贺利坚的专栏 - CSDN博客

2017年04月20日 07:46:55[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：5717


【前言】
什么前言？要不想听老贺讲故事，直接看正文。
教C/C++课时，曾带领弟子们用手机编程序。程序稍大就不好玩了，但遇个小程序，那还是很爽的。尤其是第一次课，就在课堂，还没有在台式机上干活时。
这学期教汇编语言，也想过这事，没有落实。
其实这事应该简单，如果有Android中的DOS模拟器，那就一切妥了。
今天，和DosBox Turbo偶遇了！
![这里写图片描述](https://img-blog.csdn.net/20170420073842938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420073842938?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这是个DOS游戏爱好者在Android中玩经典DOS游戏用的模拟器。能运行DOS程序，玩8086汇编和一套东东就没有问题。
想起我给老师们推广用“海马玩”在笔记本电脑上模拟Android，以此替代同屏器，将云班课用于课堂教学的案例。呵呵，一切玩艺儿，老贺手中就能用来玩学习！
不拽了，上正文。
【正文】
搜索DosBox Turbo模拟器，组装masm相关文件，还算顺利。发福利，点后面的链接，直接下载了用吧。[百度网盘下载链接…](http://pan.baidu.com/s/1c2d5E2s)[CSDN下载频道…](http://download.csdn.net/detail/sxhelijian/9819700)
下面是解压后的文件，以及处理办法：
![这里写图片描述](https://img-blog.csdn.net/20170420075706219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420075706219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意：是将masm整个文件夹拷贝到手机存储的根目录下。
masm中的文件：
![这里写图片描述](https://img-blog.csdn.net/20170420080240643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420080240643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
老贺加个编程模板，很贴心的：在手机上输入程序，能省几个字符算几个。
接下来，在手机上打开应用。
模拟器已经将手机存储的根目录挂载为C:，可以用cd masm进入到masm文件夹，这里包含着玩8086汇编的所有文件。
下面，输入debug，进入到了万能的debug中去了！
![这里写图片描述](https://img-blog.csdn.net/20170420080616332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420080616332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
编辑程序，干脆就入乡随俗，用edit吧。输入edit ptest.asm，回车，有：
![这里写图片描述](https://img-blog.csdn.net/20170420081235281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420081235281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有没有和我一样鸡动！
编完了程序，想存盘、退出，遇到新问题了！
没有鼠标不要紧，给我键盘快捷键，照样搞定！但，但是，Alt键在哪里？上下左右方向键在哪里？
受打击中，看见了上面的摇杆。玩一玩摇杆，搞定！额的神那，神！（附：后来玩了下摇杆右面的设置，将输入改为“物理鼠标/触笔”更方便。）
再往后，汇编、连接、运行……
![这里写图片描述](https://img-blog.csdn.net/20170420081949527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420081949527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一条龙搞定！
[
](https://img-blog.csdn.net/20170420081949527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)【感慨】
这年月的学习条件！
想当年，一周就能上一次机，让师兄师姐们羡慕得那个样子。
【附录】
下面的贴子，让我明白了其中的道道，然后一帆风顺地把事情做完，分享，感谢！这些对不熟悉DOS的同学应该有用。
![这里写图片描述](https://img-blog.csdn.net/20170420082508845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420082508845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
看原贴，请点[链接…](http://bbs.dospy.com/thread-16082460-1-354-1.html)
[            ](https://img-blog.csdn.net/20170420082508845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3hoZWxpamlhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

