# PADS 如何打印过孔或焊盘孔 - xqhrs232的专栏 - CSDN博客
2014年07月03日 14:04:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1545
原文地址::[http://blog.sina.com.cn/s/blog_53e4f54501014ket.html](http://blog.sina.com.cn/s/blog_53e4f54501014ket.html)
Protel打印电路板时可以显示出焊盘孔，这方便了自己DIY PCB电路板，因为焊盘中心给腐蚀掉后，钻孔时容易定位，但PADS的电路板打印就没有此功能。网上有个《PADS打印过孔.PDF》文档介绍打过孔功能，但里面遗漏了个说明，就是Top板的颜色黑色也必须选择为低优先级的才行。这个问题搞了我一个番茄钟的时间。
1.首先将调色板改变优先等级，使得白色比黑色高，注意低等级黑色必须另外加入。
![PADS <wbr>如何打印过孔或焊盘孔](http://s14.sinaimg.cn/middle/53e4f545gcb98d3eb29bd&690)
2.元件焊盘的改动,如何加入lay16层，需要参考网上文档《PADS打印过孔.PDF》。
3.在Cam中，将TOP上所有黒色都选择低等级黑色，Layer16的的白色选择高优先级。
![PADS <wbr>如何打印过孔或焊盘孔](http://s8.sinaimg.cn/middle/53e4f545gcb98d5ad6687&690)
![PADS <wbr>如何打印过孔或焊盘孔](http://s11.sinaimg.cn/middle/53e4f545gcb98d5ff1b5a&690)
4.ok，PCB中可以见到白色的通孔
![PADS <wbr>如何打印过孔或焊盘孔](http://s15.sinaimg.cn/middle/53e4f545gcb98db979e1e&690)
