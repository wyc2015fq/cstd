# PADS不更新库文件而在PCB中修改封装的方法 - xqhrs232的专栏 - CSDN博客
2013年08月14日 22:47:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1766
原文地址::[http://lvliang1023.blog.163.com/blog/static/680744372011332505983/](http://lvliang1023.blog.163.com/blog/static/680744372011332505983/)
相关网帖
1、**PADS Layout如何更新元件库中修改过的封装到PCB----[http://www.docin.com/p-242219170.html](http://www.docin.com/p-242219170.html)**
PADS2005更新库很简单----删除原来的库，把新的库文件放到原来的路径下，再加载进来即可。
这里要说的是在PCB中现场修改某个元件的封装（比如某个PAD的大小有特殊大小要求之类的） 而不希望修改库里原来的封装的方法。
1、选中你要改封装的component 2、右键---EDIT DECAL--即进入这个元件的封装修改界面。你可以修改了。
3、修改完成后 你会发现 保存 的按钮是灰色的---即不可用。不要着急，直接点击右上角的关闭窗口按钮，对出编辑窗口。
4、这时候 你会发现一个APPLY DECAL CHANGES 的对话框弹出来：Do you want to apply the changes to all components with decal type ************* or just the selected component ? 意思是 你做的修改是应用于所有这种封装的PCB 元件还是仅仅只应用于 选中 的元件？ 当然是选择 SELECTED了，这样就可以现场在PCB中修改了某个元件的封装而不必更新库了。keep
 attributes也要勾上啊。
    还需要特别强调的一点是：这个功能改动元件封装是有次数限制的，第二次的时候 selected按钮就不可以点击了。显然PADS认为你改动的次数和权限已经到此为止了，不能再让你随意更改了，以免出错！ 实际上你若真要改动很多地方，那最好去库里改了。这样能够最大限度保证设计的正确性。 所以 你一定要一次就改好，一定要严谨认真啊！
