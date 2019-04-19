# 关于cadence 16.3 的封装库 - xqhrs232的专栏 - CSDN博客
2011年11月28日 22:14:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1081标签：[components																[工具																[library																[patch																[网络																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=patch&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=components&t=blog)
个人分类：[PCB/EDA/硬件设计与仿真](https://blog.csdn.net/xqhrs232/article/category/906926)
原文地址::[http://blog.csdn.net/baiduluckyboy/article/details/6027547](http://blog.csdn.net/baiduluckyboy/article/details/6027547)
首先说明的是封装库文件是allegro下带的，不是OrCAD下带.allegro是个画PCB的工具,而OrCAD是画sch的软件,可以生成好多种网络表.
allegro安装后自带的库文件路径是:C:/Cadence/SPB_15.5/share/pcb/pcb_lib/symbols(我的安装在C盘,其他盘类似)  
symbols下的那些文件就是库文件,其实allegro的库文件有用的就是dra和psm后缀的文件.这里需要说明的一个问题是: 
我们在学protel的时候知道protel的库文件是集成的,就是韩盘和封装是现成的,一般调用就ok,但allegro是先做韩盘,然后在做封装的时候再调用刚才做的焊盘.做焊盘的工具是allegro SPB15.5/PCB editor utilities/PAD designer  这个工具是专门做焊盘,具体怎么用看相关参考书,做好后保存在某个位置,将来做封装的时候调用,如果初学者对allegro软件不熟悉的话,就把做好的结果保存在C:/Cadence/SPB_15.5/share/pcb/pcb_lib/symbols下的某个地方,因为allegro默认是在这先找焊盘滴.
至于做封装的过程,一般按照向导做,在这里我要说的是,[http://www.pcbbbs.com/dispbbs.asp?boardID=42&ID=98354&page=2](http://www.pcbbbs.com/dispbbs.asp?boardID=42&ID=98354&page=2)
论坛上有个高手做了个小软件,直接在他的软件里改参数然后直接生成封装,再保存在C:/Cadence/SPB_15.5/share/pcb/pcb_lib/symbols这个路径下(为什么保存在这后面说原因). 在做sch的时候把刚才做好的封装名字填到相应的地方.然后生成网络表.至于大家关心的怎么预缆allegro自己带的那些封装库文件,我告诉你方法 
1.打开alllegro软件,点Place下面的第一个项manually...跳出一个Placement的框,点Advanced Settings ,在Library前打钩,然后点Placement list,把Components by refdes换成Package symbols,紧跟着下面出现一个十字,点开,就会看到好多的封装,想看那个就在前面的框里打钩,就在右下角黑黑的框里显示出来了,同时也可以放到板上.如果在sch里要用到这些封装的话,就在这把这些封装的名字记下来写在元件的封装项里去. 
2.自己画的封装呢?刚才说了把网上这个高手的软件修改一下可以自动生成好多的封装库,把他保存在C:/Cadence/SPB_15.5/share/pcb/pcb_lib/symbols这个地方.为什么要保存在这个地方呢?因为这个软件安装好后,封装默认的保存地方在C盘(自己建的目录),当然你也可以把这个默认的地方改掉,具体在allegro/setup/User Preferences 这个时候出个框 ,点左边第4个confi_patch ,右边有好几个地方需要修改,在这根据我个人的建议:新手一般不要改这个地方,按照上面提到的方法把自己做的库文件(或用高手的软件改的)放到C:/Cadence/SPB_15.5/share/pcb/pcb_lib/symbols这.因为我在这第一次改了好多,现在这个软件也不怎么好用了,我也不知道怎么改回去了.(已经解决了)
3.自己建个文件夹，装做好的焊盘和封装的文件都放在这个文件夹里，做为另一个库文件夹（系统已自带了一个），打开allegro/setup/user preferences,点击左边designer paths ,然后在右边padpath和psmpath中的value中添加刚才的文件夹，就这样，你保存的新的封装可以用到你的PCB中去了。
