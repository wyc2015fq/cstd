# AD 软件使用心得记录 - xqhrs232的专栏 - CSDN博客
2015年07月27日 16:00:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3764
原文地址::[http://blog.sina.com.cn/s/blog_6150d2620100mw0p.html](http://blog.sina.com.cn/s/blog_6150d2620100mw0p.html)
相关文章
1、AD详细教程----[http://wenku.baidu.com/link?url=69hvmy6drOZdhzeu0kOPxVg8avU3ARFah1viJ303481QEU7-vCRxrhVQ7jwXgFynSLPzBPkELaz6Kx67k9FZPkOJhWmuYAowWYP_9D0-4om](http://wenku.baidu.com/link?url=69hvmy6drOZdhzeu0kOPxVg8avU3ARFah1viJ303481QEU7-vCRxrhVQ7jwXgFynSLPzBPkELaz6Kx67k9FZPkOJhWmuYAowWYP_9D0-4om)
2、Altium_Designer AD10一步一步教程最完整----[http://www.doc88.com/p-396245762825.html](http://www.doc88.com/p-396245762825.html)
3、ad10详细使用教程----[http://www.doc88.com/p-7714215228244.html](http://www.doc88.com/p-7714215228244.html)
4、AD教程----[http://wenku.baidu.com/link?url=hqi4eEjDM8JYV0opQIBWVZUfa_J7XuwlgSvtkf_ggBLu8UaRCKEXrx3vRD4ZWOGxljyyFIuzuq-5aLUHCZu7L7RUtKdFKT8VHhM5lLiU-zS](http://wenku.baidu.com/link?url=hqi4eEjDM8JYV0opQIBWVZUfa_J7XuwlgSvtkf_ggBLu8UaRCKEXrx3vRD4ZWOGxljyyFIuzuq-5aLUHCZu7L7RUtKdFKT8VHhM5lLiU-zS)
1、对于Altium
 Designer 的帮助的使用案例。
在画原理图库的元器件时，发现放置的框不透明了，所以去找解决方法。首先选中框，按F1，跳出帮助框，里面提示你一些信息，功过提示可以进入Reference，再根据里面的提示可以找到到哪里去设置。
总结：帮助文件很强大！可以随时利用帮助文件
2、集成库最好在所有项目或工作空间的外面，单独作为一个项目文件，在一个工作空间里，把这个文件加载进去就好，而不要采用把这个集成库分别放到每个工作空间中（否则，会导致在一个工作空间中添加新的元器件时，其他的集成库不能得到更新，相当于它们互相是独立了，而采用加载的方法，就会全部都得到更新）.当然，如果就是想要在单独的一个工作空间中改变，而不想改变其他的，就把它进去。
3、虽然利用软件提供的模板原理图来作为原理图可以解决右下角信息无法正常显示的问题，但是貌似会导致软件的运行不是很稳定，会提示内存出错！
4、在Altium
 Designer中，从原理图编译时，在Messages有很多的Off grid NetParameter是怎么回事?
原因：因为原理图中的元件引脚尺寸和设置的栅格尺寸不对应，导致系统无法识别而报错。
解决方法：在该编辑框中设置与“Snap”编辑框中相同的值，使显示的栅格与捕捉栅格一致。把数值设置为2.54MM,这样就解决了上面的问题了。实际设计时，为能准确快速地捕获电气节点，电气栅格应该设置得比当前捕获栅格稍微小点，否则电气对象的定位会变得相当的困难。
5、PCB打印时，想不打印出丝印层的设置方法：
点击Page Setup，再在出来的框中点击advanced，出来PCB
 Printout Properies，双击Top Overlay，在出来的Free
 Primitive，Component Primitives，Others下的Hide全部点上（全部为OFF）。
注意：在绘制PCB库文件时，在利用Find
 Similar Object功能进行焊盘参数的修改时，一定要注意把右下角的Whole Library的钩给去掉，否则会把整个库的焊盘参数给去掉了！
6、新建PCB文件时，最好不要取自模板，否则会带来打印不好打的后果。
7、在进行个人的pcb板的制作时，内孔孔径设置为20mil，外孔径设置为70mil（外径可以设置的更大一点）是比较合理的。
、打印pcb出来发现和原图刚好相反，问题在于在打印设置“advanced”上勾选了“mirror”，这个勾选上的话是用来打印顶层的（即把铜敷在顶层，要反一下，才能转印上去是正的）。而打印底层时，不用勾选“mirror”。
注意：以后打印出来以后，一定要先仔细观察，看看有没有问题，再做下一步的行动！
8、在制作自己的原理图库时，要先画个框（先不管它的大小），再放置PIN。否则会出现问题：1、在Preference中把框设置为透明，用smart
 PDF时，看不到内部的东西。2、不设为透明时，就会把PIN遮住。
所以，经过实践，解决的方法就是先在Preference把框的属性设置为“透明”，然后是在制作库元件时，“先画框，再放PIN”
