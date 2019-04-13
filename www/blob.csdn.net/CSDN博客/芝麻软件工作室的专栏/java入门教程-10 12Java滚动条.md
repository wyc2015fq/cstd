
# java入门教程-10.12Java滚动条 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:51:55[seven-soft](https://me.csdn.net/softn)阅读数：375


滚动条(JScrollBar)也称为滑块，用来表示一个相对值，该值代表指定范围内的一个整数。例如，用Word编辑文档时，编辑窗右边的滑块对应当前编辑位置在整个文档中的相对位置，可以通过移动选择新的编辑位置。在Swing中，用JScrollBar类实现和管理可调界面。JScrollBar类常用的构造方法是：
JScrollBar(int dir,int init,int width,int low,int high)
其中，dir表示滚动条的方向。JScrollBar类定义了两个常量，JScrollBar.VERTICAL表示垂直滚动条；JScrollBar.HORIZONTAL表示水平滚动条。init表示滚动条的初始值，该值确定滚动条滑块开始时的位置；width是滚动条滑块的宽度；最后两个参数指定滚动的下界和上界。注意滑块的宽度可能影响滚动条可得到的实际的最大值。例如，滚动条的范围是0至255，滑块的宽度是10，并利用滑块的左端或顶端来确定它的实际位置。那么滚动条可以达到的最大值是指定最大值减去滑块的宽度。所以滚动条的值不会超过245。
JScrollBar类其他常用方法是：setUnitIncrement()：设置增量，即单位像素的增值；
getUnitIncrement()：获取增量；
setBlockIncrement()：设置滑块增量，即滑块的幅度；
getBlockIncrement()：获取滑块增量；
setMaxinum()：设置最大值；
getMaxinum()：获取最大值；
setMininum()：设置最小值；
getMininum()：获取最小值；
setValue()：设置新值；
getValue()：获取当前值。
JScrollBar类对象的事件类型是AdjustmentEvent；类要实现的接口是AdjustmentListener,接口方法是adjustmentValueChanged();注册监视器的方法是addAdjustmentListener();获取事件源对象的方法是getAdjustable().
【例11-17】应用程序将滚动条作为值的选择。容器有一个开/关滚动条的按钮，一个文本框和一个滚动条，当滚动条处于打开状态时，移动滚动条上的滑块，滑块的对应值显示在文本框中。如果滚动条处于关闭状态 ，则移动滚动条上的滑块，滑块的对应值在文本框中不显示（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-17.txt)）。
MyScrollBar类定义的方法getPreferedSize()也是Component类中定义的方法，界面组件通过覆盖定义该方法确定界面组件的大小。当布局字处理器在安排组件布局时，就会调用该方法来确定组件的大小。这个方法返回一个Dimension类型的对象，Dimension对象含两个整数，分别为组件的宽和高。在上述程序中，为滚动条指派的区域的宽是125像素，高是20个像素。任何组件都可用上述方法来指定大小。

